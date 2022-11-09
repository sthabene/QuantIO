#pragma once
#include "daycounter.hpp"

CustomDayCounter::CustomDayCounter(std::string& name, bool& includeLast, std::string& dayCountFunction, 
	std::string& yearFractionFunction) {
	QuantLib::ext::shared_ptr<QuantLib::DayCounter::Impl> customDayCountImpl(
		new CustomDayCounter::CustomDayCounterImpl(name, includeLast, dayCountFunction, yearFractionFunction));
	this->impl_ = customDayCountImpl;
}

CustomDayCounter::CustomDayCounter(const CustomDayCounter& customDayCounter) {
	this->impl_ = customDayCounter.impl_;
}

void CustomDayCounter::setAnotherDayCounter(std::string& name, bool& includeLast, std::string& dayCountFunction,
	std::string& yearFractionFunction) {
	QuantLib::ext::shared_ptr<QuantLib::DayCounter::Impl> customDayCountImpl(
		new CustomDayCounter::CustomDayCounterImpl(name, includeLast, dayCountFunction, yearFractionFunction));
	impl_ = customDayCountImpl;
};


std::string CustomDayCounter::CustomDayCounterImpl::name() const {
	return this->m_dayCounterName;
};

QuantLib::Date::serial_type CustomDayCounter::CustomDayCounterImpl::dayCount(const QuantLib::Date& d1, 
	const QuantLib::Date& d2) const {
	
	QuantLib::Date::serial_type dayCount = getDayCount(d1, d2, this->m_dayCountFunction, this->m_yearFractionFunction)
		+ (this->m_includeLast ? 1 : 0);

	return dayCount;
}

QuantLib::Time CustomDayCounter::CustomDayCounterImpl::yearFraction(const QuantLib::Date& d1, 
	const QuantLib::Date& d2, const QuantLib::Date&, const QuantLib::Date&) const {

	QuantLib::Time yearFraction = getYearFraction(d1, d2, this->m_dayCountFunction, this->m_yearFractionFunction);

	return yearFraction;
}

