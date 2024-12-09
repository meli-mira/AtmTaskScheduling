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
        {
            string last_id = to_string(json_response[0].at("node_id"));
            CNode::setID(std::stoi(last_id.substr(1, last_id.size() - 1)));
        }
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
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    // todo
    // add node
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
        string sql_query = "DELETE FROM nodes WHERE node_id = '" + id + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);

        // todo delete from existing loaded list of resources TRIGGER
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void NodeService::updateNodeById(string id)
{
}