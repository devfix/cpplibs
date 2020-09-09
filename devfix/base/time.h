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
			t.hour = tm->tm_hour;
			//if (tm->tm_isdst == 1) { t.hour++; }
			//t.hour %= 24;
			t.minute = tm->tm_min;
			t.second = tm->tm_sec;
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
