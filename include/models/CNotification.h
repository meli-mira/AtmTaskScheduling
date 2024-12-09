#pragma once
#include "../CUtils.h"
#include "../CLogger.h"
#include "CTask.h"
typedef enum NotificationType
{
	RESOURCE_USED,
	TASK_DEADLINE_MISS,
	TASK_OVERLAPPING,
	TASK_HIGHER_PRIORITY
} NotificationType;

class CNotification
{
	static int id;
	string taskId;
	string description;
	NotificationType type;

	CTask *task;

public:
	CNotification();
	CNotification(string description, NotificationType, CTask *t);

	~CNotification();
};
