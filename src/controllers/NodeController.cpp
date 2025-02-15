#include "../../include/controllers/NodeController.hpp"
#include "../../include/services/TimetableService.hpp"
#include "../../include/models/CScheduler.hpp"
#include "../../include/services/TaskService.hpp"

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

    CNode *node = NULL;
    auto timetableService = std::make_shared<TimetableService>();

    try
    {
        auto json = boost::json::parse(req.body());
        node = NodeSerializer::fromJson(json.as_object());

        // Add the timetable created first
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
        if (node != NULL)
            timetableService->deleteTimetableById(to_string(node->getTimetableId()));
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

        // To do, reset resources used, if any

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

void NodeController::unscheduleNode(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        string node_id = ctx.getParam("node_id").c_str();
        time_t startDate = CUtils::parseDateTime(ctx.getParam("startDate").c_str(), "%Y-%m-%d");
        time_t endDate = CUtils::parseDateTime(ctx.getParam("endDate").c_str(), "%Y-%m-%d");

        auto node = CScheduler::getInstance()->searchNode(node_id);
        if (node != NULL)
        {
            auto timetableService = std::make_shared<TimetableService>();
            auto taskService = std::make_shared<TaskService>();

            for (int i = 0; i < node->getTasks().size(); i++)
            {
                if (node->getTasks()[i]->getHasBeenPlanned() && !(CUtils::compareDates_(node->getTasks()[i]->getEndDate(), startDate) == true || CUtils::compareDates_(endDate, node->getTasks()[i]->getStartDate()) == true))
                {
                    // Unschedule task and task's used resources from loaded structures
                    node->getTasks()[i]->unscheduleTask(node->getTimetable());

                    // Unschedule task fields from BD
                    taskService->unscheduleTask(node->getTasks()[i]->getID());

                    // Unschedule task's resources timetables from BD
                    for (int j = 0; j < node->getTasks()[i]->getResources().size(); j++)
                        timetableService->updateTimetable(node->getTasks()[i]->getResources()[j]->getTimetable());
                }
            }
            // Update node timetable
            timetableService->updateTimetable(node->getTimetable());

            res.result(http::status::ok);
            res.body() = "{\"success\":\" Node's timetable has been unscheduled. \"}";
        }
        else
        {
            res.result(http::status::not_found);
            res.body() = "{\"failure\":\" Node's timetable has not been unscheduled. Node doesn't exists. \"}";
        }

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