
#include "Controls.h"
#include "RenderManager.h"
#include "Menu.h"
#include "CRC32.h"
#include "Globals.h"

#define COL_WHITE			Color(255, 255, 255)
#define UI_COL_MAIN			Color(230, 230, 230, 40)
#define UI_COL_MAINDARK		Color(28,136,0, 255)
#define UI_COL_FADEMAIN		Color(28,136,0, 255)
#define UI_COL_SHADOW		Color(0, 0, 0, 0)
#define UI_COL_CLIENTBACK	Color(238, 238, 238, 255)
#define UI_COL_TABSEPERATOR	Color(229, 229, 229, 255)
#define UI_COL_TABTEXT		Color(145, 145, 145, 255)
#define UI_COL_GROUPOUTLINE Color(222, 222, 222, 255)
#pragma endregion Implementation of the Textfi
#define LIST_ITEM_HEIGHT 16
#define LIST_SCROLL_WIDTH 6

#pragma region Base Control
void CControl::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

void CControl::SetSize(int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
}

void CControl::GetSize(int &w, int &h)
{
	w = m_iWidth;
	h = m_iHeight;
}

bool CControl::Flag(int f)
{
	if (m_Flags & f)
		return true;
	else
		return false;
}

POINT CControl::GetAbsolutePos()
{
	POINT p;
	RECT client = parent->GetClientArea();
	if (parent)
	{
		p.x = m_x + client.left;
		p.y = m_y + client.top + 29;
	}
	
	return p;
}

void CGroupBox::PlaceLabledControlPLayerlist(std::string Label, CTab *Tab, CControl* control, int c, int r)
{
	int CellW = (m_iWidth / Columns);
	int x = m_x + 10 + (c * CellW);
	int y = m_y + 26 + r * 22;

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	Tab->RegisterControl(label);

	x += CellW / 2;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((CellW / 2) - 20, ch);
	Tab->RegisterControl(control);

	if (r > MaxRow)
		MaxRow = r;

	char szCRCBuffer[256];
	sprintf_s(szCRCBuffer, "%s%s", Text.c_str(), Label.c_str());

	m_iHeight = (MaxRow + 1) * 22 + 26;
}

void CControl::SetFileId(std::string fid)
{
	FileIdentifier = fid;
}
#pragma endregion Implementations of the Base control functions

#pragma region CheckBox
CCheckBox::CCheckBox()
{
	Checked = false;
	bIsSub = false;

	m_Flags = UIFlags::UI_Clickable | UIFlags::UI_Drawable | UIFlags::UI_SaveFile;
	m_iWidth = 15;
	m_iHeight = 15;

	FileControlType = UIControlTypes::UIC_CheckBox;
}

void CCheckBox::SetState(bool s)
{
	Checked = s;
}

bool CCheckBox::GetState()
{
	return Checked;
}

bool CCheckBox::GetIsSub()
{
	return bIsSub;
}

void CCheckBox::SetAsSub(bool t)
{
	bIsSub = t;
}

void CCheckBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Draw::GradientV(a.x, a.y, 15, 15, Color(206, 105, 105, 255), Color(206, 105, 105, 245));
	Draw::Outline(a.x, a.y, 15, 15, Color(2, 2, 2, 255));

	if (Checked)
	{
		Color grad;
		bool bSetRed = false;
		bool bSetGreen = false;
		bool bSetBlue = false;
		if (globals.forecolor.r() >= 15)
			bSetRed = true;
		if (globals.forecolor.g() >= 15)
			bSetGreen = true;
		if (globals.forecolor.b() >= 15)
			bSetBlue = true;

		float red = bSetRed ? globals.forecolor.r() - 15 : globals.forecolor.r();
		float green = bSetGreen ? globals.forecolor.g() - 15 : globals.forecolor.g();
		float blue = bSetBlue ? globals.forecolor.b() - 15 : globals.forecolor.b();

		grad = Color(red, green, blue, 245);

		Draw::GradientV(a.x, a.y, 15, 15, Color(100, 180, 115, 255), Color(100, 180, 115, 255));
		Draw::Outline(a.x, a.y, 15, 15, Color(2, 2, 2, 255));
	}
}

void CCheckBox::OnUpdate(){ m_iWidth = 15; m_iHeight = 15; }

void CCheckBox::OnClick()
{
	Checked = !Checked;
}
#pragma endregion Implementations of the Check Box functions

#pragma region Label
CLabel::CLabel()
{
	m_Flags = UIFlags::UI_Drawable;
	Text = "Default";
	FileIdentifier = "Default";
}

void CLabel::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Draw::Text(a.x, a.y - 1, Color(244, 244, 244, 255), Draw::Fonts::Menu, Text.c_str());
}

void CLabel::SetText(std::string text)
{
	Text = text;
}

void CLabel::OnUpdate(){}
void CLabel::OnClick(){}
#pragma endregion Implementations of the Label functions

#pragma region GroupBox
CGroupBox::CGroupBox()
{
	Items = 1;
	iYAdd = 0;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_RenderFirst;
	Text = "Default";
	FileIdentifier = "Default";
}

void CGroupBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT txtSize = Draw::GetTextSize(Draw::Fonts::Menu, Text.c_str());

	Draw::DrawRect(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(90, 90, 90, 1));
	Draw::text(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Text.c_str(), Draw::Fonts::Menu, Color(244, 244, 244, 255));
	if (group_tabs.size())
	{
		Draw::Line(a.x + 1, a.y + 8, a.x + m_iWidth, a.y + 8, Color(48, 48, 48, 255));
		Draw::Line(a.x + 1, a.y + 38, a.x + m_iWidth, a.y + 38, Color(48, 48, 48, 255));
		Draw::DrawRect(a.x + 1, a.y + 9, m_iWidth - 1, 29, Color(21, 21, 19, 255));
		Draw::DrawRect(a.x + 1, a.y + 36, m_iWidth - 1, 2, Color(62, 62, 62, 255));

		for (int i = 0; i < group_tabs.size(); i++)
		{
			RECT text_size = Draw::GetTextSize(Draw::Fonts::Menu, group_tabs[i].name.c_str());

			int width = m_iWidth - 1;

			int tab_length = (width / group_tabs.size());

			int text_position[] = {
				(a.x + (tab_length * (i + 1)) - (tab_length / 2)),
				a.y + 23 - (text_size.bottom / 2)
			};

			RECT tab_area = {
				(a.x + 1) + (tab_length * i),
				a.y + 9,
				tab_length,
				29
			};

			if (GetAsyncKeyState(VK_LBUTTON))
			{
				if (GUI.IsMouseInRegion(tab_area))
				{
					selected_tab = group_tabs[i].id;
				}
			}

			if (selected_tab == group_tabs[i].id)
			{
				Draw::DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom, globals.forecolor);
				Draw::DrawRect(tab_area.left, tab_area.top, tab_area.right, tab_area.bottom - 2, Color(39, 39, 39, 255));
				Draw::text(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), Draw::Fonts::Menu, Color(205, 205, 205, 245));
			}
			else if (selected_tab != group_tabs[i].id)
				Draw::text(text_position[0] - (text_size.right / 2), text_position[1], group_tabs[i].name.c_str(), Draw::Fonts::Menu, Color(92, 92, 92, 255));
		}
	}

	Draw::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(17, 17, 17, 0));
	Draw::Text(a.x + (m_iWidth / 2) - (txtSize.right / 2), a.y - (txtSize.bottom / 2) - 1, Color(244, 244, 244, 0), Draw::Fonts::Menu, Text.c_str());

	//Top Left
	Draw::Line(a.x, a.y, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 2, a.y, Color(48, 48, 48, 0));
	Draw::Line(a.x - 1, a.y - 1, a.x + (m_iWidth / 2) - (txtSize.right / 2) - 1, a.y - 1, Color(0, 0, 0, 0));

	//Top Right
	Draw::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y, a.x + m_iWidth, a.y, Color(48, 48, 48, 0));
	Draw::Line(a.x + (m_iWidth / 2) + (txtSize.right / 2) + 2, a.y - 1, a.x + m_iWidth + 1, a.y - 1, Color(0, 0, 0, 0));

	//Left
	Draw::Line(a.x, a.y, a.x, a.y + m_iHeight, Color(49, 49, 49, 0));
	Draw::Line(a.x - 1, a.y, a.x - 1, a.y + m_iHeight, Color(0, 0, 0, 0));

	//Bottom
	Draw::Line(a.x, a.y + m_iHeight, a.x + m_iWidth, a.y + m_iHeight, Color(48, 48, 48, 0));
	Draw::Line(a.x - 1, a.y + m_iHeight + 1, a.x + m_iWidth + 2, a.y + m_iHeight + 1, Color(0, 0, 0, 0));

	//Right
	Draw::Line(a.x + m_iWidth, a.y, a.x + m_iWidth, a.y + m_iHeight + 1, Color(48, 48, 48, 0));
	Draw::Line(a.x + m_iWidth + 1, a.y, a.x + m_iWidth + 1, a.y + m_iHeight + 1, Color(0, 0, 0, 0));
}

void CGroupBox::SetText(std::string text)
{
	Text = text;
}

