#include "../../include/controllers/NodeController.hpp"
#include "../../include/services/TimetableService.hpp"
#include "../../include/models/CScheduler.h"

void NodeController::getNodes(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = nodeService->getAllNodes();

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
        res.body() = "{\"error\": \"Failed to load nodes.\"}";
    }
}

void NodeController::getNodeById(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = nodeService->getNodeById(ctx.getParam("id").c_str());

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
        res.body() = "{\"error\": \"Failed to load node with id.\"}";
    }
}

void NodeController::getNodesByParentId(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = nodeService->getAllNodesOfParent(ctx.getParam("parent_id").c_str());

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
        res.body() = "{\"error\": \"Failed to load nodes for parent id.\"}";
    }
}

void NodeController::createNode(Context &ctx)
{
    auto &req = ctx.getRequest();
    auto &res = ctx.getResponse();
    try
    {
        auto json = boost::json::parse(req.body());
        auto node = NodeSerializer::fromJson(json.as_object());

        // Add the timetable created first
        auto timetableService = std::make_shared<TimetableService>();
        timetableService->addTimetable(node->getTimetable());

        nodeService->addNode(node);
        CScheduler::getInstance()->addNode(node);

        res.result(http::status::created);
        res.body() = "{\"success\":\"Node created. Timetable added. \"}";
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to create node.\"}";
    }
}

void NodeController::deleteNode(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        string node_id = ctx.getParam("id").c_str();
        nodeService->deleteNodeById(node_id);

        // Delete from existing loaded list of nodes from Scheduling
        CScheduler::getInstance()->deleteNode(node_id);

        res.result(http::status::no_content);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to delete node.\"}";
    }
}