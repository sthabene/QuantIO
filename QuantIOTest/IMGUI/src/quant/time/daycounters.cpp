#pragma once
#include "../quant.hpp"
#include "time/daycounter.hpp"

#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"

static std::string query = "SELECT COUNTER_ID, COUNTER_NAME, COUNTER_DESCR FROM DAY_COUNTER ORDER BY COUNTER_NAME";
static std::string title = "DayCounter###";
static std::vector<std::vector<std::string>> tableData;
static int refresh = 1;
static bool showFilter = false;

//Open item
static std::vector<std::string> selectedRow;
static ImGuiInputTextFlags popupInputFlags = ImGuiInputTextFlags_None;
static ImGuiTabItemFlags tabItemFlag = ImGuiTabItemFlags_None;
static bool includeLast;
static std::string yearFracStatus = "";
static std::string dayFuncStatus = "";

//Day counter items
float dayCountWidth = 0.f;

//QuantLib
CustomDayCounter mainDayCounter;
static int calsInit = 1;

//IMGUI Display
static ImVec2 buttonSz(25.0f * 5.0f, 32.0f); //To change 
static unsigned int iRows = 0;
static unsigned int iColumns = 0;

void QuantIODayCounters::DisplayContents() {
	ImGui::BeginChild("DayCounters", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
	{
		static const float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 3.0f;

		//Running the query
		if (refresh & 1) {
			tableData = QuantIO::dbConnection.getTableData2(query);
			iRows = tableData.size() - 1;
			iColumns = tableData[0].size();
			refresh++;
		}

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
							boost::format openQuery = boost::format("SELECT COUNTER_ID, COUNTER_NAME, COUNTER_DESCR, DAY_COUNT, YEAR_FRAC FROM DAY_COUNTER WHERE COUNTER_ID = %1%") % 
								currentRow->at(0);
							selectedRow = QuantIO::dbConnection.getTableData2(openQuery.str(), false, false)[0];

							includeLast = false;
							mainDayCounter.setAnotherDayCounter(selectedRow[1], includeLast, selectedRow[3],
								selectedRow[4]);

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

						dayFuncStatus = "";
						yearFracStatus = "";
					}
					else {
						tabItemFlag = ImGuiTabItemFlags_None;
					}

					ImGui::SetNextWindowPos(QuantIO::popupLocation(), ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
					ImGui::SetNextWindowSize(ImVec2(1200, 1050), ImGuiCond_FirstUseEver);

					if (ImGui::BeginPopupModal(title.c_str(), NULL, 
						ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {

						ImGui::SetCursorPosX(1100.0f);
						ImGui::PushItemWidth(ImGui::CalcTextSize(selectedRow[0].c_str()).x + 15.0f);
						ImGui::InputText(" Id", (char*)selectedRow[0].c_str(), 4,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();
						ImGui::Spacing();

						float titleWidth = ImGui::CalcTextSize(selectedRow[1].c_str()).x + 15.0f;
						float descrWidth = ImGui::CalcTextSize(selectedRow[2].c_str()).x > 1000 ? 1000 : 
						ImGui::CalcTextSize(selectedRow[2].c_str()).x + 15.0f;

						ImGui::SetCursorPosX(600.0f - (titleWidth + 50.0f) * 0.5f);
						ImGui::PushItemWidth(titleWidth);
						ImGui::InputText("##DayCounter", (char*)selectedRow[1].c_str(), 32,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();


						ImGui::Dummy(ImVec2(0.0f, 15.0f));
						ImGui::SetCursorPosX(600.0f - (ImGui::CalcTextSize("Description").x + 50.0f) * 0.5f);
						ImGui::TextDisabled("Description");

						ImGui::SetCursorPosX(600.0f - (descrWidth + 50.0f) * 0.5f);
						//ImGui::PushItemWidth(descrWidth);
						ImGui::InputTextMultiline("##Description", (char*)selectedRow[2].c_str(), 255,
							ImVec2(descrWidth, 35.0f),
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						//ImGui::PopItemWidth();
						/*ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::Separator();

						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::SetCursorPosX(600.0f - (ImGui::CalcTextSize("Include Last").x * 0.5f + 50.0f));

						if (ImGui::Checkbox("Include Last", &includeLast)) {
							if (includeLast) {
								mainDayCounter.setAnotherDayCounter(selectedRow[1], includeLast, selectedRow[3],
									selectedRow[4]);
								calsInit++;
							}
							else {
								mainDayCounter.setAnotherDayCounter(selectedRow[1], includeLast, selectedRow[3],
									selectedRow[4]);
								calsInit++;
							}
						};

						ImGui::SameLine(0.0f, 20.0f);
						HelpMarker("Whether to include the last day in the calculation");*/

						/*ImGui::Dummy(ImVec2(0.0f, 5.0f));
						ImGui::SetCursorPosX(600.0f - (ImGui::CalcTextSize("Year Fraction XXX").x * 0.5f + 40.0f));

						static std::string yearFrac = "365";
						ImGui::PushItemWidth(ImGui::CalcTextSize("X").x * 4.0f);
						ImGui::InputText("Year Fraction", (char*)yearFrac.c_str(), 8,
							ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();
						ImGui::SameLine(0.0f, 20.0f);
						HelpMarker("Number of days to use as denominator");*/
						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::Separator();

						//ImGui::Dummy(ImVec2(0.0f, 15.0f));

						std::string dayCountfunction = selectedRow[3];
						std::string yearFracfunction = selectedRow[4];

						std::stringstream dayCountFuncStream(dayCountfunction);
						std::stringstream yearFracFuncStream(yearFracfunction);						

						if (openOpenPopup) {
							size_t lenDayCount = 0;
							std::string dayCountLongLine = "";
							for (std::string line; std::getline(dayCountFuncStream, line, '\n');) {
								size_t len = line.length();
								if (len > lenDayCount) {
									lenDayCount = len;
									dayCountLongLine = line;
								}
							}
							size_t lenYearFrac = 0;
							for (std::string line; std::getline(yearFracFuncStream, line, '\n');) {
								size_t len = line.length();
								if (len > lenYearFrac) {
									lenYearFrac = len;
								}
							}
							dayCountWidth = ImGui::CalcTextSize(dayCountLongLine.c_str()).x + 30.0f;
						}						

						const char* availableFields[] = { "Day1", "Day2", "Month1", "Month2", "Year1", 
							"Year2", "Date1", "Date2"};
						const char* dayCountFunctions[] = {
							"date (day, month, year)",
							"isLastOfFebruary (day, month, year)",
							"daysBetween (Day1, Day2, Month1, Month2, Year1, Year2)",
							"businessDaysBetween (Day1, Day2, Month1, Month2, Year1, Year2)",
							"dayCount (Day1, Day2, Month1, Month2, Year1, Year2)",
							"yearFraction (Day1, Day2, Month1, Month2, Year1, Year2)" };
						const char* yearFractionFunctions[] = {
							"date (day, month, year)",
							"isLastOfFebruary (day, month, year)",
							"daysBetween (Day1, Day2, Month1, Month2, Year1, Year2)",
							"businessDaysBetween (Day1, Day2, Month1, Month2, Year1, Year2)",
							"dayCount (Day1, Day2, Month1, Month2, Year1, Year2)",
							"yearFraction (Day1, Day2, Month1, Month2, Year1, Year2)"};

						if (ImGui::BeginTabBar("DayCounterTabBar", ImGuiTabBarFlags_None)) {

							if (ImGui::BeginTabItem("Formula", NULL, tabItemFlag)) {
								static const char* configItems[] = { "Day Count", "Year Fraction" };
								static int selected = 0;
								{ //Left
									ImGui::BeginChild("Left pane", ImVec2(150, -1.5f * buttonSz.y), true);
									for (int i = 0; i < IM_ARRAYSIZE(configItems); i++) {
										char label[32];
										sprintf(label, configItems[i]);
										if (ImGui::Selectable(label, selected == i)) {
											selected = i;
										}
									}
									ImGui::EndChild();
								}
								ImGui::SameLine();
								{//Right
									if (selected == 0) {
										ImGui::BeginChild("##DayCount", ImVec2(0, 500), false,
											ImGuiWindowFlags_AlwaysAutoResize);
										{
											float availableRegion = ImGui::GetContentRegionAvail().x * 0.75f;
											ImGui::BeginChild("##DayCountLeft",
												ImVec2(availableRegion, -1.5f * buttonSz.y),
												false, ImGuiWindowFlags_HorizontalScrollbar);
											{
												ImGui::SetScrollY(ImGui::GetScrollMaxY());
												if (ImGui::InputTextMultiline("##Code", (char*)selectedRow[3].c_str(),
													1024 * 16, ImVec2(dayCountWidth > availableRegion ? 
														dayCountWidth : availableRegion, -FLT_MIN),
													ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly)) {
												}
											}
											ImGui::EndChild();
											ImGui::SameLine(0.0f, 5.0f);
											ImGui::BeginChild("##EditRight", ImVec2(0.0f, -1.5f * buttonSz.y), true,
												ImGuiWindowFlags_HorizontalScrollbar);
											{
												static int inputOpt = 1;
												ImGui::PushItemWidth(-FLT_MIN);
												ImGui::Combo("##InputOp", &inputOpt, "Functions\0Fields\0");
												ImGui::PopItemWidth();
												ImGui::Separator();

												if (inputOpt == 0) {
													for (int i = 0; i < IM_ARRAYSIZE(dayCountFunctions); i++) {
														static bool selection = false;
														if (ImGui::Selectable(dayCountFunctions[i], selection)) {

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
											
											if (ImGui::Button("Validate")) {
												std::string result = ";r = dayCount(31, 30, 1, 1, 2021, 2023)";
												lua_State* L = luaL_newstate();
												luaL_openlibs(L);
												lua_pushcfunction(L, isLastOfFebruaryLua);
												lua_setglobal(L, "isLastOfFebruary");

												lua_pushcfunction(L, daysBetweenLua);
												lua_setglobal(L, "daysBetween");

												printf("%s\n", (dayCountfunction + result).c_str());

												if (luaL_dostring(L, (dayCountfunction + result).c_str()) == LUA_OK) {
													lua_getglobal(L, "r");
													dayFuncStatus = lua_tonumber(L, -1) > 0.0 ? "No error" : "Error";
													dayCountfunction = "";
												}
												else {
													dayFuncStatus = lua_tostring(L, -1);
													dayCountfunction = "";
												}
												lua_close(L);

											}
											ImGui::SameLine();
											ImGui::Text(dayFuncStatus.c_str());
										}
										ImGui::EndChild();
										ImGui::Spacing();
									}

									if (selected == 1) {
										ImGui::BeginChild("##YearFracCount", ImVec2(0, 500), false,
											ImGuiWindowFlags_AlwaysAutoResize);
										{

											ImGui::BeginChild("##YearFracLeft",
												ImVec2(ImGui::GetContentRegionAvail().x * 0.75f, -1.5f * buttonSz.y),
												false, ImGuiWindowFlags_HorizontalScrollbar);
											{
												if (ImGui::InputTextMultiline("##YearFracCode", 
													(char*)yearFracfunction.c_str(),
													1024 * 16, ImVec2(-FLT_MIN, -FLT_MIN),
													ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_ReadOnly)) {
												}
											}
											ImGui::EndChild();
											ImGui::SameLine(0.0f, 1.0f);
											ImGui::BeginChild("##YearFracRight", ImVec2(0.0f, -1.5f * buttonSz.y), true,
												ImGuiWindowFlags_HorizontalScrollbar);
											{
												static int inputOpt = 1;
												ImGui::PushItemWidth(-FLT_MIN);
												ImGui::Combo("##InputOp", &inputOpt, "Functions\0Fields\0");
												ImGui::PopItemWidth();
												ImGui::Separator();

												if (inputOpt == 0) {
													for (int i = 0; i < IM_ARRAYSIZE(yearFractionFunctions); i++) {
														static bool selection = false;
														if (ImGui::Selectable(yearFractionFunctions[i], selection)) {

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
											
											if (ImGui::Button("Validate")) {

												std::string functions = dayCountfunction + "; " + yearFracfunction;
												std::string yearFracfooter = ";r = yearFraction(31, 30, 1, 1, 2021, 2023)";

												lua_State* L = luaL_newstate();
												luaL_openlibs(L);

												lua_pushcfunction(L, isLastOfFebruaryLua);
												lua_setglobal(L, "isLastOfFebruary");

												lua_pushcfunction(L, daysBetweenLua);
												lua_setglobal(L, "daysBetween");

												if (luaL_dostring(L, (functions + yearFracfooter).c_str()) == LUA_OK) {
													lua_getglobal(L, "r");
													yearFracStatus = lua_tonumber(L, -1) > 0.0 ? "No error" : "Error";
												}
												else {
													yearFracStatus = lua_tostring(L, -1);
												}
												lua_close(L);

											}
											ImGui::SameLine();
											ImGui::Text(yearFracStatus.c_str());
										}
										ImGui::EndChild();
										ImGui::Spacing();
									}

								}
								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Test", NULL)) {
								ImGui::Spacing();

								static tm Date1 = CreateDateNow();
								static tm Date2 = CreateDate(Date1.tm_mday, Date1.tm_mon + 1, Date1.tm_year + 1900);

								if (openOpenPopup) {
									Date1 = CreateDateNow();
									Date2 = CreateDate(Date1.tm_mday, Date1.tm_mon + 1, Date1.tm_year + 1900);
									calsInit++;
								}

								static int dayCountResult = 0;
								static double yearFractionResult = 0.0;

								if ((calsInit & 1)) {

									dayCountResult = mainDayCounter.dayCount(ConvertToQlDate(Date1),
										ConvertToQlDate(Date2));

									yearFractionResult = mainDayCounter.yearFraction(ConvertToQlDate(Date1),
										ConvertToQlDate(Date2));

									calsInit++;
								}
								ImGui::Dummy(ImVec2(0.0f, 10.0f));
								ImGui::TextDisabled("Inputs");
								ImGui::Separator();

								ImGui::Indent(40.0f);
								ImGui::Dummy(ImVec2(0.0f, 5.0f));
								ImGui::AlignTextToFramePadding();
								ImGui::TextUnformatted("1st Date: "); 
								ImGui::SameLine(0.f, 150.0f - ImGui::CalcTextSize("1st Date: ").x);
								ImGui::PushItemWidth(35.0f * 3.5f);
								if (ImGui::DateChooser2("##1stDate", Date1, "%Y-%m-%d", false, NULL,
									ICON_FA_CHEVRON_CIRCLE_LEFT, ICON_FA_CHEVRON_CIRCLE_RIGHT)) {

									QuantLib::Date date1 = ConvertToQlDate(Date1);
									QuantLib::Date date2 = ConvertToQlDate(Date2);

									dayCountResult = mainDayCounter.dayCount(date1, date2);
									yearFractionResult = mainDayCounter.yearFraction(date1, date2);

								}
								ImGui::Dummy(ImVec2(0.0f, 5.0f));
								ImGui::AlignTextToFramePadding();
								ImGui::TextUnformatted("2nd Date: "); 
								ImGui::SameLine(0.f, 150.0f - ImGui::CalcTextSize("2nd Date: ").x);
								if (ImGui::DateChooser2("##2ndDate", Date2, "%Y-%m-%d", false, NULL,
									ICON_FA_CHEVRON_CIRCLE_LEFT, ICON_FA_CHEVRON_CIRCLE_RIGHT)) {

									QuantLib::Date date1 = ConvertToQlDate(Date1);
									QuantLib::Date date2 = ConvertToQlDate(Date2);

									dayCountResult = mainDayCounter.dayCount(date1, date2);
									yearFractionResult = mainDayCounter.yearFraction(date1, date2);

								}
								ImGui::Unindent(40.0f);
								ImGui::Dummy(ImVec2(0.0f, 20.0f));
								ImGui::TextDisabled("Results");
								ImGui::Separator();
								ImGui::Dummy(ImVec2(0.0f, 5.0f));
								ImGui::AlignTextToFramePadding();
								ImGui::Indent(40.0f);
								ImGui::TextUnformatted("Day Count:");
								ImGui::SameLine(0.f, 150.0f - ImGui::CalcTextSize("Day Count:").x);
								ImGui::InputText("##DayCount", (char*)std::to_string(abs(dayCountResult)).c_str(), 8,
									ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
								

								ImGui::Dummy(ImVec2(0.0f, 5.0f));
								ImGui::AlignTextToFramePadding();
								ImGui::TextUnformatted("Year Fraction:");
								ImGui::SameLine(0.f, 150.0f - ImGui::CalcTextSize("Year Fraction:").x);
								ImGui::InputText("##YearFraction", 
									(char*)std::to_string(abs(yearFractionResult)).c_str(), 8,
									ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
								

								ImGui::Unindent(40.0f);
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