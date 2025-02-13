#pragma once
#include <string>
#include <unordered_map>
#include "CListener.hpp"
namespace http_server
{
    class CHttpServer
    {
    public:
        void init(const char *address, int port, int threads);
    };
}