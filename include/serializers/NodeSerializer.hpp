#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "../../include/models/CNode.h"
#include "../../include/CUtils.h"
class NodeSerializer
{
public:
    static CNode *fromJson(const boost::json::object &obj);
};