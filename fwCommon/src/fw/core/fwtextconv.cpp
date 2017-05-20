#include "fwCommon_pch.h"
#include "fwtextconv.h"
#include "fwexception.h"
#include "fwapputils.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw {

	namespace core {

		std::string
			TextConv::Unicode2UTF8(const CString& pUnicodeStr) {

#ifdef UNICODE

			//handle case when trying to convert empty string
			if (pUnicodeStr.GetLength() == 0) {
				std::string empty_str;
				return empty_str;
			}

			//first calculating size of the buffer needed for conversion
			int size = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pUnicodeStr, pUnicodeStr.GetLength(),
				NULL, 0, NULL, NULL);

			//failed to obatin size for the buffer, something is 
			//wrong. Let's through an exception
			if (size <= 0) {
				CString s = fw::core::AppUtils::getLastError();
				DWORD error = GetLastError();
				if (error != ERROR_SUCCESS) {
					fw::core::Exception ex(_T("Conversion from Unicode to UTF8 failed."));
					throw ex;
				}
			}

			//preparing the buffer
			char* pBuffer = new char[size + 1];

			int result = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)pUnicodeStr, pUnicodeStr.GetLength(),
				pBuffer, size, NULL, NULL);

			if (result == 0) {
				DWORD error = GetLastError();
				if (error != ERROR_SUCCESS) {
					fw::core::Exception ex(_T("Conversion from Unicode to UTF8 failed."));
					throw ex;

				}
			}

			//adding termination character
			pBuffer[result] = 0;

			std::string sResult(pBuffer);
			delete[] pBuffer;
			return sResult;

#else

			return (LPCTSTR)pUnicodeStr;

#endif

		}; //Unicode2UTF8



		CString
			TextConv::UTF82Unicode(const char* pUTF8Str) {


			//handle empty string case
			if (pUTF8Str == NULL || (strlen(pUTF8Str) == 0))
				return _T("");


			//first calculate size of the buffer needed for conversion
			int size = MultiByteToWideChar(CP_UTF8, 0, pUTF8Str, (int)strlen(pUTF8Str),
				NULL, 0);

			//failed to obtain buffer size
			if (size <= 0) {
				DWORD dwError = GetLastError();
				if (dwError != ERROR_SUCCESS) {
					fw::core::Exception ex(_T("Conversion from UTF8 to Unicode failed."));
					throw ex;
				}
			}

			wchar_t* pBuffer = new wchar_t[size + 1];

			int result = MultiByteToWideChar(CP_UTF8, 0, pUTF8Str, (int)strlen(pUTF8Str),
				pBuffer, size);
			if (result == 0) {
				DWORD dwError = GetLastError();
				if (dwError != ERROR_SUCCESS) {
					fw::core::Exception ex(_T("Conversion from UTF8 to Unicode failed."));
					throw ex;
				}
			}

			pBuffer[result] = 0;

			CString sResult(pBuffer, result);

			delete[] pBuffer;
			return sResult;


		}; //UTF82Unicode


		void TextConv::StripBreaks(CString& pString)
		{

			//first strip breaks from the end of the line
			while (false == pString.IsEmpty())
			{
				TCHAR c = pString.GetAt(pString.GetLength() - 1);
				if (c == 10 || c == 13)
					pString = pString.Left(pString.GetLength() - 1);
				else
					break;
			}

		}


		void TextConv::Quote(CString& pString)
		{
			pString.Replace(_T("\""), _T(""));
			pString.Replace(_T("\'"), _T(""));

			pString.Insert(0, _T("\""));
			pString.Append(_T("\""));

		}

	} //namespace core

} //namespace fw



