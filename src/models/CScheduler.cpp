#include "../../include/models/CScheduler.h"

CScheduler *CScheduler::instance = NULL;

bool CScheduler::checkTaskDependencies(CTask *t, time_t startDate, time_t endDate)
{
	vector<CResource *> resources = t->getResources();
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->isTheResourceAllocated(startDate, endDate) == true)
		{
			CLogger::logger(string("Resource ") + resources[i]->getName() + string(" is ocupied between- ") + CUtils::dateToString(startDate, "%Y.%m.%d") + CUtils::dateToString(endDate, "%Y.%m.%d"));
			return false;
		}
	}
	return true;
}

void CScheduler::setTasksResourcesOcupied(CTask *t, time_t startDate, time_t endDate)
{
	vector<CResource *> resources = t->getResources();
	for (int i = 0; i < resources.size(); i++)
	{
		resources[i]->setTheResourceOcupied(t, startDate, endDate);
	}
}

CScheduler *CScheduler::getInstance()
{
	if (instance == NULL)
		instance = new CScheduler();
	return instance;
}

void CScheduler::destroyInstance()
{
	if (instance != NULL)
		delete instance;
	instance = NULL;
}

void CScheduler::init()
{
	auto nodeService = std::make_shared<NodeService>();
	auto taskService = std::make_shared<TaskService>();
	auto timetableService = std::make_shared<TimetableService>();
	auto resourceService = std::make_shared<ResourceService>();

	// Fetch all resources
	json resources_json = resourceService->getAllResources();
	for (int i = 0; i < resources_json.size(); i++)
	{
		/* Get the resource */
		auto resource = ResourceSerializer::fromJson(boost::json::parse(to_string(resources_json[i])).as_object());

		/* Get the timetable alocated */
		json timetable_json = timetableService->getTimetableById(to_string(resource->getTimetableId()));
		auto timetable = TimetableSerializer::fromJson(boost::json::parse(to_string(timetable_json[0])).as_object());
		resource->setTimetable(timetable);

		/* Add the resource to scheduler */
		resources.push_back(resource);
	}

	// Fetch all nodes
	json nodes_json = nodeService->getAllNodes();
	for (int i = 0; i < nodes_json.size(); i++)
	{
		/* Get the node */
		auto node = NodeSerializer::fromJson(boost::json::parse(to_string(nodes_json[i])).as_object());

		/* Get the timetable alocated to the node */
		json timetable_json = timetableService->getTimetableById(to_string(node->getTimetableId()));
		auto timetable = TimetableSerializer::fromJson(boost::json::parse(to_string(timetable_json[0])).as_object());
		node->setTimetable(timetable);

		/* Get the tasks for node */
		json tasks_json = taskService->getTasksByNodeId(node->getID());
		for (int j = 0; j < tasks_json.size(); j++)
		{
			auto task = TaskSerializer::fromJson(boost::json::parse(to_string(tasks_json[j])).as_object());
			/* Get the resources for task */
			json resources_task = taskService->getResourcesOfTask(task->getID());
			for (int t = 0; t < resources_task.size(); t++)
				task->addResource(searchResource(boost::json::parse(to_string(resources_task[t])).as_object().at("resource_id").as_string().c_str()));

			node->addTask(task);
		}
		// to do add parent node
		// todo add childen nodes
		nodes.push_back(node);
	}
}

void CScheduler::scheduleAll(CNode *n)
{
	// todo
}

void CScheduler::scheduleTasksForNode(CNode *n)
{
	n->scheduleTasks();
}

void CScheduler::printScheduledTasks(CNode *n)
{
	vector<CTask *> tasks = n->getTasks();

	cout << "PROCESS\t\tSTART-TIME\tEND-TIME\n";
	for (int i = 0; i < tasks.size(); i++)
	{
		if (tasks[i]->getHasBeenPlanned() == true)
			tasks[i]->print();
		else
			cout << "Task " << tasks[i]->getName() << " can't be planned!! Admin will be notified!" << endl;
	}
}

