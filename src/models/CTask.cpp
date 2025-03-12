#include "../../include/models/CTask.hpp"
#include "../../include/models/CScheduler.hpp"
int CTask::id = 0;

int CTask::verifyInterval(CTimetable *nodeTimetable, int nodeCapacity, time_t &startDate, time_t &endDate)
{
	int startIndex = nodeTimetable->getIndexForDate(startDate);
	int n = this->duration, nrPlannedDays = 0, resourceAlocated, i, j;

	for (i = startIndex; nrPlannedDays < n && i < nodeTimetable->getNrOfDays(); i++)
	{
		// 1. check if index i day is not a working day
		if (nodeTimetable->at(i) == -1)
		{
			// 2. if the first date from interval is unavailable, change startDate by 1 day
			if (nrPlannedDays == 0)
				startDate = CUtils::addDays(startDate, 1);

			// 3. if day i is not a working day, add one day to endDate
			endDate = CUtils::addDays(endDate, 1);
		}
		// 4. check the node workload at day i
		else if (nodeTimetable->at(i) < nodeCapacity)
		{
			// 5. check if used resources are free
			resourceAlocated = 0;
			for (j = 0; j < this->usedResources.size(); j++)
			{
				if (this->usedResources[j]->isTheResourceAllocated(i) == true)
				{
					resourceAlocated = 1;
					break;
				}
			}
			// 6. if all the task's used resources are free, the day is allocated
			if (resourceAlocated == 0)
				nrPlannedDays++;
			else
			{
				// 7. one of the wanted resource is alocated in the day with index i --> try to bypass by starting a new planning interval
				startDate = nodeTimetable->getDateFromIndex(i + 1);
				endDate = CUtils::addDays(startDate, duration - 1);
				nrPlannedDays = 0; // restart the number planned days

				// 8. if the new endDate misses deadline, generate a RESOURCE_USED notification
				if (CUtils::compareDates(endDate, deadline) == false)
				{
					cout << "\nherereee\n";
					this->notification = CNotificationGenerator::generate_RESOURCE_USED_notification(this->node_id, this->task_id, this->usedResources[j]->getID(), this->startDate, this->endDate);
					return -1;
				}
			}
		}
		else
		{
			// 9. the node workload at day i is at maximum node capacity --> try to bypass by starting a new planning interval
			startDate = nodeTimetable->getDateFromIndex(i + 1);
			endDate = CUtils::addDays(startDate, duration - 1);
			nrPlannedDays = 0; // restart the number planned days
			// 10. if the new endDate misses deadline, generate a TASK_OVERLAPPING notification
			if (CUtils::compareDates(endDate, deadline) == false)
			{
				this->notification = CNotificationGenerator::generate_TASK_OVERLAPPING_notification(this->node_id, this->task_id, this->startDate, this->endDate);
				return -1;
			}
		}

		// 11. if the endDate misses deadline, generate a TASK_DEADLINE_MISS notification as there are not enough working days
		if (CUtils::compareDates(endDate, deadline) == false)
		{
			this->notification = CNotificationGenerator::generate_TASK_DEADLINE_MISS_notification(this->node_id, this->task_id, this->startDate, this->endDate);
			return -1;
		}
	}

	return nrPlannedDays;
}

CTask::CTask() {}

CTask::CTask(int priority, string name, string description, time_t startPoint, time_t endPoint, int duration, TaskType type, string node_id)
{
	this->priority = priority;
	this->name = name;
	this->description = description;
	this->duration = duration;

	this->hasBeenPlanned = false;
	this->hasIssues = false;
	this->taskType = type;
	this->taskSubType = TYPE_NULL;

	this->task_id = to_string(id);
	id++;

	this->startNoEarlierThan = startPoint;
	this->deadline = endPoint;

	this->node_id = node_id;
	this->notification = NULL;
}