void CGroupBox::PlaceLabledControl(std::string Label, CTab *Tab, CControl* control)
{
	if (control->FileControlType == UIControlTypes::UIC_CheckBox)
	{
		CCheckBox* cb = (CCheckBox*)control;

		int x = m_x + 8;
		int y = m_y + Items * 12;

		int x2 = m_x + 258;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x += m_iWidth / 2;

		int cw, ch;
		control->SetPosition(x2, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 32, ch);
		Tab->RegisterControl(control);
		Items++;
	}
	else if (control->FileControlType == UIControlTypes::UIC_MultiBox)
	{
		int x = m_x + 34;
		int y = m_y + Items * 20 + iYAdd;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 16;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize(m_iWidth - 32, ch);
		Tab->RegisterControl(control);
	}
	else if (control->FileControlType == UIControlTypes::UIC_ComboBox || control->FileControlType == UIControlTypes::UIC_ItemSelector)
	{
		int x = m_x + 8;
		int y = m_y + Items * 12;

		int x2 = m_x + 160;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x += m_iWidth / 2;

		int cw, ch;
		control->SetPosition(x2, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 32, ch);
		Tab->RegisterControl(control);
		Items++;
	}
	else if (control->FileControlType == UIControlTypes::UIC_KeyBind)
	{
		int x = m_x + 8;
		int y = m_y + Items * 12;

		int x2 = m_x + 160;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x += m_iWidth / 2;

		int cw, ch;
		control->SetPosition(x2, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 32, ch);
		Tab->RegisterControl(control);
		Items++;
	}
	else if (control->FileControlType == UIControlTypes::UIC_Slider)
	{
		int x = m_x + 8;
		int y = m_y + Items * 12;

		int x2 = m_x + 160;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x += m_iWidth / 2;

		int cw, ch;
		control->SetPosition(x2, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 32, ch);
		Tab->RegisterControl(control);
		Items++;
	}
	else if (control->FileControlType == UIControlTypes::UIC_Button)
	{
		int x = m_x + 34;
		int y = m_y + Items * 20 + iYAdd;

		x += m_iWidth / 2;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y += 2;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize(m_iWidth - 32, ch);
		Tab->RegisterControl(control);
	}
	else
	{
		int x = m_x + 34;
		int y = m_y + Items * 20 + iYAdd;

		x += m_iWidth / 2 - 11;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);
	}
	Items++;
}
void CGroupBox::PlaceLabledControl2(std::string Label, CTab *Tab, CControl* control)
{
	if (control->FileControlType == UIControlTypes::UIC_CheckBox)
	{
		CCheckBox* cb = (CCheckBox*)control;

		int x = m_x + 34;
		int y = m_y + Items * 20 + iYAdd;

		if (cb->GetIsSub())
			x += 18;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		if (cb->GetIsSub())
			x = m_x + 34;
		else
			x = m_x + 16;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 32, ch);
		Tab->RegisterControl(control);
	}
	else if (control->FileControlType == UIControlTypes::UIC_MultiBox)
	{
		int x = m_x + 34;
		int y = m_y + Items * 20 + iYAdd;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 16;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize(m_iWidth - 32, ch);
		Tab->RegisterControl(control);
	}
	else if (control->FileControlType == UIControlTypes::UIC_ComboBox || control->FileControlType == UIControlTypes::UIC_ItemSelector)
	{
		int x = m_x + 39;
		int y = m_y + Items * 20 + iYAdd;

		int m_iWidth = 350;

		x += m_iWidth;

		RECT txtSize = Draw::GetTextSize(Draw::Fonts::Menu, Label.c_str());
		y = y + 10 - (txtSize.bottom / 2) - 1;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y = m_y + Items * 20 + iYAdd - 1;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth) - 25, ch);
		Tab->RegisterControl(control);

		iYAdd += 6;
	}
	else if (control->FileControlType == UIControlTypes::UIC_KeyBind)
	{
		int x = m_x + 39;
		int y = m_y + Items * 20 + iYAdd;

		int m_iWidth = 350;

		x += m_iWidth;

		RECT txtSize = Draw::GetTextSize(Draw::Fonts::Menu, Label.c_str());
		y = y + 10 - (txtSize.bottom / 2) - 1;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y = m_y + Items * 20 + iYAdd - 1;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth)-25, ch);
		Tab->RegisterControl(control);

		iYAdd += 6;
	}
	else if (control->FileControlType == UIControlTypes::UIC_Slider)
	{
		int x = m_x + 39;
		int y = m_y + Items * 20 + iYAdd - 1;

		x += m_iWidth / 2 - 11;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y += 1;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);
	}
	else if (control->FileControlType == UIControlTypes::UIC_Button)
	{
		int x = m_x + 34;
		int y = m_y + Items * 20 + iYAdd;

		x += m_iWidth / 2;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;
		y += 2;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize(m_iWidth - 32, ch);
		Tab->RegisterControl(control);
	}
	else
	{
		int x = m_x + 34;
		int y = m_y + Items * 20 + iYAdd;

		x += m_iWidth / 2 - 11;

		CLabel* label = new CLabel;
		label->SetPosition(x, y);
		label->SetText(Label);
		Tab->RegisterControl(label);

		x = m_x + 34;

		int cw, ch;
		control->SetPosition(x, y);
		control->GetSize(cw, ch);
		control->SetSize((m_iWidth / 2) - 25, ch);
		Tab->RegisterControl(control);
	}
	Items++;
}

void CGroupBox::PlaceLabledControl3(int g_tab, std::string Label, CTab *Tab, CControl* control) {
	bool has_tabs = group_tabs.size() ? 1 : 0;

	if (has_tabs) {
		bool has_reset = false;

		for (int i = 0; i < reset_tabs.size(); i++) {
			if (reset_tabs[i] == g_tab)
				has_reset = true;
		}

		if (!has_reset) {
			initialized = false;
			reset_tabs.push_back(g_tab);
		}
	}

	if (!initialized) {
		Items = 0;
		last_y = has_tabs ? m_y + 48 : m_y + 8;
		initialized = true;
	}

	bool add_label_y = true;
	bool is_checkbox = control->FileControlType == UIControlTypes::UIC_CheckBox;
	bool is_label = control->FileControlType == UIControlTypes::UIC_Label;
	bool is_keybind = control->FileControlType == UIControlTypes::UIC_KeyBind;
	bool is_color = control->FileControlType == UIControlTypes::UIC_ColorSelector;

	int x = m_x + 38;
	int y = last_y;
	int control_width, control_height;
	control->GetSize(control_width, control_height);

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	label->parent_group = this;
	label->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(label);

	if (is_checkbox || is_label || is_color) add_label_y = false;

	if (Label != "" && add_label_y && !is_keybind) {
		RECT label_size = Draw::GetTextSize(Draw::Fonts::Menu, Label.c_str());
		last_y += 14;
		y = last_y;
	}

	if (!is_keybind)
		last_control_height = control_height + 7;

	if (is_keybind || (is_color && Label == "")) {
		y -= last_control_height;
		x = m_x + m_iWidth - 36;
	}
	if (is_color && Label != "")
		x = m_x + m_iWidth - 36;
	if (is_checkbox)
		x -= 24;

	control->SetPosition(x, is_checkbox ? y + 1 : y);
	control->SetSize(m_iWidth - (38 * 2), control_height);
	control->parent_group = this;
	control->g_tab = g_tab ? g_tab : 0;
	Tab->RegisterControl(control);

	if (!is_keybind) {
		if (!is_color || is_color && Label != "")
			last_y += control_height + 7;
	}
}

