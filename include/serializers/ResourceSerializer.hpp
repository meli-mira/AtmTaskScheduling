#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "../../include/models/CResource.h"
#include "../../include/CUtils.h"
class ResourceSerializer
{
public:
    static CResource *fromJson(const boost::json::object &obj);
};