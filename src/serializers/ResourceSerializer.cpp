#include "../../include/serializers/ResourceSerializer.hpp"

CResource *ResourceSerializer::fromJson(const boost::json::object &obj)
{
    string name = obj.at("name").as_string().c_str();
    string capacity = obj.at("capacity").as_string().c_str();

    CResource *r;
    if (obj.if_contains("resource_id")) // fetch resource
        r = new CResource(obj.at("resource_id").as_string().c_str(), name, stoi(capacity), stoi(obj.at("timetable_id").as_string().c_str()));
    else // creste resource
        r = new CResource(name, stoi(capacity));
    return r;
}