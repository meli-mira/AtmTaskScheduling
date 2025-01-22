#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <cstdlib>

#include "../context.hpp"
#include "../services/SchedulerService.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class SchedulerController
{
private:
    std::shared_ptr<ISchedulerService> schedulerService;

public:
    SchedulerController(std::shared_ptr<ISchedulerService> service) : schedulerService(service) {}
    void scheduleNode(Context &ctx);
    void scheduleAllNodesFrom(Context &ctx);
    //to do, delete scheduler
};