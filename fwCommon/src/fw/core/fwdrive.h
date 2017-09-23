#ifndef DRIVE_H_INCLUDED
#define DRIVE_H_INCLUDED

namespace fw
{
	namespace core
	{
		class Drive
		{
			public:
				enum Type
				{
					eUnknown = DRIVE_UNKNOWN,
					eNoRootDir = DRIVE_NO_ROOT_DIR,
					eRemovable = DRIVE_REMOVABLE,
					eFixed = DRIVE_FIXED,
					eNetwork = DRIVE_REMOTE,
					eCDROM = DRIVE_CDROM,
					eRAMDisk = DRIVE_RAMDISK
				};

				Drive(const CString& sLetter = _T(""), Type eType = eUnknown);
				const CString& getLetter() const { return m_sLetter;  }
				Type getType() const { return m_eType; }
			protected:
				CString m_sLetter;
				Type m_eType;
		};

	}

}

#endif // !DRIVE_H_INCLUDED
