#include "fwCommon_pch.h"

#include <wininet.h>
#include "fwapputils.h"
#include "fwfileutils.h"
#include "fwnumbers.h"
#include <shlwapi.h>


#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


namespace fw {

	namespace core {

		bool AppUtils::m_bThemesAreOn = false;
		bool AppUtils::m_bThemesChecked = false;

		CString AppUtils::getAppPath() 
		{

			TCHAR path[MAX_PATH];
			GetModuleFileName(GetModuleHandle(NULL), path, sizeof(path));
			PathRemoveFileSpec(path);

			CString s(path);
			return s;

		}; //getAppPath

		CString AppUtils::getExecutableFileName() 
		{

			TCHAR path[MAX_PATH];
			GetModuleFileName(GetModuleHandle(NULL), path, sizeof(path));
			CString sPath(path);

			return FileUtils::extractFileNameWithExt(sPath);

		};


		CString
			AppUtils::getLastError() {

			DWORD dwError = GetLastError();
			TCHAR buffer[1024];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, buffer, 1024, NULL);
			CString err(buffer);
			err.Replace(_T("\n"), _T(""));
			return err;

		}; //getLastError


		CString
			AppUtils::getRandomString(int pLength) {

			CString result;
			for (int i = 0; i < pLength; i++) {
				int val;
				do {
					val = rand() % 122;
				} while ((val < 48 || val > 57) && (val < 97 || val > 122));
				result += TCHAR(val);
			}

			return result;

		}; //getRandomString


		bool AppUtils::getOSVersion(int& iMajorVersion, int& iMinorVersion)
		{
			OSVERSIONINFO ovi;
			memset(&ovi, 0, sizeof(OSVERSIONINFO));
			ovi.dwOSVersionInfoSize = sizeof(ovi);
			if (FALSE == GetVersionEx(&ovi))
			{
				TRACE(_T("Failed to get version info!\n"));
				iMajorVersion = -1;
				iMinorVersion = -1;
				return false;
			}

			iMajorVersion = (int)ovi.dwMajorVersion;
			iMinorVersion = (int)ovi.dwMinorVersion;

			return true;

		}


		bool AppUtils::isThemeOn(CWnd* pWnd) {

			if (false == m_bThemesChecked)
			{
				m_bThemesChecked = true;

				int iMajorVersion;
				int iMinorVersion;
				if (false == getOSVersion(iMajorVersion, iMinorVersion))
				{
					TRACE(_T("Failed to get version info, returning false!\n"));
					m_bThemesAreOn = false;
					return m_bThemesAreOn;
				}

				//this function works on the assumption that windows 5.1 or later
				//have two different comctl32.dll (one for old fashion controls 
				//and one for xp-themed controls).
				//we are taking handle of the dummy control constructor
				//and check if this is v6 or earlier (if v6 (or later), we assume
				//that the theme is on!

				//if version of Windows is 5.1 or later
				if ((iMajorVersion >= 5) || (iMajorVersion == 5 && iMinorVersion >= 1))
				{

					//creating dummy control, to find out its creater handle
					CButton* pBtn = new CButton;
					if (!pBtn->Create(_T("dummy"), WS_CHILD, CRect(0, 0, 0, 0), pWnd, -1)) {
						TRACE(_T("Failed to craete dummy control\n"));
						m_bThemesAreOn = false;
						return m_bThemesAreOn;
					}

#pragma warning (push)
#pragma warning (disable: 4312)
					HANDLE ctrlHandle = (HANDLE)GetClassLongPtr(pBtn->m_hWnd, GCLP_HMODULE);
#pragma warning (pop)

					//taking handle of the cmnctrl.dll
					HANDLE cmnctrlHANDLE = GetModuleHandle(_T("comctl32.dll"));

					m_bThemesAreOn = (ctrlHandle == cmnctrlHANDLE);

					pBtn->DestroyWindow();
					delete pBtn;
				}
			}

			return m_bThemesAreOn;

		}; //isThemeOn


		CString
			AppUtils::getAppDataFolder() {

			//I wanted to use SHGetFolderPath here
			//but it only works with _WIN32_IE 0x0500
			//so I use the mose "portable" method that
			//is getting variable through environment variable
			TCHAR* path = _tgetenv(_T("APPDATA"));
			if (path)
			{
				CString result(path);
				return result;
			}

			return _T("");


		}; //getAppDataFolder


		CString
			AppUtils::getGUIDAsString()
		{

			//first create the GUID
			GUID guid;
			if (CoCreateGuid(&guid) != S_OK)
				_T("");

			//convert GUID to Ole string
			LPOLESTR pstrGuid;
			if (StringFromCLSID(guid, &pstrGuid) != S_OK)
				return _T("");

			//make CString from Ole string
			CString cstr_guid(pstrGuid);
			cstr_guid.MakeLower();


			//relase Ole string
			if (NULL != pstrGuid)
				CoTaskMemFree(pstrGuid);

			return cstr_guid;

		}; //getGUIDAsString

		bool AppUtils::compareGUIDs(const CString& sGUID1, const CString& sGUID2)
		{
			return !sGUID1.CompareNoCase(sGUID2);
		}


