#pragma once
#include "gui.hpp"

class QuantIOCurrency : public QuantIO::Window {
public:
	QuantIOCurrency() : Window("Currencies", "QuantIO", false) { };
	void DisplayContents() override;
};