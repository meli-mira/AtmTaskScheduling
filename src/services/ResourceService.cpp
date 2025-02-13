#include "../../include/services/ResourceService.hpp"
#include "../../include/services/TaskService.hpp"

void ResourceService::init()
{
    string sql_query = "SELECT CAST(resource_id AS INTEGER) FROM resources ORDER BY resource_id DESC LIMIT 1";

    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);
        if (json_response.size() == 1)
            CResource::setID(stoi(json_response[0]["resource_id"].get<std::string>()));

        else
            CResource::setID(0);
    }

    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void ResourceService::addResource(const CResource *r)
{
    string sql_query;
    sql_query = "INSERT INTO resources (resource_id, name, capacity, timetable_id)";
    sql_query += " VALUES ('" + r->getID() + "','" + r->getName() + "'," +
                 to_string(r->getCapacity()) + ",'" + to_string(r->getTimetable()->getID()) + "')";
    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::INSERT, sql_query);
        sql::database_utils::db_close(conn);

        CLogger::log("ResourceService", "Resource with id " + r->getID() + " has succesfully been added");
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

json ResourceService::getAllResources()
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT * FROM resources";

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

json ResourceService::getResourceById(string id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT * FROM resources WHERE resource_id = '" + id + "'";

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

json ResourceService::getResourcesByTaskId(string task_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT resources.resource_id, resources.name, resources.capacity, resources.timetable_id";
        sql_query += " FROM task_resource";
        sql_query += " INNER JOIN resources on task_resource.resource_id = resources.resource_id";
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

void ResourceService::deleteResourceById(string id)
{
    try
    {
        // 1. delete asociated timetable"
        string sql_query1 = "SELECT timetable_id FROM resources WHERE resource_id='" + id + "'";
        string sql_query2 = "DELETE FROM task_resource WHERE resource_id='" + id + "'";
        string sql_query3 = "DELETE FROM resources WHERE resource_id = '" + id + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query1);
        sql::database_utils::exec_sql(conn, sql::DELETE_, sql_query2);
        sql::database_utils::exec_sql(conn, sql::DELETE_, sql_query3);
        if (json_response.size() == 1)
        {
            sql_query1 = "DELETE FROM timetable WHERE timetable_id=" + json_response[0]["timetable_id"].get<std::string>();
            sql::database_utils::exec_sql(conn, sql::DELETE_, sql_query1);
        }
        sql::database_utils::db_close(conn);

        CLogger::log("ResourceService", "Resource with id " + id + " has succesfully been deleted");
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void ResourceService::updateResourceById(string id)
{
}