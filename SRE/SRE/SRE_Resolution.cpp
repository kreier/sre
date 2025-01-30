// Includes ////////////////////////////////////////////////////////////////

#include "SRE.h"

// Functions ///////////////////////////////////////////////////////////////

SRE::Resolution::Resolution(int width, int height, bool added) :
	width(width),
	height(height),
	added(added)
{
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Resolution::Compare(const Resolution &resolution, const Resolution &other)
{
	if (resolution.width == other.width && resolution.height == other.height && resolution.added == other.added)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Resolution::operator==(const Resolution &other) const
{
	if (width == other.width && height == other.height)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Resolution::operator!=(const Resolution &other) const
{
	return !(*this == other);
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Resolution::operator<(const Resolution &other) const
{
	if (width < other.width)
		return true;

	if (width == other.width && height < other.height)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Resolution::operator>(const Resolution &other) const
{
	return other < *this;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Resolution::operator<=(const Resolution &other) const
{
	return !(other < *this);
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Resolution::operator>=(const Resolution &other) const
{
	return !(*this < other);
}

////////////////////////////////////////////////////////////////////////////

std::wstring SRE::Resolution::GetText() const
{
	return std::to_wstring(width) + L'x' + std::to_wstring(height);
}

////////////////////////////////////////////////////////////////////////////

std::basic_string<unsigned char> SRE::Resolution::GetAmdMode() const
{
	std::basic_string<unsigned char> value;
	value.push_back((width / 1000 << 4) + (width / 100 % 10));
	value.push_back((width / 10 % 10 << 4) + (width % 10));
	value.push_back((height / 1000 << 4) + (height / 100 % 10));
	value.push_back((height / 10 % 10 << 4) + (height % 10));
	value.push_back(0);
	value.push_back(0);
	value.push_back(0);
	value.push_back(0);
	return value;
}

////////////////////////////////////////////////////////////////////////////

bool SRE::Resolution::SetAmdMode(const std::basic_string<unsigned char> &value)
{
	width = (value[0] >> 4) * 1000 + (value[0] & 0xF) * 100 + (value[1] >> 4) * 10 + (value[1] & 0xF);
	height = (value[2] >> 4) * 1000 + (value[2] & 0xF) * 100 + (value[3] >> 4) * 10 + (value[3] & 0xF);
	return true;
}

////////////////////////////////////////////////////////////////////////////
