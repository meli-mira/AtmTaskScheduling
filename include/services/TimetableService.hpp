#pragma once

#include "../../include/models/CTimetable.h"
#include "../../include/services/Sql.h"
#include "../../include/serializers/TimetableSerializer.hpp"

class ITimetableService
{
public:
    virtual void init() = 0;
    virtual void addTimetable(const CTimetable *t) = 0;
    virtual json getTimetableById(string id) = 0;
    virtual json getTimetableByResourceId(string resource_id) = 0;
    virtual json getTimetableByNodeId(string node_id) = 0;
    virtual void deleteTimetableById(string id) = 0;
    virtual void updateTimetableById(string id) = 0;
};

class TimetableService : public ITimetableService
{
public:
    void init();
    void addTimetable(const CTimetable *t) override;
    json getTimetableById(string id) override;
    json getTimetableByResourceId(string resource_id) override;
    json getTimetableByNodeId(string node_id) override;
    void deleteTimetableById(string id) override;
    void updateTimetableById(string id) override;
};