//Disable warnings
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../quant.hpp"
#include "boost/algorithm/string/join.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include <algorithm>

#ifndef NO_IMGUIDATECHOOSER
#include <time.h>   // very common plain c header file used only by DateChooser
#endif //NO_IMGUIDATECHOOSER

#define IMGUI_LEFT_LABEL(func, label, ...) ( ImGui::AlignTextToFramePadding(), ImGui::TextUnformatted(label), ImGui::SameLine(), func("##" label, __VA_ARGS__))

static std::string query = "SELECT CODE, NAME FROM CURRENCY ORDER BY CODE";
static std::string title = "Currency###";
static std::vector<std::vector<std::string>> tableData;
static int refresh = 1;

//Open item
std::vector<std::string> selectedRow;
std::vector<std::string> rounding;
std::vector<std::string> roundingTypes;
static bool showFilter = false;
static std::vector<std::string> selectedRouding;
ImGuiInputTextFlags popupInputFlags = ImGuiInputTextFlags_None;
ImGuiTabItemFlags roundingTabItemFlags = ImGuiTabItemFlags_None;

static ImVec2 buttonSz(25.0f * 5.0f, 32.0f); //To change 

inline std::string getRoundingType(std::string roundingType) {

	std::string m_roudingType =
		(roundingType == "1") ? "Up" :
		(roundingType == "0") ? "None" :
		(roundingType == "2") ? "Down" :
		(roundingType == "3") ? "Closest" :
		(roundingType == "5") ? "Ceiling" : "Floor";

	return m_roudingType;
}

