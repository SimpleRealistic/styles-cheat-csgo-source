
#include "GUI.h"

#include "MiscHacks.h"
#include "RenderManager.h"
#include "MetaInfo.h"
#include "Menu.h"
#include <algorithm>
#include "tinyxml2.h"
#include "Controls.h"
#include "PlayerList.h"
#include "Globals.h"


CGUI GUI;

CGUI::CGUI()
{

}

// Draws all windows 
void CGUI::Draw()
{
	bool ShouldDrawCursor = false;

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			ShouldDrawCursor = true;
			if (window == Windows[1])
			{
				DrawWindow(window, 2);
			}
			else
			{
				DrawWindow(window, 1);
			}

		}
	}

	if (ShouldDrawCursor)
		I::Engine->ClientCmd_Unrestricted(XorStr("cursortimeout 0"));
	else
		I::Engine->ClientCmd_Unrestricted(XorStr("cursortimeout 60"));

	if (ShouldDrawCursor)
	{


		Draw::Clear(Mouse.x + 1, Mouse.y, 1, 17, Color(66, 136, 247, 255));
		for (int i = 0; i < 11; i++)
			Draw::Clear(Mouse.x + 2 + i, Mouse.y + 1 + i, 1, 1, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 8, Mouse.y + 12, 5, 1, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 8, Mouse.y + 13, 1, 1, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 9, Mouse.y + 14, 1, 2, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 10, Mouse.y + 16, 1, 2, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 8, Mouse.y + 18, 2, 1, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 7, Mouse.y + 16, 1, 2, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 6, Mouse.y + 14, 1, 2, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 5, Mouse.y + 13, 1, 1, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 4, Mouse.y + 14, 1, 1, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 3, Mouse.y + 15, 1, 1, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 2, Mouse.y + 16, 1, 1, Color(66, 136, 247, 255));
		for (int i = 0; i < 4; i++)
			Draw::Clear(Mouse.x + 2 + i, Mouse.y + 2 + i, 1, 14 - (i * 2), Color(66 - (i * 4), 136 - (i * 4), 247 - (i * 4), 255));
		Draw::Clear(Mouse.x + 6, Mouse.y + 6, 1, 8, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 7, Mouse.y + 7, 1, 9, Color(66, 136, 247, 255));
		for (int i = 0; i < 4; i++)
			Draw::Clear(Mouse.x + 8 + i, Mouse.y + 8 + i, 1, 4 - i, Color(66 - (i * 4), 136 - (i * 4), 247 - (i * 4), 255));
		Draw::Clear(Mouse.x + 8, Mouse.y + 14, 1, 4, Color(66, 136, 247, 255));
		Draw::Clear(Mouse.x + 9, Mouse.y + 16, 1, 2, Color(66, 136, 247, 255));
	}
}

