#include "../../include/models/CNotification.hpp"

int CNotification::id = 0;

CNotification::CNotification()
{
}

CNotification::CNotification(string node_id, string task_id, string description, NotificationType type)
{
	this->notification_id = ++id;
	this->task_id = task_id;
	this->node_id = node_id;
	this->description = description;
	this->type = type;
}

void CNotification::setID(int id)
{
	CNotification::id = ++id;
}

void CNotification::setDescription(string description)
{
	this->description = description;
}

int CNotification::getID() const
{
	return notification_id;
}

string CNotification::getTaskID() const
{
	return task_id;
}

string CNotification::getNodeID() const
{
	return node_id;
}

string CNotification::getDescription() const
{
	return description;
}

NotificationType CNotification::getType() const
{
	return type;
}

CNotification::~CNotification()
{
}
