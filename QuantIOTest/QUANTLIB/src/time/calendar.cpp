#pragma once

#include "calendar.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"

CustomCalendar::CustomCalendar(std::string& name, std::string& weekends) {
	QuantLib::ext::shared_ptr<QuantLib::Calendar::Impl> customImpl(new CustomCalendar::CustomCalendarImpl(name, weekends));
	impl_ = customImpl;
};

CustomCalendar::CustomCalendar(const CustomCalendar& customCalendar) {
	impl_ = customCalendar.impl_;
};

void CustomCalendar::setAnotherCalendar(std::string& name, std::string& weekends) {
	QuantLib::ext::shared_ptr<QuantLib::Calendar::Impl> customImpl(new CustomCalendar::CustomCalendarImpl(name, weekends));
	impl_ = customImpl;
};

std::string CustomCalendar::CustomCalendarImpl::name() const {
	return this->m_calendarName;
};

bool CustomCalendar::CustomCalendarImpl::isBusinessDay(const QuantLib::Date& date) const {
	return !this->isWeekend(date.weekday());
};

bool CustomCalendar::CustomCalendarImpl::isWeekend(QuantLib::Weekday w) const {
	std::string weekends = this->m_weekends;

	if (boost::algorithm::contains(weekends, "0")) return false;
	else if (w == QuantLib::Weekday::Sun && boost::algorithm::contains(weekends, "1")) return true;
	else if (w == QuantLib::Weekday::Mon && boost::algorithm::contains(weekends, "2")) return true;
	else if (w == QuantLib::Weekday::Tue && boost::algorithm::contains(weekends, "3")) return true;
	else if (w == QuantLib::Weekday::Wed && boost::algorithm::contains(weekends, "4")) return true;
	else if (w == QuantLib::Weekday::Thu && boost::algorithm::contains(weekends, "5")) return true;
	else if (w == QuantLib::Weekday::Fri && boost::algorithm::contains(weekends, "6")) return true;
	else if (w == QuantLib::Weekday::Sat && boost::algorithm::contains(weekends, "7")) return true;
	else return false;
}

void CustomCalendar::addHolidays(std::vector<std::string>& holidays) {
	for (std::string date : holidays) {
		this->addHoliday(ConvertToQlDate(date));
	}
}

void CustomCalendar::removeHolidays(std::vector<std::string>& holidays) {
	for (std::string date : holidays) {
		this->removeHoliday(ConvertToQlDate(date));
	}
}