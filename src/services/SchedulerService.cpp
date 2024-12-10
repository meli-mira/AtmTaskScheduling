#include "../../include/services/SchedulerService.hpp"

void SchedulerService::init()
{
}

void SchedulerService::updateScheduler(string node_id, string task_id, string startdate, string enddate)
{
    try
    {
        string sql_query = "UPDATE scheduling";
        sql_query += " SET startdate = '" + startdate + "', enddate = '" + enddate + "'";
        sql_query += " WHERE node_id = '" + node_id + "' AND task_id = '" + task_id + "'";

        connection conn = sql::database_utils::init();
        json json_response = sql::database_utils::exec_sql(conn, sql::UPDATE, sql_query);
        sql::database_utils::db_close(conn);
    }
    catch (sql::database_exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}