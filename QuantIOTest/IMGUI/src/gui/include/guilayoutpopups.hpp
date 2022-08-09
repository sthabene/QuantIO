#pragma once
#include "gui.hpp"


//Store layout examples
void AddLayoutPopups(ImVector<QuantIO::Window>& windows);

class LayoutChildWindows : public QuantIO::Window {
public: LayoutChildWindows() : Window("Child Windows", "Layout", false) {}
	  void DisplayContents() override;
};

class LayoutWidgetsWidth : public QuantIO::Window {
public: LayoutWidgetsWidth() : Window("Widgets Width", "Layout", false) {}
	  void DisplayContents() override;
};

class LayoutHorizontal : public QuantIO::Window {
public: LayoutHorizontal() : Window("Horizontal Layout", "Layout", false) {}
	  void DisplayContents() override;
};

class LayoutGroups : public QuantIO::Window {
public: LayoutGroups() : Window("Groups", "Layout", false) {}
	  void DisplayContents() override;
};

class LayoutTextAlignment : public QuantIO::Window {
public: LayoutTextAlignment() : Window("Text Baseline Alignment", "Layout", false) {}
	  void DisplayContents() override;
};

class LayoutScrolling : public QuantIO::Window {
public: LayoutScrolling() : Window("Scrolling", "Layout", false) {}
	  void DisplayContents() override;
};

class LayoutClipping : public QuantIO::Window {
public: LayoutClipping() : Window("Clipping", "Layout", false) {}
	  void DisplayContents() override;
};

class PopupMain : public QuantIO::Window {
public: PopupMain() : Window("Popup Main", "Popup", false) {}
	  void DisplayContents() override;
};

class PopupContextMenu : public QuantIO::Window {
public: PopupContextMenu() : Window("Context Menu", "Popup", false) {}
	  void DisplayContents() override;
};

class PopupModals : public QuantIO::Window {
public: PopupModals() : Window("Modals", "Popup", false) {}
	  void DisplayContents() override;
};

class PopupMenus : public QuantIO::Window {
public: PopupMenus() : Window("Menus in windows", "Popup", false) {}
	  void DisplayContents() override;
};
