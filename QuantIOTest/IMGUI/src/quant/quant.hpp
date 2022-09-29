#pragma once
#include "gui.hpp"

class QuantIOCurrency : public QuantIO::Window {
public:
	QuantIOCurrency() : Window("Currencies", "QuantIO", false) { };
	void DisplayContents() override;
};

class QuantIOConfig : public QuantIO::Window {
public:
	QuantIOConfig() : Window("Configuration", "QuantIO", false) { };
	void DisplayContents() override;
};

//Calendar
class QuantIOCalendars : public QuantIO::Window {
public:
	QuantIOCalendars() : Window("Calendars", "Time", false) { };
	void DisplayContents() override;
};
void CalendarImplementation(std::string& weekend, std::string& calendarId);
void CalendarAdhocHolidays(std::string& calendarId);

//Day counters
class QuantIODayCounters : public QuantIO::Window {
public:
	QuantIODayCounters() : Window("Day Counters", "Time", false) { };
	void DisplayContents() override;
};