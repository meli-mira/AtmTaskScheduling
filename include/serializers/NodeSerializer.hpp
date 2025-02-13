#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include "../../include/models/CNode.hpp"
#include "../../include/CUtils.hpp"
class NodeSerializer
{
public:
    static CNode *fromJson(const boost::json::object &obj);
};