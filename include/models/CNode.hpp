#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "CTask.hpp"
#include "CTimetable.hpp"

using namespace std;
class CNode
{
private:
	static int id;
	string node_id;

	string name;
	int capacity;
	int minPriority;
	int maxPriority;

	string parent_node_id;
	CNode *parentNode;
	vector<CNode *> childNodes;
	vector<CTask *> tasks;

	int level;

	CTimetable *timetable;
	int timetable_id;

public:
	CNode(string name, int minPriority, int maxPriority, string parent_node_id, int capacity);
	CNode(string id, string name, int minPriority, int maxPriority, string parent_node_id, int capacity, int timetable_id);

	void addChildrenNode(CNode *node);

	void addTask(CTask *t);
	bool deleteTask(string task_id);

	CTask *getTask(string task_id) const;
	vector<CTask *> getTasks() const;
	vector<CTask *> getTasksBetween(time_t startDate, time_t endDate) const;

	int getLevel() const;
	int getCapacity() const;
	string getID() const;
	string getParentNodeID() const;
	string getName() const;
	int getTimetableId() const;
	CTimetable *getTimetable() const;
	int getMinPriority() const;
	int getMaxPriority() const;
	vector<CNode *> getChildren() const;
	CNode *getParentNode() const;
	void setTimetable(CTimetable *t);
	static void setID(int last_id);

	void sortTasksByDeadline();

	void scheduleTasks();
	void print();

	~CNode();
};
