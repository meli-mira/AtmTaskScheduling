#include "../../include/models/CScheduler.hpp"
#include "../../include/services/NotificationService.hpp"
CScheduler *CScheduler::instance = NULL;

CScheduler::~CScheduler()
{
	for (int i = 0; i < resources.size(); i++)
		delete resources[i];
	resources.clear();
	for (int i = 0; i < nodes.size(); i++)
		delete nodes[i];
	nodes.clear();
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
			{
				auto r = searchResource(boost::json::parse(to_string(resources_task[t])).as_object().at("resource_id").as_string().c_str());
				if (task->getHasBeenPlanned() == true)
					r->addAlocare(task, task->getStartDate(), task->getEndDate());

				task->addResource(r);
			}

			node->addTask(task);
		}
		// add the node to the parent Node
		auto parentNode = searchNode(node->getParentNodeID());
		if (parentNode != NULL) // not the root
			parentNode->addChildrenNode(node);

		nodes.push_back(node);
	}

	// init the resources for task_type from file in folder config
	CResourceAllocator::init();
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
	cout << "SCHEDULING PROCESS FOR NODE WITH ID " + n->getID() << std::endl;
	cout << "Node timetable:" << endl;
	n->getTimetable()->print();
	cout << endl;

	cout << "PROCESS\t\tSTART-TIME\tEND-TIME\n";
	for (int i = 0; i < tasks.size(); i++)
	{
		if (tasks[i]->getHasBeenPlanned() == true)
			tasks[i]->print();
		else
			cout << "\tTask " << tasks[i]->getName() << " can't be planned!! Admin will be notified!" << endl;
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
	// Add the new node to parent child list
	auto parentNode = searchNode(n->getParentNodeID());
	if (parentNode != NULL)
		parentNode->addChildrenNode(n);

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
	else
		CLogger::log("CScheduler", "Node with id " + node_id + " doesn't exist. The task could not be associated");
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

void CScheduler::addResourcesToTask(CTask *t, vector<string> resources)
{
	CResource *r;
	for (int i = 0; i < resources.size(); i++)
	{
		cout << resources[i] << " ";
		r = this->searchResourceByType(resources[i]);
		if (r != NULL)
			t->addResource(r);
	}
}

void CScheduler::deleteNode(string node_id)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getID() == node_id)
		{
			auto tasks = nodes[i]->getTasks();
			for (int i = 0; i < tasks.size(); i++)
			{
				// If the task has been planned, unschedule, to set the resources free
				if (tasks[i]->getHasBeenPlanned() == true)
					tasks[i]->unscheduleTask(nodes[i]->getTimetable());

				tasks.erase(tasks.begin() + i);
				delete tasks[i];
			}
			delete nodes[i];
			nodes.erase(nodes.begin() + i);
			return;
		}
	}
}

void CScheduler::deleteTask(string task_id)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->deleteTask(task_id))
			return;
	}
}

void CScheduler::deleteResource(string resource_id)
{
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->getID() == resource_id)
		{
			delete resources[i];
			resources.erase(resources.begin() + i);
			return;
		}
	}
}

void CScheduler::deleteResourceFromTasks(string resource_id)
{
	vector<CTask *> tasks;
	for (int i = 0; i < nodes.size(); i++)
	{
		tasks = nodes[i]->getTasks();
		for (int j = 0; j < tasks.size(); j++)
			tasks[j]->deleteResource(resource_id);
	}
}

void CScheduler::deleteTaskFromNode(string task_id, string node_id)
{
}

void CScheduler::deleteNotificationFromTask(string notification_id, string task_id, string node_id) {}

void CScheduler::deleteScheduleForTask(string task_id) {}

void CScheduler::updateSchedulingStructures(CNode *n)
{
	auto timetableService = std::make_shared<TimetableService>();
	auto schedulerService = std::make_shared<SchedulerService>();
	auto notificationService = std::make_shared<NotificationService>();
	auto taskService = std::make_shared<TaskService>();

	/* 1. Update the timetable of the node */
	timetableService->updateTimetable(n->getTimetable());

	vector<CTask *> tasks = n->getTasks();
	for (int i = 0; i < tasks.size(); i++)
	{
		if (tasks[i]->getHasBeenPlanned() == true)
		{
			/* 2. Update timetables of resources used by the planned task (if any) */
			vector<CResource *> resources = tasks[i]->getResources();
			for (int j = 0; j < resources.size(); j++)
				timetableService->updateTimetable(resources[j]->getTimetable());

			// 3. Update the rezulted schedule for the planned task
			schedulerService->updateScheduler(n->getID(), tasks[i]->getID(), CUtils::dateToString(tasks[i]->getStartDate(), "%Y-%m-%d"), CUtils::dateToString(tasks[i]->getEndDate(), "%Y-%m-%d"));
		}
		else
		{
			// 4. update notification
			if (notificationService->getNotificationsByTaskId(tasks[i]->getID()).size() == 0)
				notificationService->addNotification(tasks[i]->getNotification());
			else
				notificationService->updateNotification(tasks[i]->getNotification());
		}

		// 5. update task
		taskService->updateTask(tasks[i]);
	}
}

