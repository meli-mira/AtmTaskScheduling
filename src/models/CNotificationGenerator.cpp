#include "../../include/models/CNotificationGenerator.hpp"
#include "../../include/models/CScheduler.hpp"

CNotification *CNotificationGenerator::generate_RESOURCE_USED_notification(string node_id, string task_id, string resource_id, time_t startDate, time_t endDate)
{

    string description = " Task cannot be planned as resource " + resource_id + " is used by task(s): \n";
    description += CScheduler::getInstance()->getTasksThatUseResourceBetween(resource_id, startDate, endDate);

    return new CNotification(node_id, task_id, description, RESOURCE_ALREADY_USED);
}

CNotification *CNotificationGenerator::generate_TASK_DEADLINE_MISS_notification(string node_id, string task_id, time_t startDate, time_t endDate)
{
    string description = " Task cannot be planned as there are not enough working days between the desired dates.";

    return new CNotification(node_id, task_id, description, TASK_DEADLINE_MISS);
}

CNotification *CNotificationGenerator::generate_TASK_HIGHER_PRIORITY_notification(string node_id, string task_id, time_t startDate, time_t endDate)
{
    string description = " Task cannot be planned as there ..";

    return new CNotification(node_id, task_id, description, TASK_HIGHER_PRIORITY);
}

CNotification *CNotificationGenerator::generate_TASK_OVERLAPPING_notification(string node_id, string task_id, time_t startDate, time_t endDate)
{
    string description = " Task cannot be planned as maximum capacity of that node is reached. There are the following tasks planned:";
    description += CScheduler::getInstance()->getTasksForNodeBetween(node_id, startDate, endDate);

    return new CNotification(node_id, task_id, description, TASK_OVERLAPPING);
}