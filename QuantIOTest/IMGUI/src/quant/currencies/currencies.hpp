#pragma once
#include "gui.hpp"

class QuantIOCurrency : public QuantIO::Window {
public:
	QuantIOCurrency() : Window("Currency", "QuantIO", true) { };
	void DisplayContents() override;
};