void CScheduler::updateTask(string task_id, vector<pair<string, string>> v)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<CTask *> tasks = nodes[i]->getTasks();
		for (int j = 0; j < tasks.size(); j++)
		{
			if (tasks[j]->getID() == task_id)
			{
				for (int k = 1; k < v.size(); k++)
				{
					if (v[k].first == "node_id")
						tasks[j]->setNodeID(v[k].second);

					else if (v[k].first == "deadline")
						tasks[j]->setDeadline(CUtils::parseDateTime(v[k].second.c_str(), "%Y-%m-%d"));

					else if (v[k].first == "request")
						tasks[j]->setStartNoEarlierThan(CUtils::parseDateTime(v[k].second.c_str(), "%Y-%m-%d"));

					else if (v[k].first == "name")
						tasks[j]->setName(v[k].second);

					else if (v[k].first == "description")
						tasks[j]->setDescription(v[k].second);

					else if (v[k].first == "duration")
						tasks[j]->setDuration(stoi(v[k].second));

					else if (v[k].first == "priority")
						tasks[j]->setPriority(stoi(v[k].second));
				}
				break;
			}

			if (j < tasks.size())
				break;
		}
	}
}

void CScheduler::unscheduleResource(string resource_id, time_t startDate, time_t endDate)
{
	auto r = searchResource(resource_id);
	if (r != NULL)
		r->unsetTheResourceOcupied(startDate, endDate);
}

void CScheduler::unscheduleTask(string node_id, string task_id)
{
	auto n = searchNode(node_id);
	if (n != NULL)
	{
		auto task = n->getTask(task_id);
		if (task != NULL)
			task->unscheduleTask(n->getTimetable());
	}
}

void CScheduler::unscheduleNode(string node_id, time_t startDate, time_t endDate)
{
	auto n = searchNode(node_id);
	if (n != NULL)
		n->unscheduleTasks(startDate, endDate);
}

void CScheduler::unscheduleNode(string node_id)
{
	auto n = searchNode(node_id);
	if (n != NULL)
		n->unscheduleTasks();
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

CTask *CScheduler::searchTask(string id)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		auto t = nodes[i]->getTask(id);
		if (t != NULL)
			return t;
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

CResource *CScheduler::searchResourceByType(string resource_type)
{
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->getName() == resource_type)
			return resources[i];
	}
	return NULL;
}

void CScheduler::DFS(CNode *node, time_t startDate, time_t endDate, vector<unordered_map<string, string, util::string_hash, std::equal_to<>>> &rec)
{
	if (node != NULL)
	{
		auto tasks = node->getTasksBetween(startDate, endDate);
		for (int i = 0; i < tasks.size(); i++)
		{
			unordered_map<string, string, util::string_hash, std::equal_to<>> a;
			a.try_emplace("node_id", node->getID());
			a.try_emplace("task_id", tasks[i]->getID());
			a.try_emplace("task_priority", to_string(tasks[i]->getPriority()));
			a.try_emplace("task_startDate", CUtils::dateToString(tasks[i]->getStartDate(), "%Y.%m.%d"));
			a.try_emplace("task_endDate", CUtils::dateToString(tasks[i]->getEndDate(), "%Y.%m.%d"));
			rec.push_back(a);
		}
		for (int i = 0; i < node->getChildren().size(); i++)
		{
			DFS(node->getChildren()[i], startDate, endDate, rec);
		}
	}
}

string CScheduler::getTasksThatUseResourceBetween(string resource_id, time_t startDate, time_t endDate)
{
	CResource *r = searchResource(resource_id);
	vector<unordered_map<string, string, util::string_hash, std::equal_to<>>> rec;
	if (r != NULL)
	{
		vector<CTask *> tasks = r->getTasksThatUseResourceBetween(startDate, endDate);
		std::cerr << tasks.size();
		for (int i = 0; i < tasks.size(); i++)
		{
			unordered_map<string, string, util::string_hash, std::equal_to<>> a;
			a.try_emplace("resource_id", resource_id);
			a.try_emplace("node_id", tasks[i]->getNodeId());
			a.try_emplace("task_id", tasks[i]->getID());
			a.try_emplace("task_priority", to_string(tasks[i]->getPriority()));
			a.try_emplace("task_startDate", CUtils::dateToString(tasks[i]->getStartDate(), "%Y.%m.%d"));
			a.try_emplace("task_endDate", CUtils::dateToString(tasks[i]->getEndDate(), "%Y.%m.%d"));
			rec.push_back(a);
		}
	}
	return json(rec).dump();
}

string CScheduler::getTasksForNodeBetween(string node_id, time_t startDate, time_t endDate)
{
	CNode *node = searchNode(node_id);
	vector<CTask *> tasks;

	vector<unordered_map<string, string, util::string_hash, std::equal_to<>>> rec;
	if (node != NULL)
	{
		tasks = node->getTasksBetween(startDate, endDate);
		for (int i = 0; i < tasks.size(); i++)
		{
			unordered_map<string, string, util::string_hash, std::equal_to<>> a;
			a.try_emplace("node_id", node_id);
			a.try_emplace("task_id", tasks[i]->getID());
			a.try_emplace("task_priority", to_string(tasks[i]->getPriority()));
			a.try_emplace("task_startDate", CUtils::dateToString(tasks[i]->getStartDate(), "%Y.%m.%d"));
			a.try_emplace("task_endDate", CUtils::dateToString(tasks[i]->getEndDate(), "%Y.%m.%d"));
			rec.push_back(a);
		}
	}
	return json(rec).dump();
}

string CScheduler::getTasksFromNodeBetween(string node_id, time_t startDate, time_t endDate)
{
	CNode *node = searchNode(node_id);
	vector<unordered_map<string, string, util::string_hash, std::equal_to<>>> rec;
	DFS(node, startDate, endDate, rec);
	return json(rec).dump();
}