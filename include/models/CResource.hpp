#pragma once
#include "../CUtils.hpp"
#include "CTimetable.hpp"
class CResource
{
private:
	static int id;
	string resource_id;

	string name;
	int capacity;

	vector<pair<class CTask *, pair<time_t, time_t>>> alocare;

	CTimetable *timetable;
	int timetable_id;

public:
	CResource(string name, int capacity);
	CResource(string resource_id, string name, int capacity, int timetable_id);

	~CResource();

	string getID() const;
	string getName() const;
	int getCapacity() const;
	CTimetable *getTimetable() const;
	int getTimetableId() const;

	void addAlocare(class CTask *t, time_t startTime, time_t endTime);

	bool isTheResourceAllocated(time_t startTime, time_t endTime);
	bool isTheResourceAllocated(int index);

	static void setID(int last_id);
	void setTimetable(CTimetable *timetable);
	void setTheResourceOcupied(class CTask *t, time_t startTime, time_t endTime);
	void unsetsetTheResourceOcupied(class CTask *t, time_t startTime, time_t endTime);

	vector<class CTask *> getTasksThatUseResourceBetween(time_t startDate, time_t endDate);

	void print();
};
