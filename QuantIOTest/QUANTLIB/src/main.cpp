#include "QUANTLIB.h"
#include <ql/settings.hpp>
#include "boost/format.hpp"
#include <limits>
#include <ql/time/date.hpp>
#include <vector>
#include <ql/time/calendars/all.hpp>
#include <ql/time/calendar.hpp>
#include <time.h>
#include <map>

using namespace QuantLib;

class CustomCal : public Calendar {
public:
	class CustomCalWesternImpl : public Calendar::WesternImpl { //Class inheritance is private by default
	public:
		std::string name() const override;
		bool isBusinessDay(const Date&) const override;
	};
	class CustomCalOrthodoxImpl : public Calendar::OrthodoxImpl {
	public:
		std::string name() const override;
		bool isBusinessDay(const Date&) const override;
	};
public:
	enum ImplType { Western,  Orthodox };
	CustomCal(ImplType = Western);
};

//We need to override every function 
CustomCal::CustomCal(CustomCal::ImplType implType) {
	static ext::shared_ptr<Calendar::Impl> westernImpl(new CustomCal::CustomCalWesternImpl);
	static ext::shared_ptr<Calendar::Impl> orthodoxImpl(new CustomCal::CustomCalOrthodoxImpl);

	switch (implType) {
	case Western:
		impl_ = westernImpl;
		break;
	case Orthodox:
		impl_ = orthodoxImpl;
		break;
	default:
		QL_FAIL("unknown implementation");
	}
};

std::string CustomCal::CustomCalWesternImpl::name() const { 
	return "Any Name";
};
bool CustomCal::CustomCalWesternImpl::isBusinessDay(const Date& date) const {
	return !isWeekend(date.weekday());
};

std::string CustomCal::CustomCalOrthodoxImpl::name() const { //
	return "Any Name";
};
bool CustomCal::CustomCalOrthodoxImpl::isBusinessDay(const Date& date) const {
	return !isWeekend(date.weekday());
};

tm GetDate(int day, int month, int year) {
	struct tm tm = {0};
	tm.tm_isdst = -1;
	tm.tm_mday = day;
	tm.tm_mon = month - 1;
	tm.tm_year = year - 1900;
	return tm;
};


