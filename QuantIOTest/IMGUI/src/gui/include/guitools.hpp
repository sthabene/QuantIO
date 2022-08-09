#pragma once
#include "gui.hpp"

//Store Widget Windows
void AddToolsWindows(ImVector<QuantIO::Window>& windows);

class ToolsDebugger : public QuantIO::Window {
public:
    ToolsDebugger() : Window("Debugger", "Tools", false) {}
    void DisplayContents() override;
};

class ToolsDebugLog : public QuantIO::Window {
public:
    ToolsDebugLog() : Window("Debug Log", "Tools", false) {}
    void DisplayContents() override;
};

class ToolsStackTool : public QuantIO::Window {
public:
    ToolsStackTool() : Window("Stack Tool", "Tools", false) {}
    void DisplayContents() override;
};

class ToolsStyleEditor : public QuantIO::Window {
public:
    ToolsStyleEditor() : Window("Style Editor", "Tools", false) {}
    void DisplayContents() override;
};

