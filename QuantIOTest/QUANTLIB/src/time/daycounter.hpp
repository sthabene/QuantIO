#pragma once
#include <ql/time/daycounter.hpp>
#include "../luastate/luastate.hpp"
#include "boost/algorithm/string.hpp"
#include "calendar.hpp"

class CustomDayCounter : public QuantLib::DayCounter {
public:
	class CustomDayCounterImpl : public QuantLib::DayCounter::Impl {
	public:
		std::string m_dayCounterName;
		std::string m_dayCountFunction;
		std::string m_yearFractionFunction;
		bool m_includeLast;
		std::string name() const override;
		//number of days between two dates
		QuantLib::Date::serial_type dayCount(const QuantLib::Date& d1, const QuantLib::Date& d2) const override;
		//the period between two dates as a fraction of year
		QuantLib::Time yearFraction(const QuantLib::Date& d1, const QuantLib::Date& d2, 
			const QuantLib::Date&, const QuantLib::Date&) const override;
		CustomDayCounterImpl(std::string& name, bool& includeLast, 
			std::string& dayCountFunction, std::string& yearFractionFunction) : m_dayCounterName(name),
			m_includeLast(includeLast), m_dayCountFunction(dayCountFunction), 
			m_yearFractionFunction(yearFractionFunction) {};
	};
public:
	void setAnotherDayCounter(std::string& name, bool& includeLast, std::string& dayCountFunction, 
		std::string& yearFractionFunction);

	CustomDayCounter() = default;
	CustomDayCounter(std::string& name, bool& includeLast, std::string& dayCountFunction, 
		std::string& yearFractionFunction);
	CustomDayCounter(const CustomDayCounter& customDayCounter);

};

inline bool isLastOfFebruary(QuantLib::Day d, QuantLib::Month m, QuantLib::Year y) {
	return m == 2 && d == 28 + (QuantLib::Date::isLeap(y) ? 1 : 0);
}

inline int daysBetweenLua(lua_State* L) {
	QuantLib::Day d1 = (int)luaL_checkinteger(L, 1);
	QuantLib::Day d2 = (int)luaL_checkinteger(L, 2);

	QuantLib::Month m1 = ConvertToQlMonth((int)luaL_checkinteger(L, 3));
	QuantLib::Month m2 = ConvertToQlMonth((int)luaL_checkinteger(L, 4));
	
	QuantLib::Year y1 = (int)luaL_checkinteger(L, 5);
	QuantLib::Year y2 = (int)luaL_checkinteger(L, 6);
	
	lua_pushnumber(L, QuantLib::daysBetween(QuantLib::Date(d1, m1, y1), QuantLib::Date(d2, m2, y2)));
	return 1; //number of results */
}

inline int isLastOfFebruaryLua(lua_State* L) {
	QuantLib::Day d = (int)luaL_checkinteger(L, 1);
	QuantLib::Month m = ConvertToQlMonth((int)luaL_checkinteger(L, 2));
	QuantLib::Year y = (int)luaL_checkinteger(L, 3);
	lua_pushboolean(L, isLastOfFebruary(d, m, y));
	return 1; //number of results */
}

inline QuantLib::Time getYearFraction(const QuantLib::Date& date1, const QuantLib::Date& date2,
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

	//Evaluate
	if (luaL_dostring(L, (functions + yearFracResultStr).c_str()) == LUA_OK) {
		lua_getglobal(L, "yearFracResult");
		yearFracResult = (double)lua_tonumber(L, -1);
	}
	lua_close(L);
	return yearFracResult;
}

inline QuantLib::Date::serial_type getDayCount(const QuantLib::Date& date1, const QuantLib::Date& date2,
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

	//Evaluate
	if (luaL_dostring(L, (functions + dayCountResultStr).c_str()) == LUA_OK) {
		lua_getglobal(L, "dayCountResult");
		dayCountResult = (int)lua_tonumber(L, -1);
	}
	lua_close(L);
	return dayCountResult;
}