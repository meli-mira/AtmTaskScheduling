#include "../../include/models/CResourceAllocator.hpp"
// #include "../../include/models/CTask.hpp"
#include "../../include/models/CScheduler.hpp"

map<TaskSubtype, vector<string>> CResourceAllocator::task_resource_allocation;

void CResourceAllocator::init()
{
    // we go from the idea that the DB contains them already and were preloaded
    ifstream file_resource_allocator("../config/resource_allocation.txt");
    string task_type, resource_type;
    int resource_number;

    if (!file_resource_allocator.is_open())
    {
        CLogger::log("CResourceAllocator", "Config file for resources doesn't exists");
        return;
    }

    while (!file_resource_allocator.eof())
    {
        vector<string> resources;

        file_resource_allocator >> task_type >> resource_number;
        for (int i = 0; i < resource_number; i++)
        {
            file_resource_allocator >> resource_type;
            resources.push_back(resource_type);
        }

        if (task_type == "TYPE1")
            task_resource_allocation.insert(make_pair(TYPE1, resources));
        else if (task_type == "TYPE2")
            task_resource_allocation.insert(make_pair(TYPE2, resources));
        else if (task_type == "TYPE3")
            task_resource_allocation.insert(make_pair(TYPE3, resources));
        else if (task_type == "TYPE4")
            task_resource_allocation.insert(make_pair(TYPE4, resources));
        else if (task_type == "TYPE5")
            task_resource_allocation.insert(make_pair(TYPE5, resources));
        else if (task_type == "TYPE6")
            task_resource_allocation.insert(make_pair(TYPE6, resources));
        else if (task_type == "TYPE7")
            task_resource_allocation.insert(make_pair(TYPE7, resources));
        else if (task_type == "TYPE8")
            task_resource_allocation.insert(make_pair(TYPE8, resources));
        else if (task_type == "TYPE9")
            task_resource_allocation.insert(make_pair(TYPE9, resources));
        else if (task_type == "TYPE10")
            task_resource_allocation.insert(make_pair(TYPE10, resources));

        file_resource_allocator.close();
    }
}

void CResourceAllocator::allocateResources(CTask *task)
{
    if (task_resource_allocation.count(task->getTaskSubtype()))
    {
        auto it = task_resource_allocation.find(task->getTaskSubtype());
        vector<string> resources = it->second;
        CScheduler::getInstance()->addResourcesToTask(task, resources);
    }
    else
        CLogger::log("CResourceAllocator", "There are no resources for that type of task");
}