#include "../../include/models/CNode.h"
#include "../../include/services/NodeService.hpp"
int CNode::id = 1;

CNode::CNode(string name, int minPriority, int maxPriority, string parent_node_id, int capacity)
{
	this->name = name;
	this->minPriority = minPriority;
	this->maxPriority = maxPriority;
	this->parent_node_id = parent_node_id;
	this->capacity = capacity;

	/*if (parent_node_id != "")
	{
		//todo
		parentNode->addChildrenNode(this);
	}*/

	this->node_id = to_string(id);
	id++;

	timetable = new CTimetable(time(0));
}

CNode::CNode(string id, string name, int minPriority, int maxPriority, string parent_node_id, int capacity, int timetable_id)
{
	this->node_id = id;

	this->name = name;
	this->minPriority = minPriority;
	this->maxPriority = maxPriority;
	this->parent_node_id = parent_node_id;
	this->capacity = capacity;
	this->timetable_id = timetable_id;
}

void CNode::addChildrenNode(CNode *node)
{
	childNodes.push_back(node);
}

void CNode::addTask(CTask *t)
{
	tasks.push_back(t);
}

void CNode::addNotification(CNotification *n)
{
	notifications.push_back(n);
}

int CNode::getLevel() const
{
	return this->level;
}

int CNode::getCapacity() const
{
	return this->capacity;
}

string CNode::getID() const
{
	return node_id;
}

string CNode::getParentNodeID() const
{
	return parent_node_id;
}

string CNode::getName() const
{
	return name;
}

int CNode::getTimetableId() const
{
	return this->timetable_id;
}

CTimetable *CNode::getTimetable() const
{
	return timetable;
}

int CNode::getMinPriority() const
{
	return minPriority;
}

int CNode::getMaxPriority() const
{
	return maxPriority;
}

vector<CNode *> CNode::getChildren() const
{
	return childNodes;
}

CNode *CNode::getParentNode() const
{
	return parentNode;
}

void CNode::setTimetable(CTimetable *t)
{
	this->timetable = t;
}

void CNode::setID(int last_id)
{
	id = ++last_id;
}

void CNode::readTasksFromFile(string filename)
{
	ifstream f(filename);
	string taskName, taskDescription, taskStartNoEarlienThan, taskDeadline, type, resourceFile;
	int taskDuration, taskPriority;
	CTask *t;

	while (f >> taskName >> taskPriority >> taskDescription >> type >> taskStartNoEarlienThan >> taskDeadline >> taskDuration >> resourceFile)
	{
		if (type == "I")
			t = new CTask(taskPriority, taskName, taskDescription, CUtils::parseDateTime(taskStartNoEarlienThan.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(taskDeadline.c_str(), "%Y-%m-%d"), taskDuration, INTERVAL_BASED, resourceFile, node_id);
		else
			t = new CTask(taskPriority, taskName, taskDescription, CUtils::parseDateTime(taskStartNoEarlienThan.c_str(), "%Y-%m-%d"), CUtils::parseDateTime(taskDeadline.c_str(), "%Y-%m-%d"), taskDuration, FIXED, resourceFile, node_id);
		tasks.push_back(t);
	}

	f.close();
}

vector<CTask *> CNode::getTasks() const
{
	return tasks;
}

void CNode::sortTasksByDeadline()
{
	for (int i = 0; i < tasks.size() - 1; i++)
	{
		for (int j = i + 1; j < tasks.size(); j++)
		{
			if ((tasks[i]->getDeadline() > tasks[j]->getDeadline()) || (tasks[i]->getDeadline() == tasks[j]->getDeadline() && tasks[i]->getPriority() > tasks[j]->getPriority()))
			{
				CTask *aux = tasks[i];
				tasks[i] = tasks[j];
				tasks[j] = aux;
			}
		}
	}
}

void CNode::scheduleTasks()
{
	time_t startDate = time(0), endDate;
	sortTasksByDeadline();

	for (int i = 0; i < tasks.size(); i++)
		tasks[i]->scheduleTask(timetable, capacity);
}

void CNode::print()
{
	cout << "Node:" << name << endl;
	for (int i = 0; i < tasks.size(); i++)
		tasks[i]->print();
	cout << endl;
	timetable->print();
	cout << endl;
}
CNode::~CNode()
{
}
