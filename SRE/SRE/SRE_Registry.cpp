// Includes ////////////////////////////////////////////////////////////////

#include "SRE.h"

// Functions ///////////////////////////////////////////////////////////////

SRE::Registry::Registry() :
	handle(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////

SRE::Registry::Registry(HKEY hkey) :
	handle(hkey)
{
}

////////////////////////////////////////////////////////////////////////////

SRE::Registry::~Registry()
{
	Close();
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Registry::Open(HKEY hkey, const std::wstring &subkey, REGSAM desired)
{
	if (RegOpenKeyExW(hkey, subkey.c_str(), 0, desired, &handle) != ERROR_SUCCESS)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Registry::EnumKey(int index, std::wstring &name) const
{
	if (!handle)
		return false;

	DWORD size = MAX_PATH;
	std::vector<wchar_t> data(size);

	if (RegEnumKeyExW(handle, index, data.data(), &size, nullptr, nullptr, nullptr, nullptr) != ERROR_SUCCESS)
		return false;

	name = std::wstring(data.data(), size);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Registry::GetString(const std::wstring &name, std::wstring &value) const
{
	if (!handle)
		return false;

	DWORD size;

	if (RegQueryValueExW(handle, name.c_str(), nullptr, nullptr, nullptr, &size) != ERROR_SUCCESS)
		return false;

	std::vector<BYTE> data(size);

	if (RegQueryValueExW(handle, name.c_str(), nullptr, nullptr, data.data(), &size) != ERROR_SUCCESS)
		return false;

	value = std::wstring(reinterpret_cast<wchar_t *>(data.data()), size / sizeof(wchar_t));

	if (!value.empty() && value.back() == L'\0')
		value.pop_back();

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Registry::SetString(const std::wstring &name, const std::wstring &value, DWORD type) const
{
	if (!handle)
		return false;

	const BYTE *data = reinterpret_cast<const BYTE *>(value.c_str());
	DWORD size = static_cast<DWORD>((value.size() + 1) * sizeof(wchar_t));

	if (RegSetValueExW(handle, name.c_str(), 0, type, data, size) != ERROR_SUCCESS)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Registry::GetBinary(const std::wstring &name, std::basic_string<unsigned char> &value) const
{
	if (!handle)
		return false;

	DWORD size;

	if (RegQueryValueExW(handle, name.c_str(), nullptr, nullptr, nullptr, &size) != ERROR_SUCCESS)
		return false;

	std::vector<BYTE> data(size);

	if (RegQueryValueExW(handle, name.c_str(), nullptr, nullptr, data.data(), &size) != ERROR_SUCCESS)
		return false;

	value = std::basic_string<unsigned char>(data.data(), size);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Registry::SetBinary(const std::wstring &name, const std::basic_string<unsigned char> &value, DWORD type) const
{
	if (!handle)
		return false;

	const BYTE *data = value.c_str();
	DWORD size = static_cast<DWORD>(value.size());

	if (RegSetValueExW(handle, name.c_str(), 0, type, data, size) != ERROR_SUCCESS)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Registry::DeleteValue(const std::wstring &name) const
{
	if (!handle)
		return false;

	if (RegDeleteValueW(handle, name.c_str()) != ERROR_SUCCESS)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Registry::Close()
{
	if (!handle)
		return false;

	if (RegCloseKey(handle) != ERROR_SUCCESS)
		return false;

	handle = nullptr;
	return true;
}

////////////////////////////////////////////////////////////////////////////
