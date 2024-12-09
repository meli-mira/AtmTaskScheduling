#pragma once

#include "../../include/models/CScheduler.h"
#include "../../include/services/Sql.h"

class ISchedulerService
{
public:
    virtual void init() = 0;
    virtual void scheduleNode(string node_id) = 0;
    virtual void scheduleAllNodesFrom(string node_id) = 0;
    
};

class SchedulerService : public ISchedulerService
{
public:
    void init() override;
    void scheduleNode(string node_id) override;
    void scheduleAllNodesFrom(string node_id) override;
};