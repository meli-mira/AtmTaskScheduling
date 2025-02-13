#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "../../include/models/CResource.hpp"
#include "../../include/CUtils.hpp"
class ResourceSerializer
{
public:
    static CResource *fromJson(const boost::json::object &obj);
};