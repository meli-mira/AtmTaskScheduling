#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "../../include/models/CTimetable.h"
#include "../../include/CUtils.h"
class TimetableSerializer
{
public:
    static CTimetable *fromJson(const boost::json::object &obj);
};