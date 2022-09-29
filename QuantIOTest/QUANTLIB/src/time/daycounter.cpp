#include "daycounter.hpp"

CustomDayCounter::CustomDayCounter() {
	std::string nullName = "NULL";
	ext::shared_ptr<DayCounter::Impl> customDayCountImpl(new CustomDayCounter::CustomDayCounterImpl(nullName));
	this->impl_ = customDayCountImpl;
}

CustomDayCounter::CustomDayCounter(std::string& name) {
	ext::shared_ptr<DayCounter::Impl> customDayCountImpl(new CustomDayCounter::CustomDayCounterImpl(name));
	this->impl_ = customDayCountImpl;
}

CustomDayCounter::CustomDayCounter(const CustomDayCounter& customDayCounter) {
	this->impl_ = customDayCounter.impl_;
}

std::string CustomDayCounter::CustomDayCounterImpl::name() const {
	return this->m_dayCounterName;
};