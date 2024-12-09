#include "../../include/controllers/TimetableController.hpp"

void TimetableController::getTimetableById(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = timetableService->getTimetableById(ctx.getParam("id").c_str());

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
        res.body() = "{\"error\": \"Failed to load timetable.\"}";
    }
}

void TimetableController::getTimetableByNodeId(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = timetableService->getTimetableByNodeId(ctx.getParam("node_id").c_str());

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
        res.body() = "{\"error\": \"Failed to load timetable for node.\"}";
    }
}

void TimetableController::getTimetableByResourceId(Context &ctx)
{
    auto &res = ctx.getResponse();
    try
    {
        json json_response = timetableService->getTimetableByResourceId(ctx.getParam("resource_id").c_str());

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
        res.body() = "{\"error\": \"Failed to load timetable for resource.\"}";
    }
}

void TimetableController::updateTimetable(Context &ctx)
{
}