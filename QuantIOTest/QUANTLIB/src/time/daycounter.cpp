#pragma once
#include "daycounter.hpp"
#include "boost/algorithm/string.hpp"

static CustomCalendar customCalendar;

CustomDayCounter::CustomDayCounter(std::string& name, std::string& dayCountFunction, 
	std::string& yearFractionFunction, CustomCalendar& calendar, 
	bool code, bool inclusive, unsigned int dayCount, unsigned int denominator) {
	QuantLib::ext::shared_ptr<QuantLib::DayCounter::Impl> customDayCountImpl(
		new CustomDayCounter::CustomDayCounterImpl(name, dayCountFunction, yearFractionFunction, 
			code, inclusive, dayCount, denominator));
	this->impl_ = customDayCountImpl;
	customCalendar = calendar;
}

CustomDayCounter::CustomDayCounter(const CustomDayCounter& customDayCounter) {
	this->impl_ = customDayCounter.impl_;
	customCalendar = customDayCounter.m_customCalendar;
}

void CustomDayCounter::setAnotherDayCounter(std::string& name, std::string& dayCountFunction,
	std::string& yearFractionFunction, CustomCalendar& calendar, 
	bool code, bool inclusive, unsigned int dayCount, unsigned int denominator) {
	QuantLib::ext::shared_ptr<QuantLib::DayCounter::Impl> customDayCountImpl(
		new CustomDayCounter::CustomDayCounterImpl(name, dayCountFunction, yearFractionFunction, code, inclusive, dayCount, denominator));
	impl_ = customDayCountImpl;
	customCalendar = calendar;
};


std::string CustomDayCounter::CustomDayCounterImpl::name() const {
	return this->m_dayCounterName;
};

QuantLib::Date::serial_type CustomDayCounter::CustomDayCounterImpl::dayCount(const QuantLib::Date& d2, 
	const QuantLib::Date& d1) const {
	
	QuantLib::Date::serial_type dayCount = 0;

	if (this->m_code) {
		dayCount = getDayCount(d1, d2, this->m_dayCountFunction, this->m_yearFractionFunction);
	}
	else {
		switch (this->m_dayCount) {
		case 0: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::BondBasis).dayCount(d2, d1);
			break;
		case 1: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::BondBasis).dayCount(d2, d1);
			break;
		case 2: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::EurobondBasis).dayCount(d2, d1);
			break;
		case 3: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::European).dayCount(d2, d1);
			break;
		case 4: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::German).dayCount(d2, d1);
			break;
		case 5: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::ISDA).dayCount(d2, d1);
			break;
		case 6: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::ISMA).dayCount(d2, d1);
			break;
		case 7: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::Italian).dayCount(d2, d1);
			break;
		case 8: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::NASD).dayCount(d2, d1);
			break;
		case 9: dayCount = QuantLib::Thirty360(QuantLib::Thirty360::Convention::USA).dayCount(d2, d1);
			break;
		case 10: dayCount = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Actual365).dayCount(d2, d1);
			break;
		case 11: dayCount = QuantLib::ActualActual(QuantLib::ActualActual::Convention::AFB).dayCount(d2, d1);
			break;
		case 12: dayCount = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Bond).dayCount(d2, d1);
			break;
		case 13: dayCount = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Euro).dayCount(d2, d1);
			break;
		case 14: dayCount = QuantLib::Actual365Fixed(QuantLib::Actual365Fixed::Convention::Canadian).dayCount(d2, d1);
			break;
		case 15: dayCount = QuantLib::Actual365Fixed(QuantLib::Actual365Fixed::Convention::NoLeap).dayCount(d2, d1);
			break;
		case 16: dayCount = QuantLib::Actual365Fixed(QuantLib::Actual365Fixed::Convention::Standard).dayCount(d2, d1);
			break;
		case 17: dayCount = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Historical).dayCount(d2, d1);
			break;
		case 18: dayCount = QuantLib::ActualActual(QuantLib::ActualActual::Convention::ISDA).dayCount(d2, d1);
			break;
		case 19: dayCount = QuantLib::ActualActual(QuantLib::ActualActual::Convention::ISMA).dayCount(d2, d1);
			break;
		case 20: dayCount = QuantLib::Business252(customCalendar).dayCount(d2, d1);
			break;
		case 21: dayCount = QuantLib::OneDayCounter().dayCount(d2, d1);
			break;
		default:
			dayCount = (int)QuantLib::daysBetween(d2, d1);
			break;
		}
	}

	return dayCount + (this->m_inclusive ? 1 : 0);
}

