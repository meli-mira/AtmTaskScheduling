#include "../../include/models/CNotification.h"
int CNotification::id = 0;
CNotification::CNotification()
{
}

CNotification::CNotification(string description, NotificationType type, CTask *t)
{
	this->taskId = to_string(id++);
	this->description = description;
	this->type = type;
	this->task = t;
}
CNotification::~CNotification()
{
}
