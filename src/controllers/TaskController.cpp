#include "../../include/controllers/TaskController.hpp"
#include "../../include/models/CScheduler.hpp"
#include "../../include/services/TimetableService.hpp"

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

        /* Add task to loaded structures */
        CScheduler::getInstance()->addTaskToNode(task->getNodeId(), task);

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
        /* Add task to loaded structures */
        CScheduler::getInstance()->addResourceToTask(task_id, resource_id);

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

        auto task = CScheduler::getInstance()->searchTask(task_id);
        if (task != NULL)
        {
            // Task's node id
            auto node = CScheduler::getInstance()->searchNode(task->getNodeId());

            auto hasBeenPlanned = task->getHasBeenPlanned();

            // Task's list of resources
            auto resources = task->getResources();

            // Delete from existing loaded list of tasks from Scheduling and update timetables of node and used resources
            CScheduler::getInstance()->deleteTask(task_id);

            // Update scheduling structures if the task has been planned
            if (hasBeenPlanned)
            {
                auto timetableService = std::make_shared<TimetableService>();
                timetableService->updateTimetable(node->getTimetable());
                for (int i = 0; i < resources.size(); i++)
                    timetableService->updateTimetable(resources[i]->getTimetable());
            }
        }
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

void TaskController::updateTask(Context &ctx)
{
    auto &req = ctx.getRequest();
    auto &res = ctx.getResponse();
    try
    {
        auto json = boost::json::parse(req.body());
        vector<pair<string, string>> v = TaskSerializer::fromUpdateJson(json.as_object());
        taskService->updateTask(v);

        /* Update task to loaded structures */
        if (v.size() > 1)
            CScheduler::getInstance()->updateTask(v[0].second, v);

        res.result(http::status::ok);
        res.body() = "{\"success\":\"Task updated \"}";
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to update task.\"}";
    }
}

void TaskController::unscheduleTask(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        auto task = CScheduler::getInstance()->searchTask(ctx.getParam("task_id").c_str());
        if (task != NULL)
        {
            if (task->getHasBeenPlanned())
            {
                // Unschedule task from existing loaded list of tasks from Scheduling
                CScheduler::getInstance()->unscheduleTask(task->getNodeId(), task->getID());

                // Unschedule from DB
                taskService->unscheduleTask(task->getID());

                // Update timetable of node and used resources and task fields
                auto timetableService = std::make_shared<TimetableService>();

                timetableService->updateTimetable(CScheduler::getInstance()->searchNode(task->getNodeId())->getTimetable());
                for (int i = 0; i < task->getResources().size(); i++)
                    timetableService->updateTimetable(task->getResources()[i]->getTimetable());
            }
            res.result(http::status::ok);
            res.body() = "{\"success\":\" Task has been unscheduled. \"}";
        }
        else
        {
            res.result(http::status::not_found);
            res.body() = "{\"failure\":\" Task  has not been unscheduled. Task doesn't exists. \"}";
        }

        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to unschedule task.\"}";
    }
}