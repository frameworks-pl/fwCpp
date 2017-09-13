#include "fwCommon_pch.h"
#include "fwfileutils.h"
#include "fwapputils.h"

#include <sys\types.h> 
#include <sys\stat.h> 



namespace fw 
{

	namespace core
	{

		bool FileUtils::dirExists(const CString& pPath) {

			CFileFind finder;
			if (finder.FindFile(pPath)) {
				finder.FindNextFile();
				return (finder.IsDirectory() == 0) ? false : true;
			}

			//scenario when path is root for a given drive
			std::vector<CString> volumes = FileUtils::getVolumes();
			std::vector<CString>::const_iterator it;
			for (it = volumes.begin(); it != volumes.end(); it++)
			{
				CString sVolumeRoot;
				sVolumeRoot.Format(_T("%s:\\"), *it);
				if (!sVolumeRoot.CompareNoCase(pPath))
					return true;
			}

			return false;

		}; //dirExists


		bool
			FileUtils::fileExists(const CString& pPath) {

			CFileFind finder;
			if (finder.FindFile(pPath)) {
				finder.FindNextFile();
				return !finder.IsDirectory();
			}

			return false;

		}; //file Exists


		bool
			FileUtils::createDir(const CString& pPath, CString* pErrMsg) {

			if (CreateDirectory(pPath, NULL)) {
				if (pErrMsg)
					*pErrMsg = _T("");
				return true;
			}
			else {
				if (pErrMsg)
					*pErrMsg = AppUtils::getLastError();
				return false;
			}

		}; //createDir

		bool
			FileUtils::removeDir(const CString& pPath, CString* pErrMsg) {

			if (RemoveDirectory(pPath)) {
				if (pErrMsg)
					*pErrMsg = _T("");
				return true;
			}
			else {
				if (pErrMsg)
					*pErrMsg = AppUtils::getLastError();
				return false;
			}

		}; //removeDir


		CString
			FileUtils::getPathPart(const PartEnum& pPart, const CString& pPath) {

			TCHAR drive[_MAX_DRIVE];
			TCHAR dir[_MAX_DIR];
			TCHAR fname[_MAX_FNAME];
			TCHAR ext[_MAX_EXT];

			_tsplitpath((LPCTSTR)pPath, drive, dir, fname, ext);

			switch (pPart) {
			case PATH_PART_DRIVE:
				return CString(drive);
			case PATH_PART_DIR:
				return CString(dir);
			case PATH_PART_FNAME:
				return CString(fname);
			case PATH_PART_EXT:
				return CString(ext);
			default:
				ASSERT(FALSE);
			}

			return _T("");

		}; //getPathPart

		CString
			FileUtils::extractFileNameWithExt(const CString & pPath) {

			CString sName = getPathPart(PATH_PART_FNAME, pPath);
			CString sExt = getPathPart(PATH_PART_EXT, pPath);

			if (!sExt.IsEmpty()) {
				if (sExt.GetAt(0) == TCHAR(_T('.')))
					sName += sExt;
				else
					sName += _T(".") + sExt;

			}

			return sName;

		}; //extractFileNameWithExt


		bool
			FileUtils::getCreationDate(const CString& pPath, SYSTEMTIME& pCreationDate)
		{

			SYSTEMTIME lastaccess_date;
			SYSTEMTIME lastmodif_date;
			if (getFileDates(pPath, pCreationDate, lastaccess_date, lastmodif_date))
			{
				return true;
			}

			return false;

		}; //getCreationDate


		bool FileUtils::getFileDates(const CString& pPath, SYSTEMTIME& pCreationDate, SYSTEMTIME& pLastAccessDate, SYSTEMTIME& pLastModifDate)
		{

			//I used to have problems with CreateFile when path was
			//in quotation marks
			//to avoid this problem let's try to strip them here
			CString path = pPath;
			path.Trim(_T('"'));

			HANDLE hFile = CreateFile((LPCTSTR)path, 0, FILE_SHARE_READ,
				NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);

			if (hFile != INVALID_HANDLE_VALUE) {

				FILETIME creation_time;
				FILETIME lastaccess_time;
				FILETIME lastwrite_time;
				if (GetFileTime(hFile, &creation_time, &lastaccess_time, &lastwrite_time))
				{

					if (FileTimeToSystemTime(&creation_time, &pCreationDate) &&
						FileTimeToSystemTime(&lastaccess_time, &pLastAccessDate) &&
						FileTimeToSystemTime(&lastwrite_time, &pLastModifDate))
					{
						return true;
					}
					else {
						TRACE(_T("ERROR: FileUtils::getCreationDate: %s\n"),
							fw::core::AppUtils::getLastError());
					}

				}
			}
			else {

				//for some reason file handle is invalid
				TRACE(_T("ERROR: FileUtils::getCreationDate: %s\n"),
					fw::core::AppUtils::getLastError());
			}

			return false;

		}


