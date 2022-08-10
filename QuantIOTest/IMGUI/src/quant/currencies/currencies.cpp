#include "currencies.hpp"
#include "boost/algorithm/string/join.hpp"
#include <algorithm>

static std::string query = "SELECT CODE, NAME, NUM_CODE, SYMBOL FROM CURRENCY ORDER BY CODE";
static std::string title = "Currency###";
static std::vector<std::vector<std::string>> tableData;
static int refresh = 1;
static ImVec2 buttonSz(25.0f * 5.0f, 32.0f); //To change 

void QuantIOCurrency::DisplayContents() {

	ImGui::BeginChild(title.c_str(), ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
	{
		//Row Height
		static const float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 3.0f;

		//Running the query
		if (refresh & 1) {
			tableData = QuantIO::statement.getTableData(query); //Run Query
			refresh++;
		}

		//#rows and columns
		static int iRows = tableData.size() - 1;
		static int iColumns = tableData[0].size();

		//Filtering text field
		static ImGuiTextFilter filter;
		filter.Draw("Filter", ImGui::GetFontSize() * 25);
		ImGui::SameLine();
		HelpMarker("Use , for OR, - for NOT, filtering includes hidden columns");

		//Refresh button
		ImGui::SameLine();
		ImGui::SetCursorPosX(ImGui::GetWindowSize().x - buttonSz.x);
		if (ImGui::Button("Refresh", buttonSz)) {
			refresh++;
		}

		//Filtered Data
		int startCount = 0;
		int displayRows = iRows;
		std::vector<std::vector<std::string>> filteredData;
		if (filter.IsActive()) {
			for (int i = 1; i <= iRows; i++) {
				//std::string rowData = std::accumulate(data[i].begin(), data[i].end(), std::string{});
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
		if (ImGui::BeginTable(title.c_str(), iColumns, QuantIO::tableFlags, ImVec2(0.0f, maxTableHeight), 0.0f)) {

			//Make first row (header) always visible
			ImGui::TableSetupScrollFreeze(0, 1);

			//Column Setup
			for (int i = 0; i < iColumns; i++) { //Column label
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
				for (int row = clipper.DisplayStart + startCount; row < clipper.DisplayEnd + startCount; row++) {

					std::vector<std::string>* currentRow = &filteredData[row];
					ImGui::PushID(row);
					ImGui::TableNextRow(ImGuiTableRowFlags_None, rowHeight);

					for (int column = 0; column < (int)currentRow->size(); column++) {
						ImGui::TableSetColumnIndex(column);
						std::string* currentItem = &currentRow->at(column);
						const bool itemIsSelected = selections.contains(currentRow->at(0));
						if (ImGui::Selectable(currentItem->c_str(), itemIsSelected,
							ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick,
							ImVec2(0, 0))) {
							if (ImGui::GetIO().KeyCtrl) {
								if (itemIsSelected) {
									selections.find_erase_unsorted(currentRow->at(0));
									//selections.find_erase_unsorted(currentItem->c_str());
								}
								else {
									selections.push_back(currentRow->at(0));
									//selections.push_back(currentItem->c_str());
								}
							}
							else {
								selections.clear();
								selections.push_back(currentRow->at(0));
								//selections.push_back(currentItem->c_str());
							}
						}

					}

					//Selected row details
					std::vector<std::string>* selectedRow = currentRow;

					//Booleans for popups
					bool openOpenPopup = false;
					bool openEditPopup = false;
					bool openInsertPopup = false;
					bool openDuplicatePopup = false;
					bool openCopyLinePopup = false;
					bool openDeletePopup = false;
					bool openExitPopup = false;

					if (ImGui::BeginPopupContextItem("ContextPopup")) {
						selections.clear();
						//selections.push_back(currentItem->c_str());
						selections.push_back(currentRow->at(0));
						if (ImGui::MenuItem("Open", "Enter")) {
							openOpenPopup = true;
							printf("%s\n", currentRow->at(0).c_str());
							//selectedRow = currentRow;
						};

						//ImGui::Separator();
						if (ImGui::MenuItem("Edit", "Ctrl + Enter")) {
							//openCurrenciesPopup = true;
						};
						ImGui::Separator();
						ImGui::MenuItem("Insert", "Insert");
						ImGui::MenuItem("Duplicate", "Ctrl + D");
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
							ImGui::MenuItem("Logical Delete", "Del");
							ImGui::MenuItem("Physical Delete", "Shift + Del");
							ImGui::EndMenu();
						}
						ImGui::Separator();
						if (ImGui::MenuItem("Exit", "Esc")) {

						};
						ImGui::EndPopup();
					}


					//Open Popup
					if (openOpenPopup) {
						ImGui::OpenPopup(title.c_str());
						//openOpenPopup = false;
						printf("%d\n", (int)openOpenPopup);
					}
					static bool unusedOpen = true;
					if (ImGui::BeginPopupModal(title.c_str(), &unusedOpen, ImGuiWindowFlags_NoSavedSettings)) {
						ImGui::Text("Details");

						//char* field1 = (char*)selections[0].c_str();

						ImGui::InputText("Code", (char*)selectedRow->at(0).c_str(), 64,
							ImGuiInputTextFlags_CharsUppercase | ImGuiInputTextFlags_ReadOnly);

						ImGui::InputText("Name", (char*)selectedRow->at(1).c_str(), 64, 
							ImGuiInputTextFlags_ReadOnly);

						ImGui::InputText("Numeric code", (char*)selectedRow->at(2).c_str(), 64,
							ImGuiInputTextFlags_ReadOnly);

						ImGui::InputText("Symbol", (char*)selectedRow->at(3).c_str(), 64,
							ImGuiInputTextFlags_ReadOnly);

						// Testing behavior of widgets stacking their own regular popups over the modal.
						static int item = 1;
						static float color[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
						ImGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
						//ImGui::ColorEdit4("color", color);

						if (ImGui::Button("Add another modal.."))
							ImGui::OpenPopup("Stacked 2");
						ImGui::SetNextWindowPos(QuantIO::popupLocation(ImGui::GetWindowPos(), 1.0f),
							ImGuiCond_Appearing, ImVec2(0.0f, 0.0f));

						if (ImGui::BeginPopupModal("Stacked 2", &unusedOpen))
						{
							ImGui::Text("Hello from Stacked The Second!");
							if (ImGui::Button("Close")) {
								ImGui::CloseCurrentPopup();
								//openCurrenciesPopup = false;

							}
							ImGui::EndPopup();
						}

						if (ImGui::Button("Close")) {//ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)
							ImGui::CloseCurrentPopup();
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