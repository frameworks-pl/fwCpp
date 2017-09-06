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

TEST_F(FileUtilsTest, getFileNameTest)
{
	fw::core::Files filesAndDirs = fw::core::FileUtils::getFiles(m_sTestPath, m_Excludes);

	ASSERT_EQ(1, filesAndDirs.getFiles().size());
	ASSERT_EQ("file1.txt", filesAndDirs.getFiles()[0].getName());
}


TEST_F(FileUtilsTest, getPathTest)
{
	fw::core::Files filesAndDirs = fw::core::FileUtils::getFiles(m_sTestPath, m_Excludes);
	CString sExptected;
	sExptected = _T("c:\\temp\\listDirectoryTest\\file1.txt");
	CString sPath = filesAndDirs.getFiles()[0].getPath();

	ASSERT_TRUE(!sExptected.CompareNoCase(sPath));
	
}

TEST_F(FileUtilsTest, getFilesSlashAtTheEnd)
{
	fw::core::Files filesAndDirs = fw::core::FileUtils::getFiles(m_sTestPath + _T("\\"), m_Excludes);
	ASSERT_EQ(3, filesAndDirs.getAll().size());
}

TEST_F(FileUtilsTest, nameToIndex)
{
	fw::core::Files filesAndDirs = fw::core::FileUtils::getFiles(m_sTestPath, m_Excludes);
	int iIndex = filesAndDirs.nameToIndex(_T("file1.txt"));

	int iExpectedIndex = 0;
	std::vector<fw::core::File>::const_iterator it;
	for (it = filesAndDirs.getAll().begin(); it < filesAndDirs.getAll().end(); it++)
	{
		if (!it->getName().CompareNoCase(_T("file1.txt")))
			break;
		iExpectedIndex++;
	}

	ASSERT_EQ(iExpectedIndex, iIndex);	
}


TEST(FileUtils, cDriveExists)
{
	ASSERT_TRUE(fw::core::FileUtils::dirExists(_T("C:\\")));
}

TEST(FileUtils, getAbsolutePath)
{
	CString sPath(_T("c:\\data\\zakomed\\.."));
	CString s = fw::core::FileUtils::getAbsolutePath(sPath);
	CString sExpected(_T("c:\\data"));
	ASSERT_TRUE(!sExpected.CompareNoCase(s));
	
}


class FileUtilsFilteringTest : public ::testing::Test
{
public:
	FileUtilsFilteringTest() : m_sTestPath(_T("c:\\temp\\filteringTest"))
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
		CopyFile(sPath + _T("\\file2.txt"), m_sTestPath + _T("\\file2.txt"), FALSE);
		CopyFile(sPath + _T("\\aaa.txt"), m_sTestPath + _T("\\aaa.txt"), FALSE);
		CopyFile(sPath + _T("\\bbb.db"), m_sTestPath + _T("\\bbb.db"), FALSE);

	}
	CString m_sTestPath;
	std::set<CString> m_Excludes;
};

TEST_F(FileUtilsFilteringTest, filterFileTests)
{
	CString fileFilter(_T("*.db"));
	fw::core::Files filesAndDirs = fw::core::FileUtils::getFiles(m_sTestPath, m_Excludes, _T("*"), fileFilter);
	ASSERT_EQ(2, filesAndDirs.getDirectories().size());
	ASSERT_EQ(1, filesAndDirs.getFiles().size());
}