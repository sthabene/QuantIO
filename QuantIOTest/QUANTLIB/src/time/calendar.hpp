//Disable warnings
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#pragma warning(disable : 4996)
#pragma once

#include <ql/time/calendar.hpp>

class CustomCalendar : public QuantLib::Calendar {
public:
	class CustomCalendarImpl : public QuantLib::Calendar::Impl { //Class inheritance is private by default
	public:
		std::string m_calendarName;
		std::string m_weekends;
		std::string name() const override;
		bool isBusinessDay(const QuantLib::Date&) const override;
		bool isWeekend(QuantLib::Weekday w) const override;
		CustomCalendarImpl(std::string& name, std::string& weekends) : m_calendarName(name), m_weekends(weekends) {};
	};
public:
	void addHolidays(std::vector<std::string>& holidays);
	void removeHolidays(std::vector<std::string>& holidays);

	void setAnotherCalendar(std::string& name, std::string& weekends);

	CustomCalendar(std::string& name, std::string& weekends);
	CustomCalendar(const CustomCalendar& customCalendar);
	CustomCalendar() = default;
};


static tm CreateDate(int day, int month, int year) {
	struct tm tm = { 0 };
	tm.tm_isdst = -1;
	tm.tm_mday = day;
	tm.tm_mon = month - 1;
	tm.tm_year = year - 1900;
	return tm;
};

static tm CreateDateNow() {
	time_t now = time(NULL);
	tm today = *localtime(&now);
	today.tm_isdst = -1;
	//today.tm_year += 1900;
	return today;
};

static QuantLib::Month ConvertToQlMonth(int intMonth) {
	QuantLib::Month month;
	switch (intMonth) {
	case 0:
		month = QuantLib::Month::January;
		break;
	case 1:
		month = QuantLib::Month::February;
		break;
	case 2:
		month = QuantLib::Month::March;
		break;
	case 3:
		month = QuantLib::Month::April;
		break;
	case 4:
		month = QuantLib::Month::May;
		break;
	case 5:
		month = QuantLib::Month::June;
		break;
	case 6:
		month = QuantLib::Month::July;
		break;
	case 7:
		month = QuantLib::Month::August;
		break;
	case 8:
		month = QuantLib::Month::September;
		break;
	case 9:
		month = QuantLib::Month::October;
		break;
	case 10:
		month = QuantLib::Month::November;
		break;
	case 11:
		month = QuantLib::Month::December;
		break;
	default:
		month = QuantLib::Month::December;
		break;
	}
	return month;
}

static QuantLib::Date ConvertToQlDate(tm& date) {
	QuantLib::Month month = ConvertToQlMonth(date.tm_mon);
	return QuantLib::Date(date.tm_mday, month, date.tm_year + 1900);
};

static QuantLib::Date ConvertToQlDate(std::string& date) {
	std::string mon_str = date.substr(5, 2);
	mon_str.erase(0, mon_str.find_first_not_of('0'));
	int mon = std::stoi(mon_str);
	QuantLib::Month month = ConvertToQlMonth(mon - 1);

	std::string day_str = date.substr(8, 2);
	day_str.erase(0, day_str.find_first_not_of('0'));
	int day = std::stoi(day_str);

	std::string yr_str = date.substr(0, 4);
	int yr = std::stoi(yr_str);

	return QuantLib::Date(day, month, yr);
};

static int ConvertToMonth(QuantLib::Month& month) {
	int mon = 0;
	switch (month) {
	case QuantLib::Month::January:
		mon = 1;
		break;
	case QuantLib::Month::February:
		mon = 2;
		break;
	case QuantLib::Month::March:
		mon = 3;
		break;
	case QuantLib::Month::April:
		mon = 4;
		break;
	case QuantLib::Month::May:
		mon = 5;
		break;
	case QuantLib::Month::June:
		mon = 6;
		break;
	case QuantLib::Month::July:
		mon = 7;
		break;
	case QuantLib::Month::August:
		mon = 8;
		break;
	case QuantLib::Month::September:
		mon = 9;
		break;
	case QuantLib::Month::October:
		mon = 10;
		break;
	case QuantLib::Month::November:
		mon = 11;
		break;
	case QuantLib::Month::December:
		mon = 12;
		break;
	default:
		mon = 1;
		break;
	}
	return mon;
}

static tm ConvertToTm(QuantLib::Date& date) {
	int mday = date.dayOfMonth();
	int year = date.year();
	int mon = 1;

	QuantLib::Month month = date.month();
	mon = ConvertToMonth(month);	
	return CreateDate(mday, mon, year);
};