#ifndef UI_CONTROLS_COMBOBOX_H
#define UI_CONTROLS_COMBOBOX_H

// Includes ////////////////////////////////////////////////////////////////

#include "UI_Control.h"
#include "UI_Common.h"

// Classes /////////////////////////////////////////////////////////////////

namespace UI
{
	class Window;
	class Font;

	namespace Controls
	{
		class ComboBox : public Control
		{
		public:

			////////////////////////////////////////////////////////////////////
			// Public //////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////

			// Constants ///////////////////////////////////////////////////////

			static const DWORD DefaultStyle = WS_VSCROLL | WS_TABSTOP | CBS_DROPDOWNLIST;
			static const DWORD DefaultExStyle = 0;

			// Functions ///////////////////////////////////////////////////////

			ComboBox(const Window &window, DWORD style = DefaultStyle, DWORD exStyle = DefaultExStyle);

			bool SetSize(int width, int height, int listHeight);
			bool SetRightBottom(int right, int bottom, int listBottom);
			bool SetFont(const Font &font);
			COLORREF GetTextColor() const;
			bool SetTextColor(COLORREF color);
			COLORREF GetBackgroundColor() const;
			bool SetBackgroundColor(COLORREF color);
			COLORREF GetSelectedTextColor() const;
			bool SetSelectedTextColor(COLORREF color);
			COLORREF GetSelectedBackgroundColor() const;
			bool SetSelectedBackgroundColor(COLORREF color);
			COLORREF GetTextColor(int index) const;
			bool SetTextColor(int index, COLORREF color);
			COLORREF GetBackgroundColor(int index) const;
			bool SetBackgroundColor(int index, COLORREF color);
			COLORREF GetSelectedTextColor(int index) const;
			bool SetSelectedTextColor(int index, COLORREF color);
			COLORREF GetSelectedBackgroundColor(int index) const;
			bool SetSelectedBackgroundColor(int index, COLORREF color);
			int GetCount() const;
			bool IsValidIndex(int index) const;
			bool Add(const std::wstring &value);
			bool Insert(int index, const std::wstring &value);
			bool Delete(int index);
			bool Clear();
			bool GetText(int index, std::wstring &value) const;
			int GetIndex() const;
			bool SetIndex(int index);
			bool Deselect();
			int GetTopIndex() const;
			bool SetTopIndex(int index);
			bool IsDroppedDown() const;

			////////////////////////////////////////////////////////////////////

		private:

			////////////////////////////////////////////////////////////////////
			// Private /////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////

			// Functions ///////////////////////////////////////////////////////

			LRESULT OnSetFocus(WPARAM wParam, LPARAM lParam);
			LRESULT OnKillFocus(WPARAM wParam, LPARAM lParam);
			void DrawComboBox();
			LRESULT OnPaint(WPARAM wParam, LPARAM lParam);
			LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
			LRESULT OnCtlColor(WPARAM wParam, LPARAM lParam);
			LRESULT OnMouseMove(WPARAM wParam, LPARAM lParam);
			LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
			bool AddMessages();

			// Init Functions //////////////////////////////////////////////////

			HWND InitListHandle() const;

			// Variables ///////////////////////////////////////////////////////

			HWND listHandle;
			std::vector<std::wstring> text;
			std::vector<COLORREF> textColor;
			std::vector<COLORREF> backgroundColor;
			std::vector<COLORREF> selectedTextColor;
			std::vector<COLORREF> selectedBackgroundColor;
			bool hovering;

			////////////////////////////////////////////////////////////////////
		};
	}
}

////////////////////////////////////////////////////////////////////////////

#endif