void CGroupBox::AddTab(CGroupTab t)
{
	group_tabs.push_back(t);

	if (selected_tab == 0)
		selected_tab++;
}
void CGroupBox::OnUpdate(){}
void CGroupBox::OnClick(){}
#pragma endregion Implementations of the Group Box functions

void CGroupBox::meme1(std::string Label, CTab *Tab, CControl* control, int c, int r)
{
	int CellW = (m_iWidth / Columns);
	int x = m_x + 10 + (c * CellW);
	int y = m_y + 26 + r * 22;

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	Tab->RegisterControl(label);

	x += CellW / 2;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((CellW / 2) - 20, ch);
	Tab->RegisterControl(control);

	if (r > MaxRow)
		MaxRow = r;

	char szCRCBuffer[256];
	sprintf_s(szCRCBuffer, "%s%s", Text.c_str(), Label.c_str());
	control->SetFileID(CRC32(szCRCBuffer, strlen(szCRCBuffer)));

	m_iHeight = (MaxRow + 1) * 22 + 26;
}
#pragma endregion meme1
#pragma region Control2
void CGroupBox::Control2(CTab *Tab, CControl* control, std::string Label)
{
	int x = m_x + 16;
	int y = m_y + Items * 24;

	CLabel* label = new CLabel;
	label->SetPosition(x, y);
	label->SetText(Label);
	Tab->RegisterControl(label);

	x += m_iWidth / 2;

	int cw, ch;
	control->SetPosition(x, y);
	control->GetSize(cw, ch);
	control->SetSize((m_iWidth / 2) - 32, ch);
	Tab->RegisterControl(control);
	Items++;
}
#pragma endregion Control2

bool CSlider::GetIsAuto()
{
	return bIsSub;
}

void CSlider::SetAsAuto(bool t)
{
	bIsSub = t;
}

#pragma region Sliders
CSlider::CSlider()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	format = FORMAT_INT;
	FileControlType = UIControlTypes::UIC_Slider;
}

void CSlider::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Draw::GradientV(a.x, a.y + 1, m_iWidth, 7, Color(52, 52, 52, 255), Color(68, 68, 68, 245));

	float Ratio = Value / (Max - Min);
	float Location = Ratio*m_iWidth;

	Color grad;
	bool bSetRed = false;
	bool bSetGreen = false;
	bool bSetBlue = false;
	if (globals.forecolor.r() >= 15)
		bSetRed = true;
	if (globals.forecolor.g() >= 15)
		bSetGreen = true;
	if (globals.forecolor.b() >= 15)
		bSetBlue = true;

	float red = bSetRed ? globals.forecolor.r() - 15 : globals.forecolor.r();
	float green = bSetGreen ? globals.forecolor.g() - 15 : globals.forecolor.g();
	float blue = bSetBlue ? globals.forecolor.b() - 15 : globals.forecolor.b();

	grad = Color(red, green, blue, 245);

	Draw::GradientV(a.x, a.y + 1, Location, 7, Color(56, 56, 56, 255), Color(56, 56, 56, 245));

	Draw::Outline(a.x - 1, a.y, m_iWidth + 2, 9, Color(2, 2, 2, 255));

	Draw::Clear(a.x + Location, a.y + 0.1, 4, 9, Color(147, 147, 147, 255));

	char buffer[24];
	if (format == FORMAT_DECDIG2)
		sprintf_s(buffer, "%.2f", Value);
	else if (format == FORMAT_DECDIG1)
		sprintf_s(buffer, "%.1f", Value);
	else if (format == FORMAT_INT)
		sprintf_s(buffer, "%1.0f", Value);
	else if (format == FORMAT_DEG)
		sprintf_s(buffer, "%1.f°", Value);
	else if (format == FORMAT_PERC)
		sprintf_s(buffer, "%1.f%%", Value);

	RECT txtSize = Draw::GetTextSize(Draw::Fonts::MenuBold, buffer);
	if (bIsSub && Value == 0)
		Draw::Text(a.x + (m_iWidth + 12) - txtSize.right / 2, a.y + 0.5,  Color(244, 244, 244, 255), Draw::Fonts::MenuBold, "A");
	else
		Draw::Text(a.x + (m_iWidth + 12) - txtSize.right / 2, a.y + 0.5, Color(244, 244, 244, 255), Draw::Fonts::MenuBold, buffer);

	//RECT txtSize = Render::GetTextSize(Render::Fonts::MenuBold, buffer);
	//Render::Text(a.x + (m_iWidth / 2) - txtSize.right / 2, a.y + 10, Color(0, 0, 0, 255), Render::Fonts::MenuBold, buffer);
}

void CSlider::OnUpdate(){
	POINT a = GetAbsolutePos();
	m_iHeight = 11;

	if (DoDrag)
	{
		if (GUI.GetKeyState(VK_LBUTTON))
		{
			POINT m = GUI.GetMouse();
			float NewX;
			float Ratio;
			NewX = m.x - a.x - 1;
			if (NewX < 0) NewX = 0;
			if (NewX > m_iWidth) NewX = m_iWidth;
			Ratio = NewX / float(m_iWidth);
			Value = Min + (Max - Min)*Ratio;
		}
		else
		{
			DoDrag = false;
		}
	}
}

void CSlider::OnClick(){
	POINT a = GetAbsolutePos();
	RECT SliderRegion = { a.x, a.y, m_iWidth, 11 };
	if (GUI.IsMouseInRegion(SliderRegion))
	{
		DoDrag = true;
	}
}

float CSlider::GetValue()
{
	return Value;
}

void CSlider::SetValue(float v)
{
	Value = v;
}

void CSlider::SetBoundaries(float min, float max)
{
	Min = min; Max = max;
}

void CSlider::SetFormat(SliderFormat type)
{
	format = type;
}
#pragma endregion Implementations of the Slider functions

