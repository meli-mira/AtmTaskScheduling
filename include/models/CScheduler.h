#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "../CUtils.h"
#include "CNotification.h"
#include "../CLogger.h"
#include "../services/NodeService.hpp"
#include "../services/TaskService.h"
#include "../services/ResourceService.hpp"
#include "../services/TimetableService.hpp"
#include "../services/SchedulerService.hpp"
#include "../serializers/ResourceSerializer.hpp"
#include "../serializers/NodeSerializer.hpp"
#include "../serializers/TaskSerializer.hpp"
using namespace std;

class CScheduler
{
private:
	vector<CNode *> nodes;
	vector<CResource *> resources;

	static CScheduler *instance;
	CScheduler() {}
	~CScheduler() {}

public:
	static CScheduler *getInstance();
	static void destroyInstance();

	void init();

	void scheduleAll(CNode *n);
	void scheduleTasksForNode(CNode *n);

	void printScheduledTasks(CNode *n);
	void computeOutFileForPloting(CNode *n);

	void printAll();

	/* Add elements to loaded structures */
	void addNode(CNode *n);
	void addResource(CResource *r);
	void addTaskToNode(string node_id, CTask *t);
	void addResourceToTask(string task_id, string resource_id);

	/* todo Delete elements from loaded structures */
	void deleteNode(string node_id);
	void deleteTask(string task_id);
	void deleteResource(string resource_id);
	void deleteResourceFromTasks(string resource_id);
	void deleteTaskFromNode(string task_id, string node_id);
	void deleteNotificationFromTask(string notification_id, string task_id, string node_id);
	void deleteScheduleForTask(string task_id);

	/* Update structures */
	void updateSchedulingStructures(CNode *n);
	void updateTask(string task_id, vector<pair<string, string>> v);

	CNode *searchNode(string id);
	CResource *searchResource(string id);

	void DFS(CNode *Node);

	string getTasksThatUseResourceBetween(string resourceID, time_t startDate, time_t endDate);
	string getTasksBetween(string node_id, time_t startDate, time_t endDate);
};
