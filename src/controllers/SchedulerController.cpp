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
            CScheduler::getInstance()->scheduleTasksForNode(node);
            CScheduler::getInstance()->printScheduledTasks(node);

            /* Update scheduling structures for scheduled tasks and resources */
            CScheduler::getInstance()->updateSchedulingStructures(node);
        }

        res.result(http::status::created);
        res.body() = "{\"success\":\"Tasks of the selected node has been scheduled. \"}";
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
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