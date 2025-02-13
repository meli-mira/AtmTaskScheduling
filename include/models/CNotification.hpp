#pragma once
#include "../CUtils.hpp"
#include "../CLogger.hpp"

typedef enum NotificationType
{
	RESOURCE_ALREADY_USED,
	TASK_DEADLINE_MISS,
	TASK_OVERLAPPING,
	TASK_HIGHER_PRIORITY
} NotificationType;

class CNotification
{
	static int id;
	int notification_id;
	string task_id;
	string node_id;
	string description;
	NotificationType type;

public:
	CNotification();
	CNotification(string node_id, string task_id, string description, NotificationType type);

	static void setID(int id);
	void setDescription(string description);
	int getID() const;
	string getTaskID() const;
	string getNodeID() const;
	string getDescription() const;
	NotificationType getType() const;

	~CNotification();
};