		std::list<CString> FileUtils::getFileDirList(const CString& pSearchMask, bool bFiles, bool bDirs)
		{

			//NOTE: this function has not been used/tested so far!!!!

			std::list<CString> results;
			if (false == pSearchMask.IsEmpty())
			{
				CFileFind file_find;
				if (file_find.FindFile(pSearchMask))
				{
					CString fp;

					while (file_find.FindNextFile())
					{
						fp = file_find.GetFilePath();

						if ((file_find.IsDirectory() && bDirs) || ((FALSE == file_find.IsDirectory()) && bFiles))
							results.push_back(fp);
					}

					fp = file_find.GetFilePath(); //the last one after exiting the loop
					if ((file_find.IsDirectory() && bDirs) || ((FALSE == file_find.IsDirectory()) && bFiles))
						results.push_back(fp);

				}

			}

			return results;

		} //getFileDirList


		void FileUtils::stripEndingBackslash(CString& pPath)
		{
			if ((pPath.GetLength() > 0) && (pPath[pPath.GetLength() - 1] == _T('\\')))
				pPath = pPath.Left(pPath.GetLength() - 1);

		} //stripEndingBackslash


		DWORD FileUtils::getFileSize(const CString& pPath)
		{
			struct __stat64 fileStat;
			int err = _tstat64(pPath, &fileStat);  //<- forcing Unicode to ANSI conversion!!!
			if (0 != err)
				return 0;
			return (DWORD)fileStat.st_size;

		}



		CString FileUtils::fileSizeToString(DWORD dwFileSize)
		{
			static const DWORD dwKB = 1024;          // Kilobyte
			static const DWORD dwMB = 1024 * dwKB;   // Megabyte
			static const DWORD dwGB = 1024 * dwMB;   // Gigabyte


			DWORD dwNumber, dwRemainder;
			CString strNumber;

			if (dwFileSize < dwKB)
			{
				strNumber = InsertSeparator(dwFileSize) + _T(" B");
			}
			else
			{
				if (dwFileSize < dwMB)
				{
					dwNumber = dwFileSize / dwKB;
					dwRemainder = (dwFileSize * 100 / dwKB) % 100;

					strNumber.Format(_T("%s.%02d KB"), (LPCTSTR)InsertSeparator(dwNumber), dwRemainder);
				}
				else
				{
					if (dwFileSize < dwGB)
					{
						dwNumber = dwFileSize / dwMB;
						dwRemainder = (dwFileSize * 100 / dwMB) % 100;
						strNumber.Format(_T("%s.%02d MB"), InsertSeparator(dwNumber), dwRemainder);
					}
					else
					{
						if (dwFileSize >= dwGB)
						{
							dwNumber = dwFileSize / dwGB;
							dwRemainder = (dwFileSize * 100 / dwGB) % 100;
							strNumber.Format(_T("%s.%02d GB"), InsertSeparator(dwNumber), dwRemainder);
						}
					}
				}
			}

			// Display decimal points only if needed
			// another alternative to this approach is to check before calling str.Format, and 
			// have separate cases depending on whether dwRemainder == 0 or not.

			strNumber.Replace(_T(".00"), _T(""));

			return strNumber;
		}

		CString FileUtils::InsertSeparator(DWORD dwNumber)
		{
			CString str;
			str.Format(_T("%u"), dwNumber);

			for (int i = str.GetLength() - 3; i > 0; i -= 3)
			{
				str.Insert(i, _T(","));
			}

			return (str);
		}

