#include "../../include/controllers/NotificationController.hpp"

void NotificationController::getNotificationById(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = notificationService->getNotificationById(ctx.getParam("id").c_str());

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
        res.body() = "{\"error\": \"Failed to load notification with id.\"}";
    }
}

void NotificationController::getNotificationsByNodeId(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = notificationService->getNotificationsByNodeId(ctx.getParam("node_id").c_str());

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
        res.body() = "{\"error\": \"Failed to load notifications for node.\"}";
    }
}

void NotificationController::getNotificationsByTaskId(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = notificationService->getNotificationsByTaskId(ctx.getParam("task_id").c_str());

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
        res.body() = "{\"error\": \"Failed to load notifications for task.\"}";
    }
}

void NotificationController::deleteNotification(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        string notification_id = ctx.getParam("id").c_str();
        notificationService->deleteNotificationById(notification_id);

        //delete notification from task
        res.result(http::status::no_content);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    }
    catch (const exception &e)
    {
        std::cerr << e.what() << endl;
        res.result(http::status::internal_server_error);
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "application/json");
        res.body() = "{\"error\": \"Failed to delete notification.\"}";
    }
}