#include "QUANTLIB.h"
#include <ql/settings.hpp>
#include "boost/format.hpp"
#include <limits>

using namespace QuantLib;

int main() {

	ZARCurrency zarCurrency;

	//Date veryFirstDate(1, January, 1901);
	//Date anotherVeryFirstDate(367);
	//Date yetAnotherVeryFirstDate(veryFirstDate);
	//std::cout << (Date(10, December, 1979)).weekday() << "\n"; //Monday
	//std::cout << veryFirstDate << "\n"; //January 1st, 1901
	//std::cout << anotherVeryFirstDate << "\n"; //January 1st, 1901
	//std::cout << yetAnotherVeryFirstDate << "\n"; //January 1st, 1901
	//std::cout << Date::minDate() << "\n"; //January 1st, 1901

	//std::cout << Date::maxDate() << "\n"; //December 31st, 2199

	Date today = Date::todaysDate();
	Settings::instance().evaluationDate() = today;
	Settings::instance().anchorEvaluationDate(); //Sets evaluation date to today
	

	return 0;
}