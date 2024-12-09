#pragma once
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string_view>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

#include "../CLogger.h"

using namespace std;
using namespace pqxx;
using json = nlohmann::json;

namespace sql
{
	typedef enum db_operation
	{
		INSERT,
		SELECT,
		UPDATE,
		DELETE_
	}db_operation;

	class database_exception
	{
	public:
		explicit database_exception(std::string_view _msg) : m_msg{_msg} {}
		std::string what() const noexcept
		{
			return m_msg;
		}

	private:
		std::string m_msg;
	};

	class database_utils
	{
	private:
		static void db_exeq_sql(connection &conn, const std::string sql);
		static json db_select(connection &conn, const std::string sql);

	public:
		static connection init();
		// connect to db
		static pqxx::connection db_connect();
		// close db
		static void db_close(connection &conn);
		// executes a query and returns json
		static json exec_sql(connection &conn, db_operation db_operation_type, const std::string sql);
	};
}