QuantLib::Time CustomDayCounter::CustomDayCounterImpl::yearFraction(const QuantLib::Date& d2, 
	const QuantLib::Date& d1, const QuantLib::Date&, const QuantLib::Date&) const {

	QuantLib::Time yearFraction = 0;

	if (this->m_code) {
		yearFraction = getYearFraction(d1, d2, this->m_dayCountFunction, this->m_yearFractionFunction);
	}
	else {

		switch (this->m_denominator) {
		case 0:
			yearFraction = QuantLib::Business252(customCalendar).yearFraction(d2, d1);
			break;
		case 1:
			switch (this->m_dayCount) {
			case 0: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::BondBasis).yearFraction(d2, d1);
				break;
			case 1: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::BondBasis).yearFraction(d2, d1);
				break;
			case 2: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::EurobondBasis).yearFraction(d2, d1);
				break;
			case 3: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::European).yearFraction(d2, d1);
				break;
			case 4: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::German).yearFraction(d2, d1);
				break;
			case 5: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::ISDA).yearFraction(d2, d1);
				break;
			case 6: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::ISMA).yearFraction(d2, d1);
				break;
			case 7: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::Italian).yearFraction(d2, d1);
				break;
			case 8: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::NASD).yearFraction(d2, d1);
				break;
			case 9: yearFraction = QuantLib::Thirty360(QuantLib::Thirty360::Convention::USA).yearFraction(d2, d1);
				break;
			default:
				yearFraction = QuantLib::Actual360(this->m_inclusive).yearFraction(d2, d1);
				break;
			}
			break;
		case 2: 
			yearFraction = QuantLib::Actual364().yearFraction(d2, d1);
			break;
		case 3: 
			switch (this->m_dayCount) {
			case 0: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 1: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 2: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 3: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 4: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 5: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 6: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 7: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 8: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 9: yearFraction = QuantLib::Thirty365().yearFraction(d2, d1); break;
			case 10: 
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Actual365).yearFraction(d2, d1);
				break;
			case 14: 
				yearFraction = QuantLib::Actual365Fixed(
					QuantLib::Actual365Fixed::Convention::Canadian).yearFraction(d2, d1); 
				break;
			case 15: 
				yearFraction = QuantLib::Actual365Fixed(
					QuantLib::Actual365Fixed::Convention::NoLeap).yearFraction(d2, d1); 
				break;
			case 16: 
				yearFraction = QuantLib::Actual365Fixed(
					QuantLib::Actual365Fixed::Convention::Standard).yearFraction(d2, d1); 
				break;
			default:
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Actual365).yearFraction(d2, d1);
				break;
			}
			break;
		case 4:
			yearFraction = QuantLib::Actual36525(this->m_inclusive).yearFraction(d2, d1);
			break;
		case 5:
			yearFraction = QuantLib::Actual366(this->m_inclusive).yearFraction(d2, d1);
			break;
		case 6:
			switch (this->m_dayCount) {
			case 11: 
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::AFB).yearFraction(d2, d1); 
				break;
			case 12: 
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Bond).yearFraction(d2, d1); 
				break;
			case 13: 
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Euro).yearFraction(d2, d1); 
				break;
			case 17: 
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Historical).yearFraction(d2, d1); 
				break;
			case 18: 
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::ISDA).yearFraction(d2, d1); 
				break;
			case 19: 
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::ISMA).yearFraction(d2, d1); 
				break;
			default:
				yearFraction = QuantLib::ActualActual(QuantLib::ActualActual::Convention::Actual365).yearFraction(d2, d1);
				break;
			}
			break;
		case 7:
			yearFraction = QuantLib::OneDayCounter().yearFraction(d2, d1); 
			break;
		default:
			yearFraction = QuantLib::OneDayCounter().yearFraction(d2, d1);
			break;
		}
	}
	return yearFraction;
}

bool isLastOfFebruary(QuantLib::Day d, QuantLib::Month m, QuantLib::Year y) {
	return m == 2 && d == 28 + (QuantLib::Date::isLeap(y) ? 1 : 0);
}

int isLastOfFebruaryLua(lua_State* L) {
	QuantLib::Day d = (int)luaL_checkinteger(L, 1);
	QuantLib::Month m = ConvertToQlMonth((int)luaL_checkinteger(L, 2));
	QuantLib::Year y = (int)luaL_checkinteger(L, 3);
	lua_pushboolean(L, isLastOfFebruary(d, m, y));
	return 1; //number of results */
}

