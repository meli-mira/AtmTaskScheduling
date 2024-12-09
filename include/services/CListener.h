#pragma once
#include "CSession.h"

namespace http_server
{
    // Accepts incoming connections and launches the sessions
    class Listener : public std::enable_shared_from_this<Listener>
    {
        net::io_context &ioc_;
        tcp::acceptor acceptor_;

    public:
        Listener(net::io_context &ioc, tcp::endpoint endpoint) : ioc_(ioc), acceptor_(net::make_strand(ioc))
        {
            beast::error_code ec;

            // Open the acceptor
            acceptor_.open(endpoint.protocol(), ec);
            if (ec)
            {
                throw http_server_exception(ec, "open");
                return;
            }

            // Allow address reuse
            acceptor_.set_option(net::socket_base::reuse_address(true), ec);
            if (ec)
            {
                throw http_server_exception(ec, "set_option");
                return;
            }

            // Bind to the server address
            acceptor_.bind(endpoint, ec);
            if (ec)
            {
                throw http_server_exception(ec, "bind");
                return;
            }

            // Start listening for connections
            acceptor_.listen(net::socket_base::max_listen_connections, ec);
            if (ec)
            {
                 throw http_server_exception(ec, "listen");
                return;
            }
        }

        // Start accepting incoming connections
        void run()
        {
            do_accept();
        }

    private:
        void do_accept()
        {
            // The new connection gets its own strand
            acceptor_.async_accept(net::make_strand(ioc_), beast::bind_front_handler(&Listener::on_accept, shared_from_this()));
        }

        void on_accept(beast::error_code ec, tcp::socket socket)
        {
            if (ec)
            {
                 throw http_server_exception(ec, "accept");
                return; // To avoid infinite loop
            }
            else
            {
                // Create the session and run it
                std::make_shared<Session>(std::move(socket))->run();
            }

            // Accept another connection
            do_accept();
        }
    };
}