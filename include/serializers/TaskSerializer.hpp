#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "../../include/models/CTask.h"
#include "../../include/CUtils.h"
class TaskSerializer
{
public:
    static CTask *fromJson(const boost::json::object &obj);
};