

#pragma once

#include "GUI.h"

class CCheckBox : public CControl
{
public:
	CCheckBox();
	void SetState(bool s);
	bool GetState();
	bool GetIsSub();
	void SetAsSub(bool);
protected:
	bool bIsSub;
	bool Checked;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CLabel : public CControl
{
public:
	CLabel();
	void SetText(std::string text);
protected:
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CGroupTab
{
public:
	std::string name;
	int id;
public:
	CGroupTab(std::string _name, int _id)
	{
		this->name = _name;
		this->id = _id;
	}
};

class CGroupBox : public CControl
{
public:
	CGroupBox();
	void SetText(std::string text);
	void PlaceLabledControl(std::string Label, CTab *Tab, CControl* control);
	void PlaceLabledControl2(std::string Label, CTab * Tab, CControl * control);
	void PlaceLabledControl3(int g_tab, std::string Label, CTab* Tab, CControl* control);
	void Control2(CTab *Tab, CControl* control, std::string Label);
	void PlaceLabledControlPLayerlist(std::string Label, CTab *Tab, CControl* control, int c, int r);
	void meme1(std::string Label, CTab *Tab, CControl* control, int c, int r);
	inline void SetColumns(int c) { Columns = c; }
	void AddTab(CGroupTab t);
	int selected_tab = 0;
	std::vector<CGroupTab> group_tabs;
protected:
	std::vector<int> reset_tabs;
	int Columns;
	int MaxRow;
	int Items;
	std::string Text;
	int last_y;
	int last_control_height;
	bool initialized = false;
	float iYAdd;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CListBox : public CControl
{
public:
	CListBox();
	void AddItem(std::string text, int value = 0);
	inline int GetIndex() { return SelectedIndex; }
	std::string GetItem();
	inline int GetValue() { if (SelectedIndex >= 0 && SelectedIndex < Items.size()) return Values[SelectedIndex]; else return -1; };
	inline void ClearItems() { Items.clear(); Values.clear(); }
	void SetHeightInItems(int items);
	inline void SelectItem(int idx) { if (idx >= 0 && idx < Items.size()) SelectedIndex = idx; }

protected:
	std::vector<std::string> Items;
	std::vector<int> Values;

	int ScrollTop;

	int SelectedIndex;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

enum SliderFormat
{
	FORMAT_INT = 1,
	FORMAT_DECDIG1,
	FORMAT_DECDIG2,
	FORMAT_DEG,
	FORMAT_PERC
};

class CSlider : public CControl
{
public:
	bool GetIsAuto();
	void SetAsAuto(bool t);
	CSlider();
	float GetValue();
	void SetValue(float v);
	void SetBoundaries(float min, float max);
	void SetFormat(SliderFormat type);
protected:
	bool bIsSub;
	float Value;
	float Min;
	float Max;
	int format;
	bool DoDrag;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

enum ItemSelector_UpdateType
{
	UPDATE_NONE = 1,
	UPDATE_LEFT,
	UPDATE_RIGHT
};
enum ItemSelector_Type
{
	ITEMSELECTOR_INT = 1,
	ITEMSELECTOR_STRING
};

class CItemSelector : public CControl
{
public:
	CItemSelector();
	float GetInt();
	void SetValue(float value);
	void SetMin(float min);
	void SetMax(float max);
	void AddItem(std::string string);
	void SetType(ItemSelector_Type type);
protected:
	float Value;
	float Min;
	float Max;
	bool IsHolding;
	int preTicks;
	std::vector<std::string> Items;
	ItemSelector_Type Type;
	ItemSelector_UpdateType UpdateType;
	void Draw(bool Hover);
	void Manager();
	void OnUpdate();
	void OnClick();
};


class CKeyBind : public CControl
{
public:
	CKeyBind();
	int GetKey();
	void SetKey(int key);
protected:
	int Key;
	bool IsGettingKey;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CButton : public CControl
{
public:
	typedef void(*ButtonCallback_t)(void);
	CButton();
	void SetCallback(ButtonCallback_t callback);
	void SetText(std::string text);
protected:
	ButtonCallback_t CallBack;
	std::string Text;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CComboBox : public CControl
{
public:
	CComboBox();
	void AddItem(std::string text);
	void SelectIndex(int idx);
	int GetIndex();
	void SetIndex(int);
	std::string GetItem();
protected:
	std::vector<std::string> Items;
	int SelectedIndex;
	bool IsOpen;
	int winWidth;
	int winHeight;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

class CTextField : public CControl
{
public:
	CTextField();
	std::string getText();
	void SetText(std::string);
private:
	std::string text;
	bool IsGettingKey;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};

struct MultiBoxItem
{
	bool bselected;
	const char* text;

	MultiBoxItem(bool _bselected, const char* _text)
	{
		this->bselected = _bselected;
		this->text = _text;
	}
};

class CMultiBox : public CControl
{
public:
	CMultiBox();
	std::vector<MultiBoxItem> items;
	void SetTitle(const char*);
private:
	const char* title;
	void Draw(bool hover);
	void OnUpdate();
	void OnClick();
};