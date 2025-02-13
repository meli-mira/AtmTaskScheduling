#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "../../include/models/CTimetable.hpp"
#include "../../include/CUtils.hpp"
class TimetableSerializer
{
public:
    static CTimetable *fromJson(const boost::json::object &obj);
};