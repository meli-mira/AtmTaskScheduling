#include "../../include/services/TimetableService.hpp"

void TimetableService::init()
{
    string sql_query = "SELECT CAST(timetable_id AS INTEGER) FROM timetable ORDER BY timetable_id DESC LIMIT 1";

    try
    {
        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);

        if (json_response.size() == 1)
        {
            string last_id = to_string(json_response[0].at("timetable_id"));
            CTimetable::setID(std::stoi(last_id.substr(1, last_id.size() - 1)));
        }
        else
            CTimetable::setID(0);
    }

    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void TimetableService::addTimetable(const CTimetable *t)
{
    string sql_query;

    sql_query = "INSERT INTO timetable (timetable_id, year_mapping, days, startdateofcalendar, feb)";
    sql_query += "VALUES ('" + to_string(t->getID()) + "',";
    sql_query += "ARRAY[";

    vector<int> year = t->getYear();
    sql_query += to_string(year[0]);
    for (int i = 1; i < year.size(); i++)
        sql_query += "," + to_string(year[i]);

    sql_query += "]," + to_string(year.size()) + ",'";
    sql_query += CUtils::dateToString(t->getStartDateOfCalendar(), "%Y-%m-%d") + "',";
    if (year.size() == 365)
        sql_query += "28);";
    else
        sql_query += "29);";

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
    // add timetable if necessary trigger
}

json TimetableService::getTimetableById(string id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT timetable_id, year_mapping, days, startdateofcalendar";
        sql_query += " FROM timetable";
        sql_query += " WHERE timetable_id = '" + id + "'";

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

json TimetableService::getTimetableByResourceId(string resource_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT timetable.timetable_id, timetable.year_mapping, timetable.days, timetable.startdateofcalendar";
        sql_query += " FROM timetable";
        sql_query += " INNER JOIN resources on timetable.timetable_id = resources.timetable_id";
        sql_query += " WHERE resources.resource_id = '" + resource_id + "'";

    std:
        cerr << sql_query;
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

json TimetableService::getTimetableByNodeId(string node_id)
{
    json json_response = "";
    try
    {
        string sql_query = "SELECT timetable.timetable_id, timetable.year_mapping, timetable.days, timetable.startdateofcalendar";
        sql_query += " FROM timetable";
        sql_query += " INNER JOIN nodes on timetable.timetable_id = nodes.timetable_id";
        sql_query += " WHERE nodes.node_id = '" + node_id + "'";

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

void TimetableService::deleteTimetableById(string id)
{
    try
    {
        string sql_query = "DELETE FROM timetable WHERE timetable_id = '" + id + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::SELECT, sql_query);
        sql::database_utils::db_close(conn);

        // todo delete from existing loaded list of resources
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void TimetableService::updateTimetable(const CTimetable *t)
{
    try
    {
        string sql_query = "UPDATE timetable";
        sql_query += " SET year_mapping = ARRAY[";

        vector<int> year = t->getYear();
        sql_query += to_string(year[0]);
        for (int i = 1; i < year.size(); i++)
            sql_query += "," + to_string(year[i]);
        sql_query += "],";

        sql_query += " days = " + to_string(year.size()) + ",";
        sql_query += " startdateofcalendar = '" + CUtils::dateToString(t->getStartDateOfCalendar(), "%Y-%m-%d") + "'";
        sql_query += " WHERE timetable_id = '" + to_string(t->getID()) + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::UPDATE, sql_query);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}