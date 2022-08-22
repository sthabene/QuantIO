#include "guiwidgets.hpp"
#include "guiexamples.hpp"
#include "guilayoutpopups.hpp"
#include "guitables.hpp"
#include "guitools.hpp"
#include "guiaddons.hpp"
#include "currencies\currencies.hpp"
#include "math\rounding\rounding.hpp"

#include <algorithm>

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (disable: 4127)
#pragma warning (disable: 4996)
#pragma warning (disable: 26451)
#pragma warning (disable: 4130)
#endif

//Checks if app is still active
bool QuantIO::closeApplication;

//To store status bar message
char statusBarMessage[32] = "Ready";

//Accessible Items
bool showAboutWindow = false;
bool showDemoWindow = false;

//About Window
static void AboutWindow(bool* pOpen);

//Stores the name of tab that should be active
static const char* selectedTab; 

//Adds windows to the global ActiveWindows vector
void QuantIO::AddWindows() {

    AddWidgetWindows(QuantIO::ActiveWindows);
    AddExampleWindows(QuantIO::ActiveWindows);
    AddLayoutPopups(QuantIO::ActiveWindows);
    AddTables(QuantIO::ActiveWindows);
    AddToolsWindows(QuantIO::ActiveWindows);
    QuantIO::ActiveWindows.push_back(QuantIOCurrency());
    QuantIO::ActiveWindows.push_back(QuantIORounding());
    QuantIO::ActiveWindows.push_back(AddonsDatePicker());
}

