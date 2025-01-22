#include "../../include/models/CScheduler.h"
#include "../../include/services/NotificationService.hpp"
CScheduler *CScheduler::instance = NULL;

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

			// todo if planned dates
			if (task->getHasBeenPlanned() == true)
			{
			}
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
	cout << "-----" << tasks.size() << endl;
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

void CScheduler::deleteNode(string node_id)
{
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->getID() == node_id)
		{
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
		{
			return;
		}
	}
}

void CScheduler::deleteResource(string resource_id)
{
	for (int i = 0; i < resources.size(); i++)
	{
		if (resources[i]->getID() == resource_id)
		{
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

string CScheduler::getTasksThatUseResourceBetween(string resourceID, time_t startDate, time_t endDate)
{
	string message = "";
	for (int i = 0; i < nodes.size(); i++)
	{
		vector<CTask *> tasks = nodes[i]->getTasksBetween(startDate, endDate);
		for (int j = 0; j < tasks.size(); j++)
		{
			if (tasks[j]->isResourceByIdUsed(resourceID))
			{
				message += "Task with id:" + tasks[j]->getID() + " of node with id:" + nodes[i]->getID() +
						   " is using the resource:" + resourceID + " between:" + CUtils::dateToString(tasks[j]->getStartDate(), "%Y.%m.%d") +
						   "-" + CUtils::dateToString(tasks[j]->getEndDate(), "%Y.%m.%d") + "\n";
			}
		}
	}
	return message;
}

string CScheduler::getTasksBetween(string node_id, time_t startDate, time_t endDate)
{
	string message = "";
	CNode *node = searchNode(node_id);
	if (node != NULL)
	{
		vector<CTask *> tasks = node->getTasksBetween(startDate, endDate);
		for (int i = 0; i < tasks.size(); i++)
		{
			message += "Task with id:" + tasks[i]->getID() + " and priority:" + to_string(tasks[i]->getPriority()) +
					   " is planned between:" + CUtils::dateToString(tasks[i]->getStartDate(), "%Y.%m.%d") +
					   "-" + CUtils::dateToString(tasks[i]->getEndDate(), "%Y.%m.%d") + "\n";
		}
	}
	return message;
}