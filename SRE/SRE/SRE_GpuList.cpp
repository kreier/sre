// Includes ////////////////////////////////////////////////////////////////

#include "SRE.h"

// Constants ///////////////////////////////////////////////////////////////

const GUID SRE::GpuList::GUID_DEVCLASS_DISPLAY = {0x4D36E968, 0xE325, 0x11CE, {0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18}};

// Functions ///////////////////////////////////////////////////////////////

bool SRE::GpuList::Load()
{
	HDEVINFO devices = SetupDiGetClassDevsW(&GUID_DEVCLASS_DISPLAY, nullptr, nullptr, 0);

	if (devices == INVALID_HANDLE_VALUE)
		return false;

	SP_DEVINFO_DATA device = {sizeof device};

	for (int index = 0; SetupDiEnumDeviceInfo(devices, index, &device); ++index)
	{
		DWORD size;
		SetupDiGetDeviceInstanceIdW(devices, &device, nullptr, 0, &size);

		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			continue;

		std::vector<wchar_t> id(size);

		if (!SetupDiGetDeviceInstanceIdW(devices, &device, id.data(), size, nullptr))
			continue;

		Gpu gpu;

		if (gpu.Load(id.data()))
			gpuList.push_back(gpu);
	}

	SetupDiDestroyDeviceInfoList(devices);
	std::sort(gpuList.begin(), gpuList.end());
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::GpuList::IsSaved() const
{
	for (const Gpu &gpu : gpuList)
	{
		if (!gpu.IsSaved())
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

int SRE::GpuList::Save()
{
	int count = 0;

	for (Gpu &gpu : gpuList)
		count += gpu.Save();

	return count;
}

////////////////////////////////////////////////////////////////////////////

int SRE::GpuList::GetCount() const
{
	return static_cast<int>(gpuList.size());
}

////////////////////////////////////////////////////////////////////////////

bool SRE::GpuList::IsValidIndex(int index) const
{
	if (index < 0 || index >= GetCount())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::GpuList::Get(int index, Gpu &gpu) const
{
	if (!IsValidIndex(index))
		return false;

	gpu = gpuList[index];
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::GpuList::Set(int index, const Gpu &gpu)
{
	if (!IsValidIndex(index))
		return false;

	if (gpu.GetType() != gpuList[index].GetType())
		return false;

	if (gpu.GetId() != gpuList[index].GetId())
		return false;

	gpuList[index] = gpu;
	return true;
}

////////////////////////////////////////////////////////////////////////////