int daysBetweenLua(lua_State* L) {
	QuantLib::Day d1 = (int)luaL_checkinteger(L, 1);
	QuantLib::Day d2 = (int)luaL_checkinteger(L, 2);

	QuantLib::Month m1 = ConvertToQlMonth((int)luaL_checkinteger(L, 3) - 1);
	QuantLib::Month m2 = ConvertToQlMonth((int)luaL_checkinteger(L, 4) - 1);

	QuantLib::Year y1 = (int)luaL_checkinteger(L, 5);
	QuantLib::Year y2 = (int)luaL_checkinteger(L, 6);

	lua_pushnumber(L, QuantLib::daysBetween(QuantLib::Date(d1, m1, y1), QuantLib::Date(d2, m2, y2)));
	return 1; //number of results */
}

int businessDaysBetweenLua(lua_State* L) {

	QuantLib::Day d1 = (int)luaL_checkinteger(L, 1);
	QuantLib::Day d2 = (int)luaL_checkinteger(L, 2);

	QuantLib::Month m1 = ConvertToQlMonth((int)luaL_checkinteger(L, 3) - 1);
	QuantLib::Month m2 = ConvertToQlMonth((int)luaL_checkinteger(L, 4) - 1);

	QuantLib::Year y1 = (int)luaL_checkinteger(L, 5);
	QuantLib::Year y2 = (int)luaL_checkinteger(L, 6);

	lua_pushnumber(L, customCalendar.businessDaysBetween(QuantLib::Date(d1, m1, y1), QuantLib::Date(d2, m2, y2), 
		true, false));
	return 1; //number of results */
}

QuantLib::Time getYearFraction(const QuantLib::Date& date2, const QuantLib::Date& date1,
	const std::string& dayCountFunction, const std::string& yearFracFunction) {

	double yearFracResult = 0;
	std::string functions = dayCountFunction + ";" + yearFracFunction;

	QuantLib::Day Day1 = date1.dayOfMonth();
	QuantLib::Day Day2 = date2.dayOfMonth();
	QuantLib::Month Month1 = date1.month();
	QuantLib::Month Month2 = date2.month();
	QuantLib::Year Year1 = date1.year();
	QuantLib::Year Year2 = date2.year();

	std::string yearFracResultStr = ";yearFracResult = yearFraction ("
		+ std::to_string(Day1) + ","
		+ std::to_string(Day2) + ","
		+ std::to_string(Month1) + ","
		+ std::to_string(Month2) + ","
		+ std::to_string(Year1) + ","
		+ std::to_string(Year2) + ")";

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//Push functions
	lua_pushcfunction(L, isLastOfFebruaryLua);
	lua_setglobal(L, "isLastOfFebruary");

	lua_pushcfunction(L, daysBetweenLua);
	lua_setglobal(L, "daysBetween");

	lua_pushcfunction(L, businessDaysBetweenLua);
	lua_setglobal(L, "businessDaysBetween");

	//Evaluate
	if (luaL_dostring(L, (functions + yearFracResultStr).c_str()) == LUA_OK) {
		lua_getglobal(L, "yearFracResult");
		yearFracResult = (double)lua_tonumber(L, -1);
	}
	lua_close(L);
	return yearFracResult;
}

QuantLib::Date::serial_type getDayCount(const QuantLib::Date& date2, const QuantLib::Date& date1,
	const std::string& dayCountFunction, const std::string& yearFracFunction) {

	int dayCountResult = 0;
	std::string functions = dayCountFunction + ";" + yearFracFunction;

	QuantLib::Day Day1 = date1.dayOfMonth();
	QuantLib::Day Day2 = date2.dayOfMonth();
	QuantLib::Month Month1 = date1.month();
	QuantLib::Month Month2 = date2.month();
	QuantLib::Year Year1 = date1.year();
	QuantLib::Year Year2 = date2.year();

	std::string dayCountResultStr = ";dayCountResult = dayCount ("
		+ std::to_string(Day1) + ","
		+ std::to_string(Day2) + ","
		+ std::to_string(Month1) + ","
		+ std::to_string(Month2) + ","
		+ std::to_string(Year1) + ","
		+ std::to_string(Year2) + ")";

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	//Push functions
	lua_pushcfunction(L, isLastOfFebruaryLua);
	lua_setglobal(L, "isLastOfFebruary");

	lua_pushcfunction(L, daysBetweenLua);
	lua_setglobal(L, "daysBetween");

	lua_pushcfunction(L, businessDaysBetweenLua);
	lua_setglobal(L, "businessDaysBetween");

	//Evaluate
	if (luaL_dostring(L, (functions + dayCountResultStr).c_str()) == LUA_OK) {
		lua_getglobal(L, "dayCountResult");
		dayCountResult = (int)lua_tonumber(L, -1);
	}
	lua_close(L);
	return dayCountResult;
}