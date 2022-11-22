//#include "QUANTLIB.h"
#include <ql/settings.hpp>
#include <iostream>
//#include "boost/format.hpp"
#include <limits>
#include <ql/time/date.hpp>
#include <ql/time/daycounters/all.hpp>
#include "time/daycounter.hpp"
#include "time/calendar.hpp"
//#include <vector>
////#include <ql/time/calendar.hpp>
//#include <ql/time/daycounter.hpp>
//#include <time.h>
//#include <map>
//
//#include "time/calendar.hpp"

//#include <ql/time/daycounters/all.hpp>
#include <ql/time/calendars/all.hpp>

using namespace QuantLib;

int main() {
	/*std::string dayCountName = "Custom one";
	float yearDenominator = 360.0f;
	bool includeLast = false;
	DayCounter dayCounter = CustomDayCounter(dayCountName, includeLast);

	std::cout << dayCounter.dayCount(Date(1, Jan, 2021), Date(1, Jan, 2022)) << "\n";
	std::cout << dayCounter.yearFraction(Date(1, Jan, 2021), Date(1, Jan, 2022)) << "\n";*/
	std::string name = "name";
	std::string weekend = "0";

	CustomCalendar calendar1(name, weekend);

	calendar1.addHoliday(QuantLib::Date(2, April, 2021));
	calendar1.addHoliday(QuantLib::Date(3, April, 2021));
	calendar1.addHoliday(QuantLib::Date(4, April, 2021));
	
	std::string dayCountFunction = "function dayCount (Day1, Day2, Month1, Month2, Year1, Year2) "
		"    return businessDaysBetween(Day1, Day2, Month1, Month2, Year1, Year2)"
		" end";

	std::string yearFracFunction = "function yearFraction (Day1, Day2, Month1, Month2, Year1, Year2)"
		"    return dayCount(Day1, Day2, Month1, Month2, Year1, Year2) / 360"
		" end";

	std::string dayCountName = "Custom one";
	CustomDayCounter dayCounter = CustomDayCounter(dayCountName, dayCountFunction, yearFracFunction, calendar1);

	QuantLib::Date date1 = QuantLib::Date(1, QuantLib::Month::Apr, 2022);
	QuantLib::Date date2 = QuantLib::Date(1, QuantLib::Month::Apr, 2021);

	std::cout << calendar1.businessDaysBetween(date2, date1, false, false) << "\n";
	std::cout << dayCounter.dayCount(date2, date1) << "\n";


	return 0;
}