#include "../../include/services/TaskService.hpp"

void TaskService::init()
{
    string sql_query = "SELECT CAST(task_id AS INTEGER) FROM tasks ORDER BY task_id DESC LIMIT 1";

    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);

        if (json_response.size() == 1)
            CTask::setID(stoi(json_response[0]["task_id"].get<std::string>()));
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

        CLogger::log("TaskService", "Task with id " + t->getID() + " has succesfully been added");
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
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

        CLogger::log("TaskService", "Resource with id " + resource_id + " has succesfully been added to task with id " + task_id);
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

json TaskService::getTasksOfResourceId(string resource_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT *";
        sql_query += " FROM tasks";
        sql_query += " INNER JOIN task_resource ON tasks.task_id = task_resource.task_id";
        sql_query += " WHERE task_resource.resource_id = '" + resource_id + "'";

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
        // 1. DELETE all the asociated resources
        string sql_query1 = "DELETE FROM task_resource WHERE task_id = '" + id + "'";
        // 2. DELETE all the asociated scheduling entries
        string sql_query2 = "DELETE FROM scheduling WHERE task_id = '" + id + "'";
        // 3. DELETE all the asociated notifications
        string sql_query3 = "DELETE FROM notifications WHERE task_id = '" + id + "'";
        // 4. DELETE the task
        string sql_query4 = "DELETE FROM tasks WHERE task_id = '" + id + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query1);
        json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query2);
        json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query3);
        json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query4);
        sql::database_utils::db_close(conn);

        CLogger::log("TaskService", "Task with id " + id + " has succesfully been deleted");
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void TaskService::updateTask(CTask *t)
{
    try
    {
        string sql_query = "UPDATE tasks";
        sql_query += " SET name = '" + t->getName() + "',";
        sql_query += " description = '" + t->getDescription() + "',";
        sql_query += " priority = " + to_string(t->getPriority()) + " ,";
        sql_query += " request = '" + CUtils::dateToString(t->getStartNoEarlierThan(), "%Y-%m-%d") + "',";
        sql_query += " deadline = '" + CUtils::dateToString(t->getDeadline(), "%Y-%m-%d") + "',";
        sql_query += " duration = " + to_string(t->getDuration()) + " ,";
        sql_query += " node_id = '" + t->getNodeId() + "',";

        if (t->getTaskType() == INTERVAL_BASED)
            sql_query += " tasktype = 'INTERVAL_BASED',";
        else
            sql_query += " tasktype = 'FIXED',";

        if (t->getHasBeenPlanned())
            sql_query += " hasbeenplanned = 'true',";
        else
            sql_query += " hasbeenplanned = 'false',";

        if (t->getHasIssues())
            sql_query += " hasissues = 'true'";
        else
            sql_query += " hasissues = 'false'";

        sql_query += " WHERE task_id = '" + t->getID() + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::UPDATE, sql_query);
        sql::database_utils::db_close(conn);

        CLogger::log("TaskService", "Task with id " + t->getID() + " has succesfully been updated");
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void TaskService::updateTask(vector<pair<string, string>> v)
{
    try
    {
        if (v.size() == 0)
            return;

        string sql_query = "UPDATE tasks SET ";

        for (int i = 1; i < v.size(); i++)
            sql_query += v[i].first + " ='" + v[i].second + "',";
        sql_query.erase(sql_query.begin() + sql_query.size() - 1);
        sql_query += " WHERE task_id = '" + v[0].second + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::UPDATE, sql_query);
        sql::database_utils::db_close(conn);

        CLogger::log("TaskService", "Task with id " + v[0].second + " has succesfully been added");
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}