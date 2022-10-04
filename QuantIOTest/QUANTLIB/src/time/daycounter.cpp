#include "daycounter.hpp"

CustomDayCounter::CustomDayCounter(std::string& name, float& yearDenominator, bool& includeLast) {
	ext::shared_ptr<DayCounter::Impl> customDayCountImpl(
		new CustomDayCounter::CustomDayCounterImpl(name, yearDenominator, includeLast));
	this->impl_ = customDayCountImpl;
}

CustomDayCounter::CustomDayCounter(const CustomDayCounter& customDayCounter) {
	this->impl_ = customDayCounter.impl_;
}

std::string CustomDayCounter::CustomDayCounterImpl::name() const {
	return this->m_dayCounterName;
};

Date::serial_type CustomDayCounter::CustomDayCounterImpl::dayCount(const Date& d1, const Date& d2) const {
	//Thirty 360
	Day dd1 = d1.dayOfMonth();
	Day dd2 = d2.dayOfMonth();
	Month mm1 = d1.month();
	Month mm2 = d2.month();
	Year yy1 = d1.year();
	Year yy2 = d2.year();

	if (dd1 == 31) { dd1 = 30; }
	if (dd2 == 31 && dd1 == 30) { dd2 = 30; }

	return 360 * (yy2 - yy1) + 30 * (mm2 - mm1) + (dd2 - dd1);
}

Time CustomDayCounter::CustomDayCounterImpl::yearFraction(const Date& d1, const Date& d2, const Date&, const Date&) const {
	return this->dayCount(d1, d2) / this->m_yearDenominator;
}

void CustomDayCounter::setAnotherDayCounter(std::string& name, float& yearDenominator, bool& includeLast) {

};