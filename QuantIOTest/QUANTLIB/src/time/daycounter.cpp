#pragma once
#include "daycounter.hpp"
#include "boost/algorithm/string.hpp"

static CustomCalendar customCalendar;

CustomDayCounter::CustomDayCounter(std::string& name, std::string& dayCountFunction, 
	std::string& yearFractionFunction, CustomCalendar& calendar) {
	QuantLib::ext::shared_ptr<QuantLib::DayCounter::Impl> customDayCountImpl(
		new CustomDayCounter::CustomDayCounterImpl(name, dayCountFunction, yearFractionFunction));
	this->impl_ = customDayCountImpl;
	customCalendar = calendar;
}

CustomDayCounter::CustomDayCounter(const CustomDayCounter& customDayCounter) {
	this->impl_ = customDayCounter.impl_;
	customCalendar = customDayCounter.m_customCalendar;
}

void CustomDayCounter::setAnotherDayCounter(std::string& name, std::string& dayCountFunction,
	std::string& yearFractionFunction, CustomCalendar& calendar) {
	QuantLib::ext::shared_ptr<QuantLib::DayCounter::Impl> customDayCountImpl(
		new CustomDayCounter::CustomDayCounterImpl(name, dayCountFunction, yearFractionFunction));
	impl_ = customDayCountImpl;
	customCalendar = calendar;
};


std::string CustomDayCounter::CustomDayCounterImpl::name() const {
	return this->m_dayCounterName;
};

QuantLib::Date::serial_type CustomDayCounter::CustomDayCounterImpl::dayCount(const QuantLib::Date& d1, 
	const QuantLib::Date& d2) const {
	
	QuantLib::Date::serial_type dayCount = getDayCount(d1, d2, this->m_dayCountFunction, this->m_yearFractionFunction);

	return dayCount;
}

QuantLib::Time CustomDayCounter::CustomDayCounterImpl::yearFraction(const QuantLib::Date& d1, 
	const QuantLib::Date& d2, const QuantLib::Date&, const QuantLib::Date&) const {

	QuantLib::Time yearFraction = getYearFraction(d1, d2, this->m_dayCountFunction, this->m_yearFractionFunction);

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

QuantLib::Time getYearFraction(const QuantLib::Date& date1, const QuantLib::Date& date2,
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

QuantLib::Date::serial_type getDayCount(const QuantLib::Date& date1, const QuantLib::Date& date2,
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