void QuantIOCurrency::DisplayContents() {
	ImGui::BeginChild("Currencies", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
	{
		static const float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 3.0f;

		//Running the query
		if (refresh & 1) {
			tableData = QuantIO::dbConnection.getTableData2(query);
			refresh++;
		}

		//#rows and columns
		static std::size_t iRows = tableData.size() - 1;
		static std::size_t iColumns = tableData[0].size();

		//Filtering text field
		static ImGuiTextFilter filter;
		if (showFilter) {
			filter.Draw("Filter", ImGui::GetFontSize() * 25);
			ImGui::SameLine();
			HelpMarker("Use , for OR, - for NOT, filtering includes hidden columns");
		}

		//Refresh button
		/*ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - buttonSz.x);
		if (ImGui::Button("Refresh", buttonSz)) {
			refresh++;
		}*/

		//Filtered Data
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

		//Table construction
		if (ImGui::BeginTable("Currencies", iColumns, QuantIO::tableFlags, ImVec2(0.0f, maxTableHeight), 0.0f)) {

			//Make first row (header) always visible
			ImGui::TableSetupScrollFreeze(0, 1);

			//Column Setup
			for (std::size_t i = 0; i < iColumns; i++) { //Column label
				if (i == 0) {
					ImGui::TableSetupColumn(tableData[0][i].c_str(), ImGuiTableColumnFlags_NoHide, 0.0f, i);
				}
				else {
					ImGui::TableSetupColumn(tableData[0][i].c_str(), ImGuiTableColumnFlags_None, 0.0f, i);
				}
			}
			ImGui::TableHeadersRow();

			//Sorting, should I use filteredData???????
			/*static ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs();
			if (sortSpecs && sortSpecs->SpecsDirty) {
				if (tableData.size() > 1) {
					if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Ascending) {
						std::sort(tableData.begin() + 1, tableData.end());
					}
					else {
						std::sort(tableData.rbegin(), tableData.rend() - 1);
					}
				}
				sortSpecs->SpecsDirty = false;
			}*/

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

					/*if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
						openExitPopup = true;
					}*/

					//Context menu
					if (ImGui::BeginPopupContextItem("ContextPopup")) {
						selections.clear();
						selections.push_back(currentRow->at(0));

						if (ImGui::MenuItem("Open", "Enter")) {
							//Populate selected item
							popupInputFlags = ImGuiInputTextFlags_ReadOnly;
							roundingTabItemFlags = ImGuiTabItemFlags_None;
							std::string roundQuery = "SELECT LABEL FROM ROUNDING";
							rounding = QuantIO::dbConnection.getTableData2(roundQuery.c_str(), false, true)[0];
							boost::format openQuery = boost::format("SELECT T1.CODE, T1.NAME, T1.NUM_CODE, T1.SYMBOL, T1.FRAC_SYMBOL, T1.FRAC_UNIT, T1.FORMAT, T1.TRIANGU, T2.LABEL, T2.TYPE, T2.PRECISION, T2.DIGIT FROM CURRENCY T1, ROUNDING T2 WHERE T2.ROUNDING_ID = T1.ROUNDING AND T1.CODE = '%1%'") % currentRow->at(0);
							selectedRow = QuantIO::dbConnection.getTableData2(openQuery.str(), false, false)[0];
							//Then open dialogue
							openOpenPopup = true;
						};

						if (ImGui::MenuItem("Edit", "Ctrl + Enter")) {
							popupInputFlags = ImGuiInputTextFlags_None;
							roundingTabItemFlags = ImGuiTabItemFlags_None;
							std::string roundQuery = "SELECT LABEL FROM ROUNDING";
							rounding = QuantIO::dbConnection.getTableData2(roundQuery.c_str(), false, true)[0];
							boost::format openQuery = boost::format("SELECT T1.CODE, T1.NAME, T1.NUM_CODE, T1.SYMBOL, T1.FRAC_SYMBOL, T1.FRAC_UNIT, T1.FORMAT, T1.TRIANGU, T2.LABEL, T2.TYPE, T2.PRECISION, T2.DIGIT FROM CURRENCY T1, ROUNDING T2 WHERE T2.ROUNDING_ID = T1.ROUNDING AND T1.CODE = '%1%'") % currentRow->at(0);
							selectedRow = QuantIO::dbConnection.getTableData2(openQuery.str(), false, false)[0];

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
						if (ImGui::MenuItem("Delete", "Del")) {
							openDeletePopup = true;
						};
						ImGui::Separator();
						if (ImGui::MenuItem("Exit", "Esc")) {
							openExitPopup = true;
							
						};
						ImGui::EndPopup();
					}

					//Close Popup
					if (openExitPopup) {
						title = "Exit?";
						ImGui::OpenPopup("Exit");
						//openOpenPopup = false;
					}

					//Open Popup
					if (openOpenPopup) {
						title = "Currency: Read-only mode";
						ImGui::OpenPopup(title.c_str());
						//openOpenPopup = false;
					}

					//Open Popup
					if (openEditPopup) {
						title = "Currency: Edit mode";
						ImGui::OpenPopup(title.c_str());
						//printf("%d\n", (int)openOpenPopup);
						//openEditPopup = false;
					}

					if (openEditPopup || openOpenPopup) {
						roundingTabItemFlags = ImGuiTabItemFlags_SetSelected ;
					}
					else {
						roundingTabItemFlags = ImGuiTabItemFlags_None;
					}

					ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, 
						ImVec2(0.5f, 0.5f));
					if (ImGui::BeginPopupModal("Exit", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
						ImGui::Text("Close current tab?");
						ImGui::Separator();

						static bool askAgain = false;
						ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
						//ImGui::Checkbox("Don't ask me next time", &askAgain);
						ImGui::PopStyleVar();

						if (ImGui::Button("OK", ImVec2(120, 0))) { 
							ImGui::CloseCurrentPopup(); 
							this->DoClose();
						}
						ImGui::SetItemDefaultFocus();
						ImGui::SameLine();
						if (ImGui::Button("Cancel", ImVec2(120, 0))) { 
							ImGui::CloseCurrentPopup();
						}
						ImGui::EndPopup();
					}

					ImGui::SetNextWindowPos(QuantIO::popupLocation(), ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
					ImGui::SetNextWindowSize(ImVec2(800, 700), ImGuiCond_FirstUseEver);
					if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoSavedSettings)) {
						
						
						ImGui::SetCursorPosX(680.0f);
						ImGui::PushItemWidth(ImGui::CalcTextSize(selectedRow[0].c_str()).x + 15.0f);
						ImGui::InputText(" Code", (char*)selectedRow[0].c_str(), 4,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();
						ImGui::Spacing();


						float titleWidth = ImGui::CalcTextSize(selectedRow[1].c_str()).x + 15.0f;

						ImGui::SetCursorPosX(400.0f - (titleWidth + 50.0f) * 0.5f);
						ImGui::PushItemWidth(titleWidth);
						ImGui::InputText("##Name", (char*)selectedRow[1].c_str(), 32,
							ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);
						ImGui::PopItemWidth();

						ImGui::Dummy(ImVec2(0.0f, 35.0f));

						ImGui::Separator();
						
						
						
						
						/*ImGui::Spacing();
						ImGui::SetCursorPosX(400 - ImGui::CalcTextSize("Details").x * 0.5f);
						ImGui::TextDisabled("Details");
						ImGui::Spacing();*/
						
						ImGui::Indent(180.0f);
						ImGui::PushItemWidth(rowHeight * 10.0f);
						//Open modal details
						/*ImGui::InputText("Code", (char*)selectedRow[0].c_str(), 32,
							ImGuiInputTextFlags_CharsUppercase | ImGuiDir_Left | popupInputFlags);
						
						ImGui::InputText("Name", (char*)selectedRow.at(1).c_str(), 32,
							popupInputFlags);*/

						ImGui::InputText("Numeric code", (char*)selectedRow.at(2).c_str(), 32,
							popupInputFlags | ImGuiInputTextFlags_CharsDecimal);

						ImGui::InputText("Symbol", (char*)selectedRow.at(3).c_str(), 32,
							popupInputFlags);

						ImGui::InputText("Fraction Symbol", (char*)selectedRow.at(4).c_str(), 32,
							popupInputFlags);

						std::string fracSymbol = selectedRow.at(5);
						const char* fracSymbols[6] = { "1", "10", "100", "1000", "10000", "100000" };
						int originalItem = 
							(fracSymbol == "1") ? 0 : 
							(fracSymbol == "10") ? 1 : 
							(fracSymbol == "100") ? 2 : 
							(fracSymbol == "1000") ? 3 : 
							(fracSymbol == "10000") ? 4 : 5;
						

						static int itemCurrent = 0;
						if (openEditPopup || openOpenPopup) {
							itemCurrent = originalItem;
						}

						const char* fracPreview = fracSymbols[itemCurrent];

						if (ImGui::BeginCombo("Fractions Per Unit", fracPreview,
							ImGuiComboFlags_PopupAlignLeft)) {
							for (int n = 0; n < IM_ARRAYSIZE(fracSymbols); n++) {

								const bool isSelected = (itemCurrent == n);
								
								if (ImGui::Selectable(fracSymbols[n], isSelected)) {
									itemCurrent = n;
									//itemCurrent = n;
								}

								if (isSelected) {
									ImGui::SetItemDefaultFocus();
								}
							}
							ImGui::EndCombo();
						}

						ImGui::SameLine();
						HelpMarker("Number of fractionary parts in a unit");

						
						
						ImGui::InputText("Triangualtion", (char*)selectedRow.at(7).c_str(), 32,
							popupInputFlags);

						ImGui::SameLine();
						HelpMarker("Cross currency triangulation");
						ImGui::PopItemWidth();
						ImGui::Unindent(180.0f);

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						if (ImGui::BeginTabBar("CurrencyTabBar", ImGuiTabBarFlags_None)) {
							bool open = true;
							if (ImGui::BeginTabItem("Rouding", NULL, roundingTabItemFlags)) {
								ptrdiff_t pos = std::find(rounding.begin(), rounding.end(), selectedRow.at(8))
									- rounding.begin();

								static int currentRoundingIndex = 0;
								static std::string currentRoundingType = "0";
								static std::string currentRoundingPrecision = "0";
								static std::string currentRoundingDigits = "0";

								if (openEditPopup || openOpenPopup) {
									currentRoundingIndex = pos;
									currentRoundingType = selectedRow.at(9).c_str();
									currentRoundingPrecision = selectedRow.at(10);
									currentRoundingDigits = selectedRow.at(11);
								}

								std::string currentRounding = rounding[currentRoundingIndex];
								ImGui::Spacing();
								ImGui::Indent(15.0f);
								ImGui::PushItemWidth(rowHeight * 15.0f);

								if (ImGui::BeginCombo("##Rounding", currentRounding.c_str())) {
									for (int n = 0; n < rounding.size(); n++) {
										
										const bool isSelected = (currentRoundingIndex == n);

										if (ImGui::Selectable(rounding[n].c_str(), isSelected)) {
											currentRoundingIndex = n;

											boost::format roundIndexQuery = boost::format("SELECT LABEL, TYPE, PRECISION, DIGIT FROM ROUNDING WHERE LABEL = '%1%'") % rounding[currentRoundingIndex];
											selectedRouding = QuantIO::dbConnection.getTableData2(
												roundIndexQuery.str(), false, false)[0];

											//currentRoundingIndex = pos;
											currentRoundingType = selectedRouding.at(1).c_str();
											currentRoundingPrecision = selectedRouding.at(2);
											currentRoundingDigits = selectedRouding.at(3);

											//printf("%s\n", roundIndexQuery.str());
										}
										if (isSelected) {
											ImGui::SetItemDefaultFocus();
										}
									}
									ImGui::EndCombo();
								}
								ImGui::PopItemWidth();

								if (popupInputFlags == ImGuiInputTextFlags_None) {
									ImGui::SameLine(0.0f, 10.0f);
									if (ImGui::Button("Edit rounding...")) {
										ImGui::OpenPopup("Edit Rounding");
									}

									ImGui::SameLine(0.0f, 10.0f);
									if (ImGui::Button("New rounding...")) {
										ImGui::OpenPopup("New Rounding");
									}
								}

								ImGui::Text("Type: %s", getRoundingType(currentRoundingType).c_str());
								ImGui::Text("Precision: %s", currentRoundingPrecision.c_str());
								ImGui::Text("Digit: %s", currentRoundingDigits.c_str());

								ImGui::Unindent(15.0f);

								ImGui::SetNextWindowPos(QuantIO::popupLocation(ImGui::GetWindowPos(), 2.0f), 
									ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
								ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
								if (ImGui::BeginPopupModal("Edit Rounding", NULL, 
									ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {

									ImGui::Spacing();
									ImGui::SetCursorPosX(300 - ImGui::CalcTextSize("Rouding").x * 0.5f);
									ImGui::TextDisabled("Rouding");
									ImGui::Spacing();

									ImGui::Indent(60.0f);
									ImGui::PushItemWidth(rowHeight * 15.0f);

									//ImGui::PushStyleColor(ImGuiCol_Text, 
										//ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);

									ImGui::InputText("Label", (char*)currentRounding.c_str(), 32,
										ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly);


									ImGui::InputText("Type", 
										(char*)getRoundingType(currentRoundingType).c_str(), 
										32, ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_AutoSelectAll);

									//ImGui::PopStyleColor();
									static ImVec4 frameBg_precision = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
									ImGui::PushStyleColor(ImGuiCol_FrameBg, frameBg_precision);
									ImGui::InputText("Precision",
										(char*)currentRoundingPrecision.c_str(),
										32, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackCharFilter,
										QuantIO::NumericFilter::Filter);
									ImGui::PopStyleColor();

									static ImVec4 frameBg_digit = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
									ImGui::PushStyleColor(ImGuiCol_FrameBg, frameBg_digit);
									ImGui::InputText("Digit",
										(char*)currentRoundingDigits.c_str(),
										32, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackCharFilter,
										QuantIO::NumericFilter::Filter);
									ImGui::PopStyleColor();

									ImGui::Unindent(60.0f);

									ImGui::SetCursorPosY(400 - 1.5f * buttonSz.y);
									ImGui::Separator();

									if (ImGui::Button("Close")) {

										frameBg_precision = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
										frameBg_digit = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];

										boost::format roundIndexQuery = boost::format("SELECT LABEL, TYPE, PRECISION, DIGIT FROM ROUNDING WHERE LABEL = '%1%'") % currentRounding.c_str();
										selectedRouding = QuantIO::dbConnection.getTableData2(
											roundIndexQuery.str(), false, false)[0];

										currentRoundingPrecision = selectedRouding.at(2);
										currentRoundingDigits = selectedRouding.at(3);

										ImGui::CloseCurrentPopup();
									}
									ImGui::SameLine(ImGui::CalcTextSize("Close").x + 30.0f);
									if (ImGui::Button("Save")) {

										bool digitEr = currentRoundingDigits.c_str()[0] == '\0';
										bool precisionEr = currentRoundingPrecision.c_str()[0] == '\0';

										if (digitEr || precisionEr) {

											if (digitEr) {
												frameBg_digit = ImVec4(0.93f, 0.10f, 0.01f, 0.3f);
											}

											if (precisionEr) {
												frameBg_precision = ImVec4(0.93f, 0.10f, 0.01f, 0.3f);
											}
										}
										else {
											ImGui::CloseCurrentPopup();

											boost::format roundUpdateQuery = boost::format("UPDATE ROUNDING SET PRECISION = '%1%', DIGIT = '%2%' WHERE LABEL = '%3%'") % 
												currentRoundingPrecision.c_str() %
												currentRoundingDigits.c_str() %
												currentRounding.c_str();
											QuantIO::dbConnection.updateData(roundUpdateQuery.str());

											

											//printf("(%s)\n", roundUpdateQuery.str().c_str());
											frameBg_precision = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
											frameBg_digit = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
										}
									}
									ImGui::EndPopup();
								}


								ImGui::SetNextWindowPos(QuantIO::popupLocation(ImGui::GetWindowPos(), 2.0f),
									ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));
								ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
								if (ImGui::BeginPopupModal("New Rounding", NULL,
									ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {


									ImGui::Spacing();
									ImGui::SetCursorPosX(300 - ImGui::CalcTextSize("Rouding").x * 0.5f);
									ImGui::TextDisabled("Rouding");
									ImGui::Spacing();

									ImGui::Indent(60.0f);
									ImGui::PushItemWidth(rowHeight * 15.0f);

									static char label[32];
									static int type = 0;
									static char precision[32] = "0";
									static char digit[32] = "0";

									ImGui::InputText("Label", label, 32);

									
									ImGui::Combo("Type", &type,
										"None\0Up\0Down\0Closest\0Ceiling\0Floor\0\0");

									static ImVec4 frameBg_precision = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
									ImGui::PushStyleColor(ImGuiCol_FrameBg, frameBg_precision);
									ImGui::InputText("Precision", precision, 32, 
										ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackCharFilter,
										QuantIO::NumericFilter::Filter);
									ImGui::PopStyleColor();

									static ImVec4 frameBg_digit = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
									ImGui::PushStyleColor(ImGuiCol_FrameBg, frameBg_digit);
									ImGui::InputText("Digit", digit, 32, ImGuiInputTextFlags_AutoSelectAll |
										ImGuiInputTextFlags_CallbackCharFilter,
										QuantIO::NumericFilter::Filter);
									ImGui::PopStyleColor();


									ImGui::Unindent(60.0f);

									ImGui::SetCursorPosY(400 - 1.5f * buttonSz.y);
									ImGui::Separator();

									if (ImGui::Button("Close")) {
										sprintf(label, "");
										type = 0;
										sprintf(precision, "0");
										sprintf(digit, "0");
										ImGui::CloseCurrentPopup();
									}

									ImGui::SameLine(ImGui::CalcTextSize("Close").x + 30.0f);
									if (ImGui::Button("Save")) {

										if (label[0] != '\0' && precision[0] != '\0' && digit[0] != '\0') {
											boost::format roundInsertQuery = boost::format("INSERT INTO ROUNDING (LABEL, TYPE, PRECISION, DIGIT) VALUES ('%1%', %2%, %3%, %4%)") %
												label %
												type %
												precision %
												digit;
										QuantIO::dbConnection.updateData(roundInsertQuery.str());
										rounding = QuantIO::dbConnection.getTableData2(
											"SELECT LABEL FROM ROUNDING", false, true)[0];
										}

										sprintf(label, "");
										type = 0;
										sprintf(precision, "0");
										sprintf(digit, "0");										

										ImGui::CloseCurrentPopup();
									}

									ImGui::EndPopup();
								}

								ImGui::EndTabItem();
							}

							if (ImGui::BeginTabItem("Format")) {
								ImGui::Spacing();
								ImGui::Indent(15.0f);
								ImGui::PushItemWidth(rowHeight * 15.0f);
								ImGui::InputText("Format", (char*)selectedRow.at(6).c_str(), 32,
									popupInputFlags | ImGuiInputTextFlags_ReadOnly);
								ImGui::PopItemWidth();
								ImGui::SameLine();
								HelpMarker("(1) value (2) code (3) symbol");

								//Use try catch
								boost::format fmt;
								//try {
									if (boost::contains(selectedRow.at(6), "%3%")) {
										fmt = boost::format(selectedRow.at(6)) % 12345.67891 %
											selectedRow[0] % selectedRow[3];
									}
									else {
										fmt = boost::format(selectedRow.at(6)) % 12345.67891 % selectedRow[0];
									};
								//}
								/*catch (int x) {
									fmt = boost::format("%1%") % "Error";
								}*/

								ImGui::Indent(15.0f);
								
								ImGui::TextDisabled(fmt.str().c_str());
								ImGui::Unindent(30.0f);

								ImGui::EndTabItem();
							}
							if (ImGui::BeginTabItem("Minor Unit Codes")) {
								ImGui::Spacing();
								ImGui::Indent(15.0f);
								ImGui::TextDisabled("Minor unit codes, e.g. GBp, GBX for GBP");
								ImGui::TextDisabled("Currently not supported");
								ImGui::Unindent(15.0f);
								ImGui::EndTabItem();
							}
							ImGui::EndTabBar();
						}

						//Close button
						ImGui::SetCursorPosY(700 - 1.5f * buttonSz.y);

						ImGui::Separator();
						//if (ImGui::Button("Add another modal.."))
						//	ImGui::OpenPopup("Stacked 2");
						//ImGui::SetNextWindowPos(QuantIO::popupLocation(ImGui::GetWindowPos(), 1.0f),
						//	ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));

						
						//ImGui::SameLine();
						if (ImGui::Button("Close")) {//ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)
							ImGui::CloseCurrentPopup();
						}

						if (popupInputFlags == ImGuiInputTextFlags_None) {
							ImGui::SameLine(ImGui::CalcTextSize("Close").x + 30.0f);
							if (ImGui::Button("Save")) {//ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)
								ImGui::CloseCurrentPopup();
							}
						}

						//ImGui::SetItemDefaultFocus();
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