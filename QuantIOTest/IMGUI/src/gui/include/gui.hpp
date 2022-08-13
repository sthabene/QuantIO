#pragma once

//Includes
//#include <iostream>
#include <memory>
//#include <string>
#include <ctype.h>          // toupper
#include <limits.h>         // INT_MIN, INT_MAX
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#include "imgui.h"
#include "imgui_internal.h"

//Db connection
#include "database.hpp"
#include "sqlite.hpp"
#include "error.hpp"

static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
};


namespace QuantIO {
	//Populates windows into the ActiveWindows vector
	void AddWindows();

	//Window Declarations
	void MainWindow(bool* pOpen = NULL);					//Main Window
	void MainMenuBar();										//Main Menu Bar
	void MainStatusBar();									//Main Status bar
	//IMGUI_API void MainSideBar(bool* pOpen = NULL);		//Main Side bar

	//Styling
	void StyleColorsCustom(ImGuiStyle* dst = NULL);		//Custom colors

	//Global variable declarations
	extern bool closeApplication; //boolean to close application

	//Generic Window Class
	class Window {
	public:
		const char*		Name; //Document title
		bool            Open; //Open or closed
		const char*		Type; //Type

		Window() = default;
		Window(const char* name, const char* type, bool open)
			: Name(name), Open(open), Type(type) {}

	public:
		void DoOpen() {
			Open = true;
		};
		void DoClose() {
			Open = false;
		};

		virtual void DisplayContents() = 0; //Pure virtual function
	};

	//Store all windows
	static ImVector<QuantIO::Window> ActiveWindows;


	//DB Connection
	static QuantIO::Connection dbConnection("..\\..\\misc\\database\\main.db");

	//Getting window position
	static ImVec2 popupLocation(ImVec2 workPos = ImGui::GetMainViewport()->WorkPos, const float pads = 1.0f) {
		const float PAD = 20.0f;
		ImVec2 windowPos;
		windowPos.x = workPos.x + pads * PAD;
		windowPos.y = workPos.y + pads * PAD;
		return windowPos;
	}

	//Global table flags
	//Table flags
	static ImGuiTableFlags tableFlags = ImGuiTableFlags_RowBg |
		ImGuiTableFlags_BordersV |
		ImGuiTableFlags_Resizable |
		ImGuiTableFlags_ContextMenuInBody |
		ImGuiTableFlags_BordersOuter |
		//ImGuiTableFlags_Reorderable |
		ImGuiTableFlags_Hideable |
		ImGuiTableFlags_SizingStretchSame |
		//ImGuiTableFlags_Sortable |
		ImGuiTableFlags_ScrollY |
		ImGuiTableFlags_NoHostExtendY;

}