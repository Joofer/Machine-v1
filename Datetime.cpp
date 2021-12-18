#include "libs/Datetime.h"

string Datetime::GetSec()
{
	char out[MAXTIMECHARS];

	struct tm _time;
	time_t _time_now = time(0);
	localtime_s(&_time, &_time_now);
	strftime(out, sizeof(out), "%S", &_time);

	return out;
}

string Datetime::GetMin()
{
	char out[MAXTIMECHARS];

	struct tm _time;
	time_t _time_now = time(0);
	localtime_s(&_time, &_time_now);
	strftime(out, sizeof(out), "%M", &_time);

	return out;
}

string Datetime::GetHour()
{
	char out[MAXTIMECHARS];

	struct tm _time;
	time_t _time_now = time(0);
	localtime_s(&_time, &_time_now);
	strftime(out, sizeof(out), "%H", &_time);

	return out;
}

string Datetime::GetDay()
{
	char out[MAXTIMECHARS];

	struct tm _time;
	time_t _time_now = time(0);
	localtime_s(&_time, &_time_now);
	strftime(out, sizeof(out), "%d", &_time);

	return out;
}

string Datetime::GetMonth()
{
	char out[MAXTIMECHARS];

	struct tm _time;
	time_t _time_now = time(0);
	localtime_s(&_time, &_time_now);
	strftime(out, sizeof(out), "%m", &_time);

	return out;
}

string Datetime::GetYear()
{
	char out[MAXTIMECHARS];

	struct tm _time;
	time_t _time_now = time(0);
	localtime_s(&_time, &_time_now);
	strftime(out, sizeof(out), "%Y", &_time);

	return out;
}