//Windows Menu
void WindowsMenu() {
    if (ImGui::BeginMenu("Examples")) {
        if (ImGui::BeginMenu("Addons")) {
            for (QuantIO::Window& addon : QuantIO::ActiveWindows) {
                if (addon.Type == "Addons" && ImGui::MenuItem(addon.Name, NULL, addon.Open, true)) {
                    if (!addon.Open) {
                        addon.DoOpen();
                    }
                    selectedTab = addon.Name; //Change to this tab
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Widgets")) {
            for (QuantIO::Window& widget : QuantIO::ActiveWindows) {
                if (widget.Type == "Widgets" && ImGui::MenuItem(widget.Name, NULL, widget.Open, true)) {
                    if (!widget.Open) {
                        widget.DoOpen();
                    }
                    selectedTab = widget.Name; //Change to this tab
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples")) {
            for (QuantIO::Window& example : QuantIO::ActiveWindows) {
                if (example.Type == "Example" && ImGui::MenuItem(example.Name, NULL, example.Open, true)) {
                    if (!example.Open) {
                        example.DoOpen();
                    }
                    selectedTab = example.Name; //Change to this tab
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Layout")) {
            for (QuantIO::Window& layout : QuantIO::ActiveWindows) {
                if (layout.Type == "Layout" && ImGui::MenuItem(layout.Name, NULL, layout.Open, true)) {
                    if (!layout.Open) {
                        layout.DoOpen();
                    }
                    selectedTab = layout.Name; //Change to this tab
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Popups")) {
            for (QuantIO::Window& popup : QuantIO::ActiveWindows) {
                if (popup.Type == "Popup" && ImGui::MenuItem(popup.Name, NULL, popup.Open, true)) {
                    if (!popup.Open) {
                        popup.DoOpen();
                    }
                    selectedTab = popup.Name; //Change to this tab
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tables & Columns")) {
            for (QuantIO::Window& table : QuantIO::ActiveWindows) {
                if (table.Type == "TableColumn" && ImGui::MenuItem(table.Name, NULL, table.Open, true)) {
                    if (!table.Open) {
                        table.DoOpen();
                    }
                    selectedTab = table.Name; //Change to this tab
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    ImGui::Separator();
    for (QuantIO::Window& quantio : QuantIO::ActiveWindows) {
        if (quantio.Type == "QuantIO" && ImGui::MenuItem(quantio.Name, NULL, quantio.Open, true)) {
            if (!quantio.Open) {
                quantio.DoOpen();
            }
            selectedTab = quantio.Name; //Change to this tab
        }
    }
    ImGui::Separator();
    if (ImGui::BeginMenu("Configuration")) {
        if (ImGui::MenuItem("Demo Window", NULL, &showDemoWindow)) {
            selectedTab = "Demo Window";
        };
        ImGui::EndMenu();
    }
}

//Main Menubar
void QuantIO::MainMenuBar() {
    if (ImGui::BeginMainMenuBar()) { //Start Menu
        //File Menu
        if (ImGui::BeginMenu("File")) { //Start Menu
            if (ImGui::BeginMenu("New")) { //Start Menu
                if (ImGui::BeginMenu("Static")) { //Start Menu
                    ImGui::MenuItem("Currency");
                    ImGui::MenuItem("Index");
                    ImGui::EndMenu(); //End Menu
                }
                ImGui::EndMenu(); //End Menu
            }
            ImGui::MenuItem("Add");
            ImGui::Separator();
            if (ImGui::MenuItem("Quit", "Alt+F4", false, true)) {
                QuantIO::closeApplication = true;
            }
            ImGui::EndMenu();
        }
        //Edit Menu
        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Find");
            ImGui::Separator();
            ImGui::MenuItem("Save Preferences");
            ImGui::MenuItem("Edit Preferences");
            ImGui::EndMenu(); //End Menu
        };
        //View Menu
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("View")) {
                //statusBarMessage = "View";
            }
            ImGui::EndMenu(); //End Menu
        }
        //Controls Menu
        if (ImGui::BeginMenu("Controls")) {
            if (ImGui::MenuItem("Controls")) {
                //statusBarMessage = "Controls";
            }
            ImGui::EndMenu(); //End Menu
        }
        //Tools Menu
        if (ImGui::BeginMenu("Tools")) {
            for (QuantIO::Window& tool : QuantIO::ActiveWindows) {
                if (tool.Type == "Tools" && ImGui::MenuItem(tool.Name, NULL, tool.Open, true)) {
                    if (!tool.Open) {
                        tool.DoOpen();
                    }
                    selectedTab = tool.Name; //Change to this tab
                }
            }
            ImGui::EndMenu();
        }
        //Window Menu
        if (ImGui::BeginMenu("Window")) {
            WindowsMenu();
            ImGui::EndMenu(); //End Menu
        }
        //Help Menu 
        if (ImGui::BeginMenu("Help")) {
            ImGui::MenuItem("Online Help");
            if (ImGui::BeginMenu("Support")) {
                ImGui::MenuItem("View Error Log File");
                ImGui::MenuItem("View Settings File");
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("About", NULL, &showAboutWindow)) {
                //statusBarMessage = "About";
                sprintf(statusBarMessage, "About");
            };
            ImGui::EndMenu();
        }

        //ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ImGui::GetFrameHeight() * 3.5f); //Set X position for button
        //redockAll = ImGui::Button("Redock all", ImVec2(ImGui::GetFrameHeight() * 3.5f, ImGui::GetFrameHeight()));
        ImGui::EndMainMenuBar(); //End Menu
    }

    //Placing show functions in an item that will always be available when app starts
    if (showDemoWindow)     ImGui::ShowDemoWindow(&showDemoWindow);
    if (showAboutWindow)    AboutWindow(&showAboutWindow);

}

//Status Bar
void QuantIO::MainStatusBar() {
    ImGuiWindowFlags statusBarFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
    if (ImGui::BeginViewportSideBar("##StatusBar", NULL, ImGuiDir_Down, ImGui::GetFrameHeight(), statusBarFlags)) {
        if (ImGui::BeginMenuBar()) {
            ImGui::TextUnformatted(statusBarMessage);
            ImGui::EndMenuBar();
        }
    }
    ImGui::End();
}

//Check if tab is open or not, if not uses SetTabItemClosed to close
static void NotifyOfWindowClosedElsewhere(ImVector<QuantIO::Window>& windows) {
    static int i = 0;
    for (QuantIO::Window& window : windows) {
        if (!window.Open) {
            ImGui::SetTabItemClosed(window.Name);
        }

        //if (window.Name == "Currency" && !window.Open) {
        //    windows.erase(windows.begin() + i);
        //}
        i++;
        //doc.OpenPrev = doc.Open;
    }
}

//About Window
static void AboutWindow(bool* pOpen) {
    ImGuiWindowFlags aboutWindowFlags = ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize;
    ImGui::OpenPopup("About This Application");
    //ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowFocus(); //Make the window focus
    if (ImGui::BeginPopupModal("About This Application", pOpen, aboutWindowFlags)) {
        ImGui::Text("Application version %s", "1.0");
        ImGui::Separator();
        ImGui::Text("By the awesome team.");
        ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMax().y - 1.5f * ImGui::GetStyle().IndentSpacing); //Get
        if (ImGui::Button("OK")) {
            showAboutWindow = false;
            //ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    //SetWindowFocus("About Dear ImGui"); //Also used to set focus
}

void QuantIO::MainWindow(bool* p_open) {
    //Window Flags
    ImGuiWindowFlags mainWindowFlags = ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoSavedSettings |
        //ImGuiWindowFlags_NoBackground |
        //ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoDocking | //No Docking
        //ImGuiWindowFlags_AlwaysUseWindowPadding |
        ImGuiWindowFlags_NoCollapse; //

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->WorkPos);
    ImGui::SetNextWindowSize(ImGui::GetMainViewport()->WorkSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding | ImGuiStyleVar_FramePadding, ImGui::GetStyle().FramePadding);

    bool windowContentsVisible = ImGui::Begin("MainWindow", p_open, mainWindowFlags);
    if (!windowContentsVisible) {
        ImGui::End();
        return;
    }

    //Number of Open Tabs
    int openTabs = 0;
    for (QuantIO::Window& widget : QuantIO::ActiveWindows) {
        openTabs += widget.Open ? 1 : 0;
    }

    // Create Tabs
    ImGuiTabBarFlags tabBarFlags = ImGuiTabBarFlags_Reorderable |
        ImGuiTabBarFlags_NoTabListScrollingButtons |
        ImGuiTabBarFlags_AutoSelectNewTabs |
        ImGuiTabBarFlags_FittingPolicyResizeDown;
    
    if (openTabs > 0) {
        tabBarFlags |= ImGuiTabBarFlags_TabListPopupButton;
        sprintf(statusBarMessage, "%s", selectedTab);
    }
    if (openTabs == 0) sprintf(statusBarMessage, "Ready");

    static int nextTabId = 0;

    if (ImGui::BeginTabBar("TabBar", tabBarFlags)) {

        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoCloseWithMiddleMouseButton)) {
            ImGui::OpenPopup("MenuPopUp");
        }

        if (ImGui::BeginPopupContextItem("MenuPopUp")) {
            WindowsMenu();
            ImGui::EndPopup();
        }

        NotifyOfWindowClosedElsewhere(QuantIO::ActiveWindows);

        for (QuantIO::Window& widget : QuantIO::ActiveWindows) {
            if (!widget.Open) continue;

            bool tabBarItem;
            tabBarItem = ImGui::BeginTabItem(widget.Name, &widget.Open, ImGuiTabItemFlags_None);
            /*if (widget.Name == selectedTab) {
                tabBarItem = ImGui::BeginTabItem(widget.Name, &widget.Open, ImGuiTabItemFlags_SetSelected);
            }
            else {
                tabBarItem = ImGui::BeginTabItem(widget.Name, &widget.Open, ImGuiTabItemFlags_None);
            }*/

            if (tabBarItem) {
                if (ImGui::IsItemClicked()) {
                    selectedTab = widget.Name;
                }

                widget.DisplayContents();
                ImGui::EndTabItem();
            }
            if (ImGui::IsItemClicked()) {
                selectedTab = widget.Name;
            }
        }
        ImGui::EndTabBar();
    }

    // Early out other contents
    if (!windowContentsVisible) {
        ImGui::End();
        return;
    }

    ImGui::End();

    ImGui::PopStyleVar(); //Pop main window padding style
}