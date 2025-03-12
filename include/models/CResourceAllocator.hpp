#pragma once
#include "../CUtils.hpp"
#include "CTask.hpp"
#include <map>
class CResourceAllocator
{
private:
    static map<TaskSubtype, vector<string>> task_resource_allocation;

public:
    static void init();
    static void allocateResources(class CTask *task);
};