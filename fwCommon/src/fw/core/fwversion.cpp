#include "fwCommon_pch.h"
#include "fwversion.h"
#include "fwbytebuffer.h"
#include "fwapputils.h"
#include "fwfileutils.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw 
{

	namespace core
	{



		Version::Version(const CString& pFilePath)
		{

			mProductMajor = -1;
			mProductMinor = -1;
			mProductRevision = -1;
			mProductBuildNo = -1;


			parse(pFilePath);

		}; //constructor



		void Version::parse(const CString& pFilePath)
		{

			CString path;
			if (pFilePath.IsEmpty()) {
				//building path to the executable
				path = fw::core::AppUtils::getAppPath();
				path += _T("\\") + fw::core::AppUtils::getExecutableFileName();
			}
			else
				path = pFilePath;

			if (fw::core::FileUtils::fileExists(path)) {

				//geting required size of the buffer for the file info
				DWORD size = GetFileVersionInfoSize((LPCTSTR)path, 0);
				if (size) {
					//preparing buffer that will receive file info
					fw::core::ByteBuffer buffer(size);

					if (GetFileVersionInfo((LPCTSTR)path, 0, size, buffer.getBuffer())) {

						//this pointer recieves the fileinfo structure
						//it is destroyed automatically when 
						LPVOID pPartBuffer;
						UINT len;
						if (VerQueryValue((void*)buffer.getBuffer(), _T("\\"), &pPartBuffer, &len)) {

							VS_FIXEDFILEINFO* pFInfo = (VS_FIXEDFILEINFO*)pPartBuffer;
							mProductMajor = HIWORD(pFInfo->dwProductVersionMS);
							mProductMinor = LOWORD(pFInfo->dwProductVersionMS);

							mProductRevision = HIWORD(pFInfo->dwProductVersionLS);
							mProductBuildNo = LOWORD(pFInfo->dwProductVersionLS);

							mFileVerMajor = HIWORD(pFInfo->dwFileVersionMS);
							mFileVerMinor = LOWORD(pFInfo->dwFileVersionMS);

						}

					}

				}

			}

		}; //parse


		CString	Version::getProductVersionString(bool bWithBuild)
		{

			CString ver;
			if (true == bWithBuild)
				ver.Format(_T("%d.%d.%d (build %d)"), mProductMajor, mProductMinor, mProductRevision, mProductBuildNo);
			else
				ver.Format(_T("%d.%d.%d"), mProductMajor, mProductMinor, mProductRevision);

			return ver;


		}; //getVersionString

	} //namespace core


}; //namespace fw