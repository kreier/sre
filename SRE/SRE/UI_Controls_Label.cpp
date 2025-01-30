// Includes ////////////////////////////////////////////////////////////////

#include "UI.h"

// Functions ///////////////////////////////////////////////////////////////

UI::Controls::Label::Label(const Window &window, const std::wstring &text, DWORD style, DWORD exStyle) :
	Control(window, L"STATIC", text, style, exStyle)
{
	AddMessages();
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Label::SetFont(const Font &font)
{
	int width = font.GetWidth(text);

	if (!width)
		return false;

	int height = font.GetHeight();

	if (!height)
		return false;

	if (!SetSize(width, height))
		return false;

	if (!Control::SetFont(font))
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::Label::GetTextColor() const
{
	return theme.LabelTextColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Label::SetTextColor(COLORREF color)
{
	theme.LabelTextColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

COLORREF UI::Controls::Label::GetBackgroundColor() const
{
	return theme.LabelBackgroundColor;
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Label::SetBackgroundColor(COLORREF color)
{
	theme.LabelBackgroundColor = color;
	Invalidate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

LRESULT UI::Controls::Label::OnCtlColor(WPARAM wParam, LPARAM lParam)
{
	DC dc(wParam, false);

	if (!IsEnabled())
	{
		dc.SetTextColor(theme.LabelDisabledTextColor);
		dc.SetBackgroundColor(theme.LabelDisabledBackgroundColor);
		dc.SetBrushColor(theme.LabelDisabledBackgroundColor);
	}
	else
	{
		dc.SetTextColor(theme.LabelTextColor);
		dc.SetBackgroundColor(theme.LabelBackgroundColor);
		dc.SetBrushColor(theme.LabelBackgroundColor);
	}

	return reinterpret_cast<LRESULT>(dc.GetBrush());
}

////////////////////////////////////////////////////////////////////////////

bool UI::Controls::Label::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_CTLCOLORSTATIC, std::bind(&Label::OnCtlColor, this, _1, _2));
	return true;
}

////////////////////////////////////////////////////////////////////////////