CListBox::CListBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	SelectedIndex = 0;
}

void CListBox::Draw(bool hover)
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	Draw::Outline(a.x, a.y, m_iWidth, m_iHeight + 2, Color(17, 17, 17, 255));

	if (Items.size() > 0)
	{
		int drawnItems = 0;
		for (int i = ScrollTop; (i < Items.size() && drawnItems < ItemsToDraw); i++)
		{
			Color textColor = UI_COL_TABTEXT;
			RECT ItemRegion = { a.x + 1, a.y + 1 + drawnItems * 16, m_iWidth - LIST_SCROLL_WIDTH - 2 , 16 };

			if (i == SelectedIndex)
			{
				textColor = COL_WHITE;
				Draw::Clear(ItemRegion.left, ItemRegion.top, ItemRegion.right, ItemRegion.bottom, UI_COL_MAIN);
			}
			else if (GUI.IsMouseInRegion(ItemRegion))
			{
				textColor = COL_WHITE;
				Draw::Clear(ItemRegion.left, ItemRegion.top, ItemRegion.right, ItemRegion.bottom, UI_COL_TABTEXT);
			}

			Draw::Text(ItemRegion.left + 4, ItemRegion.top + 2, textColor, Draw::Fonts::Menu, Items[i].c_str());
			drawnItems++;
		}

		// Ratio of how many visible to how many are hidden
		float sizeRatio = float(ItemsToDraw) / float(Items.size());
		if (sizeRatio > 1.f) sizeRatio = 1.f;
		float posRatio = float(ScrollTop) / float(Items.size());
		if (posRatio > 1.f) posRatio = 1.f;

		sizeRatio *= m_iHeight;
		posRatio *= m_iHeight;

		Draw::Clear(a.x + m_iWidth - LIST_SCROLL_WIDTH, a.y + posRatio, LIST_SCROLL_WIDTH, sizeRatio, UI_COL_TABTEXT);
	}

}

void CListBox::AddItem(std::string text, int value)
{
	Items.push_back(text);
	Values.push_back(value);
}

void CListBox::OnClick()
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	// Check the items
	if (Items.size() > 0)
	{
		int drawnItems = 0;
		for (int i = ScrollTop; (i < Items.size() && drawnItems < ItemsToDraw); i++)
		{
			Color textColor = Color(25, 25, 25, 255);
			RECT ItemRegion = { a.x + 1, a.y + 1 + drawnItems * 16, m_iWidth - LIST_SCROLL_WIDTH - 2 , 16 };
			if (GUI.IsMouseInRegion(ItemRegion))
			{
				SelectItem(i);
				return;
			}
			drawnItems++;
		}
	}

	// Scrollbar checks are in update so we can just drag

}

void CListBox::OnUpdate()
{
	int ItemsToDraw = m_iHeight / LIST_ITEM_HEIGHT;
	POINT a = GetAbsolutePos();

	// Did we click in the scrollbar??
	RECT Scroll = { a.x + m_iWidth - LIST_SCROLL_WIDTH , a.y + 1, LIST_SCROLL_WIDTH - 2 ,m_iHeight };
	if (GUI.IsMouseInRegion(Scroll) && GUI.GetKeyState(VK_LBUTTON))
	{
		// Ratio of how many visible to how many are hidden
		float ratio = float(ItemsToDraw) / float(Items.size());
		POINT m = GUI.GetMouse();
		m.y -= a.y;

		float sizeRatio = float(ItemsToDraw) / float(Items.size());
		sizeRatio *= m_iHeight;
		float heightDelta = m.y + sizeRatio - m_iHeight;
		if (heightDelta > 0)
			m.y -= heightDelta;

		float mPosRatio = float(m.y) / float(m_iHeight);
		ScrollTop = mPosRatio*Items.size();
		if (ScrollTop < 0)
			ScrollTop = 0;

	}
}

void CListBox::SetHeightInItems(int items)
{
	m_iHeight = items*LIST_ITEM_HEIGHT;
}

std::string CListBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "Error";
}


#pragma endregion Implementations of the ListBox functions

#pragma region ItemSelector
CItemSelector::CItemSelector()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_ItemSelector;
	Type = ITEMSELECTOR_INT;
}

