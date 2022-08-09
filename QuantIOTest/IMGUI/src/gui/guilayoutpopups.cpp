//Disable warnings
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

//Include
#include "guilayoutpopups.hpp"

//Example Windows Content
//Add Widget Windows
void AddLayoutPopups(ImVector<QuantIO::Window>& windows) {
	windows.push_back(LayoutChildWindows());
	windows.push_back(LayoutWidgetsWidth());
	windows.push_back(LayoutHorizontal());
	windows.push_back(LayoutGroups());
	windows.push_back(LayoutTextAlignment());
	windows.push_back(LayoutScrolling());
	windows.push_back(LayoutClipping());
	windows.push_back(PopupMain());
	windows.push_back(PopupContextMenu());
	windows.push_back(PopupModals());
	windows.push_back(PopupMenus());
}

void LayoutChildWindows::DisplayContents() {
	ImGui::BeginChild("LayoutChildWindows");
	{
		HelpMarker("Use child windows to begin into a self-contained independent scrolling/clipping regions within a host window.");
		static bool disableMouseWheel = false;
		static bool disableMenu = false;
		ImGui::Checkbox("Disable Mouse Wheel", &disableMouseWheel);
		ImGui::Checkbox("Disable Menu", &disableMenu);
		{
			ImGuiWindowFlags windowFlags = ImGuiWindowFlags_HorizontalScrollbar;
			if (disableMouseWheel) windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
			ImGui::BeginChild("ChildLeft", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 
				ImGui::GetContentRegionAvail().y * 0.5f), false, windowFlags);
			{
				for (int i = 0; i < 100; i++) {
					ImGui::Text("%04d: scrollable region", i);
				}
			}
			ImGui::EndChild();
		}
		ImGui::SameLine();
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
			if (!disableMenu) window_flags |= ImGuiWindowFlags_MenuBar;
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
			ImGui::BeginChild("ChildRight", ImVec2(0, ImGui::GetContentRegionAvail().y * 0.5f), true, window_flags);
			{
				if (!disableMenu && ImGui::BeginMenuBar()) {
					if (ImGui::BeginMenu("Menu")) {
						ImGui::MenuItem("(demo menu)", NULL, false, false);
						if (ImGui::MenuItem("New")) {}
						if (ImGui::MenuItem("Open", "Ctrl+O")) {}
						if (ImGui::BeginMenu("Open Recent"))
						{
							ImGui::MenuItem("fish_hat.c");
							ImGui::MenuItem("fish_hat.inl");
							ImGui::MenuItem("fish_hat.h");
							if (ImGui::BeginMenu("More.."))
							{
								ImGui::MenuItem("Hello");
								ImGui::MenuItem("Sailor");
								ImGui::EndMenu();
							}
							ImGui::EndMenu();
						}
						if (ImGui::MenuItem("Save", "Ctrl+S")) {}
						if (ImGui::MenuItem("Save As..")) {}

						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}
				if (ImGui::BeginTable("split", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings)) {
					for (int i = 0; i < 100; i++) {
						char buf[32];
						sprintf(buf, "%03d", i);
						ImGui::TableNextColumn();
						//ImGui::Selectable(buf);
						ImGui::Button(buf, ImVec2(-FLT_MIN, 0.0f));
					}
					ImGui::EndTable();
				}
			}
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}
		{
			static int offsetX = 0;
			ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
			ImGui::DragInt("Offset X", &offsetX, 1.0f, -1000, 1000);

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (float)offsetX);
			ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(255, 0, 0, 100));

			ImGui::BeginChild("Red", ImVec2(ImGui::GetContentRegionAvail().x, -3 * ImGui::GetTextLineHeight()), 
				true, ImGuiWindowFlags_None);
			for (int n = 0; n < 50; n++) {
				ImGui::Text("Some test %d", n);
			}
			ImGui::EndChild();
			bool childIsHovered = ImGui::IsItemHovered();
			ImVec2 childRectMin = ImGui::GetItemRectMin();
			ImVec2 childRectMax = ImGui::GetItemRectMax();
			ImGui::PopStyleColor();
			ImGui::Text("Hovered: %d", childIsHovered);
			ImGui::Text("Rect of child window is: (%.0f,%.0f) (%.0f,%.0f)", childRectMin.x, childRectMin.y,
				childRectMax.x, childRectMax.y);
		}

	}
	ImGui::EndChild();
};
void LayoutWidgetsWidth::DisplayContents() {
	ImGui::BeginChild("LayoutWidgetsWidth", ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
	{
		static float f = 0.0f;
		static bool showIndentedItems = true;
		ImGui::Checkbox("Show indented items", &showIndentedItems);
		ImGui::Text("SetNextItemWidth/PushItemWidth(100)");
		ImGui::SameLine(); HelpMarker("Fixed width.");
		ImGui::PushItemWidth(100);
		ImGui::DragFloat("float##1b", &f);
		if (showIndentedItems) {
			ImGui::Indent();
			ImGui::DragFloat("float (indented)##1b", &f);
			ImGui::Unindent();
		}
		ImGui::PopItemWidth();

		ImGui::Text("SetNextItemWidth/PushItemWidth(-100)");
		ImGui::SameLine(); HelpMarker("Align to right edge minus 100");
		ImGui::PushItemWidth(-100);
		ImGui::DragFloat("float##2a", &f);
		if (showIndentedItems)
		{
			ImGui::Indent();
			ImGui::DragFloat("float (indented)##2b", &f);
			ImGui::Unindent();
		}
		ImGui::PopItemWidth();


		ImGui::Text("SetNextItemWidth/PushItemWidth(GetContentRegionAvail().x * 0.5f)");
		ImGui::SameLine(); HelpMarker("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)");
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.5f);
		ImGui::DragFloat("float##3a", &f);
		if (showIndentedItems)
		{
			ImGui::Indent();
			ImGui::DragFloat("float (indented)##3b", &f);
			ImGui::Unindent();
		}
		ImGui::PopItemWidth();

		ImGui::Text("SetNextItemWidth/PushItemWidth(-GetContentRegionAvail().x * 0.5f)");
		ImGui::SameLine(); HelpMarker("Align to right edge minus half");
		ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
		ImGui::DragFloat("float##4a", &f);
		if (showIndentedItems)
		{
			ImGui::Indent();
			ImGui::DragFloat("float (indented)##4b", &f);
			ImGui::Unindent();
		}
		ImGui::PopItemWidth();

		// Demonstrate using PushItemWidth to surround three items.
		// Calling SetNextItemWidth() before each of them would have the same effect.
		ImGui::Text("SetNextItemWidth/PushItemWidth(-FLT_MIN)");
		ImGui::SameLine(); HelpMarker("Align to right edge");
		ImGui::PushItemWidth(-FLT_MIN);
		ImGui::DragFloat("##float5a", &f);
		if (showIndentedItems)
		{
			ImGui::Indent();
			ImGui::DragFloat("float (indented)##5b", &f);
			ImGui::Unindent();
		}
		ImGui::PopItemWidth();
	}
	ImGui::EndChild();
};
void LayoutHorizontal::DisplayContents() {
	ImGui::BeginChild("HorizontalLayout");
	{
		ImGui::TextWrapped("(Use ImGui::SameLine() to keep adding items to the right of the preceding item)");
		ImGui::Text("Two items: Hello"); 
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sailor");

		ImGui::Text("More spacing: Hello"); 
		ImGui::SameLine(0, 20);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Sailor");

		// Button
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Normal buttons"); 
		ImGui::SameLine();
		ImGui::Button("Banana"); 
		ImGui::SameLine();
		ImGui::Button("Apple"); 
		ImGui::SameLine();
		ImGui::Button("Corniflower");

		// Button
		ImGui::Text("Small buttons"); 
		ImGui::SameLine();
		ImGui::SmallButton("Like this one"); 
		ImGui::SameLine();
		ImGui::Text("can fit within a text block.");

		// Aligned to arbitrary position. Easy/cheap column.
		ImGui::Text("Aligned");
		ImGui::SameLine(150); ImGui::Text("x=150");
		ImGui::SameLine(300); ImGui::Text("x=300");
		ImGui::Text("Aligned");
		ImGui::SameLine(150); ImGui::SmallButton("x=150");
		ImGui::SameLine(300); ImGui::SmallButton("x=300");

		// Checkbox
		static bool c1 = false, c2 = false, c3 = false, c4 = false;
		ImGui::Checkbox("My", &c1); ImGui::SameLine();
		ImGui::Checkbox("Tailor", &c2); ImGui::SameLine();
		ImGui::Checkbox("Is", &c3); ImGui::SameLine();
		ImGui::Checkbox("Rich", &c4);

		// Various
		static float f0 = 1.0f, f1 = 2.0f, f2 = 3.0f;
		ImGui::PushItemWidth(80);
		const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD" };
		static int item = -1;
		ImGui::Combo("Combo", &item, items, IM_ARRAYSIZE(items)); ImGui::SameLine();
		ImGui::SliderFloat("X", &f0, 0.0f, 5.0f); ImGui::SameLine();
		ImGui::SliderFloat("Y", &f1, 0.0f, 5.0f); ImGui::SameLine();
		ImGui::SliderFloat("Z", &f2, 0.0f, 5.0f);
		ImGui::PopItemWidth();

		ImGui::PushItemWidth(80);
		ImGui::Text("Lists:");
		static int selection[4] = { 0, 1, 2, 3 };
		for (int i = 0; i < 4; i++)
		{
			if (i > 0) ImGui::SameLine();
			ImGui::PushID(i);
			ImGui::ListBox("", &selection[i], items, IM_ARRAYSIZE(items));
			ImGui::PopID();
			//if (ImGui::IsItemHovered()) ImGui::SetTooltip("ListBox %d hovered", i);
		}
		ImGui::PopItemWidth();

		// Dummy
		ImVec2 button_sz(40, 40);
		ImGui::Button("A", button_sz); ImGui::SameLine();
		ImGui::Dummy(button_sz); ImGui::SameLine();
		ImGui::Button("B", button_sz);

		// Manually wrapping
		// (we should eventually provide this as an automatic layout feature, but for now you can do it manually)
		ImGui::Text("Manual wrapping:");
		ImGuiStyle& style = ImGui::GetStyle();
		int buttons_count = 20;
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
		for (int n = 0; n < buttons_count; n++)
		{
			ImGui::PushID(n);
			ImGui::Button("Box", button_sz);
			float last_button_x2 = ImGui::GetItemRectMax().x;
			float next_button_x2 = last_button_x2 + style.ItemSpacing.x + button_sz.x; // Expected position if next button was on same line
			if (n + 1 < buttons_count && next_button_x2 < window_visible_x2)
				ImGui::SameLine();
			ImGui::PopID();
		}

	}
	ImGui::EndChild();
}
void LayoutGroups::DisplayContents() {
	ImGui::BeginChild("LayoutGroups`");
	{
		HelpMarker(
			"BeginGroup() basically locks the horizontal position for new line. "
			"EndGroup() bundles the whole group so that you can use \"item\" functions such as "
			"IsItemHovered()/IsItemActive() or SameLine() etc. on the whole group.");
		ImGui::BeginGroup();
		{
			ImGui::BeginGroup();
			{
				ImGui::Button("AAA");
				ImGui::SameLine();
				ImGui::Button("BBB");
				ImGui::SameLine();

				ImGui::BeginGroup();
				{
					ImGui::Button("CCC");
					ImGui::Button("DDD");
				}
				ImGui::EndGroup();

				ImGui::SameLine();
				ImGui::Button("EEE");
			}
			ImGui::EndGroup();

			if (ImGui::IsItemHovered()) ImGui::SetTooltip("First group hovered");
		}
		// Capture the group size and create widgets using the same size
		ImVec2 size = ImGui::GetItemRectSize();
		const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
		ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

		ImGui::Button("ACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
		ImGui::SameLine();
		ImGui::Button("REACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x) * 0.5f, size.y));
		ImGui::EndGroup();

		ImGui::SameLine();

		ImGui::Button("LEVERAGE\nBUZZWORD", size);
		ImGui::SameLine();

		if (ImGui::BeginListBox("List", size)) {
			ImGui::Selectable("Selected", true);
			ImGui::Selectable("Not Selected", false);
			ImGui::EndListBox();
		}
	}
	ImGui::EndChild();
}
void LayoutTextAlignment::DisplayContents() {
	ImGui::BeginChild("LayoutTextAlignment");
	{
		{
			ImGui::BulletText("Text baseline:");
			ImGui::SameLine(); HelpMarker(
				"This is testing the vertical alignment that gets applied on text to keep it aligned with widgets. "
				"Lines only composed of text or \"small\" widgets use less vertical space than lines with framed widgets.");
			ImGui::Indent();

			ImGui::Text("KO Blahblah"); ImGui::SameLine();
			ImGui::Button("Some framed item"); ImGui::SameLine();
			HelpMarker("Baseline of button will look misaligned with text..");

			// If your line starts with text, call AlignTextToFramePadding() to align text to upcoming widgets.
			// (because we don't know what's coming after the Text() statement, we need to move the text baseline
			// down by FramePadding.y ahead of time)
			ImGui::AlignTextToFramePadding();
			ImGui::Text("OK Blahblah"); ImGui::SameLine();
			ImGui::Button("Some framed item"); ImGui::SameLine();
			HelpMarker("We call AlignTextToFramePadding() to vertically align the text baseline by +FramePadding.y");

			// SmallButton() uses the same vertical padding as Text
			ImGui::Button("TEST##1"); ImGui::SameLine();
			ImGui::Text("TEST"); ImGui::SameLine();
			ImGui::SmallButton("TEST##2");

			// If your line starts with text, call AlignTextToFramePadding() to align text to upcoming widgets.
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Text aligned to framed item"); ImGui::SameLine();
			ImGui::Button("Item##1"); ImGui::SameLine();
			ImGui::Text("Item"); ImGui::SameLine();
			ImGui::SmallButton("Item##2"); ImGui::SameLine();
			ImGui::Button("Item##3");

			ImGui::Unindent();
		}

		ImGui::Spacing();

		{
			ImGui::BulletText("Multi-line text:");
			ImGui::Indent();
			ImGui::Text("One\nTwo\nThree"); ImGui::SameLine();
			ImGui::Text("Hello\nWorld"); ImGui::SameLine();
			ImGui::Text("Banana");

			ImGui::Text("Banana"); ImGui::SameLine();
			ImGui::Text("Hello\nWorld"); ImGui::SameLine();
			ImGui::Text("One\nTwo\nThree");

			ImGui::Button("HOP##1"); ImGui::SameLine();
			ImGui::Text("Banana"); ImGui::SameLine();
			ImGui::Text("Hello\nWorld"); ImGui::SameLine();
			ImGui::Text("Banana");

			ImGui::Button("HOP##2"); ImGui::SameLine();
			ImGui::Text("Hello\nWorld"); ImGui::SameLine();
			ImGui::Text("Banana");
			ImGui::Unindent();
		}

		ImGui::Spacing();

		{
			ImGui::BulletText("Misc items:");
			ImGui::Indent();

			// SmallButton() sets FramePadding to zero. Text baseline is aligned to match baseline of previous Button.
			ImGui::Button("80x80", ImVec2(80, 80));
			ImGui::SameLine();
			ImGui::Button("50x50", ImVec2(50, 50));
			ImGui::SameLine();
			ImGui::Button("Button()");
			ImGui::SameLine();
			ImGui::SmallButton("SmallButton()");

			// Tree
			const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
			ImGui::Button("Button##1");
			ImGui::SameLine(0.0f, spacing);
			if (ImGui::TreeNode("Node##1")) {
				// Placeholder tree data
				for (int i = 0; i < 6; i++)
					ImGui::BulletText("Item %d..", i);
				ImGui::TreePop();
			}

			// Vertically align text node a bit lower so it'll be vertically centered with upcoming widget.
			// Otherwise you can use SmallButton() (smaller fit).
			ImGui::AlignTextToFramePadding();

			// Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add
			// other contents below the node.
			bool node_open = ImGui::TreeNode("Node##2");
			ImGui::SameLine(0.0f, spacing); ImGui::Button("Button##2");
			if (node_open) {
				// Placeholder tree data
				for (int i = 0; i < 6; i++)
					ImGui::BulletText("Item %d..", i);
				ImGui::TreePop();
			}

			// Bullet
			ImGui::Button("Button##3");
			ImGui::SameLine(0.0f, spacing);
			ImGui::BulletText("Bullet text");

			ImGui::AlignTextToFramePadding();
			ImGui::BulletText("Node");
			ImGui::SameLine(0.0f, spacing); ImGui::Button("Button##4");
			ImGui::Unindent();
		}
	}
	ImGui::EndChild();
}
void LayoutScrolling::DisplayContents() {
	ImGui::BeginChild("LayoutScrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	{
		// Vertical scroll functions
		HelpMarker("Use SetScrollHereY() or SetScrollFromPosY() to scroll to a given vertical position.");

		static int track_item = 50;
		static bool enable_track = true;
		static bool enable_extra_decorations = false;
		static float scroll_to_off_px = 0.0f;
		static float scroll_to_pos_px = 200.0f;

		ImGui::Checkbox("Decoration", &enable_extra_decorations);

		ImGui::Checkbox("Track", &enable_track);
		ImGui::PushItemWidth(100);
		ImGui::SameLine(140); enable_track |= ImGui::DragInt("##item", &track_item, 0.25f, 0, 99, "Item = %d");

		bool scroll_to_off = ImGui::Button("Scroll Offset");
		ImGui::SameLine(140); scroll_to_off |= ImGui::DragFloat("##off", &scroll_to_off_px, 1.00f, 0, FLT_MAX, "+%.0f px");

		bool scroll_to_pos = ImGui::Button("Scroll To Pos");
		ImGui::SameLine(140); scroll_to_pos |= ImGui::DragFloat("##pos", &scroll_to_pos_px, 1.00f, -10, FLT_MAX, "X/Y = %.0f px");
		ImGui::PopItemWidth();

		if (scroll_to_off || scroll_to_pos)
			enable_track = false;

		ImGuiStyle& style = ImGui::GetStyle();
		float child_w = (ImGui::GetContentRegionAvail().x - 4 * style.ItemSpacing.x) / 5;
		if (child_w < 1.0f)
			child_w = 1.0f;
		ImGui::PushID("##VerticalScrolling");
		for (int i = 0; i < 5; i++)
		{
			if (i > 0) ImGui::SameLine();
			ImGui::BeginGroup();
			const char* names[] = { "Top", "25%", "Center", "75%", "Bottom" };
			ImGui::TextUnformatted(names[i]);

			const ImGuiWindowFlags child_flags = enable_extra_decorations ? ImGuiWindowFlags_MenuBar : 0;
			const ImGuiID child_id = ImGui::GetID((void*)(intptr_t)i);
			const bool child_is_visible = ImGui::BeginChild(child_id, ImVec2(child_w, 200.0f), true, child_flags);
			if (ImGui::BeginMenuBar())
			{
				ImGui::TextUnformatted("abc");
				ImGui::EndMenuBar();
			}
			if (scroll_to_off)
				ImGui::SetScrollY(scroll_to_off_px);
			if (scroll_to_pos)
				ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_pos_px, i * 0.25f);
			if (child_is_visible) // Avoid calling SetScrollHereY when running with culled items
			{
				for (int item = 0; item < 100; item++)
				{
					if (enable_track && item == track_item)
					{
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
						ImGui::SetScrollHereY(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
					}
					else
					{
						ImGui::Text("Item %d", item);
					}
				}
			}
			float scroll_y = ImGui::GetScrollY();
			float scroll_max_y = ImGui::GetScrollMaxY();
			ImGui::EndChild();
			ImGui::Text("%.0f/%.0f", scroll_y, scroll_max_y);
			ImGui::EndGroup();
		}
		ImGui::PopID();

		// Horizontal scroll functions
		ImGui::Spacing();
		HelpMarker(
			"Use SetScrollHereX() or SetScrollFromPosX() to scroll to a given horizontal position.\n\n"
			"Because the clipping rectangle of most window hides half worth of WindowPadding on the "
			"left/right, using SetScrollFromPosX(+1) will usually result in clipped text whereas the "
			"equivalent SetScrollFromPosY(+1) wouldn't.");
		ImGui::PushID("##HorizontalScrolling");
		for (int i = 0; i < 5; i++)
		{
			float child_height = ImGui::GetTextLineHeight() + style.ScrollbarSize + style.WindowPadding.y * 2.0f;
			ImGuiWindowFlags child_flags = ImGuiWindowFlags_HorizontalScrollbar | (enable_extra_decorations ? ImGuiWindowFlags_AlwaysVerticalScrollbar : 0);
			ImGuiID child_id = ImGui::GetID((void*)(intptr_t)i);
			bool child_is_visible = ImGui::BeginChild(child_id, ImVec2(-100, child_height), true, child_flags);
			if (scroll_to_off)
				ImGui::SetScrollX(scroll_to_off_px);
			if (scroll_to_pos)
				ImGui::SetScrollFromPosX(ImGui::GetCursorStartPos().x + scroll_to_pos_px, i * 0.25f);
			if (child_is_visible) // Avoid calling SetScrollHereY when running with culled items
			{
				for (int item = 0; item < 100; item++)
				{
					if (item > 0)
						ImGui::SameLine();
					if (enable_track && item == track_item)
					{
						ImGui::TextColored(ImVec4(1, 1, 0, 1), "Item %d", item);
						ImGui::SetScrollHereX(i * 0.25f); // 0.0f:left, 0.5f:center, 1.0f:right
					}
					else
					{
						ImGui::Text("Item %d", item);
					}
				}
			}
			float scroll_x = ImGui::GetScrollX();
			float scroll_max_x = ImGui::GetScrollMaxX();
			ImGui::EndChild();
			ImGui::SameLine();
			const char* names[] = { "Left", "25%", "Center", "75%", "Right" };
			ImGui::Text("%s\n%.0f/%.0f", names[i], scroll_x, scroll_max_x);
			ImGui::Spacing();
		}
		ImGui::PopID();

		// Miscellaneous Horizontal Scrolling Demo
		HelpMarker(
			"Horizontal scrolling for a window is enabled via the ImGuiWindowFlags_HorizontalScrollbar flag.\n\n"
			"You may want to also explicitly specify content width by using SetNextWindowContentWidth() before Begin().");
		static int lines = 7;
		ImGui::SliderInt("Lines", &lines, 1, 15);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
		ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30);
		ImGui::BeginChild("scrolling", scrolling_child_size, true, ImGuiWindowFlags_HorizontalScrollbar);
		for (int line = 0; line < lines; line++)
		{
			// Display random stuff. For the sake of this trivial demo we are using basic Button() + SameLine()
			// If you want to create your own time line for a real application you may be better off manipulating
			// the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets
			// yourself. You may also want to use the lower-level ImDrawList API.
			int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
			for (int n = 0; n < num_buttons; n++)
			{
				if (n > 0) ImGui::SameLine();
				ImGui::PushID(n + line * 1000);
				char num_buf[16];
				sprintf(num_buf, "%d", n);
				const char* label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" : num_buf;
				float hue = n * 0.05f;
				ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f));
				ImGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
				ImGui::PopStyleColor(3);
				ImGui::PopID();
			}
		}
		float scroll_x = ImGui::GetScrollX();
		float scroll_max_x = ImGui::GetScrollMaxX();
		ImGui::EndChild();
		ImGui::PopStyleVar(2);
		float scroll_x_delta = 0.0f;
		ImGui::SmallButton("<<");
		if (ImGui::IsItemActive())
			scroll_x_delta = -ImGui::GetIO().DeltaTime * 1000.0f;
		ImGui::SameLine();
		ImGui::Text("Scroll from code"); ImGui::SameLine();
		ImGui::SmallButton(">>");
		if (ImGui::IsItemActive())
			scroll_x_delta = +ImGui::GetIO().DeltaTime * 1000.0f;
		ImGui::SameLine();
		ImGui::Text("%.0f/%.0f", scroll_x, scroll_max_x);
		if (scroll_x_delta != 0.0f)
		{
			// Demonstrate a trick: you can use Begin to set yourself in the context of another window
			// (here we are already out of your child window)
			ImGui::BeginChild("scrolling");
			ImGui::SetScrollX(ImGui::GetScrollX() + scroll_x_delta);
			ImGui::EndChild();
		}
		ImGui::Spacing();

		static bool show_horizontal_contents_size_demo_window = false;
		ImGui::Checkbox("Show Horizontal contents size demo window", &show_horizontal_contents_size_demo_window);

		if (show_horizontal_contents_size_demo_window)
		{
			static bool show_h_scrollbar = true;
			static bool show_button = true;
			static bool show_tree_nodes = true;
			static bool show_text_wrapped = false;
			static bool show_columns = true;
			static bool show_tab_bar = true;
			static bool show_child = false;
			static bool explicit_content_size = false;
			static float contents_size_x = 300.0f;
			if (explicit_content_size)
				ImGui::SetNextWindowContentSize(ImVec2(contents_size_x, 0.0f));
			ImGui::Begin("Horizontal contents size demo window", &show_horizontal_contents_size_demo_window, show_h_scrollbar ? ImGuiWindowFlags_HorizontalScrollbar : 0);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 0));
			HelpMarker("Test of different widgets react and impact the work rectangle growing when horizontal scrolling is enabled.\n\nUse 'Metrics->Tools->Show windows rectangles' to visualize rectangles.");
			ImGui::Checkbox("H-scrollbar", &show_h_scrollbar);
			ImGui::Checkbox("Button", &show_button);            // Will grow contents size (unless explicitly overwritten)
			ImGui::Checkbox("Tree nodes", &show_tree_nodes);    // Will grow contents size and display highlight over full width
			ImGui::Checkbox("Text wrapped", &show_text_wrapped);// Will grow and use contents size
			ImGui::Checkbox("Columns", &show_columns);          // Will use contents size
			ImGui::Checkbox("Tab bar", &show_tab_bar);          // Will use contents size
			ImGui::Checkbox("Child", &show_child);              // Will grow and use contents size
			ImGui::Checkbox("Explicit content size", &explicit_content_size);
			ImGui::Text("Scroll %.1f/%.1f %.1f/%.1f", ImGui::GetScrollX(), ImGui::GetScrollMaxX(), ImGui::GetScrollY(), ImGui::GetScrollMaxY());
			if (explicit_content_size)
			{
				ImGui::SameLine();
				ImGui::SetNextItemWidth(100);
				ImGui::DragFloat("##csx", &contents_size_x);
				ImVec2 p = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + 10, p.y + 10), IM_COL32_WHITE);
				ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x + contents_size_x - 10, p.y), ImVec2(p.x + contents_size_x, p.y + 10), IM_COL32_WHITE);
				ImGui::Dummy(ImVec2(0, 10));
			}
			ImGui::PopStyleVar(2);
			ImGui::Separator();
			if (show_button)
			{
				ImGui::Button("this is a 300-wide button", ImVec2(300, 0));
			}
			if (show_tree_nodes)
			{
				bool open = true;
				if (ImGui::TreeNode("this is a tree node"))
				{
					if (ImGui::TreeNode("another one of those tree node..."))
					{
						ImGui::Text("Some tree contents");
						ImGui::TreePop();
					}
					ImGui::TreePop();
				}
				ImGui::CollapsingHeader("CollapsingHeader", &open);
			}
			if (show_text_wrapped)
			{
				ImGui::TextWrapped("This text should automatically wrap on the edge of the work rectangle.");
			}
			if (show_columns)
			{
				ImGui::Text("Tables:");
				if (ImGui::BeginTable("table", 4, ImGuiTableFlags_Borders))
				{
					for (int n = 0; n < 4; n++)
					{
						ImGui::TableNextColumn();
						ImGui::Text("Width %.2f", ImGui::GetContentRegionAvail().x);
					}
					ImGui::EndTable();
				}
				ImGui::Text("Columns:");
				ImGui::Columns(4);
				for (int n = 0; n < 4; n++)
				{
					ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
					ImGui::NextColumn();
				}
				ImGui::Columns(1);
			}
			if (show_tab_bar && ImGui::BeginTabBar("Hello"))
			{
				if (ImGui::BeginTabItem("OneOneOne")) { ImGui::EndTabItem(); }
				if (ImGui::BeginTabItem("TwoTwoTwo")) { ImGui::EndTabItem(); }
				if (ImGui::BeginTabItem("ThreeThreeThree")) { ImGui::EndTabItem(); }
				if (ImGui::BeginTabItem("FourFourFour")) { ImGui::EndTabItem(); }
				ImGui::EndTabBar();
			}
			if (show_child)
			{
				ImGui::BeginChild("child", ImVec2(0, 0), true);
				ImGui::EndChild();
			}
			ImGui::End();
		}
	}
	ImGui::EndChild();
}
void LayoutClipping::DisplayContents() {
	ImGui::BeginChild("LayoutClipping");
	{
		static ImVec2 size(100.0f, 100.0f);
		static ImVec2 offset(30.0f, 30.0f);
		ImGui::DragFloat2("size", (float*)&size, 0.5f, 1.0f, 200.0f, "%.0f");
		ImGui::TextWrapped("(Click and drag to scroll)");

		HelpMarker(
			"(Left) Using ImGui::PushClipRect():\n"
			"Will alter ImGui hit-testing logic + ImDrawList rendering.\n"
			"(use this if you want your clipping rectangle to affect interactions)\n\n"
			"(Center) Using ImDrawList::PushClipRect():\n"
			"Will alter ImDrawList rendering only.\n"
			"(use this as a shortcut if you are only using ImDrawList calls)\n\n"
			"(Right) Using ImDrawList::AddText() with a fine ClipRect:\n"
			"Will alter only this specific ImDrawList::AddText() rendering.\n"
			"This is often used internally to avoid altering the clipping rectangle and minimize draw calls.");

		for (int n = 0; n < 3; n++)
		{
			if (n > 0)
				ImGui::SameLine();

			ImGui::PushID(n);
			ImGui::InvisibleButton("##canvas", size);
			if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
			{
				offset.x += ImGui::GetIO().MouseDelta.x;
				offset.y += ImGui::GetIO().MouseDelta.y;
			}
			ImGui::PopID();
			if (!ImGui::IsItemVisible()) // Skip rendering as ImDrawList elements are not clipped.
				continue;

			const ImVec2 p0 = ImGui::GetItemRectMin();
			const ImVec2 p1 = ImGui::GetItemRectMax();
			const char* text_str = "Line 1 hello\nLine 2 clip me!";
			const ImVec2 text_pos = ImVec2(p0.x + offset.x, p0.y + offset.y);
			ImDrawList* draw_list = ImGui::GetWindowDrawList();
			switch (n)
			{
			case 0:
				ImGui::PushClipRect(p0, p1, true);
				draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
				draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
				ImGui::PopClipRect();
				break;
			case 1:
				draw_list->PushClipRect(p0, p1, true);
				draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
				draw_list->AddText(text_pos, IM_COL32_WHITE, text_str);
				draw_list->PopClipRect();
				break;
			case 2:
				ImVec4 clip_rect(p0.x, p0.y, p1.x, p1.y); // AddText() takes a ImVec4* here so let's convert.
				draw_list->AddRectFilled(p0, p1, IM_COL32(90, 90, 120, 255));
				draw_list->AddText(ImGui::GetFont(), ImGui::GetFontSize(), text_pos, IM_COL32_WHITE, text_str, NULL, 0.0f, &clip_rect);
				break;
			}
		}
	}
	ImGui::EndChild();
}