CTask::CTask(int priority, string name, string description, time_t startPoint, time_t endPoint, int duration, TaskType type, string taskSubType, string node_id)
{
	this->priority = priority;
	this->name = name;
	this->description = description;
	this->duration = duration;

	this->hasBeenPlanned = false;
	this->hasIssues = false;
	this->taskType = type;
	if (taskSubType == "TYPE1")
		this->taskSubType = TYPE1;
	else if (taskSubType == "TYPE2")
		this->taskSubType = TYPE2;
	else if (taskSubType == "TYPE3")
		this->taskSubType = TYPE3;
	else if (taskSubType == "TYPE4")
		this->taskSubType = TYPE4;
	else if (taskSubType == "TYPE5")
		this->taskSubType = TYPE5;
	else if (taskSubType == "TYPE6")
		this->taskSubType = TYPE6;
	else if (taskSubType == "TYPE7")
		this->taskSubType = TYPE7;
	else if (taskSubType == "TYPE8")
		this->taskSubType = TYPE8;
	else if (taskSubType == "TYPE9")
		this->taskSubType = TYPE9;
	else if (taskSubType == "TYPE10")
		this->taskSubType = TYPE10;

	this->task_id = to_string(id);
	id++;

	this->startNoEarlierThan = startPoint;
	this->deadline = endPoint;

	this->node_id = node_id;
	this->notification = NULL;
}

CTask::CTask(string task_id, int priority, string name, string description, time_t startPoint, time_t endPoint, int duration, TaskType type, string node_id)
{
	this->priority = priority;
	this->name = name;
	this->description = description;
	this->duration = duration;

	this->hasBeenPlanned = false;
	this->hasIssues = false;
	this->taskType = type;
	this->taskSubType = TYPE_NULL;

	this->task_id = task_id;

	this->startNoEarlierThan = startPoint;
	this->deadline = endPoint;

	this->node_id = node_id;
	this->notification = NULL;
}

CTask::CTask(string task_id, int priority, string name, string description, time_t startPoint, time_t endPoint, int duration, TaskType type, string taskSubType, string node_id)
{
	this->priority = priority;
	this->name = name;
	this->description = description;
	this->duration = duration;

	this->hasBeenPlanned = false;
	this->hasIssues = false;
	this->taskType = type;
	if (taskSubType == "TYPE1")
		this->taskSubType = TYPE1;
	else if (taskSubType == "TYPE2")
		this->taskSubType = TYPE2;
	else if (taskSubType == "TYPE3")
		this->taskSubType = TYPE3;
	else if (taskSubType == "TYPE4")
		this->taskSubType = TYPE4;
	else if (taskSubType == "TYPE5")
		this->taskSubType = TYPE5;
	else if (taskSubType == "TYPE6")
		this->taskSubType = TYPE6;
	else if (taskSubType == "TYPE7")
		this->taskSubType = TYPE7;
	else if (taskSubType == "TYPE8")
		this->taskSubType = TYPE8;
	else if (taskSubType == "TYPE9")
		this->taskSubType = TYPE9;
	else if (taskSubType == "TYPE10")
		this->taskSubType = TYPE10;

	this->task_id = task_id;

	this->startNoEarlierThan = startPoint;
	this->deadline = endPoint;

	this->node_id = node_id;
	this->notification = NULL;
}

string CTask::getID() const
{
	return task_id;
}

string CTask::getNodeId() const
{
	return node_id;
}

string CTask::getName() const
{
	return name;
}

string CTask::getDescription() const
{
	return description;
}

time_t CTask::getStartDate() const
{
	return this->startDate;
}

time_t CTask::getEndDate() const
{
	return this->endDate;
}

time_t CTask::getDeadline() const
{
	return deadline;
}

time_t CTask::getStartNoEarlierThan() const
{
	return startNoEarlierThan;
}

CNotification *CTask::getNotification() const
{
	return this->notification;
}

int CTask::getPriority() const
{
	return priority;
}

int CTask::getDuration() const
{
	return duration;
}

bool CTask::getHasBeenPlanned() const
{
	return hasBeenPlanned;
}

bool CTask::getHasIssues() const
{
	return this->hasIssues;
}

bool CTask::getIsIntervalBased() const
{
	if (taskType == INTERVAL_BASED)
		return true;
	else
		return false;
}

bool CTask::isResourceByIdUsed(string resource_id)
{
	for (int i = 0; i < usedResources.size(); i++)
	{
		if (usedResources[i]->getID() == resource_id)
			return true;
	}
	return false;
}

TaskType CTask::getTaskType() const
{
	return this->taskType;
}

TaskSubtype CTask::getTaskSubtype() const
{
	return this->taskSubType;
}

