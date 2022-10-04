#include "QUANTLIB.h"
#include <ql/settings.hpp>
#include "boost/format.hpp"
#include <limits>
#include <ql/time/date.hpp>
#include <vector>
#include <ql/time/calendars/all.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/daycounters/all.hpp>
#include <time.h>
#include <map>

#include "time/calendar.hpp"
#include "time/daycounter.hpp"

using namespace QuantLib;

int main() {
	std::string dayCountName = "Custom one";
	float yearDenominator = 360.0f;
	bool includeLast = false;
	DayCounter dayCounter = CustomDayCounter(dayCountName, yearDenominator, includeLast);

	std::cout << dayCounter.dayCount(Date(1, Jan, 2021), Date(1, Jan, 2022)) << "\n";
	std::cout << dayCounter.yearFraction(Date(1, Jan, 2021), Date(1, Jan, 2022)) << "\n";

	return 0;
}