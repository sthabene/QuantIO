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
        const char* configItems[] = { "Settings", "Types"};
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
            
            

            if (selected == 1) {
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
            if (selected == 0) {
                ImGui::BeginChild("Settings", ImVec2(800, 490), true, ImGuiWindowFlags_AlwaysAutoResize);
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
                    if (ImGui::DateChooser("##EvaluationDate", evaluationDate, "%Y-%m-%d", false, NULL, ICON_FA_CHEVRON_CIRCLE_LEFT, ICON_FA_CHEVRON_CIRCLE_RIGHT)) {
                        
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