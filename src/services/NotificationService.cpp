
#include "../../include/services/NotificationService.hpp"

void NotificationService::init()
{
    string sql_query = "SELECT CAST(notification_id AS INTEGER) FROM notifications ORDER BY notification_id DESC LIMIT 1";

    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);
        if (json_response.size() == 1)
            CNotification::setID(stoi(json_response[0]["notification_id"].get<std::string>()));
        else
            CNotification::setID(0);
    }

    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void NotificationService::addNotification(const CNotification *n)
{
    string sql_query;
    sql_query = "INSERT INTO notifications(notification_id, type, description, task_id, node_id)";
    sql_query += " VALUES (" + to_string(n->getID()) + ",";

    switch (n->getType())
    {
    case RESOURCE_ALREADY_USED:
        sql_query += "'RESOURCE_ALREADY_USED',";
        break;
    case TASK_DEADLINE_MISS:
        sql_query += "'TASK_DEADLINE_MISS',";
        break;
    case TASK_OVERLAPPING:
        sql_query += "'TASK_OVERLAPPING',";
        break;
    case TASK_HIGHER_PRIORITY:
        sql_query += "'RESOURCE_ALREADY_USED',";
        break;
    default:
        sql_query += "'',";
        break;
    }
    sql_query += +"'" + n->getDescription() + "','" + n->getTaskID() + "','" + n->getNodeID() + "')";

    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::INSERT, sql_query);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

json NotificationService::getNotificationById(string id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT * FROM notifications WHERE notification_id = '" + id + "'";

        connection conn = sql::database_utils::init();
        json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return json_response;
}

json NotificationService::getNotificationsByTaskId(string task_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT * FROM notifications WHERE task_id = '" + task_id + "'";

        connection conn = sql::database_utils::init();
        json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return json_response;
}

json NotificationService::getNotificationsByNodeId(string node_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT notifications.notification_id, notifications.type, notifications.description, notifications.task_id";
        sql_query += " FROM notifications";
        sql_query += " INNER JOIN tasks on tasks.task_id = notifications.task_id";
        sql_query += " WHERE tasks.node_id = '" + node_id + "'";

        connection conn = sql::database_utils::init();
        json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return json_response;
}

void NotificationService::deleteNotificationById(string id)
{
    try
    {
        string sql_query = "DELETE FROM notifications WHERE notifications_id = '" + id + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);

        // Delete from existing loaded list of tasks from Scheduling
        // CScheduler::getInstance()->
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
void NotificationService::updateNotification(CNotification *n)
{
    try
    {
        string sql_query = "UPDATE notifications";
        sql_query += " SET description = '" + n->getDescription() + "',";
        sql_query += " task_id = '" + n->getTaskID() + "',";
        sql_query += " node_id = '" + n->getNodeID() + "',";

        switch (n->getType())
        {
        case RESOURCE_ALREADY_USED:
            sql_query += " type = 'RESOURCE_ALREADY_USED'";
            break;
        case TASK_DEADLINE_MISS:
            sql_query += " type = 'TASK_DEADLINE_MISS'";
            break;
        case TASK_OVERLAPPING:
            sql_query += " type = 'TASK_OVERLAPPING'";
            break;
        case TASK_HIGHER_PRIORITY:
            sql_query += " type = 'RESOURCE_ALREADY_USED'";
            break;
        default:
            sql_query += "type = '',";
            break;
        }
        sql_query += " WHERE notification_id = '" + to_string(n->getID()) + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::UPDATE, sql_query);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}