string CTask::getTaskSubTypeAsString() const
{
	if (this->taskSubType == TYPE1)
		return "TYPE1";
	else if (this->taskSubType == TYPE2)
		return "TYPE2";
	else if (this->taskSubType == TYPE3)
		return "TYPE3";
	else if (this->taskSubType == TYPE4)
		return "TYPE4";
	else if (this->taskSubType == TYPE5)
		return "TYPE5";
	else if (this->taskSubType == TYPE6)
		return "TYPE6";
	else if (this->taskSubType == TYPE7)
		return "TYPE7";
	else if (this->taskSubType == TYPE8)
		return "TYPE8";
	else if (this->taskSubType == TYPE9)
		return "TYPE9";
	else if (this->taskSubType == TYPE10)
		return "TYPE10";
	else
		return "TYPE_NULL";
}

vector<CResource *> CTask::getResources() const
{
	return this->usedResources;
}

bool CTask::getIsFixed() const
{
	if (taskType == FIXED)
		return true;
	else
		return false;
}

void CTask::setTaskID(string task_id)
{
	this->task_id = task_id;
}

void CTask::setNodeID(string node_id)
{
	this->node_id = node_id;
}

void CTask::setName(string name)
{
	this->name = name;
}

void CTask::setPriority(int priority)
{
	this->priority = priority;
}

void CTask::setDuration(int duration)
{
	this->duration = duration;
}

void CTask::setDescription(string description)
{
	this->description = description;
}

void CTask::setStartNoEarlierThan(time_t t)
{
	this->startNoEarlierThan = t;
}

void CTask::setDeadline(time_t t)
{
	this->deadline = t;
}

void CTask::setStartDate(time_t date)
{
	this->startDate = date;
}

void CTask::setEndDate(time_t date)
{
	this->endDate = date;
}

void CTask::setHasIssues(bool hasIssues)
{
	this->hasIssues = hasIssues;
}

void CTask::setHasBeenPlanned(bool hasBeenPlanned)
{
	this->hasBeenPlanned = hasBeenPlanned;
}

void CTask::unsetHasBeenPlanned()
{
	this->hasBeenPlanned = false;
}

void CTask::setID(int last_id)
{
	id = ++last_id;
}

void CTask::addResource(CResource *r)
{
	this->usedResources.push_back(r);
}

int CTask::scheduleTask(CTimetable *nodeTimetable, int nodeCapacity)
{
	time_t startDate, endDate;
	startDate = this->startNoEarlierThan;
	endDate = CUtils::addDays(startDate, this->duration - 1);

	int nrPlannedDays = verifyInterval(nodeTimetable, nodeCapacity, startDate, endDate);

	// 1. the task has been succesfully planned
	if (nrPlannedDays != -1)
	{
		this->startDate = startDate;
		this->endDate = endDate;
		this->hasBeenPlanned = true;

		// 2. mark the planned days as ocupied
		nodeTimetable->setOcupied(startDate, endDate);

		// 3. set the resources of the planned task ocupied
		for (int i = 0; i < this->usedResources.size(); i++)
			usedResources[i]->setTheResourceOcupied(this, startDate, endDate);
	}
	// 4. task has failed to be planned, a notification has been generated
	else
	{
		this->hasIssues = true;
		this->hasBeenPlanned = false;
		return -1;
	}

	return 0;
}

int CTask::unscheduleTask(CTimetable *nodeTimetable)
{
	if (this->hasBeenPlanned == false)
		return -1;

	// Unschedule resources used
	for (int i = 0; i < usedResources.size(); i++)
		usedResources[i]->unsetTheResourceOcupied(this->startDate, this->endDate);

	nodeTimetable->unsetOcupied(this->startDate, this->endDate);

	this->hasBeenPlanned = false;
	this->startDate = 0;
	this->endDate = 0;

	return 0;
}

void CTask::print() const
{
	cout << "\tTask:" << name << " " << duration << ":";
	if (hasBeenPlanned == true)

		cout << "\t" << CUtils::dateToString(startDate, "%Y-%m-%d") << "\t" << CUtils::dateToString(endDate, "%Y-%m-%d") << endl;

	for (int i = 0; i < usedResources.size(); i++)
		usedResources[i]->print();

	cout << endl;
}

void CTask::deleteResource(string resource_id)
{
	for (int i = 0; i < usedResources.size(); i++)
		if (usedResources[i]->getID() == resource_id)
		{
			usedResources.erase(usedResources.begin() + i);
			return;
		}
}

CTask::~CTask()
{
	if (notification != NULL)
		delete notification;
	notification = NULL;
}
