#pragma once

#include "../../include/models/CScheduler.h"
#include "../../include/services/Sql.h"

class ISchedulerService
{
public:
    virtual void init() = 0;
    virtual void updateScheduler(string node_id, string task_id, string startdate, string enddate) = 0;
    
};

class SchedulerService : public ISchedulerService
{
public:
    void init() override; 
    void updateScheduler(string node_id, string task_id, string startdate, string enddate) override;
    //todo delete
};