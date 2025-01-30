#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Includes ////////////////////////////////////////////////////////////////

#include "SRE.h"
#include "UI.h"

#include <shobjidl.h>

// Classes /////////////////////////////////////////////////////////////////

class MainWindow : public UI::Window
{
private:
	UI::Controls::Label gpuLabel;
	UI::Controls::ComboBox gpuComboBox;
	UI::Controls::GroupBox resolutionsGroupBox;
	UI::Controls::TextBox widthTextBox;
	UI::Controls::Label xLabel;
	UI::Controls::TextBox heightTextBox;
	UI::Controls::Button addButton;
	UI::Controls::Button deleteButton;
	UI::Controls::Button deleteAllButton;
	UI::Controls::Button defaultButton;
	UI::Controls::ListBox resolutionsListBox;
	UI::Controls::Button importButton;
	UI::Controls::Button exportButton;
	UI::Controls::Button copyButton;
	UI::Controls::Button pasteButton;
	UI::Controls::Button okButton;
	UI::Controls::Button cancelButton;

	SRE::GpuList gpuList;
	int gpuIndex;
	SRE::Gpu gpu;
	SRE::ResolutionList resolutionList;
	std::wstring copiedText;
	bool copied;

	static const std::wstring Name;
	static const std::wstring Title;

	static const std::wstring FontName;
	static const int FontSize;

	static const int Spacing;
	static const int LabelSpacing;

	static const int ClientWidth;
	static const int ClientHeight;

	static const std::wstring DefaultExtension;
	static const COMDLG_FILTERSPEC FileTypes[];

	LRESULT OnSize(WPARAM wParam, LPARAM lParam);
	LRESULT OnClose(WPARAM wParam, LPARAM lParam);
	LRESULT OnSysColorChange(WPARAM wParam, LPARAM lParam);
	LRESULT OnSettingChange(WPARAM wParam, LPARAM lParam);
	LRESULT OnDpiChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnThemeChanged(WPARAM wParam, LPARAM lParam);
	LRESULT OnClipboardUpdate(WPARAM wParam, LPARAM lParam);
	bool AddMessages();
	bool RefreshGpuComboBox();
	bool RefreshGpuComboBoxColors();
	bool RefreshResolutionTextBoxes();
	bool RefreshResolutionTextBoxesColors();
	bool RefreshAddButton();
	bool RefreshDeleteButton();
	bool RefreshDeleteAllButton();
	bool RefreshDefaultButton();
	bool RefreshResolutionsListBox(int top = 0);
	bool RefreshResolutionsListBoxColors();
	bool RefreshResolutionsListBoxIndex();
	bool RefreshCopyPasteButtons();
	void UpdateGpuList();
	void UpdateCopyText();
	void GpuComboBoxChange();
	void WidthTextBoxEnter();
	void WidthTextBoxChange();
	void WidthTextBoxExit();
	void HeightTextBoxEnter();
	void HeightTextBoxChange();
	void HeightTextBoxExit();
	void AddButtonClick();
	void DeleteButtonClick();
	void DeleteAllButtonClick();
	void DefaultButtonClick();
	void ResolutionsListBoxChange();
	void ImportButtonClick();
	void ExportButtonClick();
	void CopyButtonClick();
	void PasteButtonClick();
	void OkButtonClick();
	void CancelButtonClick();
	void EnterKeyPress();
	void EscKeyPress();
	bool AddCommands();
	bool InitializeTheme();
	bool InitializeControls();
	bool ScaleControls(bool resize = false);

public:
	MainWindow(const SRE::GpuList &gpuList);
};

////////////////////////////////////////////////////////////////////////////

#endif
