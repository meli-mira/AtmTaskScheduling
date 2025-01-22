#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <cstdlib>

#include "../context.hpp"
#include "../services/NotificationService.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class NotificationController
{
private:
    std::shared_ptr<INotificationService> notificationService;

public:
    NotificationController(std::shared_ptr<INotificationService> service) : notificationService(service) {}
    void getNotificationById(Context &ctx);
    void getNotificationsByNodeId(Context &ctx);
    void getNotificationsByTaskId(Context &ctx);
    void deleteNotification(Context &ctx);
};