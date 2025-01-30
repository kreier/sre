// Includes ////////////////////////////////////////////////////////////////

#include "SRE.h"

// Constants ///////////////////////////////////////////////////////////////

const int SRE::ResolutionList::MinWidth = 100;
const int SRE::ResolutionList::MaxWidth = 15999;
const int SRE::ResolutionList::MinHeight = 100;
const int SRE::ResolutionList::MaxHeight = 15999;

const std::vector<SRE::Resolution> SRE::ResolutionList::AmdDefaultResolutionList =
{
	{5120, 2880, false},
	{5120, 2160, false},
	{3840, 2400, false},
	{3840, 2160, false},
	{3840, 1620, false},
	{3200, 1800, false},
	{2560, 1600, false},
	{2560, 1440, false},
	{2048, 1536, true},
	{1920, 1200, true},
	{1920, 1080, true},
	{1680, 1050, true},
	{1600, 1200, true},
	{1600, 900, false},
	{1366, 768, false},
	{1280, 1024, true},
	{1280, 800, false},
	{1280, 720, true},
	{1024, 768, true},
	{800, 600, true},
	{640, 480, true},
};

const std::vector<SRE::Resolution> SRE::ResolutionList::NvidiaDefaultResolutionList =
{
	{1600, 1200, true},
	{1280, 1024, true},
	{1024, 768, true},
	{800, 600, true},
	{640, 480, true},
};

// Functions ///////////////////////////////////////////////////////////////

