#include "fwCommon_pch.h"
#include "fwfile.h"
#include "fwfileutils.h"
#include "fwdatetime.h"

namespace fw
{
	namespace core
	{

		SYSTEMTIME File::INVALID_SYSTEM_TIME =	{ 0, 0, 0, 0, 0, 0, 0, 0 };

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


		CString File::getLastUpdateDateAsString() const
		{
			if (DateTime::isValid(m_LastUpdated))
			{
				CString sMask = _T("Y-m-d H:i:s");
				return DateTime::timeToString(sMask, m_LastUpdated);
			}

			return _T("");
		}
	}
}
