#include "../../include/services/CHttpServer.hpp"

namespace http_server
{
    void CHttpServer::init(const char *address_, int port_, int threads_)
    {
        try
        {
            auto const address = net::ip::make_address(address_);
            auto const port = static_cast<unsigned short>(port_);
            auto const threads = std::max<int>(1, threads_);

            // The io_context is required for all I/O
            net::io_context ioc{threads};

            // Create and launch a listening port
            std::make_shared<Listener>(ioc, tcp::endpoint{address, port})->run();

            // Run the I/O service on the requested number of threads
            std::vector<std::thread> v;
            v.reserve(threads - 1);
            for (auto i = threads - 1; i > 0; --i)
                v.emplace_back([&ioc]
                               { ioc.run(); });
            ioc.run();
        }
        catch (http_server_exception &e)
        {
            cout << e.what();
        }
    }
}