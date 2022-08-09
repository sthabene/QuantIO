#pragma once
#include "gui.hpp"

//Store Widget Windows
void AddWidgetWindows(ImVector<QuantIO::Window>& windows);

//Widget Windows Content
class WidgetBasic : public QuantIO::Window {
public:
    WidgetBasic() : Window("Basic", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetTrees : public QuantIO::Window {
public:
    WidgetTrees() : Window("Trees", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetCollapsingHeaders : public QuantIO::Window {
public:
    WidgetCollapsingHeaders() : Window("CollapsingHeaders", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetBullets : public QuantIO::Window {
public:
    WidgetBullets() : Window("Bullets", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetText : public QuantIO::Window {
public:
    WidgetText() : Window("Text", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetImages : public QuantIO::Window {
public:
    WidgetImages() : Window("Images", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetCombo : public QuantIO::Window {
public:
    WidgetCombo() : Window("Combo", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetListBoxes : public QuantIO::Window {
public:
    WidgetListBoxes() : Window("List Boxes", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetSelectables : public QuantIO::Window {
public:
    WidgetSelectables() : Window("Selectables", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetTextInput : public QuantIO::Window {
public:
    WidgetTextInput() : Window("Text Input", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetTabs : public QuantIO::Window {
public:
    WidgetTabs() : Window("Tabs", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetPlotting : public QuantIO::Window {
public:
    WidgetPlotting() : Window("Plotting", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetColor : public QuantIO::Window {
public:
    WidgetColor() : Window("Color", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetDragSlider : public QuantIO::Window {
public:
    WidgetDragSlider() : Window("Drag Slider", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetRange : public QuantIO::Window {
public:
    WidgetRange() : Window("Range", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetDataTypes : public QuantIO::Window {
public:
    WidgetDataTypes() : Window("Data Types", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetMulticomponent : public QuantIO::Window {
public:
    WidgetMulticomponent() : Window("Multicomponent", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetVerticalSliders : public QuantIO::Window {
public:
    WidgetVerticalSliders() : Window("Vertical Sliders", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetDragAndDrop : public QuantIO::Window {
public:
    WidgetDragAndDrop() : Window("Drag and Drop", "Widgets", false) {}
    void DisplayContents() override;
};
class WidgetQueryingItemStatus : public QuantIO::Window {
public:
    WidgetQueryingItemStatus() : Window("Querying Item Status", "Widgets", false) {}
    void DisplayContents() override;
};