//
// Created by core on 6/9/20.
//

#pragma once

#include <ctime>

namespace devfix::base
{
	struct time
	{
		static time get_tm(std::tm* tm)
		{
			time t{};
			t.year = tm->tm_year + 1900;
			t.month = tm->tm_mon + 1;
			t.day = tm->tm_mday;
			if (tm->tm_isdst == 1) { t.hour = (tm->tm_hour + 23) % 24 + 1; }
			else { t.hour = tm->tm_hour + 1; }
			t.minute = tm->tm_min + 1;
			t.second = tm->tm_sec + 1;
			return t;
		}

		int second;
		int minute;
		int hour;
		int day;
		int month;
		int year;
	};
}