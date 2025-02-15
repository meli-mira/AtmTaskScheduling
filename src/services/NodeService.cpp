#include "../../include/services/NodeService.hpp"

void NodeService::init()
{
    string sql_query = "SELECT CAST(node_id AS INTEGER) FROM nodes ORDER BY node_id DESC LIMIT 1";

    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);

        if (json_response.size() == 1)
            CNode::setID(stoi(json_response[0]["node_id"].get<std::string>()));
        else
            CNode::setID(0);
    }

    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void NodeService::addNode(const CNode *n)
{
    string sql_query;
    sql_query = "INSERT INTO nodes (node_id, name, capacity, minpriority, maxpriority, timetable_id, parent_node_id)";
    sql_query += " VALUES ('" + n->getID() + "','" + n->getName() + "'," +
                 to_string(n->getCapacity()) + "," + to_string(n->getMinPriority()) + "," + to_string(n->getMaxPriority()) +
                 ",'" + to_string(n->getTimetable()->getID()) + "','" + n->getParentNodeID() + "')";
    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::INSERT, sql_query);
        sql::database_utils::db_close(conn);

        CLogger::log("NodeService", "Node with id " + n->getID() + " has succesfully been added");
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

json NodeService::getAllNodes()
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT * FROM nodes";

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

json NodeService::getAllNodesOfParent(string parent_node_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT * FROM nodes";
        sql_query += " WHERE parent_node_id = '" + parent_node_id + "'";

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

json NodeService::getNodeById(string id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT * FROM nodes";
        sql_query += " WHERE node_id = '" + id + "'";

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

void NodeService::deleteNodeById(string id)
{
    try
    {
        // 1. delete asociated timetable"
        string sql_query1 = "SELECT timetable_id FROM nodes WHERE node_id='" + id + "'";

        // 2. delete node
        string sql_query2 = "DELETE FROM nodes WHERE node_id = '" + id + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query1);
        sql::database_utils::exec_sql(conn, sql::DELETE_, sql_query2);
        if (json_response.size() == 1)
        {

            sql_query1 = "DELETE FROM timetable WHERE timetable_id=" + json_response[0]["timetable_id"].get<std::string>();
            sql::database_utils::exec_sql(conn, sql::DELETE_, sql_query1);
        }
        sql::database_utils::db_close(conn);

        CLogger::log("NodeService", "Node with id " + id + " has succesfully been deleted");
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void NodeService::updateNodeById(string id)
{
    //todo
}