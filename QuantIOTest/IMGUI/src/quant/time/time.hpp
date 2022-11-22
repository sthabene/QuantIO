#pragma once

#include "quant.hpp"
#include "time/daycounter.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"

inline CustomCalendar createCalendar(std::string& calendarId) {
	
	if (calendarId == "" || calendarId == "NULL") {
		std::string calendarName = "NULL";
		std::string weekend = "17";
		CustomCalendar customCalendar(calendarName, weekend);
		return customCalendar;
	}
	else {
		boost::format calendarQuery = boost::format("SELECT T1.CALENDAR_ID, T1.CALENDAR_LABEL, T1.REGION, T1.MARKET, T1.WEEKEND FROM CALENDAR T1 WHERE T1.CALENDAR_ID = %1%") % calendarId;

		boost::format holidaysQuery = boost::format("SELECT DATE FROM HOLIDAYS WHERE CALENDAR = %1% AND DATE BETWEEN DATE('%2%-01-01') AND DATE('%3%-12-31') UNION SELECT DATE FROM HOLIDAYS_ADHOC WHERE CALENDAR = %1% AND DATE BETWEEN DATE('%2%-01-01') AND DATE('%3%-12-31')") % calendarId % 2000 % 2050;

		std::vector<std::string> calendarVector = QuantIO::dbConnection.getTableData2(calendarQuery.str(), false, false)[0];
		std::vector<std::vector<std::string>> holidays = QuantIO::dbConnection.getTableData2(holidaysQuery.str(), false, true);

		CustomCalendar customCalendar(calendarVector[1], calendarVector[4]);

		if (holidays.size() > 0) {
			customCalendar.addHolidays(holidays[0]);
		}

		return customCalendar;
	}
}

inline CustomDayCounter createDayCounter(std::string& dayCounterId, std::string& calendarId) {

	CustomCalendar calendar = createCalendar(calendarId);

	boost::format dayCounterQuery = boost::format("SELECT COUNTER_ID, COUNTER_NAME, COUNTER_DESCR, DAY_COUNT, YEAR_FRAC FROM DAY_COUNTER WHERE COUNTER_ID = %1%") % dayCounterId;
	std::vector<std::string> dayCounterVector = QuantIO::dbConnection.getTableData2(dayCounterQuery.str(), false, false)[0];

	CustomDayCounter customDayCounter(dayCounterVector[1], dayCounterVector[3], dayCounterVector[4], calendar);

	return customDayCounter;
}