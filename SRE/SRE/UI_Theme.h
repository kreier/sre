#ifndef UI_THEME_H
#define UI_THEME_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_Common.h"

// Structs /////////////////////////////////////////////////////////////////

namespace UI
{
	class Dpi;

	struct Theme
	{
		bool IsDark;
		int Padding;
		COLORREF WindowBackgroundColor;
		COLORREF ButtonTextColor;
		COLORREF ButtonBackgroundColor;
		COLORREF ButtonBorderColor;
		int ButtonBorderSize;
		COLORREF ButtonFocusedTextColor;
		COLORREF ButtonFocusedBackgroundColor;
		COLORREF ButtonFocusedBorderColor;
		int ButtonFocusedBorderSize;
		bool ButtonFocusRect;
		int ButtonFocusRectPadding;
		COLORREF ButtonHoverTextColor;
		COLORREF ButtonHoverBackgroundColor;
		COLORREF ButtonHoverBorderColor;
		int ButtonHoverBorderSize;
		COLORREF ButtonHoverFocusedTextColor;
		COLORREF ButtonHoverFocusedBackgroundColor;
		COLORREF ButtonHoverFocusedBorderColor;
		int ButtonHoverFocusedBorderSize;
		COLORREF ButtonPushedTextColor;
		COLORREF ButtonPushedBackgroundColor;
		COLORREF ButtonPushedBorderColor;
		int ButtonPushedBorderSize;
		COLORREF ButtonDisabledTextColor;
		COLORREF ButtonDisabledBackgroundColor;
		COLORREF ButtonDisabledBorderColor;
		int ButtonDisabledBorderSize;
		int ListBoxPaddingSize;
		COLORREF ListBoxTextColor;
		COLORREF ListBoxBackgroundColor;
		COLORREF ListBoxSelectedTextColor;
		COLORREF ListBoxSelectedBackgroundColor;
		COLORREF ListBoxGrayTextColor;
		COLORREF ListBoxGrayBackgroundColor;
		COLORREF ListBoxGraySelectedTextColor;
		COLORREF ListBoxGraySelectedBackgroundColor;
		COLORREF ListBoxBorderColor;
		int ListBoxBorderSize;
		COLORREF ListBoxFocusedBorderColor;
		int ListBoxFocusedBorderSize;
		bool ListBoxFocusRect;
		int ListBoxFocusRectPadding;
		COLORREF ListBoxHoverBorderColor;
		int ListBoxHoverBorderSize;
		COLORREF ListBoxHoverFocusedBorderColor;
		int ListBoxHoverFocusedBorderSize;
		COLORREF ListBoxDisabledTextColor;
		COLORREF ListBoxDisabledBackgroundColor;
		COLORREF ListBoxDisabledBorderColor;
		int ListBoxDisabledBorderSize;
		int ComboBoxButtonWidth;
		COLORREF ComboBoxTextColor;
		COLORREF ComboBoxBackgroundColor;
		COLORREF ComboBoxBorderColor;
		int ComboBoxBorderSize;
		COLORREF ComboBoxFocusedTextColor;
		COLORREF ComboBoxFocusedBackgroundColor;
		COLORREF ComboBoxFocusedBorderColor;
		int ComboBoxFocusedBorderSize;
		bool ComboBoxFocusRect;
		int ComboBoxFocusRectPadding;
		COLORREF ComboBoxHoverTextColor;
		COLORREF ComboBoxHoverBackgroundColor;
		COLORREF ComboBoxHoverBorderColor;
		int ComboBoxHoverBorderSize;
		COLORREF ComboBoxHoverFocusedTextColor;
		COLORREF ComboBoxHoverFocusedBackgroundColor;
		COLORREF ComboBoxHoverFocusedBorderColor;
		int ComboBoxHoverFocusedBorderSize;
		COLORREF ComboBoxPushedTextColor;
		COLORREF ComboBoxPushedBackgroundColor;
		COLORREF ComboBoxPushedBorderColor;
		int ComboBoxPushedBorderSize;
		COLORREF ComboBoxDisabledTextColor;
		COLORREF ComboBoxDisabledBackgroundColor;
		COLORREF ComboBoxDisabledBorderColor;
		int ComboBoxDisabledBorderSize;
		COLORREF ComboBoxListTextColor;
		COLORREF ComboBoxListBackgroundColor;
		COLORREF ComboBoxListSelectedTextColor;
		COLORREF ComboBoxListSelectedBackgroundColor;
		COLORREF ComboBoxListGrayTextColor;
		COLORREF ComboBoxListGrayBackgroundColor;
		COLORREF ComboBoxListGraySelectedTextColor;
		COLORREF ComboBoxListGraySelectedBackgroundColor;
		bool ComboBoxListFocusRect;
		int ComboBoxListFocusRectPadding;
		COLORREF GroupBoxTextColor;
		COLORREF GroupBoxBackgroundColor;
		COLORREF GroupBoxBorderColor;
		int GroupBoxBorderSize;
		COLORREF GroupBoxDisabledTextColor;
		COLORREF GroupBoxDisabledBackgroundColor;
		COLORREF GroupBoxDisabledBorderColor;
		int GroupBoxDisabledBorderSize;
		COLORREF LabelTextColor;
		COLORREF LabelBackgroundColor;
		COLORREF LabelDisabledTextColor;
		COLORREF LabelDisabledBackgroundColor;
		COLORREF TextBoxTextColor;
		COLORREF TextBoxBackgroundColor;
		COLORREF TextBoxInvalidTextColor;
		COLORREF TextBoxInvalidBackgroundColor;
		COLORREF TextBoxBorderColor;
		int TextBoxBorderSize;
		COLORREF TextBoxFocusedBorderColor;
		int TextBoxFocusedBorderSize;
		COLORREF TextBoxHoverBorderColor;
		int TextBoxHoverBorderSize;
		COLORREF TextBoxHoverFocusedBorderColor;
		int TextBoxHoverFocusedBorderSize;
		COLORREF TextBoxDisabledTextColor;
		COLORREF TextBoxDisabledBackgroundColor;
		COLORREF TextBoxDisabledBorderColor;
		int TextBoxDisabledBorderSize;
	};

	struct LightTheme : public Theme
	{
		LightTheme(const Dpi &dpi);
	};

	struct DarkTheme : public Theme
	{
		DarkTheme(const Dpi &dpi);
	};

	struct HighContrastTheme : public Theme
	{
		HighContrastTheme(const Dpi &dpi);
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
