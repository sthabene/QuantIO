#include "QUANTLIB.h"
#include "boost/format.hpp"

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

	
;

	std::cout << boost::format(zarCurrency.format()) % "100.8377664" % "124" % zarCurrency.code();

	return 0;
}