// Handle all input etc
void CGUI::Update()
{

	static int bWasntHolding = false;
	static int bGrabbing = false;
	static int iOffsetX = 0, iOffsetY = 0;
	//Key Array
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	POINT mp; GetCursorPos(&mp);
	ScreenToClient(GetForegroundWindow(), &mp);
	Mouse.x = mp.x; Mouse.y = mp.y;

	RECT Screen = Draw::GetViewport();

	// Window Binds
	for (auto& bind : WindowBinds)
	{
		if (GetKeyPress(bind.first))
		{
			bind.second->Toggle();
		}
	}

	// Stop dragging
	if (IsDraggingWindow && !GetKeyState(VK_LBUTTON))
	{
		IsDraggingWindow = false;
		DraggingWindow = nullptr;

	}

	// If we are in the proccess of dragging a window
	if (IsDraggingWindow && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}

	}


	//bWasntHolding = Input->Hovering(x, y, width, 28) && !GetAsyncKeyState(VK_LBUTTON);

	// Process some windows
	for (auto window : Windows)
	{

		if (window->m_bIsOpen)
		{
			// Used to tell the widget processing that there could be a click
			bool bCheckWidgetClicks = false;

			// If the user clicks inside the window
			if (GetKeyPress(VK_LBUTTON) || GetKeyPress(VK_RETURN))
			{
				//if (IsMouseInRegion(window->m_x, window->m_y, window->m_x + window->m_iWidth, window->m_y + window->m_iHeight))
				//{
				// Is it inside the client area?
				if (IsMouseInRegion(window->GetClientArea1()))
				{
					// User is selecting a new tab
					if (IsMouseInRegion(window->GetTabArea()))
					{
						// Loose focus on the control
						window->IsFocusingControl = false;
						window->FocusedControl = nullptr;

						int iTab = 0;
						int TabCount = window->Tabs.size();
						if (TabCount) // If there are some tabs
						{
							int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
							int Dist = Mouse.x - (window->m_x + 8);
							while (Dist > TabSize)
							{
								if (Dist > TabSize)
								{
									iTab++;
									Dist -= TabSize;
								}
							}
							window->SelectedTab = window->Tabs[iTab];

							// Loose focus on the control
							bCheckWidgetClicks = false;
							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
						}

					}
					else
						bCheckWidgetClicks = true;
				}
				if (IsMouseInRegion(window->GetDragArea()))
				{
					// Must be in the around the title or side of the window
					// So we assume the user is trying to drag the window
					IsDraggingWindow = true;

					DraggingWindow = window;

					DragOffsetX = Mouse.x - window->m_x;
					DragOffsetY = Mouse.y - window->m_y;

					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}

				//else
				//{
				//    // Loose focus on the control
				//    window->IsFocusingControl = false;
				//    window->FocusedControl = nullptr;
				//}
			}


			// Controls
			if (window->SelectedTab != nullptr)
			{
				// Focused widget
				bool SkipWidget = false;
				CControl* SkipMe = nullptr;

				// this window is focusing on a widget??
				if (window->IsFocusingControl)
				{
					if (window->FocusedControl != nullptr)
					{
						CControl* control = window->FocusedControl;
						CGroupBox* group;
						if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

						if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
						{
							if ((group->group_tabs.size() > 0 && control->g_tab == group->selected_tab) || group->group_tabs.size() == 0)
							{
								// We've processed it once, skip it later
								SkipWidget = true;
								SkipMe = window->FocusedControl;

								POINT cAbs = window->FocusedControl->GetAbsolutePos();
								RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
								window->FocusedControl->OnUpdate();

								if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
								{
									window->FocusedControl->OnClick();

									bCheckWidgetClicks = false;
								}
							}
						}
						else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
						{
							// We've processed it once, skip it later
							SkipWidget = true;
							SkipMe = window->FocusedControl;

							POINT cAbs = window->FocusedControl->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
							window->FocusedControl->OnUpdate();

							if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								window->FocusedControl->OnClick();

								// If it gets clicked we loose focus
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
								bCheckWidgetClicks = false;
							}
						}
					}
				}

				// Itterate over the rest of the control
				for (auto control : window->SelectedTab->Controls)
				{
					if (control != nullptr)
					{
						CGroupBox* group;
						if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

						if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
						{
							if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
							{
								if (SkipWidget && SkipMe == control)
									continue;

								POINT cAbs = control->GetAbsolutePos();
								RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
								control->OnUpdate();

								if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
								{
									control->OnClick();
									bCheckWidgetClicks = false;

									// Change of focus
									if (control->Flag(UIFlags::UI_Focusable))
									{
										window->IsFocusingControl = true;
										window->FocusedControl = control;
									}
									else
									{
										window->IsFocusingControl = false;
										window->FocusedControl = nullptr;
									}
								}
							}
						}
						else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
						{
							if (SkipWidget && SkipMe == control)
								continue;

							POINT cAbs = control->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
							control->OnUpdate();

							if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
							{
								control->OnClick();
								bCheckWidgetClicks = false;

								// Change of focus
								if (control->Flag(UIFlags::UI_Focusable))
								{
									window->IsFocusingControl = true;
									window->FocusedControl = control;
								}
								else
								{
									window->IsFocusingControl = false;
									window->FocusedControl = nullptr;
								}
							}
						}
					}
				}

				// We must have clicked whitespace
				if (bCheckWidgetClicks)
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
		}
	}
}

// Returns 
bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int x, int y, int x2, int y2)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.left, region.top, region.left + region.right, region.top + region.bottom);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}

