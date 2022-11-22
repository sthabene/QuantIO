#include "tables.hpp"
#include <algorithm>
#include <set>
#include "boost/algorithm/string/join.hpp"

ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg | 
				ImGuiTableFlags_BordersV |
				ImGuiTableFlags_Resizable | 
				ImGuiTableFlags_ContextMenuInBody | 
				ImGuiTableFlags_BordersOuter |
				ImGuiTableFlags_Reorderable | 
				ImGuiTableFlags_Hideable | 
				ImGuiTableFlags_SizingStretchSame |
				ImGuiTableFlags_Sortable | 
				ImGuiTableFlags_ScrollY | 
				ImGuiTableFlags_NoHostExtendY;


namespace QuantIO {
	void Tables::Table(std::string& query, std::string& title, std::vector<std::vector<std::string>>& data,
		int& refresh) { //, void (f)(std::string& row)

		//if (refresh & 1) {
		//	data = QuantIO::statement.getTableData(query); //Run Query
		//	refresh++;
		//}

		const int iRows = data.size() - 1;
		const int iColumns = data[0].size();


		//ImGui::PushID(title.c_str());
		const float rowHeight = ImGui::GetTextLineHeight() + ImGui::GetStyle().CellPadding.y * 3.0f;
		ImGuiTextFilter filter;
		ImGui::BeginChild(title.c_str(), ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysAutoResize);
		{
			filter.Draw("Filter", ImGui::GetFontSize() * 25);
			ImGui::SameLine(); 
			HelpMarker("Use , for OR, - for NOT, filtering includes hidden columns");
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetWindowSize().x - buttonSz.x);
			if (ImGui::Button("Refresh", buttonSz)) {
				refresh++;
			}

			int startCount = 0;
			int displayRows = iRows;
			std::vector<std::vector<std::string>> filteredData;
			if (filter.IsActive()) {
				for (int i = 1; i <= iRows; i++) {
					//std::string rowData = std::accumulate(data[i].begin(), data[i].end(), std::string{});
					std::string rowData = boost::algorithm::join(data[i], " ");
					if (filter.PassFilter(rowData.c_str())) {
						filteredData.push_back(data[i]);
					}
				}
				displayRows = filteredData.size();
			}
			else {
				filteredData = data;
				startCount = 1;
			}

			float tableHeight = rowHeight * ((float)displayRows + 1);
			float maxTableHeight = (tableHeight > ImGui::GetContentRegionAvail().y ?
					ImGui::GetContentRegionAvail().y : tableHeight);

			//std::set<std::string> selections; //Holds selections
			static ImVector<std::string> selections;

			if (ImGui::BeginTable(title.c_str(), iColumns, tableFlags, ImVec2(0.0f, maxTableHeight), 0.0f)) {
				ImGui::TableSetupScrollFreeze(0, 1); // Make first row (header) always visible

				//Column Setup
				for (int i = 0; i < iColumns; i++) { //Column label
					if (i == 0) {
						ImGui::TableSetupColumn(data[0][i].c_str(), ImGuiTableColumnFlags_NoHide, 0.0f, i);
					}
					else {
						ImGui::TableSetupColumn(data[0][i].c_str(), ImGuiTableColumnFlags_None, 0.0f, i);
					}
				}
				ImGui::TableHeadersRow();
				
				//Sorting
				ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs();
				if (sortSpecs && sortSpecs->SpecsDirty) { 
					if (data.size() > 1) {
						if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Ascending) {
							std::sort(data.begin() + 1, data.end());
						}
						else {
							std::sort(data.rbegin(), data.rend() - 1);
						}
					}
					sortSpecs->SpecsDirty = false;
				}

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
						bool openOpenPopup = false;
						bool openEditPopup = false;
						bool openInsertPopup = false;
						bool openDuplicatePopup = false;
						bool openCopyLinePopup = false;
						bool openDeletePopup = false;
						bool openExitPopup = false;

						if (ImGui::BeginPopupContextItem("CurrenciesPopup")) {
							selections.clear();
							//selections.push_back(currentItem->c_str());
							selections.push_back(currentRow->at(0));
							if (ImGui::MenuItem("Open", "Enter")) {
								openOpenPopup = true;
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
									//selections.insert(v1.at(0));
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


						if (openOpenPopup) {
							//(f)(currentRow->at(0));
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
		//ImGui::PopID();

	}
}