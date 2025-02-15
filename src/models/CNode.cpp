#include "../../include/models/CNode.hpp"
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
	timetable_id = timetable->getID();
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

bool CNode::deleteTask(string task_id)
{
	for (int i = 0; i < tasks.size(); i++)
	{
		if (tasks[i]->getID() == task_id)
		{
			// If the task has been planned, unschedule
			if (tasks[i]->getHasBeenPlanned() == true)
				tasks[i]->unscheduleTask(this->timetable);

			delete tasks[i];
			tasks.erase(tasks.begin() + i);
			return true;
		}
	}
	return false;
}

CTask *CNode::getTask(string task_id) const
{
	for (int i = 0; i < tasks.size(); i++)
		if (tasks[i]->getID() == task_id)
			return tasks[i];
	return NULL;
}

vector<CTask *> CNode::getTasks() const
{
	return tasks;
}

vector<CTask *> CNode::getTasksBetween(time_t startDate, time_t endDate) const
{
	vector<CTask *> v;
	if (timetable->getNrOfJobsBetween(startDate, endDate) >= 1)
	{
		for (int i = 0; i < tasks.size(); i++)
		{
			// There is no intersection if plannedTaskEndDate < startDate or plannedTaskStartDate > endDate
			if (tasks[i]->getHasBeenPlanned() && !(CUtils::compareDates_(tasks[i]->getEndDate(), startDate) == true || CUtils::compareDates_(endDate, tasks[i]->getStartDate()) == true))
				v.push_back(tasks[i]);
		}
	}
	return v;
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
	sortTasksByDeadline();

	for (int i = 0; i < tasks.size(); i++)
	{
		if (tasks[i]->getHasBeenPlanned() == false)
			tasks[i]->scheduleTask(timetable, capacity);
	}
}

void CNode::unscheduleTasks()
{
	for (int i = 0; i < tasks.size(); i++)
		tasks[i]->unscheduleTask(this->timetable);
}

void CNode::unscheduleTasks(time_t startDate, time_t endDate)
{
	for (int i = 0; i < tasks.size(); i++)
	{
		// There is no intersection if plannedTaskEndDate < startDate or plannedTaskStartDate > endDate
		if (tasks[i]->getHasBeenPlanned() && !(CUtils::compareDates_(tasks[i]->getEndDate(), startDate) == true || CUtils::compareDates_(endDate, tasks[i]->getStartDate()) == true))
			tasks[i]->unscheduleTask(this->timetable);
	}
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
	for (int i = 0; i < childNodes.size(); i++)
		delete childNodes[i];
	for (int i = 0; i < tasks.size(); i++)
		delete tasks[i];

	childNodes.clear();
	tasks.clear();
	
	if (timetable != NULL)
		delete timetable;
}