bool CGUI::DrawWindow(CWindow* window, int menu)
{
	// Main Window
	//Draw::Outline(window->m_x, window->m_y, window->m_iWidth, window->m_iHeight, Color(red, green, blue, 80));
	
	//Draw::Clear(window->m_x + 2, window->m_y + 2 + 26, window->m_iWidth - 4, window->m_iHeight - 4 - 26, Color(red, green, blue, 255));
	//Draw::Outline(window->m_x + 1, window->m_y + 1, window->m_iWidth - 2, window->m_iHeight - 2, Color(red, green, blue, 255));
	//Draw::Text(window->m_x + 8, window->m_y + 8, Color(255, 255, 255, 255), Draw::Fonts::MenuBold, window->Title.c_str());

	if (menu == 1)
	{

		//Inner coon
		Draw::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, window->m_iHeight - 2 - 8 - 26, Color(25, 25, 25, 255));
		Draw::Outline(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, window->m_iHeight - 2 - 8 - 26, Color(0, 0, 0, 255));

		// Border Top

		//Draw::Clear(window->m_x + 2, window->m_y + 1 + 15 * 2 - 6, window->m_iWidth - 4 - 12 + 7, 6, Color(57, 57, 57, 255));
		//Draw::Outline(window->m_x + 2, window->m_y + 1 + 15 * 2 - 6, window->m_iWidth - 4 - 12 + 7, 1, Color(100, 100, 100, 255));
	//	Draw::Outline(window->m_x + 2, window->m_y + 1 + 17 * 2 - 6, window->m_iWidth - 4 - 12 + 7, 1, Color(100, 100, 100, 255));

		// Border Bottom

		//Draw::Clear(window->m_x + 2, window->m_y + 1 + 15 * 2 + window->m_iHeight - 2 - 8 - 35, window->m_iWidth - 4 - 12 + 9, 6, Color(57, 57, 57, 255));
		//Draw::Outline(window->m_x + 2, window->m_y + 1 + 15 * 2 + window->m_iHeight - 2 - 8 - 35, window->m_iWidth - 12 - 12 + 9, 1, Color(100, 100, 100, 255));
		//Draw::Outline(window->m_x + 2, window->m_y + 1 + 17 * 2 + window->m_iHeight - 2 - 8 - 35, window->m_iWidth - 6 - 13 + 9, 1, Color(100, 100, 100, 255));

		// Border Left

		//Draw::Clear(window->m_x + 2, window->m_y + 1 + 16 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 39 + 12, Color(57, 57, 57, 255));
		//Draw::Outline(window->m_x + 2, window->m_y + 1 + 15 * 2 - 6, 1, window->m_iHeight - 2 - 8 - 37 + 12, Color(100, 100, 100, 255));
		//Draw::Outline(window->m_x + 7, window->m_y + 1 + 17 * 2 - 6, 1, window->m_iHeight - 2 - 8 - 44 + 12, Color(100, 100, 100, 255));

		// Border Right

		//Draw::Clear(window->m_x + 4 + window->m_iWidth - 4 - 12, window->m_y + 1 + 17 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 41 + 12, Color(57, 57, 57, 255));
		//Draw::Outline(window->m_x + 4 + window->m_iWidth - 4 - 12, window->m_y + 1 + 17 * 2 - 6, 1, window->m_iHeight - 2 - 8 - 44 + 12, Color(100, 100, 100, 255));
		//Draw::Outline(window->m_x + 9 + window->m_iWidth - 4 - 12, window->m_y + 1 + 15 * 2 - 6, 1, window->m_iHeight - 2 - 8 - 38 + 14, Color(100, 100, 100, 255));

		// Coon line
		Draw::Clear(window->m_x + 8 + 81 - 6, window->m_y + 20 * 2 - 5, 3, window->m_iHeight - 50, Color(0, 0, 0, 255));
		Draw::Outline(window->m_x + 8 + 81 - 6, window->m_y + 18 * 2 - 5, 3, window->m_iHeight - 46, Color(0, 0, 0, 255));

		Draw::GradientV(window->m_x + 8, window->m_y + 25, window->m_iWidth - 16, 26, Color(25, 25, 25, 255), Color(25, 25, 25, 255));
		Draw::Outline(window->m_x + 8, window->m_y + 25, window->m_iWidth - 16, 26, Color(0, 0, 0, 255));

		Draw::GradientV(window->m_x + 8, window->m_y + 330, window->m_iWidth - 16, 26, Color(25, 25, 25, 255), Color(25, 25, 25, 255));
		Draw::Outline(window->m_x + 8, window->m_y + 330, window->m_iWidth - 16, 26, Color(0, 0, 0, 255));

		Draw::Text(window->m_x + 16, window->m_y + 32 + 1, Color(255, 255, 255, 255), Draw::Fonts::Untitled1, window->Title.c_str());

		Draw::Text(window->m_x + 16, window->m_y + 332 + 1, Color(255, 255, 255, 255), Draw::Fonts::Untitled1, window->Title2.c_str());

		// title
		//Draw::Text(window->m_x + 480, window->m_y + 43, Color(255, 255, 255, 255), Draw::Fonts::MenuBold, "Oneless CSGO");

		// rainbow bar
		//Draw::GradientH(window->m_x + 8, window->m_y + 31, (window->m_iWidth - 27) / 2, 2, Color(0 * (255 / 255.f), 142 * (255 / 255.f), 215 * (255 / 255.f), 255), Color(160 * (255 / 255.f), 0 * (255 / 255.f), 255 * (255 / 255.f), 255));
		//Draw::GradientH(window->m_x + 8 + (window->m_iWidth - 27) / 2, window->m_y + 31, (window->m_iWidth - 14) / 2, 2, Color(160 * (255 / 255.f), 0 * (255 / 255.f), 255 * (255 / 255.f), 255), Color(255 * (255 / 255.f), 0 * (255 / 255.f), 0 * (255 / 255.f), 255));
	

		int tabcount = window->Tabs.size();
		if (tabcount) // If there are some tabs
		{
			bool isOut = true;

			for (int i = 0; i < tabcount; i++)
			{
				CTab *tab = window->Tabs[i];

				float xAxis;
				float yAxis;
				float yWinPos = window->m_y + 1 + 28 * 2;
				float yWinHeight = window->m_iHeight - 2 - 8 - 26;

				yAxis = yWinPos + 16 + (i * 66) - 10;

				RECT TabDrawArea = { window->m_x + 8 + 1 + 1, yWinPos + 1 + (i *27) - 8, 78 - 4, 27 };

				RECT TextSize;
				TextSize = Draw::GetTextSize(Draw::Fonts::Riffic, tab->Title.c_str());

				RECT ClickTabArea = { xAxis,
					yAxis,
					TextSize.right,
					TextSize.bottom };

				if (GetAsyncKeyState(VK_LBUTTON))
				{
					if (IsMouseInRegion(TabDrawArea))
					{
						window->SelectedTab = window->Tabs[i];
					}
				}

				if (window->SelectedTab == tab) {
					xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
					Draw::Clear(TabDrawArea.left, TabDrawArea.top, TabDrawArea.right - 10, TabDrawArea.bottom, Color(46, 46, 46, 255));
					Draw::Clear(TabDrawArea.left + 65, TabDrawArea.top, TabDrawArea.right - 66, TabDrawArea.bottom, Color(0, 130, 255, 255));
					Draw::Line(TabDrawArea.left, TabDrawArea.top, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top, Color(0, 0, 0, 255));
					Draw::Line(TabDrawArea.left, TabDrawArea.top + TabDrawArea.bottom, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top + TabDrawArea.bottom, Color(0, 0, 0, 255));
					Draw::Text(TabDrawArea.left + 2, TabDrawArea.top + 8, Color(255, 255, 255, 255), Draw::Fonts::Untitled1, tab->Title.c_str());
				}
				else {
					xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
					Draw::Clear(TabDrawArea.left, TabDrawArea.top, TabDrawArea.right - 10, TabDrawArea.bottom, Color(122, 122, 122, 255));
					Draw::Clear(TabDrawArea.left + 68, TabDrawArea.top, TabDrawArea.right - 68, TabDrawArea.bottom, Color(0, 130, 255, 0));
					Draw::Text(TabDrawArea.left + 2, TabDrawArea.top + 8, Color(255, 255, 255, 255), Draw::Fonts::Untitled1, tab->Title.c_str());
					Draw::Line(TabDrawArea.left, TabDrawArea.top, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top, Color(0, 0, 0, 255));
					Draw::Line(TabDrawArea.left, TabDrawArea.top + TabDrawArea.bottom, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top + TabDrawArea.bottom, Color(0, 0, 0, 255));
				}

				int width = window->m_iWidth - 4 - 12;

				RECT suicideccSize = Draw::GetTextSize(Draw::Fonts::Riffic, "");
				Draw::Text(window->m_x + 8 + (width / 2) - (suicideccSize.right / 2),
					window->m_y + 1 + 28 * 2 + 13,
					Color(255, 255, 255, 255),
					Draw::Fonts::Riffic,
					"");

				//Draw::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, 2, Color(65, 55, 55, 255));
			}
		}

		//Draw::Outline(window->m_x)
		// Controls 
		if (window->SelectedTab != nullptr)
		{
			// Focused widget
			bool SkipWidget = false;
			CControl* SkipMe = nullptr;

			// this window is focusing on a widget??
			if (window->IsFocusingControl)
			{
				if (window->FocusedControl != nullptr)
				{
					CControl* control = window->FocusedControl;
					CGroupBox* group;
					if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

					if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
					{
						if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
						{
							SkipWidget = true;
							SkipMe = window->FocusedControl;
						}
					}
					else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
					{

						SkipWidget = true;
						SkipMe = window->FocusedControl;
					}
				}
			}

			// Itterate over all the other controls
			for (auto control : window->SelectedTab->Controls)
			{
				CGroupBox* group;
				if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

				if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
				{
					if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
					{
						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						bool hover = false;
						if (IsMouseInRegion(controlRect))
						{
							hover = true;
						}
						control->Draw(hover);
					}
				}
				else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
				{

					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}

			// Draw the skipped widget last
			if (SkipWidget)
			{
				auto control = window->FocusedControl;

				if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
				{
					CControl* control = window->FocusedControl;
					CGroupBox* group;
					if (control->FileControlType != UIControlTypes::UIC_GroupBox) group = control->parent_group ? (CGroupBox*)control->parent_group : nullptr;

					if (group != nullptr && control->FileControlType != UIControlTypes::UIC_GroupBox)
					{
						if (group->group_tabs.size() > 0 && control->g_tab == group->selected_tab || group->group_tabs.size() == 0)
						{
							POINT cAbs = control->GetAbsolutePos();
							RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
							bool hover = false;
							if (IsMouseInRegion(controlRect))
							{
								hover = true;
							}
							control->Draw(hover);
						}
					}
					else if (control->FileControlType == UIControlTypes::UIC_GroupBox || control->FileControlType != UIControlTypes::UIC_GroupBox && !control->parent_group)
					{

						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						bool hover = false;
						if (IsMouseInRegion(controlRect))
						{
							hover = true;
						}
						control->Draw(hover);
					}
				}
			}
		}


		return true;
	}
}