void CItemSelector::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	//Minus

	RECT rMinus = { a.x, a.y, 16, m_iHeight };

	Draw::Outline(a.x - 1, a.y - 1, 18, m_iHeight + 2, Color(0, 0, 0, 255));
	Draw::Clear(a.x, a.y, 16, m_iHeight, Color(39, 39, 39, 255));

	if (GUI.IsMouseInRegion(rMinus) && GetAsyncKeyState(VK_LBUTTON))
		Draw::GradientV(a.x, a.y, 16, m_iHeight, Color(55, 55, 55, 255), Color(40, 40, 40, 245));

	RECT txtSizeMinus = Draw::GetTextSize(Draw::Fonts::Menu, "-");
	Draw::Text(a.x + (rMinus.right / 2) - (txtSizeMinus.right / 2),
		a.y + (m_iHeight / 2) - (txtSizeMinus.bottom / 2),
		Color(255, 255, 255, 255),
		Draw::Fonts::Menu,
		"-");

	//Main

	Draw::Outline(a.x + 19, a.y - 1, m_iWidth - 42, m_iHeight + 2, Color(0, 0, 0, 255));
	Draw::GradientV(a.x + 20, a.y, m_iWidth - 40, m_iHeight, Color(55, 55, 55, 255), Color(40, 40, 40, 245));

	if (Type == ITEMSELECTOR_INT)
	{
		char buffer[24];
		sprintf_s(buffer, "%1.0f", Value);

		RECT txtValueSize = Draw::GetTextSize(Draw::Fonts::Menu, buffer);
		Draw::Text(a.x + (m_iWidth / 2) - (txtValueSize.right / 2),
			a.y + (m_iHeight / 2) - (txtValueSize.bottom / 2),
			Color(255, 255, 255, 255),
			Draw::Fonts::Menu,
			buffer);
	}
	else if (Type == ITEMSELECTOR_STRING)
	{
		std::string text = Items[Value];
		RECT txtValueSize = Draw::GetTextSize(Draw::Fonts::Menu, text.c_str());
		Draw::Text(a.x + (m_iWidth / 2) - (txtValueSize.right / 2),
			a.y + (m_iHeight / 2) - (txtValueSize.bottom / 2),
			Color(255, 255, 255, 255),
			Draw::Fonts::Menu,
			text.c_str());
	}

	//Plus

	RECT rPlus = { a.x + (m_iWidth - 16), a.y, 16, m_iHeight };

	Draw::Outline(a.x + (m_iWidth - 16) - 1, a.y - 1, 18, m_iHeight + 2, Color(0, 0, 0, 255));
	Draw::Clear(a.x + (m_iWidth - 16), a.y, 16, m_iHeight, Color(39, 39, 39, 255));

	if (GUI.IsMouseInRegion(rPlus) && GetAsyncKeyState(VK_LBUTTON))
		Draw::GradientV(a.x + (m_iWidth - 16), a.y, 16, m_iHeight, Color(55, 55, 55, 255), Color(40, 40, 40, 245));

	RECT txtSizePlus = Draw::GetTextSize(Draw::Fonts::Menu, "+");
	Draw::Text(a.x + (m_iWidth - 8) - (txtSizePlus.right / 2),
		a.y + (m_iHeight / 2) - (txtSizePlus.bottom / 2),
		Color(255, 255, 255, 255),
		Draw::Fonts::Menu,
		"+");

}

void CItemSelector::Manager()
{
	if (UpdateType == UPDATE_LEFT)
	{
		if (Value - 1 < Min)
			return;
		else
			Value -= 1;
	}
	else if (UpdateType == UPDATE_RIGHT)
	{
		if (Value + 1 > Max)
			return;
		else
			Value += 1;
	}

	if (!IsHolding)
	{
		UpdateType = UPDATE_NONE;
	}
}

void CItemSelector::OnUpdate()
{
	Manager();

	m_iHeight = 16;

	POINT a = GetAbsolutePos();

	RECT rMinus = { a.x, a.y, 16, m_iHeight };
	RECT rPlus = { a.x + (m_iWidth - 16), a.y, 16, m_iHeight };

	if (GUI.IsMouseInRegion(rMinus) && GetAsyncKeyState(VK_LBUTTON))
	{
		preTicks += 1;
		if (preTicks > 125)
		{
			UpdateType = UPDATE_LEFT;
			IsHolding = true;
		}
	}
	else if (GUI.IsMouseInRegion(rPlus) && GetAsyncKeyState(VK_LBUTTON))
	{
		preTicks += 1;
		if (preTicks > 125)
		{
			UpdateType = UPDATE_RIGHT;
			IsHolding = true;
		}
	}
	else
		preTicks = 0;

	IsHolding = false;
}

void CItemSelector::OnClick()
{
	POINT a = GetAbsolutePos();

	RECT rMinus = { a.x, a.y, 16, m_iHeight };
	RECT rPlus = { a.x + (m_iWidth - 16), a.y, 16, m_iHeight };

	if (GUI.IsMouseInRegion(rMinus))
		UpdateType = UPDATE_LEFT;
	else if (GUI.IsMouseInRegion(rPlus))
		UpdateType = UPDATE_RIGHT;
}

float CItemSelector::GetInt()
{
	return Value;
}

void CItemSelector::SetValue(float value)
{
	Value = value;
}

void CItemSelector::SetMin(float min)
{
	Min = min;
}

void CItemSelector::SetMax(float max)
{
	Max = max;
}

void CItemSelector::AddItem(std::string name)
{
	Items.push_back(name);
}

void CItemSelector::SetType(ItemSelector_Type type)
{
	Type = type;
}

#pragma endregion Implementations of the Item Selector functions

#pragma region KeyBinders

char* KeyStrings[254] = { nullptr, "M1", "M2", "Control+Break", "M3", "M4", "M5",
nullptr, "Backspace", "TAB", nullptr, nullptr, nullptr, "ENTER", nullptr, nullptr, "SHIFT", "CTRL", "ALT", "PAUSE",
"CAPS LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "ESC", nullptr, nullptr, nullptr, nullptr, "SPACEBAR",
"PG UP", "PG DOWN", "END", "HOME", "Left", "Up", "Right", "Down", nullptr, "Print", nullptr, "Print Screen", "Insert",
"Delete", nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", "Left Windows", "Right Windows", nullptr, nullptr, nullptr, "NUM 0", "NUM 1", "NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6",
"NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
"F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21", "F22", "F23", "F24", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, "NUM LOCK", "SCROLL LOCK", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, "LSHIFT", "RSHIFT", "LCONTROL", "RCONTROL", "LMENU", "RMENU", nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "Next Track", "Previous Track", "Stop", "Play/Pause", nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[{", "\\|", "}]", "'\"", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CKeyBind::CKeyBind()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

void CKeyBind::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Draw::Outline(a.x, a.y, m_iWidth, 20, Color(230, 230, 230, 255));
	Draw::GradientV(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(56, 56, 56, 255), Color(56, 56, 56, 255));
	if (hover)
		Draw::Clear(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(161, 161, 161, 0));
	bool GoodKeyName = false;
	char NameBuffer[128];
	char* KeyName = "Not Bound";

	if (IsGettingKey)
	{
		KeyName = "<Press A Key>";
	}
	else
	{
		if (Key >= 0)
		{
			KeyName = KeyStrings[Key];
			if (KeyName)
			{
				GoodKeyName = true;
			}
			else
			{
				if (GetKeyNameText(Key << 16, NameBuffer, 127))
				{
					KeyName = NameBuffer;
					GoodKeyName = true;
				}
			}
		}

		if (!GoodKeyName)
		{
			KeyName = "none";
		}
	}


	Draw::Text(a.x + 10, a.y + 4.5  * 1 - 4 + 1, Color(255, 255, 255, 255), Draw::Fonts::MenuBold, KeyName);
}