void CScheduler::computeOutFileForPloting(CNode *n)
{
	ofstream g("out.txt");

	vector<CTask *> tasks = n->getTasks();
	int ok = 0;
	for (int i = 0; i < tasks.size(); i++)
	{
		if (tasks[i]->getHasBeenPlanned() == true)
		{
			if (ok == 0)
			{
				g << CUtils::dateToString(tasks[i]->getStartDate(), "%Y-%m-%d") << endl;
				ok = 1;
			}

			int j = 1;
			while (j <= tasks[i]->getDuration())
			{
				g << tasks[i]->getName() << endl;
				j++;
			}
		}
	}
	g.close();
}

CNode *CScheduler::readNodeFromFile(string filename)
{
	ifstream f(filename);
	if (f.fail())
		return NULL;

	string nodeName, nodeTasksFile, parentNodeID;
	int minPriority, maxPriority, capacity, level;
	CNode *n = NULL;
	while (!f.eof())
	{
		f >> nodeName >> minPriority >> maxPriority >> parentNodeID >> capacity >> level >> nodeTasksFile;
		n = new CNode(nodeName, minPriority, maxPriority, parentNodeID, capacity);
		n->readTasksFromFile(nodeTasksFile);

		this->nodes.push_back(n);
	}

	f.close();
	return n;
}

void CScheduler::readNodesFromFile(string filename)
{
	ifstream f(filename);
	string nodeFile;

	while (f >> nodeFile)
		readNodeFromFile(nodeFile);

	f.close();
}

void CScheduler::readResourcesFromFile(string filename)
{
	ifstream f(filename);
	string resourceName;
	int resourceCapacity;

	CResource *r;

	while (f >> resourceName >> resourceCapacity)
	{
		r = new CResource(resourceName, resourceCapacity);
		resources.push_back(r);
	}

	f.close();
}

void CScheduler::printAll()
{
	cout << "Nodes:" << endl;
	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->print();
	cout << "Resources:" << endl;
	for (int i = 0; i < resources.size(); i++)
		resources[i]->print();
}

void CScheduler::addNode(CNode *n)
{
	nodes.push_back(n);
}

void CScheduler::addResource(CResource *r)
{
	resources.push_back(r);
}

void CScheduler::addTaskToNode(string node_id, CTask *t)
{
	CNode *n = searchNode(node_id);
	if (n != NULL)
		n->addTask(t);
}

void CScheduler::addResourceToTask(string task_id, string resource_id)
{
	CTask *t = NULL;
	CResource *r = searchResource(resource_id);
	if (r != NULL)
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			t = nodes[i]->getTask(task_id);
			if (t != NULL)
			{
				t->addResource(r);
				break;
			}
		}
	}
}

void CScheduler::updateSchedulingStructures(CNode *n)
{
	auto timetableService = std::make_shared<TimetableService>();
	auto schedulerService = std::make_shared<SchedulerService>();
	/* Update the timetable of the node */
	timetableService->updateTimetable(n->getTimetable());

	vector<CTask *> tasks = n->getTasks();
	for (int i = 0; i < tasks.size(); i++)
	{
		if (tasks[i]->getHasBeenPlanned() == true)
		{
			/* Update timetables of resources used by the planned task */
			vector<CResource *> resources = tasks[i]->getResources();
			for (int j = 0; j < resources.size(); j++)
				timetableService->updateTimetable(resources[j]->getTimetable());

			schedulerService->updateScheduler(n->getID(), tasks[i]->getID(), CUtils::dateToString(tasks[i]->getStartDate(), "%Y-%m-%d"), CUtils::dateToString(tasks[i]->getEndDate(), "%Y-%m-%d"));

			// set hasbeenplanned true
			// is has issues ,add notifications
		}
	}
}

CNode *CScheduler::searchNode(string id)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getID() == id)
			return nodes[i];
	}
	return NULL;
}

CResource *CScheduler::searchResource(string id)
{
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->getID() == id)
			return resources[i];
	}
	return NULL;
}

void CScheduler::DFS(CNode *node)
{
	cout << node->getName() << endl;
	for (int i = 0; i < node->getChildren().size(); i++)
	{
		DFS(node->getChildren()[i]);
	}
}
