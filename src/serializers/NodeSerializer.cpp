#include "../../include/serializers/NodeSerializer.hpp"

CNode *NodeSerializer::fromJson(const boost::json::object &obj)
{
    string name = obj.at("name").as_string().c_str();
    string capacity = obj.at("capacity").as_string().c_str();
    string minpriority = obj.at("minpriority").as_string().c_str();
    string maxpriority = obj.at("maxpriority").as_string().c_str();
    string parent_node_id = obj.at("parent_node_id").as_string().c_str();

    CNode *n;
    if (obj.if_contains("node_id")) // fetch node
        n = new CNode(obj.at("node_id").as_string().c_str(), name, stoi(minpriority), stoi(maxpriority), parent_node_id, stoi(capacity), stoi(obj.at("timetable_id").as_string().c_str()));
    else // create node -->>timetable
        n = new CNode(name, stoi(minpriority), stoi(maxpriority), parent_node_id, stoi(capacity));
    return n;
}
