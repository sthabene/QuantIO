//Disable warnings
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

//Include
#include "guiexamples.hpp"

//Add Widget Windows
void AddExampleWindows(ImVector<QuantIO::Window>& windows) {
	windows.push_back(ExampleConsole());
	windows.push_back(ExampleLog());
	windows.push_back(ExampleLayout());
	windows.push_back(ExamplePropertyEditor());
	windows.push_back(ExampleLongText());
	//windows.push_back(ExampleAutoResize());
	//windows.push_back(ExampleConstrainedResize());
	//windows.push_back(ExampleSimpleOverlay());
	//windows.push_back(ExampleFullscreen());
	//windows.push_back(ExampleWindowTitles());
	windows.push_back(ExampleCustomRendering());
	//windows.push_back(ExampleDockSpace());
	//windows.push_back(ExampleDocuments());
	windows.push_back(ExampleFiltering());
	windows.push_back(ExampleNavigation());
}


struct LogHelper {
	ImGuiTextBuffer     Buf;		 //Text buffer
	ImGuiTextFilter     Filter;		 //Text filter helper
	ImVector<int>       LineOffsets; //Index to lines offset. We maintain this with AddLog() calls.
	//bool                AutoScroll;  //Keep scrolling if already at the bottom

	LogHelper() {  //Constructor
		//AutoScroll = true;
		Clear();
	}

	void Clear() {
		Buf.clear(); 
		LineOffsets.clear();
		LineOffsets.push_back(0);
	}

	void AddLog(const char* fmt, ...) { //... just means the function can take on many other arguments
		int oldSize = Buf.size();
		va_list args;				//Holds info about arguments in ..., its needed by va_start, va_copy, va_arg, va_end
		va_start(args, fmt);		//Enables access to the variable arguments in ...
		Buf.appendfv(fmt, args);	//Helper for accumulating text
		va_end(args);				//Clean up of objects created by va_start or va_copy
		for (int newSize = Buf.size(); oldSize < newSize; oldSize++) {
			if (Buf[oldSize] == '\n') {
				LineOffsets.push_back(oldSize + 1);
			}
		}

	}
};

