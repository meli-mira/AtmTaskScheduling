#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <cstdlib>

#include "../context.hpp"
#include "../services/NodeService.hpp"
#include "../../include/serializers/NodeSerializer.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class NodeController
{
private:
    std::shared_ptr<INodeService> nodeService;

public:
    NodeController(std::shared_ptr<INodeService> service) : nodeService(service) {}
    void getNodes(Context& ctx);
    void getNodeById(Context& ctx);
    void getNodesByParentId(Context& ctx);
    void createNode(Context& ctx);
    void deleteNode(Context& ctx);
    //todo update
};