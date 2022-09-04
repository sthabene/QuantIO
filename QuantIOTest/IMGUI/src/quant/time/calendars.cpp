#include "../quant.hpp"

#include "boost/algorithm/string.hpp"
#include <time.h>

static std::string query = "SELECT CALENDAR_ID, WEEKEND_HOLIDAY, CALENDAR_LABEL  FROM CALENDAR";
static std::string title = "Calendar###";
static std::vector<std::vector<std::string>> tableData;
static int refresh = 1;
static int dateInit = 1;
static bool showFilter = false;

//Open item
static std::vector<std::string> selectedRow;
static ImGuiInputTextFlags popupInputFlags = ImGuiInputTextFlags_None;
static ImGuiTabItemFlags tabItemFlag = ImGuiTabItemFlags_None;

static ImVec2 buttonSz(25.0f * 5.0f, 32.0f); //To change 

void QuantIOCalendars::DisplayContents() {
	ImGui::BeginChild("Calendars", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
	{
		static const float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 3.0f;

		//Running the query
		if (refresh & 1) {
			tableData = QuantIO::dbConnection.getTableData2(query);
			refresh++;
		}

		//#rows and columns
		static int iRows = tableData.size() - 1;
		static int iColumns = tableData[0].size();

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

		if (ImGui::BeginTable("Calendars", iColumns, QuantIO::tableFlags, ImVec2(0.0f, maxTableHeight), 0.0f)) {

			//Make first row (header) always visible
			ImGui::TableSetupScrollFreeze(0, 1);

			//Column Setup
			for (std::size_t i = 0; i < iColumns; i++) { //Column label
				if (i < 2) {
					ImGui::TableSetupColumn(tableData[0][i].c_str(), ImGuiTableColumnFlags_Disabled,
						ImGui::GetContentRegionAvail().x * 0.15f, i);
				}
				else {
					ImGui::TableSetupColumn(tableData[0][i].c_str(), ImGuiTableColumnFlags_NoHide,
						ImGui::GetContentRegionAvail().x * 0.85f, i);
				}
			}
			ImGui::TableHeadersRow();

			//Clipping display results
			static ImGuiListClipper clipper;
			clipper.Begin(displayRows);

			while (clipper.Step()) {
				for (std::size_t row = clipper.DisplayStart + startCount; row < clipper.DisplayEnd + startCount; row++) {
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
							popupInputFlags = ImGuiInputTextFlags_ReadOnly;
							tabItemFlag = ImGuiTabItemFlags_None;
							selectedRow = *currentRow;
							dateInit = 1;
							openOpenPopup = true;
						};

						if (ImGui::MenuItem("Edit", "Ctrl + Enter")) {
							popupInputFlags = ImGuiInputTextFlags_None;
							openEditPopup = true;
						};
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
						if (ImGui::MenuItem("Select all", "Ctrl + A")) {
							for (std::vector <std::string>& v1 : filteredData) {
								selections.push_back(v1.at(0));
							}

						};
						ImGui::Separator();
						if (ImGui::BeginMenu("Delete")) {
							if (ImGui::MenuItem("Logical Delete", "Del")) {
								openDeletePopup = true;
							};
							ImGui::MenuItem("Physical Delete", "Shift + Del");
							ImGui::EndMenu();
						}
						ImGui::Separator();
						if (ImGui::MenuItem("Exit", "Esc")) {
							openExitPopup = true;

						};
						ImGui::EndPopup();
					}

					//Open Popup
					if (openOpenPopup) {
						title = "Calendar: Read-only mode";
						ImGui::OpenPopup(title.c_str());
						tabItemFlag = ImGuiTabItemFlags_SetSelected;
					}
					else {
						tabItemFlag = ImGuiTabItemFlags_None;
					}

					ImGui::SetNextWindowPos(QuantIO::popupLocation(), ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
					ImGui::SetNextWindowSize(ImVec2(1000, 800), ImGuiCond_FirstUseEver);
					if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {

						ImGui::Spacing();


						float titleWidth = ImGui::CalcTextSize(selectedRow[2].c_str()).x + 15.0f;
						ImGui::SetCursorPosX( 500.0f - titleWidth * 0.5f);
						ImGui::PushItemWidth(titleWidth);
						ImGui::InputText("##Calendar", (char*)selectedRow[2].c_str(), 32,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();
						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Dummy(ImVec2(0.0f, 20.0f));

						static bool weekend = (selectedRow[1] == "1") ? true : false;

						ImGui::SetCursorPosX(500.0f - (ImGui::CalcTextSize("Weekend Holidays").x * 0.5 + 50.0f));
						ImGui::Checkbox("##weekendHolidays", &weekend);
						ImGui::SameLine(0.0f, 20.0f);
						ImGui::TextUnformatted("Weekend Holidays");
						ImGui::SameLine();
						HelpMarker("Whether or not weekends are considered holidays");

						ImGui::Dummy(ImVec2(0.0f, 20.0f));

						if (ImGui::BeginTabBar("CalendarTabBar", ImGuiTabBarFlags_None)) {

							if (ImGui::BeginTabItem("Calendar", NULL, tabItemFlag)) {
								ImGui::Spacing();
								CalendarImplementation();
								ImGui::Spacing();
								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Holiday Rules", NULL)) {

								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Specific Holidays", NULL)) {

								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Find Business Days", NULL)) {

								ImGui::EndTabItem();
							}

							ImGui::EndTabBar();
						}


						ImGui::SetCursorPosY(800 - 1.5f * buttonSz.y);

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
				}
			}
			clipper.End();

			//Table end
			ImGui::EndTable();
		}

	}
	ImGui::EndChild();
}

static void RecalculateDate(tm& date) {
	date.tm_isdst = -1;   //Removes daylight saving time
	time_t tmp = mktime(&date); //Convert tm structure to time_t, reverse of localtime()
	date = *localtime(&tmp); //Convert time_t to tm as local time
}

void CalendarImplementation() {
	const float calendarWidth = 400.0f;
	const float calendarHeight = 340.0f;
	ImGui::BeginChild("CalendarDisplay", ImVec2(calendarWidth, calendarHeight), true, 
		ImGuiWindowFlags_NoResize);
	{


		static time_t now = time(NULL);
		static tm currentDate = *localtime(&now);
		static tm dateOut = *localtime(&now);

		//Running the query
		if (dateInit & 1) {
			dateOut = *localtime(&now);
			currentDate = *localtime(&now);
			currentDate.tm_mday = 1;
			RecalculateDate(currentDate);
			dateInit++;
		}
		
		static const char* dayNames[7] = { "S","M","T","W","T","F","S" };
		static const char* monthNames[12] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
		const static int numDaysPerMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

		static char yearString[12] = "";
		sprintf(yearString, "%d", 1900 + currentDate.tm_year);
		ImGui::Spacing();

		ImGui::PushID(4321);
		ImGui::Indent(25.0f);
		if (ImGui::SmallButton(ICON_FA_CHEVRON_CIRCLE_LEFT)) {
			currentDate.tm_mon -= 1;
			RecalculateDate(currentDate);
		}
		ImGui::SameLine();
		ImGui::Text("%s", monthNames[currentDate.tm_mon]);
		ImGui::SameLine();
		if (ImGui::SmallButton(ICON_FA_CHEVRON_CIRCLE_RIGHT)) {
			currentDate.tm_mon += 1;
			RecalculateDate(currentDate);
		}
		ImGui::PopID();
		ImGui::SameLine(0.0f, 70.0f);
		ImGui::PushID(1235);
		if (ImGui::SmallButton(ICON_FA_CHEVRON_CIRCLE_LEFT)) {
			currentDate.tm_year -= 1;
			if (currentDate.tm_year < 0)
				currentDate.tm_year = 0;
			RecalculateDate(currentDate);
		}
		ImGui::SameLine();
		ImGui::Text("%d", 1900 + currentDate.tm_year);
		ImGui::SameLine();
		if (ImGui::SmallButton(ICON_FA_CHEVRON_CIRCLE_RIGHT)) {
			currentDate.tm_year += 1;
			RecalculateDate(currentDate);
		}
		ImGui::PopID();
		ImGui::Unindent(25.0f);
		ImGui::Spacing();

		ImGui::Separator();

		int daysCurMonth = numDaysPerMonth[currentDate.tm_mon];

		//Check leap year
		if (daysCurMonth == 28) {
			const int year = currentDate.tm_year + 1900;
			if ((year % 4 == 0) && ((year % 100 != 0) || (year % 400) == 0)) {
				daysCurMonth = 29;
			}
		}

		static char curDayStr[3] = "";

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.17f, 0.31f, 0.46f, 0.80f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.17f, 0.31f, 0.46f, 0.80f));

		for (int dw = 0; dw < 7; dw++) {
			ImGui::BeginGroup();

			if (dw == 0 || dw == 6) {
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.667f, 0.167f, 0.167f, 1.0f));
			}
			ImGui::Text("  %s", dayNames[dw]);
			ImGui::Separator();
			ImGui::Spacing();
			if (dw == 0 || dw == 6) {
				ImGui::PopStyleColor();
			}

			int curDay = dw - currentDate.tm_wday;

			for (int row = 0; row < 6; row++) {
				int cday = curDay + 7 * row;
				if (cday >= 0 && cday < daysCurMonth) {
					ImGui::PushID(row * 10 + dw);
					if (cday < 9) {
						sprintf(curDayStr, " %d", cday + 1);
					}
					else {
						sprintf(curDayStr, "%d", cday + 1);
					}
					//Color
					if (dw == 0 || dw == 6 || (currentDate.tm_mon == 11 && cday + 1 == 16) ) {
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.667f, 0.167f, 0.167f, 1.0f));
					}
					if (ImGui::SmallButton(curDayStr)) {
						ImGui::SetItemDefaultFocus();
						dateOut.tm_mday = cday + 1;
						dateOut.tm_year = currentDate.tm_year;
						dateOut.tm_mon = currentDate.tm_mon;
						RecalculateDate(dateOut);
					}
					if (dw == 0 || dw == 6 || (currentDate.tm_mon == 11 && cday + 1 == 16) ) {
						ImGui::PopStyleColor();
					}
					
					if (dw == 0 && ImGui::IsItemHovered()) {
						ImGui::SetTooltip("Sunday");
					}
					if (dw == 6 && ImGui::IsItemHovered()) {
						ImGui::SetTooltip("Saturday");
					}
					if (currentDate.tm_year == 2022 && currentDate.tm_mon == 11 && cday + 1 == 16) {
						ImGui::SetTooltip("Day of Reconciliation");
					}
					
					ImGui::PopID();
				}
				else {
					ImGui::TextUnformatted(" ");
				}
			}


			ImGui::EndGroup();
			if (dw != 6) ImGui::SameLine(ImGui::GetWindowWidth() - (6 - dw) * (ImGui::GetWindowWidth() / 7.f));
		}

		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		if (dateInit & 1) {
			currentDate = *localtime(&now);
			RecalculateDate(currentDate);
			dateInit++;
		}

		

		ImGui::Separator();
		static char currentDateText[128];
		strftime(currentDateText, 128, "%d/%m/%Y", &dateOut);
		ImGui::PushItemWidth(35.0f * 5.0f);
		ImGui::InputText("##customDate", currentDateText, 128, 
			ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly);



	}
	ImGui::EndChild();
}