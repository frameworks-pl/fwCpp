#include "gtest.h"
#include <list>
#include <afxwin.h>
#include <atlstr.h>
#include <src/fw/core/fwapputils.h>
#include <src/fw/core/fwfileutils.h>

class FileUtilsTest : public ::testing::Test
{

	public:
		FileUtilsTest() : m_sTestPath(_T("c:\\temp\\listDirectoryTest"))
		{
			m_Excludes.insert(std::set<CString>::value_type(_T(".")));

		}
	protected:
		virtual void SetUp()
		{
			fw::core::FileUtils::createDir(m_sTestPath);
			fw::core::FileUtils::createDir(m_sTestPath + _T("\\folder1"));
			
			//CString sLastError;
			CString sAppPath = fw::core::AppUtils::getAppPath();
			CString sPath;
			sPath.Format(_T("%s\\..\\unit_tests\\resources\\"), sAppPath);
			CopyFile(sPath + _T("\\file1.txt"), m_sTestPath + _T("\\file1.txt"), FALSE);

		}

		virtual void TearDown()
		{
			fw::core::FileUtils::removeDir(m_sTestPath);
		}

		CString m_sTestPath;
		std::set<CString> m_Excludes;

};

TEST_F(FileUtilsTest, listDirectoryTest)
{
	fw::core::Files filesAndDirs = fw::core::FileUtils::getFiles(m_sTestPath, m_Excludes);
	std::vector<fw::core::File> files = filesAndDirs.getFiles();
	std::vector<fw::core::File> dirs = filesAndDirs.getDirectories();
	std::vector<fw::core::File> all = filesAndDirs.getAll();
	ASSERT_EQ(3, all.size());
	ASSERT_EQ(2, dirs.size());
	ASSERT_EQ(1, files.size());

}