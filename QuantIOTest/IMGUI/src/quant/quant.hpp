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

class QuantIOCalendars : public QuantIO::Window {
public:
	QuantIOCalendars() : Window("Calendars", "Time", true) { };
	void DisplayContents() override;
};

void CalendarImplementation();