#include "../../include/serializers/TimetableSerializer.hpp"

CTimetable *TimetableSerializer::fromJson(const boost::json::object &obj)
{
    int timetable_id = stoi(obj.at("timetable_id").as_string().c_str());
    string startDateOfCalendar = obj.at("startdateofcalendar").as_string().c_str();
    int nr_days = stoi(obj.at("days").as_string().c_str());
    int year[367] = {0}, i = 1;

    string year_mapping = obj.at("year_mapping").as_string().c_str();
    year_mapping.erase(year_mapping.begin());
    year_mapping.erase(year_mapping.begin() + year_mapping.size() - 1);

    istringstream iss(year_mapping);
    string token;
    while (getline(iss, token, ','))
        year[i++] = stoi(token);

    CTimetable *t = new CTimetable(timetable_id, CUtils::parseDateTime(startDateOfCalendar.c_str(), "%Y-%m-%d"), year, nr_days);
    return t;
}