		ImageType FileUtils::extensionToImageType(const CString& pExtension)
		{
			ImageType eType = IMG_TYPE_UNKNOWN;

			if (0 == pExtension.CompareNoCase(_T("PNG")))
				eType = IMG_TYPE_PNG;
			else if (0 == pExtension.CompareNoCase(_T("JPG")))
				eType = IMG_TYPE_JPG;
			else if (0 == pExtension.CompareNoCase(_T("GIF")))
				eType = IMG_TYPE_GIF;
			else if (0 == pExtension.CompareNoCase(_T("BMP")))
				eType = IMG_TYPE_BMP;

			return eType;

		}

		CString FileUtils::imageExtensionToString(const ImageType eImageType)
		{
			switch (eImageType)
			{
			case IMG_TYPE_UNKNOWN:
				return _T("");

			case IMG_TYPE_BMP:
				return _T("BMP");

			case IMG_TYPE_PNG:
				return _T("PNG");

			case IMG_TYPE_GIF:
				return _T("GIF");

			case IMG_TYPE_JPG:
				return _T("JPG");
			}

			return _T("");


		}

		std::vector<CString> FileUtils::getVolumes()
		{
			std::vector<CString> driveLetters;
			DWORD drives = GetLogicalDrives();
			for (int i = 0; i < 26; i++)
			{
				if (drives & (1 << i))
				{
					TCHAR cLetter = TCHAR(65 + i);
					CString sLetter(cLetter);
					driveLetters.push_back(sLetter);
				}
			}

			return driveLetters;

		}

		fw::core::Files FileUtils::getFiles(const CString& pPath, 
			const std::set<CString>& excludes, const CString& sDirFilter, const CString& sFileFilter)
		{
			//convert releative path to full path
			CString fullPath = getAbsolutePath(pPath);
			FileUtils::stripEndingBackslash(fullPath);

			fw::core::Files files;
			getFileSystemItems(fullPath, true, sDirFilter, excludes, files);
			getFileSystemItems(fullPath, false, sFileFilter, excludes, files);
			
			return files;
		}

		void FileUtils::getFileSystemItems(const CString& sPath, bool bDir, CString sFilter, 
			const std::set<CString>& excludes, fw::core::Files& files)
		{
			if (FileUtils::dirExists(sPath))
			{
				CString sFullPath(sPath);
				fw::core::FileUtils::stripEndingBackslash(sFullPath);
				sFullPath.Append(_T("\\"));
				sFullPath.Append(sFilter);


				WIN32_FIND_DATA ffd;
				HANDLE hFind = FindFirstFile((LPCTSTR)sFullPath, &ffd);
				while (INVALID_HANDLE_VALUE != hFind)
				{
					CString sFileName(ffd.cFileName);
					if (excludes.find(sFileName) == excludes.end())
					{
						bool bIsDir = false;
						DWORD dwFileSize = 0;
						SYSTEMTIME lastModifiedDate = File::INVALID_SYSTEM_TIME;

						CString sFilePath(sPath);
						fw::core::FileUtils::stripEndingBackslash(sFilePath);
						sFilePath.Append(_T("\\"));
						sFilePath.Append(sFileName);

						if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
						{
							bIsDir = true;
						}
						else
						{
							dwFileSize = getFileSize(sFilePath);
						}
						SYSTEMTIME creationDate = File::INVALID_SYSTEM_TIME;
						SYSTEMTIME lastAccessDate = File::INVALID_SYSTEM_TIME;
						if (!getFileDates(sFilePath, creationDate, lastAccessDate, lastModifiedDate))
							lastModifiedDate = File::INVALID_SYSTEM_TIME;

						if ((bIsDir && bDir) || (!bIsDir && !bDir))
						{
							fw::core::File f(sFilePath, bIsDir, dwFileSize, lastModifiedDate);
							files.addFile(f);
						}
					}

					if (!FindNextFile(hFind, &ffd))
					{
						FindClose(hFind);
						hFind = INVALID_HANDLE_VALUE;
					}
				}
			}
		}


		CString FileUtils::getAbsolutePath(const CString& sPath)
		{
			TCHAR absPath[MAX_PATH];
			CString fullPath = sPath;
			if (_wfullpath(&absPath[0], (LPCTSTR)sPath, MAX_PATH))
			{
				fullPath = CString(absPath);
			}

			return fullPath;
		}


	} //namespace core


}; //namespace