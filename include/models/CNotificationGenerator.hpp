#pragma once
#include "CNotification.h"

class CNotificationGenerator
{
public:
    static CNotification *generate_RESOURCE_USED_notification(string node_id, string task_id, string resource_id, time_t startDate, time_t endDate);
    static CNotification *generate_TASK_DEADLINE_MISS_notification(string node_id, string task_id, time_t startDate, time_t endDate);
    static CNotification *generate_TASK_HIGHER_PRIORITY_notification(string node_id, string task_id, time_t startDate, time_t endDate);
    static CNotification *generate_TASK_OVERLAPPING_notification(string node_id, string task_id, time_t startDate, time_t endDate);
};