void CKeyBind::OnUpdate() {
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	if (IsGettingKey)
	{
		for (int i = 0; i < 255; i++)
		{
			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE)
				{
					IsGettingKey = false;
					Key = -1;
					return;
				}

				Key = i;
				IsGettingKey = false;
				return;
			}
		}
	}
}

void CKeyBind::OnClick(){
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}

int CKeyBind::GetKey()
{
	return Key;
}

void CKeyBind::SetKey(int key)
{
	Key = key;
}

#pragma endregion Implementations of the KeyBind Control functions

#pragma region Button
CButton::CButton()
{
	m_iWidth = 177;
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable;
	FileControlType == UIControlTypes::UIC_Button;
	Text = "Default";
	CallBack = nullptr;
	FileIdentifier = "Default";
}

void CButton::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	Draw::Outline(a.x, a.y, m_iWidth, m_iHeight, Color(2, 2, 2, 255));
	if (hover)
		Draw::GradientV(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(39, 39, 39, 255), Color(44, 44, 44, 255));
	else
		Draw::GradientV(a.x + 2, a.y + 2, m_iWidth - 4, m_iHeight - 4, Color(35, 35, 35, 255), Color(39, 39, 39, 255));

	RECT TextSize = Draw::GetTextSize(Draw::Fonts::MenuBold, Text.c_str());
	int TextX = a.x + (m_iWidth / 2) - (TextSize.left / 2);
	int TextY = a.y + (m_iHeight / 2) - (TextSize.bottom / 2);

	Draw::Text(TextX, TextY, Color(200, 200, 200, 255), Draw::Fonts::MenuBold, Text.c_str());
}

void CButton::SetText(std::string text)
{
	Text = text;
}

void CButton::SetCallback(CButton::ButtonCallback_t callback)
{
	CallBack = callback;
}

void CButton::OnUpdate()
{
	m_iHeight = 26;
}

void CButton::OnClick()
{
	if (CallBack)
		CallBack();
}
#pragma endregion Implementations of the Button functions

#pragma region ComboBox
CComboBox::CComboBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_ComboBox;
}

void CComboBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 20 };
	Draw::Outline(a.x, a.y, m_iWidth, 20, Color(230, 230, 230, 255));
	Draw::GradientV(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(56, 56, 56, 255), Color(56, 56, 56, 255));

	// Hover for the Top Box
	if (GUI.IsMouseInRegion(Region))
		Draw::GradientV(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(56, 56, 56, 255), Color(44, 44, 44, 255));

	// If we have some items
	if (Items.size() > 0)
	{
		// The current item
		RECT txtSize = Draw::GetTextSize(Draw::Fonts::Menu, GetItem().c_str());
		Draw::Text(a.x + 10, a.y + (Region.bottom / 2) - (txtSize.bottom / 2), Color(244, 244, 244, 255), Draw::Fonts::Menu, GetItem().c_str());

		// If the drop down part is open
		if (IsOpen)
		{

			Draw::GradientV(a.x, a.y + 20, m_iWidth, Items.size() * 16, Color(56, 56, 56, 255), Color(56, 56, 56, 255));

			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 17 + i * 16, m_iWidth, 16 };

				// Hover
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					Draw::GradientV(a.x + 2, a.y + 20 + i * 16, m_iWidth - 4, 16, Color(46, 46, 46, 255), Color(46, 46, 46, 255));
				}

				if (i == SelectedIndex)
					Draw::Text(a.x + 10, a.y + 19 + i * 16 - 1 + 4, Color(255, 255, 255, 255), Draw::Fonts::MenuBold, Items[i].c_str());
				else if (GUI.IsMouseInRegion(ItemRegion))
					Draw::Text(a.x + 10, a.y + 19 + i * 16 - 1 + 4, Color(255, 255, 255, 255), Draw::Fonts::MenuBold, Items[i].c_str());
				else
					Draw::Text(a.x + 10, a.y + 19 + i * 16 - 1 + 4, Color(244, 244, 244, 255), Draw::Fonts::Menu, Items[i].c_str());
			}
			Draw::Outline(a.x, a.y + 20, m_iWidth, Items.size() * 16, Color(245, 245, 245, 0));
		}
	}
	Vertex_t Verts2[3];
	Verts2[0].m_Position.x = a.x + m_iWidth - 10;
	Verts2[0].m_Position.y = a.y + 8;
	Verts2[1].m_Position.x = a.x + m_iWidth - 5;
	Verts2[1].m_Position.y = a.y + 8;
	Verts2[2].m_Position.x = a.x + m_iWidth - 7.5;
	Verts2[2].m_Position.y = a.y + 11;

	//Draw::Polygon(3, Verts2, Color(2, 2, 2, 255));
}

void CComboBox::AddItem(std::string text)
{
	Items.push_back(text);
	SelectedIndex = 0;
}