		CString AppUtils::getEnvVar(const CString& pVarName)
		{

			CString sResult;

			//first try to determine how large buffer is needed
			DWORD dwNeededSize = GetEnvironmentVariable((LPCTSTR)pVarName, NULL, 0);

			if (dwNeededSize > 0)
			{
				TCHAR* pBuffer = new TCHAR[dwNeededSize];
				DWORD dwSize = GetEnvironmentVariable((LPCTSTR)pVarName, pBuffer, dwNeededSize);

				if (dwSize > 0)
				{
					pBuffer[dwNeededSize - 1] = 0;
					sResult = CString(pBuffer);
				}

				delete[] pBuffer;
				pBuffer = NULL;
			}

			return sResult;
		}


		int AppUtils::getEnvVarAsInt(const CString& pVarName)
		{
			int iResult = 0;
			CString sValue = getEnvVar(pVarName);
			if (false == sValue.IsEmpty())
			{
				if (fw::core::Numbers::isInt(sValue, true))
				{
					iResult = fw::core::Numbers::toInt(sValue);

				}
			}

			return iResult;
		}


		bool AppUtils::getPathToDefaultBrowser(CString& pPathToBrowser)
		{

			pPathToBrowser.Empty();

			//we need to create some HTM file to use FindExecutable
			//In order to do that we need to create the file in temp folder
			CString sTempFolder = getEnvVar(_T("TEMP"));
			if (false == sTempFolder.IsEmpty() && FileUtils::dirExists(sTempFolder))
			{
				fw::core::FileUtils::stripEndingBackslash(sTempFolder);
				CString sPathToTempHtm = sTempFolder + _T("\\") + _T("temp.htm");

				HANDLE hFile = NULL;
				if (false == fw::core::FileUtils::fileExists(sPathToTempHtm))
				{
					hFile = CreateFile(sPathToTempHtm, 0, FILE_SHARE_READ,
						NULL, CREATE_NEW, FILE_FLAG_RANDOM_ACCESS, NULL);
				}

				TCHAR path_to_def_browser[MAX_PATH];
				int iResult = (int)FindExecutable(sPathToTempHtm, NULL, path_to_def_browser);


				//if we created the file, we must destroy it
				if (hFile)
					DeleteFile(sPathToTempHtm);

				pPathToBrowser = CString(path_to_def_browser);

				if (iResult > 32)
					return true;

			}

			return false;

		}


		bool AppUtils::runtimeObjectsHaveTheSameClass(CObject* pFirstObj, CObject* pSecondObj)
		{

			if ((NULL != pFirstObj) && (NULL != pSecondObj))
			{

				CRuntimeClass* pFirstRTClass = pFirstObj->GetRuntimeClass();
				CRuntimeClass* pSecondRTClass = pSecondObj->GetRuntimeClass();
				if ((NULL != pFirstRTClass) && (NULL != pSecondRTClass))
				{
					CString sFirstClassName(pFirstRTClass->m_lpszClassName);
					CString sSecondClassName(pSecondRTClass->m_lpszClassName);

					return (sFirstClassName == sSecondClassName);

				}

			}

			return false;

		}


		TCHAR AppUtils::getDecimalPoint()
		{

			CString buffer;
			TCHAR cResult = _T('.');

			//trying to get from user settings
			int count = GetLocaleInfo(LOCALE_USER_DEFAULT,
				LOCALE_SMONDECIMALSEP,
				NULL,
				0);
			count = GetLocaleInfo(LOCALE_USER_DEFAULT,
				LOCALE_SMONDECIMALSEP,
				buffer.GetBuffer(count),
				count);
			buffer.ReleaseBuffer();

			if (count > 0)
				cResult = buffer.GetAt(0);
			else
			{
				//trying to get from system settings
				int count = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT,
					LOCALE_SMONDECIMALSEP,
					NULL,
					0);
				count = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT,
					LOCALE_SMONDECIMALSEP,
					buffer.GetBuffer(count),
					count);
				buffer.ReleaseBuffer();

				if (count > 0)
					cResult = buffer.GetAt(0);
			}


			return cResult;

		}

		MeasurementSystemEnum AppUtils::getMeasurementSystem()
		{
			DWORD dwMSys = 0;
			GetLocaleInfo(LOCALE_USER_DEFAULT,     // locale identifier (current user locale) 
				LOCALE_IMEASURE | LOCALE_RETURN_NUMBER,  // information type (measurement system) 
				(LPTSTR)&dwMSys,                               // retuned value 
				sizeof(DWORD));

			switch (dwMSys)
			{
			case 0:
				return MS_METRIC;
			case 1:
				return MS_US;
			default:
				return MS_METRIC;
			}


		}


		CString AppUtils::getTempFolderPath()
		{

			CString sTempLocation;

			CString sTempDir = fw::core::AppUtils::getEnvVar(_T("TEMP"));
			if (true == fw::core::FileUtils::dirExists(sTempDir))
				sTempLocation = sTempDir;

			if (true == sTempDir.IsEmpty())
			{
				CString sHomeDir = fw::core::AppUtils::getEnvVar(_T("USERPROFILE"));
				if (true == fw::core::FileUtils::dirExists(sHomeDir))
					sTempLocation = sHomeDir;
			}

			return sTempLocation;


		}


		bool AppUtils::isInternetActive()
		{
			DWORD dwFlags = 0;
			return (TRUE == InternetGetConnectedState(&dwFlags, 0));

		}

	} //namespace core

}; //namespace