static LogHelper logObj;
static char textIn[1024];
static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
void ExampleLog::DisplayContents() {

	ImGui::BeginChild("Log");
	{
		bool clear = ImGui::Button("Clear");
		ImGui::SameLine();
		bool copy = ImGui::Button("Copy");
		ImGui::SameLine();
		ImGui::ColorEdit3("##COL", (float*)&color);
		ImGui::SameLine();
		ImGui::TextUnformatted("Text Color");
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::InputTextMultiline("##TEXTIN", textIn, IM_ARRAYSIZE(textIn), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), 
			ImGuiInputTextFlags_CtrlEnterForNewLine);
		ImGui::PopStyleColor();
		if (ImGui::Button("Add")) {
			logObj.AddLog(strcat(textIn, "\n"));
			strcpy(textIn, ""); //Clear text after enter
		}
		ImGui::Separator();
		logObj.Filter.Draw("Filter", -100.0f);
		ImGui::Separator();

		ImGui::BeginChild("LogOut", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		{
			if (clear) logObj.Clear();
			if (copy) ImGui::LogToClipboard();
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

			const char* buf = logObj.Buf.begin();
			const char* bufEnd = logObj.Buf.end();

			if (logObj.Filter.IsActive()) {
				for (int lineNo = 0; lineNo < logObj.LineOffsets.Size; lineNo++) {
					const char* lineStart = buf + logObj.LineOffsets[lineNo];
					const char* lineEnd = (lineNo + 1 < logObj.LineOffsets.Size) ? (buf + logObj.LineOffsets[lineNo + 1] - 1) : bufEnd;
					if (logObj.Filter.PassFilter(lineStart, lineEnd)) {
						ImGui::TextUnformatted(lineStart, lineEnd);
					}
				}
			}
			else {
				ImGuiListClipper clipper;
				clipper.Begin(logObj.LineOffsets.Size);
				while (clipper.Step()) {
					for (int lineNo = clipper.DisplayStart; lineNo < clipper.DisplayEnd; lineNo++) {
						const char* lineStart = buf + logObj.LineOffsets[lineNo];
						const char* lineEnd = (lineNo + 1 < logObj.LineOffsets.Size) ? (buf + logObj.LineOffsets[lineNo + 1] - 1) : bufEnd;
						ImGui::TextUnformatted(lineStart, lineEnd);
					}
				}
				clipper.End();
			}
			ImGui::PopStyleVar();
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
};

//Console Example
ImGuiTextFilter Filter;
char InputBuf[256];
void ExampleConsole::DisplayContents() {
	ImGui::BeginChild("Console");
	{
		ImGui::TextWrapped(
			"This example implements a console with basic coloring, completion (TAB key) and history (Up/Down keys). A more elaborate "
			"implementation may want to store entries along with extra data such as timestamp, emitter, etc.");
		ImGui::TextWrapped("Enter 'HELP' for help.");

		if (ImGui::Button("Add Debug Text")) {};
		ImGui::SameLine();
		if (ImGui::Button("Add Debug Error")) {};
		ImGui::SameLine();
		if (ImGui::Button("Clear")) {};
		ImGui::SameLine();
		bool copyToClipboard = ImGui::Button("Copy");
		ImGui::Separator();

		bool AutoScroll = true;
		if (ImGui::BeginPopup("Options")) {
			ImGui::Checkbox("Auto-scroll", &AutoScroll);
			ImGui::EndPopup();
		}

		if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
		ImGui::SameLine();
		Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 600);
		ImGui::Separator();
		const float footerHeightToReserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeightToReserve), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
		{
			if (ImGui::BeginPopupContextWindow()) {
				if (ImGui::Selectable("Clear")) {};
				ImGui::EndPopup();

			};


			if (copyToClipboard) {
				ImGui::LogToClipboard(); //Use to place output text in clipboard
			};
			static char text[1024 * 16] =
				"\n"
				"One upon a time live a very old"
				"";
			//ImGui::GetCurrentWindow()->Size;
			ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImGui::GetCurrentWindow()->Size);
			//ImGui::TextUnformatted("Hello there");

			if (copyToClipboard) {
				ImGui::LogFinish();
			}
		}
		ImGui::EndChild();
		ImGui::Separator();
		if (ImGui::InputText("Input", InputBuf, IM_ARRAYSIZE(InputBuf))) {};

	}
	ImGui::EndChild();
};
void ExampleLayout::DisplayContents() {
	ImGui::BeginChild("Simple layout", ImVec2(0, 0), false, ImGuiWindowFlags_MenuBar);
	{
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Close")) {
					ExampleLayout::DoClose();
				} 
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		static int selected = 10;
		{//Create group for the left pane that takes 10% of the work size
			ImGui::BeginChild("Simple Layout Left Pane", ImVec2(ImGui::GetMainViewport()->WorkSize.x * 0.1f, 0), true);
			for (int i = 0; i < 100; i++) {
				char label[128];
				sprintf(label, "MyObject %d", i);
				if (ImGui::Selectable(label, selected == i)) //Selectable gets highlighted
					selected = i; //Selected Item
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		{//Create group for the right pane
			ImGui::BeginGroup();
			ImGui::BeginChild("Simple Layout Right Pane", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), true);
			//The spacing at the botton creates space for the buttons
			if (ImGui::BeginTabBar("###Tabs", ImGuiTabBarFlags_None)) {
				if (ImGui::BeginTabItem("Description")) {
					ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Details")) {
					ImGui::Text("ID: 0123456789");
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::Text("MyObject: %d", selected);
			ImGui::Separator();

			ImGui::EndChild();
			if (ImGui::Button("Revert")) {}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {}
			ImGui::EndGroup();

		}
	}
	ImGui::EndChild();
};
float f = 1.0f;
void ExamplePropertyEditor::DisplayContents() {
	ImGui::BeginChild("PropertyEditor");
	{
		
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		if (ImGui::BeginTable("SPLITTABLE", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable)) {
			for (int obj = 0; obj < 10; obj++) {
				ImGui::PushID(obj);
				
				ImGui::TableNextRow(); //Starts into the first cell of a new row, basically pushes to the next row
				ImGui::TableSetColumnIndex(0); //First Column
				ImGui::AlignTextToFramePadding(); //Vertically align upcoming text
				bool nodeOpen = ImGui::TreeNode("Object", "%s %u", "Object", obj); //Start a tree node

				ImGui::TableSetColumnIndex(1); //Second Column
				ImGui::Text("Text in Second column Row %d", obj); //For each item in first column, the text will be created in 2nd column
				ImGui::Separator();
				if (nodeOpen) {
					for (int i = 0; i < 6; i++) {
						ImGui::PushID(i);
						ImGui::TableNextRow();
						ImGui::TableSetColumnIndex(0); //Column 1
						ImGui::AlignTextToFramePadding();
						ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
						ImGui::TreeNodeEx("Field", flags, "Field %d", i);

						ImGui::TableSetColumnIndex(1); //Column 2
						ImGui::SetNextItemWidth(-FLT_MIN);
						if (obj > 3) {
							ImGui::InputFloat("##value", &f, 1.0f);
						}
						else {
							ImGui::DragFloat("##value", &f, 0.01f);
						}
						ImGui::NextColumn();
						ImGui::PopID();
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
			};
			ImGui::EndTable();
		}
		ImGui::PopStyleVar();
	}
	ImGui::EndChild();
};
void ExampleLongText::DisplayContents() {
	static int lines = 0;
	static int testType = 0;
	static ImGuiTextBuffer logLongText;
	ImGui::BeginChild("LongText");
	{
		ImGui::Text("Printing unusually long amount of text.");
		ImGui::Combo("Test type", &testType,
			"Single call to TextUnformatted()\0"
			"Multiple calls to Text(), clipped\0"
			"Multiple calls to Text(), not clipped (slow)\0");
		ImGui::Text("Buffer contents: %d lines, %d bytes", lines, logLongText.size());
		if (ImGui::Button("Clear")) { logLongText.clear(); lines = 0; }
		ImGui::SameLine();
		if (ImGui::Button("Add 1000 lines")) {
			for (int i = 0; i < 1000; i++) {
				logLongText.appendf("%i The quick brown fox jumps over the lazy dog\n", lines + i);
			}
			lines += 1000;
		}
		ImGui::BeginChild("Log", ImVec2(0,0), true);
		{
			switch (testType) {
			case 0:
				ImGui::TextUnformatted(logLongText.begin(), logLongText.end());
				break;
			case 1:
			{
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				//Coarse clipping using ImGuiListClipper allows you to easily
				//scale using lists with tens of thousands of items without a problem
				ImGuiListClipper clipper;
				clipper.Begin(lines);
				while (clipper.Step()) {
					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
						ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
					}
				}
				ImGui::PopStyleVar();
				break;
			}
			case 2:
			{ //Without clipper (slow)
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
				for (int i = 0; i < lines; i++) {
					ImGui::Text("%i The quick brown fox jumps over the lazy dog", i);
				}
				ImGui::PopStyleVar();
				break;
			}
			}
		}
		ImGui::EndChild();
	}
	ImGui::EndChild();
};
void ExampleAutoResize::DisplayContents() {
	static int lines = 10;
	ImGui::BeginChild("AutoResize", ImVec2(45.0f * lines, 35.0f * lines), true, ImGuiWindowFlags_AlwaysAutoResize);
	{
		//static int lines = 10;
		ImGui::TextUnformatted(
			"Window will resize every-frame to the size of its content.\n"
			"Note that you probably don't want to query the window size to\n"
			"output your content because that would create a feedback loop.");
		ImGui::SliderInt("Number of lines", &lines, 5, 20);
		for (int i = 0; i < lines; i++) {
			ImGui::Text("%*sThis is line %d", i * 4, "", i); // Pad with space to extend size horizontally
		}
	}
	ImGui::EndChild();
};
void ExampleConstrainedResize::DisplayContents() {
	ImGui::BeginChild("Constrained Resize");
	{

	}
	ImGui::EndChild();
};

bool showOverlay = false;
void ExampleSimpleOverlay::DisplayContents() {
	ImGuiIO& io = ImGui::GetIO();
	ImVec2 workPos = ImGui::GetCurrentWindow()->Pos;
	ImVec2 workSize = ImGui::GetCurrentWindow()->Size;
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoSavedSettings;// |
		//ImGuiWindowFlags_NoFocusOnAppearing | 
		//ImGuiWindowFlags_NoNav;

	ImGui::BeginChild("Simple Overlay");
	{
		ImGui::SetNextWindowFocus(); //Make the window focus
		if (ImGui::Begin("Simple overlay", &showOverlay, windowFlags)) {
			ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
			ImGui::Separator();
			if (ImGui::IsMousePosValid()) {
				ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			}
			else {
				ImGui::Text("Mouse Position: <invalid>");
			}
			if (ImGui::BeginPopupContextWindow()) {
				if (&showOverlay && ImGui::MenuItem("Close")) { showOverlay = false; };
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}
	ImGui::EndChild();
};
void ExampleFullscreen::DisplayContents() {
	ImGui::BeginChild("Fullscreen");
	{

	}
	ImGui::EndChild();
};
void ExampleWindowTitles::DisplayContents() {
	ImGui::BeginChild("Window Titles");
	{

	}
	ImGui::EndChild();
};
void ExampleCustomRendering::DisplayContents() {
	ImGui::BeginChild("Custom Rendering");
	{
		if (ImGui::BeginTabBar("##TabBar")) {
			if (ImGui::BeginTabItem("Primitives")) {
				ImGui::Text("Gradients");
				ImDrawList* drawList = ImGui::GetWindowDrawList();
				ImVec2 gradientSize = ImVec2(ImGui::CalcItemWidth(), ImGui::GetFrameHeight());
				{
					ImVec2 p0 = ImGui::GetCursorScreenPos();
					ImVec2 p1 = ImVec2(p0.x + gradientSize.x, p0.y + gradientSize.y);
					ImU32 colA = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));
					ImU32 colB = ImGui::GetColorU32(IM_COL32(255, 255, 255, 255));
					drawList->AddRectFilledMultiColor(p0, p1, colA, colB, colB, colA);
					ImGui::InvisibleButton("##gradient1", gradientSize);
				}
				{
					ImVec2 p0 = ImGui::GetCursorScreenPos();
					ImVec2 p1 = ImVec2(p0.x + gradientSize.x, p0.y + gradientSize.y);
					ImU32 colA = ImGui::GetColorU32(IM_COL32(0, 255, 0, 255));
					ImU32 colB = ImGui::GetColorU32(IM_COL32(255, 0, 0, 255));
					drawList->AddRectFilledMultiColor(p0, p1, colA, colB, colB, colA);
					ImGui::InvisibleButton("##gradient2", gradientSize);
				}
				ImGui::Text("All primitives");

				static float thickness = 3.0f;
				static float size = 36.0f;
				static int ngonSides = 6;
				const float spacing = 10.0f;

				const ImU32 col = ImColor(ImVec4(1.0f, 1.0f, 0.4f, 1.0f));
				const ImVec2 p = ImGui::GetCursorScreenPos();
				float x = p.x + 4.0f;
				float y = p.y + 4.0f;
				for (int n = 0; n < 5; n++) {
					float th = (n == 0) ? 1.0f : thickness;
					drawList->AddNgon(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, ngonSides, th);
					x += size + spacing;  // N-gon
				};

				ImGui::EndTabItem();
			};
			if (ImGui::BeginTabItem("Canvas")) {
				static bool opt_enable_grid = true;
				ImGui::Checkbox("Enable grid", &opt_enable_grid);

				ImGui::EndTabItem();
			};
			if (ImGui::BeginTabItem("BG/FG draw lists")) {
				static bool draw_bg = true;
				static bool draw_fg = true;
				ImGui::Checkbox("Draw in Background draw list", &draw_bg);
				ImGui::SameLine(); //HelpMarker("The Background draw list will be rendered below every Dear ImGui windows.");
				ImGui::Checkbox("Draw in Foreground draw list", &draw_fg);
				ImGui::SameLine(); //HelpMarker("The Foreground draw list will be rendered over every Dear ImGui windows.");
				ImVec2 window_pos = ImGui::GetWindowPos();
				ImVec2 window_size = ImGui::GetWindowSize();
				ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);
				if (draw_bg)
					ImGui::GetBackgroundDrawList()->AddCircle(window_center, window_size.x * 0.6f, IM_COL32(255, 0, 0, 200), 0, 10 + 4);
				if (draw_fg)
					ImGui::GetForegroundDrawList()->AddCircle(window_center, window_size.y * 0.6f, IM_COL32(0, 255, 0, 200), 0, 10);
				ImGui::EndTabItem();
			};

			ImGui::EndTabBar();
		}
	}
	ImGui::EndChild();
};
void ExampleDockSpace::DisplayContents() {
	ImGui::BeginChild("Dock Space");
	{

	}
	ImGui::EndChild();
};
void ExampleDocuments::DisplayContents() {
	ImGui::BeginChild("Documents");
	{

	}
	ImGui::EndChild();
};
void ExampleFiltering::DisplayContents() {
	static bool closable_group = true;
	ImGui::BeginChild("Filtering");
	{
		// Helper class to easy setup a text filter.
		// You may want to implement a more feature-full filtering scheme in your own application.
		static ImGuiTextFilter filter;
		ImGui::Text("Filter usage:\n"
			"  \"\"         display all lines\n"
			"  \"xxx\"      display lines containing \"xxx\"\n"
			"  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n"
			"  \"-xxx\"     hide lines containing \"xxx\"");
		filter.Draw();
		const char* lines[] = { "aaa1.c", "bbb1.c", "ccc1.c", "aaa2.cpp", "bbb2.cpp", "ccc2.cpp", "abc.h", "hello, world" };
		for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
			if (filter.PassFilter(lines[i])) {
				//ImGui::Text("%s", lines[i]);
				char buf[128];
				sprintf(buf, "%s", lines[i]);
				ImGui::Checkbox(buf, &closable_group);
			}
				
	}
	ImGui::EndChild();
}

void ExampleNavigation::DisplayContents() {
	ImGui::BeginChild("ExampleNavigation");
	{
		ImGuiIO& io = ImGui::GetIO();
		if (ImGui::CollapsingHeader("Output"), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet) {
			ImGui::Text("io.WantCaptureMouse: %d", io.WantCaptureMouse);
			ImGui::Text("io.WantCaptureMouseUnlessPopupClose: %d", io.WantCaptureMouseUnlessPopupClose);
			ImGui::Text("io.WantCaptureKeyboard: %d", io.WantCaptureKeyboard);
			ImGui::Text("io.WantTextInput: %d", io.WantTextInput);
			ImGui::Text("io.WantSetMousePos: %d", io.WantSetMousePos);
			ImGui::Text("io.NavActive: %d, io.NavVisible: %d", io.NavActive, io.NavVisible);
		}

		if (ImGui::CollapsingHeader("Mouse State"), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet) {
			if (ImGui::IsMousePosValid()) {
				ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
			}
			else {
				ImGui::Text("Mouse pos: <INVALID>");
			}

			ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
			int count = IM_ARRAYSIZE(io.MouseDown);
			ImGui::Text("Mouse down:");         
			for (int i = 0; i < count; i++) {
				if (ImGui::IsMouseDown(i)) {
					ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
				}
			}
			ImGui::Text("Mouse clicked:");      
			for (int i = 0; i < count; i++) {
				if (ImGui::IsMouseClicked(i)) {
					ImGui::SameLine(); ImGui::Text("b%d (%d)", i, ImGui::GetMouseClickedCount(i));
				}
			}
			ImGui::Text("Mouse released:");     
			for (int i = 0; i < count; i++) {
				if (ImGui::IsMouseReleased(i)) {
					ImGui::SameLine(); ImGui::Text("b%d", i);
				}
			}
			ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
			ImGui::Text("Pen Pressure: %.1f", io.PenPressure); // Note: currently unused
		}
		if (ImGui::CollapsingHeader("Keyboard, Gamepad & Navigation State"), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet) {
			struct funcs { static bool IsLegacyNativeDupe(ImGuiKey key) { return key < 512 && ImGui::GetIO().KeyMap[key] != -1; } }; 
			// Hide Native<>ImGuiKey duplicates when both exists in the array
			const ImGuiKey key_first = 0;

			ImGui::Text("Keys down:");          
			for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { 
				if (funcs::IsLegacyNativeDupe(key)) continue; if (ImGui::IsKeyDown(key)) { 
					ImGui::SameLine(); 
					ImGui::Text("\"%s\" %d (%.02f secs)", ImGui::GetKeyName(key), key, ImGui::GetKeyData(key)->DownDuration); 
				} 
			}
			ImGui::Text("Keys pressed:");       
			for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { 
				if (funcs::IsLegacyNativeDupe(key)) continue; 
				if (ImGui::IsKeyPressed(key)) { 
					ImGui::SameLine(); ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key); 
				} 
			}
			ImGui::Text("Keys released:");      
			for (ImGuiKey key = key_first; key < ImGuiKey_COUNT; key++) { 
				if (funcs::IsLegacyNativeDupe(key)) continue; 
				if (ImGui::IsKeyReleased(key)) { 
					ImGui::SameLine(); ImGui::Text("\"%s\" %d", ImGui::GetKeyName(key), key); 
				} 
			}
			ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
			ImGui::Text("Chars queue:");        
			for (int i = 0; i < io.InputQueueCharacters.Size; i++) { 
				ImWchar c = io.InputQueueCharacters[i]; ImGui::SameLine();  
				ImGui::Text("\'%c\' (0x%04X)", (c > ' ' && c <= 255) ? (char)c : '?', c); 
				// FIXME: We should convert 'c' to UTF-8 here but the functions are not public.
			} 
			ImGui::Text("NavInputs down:");     
			for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) {
				if (io.NavInputs[i] > 0.0f) {
					ImGui::SameLine(); ImGui::Text("[%d] %.2f (%.02f secs)", i, io.NavInputs[i], io.NavInputsDownDuration[i]);
				}
			}
			ImGui::Text("NavInputs pressed:");  
			for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) {
				if (io.NavInputsDownDuration[i] == 0.0f) {
					ImGui::SameLine(); ImGui::Text("[%d]", i);
				}
			}
			{
				const ImVec2 key_size = ImVec2(35.0f, 35.0f);
				const float  key_rounding = 3.0f;
				const ImVec2 key_face_size = ImVec2(25.0f, 25.0f);
				const ImVec2 key_face_pos = ImVec2(5.0f, 3.0f);
				const float  key_face_rounding = 2.0f;
				const ImVec2 key_label_pos = ImVec2(7.0f, 4.0f);
				const ImVec2 key_step = ImVec2(key_size.x - 1.0f, key_size.y - 1.0f);
				const float  key_row_offset = 9.0f;

				ImVec2 board_min = ImGui::GetCursorScreenPos();
				ImVec2 board_max = ImVec2(board_min.x + 3 * key_step.x + 2 * key_row_offset + 10.0f, board_min.y + 3 * key_step.y + 10.0f);
				ImVec2 start_pos = ImVec2(board_min.x + 5.0f - key_step.x, board_min.y);

				struct KeyLayoutData { int Row, Col; const char* Label; ImGuiKey Key; };
				const KeyLayoutData keys_to_display[] =
				{
					{ 0, 0, "", ImGuiKey_Tab },      
					{ 0, 1, "Q", ImGuiKey_Q }, 
					{ 0, 2, "W", ImGuiKey_W }, 
					{ 0, 3, "E", ImGuiKey_E }, 
					{ 0, 4, "R", ImGuiKey_R },
					{ 1, 0, "", ImGuiKey_CapsLock }, 
					{ 1, 1, "A", ImGuiKey_A }, 
					{ 1, 2, "S", ImGuiKey_S }, 
					{ 1, 3, "D", ImGuiKey_D }, 
					{ 1, 4, "F", ImGuiKey_F },
					{ 2, 0, "", ImGuiKey_LeftShift },
					{ 2, 1, "Z", ImGuiKey_Z }, 
					{ 2, 2, "X", ImGuiKey_X }, 
					{ 2, 3, "C", ImGuiKey_C }, 
					{ 2, 4, "V", ImGuiKey_V }
				};

				// Elements rendered manually via ImDrawList API are not clipped automatically.
				// While not strictly necessary, here IsItemVisible() is used to avoid rendering these shapes when they are out of view.
				ImGui::Dummy(ImVec2(board_max.x - board_min.x, board_max.y - board_min.y));
				if (ImGui::IsItemVisible()) {
					ImDrawList* draw_list = ImGui::GetWindowDrawList();
					draw_list->PushClipRect(board_min, board_max, true);
					for (int n = 0; n < IM_ARRAYSIZE(keys_to_display); n++) {
						const KeyLayoutData* key_data = &keys_to_display[n];
						ImVec2 key_min = ImVec2(start_pos.x + key_data->Col * key_step.x + key_data->Row * key_row_offset, 
							start_pos.y + key_data->Row * key_step.y);
						ImVec2 key_max = ImVec2(key_min.x + key_size.x, key_min.y + key_size.y);
						draw_list->AddRectFilled(key_min, key_max, IM_COL32(204, 204, 204, 255), key_rounding);
						draw_list->AddRect(key_min, key_max, IM_COL32(24, 24, 24, 255), key_rounding);
						ImVec2 face_min = ImVec2(key_min.x + key_face_pos.x, key_min.y + key_face_pos.y);
						ImVec2 face_max = ImVec2(face_min.x + key_face_size.x, face_min.y + key_face_size.y);
						draw_list->AddRect(face_min, face_max, IM_COL32(193, 193, 193, 255), key_face_rounding, ImDrawFlags_None, 2.0f);
						draw_list->AddRectFilled(face_min, face_max, IM_COL32(252, 252, 252, 255), key_face_rounding);
						ImVec2 label_min = ImVec2(key_min.x + key_label_pos.x, key_min.y + key_label_pos.y);
						draw_list->AddText(label_min, IM_COL32(64, 64, 64, 255), key_data->Label);
						if (ImGui::IsKeyDown(key_data->Key))
							draw_list->AddRectFilled(key_min, key_max, IM_COL32(255, 0, 0, 128), key_rounding);
					}
					draw_list->PopClipRect();
				}
			}
		}

		if (ImGui::CollapsingHeader("Capture override"), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet) {
			HelpMarker(
				"The value of io.WantCaptureMouse and io.WantCaptureKeyboard are normally set by Dear ImGui "
				"to instruct your application of how to route inputs. Typically, when a value is true, it means "
				"Dear ImGui wants the corresponding inputs and we expect the underlying application to ignore them.\n\n"
				"The most typical case is: when hovering a window, Dear ImGui set io.WantCaptureMouse to true, "
				"and underlying application should ignore mouse inputs (in practice there are many and more subtle "
				"rules leading to how those flags are set).");

			ImGui::Text("io.WantCaptureMouse: %d", io.WantCaptureMouse);
			ImGui::Text("io.WantCaptureMouseUnlessPopupClose: %d", io.WantCaptureMouseUnlessPopupClose);
			ImGui::Text("io.WantCaptureKeyboard: %d", io.WantCaptureKeyboard);

			HelpMarker(
				"Hovering the colored canvas will override io.WantCaptureXXX fields.\n"
				"Notice how normally (when set to none), the value of io.WantCaptureKeyboard would be false when hovering and true when clicking.");
			static int capture_override_mouse = -1;
			static int capture_override_keyboard = -1;
			const char* capture_override_desc[] = { "None", "Set to false", "Set to true" };
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 15);
			ImGui::SliderInt("SetNextFrameWantCaptureMouse()", &capture_override_mouse, -1, +1, capture_override_desc[capture_override_mouse + 1], ImGuiSliderFlags_AlwaysClamp);
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 15);
			ImGui::SliderInt("SetNextFrameWantCaptureKeyboard()", &capture_override_keyboard, -1, +1, capture_override_desc[capture_override_keyboard + 1], ImGuiSliderFlags_AlwaysClamp);

			ImGui::ColorButton("##panel", ImVec4(0.7f, 0.1f, 0.7f, 1.0f), ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, ImVec2(256.0f, 192.0f)); // Dummy item
			if (ImGui::IsItemHovered() && capture_override_mouse != -1)
				ImGui::SetNextFrameWantCaptureMouse(capture_override_mouse == 1);
			if (ImGui::IsItemHovered() && capture_override_keyboard != -1)
				ImGui::SetNextFrameWantCaptureKeyboard(capture_override_keyboard == 1);
		}

		if (ImGui::CollapsingHeader("Tabbing"), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet) {
			ImGui::Text("Use TAB/SHIFT+TAB to cycle through keyboard editable fields.");
			static char buf[32] = "hello";
			ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
			ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
			ImGui::InputText("3", buf, IM_ARRAYSIZE(buf));
			ImGui::PushAllowKeyboardFocus(false);
			ImGui::InputText("4 (tab skip)", buf, IM_ARRAYSIZE(buf));
			ImGui::SameLine(); HelpMarker("Item won't be cycled through when using TAB or Shift+Tab.");
			ImGui::PopAllowKeyboardFocus();
			ImGui::InputText("5", buf, IM_ARRAYSIZE(buf));
		}

		if (ImGui::CollapsingHeader("Focus from code"), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet) {
			bool focus_1 = ImGui::Button("Focus on 1"); ImGui::SameLine();
			bool focus_2 = ImGui::Button("Focus on 2"); ImGui::SameLine();
			bool focus_3 = ImGui::Button("Focus on 3");
			int has_focus = 0;
			static char buf[128] = "click on a button to set focus";

			if (focus_1) ImGui::SetKeyboardFocusHere();
			ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
			if (ImGui::IsItemActive()) has_focus = 1;

			if (focus_2) ImGui::SetKeyboardFocusHere();
			ImGui::InputText("2", buf, IM_ARRAYSIZE(buf));
			if (ImGui::IsItemActive()) has_focus = 2;

			ImGui::PushAllowKeyboardFocus(false);
			if (focus_3) ImGui::SetKeyboardFocusHere();
			ImGui::InputText("3 (tab skip)", buf, IM_ARRAYSIZE(buf));
			if (ImGui::IsItemActive()) has_focus = 3;
			ImGui::SameLine(); HelpMarker("Item won't be cycled through when using TAB or Shift+Tab.");
			ImGui::PopAllowKeyboardFocus();

			if (has_focus)
				ImGui::Text("Item with focus: %d", has_focus);
			else
				ImGui::Text("Item with focus: <none>");

			// Use >= 0 parameter to SetKeyboardFocusHere() to focus an upcoming item
			static float f3[3] = { 0.0f, 0.0f, 0.0f };
			int focus_ahead = -1;
			if (ImGui::Button("Focus on X")) { focus_ahead = 0; } ImGui::SameLine();
			if (ImGui::Button("Focus on Y")) { focus_ahead = 1; } ImGui::SameLine();
			if (ImGui::Button("Focus on Z")) { focus_ahead = 2; }
			if (focus_ahead != -1) ImGui::SetKeyboardFocusHere(focus_ahead);
			ImGui::SliderFloat3("Float3", &f3[0], 0.0f, 1.0f);

			ImGui::TextWrapped("NB: Cursor & selection are preserved when refocusing last used item in code.");
		}

		if (ImGui::CollapsingHeader("Dragging"), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet) {
			ImGui::TextWrapped("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget.");
			for (int button = 0; button < 3; button++)
			{
				ImGui::Text("IsMouseDragging(%d):", button);
				ImGui::Text("  w/ default threshold: %d,", ImGui::IsMouseDragging(button));
				ImGui::Text("  w/ zero threshold: %d,", ImGui::IsMouseDragging(button, 0.0f));
				ImGui::Text("  w/ large threshold: %d,", ImGui::IsMouseDragging(button, 20.0f));
			}

			ImGui::Button("Drag Me");
			if (ImGui::IsItemActive())
				ImGui::GetForegroundDrawList()->AddLine(io.MouseClickedPos[0], io.MousePos, ImGui::GetColorU32(ImGuiCol_Button), 4.0f); // Draw a line between the button and the mouse cursor

			// Drag operations gets "unlocked" when the mouse has moved past a certain threshold
			// (the default threshold is stored in io.MouseDragThreshold). You can request a lower or higher
			// threshold using the second parameter of IsMouseDragging() and GetMouseDragDelta().
			ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
			ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
			ImVec2 mouse_delta = io.MouseDelta;
			ImGui::Text("GetMouseDragDelta(0):");
			ImGui::Text("  w/ default threshold: (%.1f, %.1f)", value_with_lock_threshold.x, value_with_lock_threshold.y);
			ImGui::Text("  w/ zero threshold: (%.1f, %.1f)", value_raw.x, value_raw.y);
			ImGui::Text("io.MouseDelta: (%.1f, %.1f)", mouse_delta.x, mouse_delta.y);
		}

		if (ImGui::CollapsingHeader("Mouse cursors"), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet) {
			ImGuiIO& io = ImGui::GetIO();
			const char* mouse_cursors_names[] = { "Arrow", "TextInput", "ResizeAll", "ResizeNS", "ResizeEW", "ResizeNESW", "ResizeNWSE", "Hand", "NotAllowed" };
			IM_ASSERT(IM_ARRAYSIZE(mouse_cursors_names) == ImGuiMouseCursor_COUNT);

			ImGuiMouseCursor current = ImGui::GetMouseCursor();
			ImGui::Text("Current mouse cursor = %d: %s", current, mouse_cursors_names[current]);
			ImGui::Text("Hover to see mouse cursors:");
			ImGui::SameLine(); HelpMarker(
				"Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. "
				"If software cursor rendering (io.MouseDrawCursor) is set ImGui will draw the right cursor for you, "
				"otherwise your backend needs to handle it.");
			for (int i = 0; i < ImGuiMouseCursor_COUNT; i++) {
				char label[32];
				sprintf(label, "Mouse cursor %d: %s", i, mouse_cursors_names[i]);
				ImGui::Bullet(); ImGui::Selectable(label, false);
				if (ImGui::IsItemHovered())
					ImGui::SetMouseCursor(i);
			}
		}
		
	}
	ImGui::EndChild();
}