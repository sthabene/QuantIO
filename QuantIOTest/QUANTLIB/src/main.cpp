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

#include "time/calendar.hpp"

using namespace QuantLib;

int main() {

	std::string calendarName = "custom calendar";
	std::string calendarWeekends = "17";
	static ext::shared_ptr<CustomCalendar> customCalendar(new CustomCalendar(calendarName, calendarWeekends));
	std::vector<std::string> holidays = { "2022-12-12",  "2022-12-16",  "2025-12-31" };
	
	customCalendar->addHolidays(holidays);
	
	std::cout << customCalendar->isBusinessDay(Date(12, Dec, 2022)) << "\n";
	std::cout << customCalendar->isBusinessDay(Date(25, Dec, 2024)) << "\n";
	std::cout << customCalendar->isBusinessDay(Date(31, Dec, 2025)) << "\n";

	customCalendar->resetAddedAndRemovedHolidays();

	std::cout << customCalendar->isBusinessDay(Date(12, Dec, 2022)) << "\n";
	std::cout << customCalendar->isBusinessDay(Date(25, Dec, 2024)) << "\n";
	std::cout << customCalendar->isBusinessDay(Date(31, Dec, 2025)) << "\n";

	customCalendar->addHolidays(holidays);

	std::cout << customCalendar->isBusinessDay(Date(12, Dec, 2022)) << "\n";
	std::cout << customCalendar->isBusinessDay(Date(25, Dec, 2024)) << "\n";
	std::cout << customCalendar->isBusinessDay(Date(31, Dec, 2025)) << "\n";

	return 0;
}