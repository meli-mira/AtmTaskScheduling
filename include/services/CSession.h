#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "Sql.h"

namespace beast = boost::beast;   // from <boost/beast.hpp>
namespace http = beast::http;     // from <boost/beast/http.hpp>
namespace net = boost::asio;      // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp; // from <boost/asio/ip/tcp.hpp>

namespace http_server
{
    class http_server_exception
    {
    public:
        explicit http_server_exception(beast::error_code ec, string _msg) : m_msg{_msg + ": " + ec.message()} {}
        std::string what() const noexcept
        {
            return m_msg;
        }

    private:
        std::string m_msg;
    };

    template <class Body, class Allocator>
    http::message_generator handle_request(http::request<Body, http::basic_fields<Allocator>> &&req)
    {
        // Respond to GET request !!!!
        if (req.method() == http::verb::get)
        {
            string request_target = req.target();
            string sql_query;

            if (req.target() == "/resources")
                sql_query = "SELECT * FROM resources";
            else if (req.target() == "/nodes")
                sql_query = "SELECT * FROM nodes";
            else
            {
                if (request_target.find("tasks") != -1)
                {
                    string node_id = request_target.substr(request_target.find("=") + 1, request_target.size() - request_target.find("="));
                    sql_query = "SELECT * FROM tasks WHERE node_id = '" + node_id + "'";
                }
                else if (request_target.find("scheduling") != -1)
                {
                    string node_id = request_target.substr(request_target.find("=") + 1, request_target.size() - request_target.find("="));
                    sql_query = "SELECT nodes.name, tasks.name, scheduling.startDate, scheduling.endDate FROM scheduling JOIN nodes on scheduling.node_id = nodes.node_id JOIN tasks on scheduling.task_id = tasks.task_id WHERE scheduling.node_id = '" + node_id + "'";
                }
                 else if (request_target.find("schedule") != -1)
                {
                    string node_id = request_target.substr(request_target.find("=") + 1, request_target.size() - request_target.find("="));
                   
                }
            }

            connection conn = sql::database_utils::init();
            json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
            sql::database_utils::db_close(conn);

            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.keep_alive(req.keep_alive());
            res.body() = json_response.dump();
            res.prepare_payload();

            return res;
        }
        else if (req.method() == http::verb::post && req.target() == "/api/data")
        {

            auto json_request = nlohmann::json::parse(req.body());
            std::string response_message = "Received: " + json_request.dump();
            nlohmann::json json_response = {{"message", response_message}};

            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.keep_alive(req.keep_alive());
            res.body() = json_response.dump();
            res.prepare_payload();
            return res;
        }
        else if (req.method() == http::verb::put && req.target() == "/api/data")
        {

            auto json_request = nlohmann::json::parse(req.body());
            std::string response_message = "Update: " + json_request.dump();
            nlohmann::json json_response = {{"message", response_message}};

            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.keep_alive(req.keep_alive());
            res.body() = json_response.dump();
            res.prepare_payload();
            return res;
        }
        else if (req.method() == http::verb::delete_ && req.target() == "/api/data")
        {

            nlohmann::json json_response = {{"message", "Resource deleted"}};

            http::response<http::string_body> res{http::status::ok, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.keep_alive(req.keep_alive());
            res.body() = json_response.dump();
            res.prepare_payload();
            return res;
        }

        return http::response<http::string_body>{http::status::bad_request, req.version()};
    }

    // Handles an HTTP server connection
    class Session : public std::enable_shared_from_this<Session>
    {
        beast::tcp_stream stream_;
        beast::flat_buffer buffer_;
        http::request<http::string_body> req_;

    public:
        // Take ownership of the stream
        Session(tcp::socket &&socket) : stream_(std::move(socket)) {}

        // Start the asynchronous operation
        void run()
        {
            // We need to be executing within a strand to perform async operations
            // on the I/O objects in this session. Although not strictly necessary
            // for single-threaded contexts, this example code is written to be
            // thread-safe by default.
            net::dispatch(stream_.get_executor(), beast::bind_front_handler(&Session::do_read, shared_from_this()));
        }

        void do_read()
        {
            // Make the request empty before reading,
            // otherwise the operation behavior is undefined.
            req_ = {};

            // Set the timeout.
            stream_.expires_after(std::chrono::seconds(30));

            // Read a request
            http::async_read(stream_, buffer_, req_, beast::bind_front_handler(&Session::on_read, shared_from_this()));
        }

        void on_read(beast::error_code ec, std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);

            // This means they closed the connection
            if (ec == http::error::end_of_stream)
                return do_close();

            if (ec)
                throw http_server_exception(ec, "read");

            // Send the response
            send_response(handle_request(std::move(req_)));
        }

        void send_response(http::message_generator &&msg)
        {
            bool keep_alive = msg.keep_alive();

            // Write the response
            beast::async_write(stream_, std::move(msg), beast::bind_front_handler(&Session::on_write, shared_from_this(), keep_alive));
        }

        void
        on_write(
            bool keep_alive,
            beast::error_code ec,
            std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);

            if (ec)
                throw http_server_exception(ec, "write");

            if (!keep_alive)
            {
                // This means we should close the connection, usually because
                // the response indicated the "Connection: close" semantic.
                return do_close();
            }

            // Read another request
            do_read();
        }

        void do_close()
        {
            // Send a TCP shutdown
            beast::error_code ec;
            stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

            // At this point the connection is closed gracefully
        }
    };
}