int main() {

	CustomCal customCal(CustomCal::ImplType::Western);

	customCal.addHoliday(Date(16, Dec, 2022));
	customCal.name();

	//std::vector<QuantLib::Date> holidays = customCal.holidayList(Date(1, Dec, 2022), Date(31, Dec, 2022));

	//Date(1, 12, 2022);????

	Argentina saCal;

	std::vector<QuantLib::Date> saHolidays = saCal.holidayList(Date(1, Jan, 2000), Date(31, Dec, 2100), false);
	for (Date date : saHolidays) {
		Weekday w = date.weekday();
		Day d = date.dayOfMonth(), dd = date.dayOfYear();
		Month m = date.month();
		Year y = date.year();
		Day em = CustomCal::CustomCalWesternImpl::easterMonday(y);
		std::cout << date.year() << "-" << date.month() << "-" << date.dayOfMonth() << ",";
		{
			if (d == 1 && m == January) {
				std::cout << " New Year's Day";
			}
			if (dd == em - 4) {
				std::cout << " Holy Thursday";
			}
			if (dd == em - 3) {
				std::cout << " Good Friday";
			}
			if (d == 1 && m == May) {
				std::cout << " Labour Day";
			}
			if (d == 25 && m == May) {
				std::cout << " May Revolution";
			}
			if (d >= 15 && d <= 21 && w == Monday && m == June) {
				std::cout << " Death of General Manuel Belgrano";
			}
			if (d == 9 && m == July) {
				std::cout << " Independence Day";
			}
			if (d >= 15 && d <= 21 && w == Monday && m == August) {
				std::cout << " Death of General José de San Martín";
			}
			if ((d == 10 || d == 11 || d == 12 || d == 15 || d == 16) && w == Monday && m == October) {
				std::cout << " Columbus Day";
			}
			if (d == 8 && m == December) {
				std::cout << " Immaculate Conception";
			}
			if (d == 24 && m == December) {
				std::cout << " Christmas Eve";
			}
			if ((d == 31 || (d == 30 && w == Friday)) && m == December) {
				std::cout << " New Year's Eve";
			}
		}
		std::cout << "\n";
	}

	std::vector<tm> holdDays;
	tm start = GetDate(1, 1, 2020);
	tm end = GetDate(31, 12, 2022);

	/*for (tm date = start; mktime(&date) <= mktime(&end); ++date.tm_mday) {
		char dateTxt[16];
		strftime(dateTxt, 16, "%d %b %Y", &date);
		if (date.tm_mday == 1 && date.tm_mon + 1 == 1) {
			std::cout << dateTxt << " New Year's Day" << "\n";
		};
		if (date.tm_mday == 2 && date.tm_mon + 1 == 1 && date.tm_wday == 1) {
			std::cout << dateTxt << " New Year's Day Observed" << "\n";
		};
		if (date.tm_mday == 21 && date.tm_mon + 1 == 3) {
			std::cout << dateTxt << " Human Rights Day" << "\n";
		};
		if (date.tm_mday == 22 && date.tm_mon + 1 == 3 && date.tm_wday == 1) {
			std::cout << dateTxt << " Human Rights Day Observed" << "\n";
		};
		if (date.tm_mday == 27 && date.tm_mon + 1 == 4) {
			std::cout << dateTxt << " Freedom Day" << "\n";
		};
		if (date.tm_mday == 28 && date.tm_mon + 1 == 5 && date.tm_wday == 1) {
			std::cout << dateTxt << " Freedom Day Observed" << "\n";
		};
		if (date.tm_mday == 1 && date.tm_mon + 1 == 5) {
			std::cout << dateTxt << " Workers Day" << "\n";
		};
		if (date.tm_mday == 2 && date.tm_mon + 1 == 5 && date.tm_wday == 1) {
			std::cout << dateTxt << " Workers Day Observed" << "\n";
		};
		if (date.tm_mday == 16 && date.tm_mon + 1 == 6) {
			std::cout << dateTxt << " Youth Day" << "\n";
		};
		if (date.tm_mday == 17 && date.tm_mon + 1 == 6 && date.tm_wday == 1) {
			std::cout << dateTxt << " Youth Day Observed" << "\n";
		};
		if (date.tm_mday == 9 && date.tm_mon + 1 == 8) {
			std::cout << dateTxt << " National Women's Day" << "\n";
		};
		if (date.tm_mday == 10 && date.tm_mon + 1 == 8 && date.tm_wday == 1) {
			std::cout << dateTxt << " National Women's Day Observed" << "\n";
		};
		if (date.tm_mday == 24 && date.tm_mon + 1 == 9) {
			std::cout << dateTxt << " Heritage Day" << "\n";
		};
		if (date.tm_mday == 25 && date.tm_mon + 1 == 9 && date.tm_wday == 1) {
			std::cout << dateTxt << " Heritage Day Observed" << "\n";
		};
		if (date.tm_mday == 16 && date.tm_mon + 1 == 12) {
			std::cout << dateTxt << " Day of Reconciliation" << "\n";
		};
		if (date.tm_mday == 17 && date.tm_mon + 1 == 12 && date.tm_wday == 1) {
			std::cout << dateTxt << " Day of Reconciliation Observed" << "\n";
		};
		if (date.tm_mday == 25 && date.tm_mon + 1 == 12) {
			std::cout << dateTxt << " Christmas Day" << "\n";
		};
		if (date.tm_mday == 26 && date.tm_mon + 1 == 12 && date.tm_wday == 1) {
			std::cout << dateTxt << " Christmas Day Observed" << "\n";
		};
		if (date.tm_mday == 26 && date.tm_mon + 1 == 12) {
			std::cout << dateTxt << " Day of Goodwill" << "\n";
		};
		if (date.tm_mday == 27 && date.tm_mon + 1 == 12 && date.tm_wday == 1) {
			std::cout << dateTxt << " Day of Goodwill Observed" << "\n";
		};
	}*/


	std::map<tm, std::string> holidays2;
	


	//std::cout << customCal.isBusinessDay(Date(1, Jan, 2021)) << "\n";
	//Date veryFirstDate(1, January, 1901);
	//Date anotherVeryFirstDate(367);
	//Date yetAnotherVeryFirstDate(veryFirstDate);
	//std::cout << (Date(10, December, 1979)).weekday() << "\n"; //Monday
	//std::cout << veryFirstDate << "\n"; //January 1st, 1901
	//std::cout << anotherVeryFirstDate << "\n"; //January 1st, 1901
	//std::cout << yetAnotherVeryFirstDate << "\n"; //January 1st, 1901
	//std::cout << Date::minDate() << "\n"; //January 1st, 1901
	//std::cout << Date::maxDate() << "\n"; //December 31st, 2199
	//std::cout << "" << "\n";


	return 0;
}