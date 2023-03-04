#pragma once
#include "gui.hpp"

//Db connection
#include "database.hpp"
#include "sqlite.hpp"
#include "error.hpp"

//IMGUI
static ImVec2 buttonSz(25.0f * 4.0f, 30.0f); //To change 

namespace QuantIO {
	//DB Connection
	static QuantIO::Connection dbConnection("..\\..\\misc\\database\\main.db");
}

//Rounding
class QuantIORounding : public QuantIO::Window {
public:
	QuantIORounding() : Window("Rounding", "QuantIO", false) { };
	void DisplayContents() override;
};

//Currencies
class QuantIOCurrency : public QuantIO::Window {
public:
	QuantIOCurrency() : Window("Currencies", "QuantIO", false) { };
	void DisplayContents() override;
};

//Configuration
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

//Day conventions -- Removed it
class QuantIODayConventions : public QuantIO::Window {
public:
	QuantIODayConventions() : Window("Day Conventions", "Time", false) { };
	void DisplayContents() override;
};

//Interest Rate Indexes
class QuantIOInterestRateIndexes : public QuantIO::Window {
public:
	QuantIOInterestRateIndexes() : Window("Interest Rate Indexes", "Indexes", false) { };
	void DisplayContents() override;
};

//Inflation Indexes
class QuantIOInflationIndexes : public QuantIO::Window {
public:
	QuantIOInflationIndexes() : Window("Inflation Indexes", "Indexes", false) { };
	void DisplayContents() override;
};