#pragma once
#include "../CUtils.h"
#define MAX_CAPACITY 3
class CTimetable
{
private:
	static int id;

	int timetable_id;
	int year[367];//1 366
	int nrDays;
	time_t startDateOfCalendar;
	int ian, feb, mar, apr, may, iun, iul, aug, sep, oct, nov, dec;

public:
	CTimetable(time_t startDateOfCalendar);
	CTimetable(int timetable_id, time_t startDateOfCalendar, int year[367], int nrDays);

	void setOcupied(time_t startDate, time_t endDate);
	void unsetOcupied(time_t startDate, time_t endDate);
	void setDayOff(time_t date);
	void print();

	int getNrOfJobsBetween(time_t startDate, time_t endDate) const;
	int getIndexForDate(time_t date) const;
	time_t getDateFromIndex(int index) const;
	time_t getStartDateOfCalendar() const;
	int getNrOfDays() const;
	int getID() const;
	vector<int> getYear() const;

	int at(int i);
	static void setID(int last_id);
	~CTimetable();
};
