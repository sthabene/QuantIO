#pragma once
#include "gui.hpp"

class QuantIORounding : public QuantIO::Window {
public:
	QuantIORounding() : Window("Rounding", "QuantIO", false) { };
	void DisplayContents() override;
};
