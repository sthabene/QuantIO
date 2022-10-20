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

						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::Separator();

						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						const char* availableFields[] = { "Day 1", "Day 2", "Month 1", "Month 2", "Year 1", 
							"Year 2", "Date 1", "Date 2"};
						const char* availableFunctions[] = { "$if(bool, value, value)", "$isLastOfFebruary(day, month, year)", 
						"$daysBetween(date, date)", "$equal(value, value)"};

						//Selection details
						const char* dayCountObjects[] = { "###Empty", "Day 1", "Day 2", "Month 1", "Month 2", "Year 1",
							"Year 2", "Date 1", "Date 2", "Result"};
						
						static std::vector<int> selectedIndex;
						static std::vector<int> selectedItemValues = { 1, 2, 1, 2, 6, 4, 4, 6, 8, 7, 9 };

						ImGui::BeginChild("##DayCountDetails", ImVec2(0, 500), false, ImGuiWindowFlags_AlwaysAutoResize);
						{
							if (ImGui::BeginTable("DayCountersDetails", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ContextMenuInBody, 
								ImVec2(0.0f, 0.0f), 0.0f)) {
								ImGui::TableSetupScrollFreeze(0, 1);
								ImGui::TableSetupColumn("Field", ImGuiTableColumnFlags_NoHide, 20.0f);
								ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_NoHide, 40.0f);
								ImGui::TableSetupColumn("Condition", ImGuiTableColumnFlags_NoHide, 40.0f);

								ImGui::TableHeadersRow();

								for (int row_n = 0; row_n < selectedItemValues.size(); row_n++) {

									ImGui::PushID(row_n + 300);

									ImGui::TableNextRow(ImGuiTableRowFlags_None, rowHeight);


									if (ImGui::TableSetColumnIndex(0)) {
										char label[32];
										sprintf(label, "%04d", row_n);

										//ImGui::Button(label, ImVec2(-FLT_MIN, 0.0f));
										ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
										ImGui::PushItemWidth(-FLT_MIN);

										int ivalue = row_n >= selectedItemValues.size() ? 0 : selectedItemValues[row_n];

										if (ImGui::BeginCombo("##Combo", dayCountObjects[ivalue],
											ImGuiComboFlags_NoArrowButton)) {
											for (int n = 0; n < IM_ARRAYSIZE(dayCountObjects); n++) {
												const bool isSelected = (ivalue == n);
												if (ImGui::Selectable(dayCountObjects[n], isSelected)) {
													ivalue = n;
													selectedItemValues[row_n] = n;
												}
												if (isSelected) {
													ImGui::SetItemDefaultFocus();
												}
											}
											ImGui::EndCombo();
										}
										ImGui::PopItemWidth();
										ImGui::PopStyleVar();

									};

									if (ImGui::TableSetColumnIndex(1)) {
										static char value[8];
										ImGui::PushItemWidth(-FLT_MIN-31.0f);
										ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
										ImGui::InputText("##value", value, 8, ImGuiInputTextFlags_AutoSelectAll);
										ImGui::PopItemWidth();
										ImGui::SameLine(0.0f, 1.0f);
										ImGui::PushID(1235);
										if (ImGui::Button("...", ImVec2(30.0f, buttonSz.y))) {
											ImGui::OpenPopup("EditModal");
										}


										ImGui::SetNextWindowPos(QuantIO::popupLocation(ImGui::GetWindowPos(), (float)row_n),
											ImGuiCond_Appearing,
											ImVec2(0.0f, 0.0f));
										ImGui::SetNextWindowSize(ImVec2(1000, 500), ImGuiCond_FirstUseEver);

										if (ImGui::BeginPopupModal("EditModal", NULL,
											ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
											ImGuiWindowFlags_NoTitleBar)) {

											ImGui::BeginChild("##EditLeft",
												ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, -1.5f * buttonSz.y), false,
												ImGuiWindowFlags_AlwaysAutoResize);
											{
												static char InputBuf[1024 * 16] = { '\0' };
												if (ImGui::InputTextMultiline("##source", InputBuf, IM_ARRAYSIZE(InputBuf),
													ImVec2(-FLT_MIN, -FLT_MIN))) {

												}
											}
											ImGui::EndChild();
											ImGui::SameLine(0.0f, 1.0f);
											ImGui::BeginChild("##EditRight", ImVec2(0.0f, -1.5f * buttonSz.y), true,
												ImGuiWindowFlags_HorizontalScrollbar);
											{
												static int inputOpt = 0;
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

											//ImGui::SetCursorPosY(500 - 1.5f * buttonSz.y);

											ImGui::Separator();
											if (ImGui::Button("Close")) {
												ImGui::CloseCurrentPopup();
											}

											if (popupInputFlags == ImGuiInputTextFlags_None) {
												ImGui::SameLine(ImGui::CalcTextSize("Close").x + 30.0f);
												if (ImGui::Button("Save")) {
													ImGui::CloseCurrentPopup();
												}
											}


											ImGui::EndPopup();
										}
										ImGui::PopID();
										ImGui::PopStyleVar();
									}

									
									if (ImGui::TableSetColumnIndex(2)) {
										static char formula[255];
										ImGui::PushItemWidth(-FLT_MIN-93.0f);
										ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
										ImGui::InputText("##Formula", formula, 255, ImGuiInputTextFlags_AutoSelectAll);
										ImGui::PopStyleVar();
										ImGui::PopItemWidth();
										ImGui::SameLine(0.0f, 0.0f);
										ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
										ImGui::SameLine(0.0f, 1.0f);
										if (ImGui::Button("-", ImVec2(30.0f, buttonSz.y))) {
											selectedItemValues.erase(selectedItemValues.begin() + row_n);
										}
										ImGui::SameLine(0.0f, 1.0f);
										if (ImGui::Button("+", ImVec2(30.0f, buttonSz.y))) {
											selectedItemValues.insert(selectedItemValues.begin() + row_n + 1, 0);
										}
										ImGui::SameLine(0.0f, 1.0f);
										ImGui::PushID(1234);
										if (ImGui::Button("...", ImVec2(30.0f, buttonSz.y))) {
											ImGui::OpenPopup("EditModal");
										}
										

										ImGui::SetNextWindowPos(QuantIO::popupLocation(ImGui::GetWindowPos(), (float)row_n),
											ImGuiCond_Appearing,
											ImVec2(0.0f, 0.0f));
										ImGui::SetNextWindowSize(ImVec2(1000, 500), ImGuiCond_FirstUseEver);

										if (ImGui::BeginPopupModal("EditModal", NULL,
											ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
											ImGuiWindowFlags_NoTitleBar)) {

											ImGui::BeginChild("##EditLeft",
												ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, -1.5f * buttonSz.y), false,
												ImGuiWindowFlags_AlwaysAutoResize);
											{
												static char InputBuf[1024 * 16] = { '\0' };
												if (ImGui::InputTextMultiline("##source", InputBuf, IM_ARRAYSIZE(InputBuf),
													ImVec2(-FLT_MIN, -FLT_MIN))) {

												}
											}
											ImGui::EndChild();
											ImGui::SameLine(0.0f, 1.0f);
											ImGui::BeginChild("##EditRight", ImVec2(0.0f, -1.5f * buttonSz.y), true,
												ImGuiWindowFlags_HorizontalScrollbar);
											{
												static int inputOpt = 0;
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

											//ImGui::SetCursorPosY(500 - 1.5f * buttonSz.y);

											ImGui::Separator();
											if (ImGui::Button("Close")) {
												ImGui::CloseCurrentPopup();
											}

											if (popupInputFlags == ImGuiInputTextFlags_None) {
												ImGui::SameLine(ImGui::CalcTextSize("Close").x + 30.0f);
												if (ImGui::Button("Save")) {
													ImGui::CloseCurrentPopup();
												}
											}


											ImGui::EndPopup();
										}
										ImGui::PopID();
										
										ImGui::PopStyleVar();
									};

									if (ImGui::BeginPopupContextItem("DayCountContextPopup")) {
										if (ImGui::MenuItem("Delete", NULL)) {
											selectedItemValues.pop_back();
										}
										ImGui::EndPopup();
									}

									ImGui::PopID();

								}

								ImGui::EndTable();
							}
						}
						ImGui::EndChild();

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