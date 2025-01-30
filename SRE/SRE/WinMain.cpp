// Includes ////////////////////////////////////////////////////////////////

#include "MainWindow.h"

// WinMain /////////////////////////////////////////////////////////////////

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ wchar_t *, _In_ int)
{
	try
	{
		SRE::GpuList gpuList;

		if (!gpuList.Load())
			throw L"Failed to load GPUs from the registry.";

		if (gpuList.GetCount() == 0)
			throw L"No compatible GPUs found in the registry.";

		MainWindow mainWindow(gpuList);
		return mainWindow.MessageLoop();
	}
	catch (const wchar_t *message)
	{
		MessageBoxW(nullptr, message, L"Error", MB_ICONERROR);
		return 1;
	}
}

////////////////////////////////////////////////////////////////////////////
