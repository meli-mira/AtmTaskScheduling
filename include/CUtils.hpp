#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <ctime>
#include <time.h>
#include <string>
#include <unordered_map>
#include <map>

using namespace std;

class CUtils
{
public:
    static string time_to_string(time_t t)
    {
        stringstream ss;
        ss << t;
        return ss.str();
    }
    // function to parse a date or time string.
    static time_t parseDateTime(const char *datetimeString, const char *format)
    {
        struct tm tmStruct = {};
        istringstream ss(datetimeString);
        ss >> get_time(&tmStruct, format);
        return mktime(&tmStruct);
    }

    // Function to format a time_t value into a date or time string.
    static string dateToString(time_t time, const char *format)
    {
        char buffer[90];
        struct tm timeinfo;
        localtime_r(&time, &timeinfo);

        strftime(buffer, sizeof(buffer), format, &timeinfo);
        return buffer;
    }
    static double getDaysBetween(time_t x, time_t y)
    {
        double difference = -1;
        if (x != (std::time_t)(-1) && y != (std::time_t)(-1))
        {
            difference = std::difftime(y, x) / (60 * 60 * 24);
        }

        return difference;
    }

    static time_t addDays(time_t date, int days)
    {
        date += (60 * 60 * 24 * (days));
        return date;
    }

    static time_t substractDays(time_t date, int days)
    {
        date -= (60 * 60 * 24 * days);
        return date;
    }
    static int compareDates(time_t date1, time_t date2) // true if date1<=date2
    {
        struct tm timeinfo1, timeinfo2;
        localtime_r(&date1, &timeinfo1);
        localtime_r(&date2, &timeinfo2);

        if (timeinfo1.tm_year < timeinfo2.tm_year)
            return true;
        else if (timeinfo1.tm_year == timeinfo2.tm_year && timeinfo1.tm_mon < timeinfo2.tm_mon)
            return true;
        else if (timeinfo1.tm_year == timeinfo2.tm_year && timeinfo1.tm_mon == timeinfo2.tm_mon && timeinfo1.tm_mday <= timeinfo2.tm_mday)
            return true;
        return false;
    }
    static bool compareDates_(time_t date1, time_t date2) // true if date1<date2
    {
        struct tm timeinfo1, timeinfo2;
        localtime_r(&date1, &timeinfo1);
        localtime_r(&date2, &timeinfo2);

        if (timeinfo1.tm_year < timeinfo2.tm_year)
            return true;
        else if (timeinfo1.tm_year == timeinfo2.tm_year && timeinfo1.tm_mon < timeinfo2.tm_mon)
            return true;
        else if (timeinfo1.tm_year == timeinfo2.tm_year && timeinfo1.tm_mon == timeinfo2.tm_mon && timeinfo1.tm_mday < timeinfo2.tm_mday)
            return true;
        return false;
    }
};
namespace util
{

    struct string_hash
    {
        using is_transparent = void;
        [[nodiscard]] size_t operator()(const char *txt) const
        {
            return std::hash<std::string_view>{}(txt);
        }
        [[nodiscard]] size_t operator()(std::string_view txt) const
        {
            return std::hash<std::string_view>{}(txt);
        }
        [[nodiscard]] size_t operator()(const std::string &txt) const
        {
            return std::hash<std::string>{}(txt);
        }
    };

}
