#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include "CUtils.hpp"

class CLogger
{
public:
	static void logger(string logMsg);
	static void log(string source, string level, string msg, string x_request_id = "") noexcept;
	static void log(string source, string msg) noexcept;
};
