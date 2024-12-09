#include "../../include/services/TaskService.h"

void TaskService::init()
{
    string sql_query = "SELECT CAST(task_id AS INTEGER) FROM tasks ORDER BY task_id DESC LIMIT 1";

    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);

        if (json_response.size() == 1)
        {
            string last_id = to_string(json_response[0].at("task_id"));
            CTask::setID(std::stoi(last_id.substr(1, last_id.size() - 1)));
        }
        else
            CTask::setID(0);
    }

    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void TaskService::addTask(const CTask *t)
{
    string sql_query;

    // Insert an entry into scheduling
    string sql_query2 = "INSERT INTO scheduling(node_id, task_id)";
    sql_query2 += " VALUES ('" + t->getNodeId() + "','" + t->getID() + "')";

    if (t->getTaskType() == INTERVAL_BASED)
    {
        sql_query = "INSERT INTO tasks(task_id, name, description, priority, request, deadline, duration, hasissues, hasbeenplanned, tasktype, node_id)";
        sql_query += "VALUES ('" + t->getID() + "','" + t->getName() + "','" +
                     t->getDescription() + "'," + to_string(t->getPriority()) + ",'" +
                     CUtils::dateToString(t->getStartNoEarlierThan(), "%Y-%m-%d") + "','" +
                     CUtils::dateToString(t->getDeadline(), "%Y-%m-%d") + "'," + to_string(t->getDuration()) +
                     "," + "'false'," + "'false'," + "'INTERVAL_BASED'," + "'" + t->getNodeId() + "')";
    }
    else
    {
        sql_query = "INSERT INTO tasks(task_id, name, description, priority, request, deadline, duration, hasissues, hasbeenplanned, tasktype, node_id)";
        sql_query += "VALUES ('" + t->getID() + "','" + t->getName() + "','" +
                     t->getDescription() + "'," + to_string(t->getPriority()) + ",'" +
                     CUtils::dateToString(t->getStartNoEarlierThan(), "%Y-%m-%d") + "','" +
                     CUtils::dateToString(t->getDeadline(), "%Y-%m-%d") + "'," + to_string(t->getDuration()) +
                     "," + "'false'," + "'false'," + "'FIXED'," + "'" + t->getNodeId() + "')";
    }
    try
    {
        connection conn = sql::database_utils::init();
        sql::database_utils::exec_sql(conn, sql::INSERT, sql_query);
        sql::database_utils::exec_sql(conn, sql::INSERT, sql_query2);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // todo
    // add task to the CNode..
}

void TaskService::addResourceToTask(const string task_id, const string resource_id)
{
    string sql_query = "INSERT INTO task_resource(task_id, resource_id)";
    sql_query += " VALUES('" + task_id + "','" + resource_id + "')";

    try
    {
        connection conn = sql::database_utils::init();
        sql::database_utils::exec_sql(conn, sql::INSERT, sql_query);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

json TaskService::getAllTasks()
{
    json json_response = "";
    try
    {

        string sql_query = "SELECT * FROM tasks";

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

json TaskService::getTaskById(string id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT tasks.task_id, tasks.name, tasks.description, tasks.priority, tasks.request, tasks.deadline, tasks.duration, tasks.hasissues,";
        sql_query += " tasks.hasbeenplanned, tasks.tasktype, scheduling.startdate, scheduling.enddate, scheduling.node_id";
        sql_query += " FROM tasks";
        sql_query += " INNER JOIN scheduling ON scheduling.task_id = tasks.task_id";
        sql_query += " WHERE tasks.task_id = '" + id + "'";

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

json TaskService::getTasksByNodeId(string node_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT tasks.task_id, tasks.name, tasks.description, tasks.priority, tasks.request, tasks.deadline, tasks.duration, tasks.hasissues,";
        sql_query += " tasks.hasbeenplanned, tasks.tasktype, scheduling.startdate, scheduling.enddate, scheduling.node_id";
        sql_query += " FROM tasks";
        sql_query += " INNER JOIN scheduling ON scheduling.task_id = tasks.task_id";
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

json TaskService::getResourcesOfTask(string task_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT resources.resource_id, resources.name, resources.capacity, resources.timetable_id";
        sql_query += " FROM task_resource";
        sql_query += " INNER JOIN resources ON resources.resource_id = task_resource.resource_id";
        sql_query += " WHERE task_resource.task_id = '" + task_id + "'";

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

void TaskService::deleteTaskById(string id)
{
    try
    {
        string sql_query = "DELETE FROM tasks WHERE task_id = '" + id + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);

        // todo delete from existing loaded list of tasks
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}