const char* availableFields[] = { "Day 1", "Day 2", "Month 1", "Month 2", "Year 1", 
    "Year 2", "Date 1", "Date 2"};
const char* availableFunctions[] = { "$if(bool, value, value)", "$isLastOfFebruary(day, month, year)", 
"$daysBetween(date, date)", "$equal(value, value)"};

//Selection details
const char* dayCountObjects[] = { "###Empty", "Day 1", "Day 2", "Month 1", "Month 2", "Year 1",
    "Year 2", "Date 1", "Date 2", "Result"};

static std::vector<std::string> values = { "30", "30", 
    "360 * (<Year 2> - <Year 1>) + 30 * (<Month 2> - <Month 1>) + (<Day 2> - <Day 1>)"};
static std::vector<std::string> formulae = { "if (<Day 1> == 31)", 
    "if (<Day 2> == 31 && <Day 1> == 30)", 
    "" };

static std::vector<int> selectedIndex;
static std::vector<int> selectedItemValues = { 1, 2, 9 };

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

            //Column 1
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

            //Column 2
            if (ImGui::TableSetColumnIndex(1)) {
                //static char value[8];
                ImGui::PushItemWidth(-FLT_MIN-31.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);

                ImGui::InputText("##value", (char*)values[row_n].c_str(), 128,
                    ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly);

                //ImGui::InputText("##value", value, 8, ImGuiInputTextFlags_AutoSelectAll);

                ImGui::PopItemWidth();
                ImGui::SameLine(0.0f, 1.0f);
                ImGui::PushID(1235);
                if (ImGui::Button("...", ImVec2(30.0f, buttonSz.y))) {
                    ImGui::OpenPopup("EditModal");
                }


                ImGui::SetNextWindowPos(QuantIO::popupLocation(ImGui::GetWindowPos(), 
                    (float)row_n),
                    ImGuiCond_Appearing,
                    ImVec2(0.0f, 0.0f));
                ImGui::SetNextWindowSize(ImVec2(1000, 500), ImGuiCond_FirstUseEver);

                if (ImGui::BeginPopupModal("EditModal", NULL,
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                    ImGuiWindowFlags_NoTitleBar)) {

                    ImGui::BeginChild("##EditLeft",
                        ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, -1.5f * buttonSz.y), false,
                        ImGuiWindowFlags_HorizontalScrollbar);
                    {
                        //static char InputBuf[1024 * 16] = { '\0' };
                        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                        if (ImGui::InputTextMultiline("##source", (char*)values[row_n].c_str(),
                            128, ImVec2(-FLT_MIN, -FLT_MIN))) {

                        }
                        ImGui::PopTextWrapPos();
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

            //Column 3
            if (ImGui::TableSetColumnIndex(2)) {
                //static char formula[255];
                ImGui::PushItemWidth(-FLT_MIN-93.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
                ImGui::InputText("##Formula", (char*)formulae[row_n].c_str(), 128,
                    ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_ReadOnly);
                ImGui::PopStyleVar();
                ImGui::PopItemWidth();
                ImGui::SameLine(0.0f, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
                ImGui::PushID(1234);
                if (ImGui::Button("...", ImVec2(30.0f, buttonSz.y))) {
                    ImGui::OpenPopup("EditModal");
                }
                ImGui::SameLine(0.0f, 1.0f);
                if (ImGui::Button("-", ImVec2(30.0f, buttonSz.y))) {
                    selectedItemValues.erase(selectedItemValues.begin() + row_n);
                    formulae.erase(formulae.begin() + row_n);
                    values.erase(values.begin() + row_n);
                }
                ImGui::SameLine(0.0f, 1.0f);
                if (ImGui::Button("+", ImVec2(30.0f, buttonSz.y))) {
                    selectedItemValues.insert(selectedItemValues.begin() + row_n + 1, 0);
                    formulae.insert(formulae.begin() + row_n + 1, "");
                    values.insert(values.begin() + row_n + 1, "");
                }
                ImGui::SameLine(0.0f, 1.0f);
                

                ImGui::SetNextWindowPos(QuantIO::popupLocation(ImGui::GetWindowPos(), 
                    (float)row_n),
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
                        //static char InputBuf[1024 * 16] = { '\0' };
                        if (ImGui::InputTextMultiline("##source", (char*)formulae[row_n].c_str(), 
                            128, ImVec2(-FLT_MIN, -FLT_MIN))) {

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