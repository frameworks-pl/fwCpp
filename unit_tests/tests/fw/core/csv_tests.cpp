#include "gtest.h"
#include <list>
#include <afxwin.h>
#include <atlstr.h>
#include <src/fw/core/fwapputils.h>
#include <src/fw/core/fwfileutils.h>
#include <src/fw/core/fwcsv.h>



class CSVTest : public ::testing::Test
{
	public:
		CSVTest() : m_sTestPath(_T("c:\\temp\\csvTest"))
		{
			m_Excludes.insert(std::set<CString>::value_type(_T(".")));
		}
	protected:
		virtual void SetUp()
		{
			fw::core::FileUtils::createDir(m_sTestPath);

			//CString sLastError;
			CString sAppPath = fw::core::AppUtils::getAppPath();
			CString sPath;
			sPath.Format(_T("%s\\..\\unit_tests\\resources\\"), sAppPath);
			CopyFile(sPath + _T("\\duetone.csv"), m_sTestPath + _T("\\duetone.csv"), FALSE);

		}

		void TearDown() override
		{
			fw::core::FileUtils::removeDir(m_sTestPath);
		}

		CString m_sTestPath;
		std::set<CString> m_Excludes;
};

TEST_F(CSVTest, filterFileTests)
{
	CString content;
	CString path;
	path.Format(_T("%s\\%s"), m_sTestPath, _T("duetone.csv"));
	FILE* fStream;
	errno_t errCode = _tfopen_s(&fStream, path, _T("r, ccs=UNICODE"));
	if (!errCode)
	{
		CStdioFile File(fStream);
		CString sLine;
		while (File.ReadString(sLine))
		{
			content.Append(sLine);
		}
	}
	fclose(fStream);

	fw::core::CSV csv;
	csv.fromString(content);
}
