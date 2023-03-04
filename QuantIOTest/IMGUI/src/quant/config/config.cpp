#include "../quant.hpp"
#include <ql/time/date.hpp>
#include <limits>

#ifdef _MSC_VER
#define IM_PRId64   "I64d"
#define IM_PRIu64   "I64u"
#else
#define IM_PRId64   "lld"
#define IM_PRIu64   "llu"
#endif

void QuantIOConfig::DisplayContents() {
	ImGui::BeginChild("Configuration", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
	{
        const char* configItems[] = { "Data Types", "Pricing Settings", "Business Day Conventions", 
            "Date Generation Rules", "Frequency"};
        static int selected = 0;
        { //Left


            ImGui::BeginChild("Left pane", ImVec2(300, 0), true);

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

        { //Right
            ImGui::BeginGroup();
            ImGui::BeginChild("Right pane", ImVec2(0, 0), true); 
            
            

            if (selected == 0) {
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::SetCursorPosX(20.0f);
                ImGui::TextDisabled(configItems[selected]);
                ImGui::Spacing();
                ImGui::Separator();

                ImGui::Indent(20.0f);


                ImGui::Spacing();
                ImGui::Spacing();
                //Integer
                static QuantLib::Integer IntegerV = 0;
                const QuantLib::Integer IntegerMin = INT_MIN / 2;
                const QuantLib::Integer IntegerMax = INT_MAX / 2;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Integer Number (INT)");
                ImGui::SliderScalar("Integer", ImGuiDataType_S32, &IntegerV, &IntegerMin, &IntegerMax, "%d");


                ImGui::Spacing();
                ImGui::Spacing();
                //BigInteger
                static QuantLib::BigInteger BigIntegerV = 0;
                const QuantLib::BigInteger BigIntegerMin = LONG_MIN / 2;
                const QuantLib::BigInteger BigIntegerMax = LONG_MAX / 2;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Large Integer Number (LONG)");
                ImGui::SliderScalar("BigInteger", ImGuiDataType_S32, &BigIntegerV, &BigIntegerMin, &BigIntegerMax, "%d");


                ImGui::Spacing();
                ImGui::Spacing();
                //Natural
                static QuantLib::Natural NaturalV = UINT_MAX / 4;
                const QuantLib::Natural NaturalMin = 0;
                const QuantLib::Natural NaturalMax = UINT_MAX / 2;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Positive Integer (UNSIGNED INT)");
                ImGui::SliderScalar("Natural", ImGuiDataType_U32, &NaturalV, &NaturalMin, &NaturalMax, "%u");


                ImGui::Spacing();
                ImGui::Spacing();
                //BigNatural
                static QuantLib::BigNatural BigNaturalV = ULONG_MAX / 4;
                const QuantLib::BigNatural BigNaturalMin = 0;
                const QuantLib::BigNatural BigNaturalMax = ULONG_MAX / 2;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Large Positive Integer (UNSIGNED LONG)");
                ImGui::SliderScalar("BigNatural", ImGuiDataType_U32, &BigNaturalV, &BigNaturalMin, &BigNaturalMax, "%u");


                ImGui::Spacing();
                ImGui::Spacing();
                //Real
                static QuantLib::Real RealV = 0;
                const QuantLib::Real RealMin = -1000000000000000.0;
                const QuantLib::Real RealMax = +1000000000000000.0;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Real Number (DOUBLE)");
                ImGui::SliderScalar("Real", ImGuiDataType_Double, &RealV, &RealMin, &RealMax, "%.5f");
                ImGui::SliderScalar("Real scientific notation", ImGuiDataType_Double, &RealV, &RealMin, 
                    &RealMax, "%e");


                ImGui::Spacing();
                ImGui::Spacing();
                //Decimal
                static QuantLib::Decimal DecimalV = 0;
                const QuantLib::Decimal DecimalMin = -2.5;
                const QuantLib::Decimal DecimalMax = 2.5;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Decimal Number (DOUBLE)");
                ImGui::SliderScalar("Decimal", ImGuiDataType_Double, &DecimalV, &DecimalMin, &DecimalMax, "%.20f");


                ImGui::Spacing();
                ImGui::Spacing();
                //Size
                static QuantLib::Size SizeV = UINT_MAX / 4;
                const QuantLib::Size SizeMin = 0;
                const QuantLib::Size SizeMax = UINT_MAX / 2;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Size Of A Container (UNSIGNED INT)");
                ImGui::SliderScalar("Size", ImGuiDataType_U32, &SizeV, &SizeMin, &SizeMax, "%d");

                ImGui::Spacing();
                ImGui::Spacing();
                //Time
                static QuantLib::Time TimeV = 12500;
                const QuantLib::Time TimeMin = -50000.0;
                const QuantLib::Time TimeMax = 75000.0;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Continuous Quantity With 1-Year Units (DOUBLE)");
                ImGui::SliderScalar("Time", ImGuiDataType_Double, &TimeV, &TimeMin, &TimeMax, "%.2f");


                ImGui::Spacing();
                ImGui::Spacing();

                //DiscountFactor
                static QuantLib::DiscountFactor DiscountFactorV = 0.5;
                const QuantLib::DiscountFactor DiscountFactorMin = 0;
                const QuantLib::DiscountFactor DiscountFactorMax = 1.0;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Discount Factor Between Dates (DOUBLE)");
                ImGui::SliderScalar("DiscountFactor", ImGuiDataType_Double, &DiscountFactorV, &DiscountFactorMin, &DiscountFactorMax, "%.20f");


                ImGui::Spacing();
                ImGui::Spacing();

                //Rate
                static QuantLib::Rate RateV = 0.5;
                const QuantLib::Rate RateMin = 0.0;
                const QuantLib::Rate RateMax = 1.0;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Interest Rates (DOUBLE)");
                ImGui::SliderScalar("Rate", ImGuiDataType_Double, &RateV, &RateMin, &RateMax, "%.10f");


                ImGui::Spacing();
                ImGui::Spacing();
                //Spread
                static QuantLib::Spread SpreadV = 0.5;
                const QuantLib::Spread SpreadMin = 0.0;
                const QuantLib::Spread SpreadMax = 1.0;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Spreads On Interest Rates (DOUBLE)");
                ImGui::SliderScalar("Spread", ImGuiDataType_Double, &SpreadV, &SpreadMin, &SpreadMax, "%.10f");



                ImGui::Spacing();
                ImGui::Spacing();
                //Volatility
                static QuantLib::Volatility VolatilityV = 0.5;
                const QuantLib::Volatility VolatilityMin = 0;
                const QuantLib::Volatility VolatilityMax = 1.0;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Volatility (DOUBLE)");
                ImGui::SliderScalar("Volatility", ImGuiDataType_Double, &VolatilityV, &VolatilityMin, 
                    &VolatilityMax, "%.10f");



                ImGui::Spacing();
                ImGui::Spacing();
                //Probability
                static QuantLib::Probability ProbabilityV = 0.5;
                const QuantLib::Probability ProbabilityMin = 0.0;
                const QuantLib::Probability ProbabilityMax = 1.0;
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Probability (UNSIGNED LONG)");
                ImGui::SliderScalar("Probability", ImGuiDataType_Double, &ProbabilityV, &ProbabilityMin, 
                    &ProbabilityMax, "%.10f");


                ImGui::Spacing();
                ImGui::Spacing();
                //Date
                static QuantLib::Date DateV(1, QuantLib::January, 2084);
                const QuantLib::Date DateMin(1, QuantLib::January, 1970);
                const QuantLib::Date DateMax = QuantLib::Date::maxDate();
                ImGui::Text(ICON_FA_ARROW_CIRCLE_RIGHT " Date");
                ImGui::SameLine();
                ImGui::Text("%02d/%02d/%04d", DateV.dayOfMonth(), DateV.month(), DateV.year());
                ImGui::SliderScalar("Date", ImGuiDataType_U32, &DateV, &DateMin, &DateMax);

                

                ImGui::Unindent(20.0f);
            }
            if (selected == 1) {
                ImGui::BeginChild("Pricing Settings", ImVec2(800, 490), true, ImGuiWindowFlags_AlwaysAutoResize);
                {
                    ImGui::Spacing();
                    ImGui::SetCursorPosX(20.0f);
                    ImGui::TextDisabled("Global Pricing Run-Time Settings");
                    ImGui::Spacing();
                    ImGui::Separator();

                    ImGui::Spacing();

                    static tm evaluationDate = {};

                    ImGui::SetCursorPosX(190.0f);
                    ImGui::AlignTextToFramePadding();
                    ImGui::Text("Evaluation Date: ");
                    ImGui::SameLine(0.0f, 10.0f);
                    ImGui::PushItemWidth(35.0f * 5.0f);
                    if (ImGui::DateChooser2("##EvaluationDate", evaluationDate, "%Y-%m-%d", true, NULL,
                        ICON_FA_CHEVRON_CIRCLE_LEFT, ICON_FA_CHEVRON_CIRCLE_RIGHT)) {
                    }
                    ImGui::SameLine();
                    HelpMarker("The date at which pricing is to be performed");

                    ImGui::Spacing();
                    ImGui::Spacing();


                    ImGui::Dummy(ImVec2(0.0f, 20.0f));

                    ImGui::Indent(30.0f);

                    if (ImGui::Button("Run", ImVec2(80, 0))) { }
                    ImGui::SameLine();
                    ImGui::TextUnformatted("Anchor Evaluation Date");
                    ImGui::SameLine();
                    HelpMarker("To prevent the evaluation date to change at midnight");

                    if (ImGui::Button("Run", ImVec2(80, 0))) { }
                    ImGui::SameLine();
                    ImGui::TextUnformatted("Reset Evaluation Date");
                    ImGui::SameLine();
                    HelpMarker("To reset the evaluation date to todays date");

                    static bool includeReferenceDateEvents = false;
                    static bool enforceTodaysHistoricFixings = false;
                    static bool includeTodaysCashFlows = false;
                  
                    ImGui::Indent(25.0f);
                    ImGui::Checkbox("##includeReferenceDateEvents", &includeReferenceDateEvents);
                    ImGui::SameLine(0.0f, 40.0f);
                    ImGui::TextUnformatted("Include Reference Date Events");
                    ImGui::SameLine();
                    HelpMarker("Whether or not Events occurring on the reference date should, by default, be taken into account as not happened yet");

                    ImGui::Checkbox("##includeTodaysCashFlows", &includeTodaysCashFlows);
                    ImGui::SameLine(0.0f, 40.0f);
                    ImGui::TextUnformatted("Include Todays Cashflows");
                    ImGui::SameLine();
                    HelpMarker("Whether or not CashFlows occurring on today's date should enter the NPV");

                    ImGui::Checkbox("##enforceTodaysHistoricFixings", &enforceTodaysHistoricFixings);
                    ImGui::SameLine(0.0f, 40.0f);
                    ImGui::TextUnformatted("Enforce Todays Historic Fixings");
                    ImGui::Unindent(25.0f);

                    ImGui::Unindent(30.0f);

                    ImGui::Dummy(ImVec2(0.0f, 80.0f));

                    ImGui::Separator();

                    if (ImGui::Button("Revert")) {}
                    ImGui::SameLine();
                    if (ImGui::Button("Save")) {}


                }
                ImGui::EndChild();
            }
            if (selected == 2) {
                ImGui::BeginChild("BusinessDayConvention", ImVec2(1500, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
                {
                    ImGui::Spacing();
                    ImGui::TextWrapped("These conventions specify the algorithm used to adjust a date in case it is not a valid business day");

                    ImGui::Dummy(ImVec2(0.f, 5.0f));
                    ImGui::Separator();

                    if (ImGui::BeginTable("BusinessDayConventions", 2, QuantIO::tableFlags,
                        ImVec2(0.0f, 415.0f), 0.0f)) {
                        ImGui::TableSetupScrollFreeze(0, 1);

                        ImGui::TableSetupColumn("LABEL", ImGuiTableColumnFlags_NoHide);
                        ImGui::TableSetupColumn("DESCRIPTION", ImGuiTableColumnFlags_NoHide);
                        ImGui::TableHeadersRow();

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Following", false, ImGuiSelectableFlags_SpanAllColumns,
                            ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Choose the first business day after the given holiday.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Modified Following", false, ImGuiSelectableFlags_SpanAllColumns, 
                            ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Choose the first business day after the given holiday unless it belongs"
                            "to a different month, in which case choose the first business day before the holiday.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Preceding", false, ImGuiSelectableFlags_SpanAllColumns,
                            ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Choose the first business day before the given holiday.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Modified Preceding", false, ImGuiSelectableFlags_SpanAllColumns,
                            ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Choose the first business day before the given holiday unless it belongs"
                            "to a different month, in which case choose the first business day after the holiday.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Unadjusted", false, ImGuiSelectableFlags_SpanAllColumns,
                            ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Do not adjust.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Half Month Modified Following", false, ImGuiSelectableFlags_SpanAllColumns,
                            ImVec2(0.f, 25.0f * 2.5f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Choose the first business day after the given holiday unless that day"
                            "crosses the mid - month(15th) or the end of month, in which case choose"
                            "the first business day before the holiday.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Nearest", false, ImGuiSelectableFlags_SpanAllColumns,
                            ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Choose the nearest business day to the given holiday.If both the"
                            "preceding and following business days are equally far away, default"
                            "to following business day.");

                        ImGui::EndTable();
                    }
                }
                ImGui::EndChild();
            }
            if (selected == 3) {
                ImGui::BeginChild("DateGenerationRules", ImVec2(1500, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
                {
                    ImGui::Spacing();
                    ImGui::TextWrapped("These conventions specify the rule used to generate dates in a Schedule");

                    ImGui::Dummy(ImVec2(0.f, 5.0f));
                    ImGui::Separator();
                    if (ImGui::BeginTable("BusinessDayConventions", 2, QuantIO::tableFlags,
                        ImVec2(0.0f, 565.0f), 0.0f)) {

                        ImGui::TableSetupScrollFreeze(0, 1);

                        ImGui::TableSetupColumn("LABEL", ImGuiTableColumnFlags_NoHide);
                        ImGui::TableSetupColumn("DESCRIPTION", ImGuiTableColumnFlags_NoHide);
                        ImGui::TableHeadersRow();

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Backward", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Backward from termination date to effective date.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Forward", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Forward from effective date to termination date.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Zero", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("No intermediate dates between effective date and termination date.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Third Wednesday", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("All dates but effective date and termination date are taken to be on the third wednesday of their month (with forward calculation.)");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Third Wednesday Inclusive", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("All dates including effective date and termination date are taken to be on the third wednesday of their month (with forward calculation.)");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Twentieth", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("All dates but the effective date are taken to be the twentieth of their month (used for CDS schedules in emerging markets.) The termination date is also modified.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Twentieth IMM", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("All dates but the effective date are taken to be the twentieth of an IMM month (used for CDS schedules.) The termination date is also modified.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Old CDS", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Same as TwentiethIMM with unrestricted date ends and log/short stub coupon period (old CDS convention).");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("CDS", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Credit derivatives standard rule since 'Big Bang' changes in 2009.");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("CDS2015", false, ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.f, 25.0f * 2.0f));
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Credit derivatives standard rule since December 20th, 2015.");

                        ImGui::EndTable();
                    }
                }
                ImGui::EndChild();
            }
            if (selected == 4) {
                ImGui::BeginChild("Frequency", ImVec2(1500, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
                {
                    ImGui::Spacing();
                    ImGui::TextWrapped("Frequency of events");

                    ImGui::Dummy(ImVec2(0.f, 5.0f));
                    ImGui::Separator();

                    if (ImGui::BeginTable("BusinessDayConventions", 2, QuantIO::tableFlags,
                        ImVec2(0.0f, 340.0f), 0.0f)) {

                        ImGui::TableSetupScrollFreeze(0, 1);

                        ImGui::TableSetupColumn("LABEL", ImGuiTableColumnFlags_NoHide);
                        ImGui::TableSetupColumn("DESCRIPTION", ImGuiTableColumnFlags_NoHide);
                        ImGui::TableHeadersRow();

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("No Frequency", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Null frequency");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Once", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Only once");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Annual", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Once a year");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Semiannual", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Twice a year");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Every Fourth Month", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Every fourth month");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Quarterly", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Every third month");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Bimonthly", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Every second month");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Monthly", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Once a month");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Every Fourth Week", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Every fourth week");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Biweekly", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Every second week");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Weekly", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Once a week");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Daily", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Once a day");

                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Selectable("Other Frequency", false, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::TextWrapped("Some other unknown frequency");

                        ImGui::EndTable();
                    }
                }
                ImGui::EndChild();
            }

            ImGui::EndChild();

            /*-ImGui::GetFrameHeightWithSpacing();
            if (ImGui::Button("Cancel")) {}
            ImGui::SameLine();
            if (ImGui::Button("Save")) {}*/
            ImGui::EndGroup();
        }

	}
	ImGui::EndChild();
}