SRE::ResolutionList::ResolutionList() :
	type(GpuType::Invalid)
{
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::operator==(const ResolutionList &other) const
{
	if (std::equal(resolutionList.begin(), resolutionList.end(), other.resolutionList.begin(), other.resolutionList.end(), Resolution::Compare))
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::operator!=(const ResolutionList &other) const
{
	return !(*this == other);
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::Add(const Resolution &resolution)
{
	if (!IsValidWidth(resolution.width) || !IsValidHeight(resolution.height))
		return false;

	auto iterator = std::lower_bound(resolutionList.begin(), resolutionList.end(), resolution, std::greater<Resolution>());

	if (iterator != resolutionList.end() && *iterator == resolution)
	{
		if (iterator->added)
			return false;

		*iterator = Resolution(resolution.width, resolution.height);
		return true;
	}

	resolutionList.insert(iterator, Resolution(resolution.width, resolution.height));
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::IsAmdDefault(const Resolution &resolution) const
{
	auto iterator = std::find(AmdDefaultResolutionList.begin(), AmdDefaultResolutionList.end(), resolution);

	if (iterator == AmdDefaultResolutionList.end())
		return false;

	if (iterator->added)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::AddDefault(const Resolution &resolution)
{
	if (!IsValidWidth(resolution.width) || !IsValidHeight(resolution.height))
		return false;

	switch (type)
	{
		case GpuType::Amd:
			if (!IsAmdDefault(resolution))
				return Add(resolution);

			break;

		default:
			return Add(resolution);
	}

	auto iterator = std::lower_bound(resolutionList.begin(), resolutionList.end(), resolution, std::greater<Resolution>());

	if (iterator != resolutionList.end() && *iterator == resolution)
		return false;

	resolutionList.insert(iterator, Resolution(resolution.width, resolution.height, false));
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::IsAdded(const Resolution &resolution) const
{
	auto iterator = std::find(resolutionList.begin(), resolutionList.end(), resolution);

	if (iterator == resolutionList.end())
		return false;

	return iterator->added;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::Exists(const Resolution &resolution) const
{
	if (std::find(resolutionList.begin(), resolutionList.end(), resolution) == resolutionList.end())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

int SRE::ResolutionList::GetIndex(const Resolution &resolution) const
{
	auto iterator = std::find(resolutionList.begin(), resolutionList.end(), resolution);

	if (iterator == resolutionList.end())
		return -1;

	return static_cast<int>(iterator - resolutionList.begin());
}

////////////////////////////////////////////////////////////////////////////

int SRE::ResolutionList::GetCount() const
{
	return static_cast<int>(resolutionList.size());
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::IsValidIndex(int index) const
{
	if (index < 0 || index >= GetCount())
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::Get(int index, Resolution &resolution) const
{
	if (!IsValidIndex(index))
		return false;

	resolution = resolutionList[index];
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::Delete(int index)
{
	if (!IsValidIndex(index))
		return false;

	resolutionList.erase(resolutionList.begin() + index);
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::Clear()
{
	if (GetCount() <= 0)
		return false;

	resolutionList.clear();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::IsValidWidth(int width) const
{
	if (width < MinWidth || width > MaxWidth)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::IsValidHeight(int height) const
{
	if (height < MinHeight || height > MaxHeight)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

std::basic_string<unsigned char> SRE::ResolutionList::GetAmdModes() const
{
	std::basic_string<unsigned char> value;

	for (const Resolution &resolution : resolutionList)
	{
		if (resolution.added)
			value += resolution.GetAmdMode();
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////

std::basic_string<unsigned char> SRE::ResolutionList::GetAmdRestrictedModes() const
{
	std::basic_string<unsigned char> value;

	for (const Resolution &resolution : AmdDefaultResolutionList)
	{
		if (!Exists(resolution))
			value += resolution.GetAmdMode();
	}

	return value;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::SetAmdModes(const std::basic_string<unsigned char> &value)
{
	type = GpuType::Amd;
	resolutionList = AmdDefaultResolutionList;

	for (unsigned index = 0; index < value.size(); index += 8)
	{
		Resolution resolution;
		resolution.SetAmdMode(value.substr(index, 8));
		Add(resolution);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::DeleteAmdModes(const std::basic_string<unsigned char> &value)
{
	for (unsigned index = 0; index < value.size(); index += 8)
	{
		Resolution resolution;
		resolution.SetAmdMode(value.substr(index, 8));
		Delete(GetIndex(resolution));
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

std::wstring SRE::ResolutionList::GetNvidiaModes() const
{
	std::wstring string = L"{*}SHV";

	if (GetCount() <= 0)
	{
		string += L" 99999x99999";
	}
	else
	{
		for (const Resolution &resolution : resolutionList)
			string += L' ' + resolution.GetText();
	}

	string += L"=1" + L'\0';
	return string;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::SetNvidiaModes(const std::wstring &value)
{
	type = GpuType::Nvidia;

	if (value.find(L'=') == std::string::npos)
	{
		resolutionList = NvidiaDefaultResolutionList;
		return true;
	}

	resolutionList.clear();
	std::wstring string = value;
	std::replace(string.begin(), string.end(), L'\0', L' ');
	std::wstringstream stream(string);

	while (stream)
	{
		Resolution resolution;
		wchar_t x;

		stream >> resolution.width >> std::noskipws >> x >> resolution.height >> std::skipws;

		if (stream)
		{
			if (x == L'x')
				Add(resolution);

			stream.ignore(value.size(), L' ');
		}
		else if (!stream.eof())
		{
			stream.clear();
			stream.ignore(value.size(), L' ');
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::GetText(std::wstring &text) const
{
	std::wstringstream stream;

	for (const Resolution &resolution : resolutionList)
	{
		stream << resolution.width << L'x' << resolution.height;

		if (!resolution.added)
			stream << L'*';

		stream << L"\r\n";
	}

	text = stream.str();
	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::SetText(const std::wstring &text)
{
	std::wstringstream stream(text);
	bool valid = false;

	while (stream)
	{
		Resolution resolution;
		stream >> resolution.width;

		if (!stream)
		{
			if (!valid && stream.eof())
			{
				resolutionList.clear();
				valid = true;
			}

			break;
		}

		stream.get();
		stream >> resolution.height;

		if (!stream)
			break;

		if (!valid)
		{
			resolutionList.clear();
			valid = true;
		}

		int isDefault = stream.get();

		if (stream && isDefault == L'*')
		{
			AddDefault(resolution);
			stream.get();
		}
		else
		{
			Add(resolution);
		}
	}

	if (!valid)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::Import(const std::wstring &file)
{
	std::ifstream stream(file);

	if (!stream)
		return false;

	bool valid = false;

	while (stream)
	{
		Resolution resolution;
		stream >> resolution.width;

		if (!stream)
		{
			if (!valid && stream.eof())
			{
				resolutionList.clear();
				valid = true;
			}

			break;
		}

		stream.get();
		stream >> resolution.height;

		if (!stream)
			break;

		if (!valid)
		{
			resolutionList.clear();
			valid = true;
		}

		int isDefault = stream.get();

		if (stream && isDefault == '*')
		{
			AddDefault(resolution);
			stream.get();
		}
		else
		{
			Add(resolution);
		}
	}

	if (!valid)
		return false;

	return true;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::ResolutionList::Export(const std::wstring &file) const
{
	std::ofstream stream(file);

	if (!stream)
		return false;

	for (const Resolution &resolution : resolutionList)
	{
		stream << resolution.width << 'x' << resolution.height;

		if (!resolution.added)
			stream << '*';

		stream << '\n';
	}

	stream.close();
	return true;
}

////////////////////////////////////////////////////////////////////////////
