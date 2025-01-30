#ifndef SRE_RESOLUTION_H
#define SRE_RESOLUTION_H

// Includes ////////////////////////////////////////////////////////////////

#include "SRE_Common.h"

// Structs /////////////////////////////////////////////////////////////////

namespace SRE
{
	struct Resolution
	{
		int width;
		int height;
		bool added;

		Resolution() = default;
		Resolution(int width, int height, bool added = true);
		static bool Compare(const Resolution &resolution, const Resolution &other);
		bool operator==(const Resolution &other) const;
		bool operator!=(const Resolution &other) const;
		bool operator<(const Resolution &other) const;
		bool operator>(const Resolution &other) const;
		bool operator<=(const Resolution &other) const;
		bool operator>=(const Resolution &other) const;
		std::wstring GetText() const;
		std::basic_string<unsigned char> GetAmdMode() const;
		bool SetAmdMode(const std::basic_string<unsigned char> &value);
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
