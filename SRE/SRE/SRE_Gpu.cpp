// Includes ////////////////////////////////////////////////////////////////

#include "SRE.h"

// Constants ///////////////////////////////////////////////////////////////

const GUID SRE::Gpu::GUID_DEVINTERFACE_DISPLAY_ADAPTER = {0x5B45201D, 0xF2F2, 0x4F3B, {0x85, 0xBB, 0x30, 0xFF, 0x1F, 0x95, 0x35, 0x99}};

// Functions ///////////////////////////////////////////////////////////////

SRE::Gpu::Gpu() :
	type(GpuType::Invalid),
	active(false),
	backupExists(false)
{
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::operator<(const Gpu &other) const
{
	if (active && !other.active)
		return true;

	if (!active && other.active)
		return false;

	if (name.compare(other.name) < 0)
		return true;

	if (name.compare(other.name) > 0)
		return false;

	if (id.compare(other.id) < 0)
		return true;

	if (id.compare(other.id) > 0)
		return false;

	return false;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::operator>(const Gpu &other) const
{
	return other < *this;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::operator<=(const Gpu &other) const
{
	return !(other < *this);
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::operator>=(const Gpu &other) const
{
	return !(*this < other);
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::Load(const std::wstring &id)
{
	if (id.empty())
		return false;

	HDEVINFO devices = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_DISPLAY_ADAPTER, id.c_str(), nullptr, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);

	if (devices == INVALID_HANDLE_VALUE)
		return false;

	SP_DEVINFO_DATA device = {sizeof device};
	active = SetupDiEnumDeviceInfo(devices, 0, &device);

	if (!active)
	{
		SetupDiDestroyDeviceInfoList(devices);
		devices = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_DISPLAY_ADAPTER, id.c_str(), nullptr, DIGCF_DEVICEINTERFACE);

		if (devices == INVALID_HANDLE_VALUE)
			return false;

		if (!SetupDiEnumDeviceInfo(devices, 0, &device))
		{
			SetupDiDestroyDeviceInfoList(devices);
			return false;
		}
	}

	HKEY hkey = SetupDiOpenDevRegKey(devices, &device, DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_QUERY_VALUE);
	SetupDiDestroyDeviceInfoList(devices);

	if (!hkey)
		return false;

	Registry registry(hkey);

	if (!registry.GetString(L"DriverDesc", name))
		return false;

	std::wstring provider;

	if (!registry.GetString(L"ProviderName", provider))
		return false;

	if (provider.compare(L"Advanced Micro Devices, Inc.") == 0 || provider.compare(L"ATI Technologies Inc.") == 0)
	{
		std::basic_string<unsigned char> value;
		std::basic_string<unsigned char> modes;

		if (registry.GetBinary(L"DALNonStandardModesBCD", value))
			modes += value.substr(0, value.size() / 8 * 8);

		for (int index = 1; registry.GetBinary(L"DALNonStandardModesBCD" + std::to_wstring(index), value); ++index)
			modes += value.substr(0, value.size() / 8 * 8);

		std::basic_string<unsigned char> restrictedModes;

		if (registry.GetBinary(L"DALRestrictedModesBCD", value))
			restrictedModes += value.substr(0, value.size() / 8 * 8);

		for (int index = 1; registry.GetBinary(L"DALRestrictedModesBCD" + std::to_wstring(index), value); ++index)
			restrictedModes += value.substr(0, value.size() / 8 * 8);

		resolutionList.SetAmdModes(modes);
		resolutionList.DeleteAmdModes(restrictedModes);
		savedResolutionList = resolutionList;

		if (registry.GetBinary(L"DALNonStandardModesBCD.bak", amdBackup))
		{
			defaultResolutionList.SetAmdModes(amdBackup);

			if (registry.GetBinary(L"DALRestrictedModesBCD.bak", amdRestrictedBackup))
				defaultResolutionList.DeleteAmdModes(amdRestrictedBackup);
			else
				amdRestrictedBackup.clear();

			backupExists = true;
		}
		else
		{
			defaultResolutionList = resolutionList;
			amdBackup = modes;
			amdRestrictedBackup = restrictedModes;
			backupExists = false;
		}

		type = GpuType::Amd;
		this->id = id;
		return true;
	}

	if (provider.compare(L"NVIDIA") == 0)
	{
		std::wstring modes;
		registry.GetString(L"NV_Modes", modes);
		resolutionList.SetNvidiaModes(modes);
		savedResolutionList = resolutionList;

		if (registry.GetString(L"NV_Modes.bak", nvidiaBackup))
		{
			defaultResolutionList.SetNvidiaModes(nvidiaBackup);
			backupExists = true;
		}
		else
		{
			defaultResolutionList = resolutionList;
			nvidiaBackup = modes;
			backupExists = false;
		}

		type = GpuType::Nvidia;
		this->id = id;
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::SaveAmdModes(const Registry &registry, const std::wstring &name, const std::basic_string<unsigned char> &modes) const
{
	int size;
	int start;

	if (modes.size() <= 1280)
	{
		size = 64;
		start = 0;

		registry.DeleteValue(name);
	}
	else
	{
		size = 128;
		start = size;

		if (!registry.SetBinary(name, modes.substr(0, size)))
			return false;
	}

	int index = 1;

	for (unsigned offset = start; offset < modes.size(); offset += size)
	{
		if (!registry.SetBinary(name + std::to_wstring(index), modes.substr(offset, size)))
			return false;

		++index;
	}

	while (registry.DeleteValue(name + std::to_wstring(index)))
		++index;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::IsSaved() const
{
	if (resolutionList != savedResolutionList)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::Save()
{
	if (type == GpuType::Invalid)
		return false;

	if (id.empty())
		return false;

	if (IsSaved())
		return true;

	HDEVINFO devices = SetupDiGetClassDevsW(&GUID_DEVINTERFACE_DISPLAY_ADAPTER, id.c_str(), nullptr, DIGCF_DEVICEINTERFACE);

	if (devices == INVALID_HANDLE_VALUE)
		return false;

	SP_DEVINFO_DATA device = {sizeof device};

	if (!SetupDiEnumDeviceInfo(devices, 0, &device))
	{
		SetupDiDestroyDeviceInfoList(devices);
		return false;
	}

	HKEY hkey = SetupDiOpenDevRegKey(devices, &device, DICS_FLAG_GLOBAL, 0, DIREG_DRV, KEY_SET_VALUE);
	SetupDiDestroyDeviceInfoList(devices);

	if (!hkey)
		return false;

	Registry registry(hkey);

	switch (type)
	{
		case GpuType::Amd:
			if (resolutionList == defaultResolutionList)
			{
				if (backupExists)
				{
					if (!SaveAmdModes(registry, L"DALNonStandardModesBCD", amdBackup))
						return false;

					if (!SaveAmdModes(registry, L"DALRestrictedModesBCD", amdRestrictedBackup))
						return false;

					if (!registry.DeleteValue(L"DALNonStandardModesBCD.bak"))
						return false;

					if (!registry.DeleteValue(L"DALRestrictedModesBCD.bak"))
						return false;

					backupExists = false;
				}
			}
			else
			{
				if (!backupExists)
				{
					if (!registry.SetBinary(L"DALNonStandardModesBCD.bak", amdBackup))
						return false;

					if (!registry.SetBinary(L"DALRestrictedModesBCD.bak", amdRestrictedBackup))
						return false;

					backupExists = true;
				}

				if (!SaveAmdModes(registry, L"DALNonStandardModesBCD", resolutionList.GetAmdModes()))
					return false;

				if (!SaveAmdModes(registry, L"DALRestrictedModesBCD", resolutionList.GetAmdRestrictedModes()))
					return false;
			}

			savedResolutionList = resolutionList;
			return true;

		case GpuType::Nvidia:
			if (resolutionList == defaultResolutionList)
			{
				if (backupExists)
				{
					if (!nvidiaBackup.empty())
					{
						if (!registry.SetString(L"NV_Modes", nvidiaBackup, REG_MULTI_SZ))
							return false;
					}
					else
					{
						if (!registry.DeleteValue(L"NV_Modes"))
							return false;
					}

					if (!registry.DeleteValue(L"NV_Modes.bak"))
						return false;

					backupExists = false;
				}
			}
			else
			{
				if (!backupExists)
				{
					if (!registry.SetString(L"NV_Modes.bak", nvidiaBackup, REG_MULTI_SZ))
						return false;

					backupExists = true;
				}

				if (!registry.SetString(L"NV_Modes", resolutionList.GetNvidiaModes(), REG_MULTI_SZ))
					return false;
			}

			savedResolutionList = resolutionList;
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////

SRE::GpuType SRE::Gpu::GetType() const
{
	return type;
}

////////////////////////////////////////////////////////////////////////////

std::wstring SRE::Gpu::GetId() const
{
	return id;
}

////////////////////////////////////////////////////////////////////////////

std::wstring SRE::Gpu::GetName() const
{
	return name;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::IsActive() const
{
	return active;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::IsModified() const
{
	return backupExists;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::GetResolutionList(ResolutionList &resolutionList) const
{
	resolutionList = this->resolutionList;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::SetResolutionList(const ResolutionList &resolutionList)
{
	this->resolutionList = resolutionList;
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Gpu::GetDefaultResolutionList(ResolutionList &resolutionList) const
{
	resolutionList = defaultResolutionList;
	return true;
}

////////////////////////////////////////////////////////////////////////////
