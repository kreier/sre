#ifndef SRE_GPU_H
#define SRE_GPU_H

// Includes ////////////////////////////////////////////////////////////////

#include "SRE_ResolutionList.h"
#include "SRE_Common.h"

// Enums ///////////////////////////////////////////////////////////////////

namespace SRE
{
	enum class GpuType
	{
		Invalid,
		Amd,
		Nvidia
	};
}

// Classes /////////////////////////////////////////////////////////////////

namespace SRE
{
	class Registry;

	class Gpu
	{
	public:

		////////////////////////////////////////////////////////////////////
		// Public //////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Functions ///////////////////////////////////////////////////////

		Gpu();

		bool operator<(const Gpu &other) const;
		bool operator>(const Gpu &other) const;
		bool operator<=(const Gpu &other) const;
		bool operator>=(const Gpu &other) const;

		bool Load(const std::wstring &id);
		bool SaveAmdModes(const Registry &registry, const std::wstring &name, const std::basic_string<unsigned char> &modes) const;
		bool IsSaved() const;
		bool Save();
		GpuType GetType() const;
		std::wstring GetId() const;
		std::wstring GetName() const;
		bool IsActive() const;
		bool IsModified() const;
		bool GetResolutionList(ResolutionList &resolutionList) const;
		bool SetResolutionList(const ResolutionList &resolutionList);
		bool GetDefaultResolutionList(ResolutionList &resolutionList) const;

		////////////////////////////////////////////////////////////////////

	private:

		////////////////////////////////////////////////////////////////////
		// Private /////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////

		// Constants ///////////////////////////////////////////////////////

		static const GUID GUID_DEVINTERFACE_DISPLAY_ADAPTER;

		// Variables ///////////////////////////////////////////////////////

		GpuType type;
		std::wstring id;
		std::wstring name;
		bool active;
		ResolutionList resolutionList;
		ResolutionList savedResolutionList;
		ResolutionList defaultResolutionList;
		std::basic_string<unsigned char> amdBackup;
		std::basic_string<unsigned char> amdRestrictedBackup;
		std::wstring nvidiaBackup;
		bool backupExists;

		////////////////////////////////////////////////////////////////////
	};
}

////////////////////////////////////////////////////////////////////////////

#endif
