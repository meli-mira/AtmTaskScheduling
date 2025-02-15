#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "../CUtils.hpp"
#include "CNotification.hpp"
#include "../CLogger.hpp"
#include "../services/NodeService.hpp"
#include "../services/TaskService.hpp"
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
	~CScheduler();

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

	/* Delete elements from loaded structures */
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

	/* Unschedule entities */
	void unscheduleResource(string resource_id, time_t startDate, time_t endDate);
	void unscheduleTask(string node_id, string task_id);
	void unscheduleNode(string node_id, time_t startDate, time_t endDate);
	void unscheduleNode(string node_id);

	CNode *searchNode(string id);
	CTask *searchTask(string id);
	CResource *searchResource(string id);

	/* Get scheduling reports */
	void DFS(CNode *Node, time_t startDate, time_t endDate, vector<unordered_map<string, string, util::string_hash, std::equal_to<>>> &rec);
	string getTasksThatUseResourceBetween(string resource_id, time_t startDate, time_t endDate);
	string getTasksForNodeBetween(string node_id, time_t startDate, time_t endDate);
	string getTasksFromNodeBetween(string node_id, time_t startDate, time_t endDate);
};
