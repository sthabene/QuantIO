#include "../quant.hpp"
#include "time/daycounter.hpp"

#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static std::string query = "SELECT COUNTER_ID, COUNTER_NAME, COUNTER_DESCR FROM DAY_COUNTER ORDER BY COUNTER_NAME";
static std::string title = "DayCounter###";
static std::vector<std::vector<std::string>> tableData;
static int refresh = 1;
static bool showFilter = false;

//Open item
static std::vector<std::string> selectedRow;
static ImGuiInputTextFlags popupInputFlags = ImGuiInputTextFlags_None;
static ImGuiTabItemFlags tabItemFlag = ImGuiTabItemFlags_None;


//Day counter items


//IMGUI 
static ImVec2 buttonSz(25.0f * 5.0f, 32.0f); //To change 

void QuantIODayCounters::DisplayContents() {
	ImGui::BeginChild("DayCounters", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
	{
		static const float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 3.0f;

		//Running the query
		if (refresh & 1) {
			tableData = QuantIO::dbConnection.getTableData2(query);
			refresh++;
		}

		//#rows and columns
		static int iRows = tableData.size() - 1;
		static unsigned int iColumns = tableData[0].size();

		//Filtering text field
		static ImGuiTextFilter filter;
		if (showFilter) {
			filter.Draw("Filter", ImGui::GetFontSize() * 25);
			ImGui::SameLine();
			HelpMarker("Use , for OR, - for NOT, filtering includes hidden columns");
		}

		int startCount = 0;
		int displayRows = iRows;
		std::vector<std::vector<std::string>> filteredData;
		if (filter.IsActive()) {
			for (int i = 1; i <= iRows; i++) {
				std::string rowData = boost::algorithm::join(tableData[i], " ");
				if (filter.PassFilter(rowData.c_str())) {
					filteredData.push_back(tableData[i]);
				}
			}
			displayRows = filteredData.size();
		}
		else {
			filteredData = tableData;
			startCount = 1;
		}

		//Table Height
		float tableHeight = rowHeight * ((float)displayRows + 1);
		float maxTableHeight = (tableHeight > ImGui::GetContentRegionAvail().y ?
			ImGui::GetContentRegionAvail().y : tableHeight);

		//Holds selections
		static ImVector<std::string> selections;

		if (ImGui::BeginTable("DayCounters", iColumns, QuantIO::tableFlags, ImVec2(0.0f, maxTableHeight), 0.0f)) {

			//Make first row (header) always visible
			ImGui::TableSetupScrollFreeze(0, 1);

			//Column Setup
			for (std::size_t i = 0; i < iColumns; i++) { //Column label
				if (i == 0) {
					ImGui::TableSetupColumn(tableData[0][i].c_str(), ImGuiTableColumnFlags_Disabled);
				}
				else if (i == 1) {
					ImGui::TableSetupColumn(tableData[0][i].c_str(), ImGuiTableColumnFlags_NoHide);
				}
				else {
					ImGui::TableSetupColumn(tableData[0][i].c_str());
				}
			}
			ImGui::TableHeadersRow();

			//Clipping display results
			static ImGuiListClipper clipper;
			clipper.Begin(displayRows);

			while (clipper.Step()) {
				for (int row = clipper.DisplayStart + startCount; row < clipper.DisplayEnd + startCount; row++) {
					std::vector<std::string>* currentRow = &filteredData[row];
					ImGui::PushID(row);
					ImGui::TableNextRow(ImGuiTableRowFlags_None, rowHeight);

					for (std::size_t column = 0; column < currentRow->size(); column++) {
						ImGui::TableSetColumnIndex(column);
						std::string* currentItem = &currentRow->at(column);
						const bool itemIsSelected = selections.contains(currentRow->at(0));
						if (ImGui::Selectable(currentItem->c_str(), itemIsSelected,
							ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick,
							ImVec2(0, 0))) {
							if (ImGui::GetIO().KeyCtrl) {
								if (itemIsSelected) {
									selections.find_erase_unsorted(currentRow->at(0));
								}
								else {
									selections.push_back(currentRow->at(0));
								}
							}
							else {
								selections.clear();
								selections.push_back(currentRow->at(0));
							}
						}

					}

					//Booleans for popups
					bool openOpenPopup = false;
					bool openEditPopup = false;
					bool openInsertPopup = false;
					bool openDuplicatePopup = false;
					bool openCopyLinePopup = false;
					bool openDeletePopup = false;
					bool openExitPopup = false;

					//Context menu
					if (ImGui::BeginPopupContextItem("ContextPopup")) {
						selections.clear();
						selections.push_back(currentRow->at(0));

						if (ImGui::MenuItem("Open", "Enter")) {
							boost::format openQuery = boost::format("SELECT COUNTER_ID, COUNTER_NAME, COUNTER_DESCR FROM DAY_COUNTER WHERE COUNTER_ID = %1%") % currentRow->at(0);
							selectedRow = QuantIO::dbConnection.getTableData2(openQuery.str(), false, false)[0];

							openOpenPopup = true;
						}

						if (ImGui::MenuItem("Edit", "Ctrl + Enter")) {

						}

						ImGui::Separator();
						if (ImGui::MenuItem("Refresh", "F5")) {
							refresh++;
						};
						ImGui::MenuItem("Filter", "Ctrl + F", &showFilter, true);
						ImGui::Separator();
						if (ImGui::MenuItem("Insert", "Insert")) {
							openInsertPopup = true;
						};
						if (ImGui::MenuItem("Duplicate", "Ctrl + D")) {
							openDuplicatePopup = true;
						};
						if (ImGui::MenuItem("Copy line", "Ctrl + C")) {
							ImGui::LogToClipboard();
							std::string copy = boost::algorithm::join(*currentRow, "\t");
							ImGui::LogText(copy.c_str());
							ImGui::LogFinish();
						};
						ImGui::Separator();
						if (ImGui::MenuItem("Delete")) {

						}
						ImGui::EndPopup();
					}

					//Open Popup
					if (openOpenPopup) {
						title = "Day Counter: Read-only mode";
						ImGui::OpenPopup(title.c_str());
						tabItemFlag = ImGuiTabItemFlags_SetSelected;
					}
					else {
						tabItemFlag = ImGuiTabItemFlags_None;
					}

					ImGui::SetNextWindowPos(QuantIO::popupLocation(), ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
					ImGui::SetNextWindowSize(ImVec2(1000, 1050), ImGuiCond_FirstUseEver);

					if (ImGui::BeginPopupModal(title.c_str(), NULL, 
						ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {

						ImGui::SetCursorPosX(900.0f);
						ImGui::PushItemWidth(ImGui::CalcTextSize(selectedRow[0].c_str()).x + 15.0f);
						ImGui::InputText(" Id", (char*)selectedRow[0].c_str(), 4,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();
						ImGui::Spacing();

						float titleWidth = ImGui::CalcTextSize(selectedRow[1].c_str()).x + 15.0f;
						float descrWidth = ImGui::CalcTextSize(selectedRow[2].c_str()).x > 1000 ? 1000 : 
						ImGui::CalcTextSize(selectedRow[2].c_str()).x + 15.0f;

						ImGui::SetCursorPosX(500.0f - (titleWidth + 50.0f) * 0.5f);
						ImGui::PushItemWidth(titleWidth);
						ImGui::InputText("##DayCounter", (char*)selectedRow[1].c_str(), 32,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();


						ImGui::Dummy(ImVec2(0.0f, 15.0f));
						ImGui::SetCursorPosX(500.0f - (ImGui::CalcTextSize("Description").x + 50.0f) * 0.5f);
						ImGui::TextDisabled("Description");

						ImGui::SetCursorPosX(500.0f - (descrWidth + 50.0f) * 0.5f);
						//ImGui::PushItemWidth(descrWidth);
						ImGui::InputTextMultiline("##Description", (char*)selectedRow[2].c_str(), 255,
							ImVec2(descrWidth, 35.0f),
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						//ImGui::PopItemWidth();
						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::Separator();

						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::SetCursorPosX(500.0f - (ImGui::CalcTextSize("Include Last").x * 0.5f + 50.0f));

						bool includeLast = false;
						ImGui::Checkbox("Include Last", &includeLast);
						ImGui::SameLine(0.0f, 20.0f);
						HelpMarker("Whether to include the last day in the calculation");

						ImGui::Dummy(ImVec2(0.0f, 5.0f));
						ImGui::SetCursorPosX(500.0f - (ImGui::CalcTextSize("Year Fraction XXX").x * 0.5f + 40.0f));

						const char * yearFrac = "365";
						ImGui::PushItemWidth(ImGui::CalcTextSize("X").x * 4.0f);
						ImGui::InputText("Year Fraction", (char*)yearFrac, 8,
							ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();
						ImGui::SameLine(0.0f, 20.0f);
						HelpMarker("Number of days to use as denominator");
						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::Separator();

						//ImGui::Dummy(ImVec2(0.0f, 15.0f));

						const char* availableFields[] = { "Day1", "Day2", "Month1", "Month2", "Year1", 
							"Year2", "Date1", "Date2"};
						const char* availableFunctions[] = {
							"isLastOfFebruary(day, month, year)", 
							"daysBetween(date, date)",
							"businessDaysBetween(date, date)"};

						static std::string function = "if (Day1 == 31) then Day1 = 30 end \n"
							"if (Day2 == 31 and Day1 == 30) then Day2 = 30 end \n"
							"return 360 * (Year2 - Year1) + 30 * (Month2 - Month1) + (Day2 - Day1) end";

						if (ImGui::BeginTabBar("DayCounterTabBar", ImGuiTabBarFlags_None)) {

							if (ImGui::BeginTabItem("Formula", NULL, tabItemFlag)) {
								ImGui::BeginChild("##DayCountDetails", ImVec2(0, 500), false, ImGuiWindowFlags_AlwaysAutoResize);
								{

									ImGui::BeginChild("##EditLeft",
										ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, -1.5f * buttonSz.y), false,
										ImGuiWindowFlags_HorizontalScrollbar);
									{


										if (ImGui::InputTextMultiline("##Code", (char*)function.c_str(),
											1024 * 16, ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_AllowTabInput)) {

										}

									}
									ImGui::EndChild();
									ImGui::SameLine(0.0f, 1.0f);
									ImGui::BeginChild("##EditRight", ImVec2(0.0f, -1.5f * buttonSz.y), true,
										ImGuiWindowFlags_HorizontalScrollbar);
									{
										static int inputOpt = 1;
										ImGui::PushItemWidth(-FLT_MIN);
										ImGui::Combo("##InputOp", &inputOpt, "Functions\0Fields\0");
										ImGui::PopItemWidth();
										ImGui::Separator();

										if (inputOpt == 0) {
											for (int i = 0; i < IM_ARRAYSIZE(availableFunctions); i++) {
												static bool selection = false;
												if (ImGui::Selectable(availableFunctions[i], selection)) {

												}
											}
										}
										else {
											for (int i = 0; i < IM_ARRAYSIZE(availableFields); i++) {
												static bool selection = false;
												if (ImGui::Selectable(availableFields[i], selection)) {

												}
											}
										}
									}
									ImGui::EndChild();
									static std::string status = "";
									if (ImGui::Button("Validate")) {
										

										static std::string header = 
											"function dayCount (Day1, Day2, Month1, Month2, Year1, Year2, Date1, Date2) ";

										static std::string footer = ";r = dayCount(31, 30, 1, 1, 2021, 2023)";

										std::string functionFinal = header + function + footer;

										printf("%s\n", functionFinal.c_str());

										lua_State* L = luaL_newstate(); //We need to start a lua virtual machine
										luaL_openlibs(L); //Opens a set of libraries that a typical prgram will use

										if (luaL_dostring(L, functionFinal.c_str()) == LUA_OK) {
											lua_getglobal(L, "r"); /* function to be called */
											lua_tonumber(L, -1);
											printf("%f\n", lua_tonumber(L, -1));
											status = "No error";
										}
										else {
											status = lua_tostring(L, -1);
										}
										lua_close(L);
										
									}
									ImGui::SameLine();
									ImGui::Text(status.c_str());
								}
								ImGui::EndChild();
								ImGui::Spacing();
								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Test", NULL)) {
								ImGui::Spacing();
								
								ImGui::Spacing();
								ImGui::EndTabItem();
							}
							ImGui::EndTabBar();
						}

						ImGui::SetCursorPosY(1050 - 1.5f * buttonSz.y);

						ImGui::Separator();
						if (ImGui::Button("Close")) {
							ImGui::CloseCurrentPopup();
							openOpenPopup = false;
						}

						if (popupInputFlags == ImGuiInputTextFlags_None) {
							ImGui::SameLine(ImGui::CalcTextSize("Close").x + 30.0f);
							if (ImGui::Button("Save")) {
								ImGui::CloseCurrentPopup();
								openOpenPopup = false;
							}
						}

						ImGui::EndPopup();
					}


					ImGui::PopID();
				}
			}
			clipper.End();
			//Table end
			ImGui::EndTable();
		}
	}
	ImGui::EndChild();
}