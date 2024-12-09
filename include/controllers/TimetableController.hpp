#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <cstdlib>

#include "../context.hpp"
#include "../services/TimetableService.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class TimetableController
{
private:
    std::shared_ptr<ITimetableService> timetableService;

public:
    TimetableController(std::shared_ptr<ITimetableService> service) : timetableService(service) {}
    void getTimetableById(Context &ctx);
    void getTimetableByNodeId(Context &ctx);
    void getTimetableByResourceId(Context &ctx);
    //void createTimetable(Context &ctx);
    void updateTimetable(Context &ctx);
};