#pragma once
#include "gui.hpp"


//Windows Content
class AddonsDatePicker : public QuantIO::Window {
public: AddonsDatePicker() : Window("Date Picker", "Addons", false) {}
	  void DisplayContents() override;
};

class AddonsIcons : public QuantIO::Window {
public: AddonsIcons() : Window("Icons", "Addons", false) {}
	  void DisplayContents() override;
};