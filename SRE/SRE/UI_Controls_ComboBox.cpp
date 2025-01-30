// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::ComboBox::ComboBox(const Window &window, DWORD style, DWORD exStyle) :
	Control(window, L"COMBOBOX", L"", style | CBS_OWNERDRAWFIXED, exStyle),
	listHandle(InitListHandle()),
	hovering(false)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetSize(int width, int height, int listHeight)
{
	if (SendMessageW(handle, CB_SETITEMHEIGHT, -1, height - 6) == CB_ERR)
		return false;

	return Control::SetSize(width, height + listHeight);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetRightBottom(int right, int bottom, int listBottom)
{
	return SetSize(right - x, bottom - y, listBottom - bottom);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetFont(const Font &font)
{
	if (SendMessageW(handle, CB_SETITEMHEIGHT, 0, font.GetHeight()) == CB_ERR)
		return false;

	return Control::SetFont(font);
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ComboBox::GetTextColor() const
{
	return theme.ComboBoxListTextColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetTextColor(COLORREF color)
{
	theme.ComboBoxListTextColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ComboBox::GetBackgroundColor() const
{
	return theme.ComboBoxListBackgroundColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetBackgroundColor(COLORREF color)
{
	theme.ComboBoxListBackgroundColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ComboBox::GetSelectedTextColor() const
{
	return theme.ComboBoxListSelectedTextColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetSelectedTextColor(COLORREF color)
{
	theme.ComboBoxListSelectedTextColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ComboBox::GetSelectedBackgroundColor() const
{
	return theme.ComboBoxListSelectedBackgroundColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetSelectedBackgroundColor(COLORREF color)
{
	theme.ComboBoxListSelectedBackgroundColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ComboBox::GetTextColor(int index) const
{
	if (!IsValidIndex(index))
		return theme.ComboBoxListTextColor;

	return textColor[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetTextColor(int index, COLORREF color)
{
	if (!IsValidIndex(index))
		return false;

	textColor[index] = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ComboBox::GetBackgroundColor(int index) const
{
	if (!IsValidIndex(index))
		return theme.ComboBoxListBackgroundColor;

	return backgroundColor[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetBackgroundColor(int index, COLORREF color)
{
	if (!IsValidIndex(index))
		return false;

	backgroundColor[index] = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ComboBox::GetSelectedTextColor(int index) const
{
	if (!IsValidIndex(index))
		return theme.ComboBoxListSelectedTextColor;

	return selectedTextColor[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetSelectedTextColor(int index, COLORREF color)
{
	if (!IsValidIndex(index))
		return false;

	selectedTextColor[index] = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ComboBox::GetSelectedBackgroundColor(int index) const
{
	if (!IsValidIndex(index))
		return theme.ComboBoxListSelectedBackgroundColor;

	return selectedBackgroundColor[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetSelectedBackgroundColor(int index, COLORREF color)
{
	if (!IsValidIndex(index))
		return false;

	selectedBackgroundColor[index] = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ComboBox::GetCount() const
{
	return static_cast<int>(text.size());
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::IsValidIndex(int index) const
{
	return index >= 0 && index < GetCount();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Add(const std::wstring &value)
{
	SendMessageW(handle, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(value.c_str()));
	text.push_back(value);
	textColor.push_back(theme.ComboBoxListTextColor);
	backgroundColor.push_back(theme.ComboBoxListBackgroundColor);
	selectedTextColor.push_back(theme.ComboBoxListSelectedTextColor);
	selectedBackgroundColor.push_back(theme.ComboBoxListSelectedBackgroundColor);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Insert(int index, const std::wstring &value)
{
	if (!IsValidIndex(index) && index != GetCount())
		return false;

	SendMessageW(handle, CB_INSERTSTRING, index, reinterpret_cast<LPARAM>(value.c_str()));
	text.insert(text.begin() + index, value);
	textColor.insert(textColor.begin() + index, theme.ComboBoxListTextColor);
	backgroundColor.insert(backgroundColor.begin() + index, theme.ComboBoxListBackgroundColor);
	selectedTextColor.insert(selectedTextColor.begin() + index, theme.ComboBoxListSelectedTextColor);
	selectedBackgroundColor.insert(selectedBackgroundColor.begin() + index, theme.ComboBoxListSelectedBackgroundColor);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Delete(int index)
{
	if (!IsValidIndex(index))
		return false;

	SendMessageW(handle, CB_DELETESTRING, index, 0);
	text.erase(text.begin() + index);
	textColor.erase(textColor.begin() + index);
	backgroundColor.erase(backgroundColor.begin() + index);
	selectedTextColor.erase(selectedTextColor.begin() + index);
	selectedBackgroundColor.erase(selectedBackgroundColor.begin() + index);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Clear()
{
	if (GetCount() <= 0)
		return false;

	SendMessageW(handle, CB_RESETCONTENT, 0, 0);
	text.clear();
	textColor.clear();
	backgroundColor.clear();
	selectedTextColor.clear();
	selectedBackgroundColor.clear();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::GetText(int index, std::wstring &value) const
{
	if (!IsValidIndex(index))
		return false;

	value = text[index];
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ComboBox::GetIndex() const
{
	return static_cast<int>(SendMessageW(handle, CB_GETCURSEL, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetIndex(int index)
{
	return SendMessageW(handle, CB_SETCURSEL, index, 0) != CB_ERR;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::Deselect()
{
	SetIndex(-1);
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ComboBox::GetTopIndex() const
{
	return static_cast<int>(SendMessageW(handle, CB_GETTOPINDEX, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::SetTopIndex(int index)
{
	return SendMessageW(handle, CB_SETTOPINDEX, index, 0) != CB_ERR;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::IsDroppedDown() const
{
	return SendMessageW(handle, CB_GETDROPPEDSTATE, 0, 0);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_SETFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_KILLFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

void UI::Controls::ComboBox::DrawComboBox()
{
	WindowDC dc(*this);
	RECT rect;
	GetClientRect(handle, &rect);

	if (!IsEnabled())
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxDisabledTextColor, theme.ComboBoxDisabledBackgroundColor, theme.ComboBoxDisabledBorderColor, theme.ComboBoxDisabledBorderSize);
	else if (IsDroppedDown())
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxPushedTextColor, theme.ComboBoxPushedBackgroundColor, theme.ComboBoxPushedBorderColor, theme.ComboBoxPushedBorderSize);
	else if (hovering && IsFocused())
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxHoverFocusedTextColor, theme.ComboBoxHoverFocusedBackgroundColor, theme.ComboBoxHoverFocusedBorderColor, theme.ComboBoxHoverFocusedBorderSize);
	else if (hovering)
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxHoverTextColor, theme.ComboBoxHoverBackgroundColor, theme.ComboBoxHoverBorderColor, theme.ComboBoxHoverBorderSize);
	else if (IsFocused())
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxFocusedTextColor, theme.ComboBoxFocusedBackgroundColor, theme.ComboBoxFocusedBorderColor, theme.ComboBoxFocusedBorderSize);
	else
		rect = DrawFrameFillRect(dc, rect, theme.ComboBoxTextColor, theme.ComboBoxBackgroundColor, theme.ComboBoxBorderColor, theme.ComboBoxBorderSize);

	RECT buttonRect;
	GetClientRect(handle, &buttonRect);
	buttonRect.left = buttonRect.right - theme.ComboBoxButtonWidth;

	if (theme.IsDark)
		dc.InvertRect(buttonRect);

	HTHEME htheme = OpenThemeData(handle, L"COMBOBOX");

	if (htheme)
	{
		DrawThemeBackground(htheme, dc.GetHandle(), CP_DROPDOWNBUTTONRIGHT, CBXSR_NORMAL, &buttonRect, nullptr);
		CloseThemeData(htheme);
	}
	else
	{
		const int DFCS_MENUARROWDOWN = 16;
		DrawFrameControl(dc.GetHandle(), &buttonRect, DFC_MENU, DFCS_TRANSPARENT | DFCS_MENUARROWDOWN);
	}

	if (theme.IsDark)
		dc.InvertRect(buttonRect);

	std::wstring text;
	GetText(GetIndex(), text);
	rect.left = theme.Padding;
	rect.right = buttonRect.left;
	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect, DT_SINGLELINE | DT_VCENTER);

	if (theme.ComboBoxFocusRect && IsFocused() && IsFocusVisible() && !IsDroppedDown())
	{
		GetClientRect(handle, &rect);
		rect.right = buttonRect.left + 1;
		InflateRect(&rect, -theme.ComboBoxFocusRectPadding, -theme.ComboBoxFocusRectPadding);
		dc.DrawFocusRect(rect);
	}
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PaintDC(*this);
	DrawComboBox();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	DRAWITEMSTRUCT *item = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);

	if (item->itemState & ODS_COMBOBOXEDIT)
	{
		DrawComboBox();
		return true;
	}

	DC dc(item->hDC);
	RECT rect = item->rcItem;

	if (item->itemState & ODS_SELECTED)
		DrawFillRect(dc, rect, GetSelectedTextColor(item->itemID), GetSelectedBackgroundColor(item->itemID));
	else
		DrawFillRect(dc, rect, GetTextColor(item->itemID), GetBackgroundColor(item->itemID));

	std::wstring text;
	GetText(item->itemID, text);
	rect.left = theme.Padding - 1;
	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect, DT_SINGLELINE | DT_VCENTER);

	if (theme.ComboBoxListFocusRect && (item->itemState & ODS_FOCUS) && !(item->itemState & ODS_NOFOCUSRECT))
	{
		rect = item->rcItem;
		InflateRect(&rect, -theme.ComboBoxListFocusRectPadding, -theme.ComboBoxListFocusRectPadding);
		dc.DrawFocusRect(rect);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);
	dc.SetTextColor(theme.ComboBoxListTextColor);
	dc.SetBackgroundColor(theme.ComboBoxListBackgroundColor);
	dc.SetBrushColor(theme.ComboBoxListBackgroundColor);
	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	if (!hovering)
	{
		TRACKMOUSEEVENT event = {sizeof event};
		event.dwFlags = TME_LEAVE;
		event.hwndTrack = handle;
		TrackMouseEvent(&event);

		hovering = true;
		Invalidate();
	}

	return CallWindowProcW(OldWindowProc, handle, WM_MOUSEMOVE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ComboBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	hovering = false;

	if (!IsDroppedDown())
		Invalidate();

	return CallWindowProcW(OldWindowProc, handle, WM_MOUSELEAVE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ComboBox::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_SETFOCUS, std::bind(&ComboBox::OnSetFocus, this, _1, _2));
	AddMessage(WM_KILLFOCUS, std::bind(&ComboBox::OnKillFocus, this, _1, _2));
	AddMessage(WM_PAINT, std::bind(&ComboBox::OnPaint, this, _1, _2));
	AddMessage(WM_DRAWITEM, std::bind(&ComboBox::OnDrawItem, this, _1, _2));
	AddMessage(WM_CTLCOLORLISTBOX, std::bind(&ComboBox::OnCtlColor, this, _1, _2));
	AddMessage(WM_MOUSEMOVE, std::bind(&ComboBox::OnMouseMove, this, _1, _2));
	AddMessage(WM_MOUSELEAVE, std::bind(&ComboBox::OnMouseLeave, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////

HWND UI::Controls::ComboBox::InitListHandle() const
{
	COMBOBOXINFO info = {sizeof info};

	if (!GetComboBoxInfo(handle, &info))
		throw L"GetComboBoxInfo failed.";

	return info.hwndList;
}

////////////////////////////////////////////////////////////////////////////
