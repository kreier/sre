// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::ListBox::ListBox(const Window &window, DWORD style, DWORD exStyle) :
	Control(window, L"LISTBOX", L"", style | LBS_OWNERDRAWFIXED, exStyle),
	hovering(false)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetFont(const Font &font)
{
	if (SendMessageW(handle, LB_SETITEMHEIGHT, 0, font.GetHeight()) == LB_ERR)
		return false;

	return Control::SetFont(font);
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ListBox::GetTextColor() const
{
	return theme.ListBoxTextColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetTextColor(COLORREF color)
{
	theme.ListBoxTextColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ListBox::GetBackgroundColor() const
{
	return theme.ListBoxBackgroundColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetBackgroundColor(COLORREF color)
{
	theme.ListBoxBackgroundColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ListBox::GetSelectedTextColor() const
{
	return theme.ListBoxSelectedTextColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetSelectedTextColor(COLORREF color)
{
	theme.ListBoxSelectedTextColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ListBox::GetSelectedBackgroundColor() const
{
	return theme.ListBoxSelectedBackgroundColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetSelectedBackgroundColor(COLORREF color)
{
	theme.ListBoxSelectedBackgroundColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ListBox::GetTextColor(int index) const
{
	if (!IsValidIndex(index))
		return theme.ListBoxTextColor;

	return textColor[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetTextColor(int index, COLORREF color)
{
	if (!IsValidIndex(index))
		return false;

	textColor[index] = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ListBox::GetBackgroundColor(int index) const
{
	if (!IsValidIndex(index))
		return theme.ListBoxBackgroundColor;

	return backgroundColor[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetBackgroundColor(int index, COLORREF color)
{
	if (!IsValidIndex(index))
		return false;

	backgroundColor[index] = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ListBox::GetSelectedTextColor(int index) const
{
	if (!IsValidIndex(index))
		return theme.ListBoxSelectedTextColor;

	return selectedTextColor[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetSelectedTextColor(int index, COLORREF color)
{
	if (!IsValidIndex(index))
		return false;

	selectedTextColor[index] = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::ListBox::GetSelectedBackgroundColor(int index) const
{
	if (!IsValidIndex(index))
		return theme.ListBoxSelectedBackgroundColor;

	return selectedBackgroundColor[index];
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetSelectedBackgroundColor(int index, COLORREF color)
{
	if (!IsValidIndex(index))
		return false;

	selectedBackgroundColor[index] = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ListBox::GetCount() const
{
	return static_cast<int>(text.size());
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::IsValidIndex(int index) const
{
	return index >= 0 && index < GetCount();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Add(const std::wstring &value)
{
	SendMessageW(handle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(value.c_str()));
	text.push_back(value);
	textColor.push_back(theme.ListBoxTextColor);
	backgroundColor.push_back(theme.ListBoxBackgroundColor);
	selectedTextColor.push_back(theme.ListBoxSelectedTextColor);
	selectedBackgroundColor.push_back(theme.ListBoxSelectedBackgroundColor);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Insert(int index, const std::wstring &value)
{
	if (!IsValidIndex(index) && index != GetCount())
		return false;

	SendMessageW(handle, LB_INSERTSTRING, index, reinterpret_cast<LPARAM>(value.c_str()));
	text.insert(text.begin() + index, value);
	textColor.insert(textColor.begin() + index, theme.ListBoxTextColor);
	backgroundColor.insert(backgroundColor.begin() + index, theme.ListBoxBackgroundColor);
	selectedTextColor.insert(selectedTextColor.begin() + index, theme.ListBoxSelectedTextColor);
	selectedBackgroundColor.insert(selectedBackgroundColor.begin() + index, theme.ListBoxSelectedBackgroundColor);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Delete(int index)
{
	if (!IsValidIndex(index))
		return false;

	SendMessageW(handle, LB_DELETESTRING, index, 0);
	text.erase(text.begin() + index);
	textColor.erase(textColor.begin() + index);
	backgroundColor.erase(backgroundColor.begin() + index);
	selectedTextColor.erase(selectedTextColor.begin() + index);
	selectedBackgroundColor.erase(selectedBackgroundColor.begin() + index);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Clear()
{
	if (GetCount() <= 0)
		return false;

	SendMessageW(handle, LB_RESETCONTENT, 0, 0);
	text.clear();
	textColor.clear();
	backgroundColor.clear();
	selectedTextColor.clear();
	selectedBackgroundColor.clear();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::GetText(int index, std::wstring &value) const
{
	if (!IsValidIndex(index))
		return false;

	value = text[index];
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ListBox::GetIndex() const
{
	return static_cast<int>(SendMessageW(handle, LB_GETCURSEL, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetIndex(int index)
{
	return SendMessageW(handle, LB_SETCURSEL, index, 0) != LB_ERR;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::Deselect()
{
	SetIndex(-1);
	return true;
}

////////////////////////////////////////////////////////////////////////////

int UI::Controls::ListBox::GetTopIndex() const
{
	return static_cast<int>(SendMessageW(handle, LB_GETTOPINDEX, 0, 0));
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::SetTopIndex(int index)
{
	return SendMessageW(handle, LB_SETTOPINDEX, index, 0) != LB_ERR;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnSetFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_SETFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnKillFocus(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_KILLFOCUS, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	if (GetCount() <= 0)
	{
		PaintDC dc(*this);
		RECT rect;
		GetClientRect(handle, &rect);
		DrawFillRect(dc, rect, theme.ListBoxGrayTextColor, theme.ListBoxGrayBackgroundColor);
		rect.left = theme.Padding - theme.ListBoxPaddingSize;
		dc.SelectObject(font.GetHandle());
		dc.DrawText(Control::text, rect);

		if (theme.ListBoxFocusRect && IsFocused() && IsFocusVisible())
		{
			dc.SetTextColor(theme.ListBoxTextColor);
			dc.SetBackgroundColor(theme.ListBoxBackgroundColor);
			GetClientRect(handle, &rect);
			rect.bottom = font.GetHeight();
			InflateRect(&rect, -theme.ListBoxFocusRectPadding, -theme.ListBoxFocusRectPadding);
			dc.DrawFocusRect(rect);
		}

		return 0;
	}

	return CallWindowProcW(OldWindowProc, handle, WM_PAINT, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
	if (GetCount() <= 0)
		return true;

	DRAWITEMSTRUCT *item = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);
	DC dc(item->hDC);
	RECT rect = item->rcItem;

	if (!IsEnabled())
		DrawFillRect(dc, rect, theme.ListBoxDisabledTextColor, theme.ListBoxDisabledBackgroundColor);
	else if (item->itemState & ODS_SELECTED)
		DrawFillRect(dc, rect, GetSelectedTextColor(item->itemID), GetSelectedBackgroundColor(item->itemID));
	else
		DrawFillRect(dc, rect, GetTextColor(item->itemID), GetBackgroundColor(item->itemID));

	std::wstring text;
	GetText(item->itemID, text);
	rect.left = theme.Padding - theme.ListBoxPaddingSize;
	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect, DT_SINGLELINE | DT_VCENTER);

	if (theme.ListBoxFocusRect && (item->itemState & ODS_FOCUS) && !(item->itemState & ODS_NOFOCUSRECT))
	{
		rect = item->rcItem;
		InflateRect(&rect, -theme.ListBoxFocusRectPadding, -theme.ListBoxFocusRectPadding);
		dc.DrawFocusRect(rect);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnNcCalcSize(WPARAM wParam, LPARAM lParam)
{
	CallWindowProcW(OldWindowProc, handle, WM_NCCALCSIZE, wParam, lParam);

	if (wParam)
	{
		NCCALCSIZE_PARAMS *size = reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);
		InflateRect(&size->rgrc[0], -theme.ListBoxPaddingSize, -theme.ListBoxPaddingSize);
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnNcPaint(WPARAM wParam, LPARAM lParam)
{
	WindowDC dc(*this);
	RECT rect;
	GetWindowRect(handle, &rect);
	rect.right -= rect.left;
	rect.bottom -= rect.top;
	rect.left = 0;
	rect.top = 0;
	CallWindowProcW(OldWindowProc, handle, WM_NCPAINT, wParam, lParam);

	if (!IsEnabled())
		DrawFrameFrameRect(dc, rect, theme.ListBoxDisabledTextColor, theme.ListBoxDisabledBackgroundColor, theme.ListBoxDisabledBorderColor, theme.ListBoxDisabledBorderSize, theme.ListBoxPaddingSize);
	else if (hovering && IsFocused())
		DrawFrameFrameRect(dc, rect, theme.ListBoxTextColor, theme.ListBoxBackgroundColor, theme.ListBoxHoverFocusedBorderColor, theme.ListBoxHoverFocusedBorderSize, theme.ListBoxPaddingSize);
	else if (hovering)
		DrawFrameFrameRect(dc, rect, theme.ListBoxTextColor, theme.ListBoxBackgroundColor, theme.ListBoxHoverBorderColor, theme.ListBoxHoverBorderSize, theme.ListBoxPaddingSize);
	else if (IsFocused())
		DrawFrameFrameRect(dc, rect, theme.ListBoxTextColor, theme.ListBoxBackgroundColor, theme.ListBoxFocusedBorderColor, theme.ListBoxFocusedBorderSize, theme.ListBoxPaddingSize);
	else
		DrawFrameFrameRect(dc, rect, theme.ListBoxTextColor, theme.ListBoxBackgroundColor, theme.ListBoxBorderColor, theme.ListBoxBorderSize, theme.ListBoxPaddingSize);

	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);

	if (!IsEnabled())
	{
		dc.SetTextColor(theme.ListBoxDisabledTextColor);
		dc.SetBackgroundColor(theme.ListBoxDisabledBackgroundColor);
		dc.SetBrushColor(theme.ListBoxDisabledBackgroundColor);
	}
	else
	{
		dc.SetTextColor(theme.ListBoxTextColor);
		dc.SetBackgroundColor(theme.ListBoxBackgroundColor);
		dc.SetBrushColor(theme.ListBoxBackgroundColor);
	}

	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::ListBox::OnMouseMove(WPARAM wParam, LPARAM lParam)
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

LRESULT UI::Controls::ListBox::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	hovering = false;
	Invalidate();
	return CallWindowProcW(OldWindowProc, handle, WM_MOUSELEAVE, wParam, lParam);
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::ListBox::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_SETFOCUS, std::bind(&ListBox::OnSetFocus, this, _1, _2));
	AddMessage(WM_KILLFOCUS, std::bind(&ListBox::OnKillFocus, this, _1, _2));
	AddMessage(WM_PAINT, std::bind(&ListBox::OnPaint, this, _1, _2));
	AddMessage(WM_DRAWITEM, std::bind(&ListBox::OnDrawItem, this, _1, _2));
	AddMessage(WM_NCCALCSIZE, std::bind(&ListBox::OnNcCalcSize, this, _1, _2));
	AddMessage(WM_NCPAINT, std::bind(&ListBox::OnNcPaint, this, _1, _2));
	AddMessage(WM_CTLCOLORLISTBOX, std::bind(&ListBox::OnCtlColor, this, _1, _2));
	AddMessage(WM_MOUSEMOVE, std::bind(&ListBox::OnMouseMove, this, _1, _2));
	AddMessage(WM_MOUSELEAVE, std::bind(&ListBox::OnMouseLeave, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////
