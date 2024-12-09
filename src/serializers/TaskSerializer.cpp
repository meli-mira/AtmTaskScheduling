#include "../../include/serializers/TaskSerializer.hpp"

CTask *TaskSerializer::fromJson(const boost::json::object &obj)
{

    int priority = stoi(obj.at("priority").as_string().c_str());
    string name = obj.at("name").as_string().c_str();
    string description = obj.at("description").as_string().c_str();
    string request = obj.at("request").as_string().c_str();
    string deadline = obj.at("deadline").as_string().c_str();
    int duration = stoi(obj.at("duration").as_string().c_str());
    string taskType = obj.at("tasktype").as_string().c_str();
    string node_id = obj.at("node_id").as_string().c_str();

    CTask *t;

    if (taskType == "INTERVAL_BASED")
        t = new CTask(priority, name, description, CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), duration, INTERVAL_BASED, node_id);
    else
        t = new CTask(priority, name, description, CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), duration, FIXED, node_id);

    // check if is a new request or not
    if (obj.if_contains("task_id"))
        t->setTaskID(obj.at("task_id").as_string().c_str());

    if (obj.if_contains("hasbeenplanned"))
        if (obj.at("hasbeenplanned").as_string() == "f")
            t->setHasBeenPlanned(false);
        else
            t->setHasBeenPlanned(true);

    if (obj.if_contains("hasissues"))
        if (obj.at("hasissues").as_string() == "f")
            t->setHasIssues(false);
        else
            t->setHasIssues(true);
    return t;
}