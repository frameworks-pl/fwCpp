#include "fwCommon_pch.h"
#include "fwfile.h"
#include "fwfileutils.h"

namespace fw
{
	namespace core
	{
		CString File::getName() const
		{
			return fw::core::FileUtils::extractFileNameWithExt(m_sPath);

		}

		CString File::getSizeAsString() const
		{
			TCHAR buff[1024];
			StrFormatByteSize(m_Size, &buff[0], 1024);

			return CString(buff);
		}
	}
}
