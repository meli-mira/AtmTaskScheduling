#pragma once

#include "../../include/models/CTask.h"
#include "../../include/services/Sql.h"

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
    virtual void deleteTaskById(string id) = 0;
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
    void deleteTaskById(string id) override;
};