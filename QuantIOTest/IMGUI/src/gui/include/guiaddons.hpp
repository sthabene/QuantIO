#pragma once
#include "gui.hpp"


//Windows Content
class AddonsDatePicker : public QuantIO::Window {
public: AddonsDatePicker() : Window(ICON_FA_CALENDAR " Date Picker" , "Addons", false) {}
	  void DisplayContents() override;
};

class AddonsIcons : public QuantIO::Window {
public: AddonsIcons() : Window(ICON_FA_ICONS " Icons", "Addons", false) {}
	  void DisplayContents() override;
};