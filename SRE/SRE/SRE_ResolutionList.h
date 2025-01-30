#ifndef SRE_RESOLUTIONLIST_H
#define SRE_RESOLUTIONLIST_H

// Includes ////////////////////////////////////////////////////////////////

#include "SRE_Common.h"

// Classes /////////////////////////////////////////////////////////////////

namespace SRE
{
	enum class GpuType;
	struct Resolution;

	class ResolutionList
	{
	public:

		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Functions ///////////////////////////////////////////////////////

		ResolutionList();

		bool operator==(const ResolutionList &other) const;
		bool operator!=(const ResolutionList &other) const;

		bool Add(const Resolution &resolution);
		bool IsAmdDefault(const Resolution &resolution) const;
		bool AddDefault(const Resolution &resolution);
		bool IsAdded(const Resolution &resolution) const;
		bool Exists(const Resolution &resolution) const;
		int GetIndex(const Resolution &resolution) const;
		int GetCount() const;
		bool IsValidIndex(int index) const;
		bool Get(int index, Resolution &resolution) const;
		bool Delete(int index);
		bool Clear();
		bool IsValidWidth(int width) const;
		bool IsValidHeight(int height) const;
		std::basic_string<unsigned char> GetAmdModes() const;
		std::basic_string<unsigned char> GetAmdRestrictedModes() const;
		bool SetAmdModes(const std::basic_string<unsigned char> &value);
		bool DeleteAmdModes(const std::basic_string<unsigned char> &value);
		std::wstring GetNvidiaModes() const;
		bool SetNvidiaModes(const std::wstring &value);
		bool GetText(std::wstring &text) const;
		bool SetText(const std::wstring &text);
		bool Import(const std::wstring &file);
		bool Export(const std::wstring &file) const;

		////////////////////////////////////////////////////////////////////

	private:

		////////////////////////////////////////////////////////////////////
		// Private /////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Constants ///////////////////////////////////////////////////////

		static const int MinWidth;
		static const int MaxWidth;
		static const int MinHeight;
		static const int MaxHeight;

		static const std::vector<Resolution> AmdDefaultResolutionList;
		static const std::vector<Resolution> NvidiaDefaultResolutionList;

		// Variables ///////////////////////////////////////////////////////

		GpuType type;
		std::vector<Resolution> resolutionList;

		////////////////////////////////////////////////////////////////////
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
