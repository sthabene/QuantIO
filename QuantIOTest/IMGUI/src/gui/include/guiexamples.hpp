#pragma once
#include "gui.hpp"

//Store Windows
void AddExampleWindows(ImVector<QuantIO::Window>& windows);

//Windows Content
class ExampleConsole : public QuantIO::Window {
public: ExampleConsole() : Window("Console", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleLog : public QuantIO::Window {
public: ExampleLog() : Window("Log", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleLayout : public QuantIO::Window {
public: ExampleLayout() : Window("Layout", "Example", false) {}
	  void DisplayContents() override;
};
class ExamplePropertyEditor : public QuantIO::Window {
public: ExamplePropertyEditor() : Window("PropertyEditor", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleLongText : public QuantIO::Window {
public: ExampleLongText() : Window("LongText", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleAutoResize : public QuantIO::Window {
public: ExampleAutoResize() : Window("AutoResize", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleConstrainedResize : public QuantIO::Window {
public: ExampleConstrainedResize() : Window("ConstrainedResize", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleSimpleOverlay : public QuantIO::Window {
public: ExampleSimpleOverlay() : Window("SimpleOverlay", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleFullscreen : public QuantIO::Window {
public: ExampleFullscreen() : Window("Fullscreen", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleWindowTitles : public QuantIO::Window {
public: ExampleWindowTitles() : Window("WindowTitles", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleCustomRendering : public QuantIO::Window {
public: ExampleCustomRendering() : Window("CustomRendering", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleDockSpace : public QuantIO::Window {
public: ExampleDockSpace() : Window("DockSpace", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleDocuments : public QuantIO::Window {
public: ExampleDocuments() : Window("Documents", "Example", false) {}
	  void DisplayContents() override;
};
class ExampleFiltering : public QuantIO::Window {
public: ExampleFiltering() : Window("Filtering", "Example", false) {}
	  void DisplayContents() override;
};

class ExampleNavigation : public QuantIO::Window {
public: ExampleNavigation() : Window("Inputs & Navigation", "Example", false) {}
	  void DisplayContents() override;
};