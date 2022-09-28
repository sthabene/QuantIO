#include <ql/time/calendar.hpp>

using namespace QuantLib;

class CustomCalendar : public Calendar {
public:
	class CustomCalendarImpl : public Calendar::Impl { //Class inheritance is private by default
	public:
		std::string m_calendarName;
		std::string m_weekends;
		std::string name() const override;
		bool isBusinessDay(const Date&) const override;
		bool isWeekend(Weekday w) const override;
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

static QuantLib::Date ConvertToQlDate(tm& date) {
	QuantLib::Month month;
	switch (date.tm_mon) {
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
	return QuantLib::Date(date.tm_mday, month, date.tm_year + 1900);
};

static QuantLib::Date ConvertToQlDate(std::string& date) {
	std::string mon_str = date.substr(5, 2);
	mon_str.erase(0, mon_str.find_first_not_of('0'));
	int mon = std::stoi(mon_str);

	std::string day_str = date.substr(8, 2);
	day_str.erase(0, day_str.find_first_not_of('0'));
	int day = std::stoi(day_str);

	std::string yr_str = date.substr(0, 4);
	int yr = std::stoi(yr_str);

	QuantLib::Month month;
	switch (mon) {
	case 1:
		month = QuantLib::Month::January;
		break;
	case 2:
		month = QuantLib::Month::February;
		break;
	case 3:
		month = QuantLib::Month::March;
		break;
	case 4:
		month = QuantLib::Month::April;
		break;
	case 5:
		month = QuantLib::Month::May;
		break;
	case 6:
		month = QuantLib::Month::June;
		break;
	case 7:
		month = QuantLib::Month::July;
		break;
	case 8:
		month = QuantLib::Month::August;
		break;
	case 9:
		month = QuantLib::Month::September;
		break;
	case 10:
		month = QuantLib::Month::October;
		break;
	case 11:
		month = QuantLib::Month::November;
		break;
	case 12:
		month = QuantLib::Month::December;
		break;
	default:
		month = QuantLib::Month::December;
		break;
	}

	return QuantLib::Date(day, month, yr);
};