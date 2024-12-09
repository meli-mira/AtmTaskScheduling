#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "CTask.h"
#include "CNotification.h"
#include "CTimetable.h"

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
	vector<CNotification *> notifications;

	int level;

	CTimetable *timetable;
	int timetable_id;

public:
	CNode(string name, int minPriority, int maxPriority, string parent_node_id, int capacity);
	CNode(string id, string name, int minPriority, int maxPriority, string parent_node_id, int capacity, int timetable_id);

	void addChildrenNode(CNode *node);

	void addTask(CTask *t);
	void addNotification(CNotification *n);

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

	void readTasksFromFile(string filename);

	vector<CTask *> getTasks() const;

	void sortTasksByDeadline();

	void scheduleTasks();
	void print();

	~CNode();
};
