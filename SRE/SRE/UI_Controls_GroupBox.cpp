// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::GroupBox::GroupBox(const Window &window, const std::wstring &text, DWORD style, DWORD exStyle) :
	Control(window, L"BUTTON", text, style | BS_GROUPBOX, exStyle)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetPosition(int x, int y)
{
	if (!SetWindowPos(handle, nullptr, x, y - font.GetHeight() / 2, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE))
		return false;

	this->x = x;
	this->y = y;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetSize(int width, int height)
{
	if (!SetWindowPos(handle, nullptr, 0, 0, width, height + font.GetHeight() / 2, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE))
		return false;

	this->width = width;
	this->height = height;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetFont(const Font &font)
{
	if (!Control::SetFont(font))
		return false;

	if (!SetPosition(x, y))
		return false;

	if (!SetSize(width, height))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::GroupBox::GetTextColor() const
{
	return theme.GroupBoxTextColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetTextColor(COLORREF color)
{
	theme.GroupBoxTextColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::GroupBox::GetBackgroundColor() const
{
	return theme.GroupBoxBackgroundColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::SetBackgroundColor(COLORREF color)
{
	theme.GroupBoxBackgroundColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::GroupBox::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PaintDC dc(*this);
	RECT rect;
	GetClientRect(handle, &rect);
	rect.top += font.GetHeight() / 2;

	if (!IsEnabled())
		DrawFrameRect(dc, rect, theme.GroupBoxDisabledTextColor, theme.GroupBoxDisabledBackgroundColor, theme.GroupBoxDisabledBorderColor, theme.GroupBoxDisabledBorderSize);
	else
		DrawFrameRect(dc, rect, theme.GroupBoxTextColor, theme.GroupBoxBackgroundColor, theme.GroupBoxBorderColor, theme.GroupBoxBorderSize);

	GetClientRect(handle, &rect);
	rect.left += font.GetHeight() / 2;
	dc.SelectObject(font.GetHandle());
	dc.DrawText(text, rect);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::GroupBox::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);

	if (!IsEnabled())
	{
		dc.SetTextColor(theme.GroupBoxDisabledTextColor);
		dc.SetBackgroundColor(theme.GroupBoxDisabledBackgroundColor);
		dc.SetBrushColor(theme.GroupBoxDisabledBackgroundColor);
	}
	else
	{
		dc.SetTextColor(theme.GroupBoxTextColor);
		dc.SetBackgroundColor(theme.GroupBoxBackgroundColor);
		dc.SetBrushColor(theme.GroupBoxBackgroundColor);
	}

	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::GroupBox::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_PAINT, std::bind(&GroupBox::OnPaint, this, _1, _2));
	AddMessage(WM_CTLCOLORSTATIC, std::bind(&GroupBox::OnCtlColor, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////
