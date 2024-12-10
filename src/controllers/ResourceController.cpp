#include "../../include/controllers/ResourceController.hpp"
#include "../../include/services/TimetableService.hpp"
#include "../../include/models/CScheduler.h"

void ResourceController::getResources(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = resourceService->getAllResources();

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
        res.body() = "{\"error\": \"Failed to load resources.\"}";
    }
}

void ResourceController::getResourceById(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = resourceService->getResourceById(ctx.getParam("id").c_str());

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
        res.body() = "{\"error\": \"Failed to load resource with id.\"}";
    }
}

void ResourceController::getResourcesByTaskId(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = resourceService->getResourcesByTaskId(ctx.getParam("task_id").c_str());

        res.result(http::status::ok);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = json_response.dump();
    }
    catch (const exception &e)
    {
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to load resources for task.\"}";
    }
}

void ResourceController::createResource(Context &ctx)
{
    auto &req = ctx.getRequest();
    auto &res = ctx.getResponse();
    try
    {
        auto json = boost::json::parse(req.body());
        auto resource = ResourceSerializer::fromJson(json.as_object());

        // Add the timetable created first
        auto timetableService = std::make_shared<TimetableService>();
        timetableService->addTimetable(resource->getTimetable());

        resourceService->addResource(resource);
        CScheduler::getInstance()->addResource(resource);

        res.result(http::status::created);
        res.body() = "{\"success\":\"Resource created. Timetable added. \"}";
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to create resource.\"}";
    }
}

void ResourceController::deleteResource(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        string resource_id = ctx.getParam("id").c_str();
        resourceService->deleteResourceById(resource_id);

        // TODO DELETE RESOURCE
        res.result(http::status::no_content);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to delete resource.\"}";
    }
}