void CComboBox::OnUpdate()
{
	if (IsOpen)
	{
		m_iHeight = 16 + 16 * Items.size();

		if (parent->GetFocus() != this)
			IsOpen = false;
	}
	else
	{
		m_iHeight = 16;
	}
		
}

void CComboBox::OnClick()
{
	POINT a = GetAbsolutePos();
	RECT Region = { a.x, a.y, m_iWidth, 16 };

	if (IsOpen)
	{
		// If we clicked one of the items(Not in the top bar)
		if (!GUI.IsMouseInRegion(Region))
		{
			// Draw the items
			for (int i = 0; i < Items.size(); i++)
			{
				RECT ItemRegion = { a.x, a.y + 16 + i * 16, m_iWidth, 16 };

				// Hover
				if (GUI.IsMouseInRegion(ItemRegion))
				{
					SelectedIndex = i;
				}
			}
		}

		// Close the drop down
		IsOpen = false;
	}
	else
	{
		IsOpen = true;
	}
	
}

int CComboBox::GetIndex()
{
	return SelectedIndex; 
}

void CComboBox::SetIndex(int index)
{
	SelectedIndex = index;
}

std::string CComboBox::GetItem()
{
	if (SelectedIndex >= 0 && SelectedIndex < Items.size())
	{
		return Items[SelectedIndex];
	}

	return "Error";
}

void CComboBox::SelectIndex(int idx)
{
	if (idx >= 0 && idx < Items.size())
	{
		SelectedIndex = idx;
	}
}

#pragma endregion Implementations of the ComboBox functions

#pragma region MultiBox
CMultiBox::CMultiBox()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_Focusable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_MultiBox;
}

void CMultiBox::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	for (int i = 0; i < items.size(); i++)
	{
		MultiBoxItem item = items[i];
		RECT txtsize = Draw::GetTextSize(Draw::Fonts::Menu, item.text);
		int item_x = a.x - 60 + (m_iWidth / 2) - (txtsize.right / 2);
		int item_y = a.y + 20 + (i * 16) - (txtsize.bottom / 2) + 7;
		if (!item.bselected)
			Draw::Text(item_x, item_y, Color(255, 255, 255, 255), Draw::Fonts::Menu, item.text);
		else
			Draw::Text(item_x, item_y, globals.forecolor, Draw::Fonts::Menu, item.text);
	}
}

void CMultiBox::OnUpdate()
{
	m_iHeight = (items.size() * 16) + 19;

}

void CMultiBox::OnClick()
{
	POINT a = GetAbsolutePos();
	for (int i = 0; i < items.size(); i++)
	{
		RECT area = { a.x, a.y + i * 16 + 19, m_iWidth, 16 };
		if (GUI.IsMouseInRegion(area))
		{
			items[i].bselected = !items[i].bselected;
		}
	}
}

void CMultiBox::SetTitle(const char* tl)
{
	title = tl;
}
#pragma endregion Implementations of the MultiBox functions

#pragma region TextField
char* KeyDigitsLowercase[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
"y", "z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "/?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "[", "\\", "]", "'", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

char* KeyDigitsCapital[254] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
"Y", "Z", nullptr, nullptr, nullptr, nullptr, nullptr, "0", "1", "2", "3", "4", "5", "6",
"7", "8", "9", nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, ";", "+", ",", "-", ".", "?", "~", nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, "{", "|", "}", "\"", nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };

CTextField::CTextField()
{
	m_Flags = UIFlags::UI_Drawable | UIFlags::UI_Clickable | UIFlags::UI_SaveFile;
	FileControlType = UIControlTypes::UIC_KeyBind;
}

std::string CTextField::getText()
{
	return text;
}

void CTextField::SetText(std::string stext)
{
	text = stext;
}

void CTextField::Draw(bool hover)
{
	POINT a = GetAbsolutePos();

	Draw::Outline(a.x, a.y, m_iWidth, 20, Color(230, 230, 230, 255));
	Draw::GradientV(a.x + 2, a.y + 2, m_iWidth - 4, 16, Color(56, 56, 56, 255), Color(56, 56, 56, 255));
	if (hover || IsGettingKey)
		Draw::GradientV(a.x + 2, a.y + 2, m_iWidth - 4, 12, Color(63, 63, 63, 255), Color(46, 46, 46, 245));

	const char *cstr = text.c_str();

	Draw::Text(a.x + 2, a.y + 2 - 1, Color(244, 244, 244, 255), Draw::Fonts::Menu, cstr);
}

void CTextField::OnUpdate()
{
	m_iHeight = 16;
	POINT a = GetAbsolutePos();
	POINT b;
	const char *strg = text.c_str();

	if (IsGettingKey)
	{
		b = GetAbsolutePos();
		for (int i = 0; i < 255; i++)
		{

			if (GUI.GetKeyPress(i))
			{
				if (i == VK_ESCAPE || i == VK_RETURN || i == VK_INSERT)
				{
					IsGettingKey = false;
					return;
				}

				if (i == VK_BACK && strlen(strg) != 0)
				{
					text = text.substr(0, strlen(strg) - 1);
				}

				if (strlen(strg) < 20 && i != NULL && KeyDigitsCapital[i] != nullptr)
				{
					if (GetAsyncKeyState(VK_SHIFT))
					{
						text = text + KeyDigitsCapital[i];
					}
					else
					{
						text = text + KeyDigitsLowercase[i];
					}
					return;
				}

				if (strlen(strg) < 20 && i == 32)
				{
					text = text + " ";
					return;
				}
			}
		}
	}
}

void CTextField::OnClick()
{
	POINT a = GetAbsolutePos();
	if (!IsGettingKey)
	{
		IsGettingKey = true;
	}
}
#pragma endregion Implementations of the Text Field functions