#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <cstdlib>

#include "../context.hpp"
#include "../services/ResourceService.hpp"
#include "../../include/serializers/ResourceSerializer.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class ResourceController
{
private:
    std::shared_ptr<IResourceService> resourceService;

public:
    ResourceController(std::shared_ptr<IResourceService> service) : resourceService(service) {}
    void getResources(Context& ctx);
    void getResourceById(Context& ctx);
    void getResourcesByTaskId(Context& ctx);
    void createResource(Context& ctx);
    void deleteResource(Context& ctx);
    //todo update
};