void PopupMain::DisplayContents() {
	ImGui::BeginChild("PopupMain");
	{
		ImGui::TextWrapped(
			"When a popup is active, it inhibits interacting with windows that are behind the popup. "
			"Clicking outside the popup closes it.");
		static int selectedFish = -1;
		const char* names[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
		static bool toggles[] = { true, false, false, false, false };

		if (ImGui::Button("Select..")) ImGui::OpenPopup("my_select_popup");
		ImGui::SameLine();
		ImGui::TextUnformatted(selectedFish == -1 ? "<None>" : names[selectedFish]);
		if (ImGui::BeginPopup("my_select_popup")) {
			ImGui::Text("Aquarium");
			ImGui::Separator();
			for (int i = 0; i < IM_ARRAYSIZE(names); i++) {
				if (ImGui::Selectable(names[i])) {
					selectedFish = i;
				}
			}
			ImGui::EndPopup();
		}
		if (ImGui::Button("Toggle..")) {
			ImGui::OpenPopup("my_toggle_popup");
		}
		if (ImGui::BeginPopup("my_toggle_popup")) {
			for (int i = 0; i < IM_ARRAYSIZE(names); i++) {
				ImGui::MenuItem(names[i], "", &toggles[i]);
			}
			if (ImGui::BeginMenu("Sub-menu")) {
				ImGui::MenuItem("Click me");
				ImGui::EndMenu();
			}
			ImGui::Separator();
			ImGui::Text("Tooltip here");
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip("I am a tooltip over a popup");
			}
			if (ImGui::Button("Stacked Popup")) {
				ImGui::OpenPopup("another popup");
			}
			if (ImGui::BeginPopup("another popup")) {
				for (int i = 0; i < IM_ARRAYSIZE(names); i++) {
					ImGui::MenuItem(names[i], "", &toggles[i]);
				}
				if (ImGui::BeginMenu("Sub-menu")) {
					ImGui::MenuItem("Click me");
					if (ImGui::Button("Stacked Popup")) {
						ImGui::OpenPopup("another popup");
					}
					if (ImGui::BeginPopup("another popup")) {
						ImGui::Text("I am the last one here.");
						ImGui::EndPopup();
					}
					ImGui::EndMenu();
				}
				ImGui::EndPopup();
			}
			ImGui::EndPopup();
		}

		if (ImGui::Button("With a menu..")) {
			ImGui::OpenPopup("my_file_popup");
		}
		if (ImGui::BeginPopup("my_file_popup", ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit")) {
					ImGui::MenuItem("Dummy");
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Text("Hello from popup!");
			ImGui::Button("This is a dummy button..");
			ImGui::EndPopup();
		}
	}
	ImGui::EndChild();
}
void PopupContextMenu::DisplayContents() {
	ImGui::BeginChild("PopupContextMenu");
	{
		HelpMarker("\"Context\" functions are simple helpers to associate a Popup to a given Item or Window identifier.");
		{
			const char* names[5] = { "Label1", "Label2", "Label3", "Label4", "Label5" };
			for (int n = 0; n < 5; n++) {
				ImGui::Selectable(names[n]);
				if (ImGui::BeginPopupContextItem()) {
					ImGui::Text("This a popup for \"%s\"!", names[n]);
					if (ImGui::Button("Close")) {
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Right-click to open popup");
				}
			}
		}
		{
			HelpMarker("Text() elements don't have stable identifiers so we need to provide one.");
			static float value = 0.5f;
			ImGui::Text("Value = %.3f <-- (1) right-click this text", value);
			if (ImGui::BeginPopupContextItem("my popup")) {
				if (ImGui::Selectable("Set to zero")) value = 0.0f;
				if (ImGui::Selectable("Set to PI")) value = 3.1415f;
				ImGui::SetNextItemWidth(-FLT_MIN);
				ImGui::DragFloat("##Value", &value, 0.1f, 0.0f, 0.0f);
				ImGui::EndPopup();
			}
			ImGui::Text("(2) Or right-click this text");
			ImGui::OpenPopupOnItemClick("my popup", ImGuiPopupFlags_MouseButtonRight);
			if (ImGui::Button("(3) Or click this button")) {
				ImGui::OpenPopup("my popup");
			}
		}
		{
			HelpMarker("Showcase using a popup ID linked to item ID, with the item having a changing label + stable ID using the ### operator.");
			static char name[32] = "Label1";
			char buf[64];
			sprintf(buf, "Button: %s###Button", name); // ### operator override ID ignoring the preceding label
			ImGui::Button(buf);
			if (ImGui::BeginPopupContextItem()) {
				ImGui::Text("Edit name:");
				ImGui::InputText("##edit", name, IM_ARRAYSIZE(name));
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::SameLine(); ImGui::Text("(<-- right-click here)");
		}
	}
	ImGui::EndChild();
}
void PopupModals::DisplayContents() {
	ImGui::BeginChild("PopupModals");
	{
		ImGui::TextWrapped("Modal windows are like popups but the user cannot close them by clicking outside.");
		if (ImGui::Button("Delete..")) {
			ImGui::OpenPopup("Delete?");
		}
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		
		if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::Text("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			static bool dont_ask_me_next_time = false;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
			ImGui::PopStyleVar();
			if (ImGui::Button("OK", ImVec2(120, 0))) { 
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		if (ImGui::Button("Stacked modals..")) {
			ImGui::OpenPopup("Stacked 1");
		}
		if (ImGui::BeginPopupModal("Stacked 1", NULL, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Some menu item")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			ImGui::Text("Hello from Stacked The First\nUsing style.Colors[ImGuiCol_ModalWindowDimBg] behind it.");

			static int item = 1;
			static float color[4] = { 0.4f, 0.7f, 0.0f, 0.5f };
			ImGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
			ImGui::ColorEdit4("color", color);
			if (ImGui::Button("Add another modal..")) {
				ImGui::OpenPopup("Stacked 2");
			}

			bool unused_open = true;
			if (ImGui::BeginPopupModal("Stacked 2", &unused_open)) {
				ImGui::Text("Hello from Stacked The Second!");
				if (ImGui::Button("Close")) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			if (ImGui::Button("Close")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}
	ImGui::EndChild();
}
void PopupMenus::DisplayContents() {
	ImGui::BeginChild("PopupMenus");
	{
		ImGui::TextWrapped("Below we are testing adding menu items to a regular window. It's rather unusual but should work!");
		ImGui::Separator();
		ImGui::MenuItem("Menu item", "CTRL+M");
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Some menu item")) {}
			if (ImGui::MenuItem("Another menu item")) {}
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Some menu item")) {}
				if (ImGui::MenuItem("Another menu item")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::Separator();
	}
	ImGui::EndChild();
}