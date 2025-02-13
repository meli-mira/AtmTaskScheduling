#include "../../include/controllers/SchedulerController.hpp"

void SchedulerController::scheduleNode(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        auto nodeService = std::make_shared<NodeService>();
        auto node = CScheduler::getInstance()->searchNode(ctx.getParam("node_id").c_str());
        if (node != NULL)
        {
            /* Schedule tasks for node with given node_id */
            CScheduler::getInstance()->scheduleTasksForNode(node);
            CScheduler::getInstance()->printScheduledTasks(node);

            /* Update scheduling structures for scheduled tasks and resources */
            CScheduler::getInstance()->updateSchedulingStructures(node);
        }

        res.result(http::status::created);
        res.body() = "{\"success\":\"Tasks of the selected node has been scheduled. \"}";
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");

        CLogger::log("SchedulerController", "Scheduling for node with id " + node->getID() + " has been done");
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to schedule tasks of the selected node.\"}";
    }
}

void SchedulerController::scheduleAllNodesFrom(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        auto nodeService = std::make_shared<NodeService>();
        auto node = CScheduler::getInstance()->searchNode(ctx.getParam("node_id").c_str());
        if (node != NULL)
        {
            CScheduler::getInstance()->scheduleAll(node);
            CScheduler::getInstance()->printScheduledTasks(node);
        }

        res.result(http::status::created);
        res.body() = "{\"success\":\"Tasks of the tree starting from the selected node has been scheduled. \"}";
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to schedule tasks of the tree starting from the selected node.\"}";
    }
}

void SchedulerController::unscheduleNode(Context &ctx)
{
    // todo
}

void SchedulerController::unscheduleTaskOfNode(Context &ctx)
{
    // todo
}

void SchedulerController::getSchedulingReportBetweenFromNode(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        string json_response = CScheduler::getInstance()->getTasksFromNodeBetween(ctx.getParam("node_id").c_str(), CUtils::parseDateTime(ctx.getParam("startDate").c_str(), "%Y-%m-%d"), CUtils::parseDateTime(ctx.getParam("endDate").c_str(), "%Y-%m-%d"));

        res.result(http::status::ok);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = json_response;
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to load scheduling report of node.\"}";
    }
}

void SchedulerController::getSchedulingReportBetweenForNode(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        string json_response = CScheduler::getInstance()->getTasksForNodeBetween(ctx.getParam("node_id").c_str(), CUtils::parseDateTime(ctx.getParam("startDate").c_str(), "%Y-%m-%d"), CUtils::parseDateTime(ctx.getParam("endDate").c_str(), "%Y-%m-%d"));

        res.result(http::status::ok);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = json_response;
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to load scheduling report of node.\"}";
    }
}

void SchedulerController::getSchedulingReportBetweenForResource(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        std::cerr << ctx.getParam("resource_id").c_str();
        string json_response = CScheduler::getInstance()->getTasksThatUseResourceBetween(ctx.getParam("resource_id").c_str(), CUtils::parseDateTime(ctx.getParam("startDate").c_str(), "%Y-%m-%d"), CUtils::parseDateTime(ctx.getParam("endDate").c_str(), "%Y-%m-%d"));

        res.result(http::status::ok);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = json_response;
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to load scheduling report of node.\"}";
    }
}