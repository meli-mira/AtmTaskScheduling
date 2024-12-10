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

	bool checkTaskDependencies(CTask *t, time_t startDate, time_t endDate);
	void setTasksResourcesOcupied(CTask *t, time_t startDate, time_t endDate);

public:
	static CScheduler *getInstance();
	static void destroyInstance();

	void init();

	void scheduleAll(CNode *n);
	void scheduleTasksForNode(CNode *n);

	void printScheduledTasks(CNode *n);
	void computeOutFileForPloting(CNode *n);

	CNode *readNodeFromFile(string filename);
	void readNodesFromFile(string filaname);
	void readResourcesFromFile(string filename);

	void printAll();

	void addNode(CNode *n);
	void addResource(CResource *r);
	void addTaskToNode(string node_id, CTask *t);
	void addResourceToTask(string task_id, string resource_id);

	void updateSchedulingStructures(CNode *n);

	CNode *searchNode(string id);
	CResource *searchResource(string id);

	void DFS(CNode *Node);
};