void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);

	// Resorting to put groupboxes at the start
	for (auto tab : window->Tabs)
	{
		for (auto control : tab->Controls)
		{
			if (control->Flag(UIFlags::UI_RenderFirst))
			{
				CControl * c = control;
				tab->Controls.erase(std::remove(tab->Controls.begin(), tab->Controls.end(), control), tab->Controls.end());
				tab->Controls.insert(tab->Controls.begin(), control);
			}
		}
	}
}

void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}

void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	// Create a whole new document and we'll just save over top of the old one
	tinyxml2::XMLDocument Doc;

	// Root Element is called "ayy"
	tinyxml2::XMLElement *Root = Doc.NewElement("BlackDeath.cc");
	Doc.LinkEndChild(Root);

	U::Log("Saving Window %s", window->Title.c_str());

	// If the window has some tabs..
	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			// Add a new section for this tab
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);

			U::Log("Saving Tab %s", Tab->Title.c_str());

			// Now we itterate the controls this tab contains
			if (TabElement && Tab->Controls.size() > 0)
			{
				for (auto Control : Tab->Controls)
				{
					// If the control is ok to be saved
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						// Create an element for the control
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						U::Log("Saving control %s", Control->FileIdentifier.c_str());

						if (!ControlElement)
						{
							U::Log("Error");
							return;
						}

						CCheckBox* cbx = nullptr; 
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;
						CItemSelector* itms = nullptr;
						CMultiBox* mtbx = nullptr;

						// Figure out what kind of control and data this is
						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						case UIControlTypes::UIC_ItemSelector:
							itms = (CItemSelector*)Control;
							ControlElement->SetText(itms->GetInt());
							break;
						case UIControlTypes::UIC_MultiBox:
							mtbx = (CMultiBox*)Control;
							std::string x;
							for (int i = 0; i < mtbx->items.size(); i++)
							{
								std::string status;
								status = mtbx->items[i].bselected ? "1" : "0";
								x = x + status;
							}
							ControlElement->SetText(x.c_str());
							break;
						}
					}
				}
			}
		}
	}

	//Save the file
	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "Failed To Save Config File!", "Black Death Upon you", MB_OK);
	}
	
}

void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	// Lets load our meme
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();
		
		// The root "ayy" element
		if (Root)
		{
			// If the window has some tabs..
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					// We find the corresponding element for this tab
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						// Now we itterate the controls this tab contains
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								// If the control is ok to be saved
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									// Get the controls element
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;
										CItemSelector* itms = nullptr;
										CMultiBox* mtbx = nullptr;

										// Figure out what kind of control and data this is
										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0]=='1'?true:false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_ItemSelector:
											itms = (CItemSelector*)Control;
											itms->SetValue(atof(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_MultiBox:
											mtbx = (CMultiBox*)Control;
											for (int i = 0; i < mtbx->items.size(); i++)
											{
												mtbx->items[i].bselected = ControlElement->GetText()[i] == '1' ? true : false;
											}
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}