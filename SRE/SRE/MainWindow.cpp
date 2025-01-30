// Includes ////////////////////////////////////////////////////////////////

#include "MainWindow.h"

// Constants ///////////////////////////////////////////////////////////////

const std::wstring MainWindow::Name = L"MainWindow";
const std::wstring MainWindow::Title = L"Scaled Resolution Editor Beta 2 by ToastyX";

const std::wstring MainWindow::FontName = L"Tahoma";
const int MainWindow::FontSize = 16;

const int MainWindow::Spacing = FontSize / 2;
const int MainWindow::LabelSpacing = FontSize / 4;

const int MainWindow::ClientWidth = FontSize * 25;
const int MainWindow::ClientHeight = FontSize * 40;

const std::wstring MainWindow::DefaultExtension = L"txt";

const COMDLG_FILTERSPEC MainWindow::FileTypes[] =
{
	{ L"Text Files", L"*.txt" },
	{ L"All Files", L"*.*" }
};

// Functions ///////////////////////////////////////////////////////////////

LRESULT MainWindow::OnSize(WPARAM wParam, LPARAM lParam)
{
	ScaleControls();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnClose(WPARAM wParam, LPARAM lParam)
{
	UpdateGpuList();

	if (gpuList.IsSaved())
	{
		CancelButtonClick();
		return 0;
	}

	switch (MessageBoxW(GetHandle(), L"Do you want to save changes?", L"", MB_YESNOCANCEL))
	{
		case IDYES:
			OkButtonClick();
			return 0;

		case IDNO:
			CancelButtonClick();
			return 0;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnSysColorChange(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnSettingChange(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		std::wstring setting = reinterpret_cast<wchar_t *>(lParam);

		if (setting == L"ImmersiveColorSet")
			InitializeTheme();
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnDpiChanged(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	ScaleControls();
	RECT *rect = reinterpret_cast<RECT *>(lParam);
	SetPosition(rect->left, rect->top);
	SetRightBottom(rect->right, rect->bottom);
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnThemeChanged(WPARAM wParam, LPARAM lParam)
{
	InitializeTheme();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

LRESULT MainWindow::OnClipboardUpdate(WPARAM wParam, LPARAM lParam)
{
	UpdateCopyText();
	RefreshCopyPasteButtons();
	return 0;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::AddMessages()
{
	using namespace std::placeholders;
	AddMessage(WM_SIZE, std::bind(&MainWindow::OnSize, this, _1, _2));
	AddMessage(WM_CLOSE, std::bind(&MainWindow::OnClose, this, _1, _2));
	AddMessage(WM_SYSCOLORCHANGE, std::bind(&MainWindow::OnSysColorChange, this, _1, _2));
	AddMessage(WM_SETTINGCHANGE, std::bind(&MainWindow::OnSettingChange, this, _1, _2));
	AddMessage(WM_DPICHANGED, std::bind(&MainWindow::OnDpiChanged, this, _1, _2));
	AddMessage(WM_THEMECHANGED, std::bind(&MainWindow::OnThemeChanged, this, _1, _2));
	AddMessage(WM_CLIPBOARDUPDATE, std::bind(&MainWindow::OnClipboardUpdate, this, _1, _2));
	AddClipboardFormatListener(GetHandle());
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshGpuComboBox()
{
	SRE::Gpu gpu;
	gpuComboBox.BeginUpdate();
	gpuComboBox.Clear();

	for (int index = 0; gpuList.Get(index, gpu); ++index)
	{
		std::wstring text = gpu.GetName();

		if (gpu.IsActive())
			text += L" (active)";

		if (gpu.IsModified())
			text += L"*";

		gpuComboBox.Add(text);
	}

	RefreshGpuComboBoxColors();
	gpuComboBox.EndUpdate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshGpuComboBoxColors()
{
	SRE::Gpu gpu;
	UI::Theme theme = GetTheme();

	for (int index = 0; gpuList.Get(index, gpu); ++index)
	{
		if (gpu.IsActive())
		{
			gpuComboBox.SetTextColor(index, theme.ComboBoxListTextColor);
			gpuComboBox.SetBackgroundColor(index, theme.ComboBoxListBackgroundColor);
			gpuComboBox.SetSelectedTextColor(index, theme.ComboBoxListSelectedTextColor);
			gpuComboBox.SetSelectedBackgroundColor(index, theme.ComboBoxListSelectedBackgroundColor);
		}
		else
		{
			gpuComboBox.SetTextColor(index, theme.ComboBoxListGrayTextColor);
			gpuComboBox.SetBackgroundColor(index, theme.ComboBoxListGrayBackgroundColor);
			gpuComboBox.SetSelectedTextColor(index, theme.ComboBoxListGraySelectedTextColor);
			gpuComboBox.SetSelectedBackgroundColor(index, theme.ComboBoxListGraySelectedBackgroundColor);
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshResolutionTextBoxes()
{
	SRE::Resolution resolution;

	if (!resolutionList.Get(resolutionsListBox.GetIndex(), resolution))
	{
		widthTextBox.ClearText();
		heightTextBox.ClearText();
	}
	else
	{
		widthTextBox.SetInteger(resolution.width);
		heightTextBox.SetInteger(resolution.height);
	}

	RefreshResolutionTextBoxesColors();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshResolutionTextBoxesColors()
{
	int value;
	UI::Theme theme = GetTheme();

	if (widthTextBox.GetInteger(value) && resolutionList.IsValidWidth(value))
	{
		widthTextBox.SetTextColor(theme.TextBoxTextColor);
		widthTextBox.SetBackgroundColor(theme.TextBoxBackgroundColor);
	}
	else
	{
		widthTextBox.SetTextColor(theme.TextBoxInvalidTextColor);
		widthTextBox.SetBackgroundColor(theme.TextBoxInvalidBackgroundColor);
	}

	if (heightTextBox.GetInteger(value) && resolutionList.IsValidHeight(value))
	{
		heightTextBox.SetTextColor(theme.TextBoxTextColor);
		heightTextBox.SetBackgroundColor(theme.TextBoxBackgroundColor);
	}
	else
	{
		heightTextBox.SetTextColor(theme.TextBoxInvalidTextColor);
		heightTextBox.SetBackgroundColor(theme.TextBoxInvalidBackgroundColor);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshAddButton()
{
	SRE::Resolution resolution;

	if (!widthTextBox.GetInteger(resolution.width) || !resolutionList.IsValidWidth(resolution.width))
	{
		addButton.Disable();
		return true;
	}

	if (!heightTextBox.GetInteger(resolution.height) || !resolutionList.IsValidHeight(resolution.height))
	{
		addButton.Disable();
		return true;
	}

	if (resolutionList.IsAdded(resolution))
	{
		addButton.Disable();
		return true;
	}

	addButton.Enable();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshDeleteButton()
{
	if (resolutionsListBox.GetIndex() < 0)
		deleteButton.Disable();
	else
		deleteButton.Enable();

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshDeleteAllButton()
{
	if (resolutionList.GetCount() <= 0)
		deleteAllButton.Disable();
	else
		deleteAllButton.Enable();

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshDefaultButton()
{
	SRE::ResolutionList defaultResolutionList;

	if (!gpu.GetDefaultResolutionList(defaultResolutionList))
		return false;

	if (resolutionList == defaultResolutionList)
		defaultButton.Disable();
	else
		defaultButton.Enable();

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshResolutionsListBox(int top)
{
	SRE::Resolution resolution;
	resolutionsListBox.BeginUpdate();
	resolutionsListBox.Clear();

	for (int index = 0; resolutionList.Get(index, resolution); ++index)
		resolutionsListBox.Add(resolution.GetText());

	if (top > 0)
		resolutionsListBox.SetTopIndex(top);

	RefreshResolutionsListBoxColors();
	resolutionsListBox.EndUpdate();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshResolutionsListBoxColors()
{
	SRE::Resolution resolution;
	UI::Theme theme = GetTheme();

	for (int index = 0; resolutionList.Get(index, resolution); ++index)
	{
		if (resolution.added)
		{
			resolutionsListBox.SetTextColor(index, theme.ListBoxTextColor);
			resolutionsListBox.SetBackgroundColor(index, theme.ListBoxBackgroundColor);
			resolutionsListBox.SetSelectedTextColor(index, theme.ListBoxSelectedTextColor);
			resolutionsListBox.SetSelectedBackgroundColor(index, theme.ListBoxSelectedBackgroundColor);
		}
		else
		{
			resolutionsListBox.SetTextColor(index, theme.ListBoxGrayTextColor);
			resolutionsListBox.SetBackgroundColor(index, theme.ListBoxGrayBackgroundColor);
			resolutionsListBox.SetSelectedTextColor(index, theme.ListBoxGraySelectedTextColor);
			resolutionsListBox.SetSelectedBackgroundColor(index, theme.ListBoxGraySelectedBackgroundColor);
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshResolutionsListBoxIndex()
{
	SRE::Resolution resolution;

	if (!widthTextBox.GetInteger(resolution.width) || !heightTextBox.GetInteger(resolution.height))
		resolutionsListBox.Deselect();
	else
		resolutionsListBox.SetIndex(resolutionList.GetIndex(resolution));

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::RefreshCopyPasteButtons()
{
	if (!copied)
	{
		copyButton.Enable();
		pasteButton.Disable();
		return true;
	}

	std::wstring text;
	resolutionList.GetText(text);

	if (text == copiedText)
		copyButton.Disable();
	else
		copyButton.Enable();

	SRE::ResolutionList pasteResolutionList = resolutionList;
	pasteResolutionList.SetText(copiedText);

	if (resolutionList == pasteResolutionList)
		pasteButton.Disable();
	else
		pasteButton.Enable();

	return true;
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::UpdateGpuList()
{
	if (!gpuList.IsValidIndex(gpuIndex))
		return;

	if (!gpu.SetResolutionList(resolutionList))
		return;

	gpuList.Set(gpuIndex, gpu);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::UpdateCopyText()
{
	UI::Clipboard clipboard(*this);
	copied = clipboard.GetText(copiedText);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::GpuComboBoxChange()
{
	UpdateGpuList();
	gpuIndex = gpuComboBox.GetIndex();

	if (!gpuList.Get(gpuIndex, gpu))
		return;

	if (!gpu.GetResolutionList(resolutionList))
		return;

	RefreshResolutionsListBox();
	RefreshResolutionTextBoxes();
	RefreshAddButton();
	RefreshDeleteButton();
	RefreshDeleteAllButton();
	RefreshDefaultButton();
	RefreshCopyPasteButtons();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::WidthTextBoxEnter()
{
	addButton.SetDefault(true);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::WidthTextBoxChange()
{
	RefreshResolutionTextBoxesColors();
	RefreshResolutionsListBoxIndex();
	RefreshAddButton();
	RefreshDeleteButton();
	addButton.SetDefault(true);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::WidthTextBoxExit()
{
	int width;

	if (!widthTextBox.GetInteger(width) || !resolutionList.IsValidWidth(width))
		return;

	widthTextBox.SetInteger(width);
	addButton.SetDefault(false);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::HeightTextBoxEnter()
{
	addButton.SetDefault(true);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::HeightTextBoxChange()
{
	RefreshResolutionTextBoxesColors();
	RefreshResolutionsListBoxIndex();
	RefreshAddButton();
	RefreshDeleteButton();
	addButton.SetDefault(true);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::HeightTextBoxExit()
{
	int height;

	if (!heightTextBox.GetInteger(height) || !resolutionList.IsValidHeight(height))
		return;

	heightTextBox.SetInteger(height);
	addButton.SetDefault(false);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::AddButtonClick()
{
	SRE::Resolution resolution;

	if (!widthTextBox.GetInteger(resolution.width))
		return;

	if (!heightTextBox.GetInteger(resolution.height))
		return;

	if (!resolutionList.Add(resolution))
		return;

	RefreshResolutionsListBox(resolutionsListBox.GetTopIndex());
	RefreshResolutionsListBoxIndex();
	RefreshAddButton();
	RefreshDeleteButton();
	RefreshDeleteAllButton();
	RefreshDefaultButton();
	RefreshCopyPasteButtons();
	widthTextBox.Select();
	widthTextBox.SetFocus();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::DeleteButtonClick()
{
	int index = resolutionsListBox.GetIndex();

	if (!resolutionList.Delete(index))
		return;

	RefreshResolutionsListBox(resolutionsListBox.GetTopIndex());
	resolutionsListBox.SetIndex(index);
	RefreshResolutionTextBoxes();
	RefreshAddButton();
	RefreshDeleteButton();
	RefreshDeleteAllButton();
	RefreshDefaultButton();
	RefreshCopyPasteButtons();

	if (!deleteButton.IsEnabled())
		widthTextBox.SetFocus();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::DeleteAllButtonClick()
{
	if (!resolutionList.Clear())
		return;

	RefreshResolutionsListBox();
	RefreshResolutionTextBoxes();
	RefreshAddButton();
	RefreshDeleteButton();
	RefreshDeleteAllButton();
	RefreshDefaultButton();
	RefreshCopyPasteButtons();
	widthTextBox.SetFocus();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::DefaultButtonClick()
{
	if (!gpu.GetDefaultResolutionList(resolutionList))
		return;

	RefreshResolutionsListBox();
	RefreshResolutionTextBoxes();
	RefreshAddButton();
	RefreshDeleteButton();
	RefreshDeleteAllButton();
	RefreshDefaultButton();
	RefreshCopyPasteButtons();
	widthTextBox.SetFocus();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::ResolutionsListBoxChange()
{
	RefreshResolutionTextBoxes();
	RefreshAddButton();
	RefreshDeleteButton();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::ImportButtonClick()
{
	bool initialized = false;
	IFileOpenDialog *openDialog = nullptr;
	IShellItem *result = nullptr;
	wchar_t *file = nullptr;

	try
	{
		if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
			throw L"CoInitializeEx failed.";

		initialized = true;

		if (!SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&openDialog))))
			throw L"CoCreateInstance failed.";

		if (!SUCCEEDED(openDialog->SetTitle(L"Import")))
			throw L"IFileSaveDialog::SetTitle failed.";

		if (!SUCCEEDED(openDialog->SetOkButtonLabel(L"Import")))
			throw L"IFileSaveDialog::SetOkButtonLabel failed.";

		if (!SUCCEEDED(openDialog->SetDefaultExtension(DefaultExtension.c_str())))
			throw L"IFileSaveDialog::SetDefaultExtension failed.";

		if (!SUCCEEDED(openDialog->SetFileTypes(ARRAYSIZE(FileTypes), FileTypes)))
			throw L"IFileSaveDialog::SetFileTypes failed.";

		if (SUCCEEDED(openDialog->Show(GetHandle())))
		{
			if (!SUCCEEDED(openDialog->GetResult(&result)))
				throw L"IFileSaveDialog::GetResult failed.";

			if (!SUCCEEDED(result->GetDisplayName(SIGDN_FILESYSPATH, &file)))
				throw L"IShellItem::GetDisplayName failed.";

			if (!resolutionList.Import(file))
				throw L"Failed to import file.";

			RefreshResolutionsListBox();
			RefreshResolutionTextBoxes();
			RefreshAddButton();
			RefreshDeleteButton();
			RefreshDeleteAllButton();
			RefreshDefaultButton();
			RefreshCopyPasteButtons();
		}
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(GetHandle(), message, L"Error", MB_ICONERROR);
	}

	if (file)
		CoTaskMemFree(file);

	if (result)
		result->Release();

	if (openDialog)
		openDialog->Release();

	if (initialized)
		CoUninitialize();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::ExportButtonClick()
{
	bool initialized = false;
	IFileSaveDialog *saveDialog = nullptr;
	IShellItem *result = nullptr;
	wchar_t *file = nullptr;

	try
	{
		if (!SUCCEEDED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
			throw L"CoInitializeEx failed.";

		initialized = true;

		if (!SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&saveDialog))))
			throw L"CoCreateInstance failed.";

		if (!SUCCEEDED(saveDialog->SetTitle(L"Export")))
			throw L"IFileSaveDialog::SetTitle failed.";

		if (!SUCCEEDED(saveDialog->SetOkButtonLabel(L"Export")))
			throw L"IFileSaveDialog::SetOkButtonLabel failed.";

		if (!SUCCEEDED(saveDialog->SetDefaultExtension(DefaultExtension.c_str())))
			throw L"IFileSaveDialog::SetDefaultExtension failed.";

		if (!SUCCEEDED(saveDialog->SetFileTypes(ARRAYSIZE(FileTypes), FileTypes)))
			throw L"IFileSaveDialog::SetFileTypes failed.";

		if (SUCCEEDED(saveDialog->Show(GetHandle())))
		{
			if (!SUCCEEDED(saveDialog->GetResult(&result)))
				throw L"IFileSaveDialog::GetResult failed.";

			if (!SUCCEEDED(result->GetDisplayName(SIGDN_FILESYSPATH, &file)))
				throw L"IShellItem::GetDisplayName failed.";

			if (!resolutionList.Export(file))
				throw L"Failed to export file.";
		}
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(GetHandle(), message, L"Error", MB_ICONERROR);
	}

	if (file)
		CoTaskMemFree(file);

	if (result)
		result->Release();

	if (saveDialog)
		saveDialog->Release();

	if (initialized)
		CoUninitialize();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::CopyButtonClick()
{
	std::wstring text;

	if (!resolutionList.GetText(text))
		return;

	UI::Clipboard clipboard(*this);

	if (!clipboard.SetText(text))
		return;

	copiedText = text;
	copied = true;
	RefreshCopyPasteButtons();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::PasteButtonClick()
{
	UI::Clipboard clipboard(*this);
	std::wstring text;

	if (!clipboard.GetText(text))
		return;

	if (!resolutionList.SetText(text))
		return;

	RefreshResolutionsListBox();
	RefreshResolutionTextBoxes();
	RefreshAddButton();
	RefreshDeleteButton();
	RefreshDeleteAllButton();
	RefreshDefaultButton();
	RefreshCopyPasteButtons();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::OkButtonClick()
{
	UpdateGpuList();

	int count = gpuList.Save();

	if (count <= 0)
		throw L"Failed to save changes.";

	if (count != gpuList.GetCount())
		throw L"Failed to save some changes.";

	Close(0);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::CancelButtonClick()
{
	Close(0);
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::EnterKeyPress()
{
	if (!widthTextBox.IsFocused() && !heightTextBox.IsFocused())
		return;

	AddButtonClick();
}

////////////////////////////////////////////////////////////////////////////

void MainWindow::EscKeyPress()
{
	CancelButtonClick();
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::AddCommands()
{
	gpuComboBox.AddCommand(CBN_SELCHANGE, std::bind(&MainWindow::GpuComboBoxChange, this));
	widthTextBox.AddCommand(EN_SETFOCUS, std::bind(&MainWindow::WidthTextBoxEnter, this));
	widthTextBox.AddCommand(EN_CHANGE, std::bind(&MainWindow::WidthTextBoxChange, this));
	widthTextBox.AddCommand(EN_KILLFOCUS, std::bind(&MainWindow::WidthTextBoxExit, this));
	heightTextBox.AddCommand(EN_SETFOCUS, std::bind(&MainWindow::HeightTextBoxEnter, this));
	heightTextBox.AddCommand(EN_CHANGE, std::bind(&MainWindow::HeightTextBoxChange, this));
	heightTextBox.AddCommand(EN_KILLFOCUS, std::bind(&MainWindow::HeightTextBoxExit, this));
	addButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::AddButtonClick, this));
	deleteButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::DeleteButtonClick, this));
	deleteAllButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::DeleteAllButtonClick, this));
	defaultButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::DefaultButtonClick, this));
	resolutionsListBox.AddCommand(LBN_SELCHANGE, std::bind(&MainWindow::ResolutionsListBoxChange, this));
	importButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::ImportButtonClick, this));
	exportButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::ExportButtonClick, this));
	copyButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::CopyButtonClick, this));
	pasteButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::PasteButtonClick, this));
	okButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::OkButtonClick, this));
	cancelButton.AddCommand(BN_CLICKED, std::bind(&MainWindow::CancelButtonClick, this));
	AddCommand(IDOK, std::bind(&MainWindow::EnterKeyPress, this));
	AddCommand(IDCANCEL, std::bind(&MainWindow::EscKeyPress, this));
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::InitializeTheme()
{
	UI::Dpi dpi = GetDpi();
	UI::SystemTheme systemTheme;
	UI::Theme theme;

	if (systemTheme.IsHighContrast())
		theme = UI::HighContrastTheme(dpi);
	else if (systemTheme.IsDarkMode())
		theme = UI::DarkTheme(dpi);
	else
		theme = UI::LightTheme(dpi);

	SetTheme(theme);
	gpuLabel.SetTheme(theme);
	gpuComboBox.SetTheme(theme);
	resolutionsGroupBox.SetTheme(theme);
	widthTextBox.SetTheme(theme);
	xLabel.SetTheme(theme);
	heightTextBox.SetTheme(theme);
	addButton.SetTheme(theme);
	deleteButton.SetTheme(theme);
	deleteAllButton.SetTheme(theme);
	defaultButton.SetTheme(theme);
	resolutionsListBox.SetTheme(theme);
	importButton.SetTheme(theme);
	exportButton.SetTheme(theme);
	copyButton.SetTheme(theme);
	pasteButton.SetTheme(theme);
	okButton.SetTheme(theme);
	cancelButton.SetTheme(theme);

	RefreshGpuComboBoxColors();
	RefreshResolutionTextBoxesColors();
	RefreshResolutionsListBoxColors();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::InitializeControls()
{
	RefreshGpuComboBox();
	widthTextBox.SetMaxLength(5);
	heightTextBox.SetMaxLength(5);
	resolutionsListBox.SetText(L"No resolutions");
	UpdateCopyText();
	gpuComboBox.SetIndex(0);
	GpuComboBoxChange();
	widthTextBox.SetFocus();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool MainWindow::ScaleControls(bool resize)
{
	UI::Dpi dpi = GetDpi();
	UI::Font font(FontName, dpi.Scale(FontSize));
	UI::Theme theme = GetTheme();

	const int Spacing = dpi.Scale(this->Spacing);
	const int LabelSpacing = dpi.Scale(this->LabelSpacing);
	const int LinePaddingHeight = theme.Padding - 1;
	const int LineHeight = LinePaddingHeight + font.GetHeight() + LinePaddingHeight + 1;
	const int ResolutionTextBoxWidth = theme.Padding + font.GetWidth(L"00000") + theme.Padding;
	const int ButtonSpacing = dpi.Scale(2);
	const int ResolutionsListBoxMinRows = 5;
	const int ResolutionsListBoxMinHeight = theme.ListBoxPaddingSize + font.GetHeight() * ResolutionsListBoxMinRows + theme.ListBoxPaddingSize;
	const int ButtonWidth = font.GetHeight() * 5;
	const int ButtonHeight = LineHeight;

	SetIcon(L"ICON", 192);

	gpuLabel.SetFont(font);
	gpuLabel.SetPosition(Spacing, Spacing + LinePaddingHeight);

	gpuComboBox.SetFont(font);
	gpuComboBox.SetPosition(gpuLabel.GetRight() + LabelSpacing, gpuLabel.GetY() - LinePaddingHeight);
	gpuComboBox.SetRightBottom(GetClientWidth() - Spacing, gpuComboBox.GetY() + LineHeight, GetClientHeight() - Spacing);

	resolutionsGroupBox.SetFont(font);
	resolutionsGroupBox.SetPosition(gpuLabel.GetX(), gpuComboBox.GetY() + LineHeight + Spacing + Spacing);
	resolutionsGroupBox.SetRightBottom(GetClientWidth() - Spacing, GetClientHeight() - Spacing - ButtonHeight - Spacing);

	widthTextBox.SetFont(font);
	widthTextBox.SetPosition(resolutionsGroupBox.GetX() + theme.GroupBoxBorderSize + Spacing, resolutionsGroupBox.GetY() + theme.GroupBoxBorderSize + Spacing + Spacing);
	widthTextBox.SetSize(ResolutionTextBoxWidth, LineHeight);

	xLabel.SetFont(font);
	xLabel.SetPosition(widthTextBox.GetRight() + LabelSpacing, widthTextBox.GetY() + LinePaddingHeight);

	heightTextBox.SetFont(font);
	heightTextBox.SetPosition(xLabel.GetRight() + LabelSpacing, xLabel.GetY() - LinePaddingHeight);
	heightTextBox.SetSize(ResolutionTextBoxWidth, LineHeight);

	addButton.SetFont(font);
	addButton.SetPosition(heightTextBox.GetRight() + ButtonSpacing, heightTextBox.GetY());
	addButton.SetSize(LineHeight, LineHeight);

	deleteButton.SetFont(font);
	deleteButton.SetPosition(addButton.GetRight() + ButtonSpacing, addButton.GetY());
	deleteButton.SetSize(LineHeight, LineHeight);

	defaultButton.SetFont(font);
	defaultButton.SetSize(Spacing + font.GetWidth(defaultButton.GetText()) + Spacing, LineHeight);
	defaultButton.SetPosition(resolutionsGroupBox.GetRight() - theme.GroupBoxBorderSize - Spacing - defaultButton.GetWidth(), deleteButton.GetY());

	deleteAllButton.SetFont(font);
	deleteAllButton.SetSize(Spacing + font.GetWidth(deleteAllButton.GetText()) + Spacing, LineHeight);
	deleteAllButton.SetPosition(defaultButton.GetX() - ButtonSpacing - deleteAllButton.GetWidth(), defaultButton.GetY());

	resolutionsListBox.SetFont(font);
	resolutionsListBox.SetPosition(widthTextBox.GetX(), widthTextBox.GetBottom() + Spacing);
	resolutionsListBox.SetRightBottom(resolutionsGroupBox.GetRight() - theme.GroupBoxBorderSize - Spacing, resolutionsGroupBox.GetBottom() - theme.GroupBoxBorderSize - Spacing - LineHeight - Spacing);

	importButton.SetFont(font);
	importButton.SetPosition(resolutionsListBox.GetX(), resolutionsListBox.GetBottom() + Spacing);
	importButton.SetSize(Spacing + font.GetWidth(importButton.GetText()) + Spacing, LineHeight);

	exportButton.SetFont(font);
	exportButton.SetPosition(importButton.GetRight() + ButtonSpacing, importButton.GetY());
	exportButton.SetSize(importButton.GetWidth(), LineHeight);

	pasteButton.SetFont(font);
	pasteButton.SetSize(Spacing + font.GetWidth(pasteButton.GetText()) + Spacing, LineHeight);
	pasteButton.SetPosition(resolutionsGroupBox.GetRight() - theme.GroupBoxBorderSize - Spacing - pasteButton.GetWidth(), exportButton.GetY());

	copyButton.SetFont(font);
	copyButton.SetSize(pasteButton.GetWidth(), LineHeight);
	copyButton.SetPosition(pasteButton.GetX() - ButtonSpacing - copyButton.GetWidth(), pasteButton.GetY());

	cancelButton.SetFont(font);
	cancelButton.SetPosition(resolutionsGroupBox.GetRight() - ButtonWidth, resolutionsGroupBox.GetBottom() + Spacing);
	cancelButton.SetSize(ButtonWidth, ButtonHeight);

	okButton.SetFont(font);
	okButton.SetPosition(cancelButton.GetX() - Spacing - ButtonWidth, cancelButton.GetY());
	okButton.SetSize(ButtonWidth, ButtonHeight);

	int minWidth = deleteButton.GetRight() + ButtonSpacing + deleteAllButton.GetWidth() + ButtonSpacing + defaultButton.GetWidth() + Spacing + theme.GroupBoxBorderSize + Spacing;
	int minHeight = resolutionsListBox.GetY() + ResolutionsListBoxMinHeight + Spacing + LineHeight + Spacing + theme.GroupBoxBorderSize + Spacing + ButtonHeight + Spacing;
	SetMinClientSize(minWidth, minHeight);

	if (resize)
	{
		SetClientSize(dpi.Scale(ClientWidth), dpi.Scale(ClientHeight));
		Fit();
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

MainWindow::MainWindow(const SRE::GpuList &gpuList) :
	Window(Name, Title, WS_OVERLAPPEDWINDOW),
	gpuLabel(*this, L"GPU:"),
	gpuComboBox(*this),
	resolutionsGroupBox(*this, L" Resolutions "),
	widthTextBox(*this),
	xLabel(*this, L"\xD7"),
	heightTextBox(*this),
	addButton(*this, L"+"),
	deleteButton(*this, L"\x2013"),
	deleteAllButton(*this, L"Delete all"),
	defaultButton(*this, L"Default"),
	resolutionsListBox(*this),
	importButton(*this, L"Import..."),
	exportButton(*this, L"Export..."),
	copyButton(*this, L"Copy"),
	pasteButton(*this, L"Paste"),
	okButton(*this, L"OK"),
	cancelButton(*this, L"Cancel"),
	gpuList(gpuList),
	gpuIndex(-1),
	copied(false)
{
	AddMessages();
	AddCommands();
	InitializeTheme();
	InitializeControls();
	ScaleControls(true);
	Center();
	Show();
}

////////////////////////////////////////////////////////////////////////////
