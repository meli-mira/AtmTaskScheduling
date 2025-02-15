#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>

#include "../../include/models/CTask.hpp"
#include "../../include/services/Sql.hpp"

class ITaskService
{
public:
    virtual void init() = 0;
    virtual void addTask(const CTask *t) = 0;
    virtual void addResourceToTask(const string task_id, const string resource_id) = 0;
    virtual json getAllTasks() = 0;
    virtual json getTaskById(string id) = 0;
    virtual json getTasksByNodeId(string node_id) = 0;
    virtual json getResourcesOfTask(string task_id) = 0;
    virtual json getTasksOfResourceId(string resource_id) = 0;
    virtual void deleteTaskById(string id) = 0;
    virtual void updateTask(CTask *t) = 0;
    virtual void updateTask(vector<pair<string, string>> v) = 0;
    virtual void unscheduleTask(string task_id) = 0;
};

class TaskService : public ITaskService
{
public:
    void init() override;
    void addTask(const CTask *t) override;
    void addResourceToTask(const string task_id, const string resource_id) override;
    json getAllTasks() override;
    json getTaskById(string id) override;
    json getTasksByNodeId(string node_id) override;
    json getResourcesOfTask(string task_id) override;
    json getTasksOfResourceId(string resource_id) override;
    void deleteTaskById(string id) override;
    void updateTask(CTask *t) override;
    void updateTask(vector<pair<string, string>> v) override;
    void unscheduleTask(string task_id) override;
};