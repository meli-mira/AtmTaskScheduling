#include "../../include/controllers/TaskController.h"

void TaskController::getTasks(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = taskService->getAllTasks();

        res.result(http::status::ok);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = json_response.dump();
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to load tasks.\"}";
    }
}

void TaskController::getTaskById(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = taskService->getTaskById(ctx.getParam("id").c_str());

        res.result(http::status::ok);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = json_response.dump();
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to load task.\"}";
    }
}

void TaskController::getTasksByNodeId(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = taskService->getTasksByNodeId(ctx.getParam("node_id").c_str());

        res.result(http::status::ok);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = json_response.dump();
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to load tasks.\"}";
    }
}

void TaskController::getResourcesOfTask(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = taskService->getResourcesOfTask(ctx.getParam("task_id").c_str());

        res.result(http::status::ok);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = json_response.dump();
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to load resources of task.\"}";
    }
}

void TaskController::createTask(Context &ctx)
{
    auto &req = ctx.getRequest();
    auto &res = ctx.getResponse();
    try
    {
        auto json = boost::json::parse(req.body());
        auto task = TaskSerializer::fromJson(json.as_object());

        taskService->addTask(task);

        res.result(http::status::created);
        res.body() = "{\"success\":\"Task created. Scheduler added. \"}";
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to create task.\"}";
    }
}

void TaskController::addResourceToTask(Context &ctx)
{
    auto &req = ctx.getRequest();
    auto &res = ctx.getResponse();
    try
    {
        auto json = boost::json::parse(req.body());
        string task_id = json.at("task_id").as_string().c_str();
        string resource_id = json.at("resource_id").as_string().c_str();

        taskService->addResourceToTask(task_id, resource_id);

        res.result(http::status::created);
        res.body() = "{\"success\":\"Resource added to task. \"}";
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to add resource to task.\"}";
    }
}

void TaskController::deleteTask(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        string task_id = ctx.getParam("id").c_str();
        taskService->deleteTaskById(task_id);

        res.result(http::status::no_content);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to delete task.\"}";
    }
}