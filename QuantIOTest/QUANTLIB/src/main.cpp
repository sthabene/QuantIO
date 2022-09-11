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

	std::vector<QuantLib::Date> holidays = customCal.holidayList(Date(1, Dec, 2022), Date(31, Dec, 2022));

	//Date(1, 12, 2022);????

	SouthAfrica saCal;

	std::vector<QuantLib::Date> saHolidays = saCal.holidayList(Date(1, Jan, 2000), Date(31, Dec, 2100), false);
	for (Date date : saHolidays) {
		std::cout << date.dayOfMonth() << " " << date.month() << " " << date.year() << ",";
		if (date.dayOfMonth() == 1 && date.month() == Jan) {
			std::cout << " New Year's Day";
		}
		if (date.dayOfMonth() == 2 && date.month() == Jan && date.weekday() == Mon) {
			std::cout << " New Year's Day Observed";
		};
		if (date.dayOfMonth() == 21 && date.month() == Mar) {
			std::cout << " Human Rights Day";
		};
		if (date.dayOfMonth() == 22 && date.month() == Mar && date.weekday() == Mon) {
			std::cout << " Human Rights Day Observed";
		};
		if (date.dayOfMonth() == 27 && date.month() == Apr) {
			std::cout << " Freedom Day";
		};
		if (date.dayOfMonth() == 28 && date.month() == May && date.weekday() == Mon) {
			std::cout << " Freedom Day Observed";
		};
		if (date.dayOfMonth() == 1 && date.month() == May) {
			std::cout << " Workers Day";
		};
		if (date.dayOfMonth() == 2 && date.month() == May && date.weekday() == Mon) {
			std::cout << " Workers Day Observed";
		};
		if (date.dayOfMonth() == 16 && date.month() == Jun) {
			std::cout << " Youth Day";
		};
		if (date.dayOfMonth() == 17 && date.month() == Jun && date.weekday() == Mon) {
			std::cout << " Youth Day Observed";
		};
		if (date.dayOfMonth() == 9 && date.month() == Aug) {
			std::cout << " National Women's Day";
		};
		if (date.dayOfMonth() == 10 && date.month() == Aug && date.weekday() == Mon) {
			std::cout << " National Women's Day Observed";
		};
		if (date.dayOfMonth() == 24 && date.month() == Sep) {
			std::cout << " Heritage Day";
		};
		if (date.dayOfMonth() == 25 && date.month() == Sep && date.weekday() == Mon) {
			std::cout << " Heritage Day Observed";
		};
		if (date.dayOfMonth() == 16 && date.month() == Dec) {
			std::cout << " Day of Reconciliation";
		};
		if (date.dayOfMonth() == 17 && date.month() == Dec && date.weekday() == Mon) {
			std::cout << " Day of Reconciliation Observed";
		};
		if (date.dayOfMonth() == 25 && date.month() == Dec) {
			std::cout << " Christmas Day";
		};
		if (date.dayOfMonth() == 26 && date.month() == Dec && date.weekday() == Mon) {
			std::cout << " Christmas Day Observed";
		};
		if (date.dayOfMonth() == 26 && date.month() == Dec) {
			std::cout << " Day of Goodwill";
		};
		if (date.dayOfMonth() == 27 && date.month() == Dec && date.weekday() == Mon) {
			std::cout << " Day of Goodwill Observed";
		};
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