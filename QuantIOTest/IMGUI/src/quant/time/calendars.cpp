//Disable warnings
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../quant.hpp"

#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "boost/regex.hpp"
#include "ql/time/calendar.hpp"


static std::string query = "SELECT CALENDAR_ID, CALENDAR_LABEL, REGION, MARKET FROM CALENDAR ORDER BY REGION, CALENDAR_LABEL";
static std::string title = "Calendar###";
static std::vector<std::vector<std::string>> tableData;
static int refresh = 1;
static int dateInit = 1;
static int holidayInit = 1;
static int holidayListInit = 1;
static bool showFilter = false;

//Open item
static std::vector<std::string> selectedRow;
std::vector<std::string> implementations;
static std::vector<std::string> selectedImpl;
static ImGuiInputTextFlags popupInputFlags = ImGuiInputTextFlags_None;
static ImGuiTabItemFlags tabItemFlag = ImGuiTabItemFlags_None;


//Calendar Items
static std::vector<std::vector<std::string>> holidays;
static std::vector<std::vector<std::string>> holidaysInMonth;
static int holidaysInMonthInit = 1;
static int holidaysToShow = 3;

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

							boost::format calQuery = boost::format("SELECT T1.CALENDAR_ID, T1.CALENDAR_LABEL, T1.REGION, T1.MARKET, T2.IMPLEM_DESC, T2.WEEKEND FROM CALENDAR T1, IMPLEMENTATION T2 WHERE T1.IMPLEMENTATION = T2.IMPLEM_ID AND T1.CALENDAR_ID = %1%") % currentRow->at(0);
							selectedRow = QuantIO::dbConnection.getTableData2(calQuery.str(), false, false)[0];

							std::string implQuery = "SELECT IMPLEM_DESC FROM IMPLEMENTATION";
							implementations = QuantIO::dbConnection.getTableData2(implQuery, false, true)[0];

							//selectedRow = *currentRow;
							dateInit = 1;
							holidaysInMonthInit = 1;
							holidayListInit = 1;
							holidaysToShow = 3;
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
					ImGui::SetNextWindowSize(ImVec2(1000, 1000), ImGuiCond_FirstUseEver);
					if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {

						ImGui::SetCursorPosX(900.0f);
						ImGui::PushItemWidth(ImGui::CalcTextSize(selectedRow[0].c_str()).x + 15.0f);
						ImGui::InputText(" Id", (char*)selectedRow[0].c_str(), 4,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();
						ImGui::Spacing();


						float titleWidth = ImGui::CalcTextSize(selectedRow[1].c_str()).x + 15.0f;
						float regionWidth = ImGui::CalcTextSize(selectedRow[2].c_str()).x + 15.0f;

						ImGui::SetCursorPosX( 500.0f - (titleWidth + regionWidth + 50.0f) * 0.5f);
						ImGui::PushItemWidth(titleWidth);
						ImGui::InputText("##Calendar", (char*)selectedRow[1].c_str(), 32,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();

						ImGui::SameLine();
						ImGui::TextUnformatted(" - ");
						ImGui::SameLine();

						ImGui::PushItemWidth(regionWidth);
						ImGui::InputText("##CalendarRegion", (char*)selectedRow[2].c_str(), 32,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();
						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						ImGui::Separator();

						ImGui::Dummy(ImVec2(0.0f, 15.0f));

						const char* market[2] = { "Exchange", "Settlement" };
						int selectedItemMarket = selectedRow[3] == "Exchange" ? 0 : 1;
						static int itemCurrent = 0;
						if (openEditPopup || openOpenPopup) {
							itemCurrent = selectedItemMarket;
						}

						ImGui::Indent(320.0f);
						ImGui::PushItemWidth(rowHeight * 8.0f);
						if (ImGui::BeginCombo(" Market", market[itemCurrent],
							ImGuiComboFlags_PopupAlignLeft)) {
							for (int n = 0; n < IM_ARRAYSIZE(market); n++) {
								const bool isSelected = (itemCurrent == n);
								if (ImGui::Selectable(market[n], isSelected)) {
									itemCurrent = n;
								}
								if (isSelected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
					
						ptrdiff_t pos = std::find(implementations.begin(), implementations.end(), selectedRow.at(4))
							- implementations.begin();

						static int currentImpIndex = 0;
						static std::string currentImpl = "Western";
						static std::string currentWeek = "17";

						if (openEditPopup || openOpenPopup) {
							currentImpIndex = pos;
							currentImpl = selectedRow.at(4).c_str();
							currentWeek = selectedRow.at(5);
						}
						currentImpl = implementations[currentImpIndex];

						if (ImGui::BeginCombo(" Implementation", currentImpl.c_str())) {
							for (int n = 0; n < implementations.size(); n++) {
								const bool isSelected = (currentImpIndex == n);
								if (ImGui::Selectable(implementations[n].c_str(), isSelected)) {
									currentImpIndex = n;
									boost::format implIndexQuery = boost::format(
										"SELECT IMPLEM_DESC, WEEKEND FROM IMPLEMENTATION WHERE IMPLEM_DESC = '%1%'") % 
										implementations[currentImpIndex];
									selectedImpl = QuantIO::dbConnection.getTableData2(
										implIndexQuery.str(), false, false)[0];
									currentWeek = selectedImpl[1];
								}
								if (isSelected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}
						ImGui::SameLine();
						HelpMarker("Drives weekend days and easter holidays");

						ImGui::Unindent(320.0f);
						ImGui::PopItemWidth();

						ImGui::Dummy(ImVec2(0.0f, 20.0f));

						ImGui::SetCursorPosX(500.0f - (ImGui::CalcTextSize("Weekend days").x * 0.5f + 50.0f));
						ImGui::TextUnformatted("Weekend days");

						bool sun = boost::algorithm::contains(currentWeek, "1");
						bool mon = boost::algorithm::contains(currentWeek, "2");
						bool tue = boost::algorithm::contains(currentWeek, "3");
						bool wed = boost::algorithm::contains(currentWeek, "4");
						bool thu = boost::algorithm::contains(currentWeek, "5");
						bool fri = boost::algorithm::contains(currentWeek, "6");
						bool sat = boost::algorithm::contains(currentWeek, "7");

						ImGui::Indent(150.0f);
						ImGui::Checkbox("Sun", &sun);ImGui::SameLine(0.0f, 20.0f);
						ImGui::Checkbox("Mon", &mon);ImGui::SameLine(0.0f, 20.0f);
						ImGui::Checkbox("Tue", &tue);ImGui::SameLine(0.0f, 20.0f);
						ImGui::Checkbox("Wed", &wed);ImGui::SameLine(0.0f, 20.0f);
						ImGui::Checkbox("Thu", &thu);ImGui::SameLine(0.0f, 20.0f);
						ImGui::Checkbox("Fri", &fri);ImGui::SameLine(0.0f, 20.0f);
						ImGui::Checkbox("Sat", &sat);
						ImGui::Unindent(150.0f);

						ImGui::Dummy(ImVec2(0.0f, 20.0f));

						if (ImGui::BeginTabBar("CalendarTabBar", ImGuiTabBarFlags_None)) {
							if (ImGui::BeginTabItem("Calendar", NULL, tabItemFlag)) {
								ImGui::Spacing();
								CalendarImplementation(currentWeek, selectedRow[0]);
								ImGui::Spacing();
								ImGui::EndTabItem();
							}
							/*if (ImGui::BeginTabItem("Holiday Rules", NULL)) {

								ImGui::EndTabItem();
							}*/
							if (ImGui::BeginTabItem("Adhoc Holidays", NULL)) {
								ImGui::Spacing();
								CalendarAdhocHolidays();
								ImGui::Spacing();
								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Find Business Days", NULL)) {
								ImGui::Spacing();
								
								ImGui::Indent(40.0f);

								static tm Date1 = QuantIO::CreateDateNow();
								static tm Date2 = QuantIO::CreateDateNow();
								
								ImGui::PushItemWidth(35.0f * 4.0f);
								if (ImGui::DateChooser2("First Date", Date1, "%Y-%m-%d", false, NULL, 
									ICON_FA_CHEVRON_CIRCLE_LEFT, ICON_FA_CHEVRON_CIRCLE_RIGHT)) {
								}
								if (ImGui::DateChooser("Second Date", Date2, "%Y-%m-%d", false, NULL,
									ICON_FA_CHEVRON_CIRCLE_LEFT, ICON_FA_CHEVRON_CIRCLE_RIGHT)) {
								}

								ImGui::Unindent(40.0f);
								ImGui::Spacing();

								ImGui::EndTabItem();
							}
							ImGui::EndTabBar();
						}


						ImGui::SetCursorPosY(1000 - 1.5f * buttonSz.y);

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
};

static void RecalculateDate(tm& date) {
	date.tm_isdst = -1;   //Removes daylight saving time
	tm minDate = QuantIO::CreateDate(2, 1, 1970);
	time_t tmp; //Convert tm structure to time_t, reverse of localtime()
	if (mktime(&date) < mktime(&minDate)) {
		tmp = mktime(&minDate);
	}
	else {
		tmp = mktime(&date);
	}
	date = *localtime(&tmp); //Convert time_t to tm as local time
};

void FindPublicHolidays(tm& currentDate, std::vector<std::vector<std::string>>& holidayList) {
	std::vector<std::vector<std::string>> holidaysInMonth;
	int numOfHolidays = holidayList.size();
	for (std::vector<std::string> holiday : holidayList) {

	}
}

static tm dateOut;

void CalendarImplementation(std::string& weekend, std::string& calendarId) {

	ImGui::BeginChild("##CalendarLeft", ImVec2(ImGui::GetContentRegionAvail().x*0.5f, 490.0f), false,
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
	{
		ImGuiContext& currentContext = *GImGui;

		const float calendarWidth = 400.0f;
		const float calendarHeight = 340.0f;
		ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.25f - 80.0f);
		ImGui::BeginChild("CalendarDisplay", ImVec2(calendarWidth, calendarHeight), true,
			ImGuiWindowFlags_NoResize);
		{

			static time_t now = time(NULL);
			static tm currentDate = *localtime(&now);
			static tm today = *localtime(&now);

			static char currentDateText[10] = { '\0' };// = "2022-12-01";
			static char oldDateText[10] = { '\0' };
			std::string storeDateInput;
			

			//Running the query
			if (dateInit & 1) {
				dateOut = *localtime(&now);
				currentDate = *localtime(&now);
				currentDate.tm_mday = 1;
				RecalculateDate(currentDate);
				holidaysToShow = 3;
				boost::format holQuery;
				switch (holidaysToShow) {
				case 3:
					holQuery = boost::format("SELECT DATE, HOLIDAY_DESC FROM HOLIDAYS WHERE CALENDAR = %1% AND DATE BETWEEN DATE('%2%-01-01') AND DATE('%2%-12-31')") % calendarId % (1900 + today.tm_year);
					break;
				case 1:
					holQuery = boost::format("SELECT DATE, HOLIDAY_DESC FROM HOLIDAYS WHERE CALENDAR = %1% AND DATE BETWEEN DATE('%2%-01-01') AND DATE('%2%-12-31')") % calendarId % (1900 + today.tm_year - 1);
					break;
				case 2:
					holQuery = boost::format("SELECT DATE, HOLIDAY_DESC FROM HOLIDAYS WHERE CALENDAR = %1% AND DATE BETWEEN DATE('%2%-01-01') AND DATE('%2%-12-31')") % calendarId % (1900 + today.tm_year + 1);
					break;
				default:
					holQuery = boost::format("SELECT DATE, HOLIDAY_DESC FROM HOLIDAYS WHERE CALENDAR = %1% AND DATE BETWEEN DATE('%2%-01-01') AND DATE('%3%-12-31')") % calendarId % (1900 + today.tm_year) % 
						(1900 + today.tm_year + 5);
					break;
				}
				/*boost::format holQuery = boost::format("SELECT DATE, HOLIDAY_DESC FROM HOLIDAYS WHERE CALENDAR = %1% AND DATE BETWEEN DATETIME('NOW', '-5 YEAR') AND DATETIME('NOW', '10 YEAR')") % calendarId;*/
				holidays = QuantIO::dbConnection.getTableData2(holQuery.str(), false);
				sprintf(currentDateText, "%04d-%02d-%02d", dateOut.tm_year + 1900, dateOut.tm_mon + 1, dateOut.tm_mday);
				sprintf(oldDateText, currentDateText);
				dateInit++;
			}

			if (holidaysInMonthInit & 1) {
				boost::format currentMon("%02d");
				currentMon % (currentDate.tm_mon + 1);
				boost::format holInMonthQuery = boost::format("SELECT SUBSTR(DATE, 9), HOLIDAY_DESC FROM HOLIDAYS WHERE CALENDAR = %1% AND DATE BETWEEN DATE('%2%-%3%-01') AND DATE('%2%-%3%-31')") % calendarId % (1900 + currentDate.tm_year) % currentMon.str();
				holidaysInMonth = QuantIO::dbConnection.getTableData2(holInMonthQuery.str(), false);
				holidaysInMonthInit++;
			}

			static const char* dayNames[7] = { "S","M","T","W","T","F","S" };
			static const char* monthNames[12] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
			const static int numDaysPerMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

			static char yearString[12] = "";
			sprintf(yearString, "%d", 1900 + currentDate.tm_year);
			ImGui::Spacing();

			ImGui::PushID(4321);
			ImGui::Indent(5.0f);
			if (ImGui::SmallButton(ICON_FA_CHEVRON_CIRCLE_LEFT)) {
				currentDate.tm_mon -= 1;
				if (currentDate.tm_year <= 70 && currentDate.tm_mon <= 0)
					currentDate.tm_mon = 0;
				RecalculateDate(currentDate);
				holidaysInMonthInit++;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(100.0f);
			ImGui::Text("%s", monthNames[currentDate.tm_mon]);
			ImGui::PopItemWidth();

			ImGui::SameLine();

			if (ImGui::SmallButton(ICON_FA_CHEVRON_CIRCLE_RIGHT)) {
				currentDate.tm_mon += 1;
				RecalculateDate(currentDate);
				holidaysInMonthInit++;
			}

			ImGui::SameLine(0.0f, 20.0f);

			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
			if (ImGui::SmallButton("Today")) {
				currentDate = today;
				currentDate.tm_mday = 1;
				RecalculateDate(currentDate);
				holidaysInMonthInit++;
			}
			ImGui::PopStyleColor();
			ImGui::SameLine(0.0f, 20.0f);

			ImGui::PopID();
			ImGui::PushID(1235);

			if (ImGui::SmallButton(ICON_FA_CHEVRON_CIRCLE_LEFT)) {
				currentDate.tm_year -= 1;
				if (currentDate.tm_year < 70)
					currentDate.tm_year = 70;
				RecalculateDate(currentDate);
				holidaysInMonthInit++;
			}

			ImGui::SameLine();
			ImGui::Text("%d", 1900 + currentDate.tm_year);
			ImGui::SameLine();
			if (ImGui::SmallButton(ICON_FA_CHEVRON_CIRCLE_RIGHT)) {
				currentDate.tm_year += 1;
				RecalculateDate(currentDate);
				holidaysInMonthInit++;
			}
			ImGui::PopID();
			ImGui::Unindent(5.0f);
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

			const std::string weekendDay1 = weekend.substr(0, 1);
			const std::string weekendDay2 = weekend.substr(1, 1);

			int weekendDays[2] = { std::stoi(weekendDay1) - 1, std::stoi(weekendDay2) - 1};
			
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.17f, 0.31f, 0.46f, 0.80f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.17f, 0.31f, 0.46f, 0.80f));

			for (int dw = 0; dw < 7; dw++) {
				ImGui::BeginGroup();

				if (dw == weekendDays[1] || dw == weekendDays[0]) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.667f, 0.167f, 0.167f, 1.0f));
				}
				ImGui::Text("  %s", dayNames[dw]);
				ImGui::Separator();
				ImGui::Spacing();

				if (dw == weekendDays[1] || dw == weekendDays[0]) {
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

						//Color specific days
						
						bool todayDay = (currentDate.tm_year == today.tm_year && currentDate.tm_mon == today.tm_mon 
							&& cday + 1 == today.tm_mday);
						bool selectedDay = (cday + 1 == dateOut.tm_mday && currentDate.tm_year == dateOut.tm_year && 
							currentDate.tm_mon == dateOut.tm_mon);

						if (dw == weekendDays[1] || dw == weekendDays[0]) {
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.667f, 0.167f, 0.167f, 1.0f));
						}
						
						if (todayDay) {
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.69f, 0.313f, 1.0f));
						}
						int iholidaysInMonth = holidaysInMonth.size();
						for (int i = 0; i < iholidaysInMonth; i++) {
							std::string holidayDate = holidaysInMonth[i][0];
							holidayDate.erase(0, holidayDate.find_first_not_of('0'));
							if (cday + 1 == std::stoi(holidayDate)) {
								ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.99f, 0.167f, 0.167f, 1.0f));
							}
						};
						if (selectedDay) {
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.69f, 0.313f, 1.0f));
						}

						if (ImGui::SmallButton(curDayStr)) {
							ImGui::SetItemDefaultFocus();
							dateOut.tm_mday = cday + 1;
							dateOut.tm_year = currentDate.tm_year;
							dateOut.tm_mon = currentDate.tm_mon;
							RecalculateDate(dateOut);
							sprintf(currentDateText, "%04d-%02d-%02d", dateOut.tm_year + 1900, dateOut.tm_mon + 1,
								dateOut.tm_mday);
							sprintf(oldDateText, currentDateText);
						}
						if (dw == weekendDays[1] || dw == weekendDays[0]) {
							ImGui::PopStyleColor();
						}
						if (todayDay) {
							ImGui::PopStyleColor();
						}
						if (selectedDay) {
							ImGui::PopStyleColor();
						}
						if (todayDay && ImGui::IsItemHovered()) {
							ImGui::SetTooltip("Current day");
						}
						for (int i = 0; i < iholidaysInMonth; i++) {
							std::string holidayDate = holidaysInMonth[i][0];
							holidayDate.erase(0, holidayDate.find_first_not_of('0'));
							if (cday + 1 == std::stoi(holidayDate)) {
								ImGui::PopStyleColor();
								if (ImGui::IsItemHovered()) {
									ImGui::SetTooltip(holidaysInMonth[i][1].c_str());
								}
							}
						};
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

			/*if (dateInit & 1) {
				currentDate = *localtime(&now);
				RecalculateDate(currentDate);
				dateInit++;
			}*/

			ImGui::Separator();
			
			if (ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)) {
				ImGui::SetKeyboardFocusHere(0);
			}
			if (ImGui::InputText("##customDate", currentDateText, 11,
				ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll |
				ImGuiInputTextFlags_CallbackCharFilter, QuantIO::DateFilter::Filter)) {

				storeDateInput = currentDateText;

				const boost::regex 
					e1("\\b^(1[9][7-9]\\d{1}|2[01]\\d{2})[\\/\\s-](0[1-9]|1[012])[\\/\\s-](0[1-9]|[12][0-9]|3[01])$\\b"); //2022-02-12
				const boost::regex 
					e2("\\b^(1[9][7-9]\\d{1}|2[01]\\d{2})[\\/\\s-]([1-9]|1[012])[\\/\\s-]([1-9]|[12][0-9]|3[01])$\\b"); //2022-2-1
				const boost::regex 
					e3("\\b^(1[9][7-9]\\d{1}|2[01]\\d{2})[\\/\\s-](0[1-9]|1[012])[\\/\\s-]([1-9]|[12][0-9]|3[01])$\\b"); //2022-02-1
				const boost::regex 
					e4("\\b^(1[9][7-9]\\d{1}|2[01]\\d{2})[\\/\\s-]([1-9]|1[012])[\\/\\s-](0[1-9]|[12][0-9]|3[01])$\\b"); //2022-2-01
				const boost::regex 
					e5("\\b^(1[9][7-9]\\d{1}|2[01]\\d{2})(0[1-9]|1[012])(0[1-9]|[12][0-9]|3[01])$\\b"); //20220201

				const boost::regex y1("\\b^(\\d+[Yy])$\\b"); //10Y
				const boost::regex y2("^-\\d+[yY]$"); //-10Y
				const boost::regex m1("\\b^(\\d+[mM])$\\b"); //10m
				const boost::regex m2("^-\\d+[mM]$"); //-10M
				const boost::regex d1("\\b^(\\d+[dD])$\\b"); //10d
				const boost::regex d2("^-\\d+[dD]$"); //-10d

				const bool e1Check = boost::regex_match(storeDateInput, e1, boost::regex_constants::match_all);
				const bool e2Check = boost::regex_match(storeDateInput, e2, boost::regex_constants::match_all);
				const bool e3Check = boost::regex_match(storeDateInput, e3, boost::regex_constants::match_all);
				const bool e4Check = boost::regex_match(storeDateInput, e4, boost::regex_constants::match_all);
				const bool e5Check = boost::regex_match(storeDateInput, e5, boost::regex_constants::match_all);
				const bool y1Check = boost::regex_match(storeDateInput, y1, boost::regex_constants::match_all);
				const bool y2Check = boost::regex_match(storeDateInput, y2, boost::regex_constants::match_all);

				const bool m1Check = boost::regex_match(storeDateInput, m1, boost::regex_constants::match_all);
				const bool m2Check = boost::regex_match(storeDateInput, m2, boost::regex_constants::match_all);
				const bool d1Check = boost::regex_match(storeDateInput, d1, boost::regex_constants::match_all);
				const bool d2Check = boost::regex_match(storeDateInput, d2, boost::regex_constants::match_all);

				if (e1Check || e2Check || e3Check || e4Check || e5Check || y1Check || y2Check || m1Check || m2Check || 
					d1Check || d2Check) {

					std::string year;
					std::string mon;
					std::string day;

					int iyear;
					int imon;
					int iday;

					if (e1Check) {
						year = storeDateInput.substr(0, 4);
						mon = storeDateInput.substr(5, 2);
						day = storeDateInput.substr(8, 2);

						mon.erase(0, mon.find_first_not_of('0'));
						day.erase(0, day.find_first_not_of('0'));

						iyear = std::stoi(year);
						imon = std::stoi(mon);
						iday = std::stoi(day);
					}
					else if (e2Check) {
						year = storeDateInput.substr(0, 4);
						mon = storeDateInput.substr(5, 1);
						day = storeDateInput.substr(7, 1);

						mon.erase(0, mon.find_first_not_of('0'));
						day.erase(0, day.find_first_not_of('0'));

						iyear = std::stoi(year);
						imon = std::stoi(mon);
						iday = std::stoi(day);
					}
					else if (e3Check) {
						year = storeDateInput.substr(0, 4);
						mon = storeDateInput.substr(5, 2);
						day = storeDateInput.substr(8, 1);

						mon.erase(0, mon.find_first_not_of('0'));
						day.erase(0, day.find_first_not_of('0'));

						iyear = std::stoi(year);
						imon = std::stoi(mon);
						iday = std::stoi(day);
					}
					else if (e4Check) {
						year = storeDateInput.substr(0, 4);
						mon = storeDateInput.substr(5, 1);
						day = storeDateInput.substr(7, 2);

						mon.erase(0, mon.find_first_not_of('0'));
						day.erase(0, day.find_first_not_of('0'));

						iyear = std::stoi(year);
						imon = std::stoi(mon);
						iday = std::stoi(day);
					}
					else if (e5Check) {

						year = storeDateInput.substr(0, 4);
						mon = storeDateInput.substr(4, 2);
						day = storeDateInput.substr(6, 2);

						mon.erase(0, mon.find_first_not_of('0'));
						day.erase(0, day.find_first_not_of('0'));

						iyear = std::stoi(year);
						imon = std::stoi(mon);
						iday = std::stoi(day);
					}

					else if (y1Check) {
						std::string sy1 = storeDateInput.substr(0, storeDateInput.length() - 1);
						//sy1.erase(0, mon.find_first_not_of('0'));
						int iy1 = std::stoi(sy1);

						std::string storedDate = oldDateText; //2022-12-12

						year = storedDate.substr(0, 4);
						mon = storedDate.substr(5, 2);
						day = storedDate.substr(8, 2);

						iyear = std::stoi(year) + iy1;
						imon = std::stoi(mon);
						iday = std::stoi(day);
					}

					else if (y2Check) {
						std::string sy1 = storeDateInput.substr(1, storeDateInput.length() - 1);
						//sy1.erase(0, mon.find_first_not_of('0'));
						int iy1 = std::stoi(sy1);

						std::string storedDate = oldDateText; //2022-12-12

						year = storedDate.substr(0, 4);
						mon = storedDate.substr(5, 2);
						day = storedDate.substr(8, 2);

						iyear = std::stoi(year) - iy1 < 1970 ? 1970 : std::stoi(year) - iy1;
						imon = std::stoi(mon);
						iday = std::stoi(day);
					}

					else if (m1Check) {
						std::string sm = storeDateInput.substr(0, storeDateInput.length() - 1);
						int im = std::stoi(sm);

						std::string storedDate = oldDateText; //2022-12-12

						year = storedDate.substr(0, 4);
						mon = storedDate.substr(5, 2);
						day = storedDate.substr(8, 2);

						iyear = std::stoi(year);
						imon = std::stoi(mon) + im;
						iday = std::stoi(day);
					}

					else if (m2Check) {
						std::string sm = storeDateInput.substr(1, storeDateInput.length() - 1);
						int im = std::stoi(sm);

						std::string storedDate = oldDateText; //2022-12-12

						year = storedDate.substr(0, 4);
						mon = storedDate.substr(5, 2);
						day = storedDate.substr(8, 2);

						iyear = std::stoi(year);
						imon = std::stoi(mon) - im;
						iday = std::stoi(day);
					}

					else if (d2Check) {
						std::string sd = storeDateInput.substr(1, storeDateInput.length() - 1);
						int id = std::stoi(sd);

						std::string storedDate = oldDateText; //2022-12-12

						year = storedDate.substr(0, 4);
						mon = storedDate.substr(5, 2);
						day = storedDate.substr(8, 2);

						iyear = std::stoi(year);
						imon = std::stoi(mon);
						iday = std::stoi(day) - id;
					}

					else {
						std::string sd = storeDateInput.substr(0, storeDateInput.length() - 1);
						int id = std::stoi(sd);

						std::string storedDate = oldDateText; //2022-12-12

						year = storedDate.substr(0, 4);
						mon = storedDate.substr(5, 2);
						day = storedDate.substr(8, 2);

						iyear = std::stoi(year);
						imon = std::stoi(mon);
						iday = std::stoi(day) + id;

					}

					dateOut = QuantIO::CreateDate(iday, imon, iyear);

					RecalculateDate(dateOut);

					currentDate.tm_mday = 1;
					currentDate.tm_year = dateOut.tm_year;
					currentDate.tm_mon = dateOut.tm_mon;
					RecalculateDate(currentDate);

					holidaysInMonthInit++; //To show or refresh holidays

					sprintf(currentDateText, "%04d-%02d-%02d", dateOut.tm_year + 1900, dateOut.tm_mon + 1, dateOut.tm_mday);
					sprintf(oldDateText, currentDateText);

				}
				else {
					sprintf(currentDateText, oldDateText);
				}

				ImGui::SetKeyboardFocusHere(-1);

				/*

				int outLen = storeDateInput.length();

				std::string year;
				std::string mon;
				std::string day;

				//Creating the date 2012-12-12
				{
					year = storeDateInput.substr(0, 4);
					mon = storeDateInput.substr(5, 2);
					day = storeDateInput.substr(8, 2);
				}

				mon.erase(0, mon.find_first_not_of('0'));
				day.erase(0, day.find_first_not_of('0'));
				
				currentDate = QuantIO::CreateDate(1, std::stoi(mon), std::stoi(year));
				RecalculateDate(currentDate);

				holidaysInMonthInit++; //To show or refresh holidays

				dateOut = QuantIO::CreateDate(std::stoi(day), std::stoi(mon), std::stoi(year));
				RecalculateDate(dateOut);

				*/

				//currentDate.tm_mday = 1;
				//RecalculateDate(currentDate);

				//dateOut.tm_year = inputDateText.substr(0, 1)
				
				//sprintf(inputDateText, currentDateText);

				//printf("%s\n", year.c_str());
				//printf("%s\n", mon.c_str());
				//printf("%s\n", day.c_str());
			}

		}
		ImGui::EndChild();
	}
	
	ImGui::EndChild();

	ImGui::SameLine();

	ImGui::BeginChild("##CalendarRight", ImVec2(0.0f, 490.0f), true);
	{
		static time_t now = time(NULL);
		static tm today = *localtime(&now);

		//Calendar holidays list
		ImGui::RadioButton("This year", &holidaysToShow, 3); ImGui::SameLine();
		//ImGui::RadioButton("Past year", &holidaysToShow, 1); ImGui::SameLine();
		ImGui::RadioButton("Next year", &holidaysToShow, 2); ImGui::SameLine();
		ImGui::RadioButton("Next 5yrs", &holidaysToShow, 0);

		ImGui::Separator();

		/*int years = show == 3 ? 0 : show == 0 ? 219 : ( show == 1 ? today.tm_year - 80 : 299 - today.tm_year);
		int yearsInit = show == 0 ? 1980 : (show == 1 ? 1980 : today.tm_year + 1900);*/

		static ImGuiTextFilter filter;
		filter.Draw("Filter", ImGui::GetFontSize() * 18);
		ImGui::SameLine();
		HelpMarker("Use , for OR, - for NOT");


		if (holidayListInit & 1) {
			filter.Clear();
			holidayListInit++;
		}

		static int iholidays = holidays.size();
		std::vector<std::vector<std::string>> filteredholidays;
		if (filter.IsActive()) {
			for (int i = 0; i < iholidays; i++) {
				std::string rowData = boost::algorithm::join(holidays[i], " ");
				if (filter.PassFilter(rowData.c_str())) {
					filteredholidays.push_back(holidays[i]);
				}
			}
		}
		else {
			filteredholidays = holidays;
		}
		if (ImGui::BeginTable("CalendarHolidays", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX |
			ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV,
			ImVec2(0.0f, 0.0f), 0.0f)) {
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableSetupColumn("DATE", ImGuiTableColumnFlags_NoHide);
			//ImGui::TableSetupColumn(holidays[0][0].c_str(), ImGuiTableColumnFlags_NoHide, 0.0f);
			ImGui::TableSetupColumn("HOLIDAY", ImGuiTableColumnFlags_NoHide);
			//ImGui::TableSetupColumn(holidays[0][1].c_str(), ImGuiTableColumnFlags_NoHide, 0.0f);
			ImGui::TableHeadersRow();

			static ImGuiListClipper clipper;
			clipper.Begin(filteredholidays.size());

			while (clipper.Step()) {
				for (std::size_t i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
					ImGui::PushID(i + 101);

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text(filteredholidays[i][0].c_str());

					ImGui::TableSetColumnIndex(1);
					ImGui::Text(filteredholidays[i][1].c_str());

					ImGui::PopID();
				}
			}

			clipper.End();
			ImGui::EndTable();
		}			
	}
	ImGui::EndChild();
}

void CalendarAdhocHolidays() {
	ImGui::BeginChild("##AdhocHolidays", ImVec2(0, 0), false, ImGuiWindowFlags_NoBackground);
	{
		if (ImGui::BeginTable("CalendarHolidays", 2, ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX |
			ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV,
			ImVec2(0.0f, 0.0f), 0.0f)) {
			ImGui::TableSetupScrollFreeze(0, 1);
			ImGui::TableSetupColumn("DATE", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("HOLIDAY", ImGuiTableColumnFlags_NoHide);
			ImGui::TableHeadersRow();

			/*static ImGuiListClipper clipper;
			clipper.Begin(filteredholidays.size());

			while (clipper.Step()) {
				for (std::size_t i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
					ImGui::PushID(i + 101);

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text(filteredholidays[i][0].c_str());

					ImGui::TableSetColumnIndex(1);
					ImGui::Text(filteredholidays[i][1].c_str());

					ImGui::PopID();
				}
			}

			clipper.End();*/
			ImGui::EndTable();
		}
	}
	ImGui::EndChild();
}