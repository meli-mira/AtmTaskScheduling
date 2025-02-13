#include "../../include/services/Sql.hpp"

namespace sql
{
    const char *LOGGER_SRC{"sql"};
    string DATABASE_NAME = "ppa";
    string DATABASE_USER = "postgres";
    string DATABASE_USER_PASS = "1234";
    string HOST_ADDRESS = "127.0.0.1";
    string HOST_PORT = "5432";
    void database_utils::db_exeq_sql(connection &conn, const std::string sql)
    {
        work w(conn);
        w.exec(sql);
        w.commit();
    }

    json database_utils::db_select(connection &conn, const std::string sql)
    {
        nontransaction nt(conn);
        result r(nt.exec(sql));

        int rows = r.size();
        int colums = r.columns();
        vector<unordered_map<string, string, util::string_hash, std::equal_to<>>> rec;

        for (int i = 0; i < rows; i++)
        {
            unordered_map<string, string, util::string_hash, std::equal_to<>> a;
            for (int j = 0; j < colums; j++)
            {
                if (r[i][j].is_null())
                    a.try_emplace(r.column_name(j), "");
                else
                    a.try_emplace(r.column_name(j), r[i][j].as<string>());
            }
            rec.push_back(a);
        }
        json j(rec);
        return j;
    }

    connection database_utils::init()
    {
        return db_connect();
    }

    pqxx::connection database_utils::db_connect()
    {
        connection c("dbname = " + DATABASE_NAME + " user = " + DATABASE_USER + " password = " + DATABASE_USER_PASS + " hostaddr = " + HOST_ADDRESS + " port = " + HOST_PORT);
        if (c.is_open())
        {
           // CLogger::log(LOGGER_SRC, "database_connect", "Connecting to database " + DATABASE_NAME + " with success!");
            return c;
        }
        else
        {
            //CLogger::log(LOGGER_SRC, "database_connect", "Connecting to database" + DATABASE_NAME + "failed!");
            throw database_exception("Connection failed!");
        }
    }

    void database_utils::db_close(connection &conn)
    {
        if (conn.is_open())
            conn.close();
    }

    json database_utils::exec_sql(connection &conn, db_operation db_operation_type, const std::string sql)
    {
        json response = "";
        try
        {
            if (!conn.is_open())
                throw new data_exception("Database is closed!");
            if (db_operation_type == SELECT)
                response = db_select(conn, sql);
            else
                db_exeq_sql(conn, sql);
        }
        catch (database_exception &e)
        {
            e.what();
        }
        return response;
    }
}