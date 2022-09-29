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

using namespace QuantLib;

int main() {

	DayCounter dayCounter = ActualActual(ActualActual::ISDA);
	Date::serial_type result = dayCounter.dayCount(Date(1, Jan, 2021), Date(31, Dec, 2021));

	std::cout << result;

	return 0;
}