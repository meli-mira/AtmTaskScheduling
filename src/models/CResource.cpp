#include "../../include/models/CResource.h"
#include "../../include/models/CTask.h"
int CResource::id = 1;

CResource::CResource(string name, int capacity)
{
	this->name = name;
	this->resource_id = to_string(id);
	id++;
	this->capacity = capacity;

	timetable = new CTimetable(time(0));
}

CResource::CResource(string resource_id, string name, int capacity, int timetable_id)
{
	this->name = name;
	this->resource_id = resource_id;
	this->capacity = capacity;
	this->timetable_id = timetable_id;
}

CResource::~CResource() {}

string CResource::getID() const
{
	return resource_id;
}

string CResource::getName() const
{
	return name;
}

int CResource::getCapacity() const
{
	return capacity;
}

CTimetable *CResource::getTimetable() const
{
	return timetable;
}

int CResource::getTimetableId() const
{
	return timetable_id;
}

bool CResource::isTheResourceAllocated(time_t startTime, time_t endTime)
{
	if (timetable->getNrOfJobsBetween(startTime, endTime) >= 1)
		return true;
	else
		return false;
}

bool CResource::isTheResourceAllocated(int index)
{
	if (timetable->at(index) != 0)
		return true;
	else
		return false;
}

void CResource::setID(int last_id)
{
	id = ++last_id;
}

void CResource::setTimetable(CTimetable *timetable)
{
	this->timetable = timetable;
}

void CResource::setTheResourceOcupied(CTask *t, time_t startTime, time_t endTime)
{
	timetable->setOcupied(startTime, endTime);
	alocare.push_back(make_pair(t, make_pair(startTime, endTime)));
}

void CResource::unsetsetTheResourceOcupied(CTask *t, time_t startTime, time_t endTime)
{
	timetable->unsetOcupied(startTime, endTime);
	for (int i = 0; i < alocare.size(); i++)
	{
		if (alocare[i].first->getID() == t->getID())
			alocare.erase(alocare.begin() + i);
	}
}

void CResource::print()
{
	cout << "Resource:" << name << endl;
	timetable->print();
	cout << endl;
}