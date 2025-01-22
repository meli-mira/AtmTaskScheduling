#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <vector>
#include "../../include/models/CTask.h"
#include "../../include/CUtils.h"
class TaskSerializer
{
public:
    static CTask *fromJson(const boost::json::object &obj);
    static vector<pair<string, string>> fromUpdateJson(const boost::json::object &obj);
};