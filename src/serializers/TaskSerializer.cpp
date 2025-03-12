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

    // check if is a new request or not
    if (obj.if_contains("task_id"))
    {
        if (taskType == "INTERVAL_BASED")
        {
            if (obj.if_contains("tasksubtype"))
                t = new CTask(obj.at("task_id").as_string().c_str(), priority, name, description, CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(deadline.c_str(), "%Y-%m-%d"), duration, INTERVAL_BASED, obj.at("tasksubtype").as_string().c_str(), node_id);
            else
                t = new CTask(obj.at("task_id").as_string().c_str(), priority, name, description, CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(deadline.c_str(), "%Y-%m-%d"), duration, INTERVAL_BASED, node_id);
        }
        else
            t = new CTask(obj.at("task_id").as_string().c_str(), priority, name, description, CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(deadline.c_str(), "%Y-%m-%d"), duration, FIXED, node_id);
    }
    else
    {
        if (taskType == "INTERVAL_BASED")
        {
            if (obj.if_contains("tasksubtype"))
                t = new CTask(priority, name, description, CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(deadline.c_str(), "%Y-%m-%d"), duration, INTERVAL_BASED, obj.at("tasksubtype").as_string().c_str(), node_id);
            else
                t = new CTask(priority, name, description, CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(deadline.c_str(), "%Y-%m-%d"), duration, INTERVAL_BASED, node_id);
        }
        else
            t = new CTask(priority, name, description, CUtils::parseDateTime(request.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(deadline.c_str(), "%Y-%m-%d"), duration, FIXED, node_id);
    }
    if (obj.if_contains("hasbeenplanned"))
        if (obj.at("hasbeenplanned").as_string() == "f")
            t->setHasBeenPlanned(false);
        else
        {
            t->setHasBeenPlanned(true);
            // set start/end date
            t->setStartDate(CUtils::parseDateTime(obj.at("startdate").as_string().c_str(), "%Y-%m-%d"));
            t->setEndDate(CUtils::parseDateTime(obj.at("enddate").as_string().c_str(), "%Y-%m-%d"));
        }

    if (obj.if_contains("hasissues"))
        if (obj.at("hasissues").as_string() == "f")
            t->setHasIssues(false);
        else
            t->setHasIssues(true);
    return t;
}

vector<pair<string, string>> TaskSerializer::fromUpdateJson(const boost::json::object &obj)
{
    vector<pair<string, string>> v;
    if (obj.if_contains("task_id"))
        v.push_back(make_pair("task_id", obj.at("task_id").as_string().c_str()));
    else
        return v;

    if (obj.if_contains("name"))
        v.push_back(make_pair("name", obj.at("name").as_string().c_str()));
    if (obj.if_contains("description"))
        v.push_back(make_pair("description", obj.at("description").as_string().c_str()));
    if (obj.if_contains("priority"))
        v.push_back(make_pair("priority", obj.at("priority").as_string().c_str()));
    if (obj.if_contains("request"))
        v.push_back(make_pair("request", obj.at("request").as_string().c_str()));
    if (obj.if_contains("deadline"))
        v.push_back(make_pair("deadline", obj.at("deadline").as_string().c_str()));
    if (obj.if_contains("duration"))
        v.push_back(make_pair("duration", obj.at("duration").as_string().c_str()));
    if (obj.if_contains("node_id"))
        v.push_back(make_pair("node_id", obj.at("node_id").as_string().c_str()));
    if (obj.if_contains("tasktype"))
        v.push_back(make_pair("tasktype", obj.at("tasktype").as_string().c_str()));
    if (obj.if_contains("hasbeenplanned"))
        v.push_back(make_pair("hasbeenplanned", obj.at("hasbeenplanned").as_string().c_str()));
    if (obj.if_contains("hasissues"))
        v.push_back(make_pair("hasissues", obj.at("hasissues").as_string().c_str()));
    return v;
}