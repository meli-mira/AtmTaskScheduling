#pragma once
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <cstdlib>

#include "../context.hpp"
#include "../services/TaskService.h"
#include "../../include/serializers/TaskSerializer.hpp"

namespace beast = boost::beast; // from <boost/beast.hpp>
namespace http = beast::http;   // from <boost/beast/http.hpp>
namespace net = boost::asio;    // from <boost/asio.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class TaskController
{
private:
    std::shared_ptr<ITaskService> taskService;

public:
    TaskController(std::shared_ptr<ITaskService> service) : taskService(service) {}
    void getTasks(Context &ctx);
    void getTaskById(Context &ctx);
    void getTasksByNodeId(Context &ctx);
    void getResourcesOfTask(Context &ctx);
    void createTask(Context &ctx);
    void addResourceToTask(Context &ctx);
    void deleteTask(Context &ctx);
};