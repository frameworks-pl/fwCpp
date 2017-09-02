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
	}
}
