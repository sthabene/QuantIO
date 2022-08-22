#include "include/guiaddons.hpp"

//#include "addons/imguibindings/imguibindings.h"
//#include "addons/imguilistview/imguilistview.h"
//#include "addons/imguifilesystem/imguifilesystem.h"
//#include "addons/imguitoolbar/imguitoolbar.h"
//#include "addons/imguipanelmanager/imguipanelmanager.h"
//#include "addons/imguivariouscontrols/imguivariouscontrols.h"
//#include "addons/imguistyleserializer/imguistyleserializer.h"
#include "addons/imguidatechooser/imguidatechooser.h"
//#include "addons/imguicodeeditor/imguicodeeditor.h"
//#include "addons/imguinodegrapheditor/imguinodegrapheditor.h"
//#include "addons/imguidock/imguidock.h"

ImTextureID myImageTextureId2 = 0;
void AddonsDatePicker::DisplayContents() {
    ImGui::BeginChild("Addons");
    {
		ImGui::Text("Choose a date:");
		ImGui::SameLine();
		static tm myDate = {};       // IMPORTANT: must be static! (plenty of compiler warnings here if we write: static tm myDate={0}; Is there any difference?)
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		ImGui::PushItemWidth(35.0f * 5.0f);
		if (ImGui::DateChooser("Date Chooser##MyDateChooser", myDate, "%a, %d %b %Y")) {
			// A new date has been chosen
			//fprintf(stderr,"A new date has been chosen exacty now: \"%.2d-%.2d-%.4d\"\n",myDate.tm_mday,myDate.tm_mon+1,myDate.tm_year+1900);
		}
		ImGui::PopItemWidth();
		ImGui::Text("Chosen date: \"%.2d-%.2d-%.4d\"", myDate.tm_mday, myDate.tm_mon + 1, myDate.tm_year + 1900);
        ImGui::Spacing();
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Choose another date:");
        ImGui::SameLine();
        static tm myDate2 = {};       // IMPORTANT: must be static! (plenty of compiler warnings here if we write: static tm myDate={0}; Is there any difference?)
        ImGui::PushItemWidth(35.0f * 10.0f);
        if (ImGui::DateChooser("##MyDateChooser2", myDate2, "%d/%m/%Y")) {
            // A new date has been chosen
            //fprintf(stderr,"A new date has been chosen exacty now: \"%.2d-%.2d-%.4d\"\n",myDate2.tm_mday,myDate2.tm_mon+1,myDate2.tm_year+1900);
        }
        ImGui::PopItemWidth();
        ImGui::Text("Chosen date2: \"%.2d-%.2d-%.4d\"", myDate2.tm_mday, myDate2.tm_mon + 1, myDate2.tm_year + 1900);

        
    }
    ImGui::EndChild();
}