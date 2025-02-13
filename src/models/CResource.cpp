#include "../../include/models/CResource.hpp"
#include "../../include/models/CTask.hpp"
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

CResource::~CResource()
{
	if (timetable != NULL)
		delete timetable;
}

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

void CResource::addAlocare(class CTask *t, time_t startTime, time_t endTime)
{
	alocare.push_back(make_pair(t, make_pair(startTime, endTime)));
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

vector<CTask *> CResource::getTasksThatUseResourceBetween(time_t startDate, time_t endDate)
{
	vector<CTask *> v;
	if (timetable->getNrOfJobsBetween(startDate, endDate) >= 1)
	{
		std::cerr << alocare.size();
		for (int i = 0; i < alocare.size(); i++)
		{
			// There is no intersection if plannedTaskEndDate < startDate or plannedTaskStartDate > endDate
			if (!(CUtils::compareDates_(alocare[i].second.second, startDate) == true || CUtils::compareDates_(endDate, alocare[i].second.first) == true))
				v.push_back(alocare[i].first);
		}
	}
	return v;
}

void CResource::print()
{
	cout << "Resource:" << name << endl;
	timetable->print();
	cout << endl;
}