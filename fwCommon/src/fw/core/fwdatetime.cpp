#include "fwCommon_pch.h"
#include "fwdatetime.h"
#include "fwnumbers.h"
#include <boost/date_time/gregorian/gregorian.hpp>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

namespace fw {

	namespace core
	{

		const TCHAR DateTime::mSymbols[DateTime::LAST] = 
		{
			_T('Y'), //YEAR_4DIGIT
			_T('y'), //YEAR_2DIGIT
			_T('M'), //MONTH_2DIGIT
			_T('m'), //MONTH_1DIGIT
			_T('D'), //DAY_2DIGIT
			_T('d'), //DAY_1DIGIT
			_T('H'), //HOUR24
			_T('h'), //HOUR12
			_T('h'), //HOUR12_2
			_T('i'), //MINUTE
			_T('s'), //SECOND
			_T('t'), //AM_PM

		};

		const TCHAR* DateTime::mSystemSymbols[DateTime::LAST]=
		{
			_T("yyyy"), //YEAR_4DIGIT
			_T("yy"), //YEAR_2DIGIT
			_T("MM"), //MONTH_2DIGIT
			_T("M"),  //MONTH_1DIGIT
			_T("dd"), //DAY_2DIGIT
			_T("d"),  //DAY_1DIGIT
			_T("HH"), //HOUR24
			_T("hh"), //HOUR12
			_T("h"),  //HOUR12_2
			_T("mm"), //MINUTE
			_T("ss"), //SECOND
			_T("tt")  //AM_PM
		};

		DateTime::DateTime() {

		}; //constructor

		bool 
		DateTime::isWhiteSpace(const TCHAR& pSymbol) {

			CString sWhiteSpace = _T(":-/ _.");
			int count = sWhiteSpace.GetLength();
			for (int i=0; i < count; i++)
				if (pSymbol == sWhiteSpace.GetAt(i))
					return true;

			return false;

		}; //isWhiteSpace


		bool 
		DateTime::stringToTime(const CString& pDateTime, const CString& pMask, SYSTEMTIME* pTimeStruct) {

			CString sMask = pMask;
			CString sDateTime = pDateTime;
			//CString sWhiteSpace = _T(":-/");
			if (sMask.IsEmpty()) {
				throw (Exception(_T("Date/time mask is empty!")));
			}

		  if (pTimeStruct)
			memset(pTimeStruct, 0, sizeof(SYSTEMTIME));
		  SYSTEMTIME st;
		  memset(&st, 0, sizeof(SYSTEMTIME));

			do { 
				do {
					TCHAR sMaskSymbol = sMask.GetAt(0);
					TCHAR sDTSymbol = sDateTime.GetAt(0);
      
			  // If current mask symbol is a white space
			  // we need to get rid of it and delete
			  // the corresponding symbol in the date itself
			  if (isWhiteSpace(sMaskSymbol)) {

				if (isWhiteSpace(sDTSymbol)) {

				  // we know here that mask symbol is a space
				  // and date contains space
				  // both symbol can be cut off
				  if (sMaskSymbol == sDTSymbol) {
							sMask = sMask.Right(sMask.GetLength()-1);
							sDateTime = sDateTime.Right(sDateTime.GetLength()-1);
				  }
				  else return false; // <- mask and date have space but it is not the same character
				}
				else return false; // <- mask has space symbol but the date does not


			  }

				} while (!sMask.IsEmpty() && isWhiteSpace(sMask.GetAt(0)));

			CString pPart;
				if (!sMask.IsEmpty() && !extractSymbol(sMask, sDateTime, &st)) 
					return false;

			} while(!sMask.IsEmpty() && !sDateTime.IsEmpty());

			//mask is empty but value is not OR value is empty but mask is not
			if ((sMask.IsEmpty() && !sDateTime.IsEmpty())
				|| (!sMask.IsEmpty() && sDateTime.IsEmpty()))
				return false;

		  //constructing boost::gregorian::date with invalid
		  //date throws std::out_of_range exception
		  //this way we can find out if the date is valid!
		  try {
			boost::gregorian::date d(st.wYear, st.wMonth, st.wDay);
			st.wDayOfWeek = d.day_of_week();
		  }
		  catch (std::out_of_range) {
			return false;
		  }

		  //final checking (mainly range of values)
		  if (!isValid(st))
			return false;

		  if (pTimeStruct)
			*pTimeStruct = st;

 
			return true;

		}; //validate

		bool 
		DateTime::extractPart(CString& pSource, CString& pTarget, const int& pCount) {

			if (pSource.GetLength() < pCount) return false;
			pTarget = pSource.Left(pCount);
			pSource = pSource.Right(pSource.GetLength()-pCount);
			return true;

		}; //extractPart

		bool 
		DateTime::extractSymbol(CString& pMask, CString& pDateTime, SYSTEMTIME* pTimeStruct) {

		  int iPart;
			int iPath = -1;
			TCHAR symbol = pMask.GetAt(0);
			pMask = pMask.Right(pMask.GetLength()-1);
			for (int i=0; i  < LAST; i++) {
				if (mSymbols[i] == symbol) {
					iPath = i;
					break;
				};
			};

			CString sPart;
	
			if (iPath < 0)
				throw Exception(_T("Date/time convertion error!!!"));

			switch (iPath) {
			  case YEAR_4DIGIT:
						if (!extractPart(pDateTime, sPart, 4))
							return false;
						if (!Numbers::isInt(sPart))
							return false;
				if (pTimeStruct) {
				  iPart = Numbers::toInt(sPart);
				  pTimeStruct->wYear = (WORD)iPart;
				}
					break;
					case YEAR_2DIGIT: 
						if (!extractPart(pDateTime, sPart, 2))
							return false;
						if (!Numbers::isInt(sPart))
							return false;
				if (pTimeStruct) 
				{
				  iPart = Numbers::toInt(sPart);
				  //fixing issue realted to two digit year conversion into a four digit year
				  if (iPart <= 99)
					  iPart += 2000; //assuming that it is always about 21 century
				  pTimeStruct->wYear = (WORD)iPart;
				}
				break;

				case DAY_2DIGIT: {
					if (!extractPart(pDateTime, sPart, 2))
						return false;
					if (!Numbers::isInt(sPart))
						return false;
					int iDays = Numbers::toInt(sPart);
					if (iDays < 1 || iDays > 31)
						return false;
					else if (pTimeStruct)
						pTimeStruct->wDay = (WORD)iDays;
					}

					break;

				case DAY_1DIGIT:
					{
						CString sTmpDateTime(pDateTime);
						CString sTmpPart;
						//first try with two digits
						if ((false == extractPart(sTmpDateTime, sTmpPart, 2)) || (false == Numbers::isInt(sTmpPart)))
						{
							//now try with one digit only
							if (false == extractPart(pDateTime, sPart, 1))
								return false;
						}
						else
						{
							//one more time on real variables
							extractPart(pDateTime, sPart, 2);
						}

						if (false == Numbers::isInt(sPart))
							return false;

						int iDay = Numbers::toInt(sPart);
						if (iDay < 1 || iDay > 31)
							return false;
						else if (pTimeStruct)
							pTimeStruct->wDay = (WORD)iDay;

					}
				break;

				case MONTH_2DIGIT: {
						if (!extractPart(pDateTime, sPart, 2))
							return false;
						if (!Numbers::isInt(sPart))
							return false;
						int iMonth = Numbers::toInt(sPart);
						if (iMonth < 1 || iMonth > 12)
							return false;
				else if (pTimeStruct)
				  pTimeStruct->wMonth = (WORD)iMonth;
				}
			  break;
			  case MONTH_1DIGIT:
				  {
					  //first try two digits, if it will faile than one digit
					  CString sTmpDateTime(pDateTime);
					  CString sTmpPart;
					  if (false == extractPart(sTmpDateTime, sTmpPart, 2) ||(false == Numbers::isInt(sTmpPart)))
					  {
						  //try with one digit
						  if (false == extractPart(pDateTime, sPart, 1))
							  return false;
					  }
					  else
					  {
						  //it worked with two digits, now we have to repeat it on real variables
						  extractPart(pDateTime, sPart, 2);
					  }
					  if (false == Numbers::isInt(sPart))
						  return false;
					  int iMonth = Numbers::toInt(sPart);
					  if (iMonth < 1 && iMonth > 12)
						  return false;
					  else if (pTimeStruct)
						  pTimeStruct->wMonth = iMonth;

				  }
			  break;
			  case HOUR12_2:
			  case HOUR12: {
						if (!extractPart(pDateTime, sPart, 2))
							return false;
						if (!Numbers::isInt(sPart))
							return false;
						int iHours = Numbers::toInt(sPart);
						if (iHours < 0 || iHours > 12)
							return false;
				else if (pTimeStruct)
				  pTimeStruct->wHour = (WORD)iHours;
						}
					  break;
					case HOUR24: {
						if (!extractPart(pDateTime, sPart, 2))
							return false;
						if (!Numbers::isInt(sPart))
							return false;
						int iHours = Numbers::toInt(sPart);
						if (iHours < 0 || iHours > 24)
							return false;
				else if (pTimeStruct)
				  pTimeStruct->wHour = (WORD)iHours;          
						}
						break;
			  case MINUTE: 
				{
				if (!extractPart(pDateTime, sPart, 2))
				  return false;
				if (!Numbers::isInt(sPart))
				  return false;
				int iMinutes = Numbers::toInt(sPart);
				if (iMinutes < 0 || iMinutes > 59)
				  return false;
				else if (pTimeStruct)
				  pTimeStruct->wMinute = (WORD)iMinutes;
				}
				break;
			  case SECOND:
				{
				if (!extractPart(pDateTime, sPart, 2))
				  return false;
				if (!Numbers::isInt(sPart))
				  return false;
				int iSeconds = Numbers::toInt(sPart);
				if (iSeconds < 0 || iSeconds > 59)
				  return false;
				else if (pTimeStruct)
				  pTimeStruct->wSecond = (WORD)iSeconds;
				}
			  break;
			  case AM_PM:
				  {
					  if (!extractPart(pDateTime, sPart, 2))
						  return false;
					  if ((0 != sPart.CompareNoCase(_T("AM"))) &&(0 != sPart.CompareNoCase(_T("PM"))))
						  return false;
              
					  if (pTimeStruct && (pTimeStruct->wHour <= 12) && (0 == sPart.CompareNoCase(_T("PM"))))
						  pTimeStruct->wHour += 12;
				  }
			  break;
			  default:
				return false;
			};

			return true;

		}; //extractSymbol


		CString 
		DateTime::timeToString(const CString& pMask, const SYSTEMTIME& pTime) {

  
		  CString sDate, s;
		  CString sMask = pMask;
		  if (sMask.IsEmpty())
			return SIEmptyString;

		  do {

			//take first character from right of the string
			TCHAR sMaskSymbol = sMask.GetAt(0);

			if (isWhiteSpace(sMaskSymbol)) {

			  //add white space symbol to the output
			  sDate += sMaskSymbol;      
			}
			else { //if it is some kind of symbol process it

			  //find the sMaskSymbol among known symbols
			  int iPath = -1;
			  for (int i=0; i < LAST; i++) 
				if (mSymbols[i] == sMaskSymbol) {
				  iPath = i;
				  break;
				}

			  if (iPath < 0) //return error if symbol is unknown (not found)!
					return SIEmptyString;

			  switch (iPath) {
				case YEAR_4DIGIT:
				  s.Format(_T("%d"), pTime.wYear);
				  addLeadingZeros(s, 4);
				  break;
				case YEAR_2DIGIT:
				  s.Format(_T("%d"), pTime.wYear);
				  s = s.Right(2);
				  addLeadingZeros(s, 2);
				  break;
				case MONTH_2DIGIT:
				  s.Format(_T("%d"), pTime.wMonth);
				  addLeadingZeros(s, 2);
				  break;
				case MONTH_1DIGIT:
				  s.Format(_T("%d"), pTime.wMonth);
				break;
				case DAY_2DIGIT:
				  s.Format(_T("%d"), pTime.wDay);
				  addLeadingZeros(s, 2);
				  break;
				case DAY_1DIGIT:
					s.Format(_T("%d"), pTime.wDay);
				break;
				case HOUR12_2:
				case HOUR12:
				  {
					 int hour = pTime.wHour % 12;
					 s.Format(_T("%d"), hour);
					 addLeadingZeros(s, 2);
				  }                 
				  break;
				case HOUR24:
					s.Format(_T("%d"), pTime.wHour);
					addLeadingZeros(s, 2);
				  break;
				case MINUTE:
					s.Format(_T("%d"), pTime.wMinute);
					addLeadingZeros(s, 2);
				  break;
				case SECOND:
					s.Format(_T("%d"), pTime.wSecond);
					addLeadingZeros(s, 2);
				  break;
				case AM_PM:
					s.Format(_T("%s"), pTime.wHour > 12 ? _T("PM") : _T("AM"));
				break;

			  }; //switch

			  sDate += s;
			}

			//eat symbol that has been just processed
			//(so if the mask was Y-m-d, after this line
			// it will be -m-d)
			sMask = sMask.Right(sMask.GetLength()-1);

		   } while (!sMask.IsEmpty());

		  return sDate;

		}; //timeToString

		void 
		DateTime::addLeadingZeros(CString& pValue, const int& pExpectedLength) {

  
		  if (pValue.GetLength() >= pExpectedLength)
			return;
		  int count = pExpectedLength - pValue.GetLength();

		  for (int i=0; i < count; i++)
			pValue.Insert(0, _T("0"));

		}; //addLeadingZeros


		bool 
		DateTime::isValid(const SYSTEMTIME& pSysTime) {


		  //FILETIME remembers dates after 1600
		  if (pSysTime.wYear < 1600 || pSysTime.wYear > 9999)
			return false;

		  if (pSysTime.wMonth < 1 || pSysTime.wMonth > 12)
			return false;

		  if (pSysTime.wDay < 1 || pSysTime.wDay > 31)
			return false;


		  try {
			boost::gregorian::date(pSysTime.wYear, pSysTime.wMonth, pSysTime.wDay);
		  }
		  catch (std::out_of_range) {
			return false;
		  }

		  if (pSysTime.wDayOfWeek < 0 || pSysTime.wDayOfWeek > 6)
			return false;

		  if (pSysTime.wHour < 0 || pSysTime.wHour > 23)
			return false;

		  if (pSysTime.wMinute < 0 || pSysTime.wMinute > 59)
			return false;

		  if (pSysTime.wSecond < 0 || pSysTime.wSecond > 59)
			return false;

		  if (pSysTime.wMilliseconds < 0 || pSysTime.wMilliseconds > 999)
			return false;

		  return true;

		}; //isValid


		int 
		DateTime::getDayOfWeek(int pYear, int pMonth, int pDay) {

		  boost::gregorian::date d(pYear, pMonth, pDay);  
		  return d.day_of_week();

		}; //getDayOfWeek



		SYSTEMTIME 
		DateTime::getCurrentDateTime() {

		  COleDateTime now = COleDateTime::GetCurrentTime();
		  SYSTEMTIME st;
		  now.GetAsSystemTime(st);
  

		  return st;

		}; // getCurrentDateTime


		int 
		DateTime::compareTimes(const SYSTEMTIME& pDateTime1, const SYSTEMTIME& pDateTime2) {

		  FILETIME file_datetime1, file_datetime2;
		  SystemTimeToFileTime(&pDateTime1, &file_datetime1);
		  SystemTimeToFileTime(&pDateTime2, &file_datetime2);

		  return CompareFileTime(&file_datetime1, &file_datetime2);

		}; //compareTimes


		void 
		DateTime::addPeriod(SYSTEMTIME& pDateTime, const int& pDays, const int& pHours, const int& pMinutes, const int& pSeconds) {

		  CTime datetime(pDateTime);
		  CTimeSpan span(pDays, pHours, pMinutes, pSeconds);

		  datetime += span;

		  if (!datetime.GetAsSystemTime(pDateTime)) {
			Exception ex(_T("addPeriod: Conversion error!"));
			throw ex;
		  }

		  //after adding CTimeSpan day of week becomes invalid
		  //this must be done to make it right!!
		  pDateTime.wDayOfWeek = getDayOfWeek(pDateTime.wYear, pDateTime.wMonth,
			pDateTime.wDay);

		}; //addPeriod


		CString DateTime::convertToSystemDateFormat(const SYSTEMTIME& pDateTime, bool bLong)
		{

		  CString sBuffer;
		  int iCount = GetDateFormat(LOCALE_USER_DEFAULT, bLong ? DATE_LONGDATE : DATE_SHORTDATE, 
			&pDateTime, NULL, NULL, 0);

		  iCount = GetDateFormat(LOCALE_USER_DEFAULT, bLong ? DATE_LONGDATE : DATE_SHORTDATE, 
			&pDateTime, NULL, sBuffer.GetBuffer(iCount), iCount);

		  sBuffer.ReleaseBuffer();

		  if (iCount > 0)
			return sBuffer;
		  else
		  {
			iCount = GetDateFormat(LOCALE_SYSTEM_DEFAULT, bLong ? DATE_LONGDATE : DATE_SHORTDATE, 
			  &pDateTime, NULL, NULL, 0);

			iCount = GetDateFormat(LOCALE_SYSTEM_DEFAULT, bLong ? DATE_LONGDATE : DATE_SHORTDATE, 
			  &pDateTime, NULL, sBuffer.GetBuffer(iCount), iCount);

			sBuffer.ReleaseBuffer();

			if (iCount > 0)
			  return sBuffer;
		  }

		  throw fw::core::Exception(_T("Conversion to system date format failed."));

		  return sBuffer;

		}


		CString DateTime::convertToSystemTimeFormat(const SYSTEMTIME& pDateTime)
		{
		  CString sBuffer;
		  int iCount = GetTimeFormat(LOCALE_USER_DEFAULT, 0, &pDateTime, NULL, NULL, 0);

		  iCount = GetTimeFormat(LOCALE_USER_DEFAULT, 0, &pDateTime, NULL, sBuffer.GetBuffer(iCount), iCount);

		  sBuffer.ReleaseBuffer();

		  if (iCount > 0)
			return sBuffer;
		  else
		  {
			iCount = GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &pDateTime, NULL, NULL, 0);

			iCount = GetTimeFormat(LOCALE_SYSTEM_DEFAULT, 0, &pDateTime, NULL, sBuffer.GetBuffer(iCount), iCount);

			sBuffer.ReleaseBuffer();

			if (iCount > 0)
			  return sBuffer;
		  }

		  throw fw::core::Exception(_T("Conversion to system date format failed."));


		}


		CString DateTime::getSystemDateMask(bool bShort)
		{
			CString sBuffer;
			LCTYPE lcType = bShort ? LOCALE_SSHORTDATE : LOCALE_SLONGDATE;

			int cnt = GetLocaleInfo(LOCALE_USER_DEFAULT, lcType, NULL, 0);
			GetLocaleInfo(LOCALE_USER_DEFAULT, lcType, sBuffer.GetBuffer(cnt), cnt);
			sBuffer.ReleaseBuffer();

			if (true == sBuffer.IsEmpty())
			{
				cnt = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, lcType, NULL, 0);
				GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, lcType, sBuffer.GetBuffer(cnt), cnt);
				sBuffer.ReleaseBuffer();
			}

			return sBuffer;
		}


		CString DateTime::getSystemTimeMask()
		{
			CString sBuffer;

			int cnt = GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, NULL, 0);
			GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIMEFORMAT, sBuffer.GetBuffer(cnt), cnt);
			sBuffer.ReleaseBuffer();

			if (true == sBuffer.IsEmpty())
			{
				cnt = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_STIMEFORMAT, NULL, 0);
				GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_STIMEFORMAT, sBuffer.GetBuffer(cnt), cnt);
				sBuffer.ReleaseBuffer();
			}

			return sBuffer;

		}


		bool DateTime::stringToTimeUsingSystemSettings(const CString& pDateTimeString, bool bDatePart, bool bTimePart, SYSTEMTIME* pSystemTime, bool bAcceptIfConversionSucceeded, const CString& pDateTimeSeparator)
		{

			if ((true == bDatePart) || (true == bTimePart))
			{
				CString sDateTimeString(pDateTimeString);
				sDateTimeString.TrimRight();

				COleDateTime oleDateTime;
				bool bResult = false;
				if ((true == bDatePart) && (true == bTimePart))
					bResult = oleDateTime.ParseDateTime(sDateTimeString);
				else if (true == bDatePart)
					bResult = oleDateTime.ParseDateTime(sDateTimeString, VAR_DATEVALUEONLY);
				else if (true == bTimePart)
					bResult = oleDateTime.ParseDateTime(sDateTimeString, VAR_TIMEVALUEONLY);
		
				SYSTEMTIME st;
				if ((true == bResult) && (true == oleDateTime.GetAsSystemTime(st)))
				{

					if (NULL != pSystemTime)
						*pSystemTime = st;		

					if (true == bAcceptIfConversionSucceeded)
						return true;		

					//make sure that if we convert back to the string, value is the same
					CString sDatePart;
					CString sTimePart;
					CString sDateAndTimeTogether;
					if (true == bDatePart)
						sDatePart = fw::core::DateTime::convertToSystemDateFormat(st);

					if (true == bTimePart)
						sTimePart = fw::core::DateTime::convertToSystemTimeFormat(st);

					sDateAndTimeTogether.Format(_T("%s%s%s"), sDatePart, pDateTimeSeparator, sTimePart);

					if ((true == bDatePart) && (true == bTimePart) && (sDateAndTimeTogether == sDateTimeString))
						return true;
					else if ((true == bDatePart) && (sDateTimeString == sDatePart))
						return true;
					else if ((true == bTimePart) && (sDateTimeString == sTimePart))
						return true;

					return false;

				}
			}

			return false;
	
		}


		bool DateTime::convertSystemMaskToSomiftMask(const CString& pSystemMask, CString& pSomiftMask)
		{
			CString sSysMask(pSystemMask);
			pSomiftMask.Empty();

			while (false == sSysMask.IsEmpty())
			{
				int iCurrentSymbol = 0;
				bool bHandled = false;
				while (iCurrentSymbol < LAST)
				{
					CString sCurrentSymbol = mSystemSymbols[iCurrentSymbol];
					if (sSysMask.Find(sCurrentSymbol) == 0)
					{
						CString sCurrentSomiftSymbol(mSymbols[iCurrentSymbol]);
						pSomiftMask += sCurrentSomiftSymbol;
						sSysMask = sSysMask.Right(sSysMask.GetLength() - sCurrentSymbol.GetLength());
						bHandled = true;
						iCurrentSymbol = LAST; //quit current itteration and start over
					}
					else if (isWhiteSpace(sSysMask.GetAt(0)))
					{
						//add mask to the result mask and continue
						pSomiftMask += sSysMask.GetAt(0);
						sSysMask = sSysMask.Right(sSysMask.GetLength() - 1);
						bHandled = true;
						iCurrentSymbol = LAST;
					}
					iCurrentSymbol++;
				}

				//we went through all symbols and we did not much anything (not even white space)
				if (false == bHandled)
				{
					pSomiftMask.Empty();
					return false;
				}
			}

			return true;

		} 


		int DateTime::getTimeSpanAsSeconds(const SYSTEMTIME& pStartDateTime, const SYSTEMTIME& pEndDateTime)
		{
			
			std::tm start_tm;
			std::tm end_tm;

			start_tm.tm_sec = pStartDateTime.wSecond;
			start_tm.tm_min = pStartDateTime.wMinute;
			start_tm.tm_hour = pStartDateTime.wHour;
			start_tm.tm_mday = pStartDateTime.wDay;
			start_tm.tm_mon = pStartDateTime.wMonth - 1;
			start_tm.tm_year = pStartDateTime.wYear - 1900;
			start_tm.tm_isdst = -1;

			end_tm.tm_sec = pEndDateTime.wSecond;
			end_tm.tm_min = pEndDateTime.wMinute;
			end_tm.tm_hour = pEndDateTime.wHour;
			end_tm.tm_mday = pEndDateTime.wDay;
			end_tm.tm_mon = pEndDateTime.wMonth - 1;
			end_tm.tm_year = pEndDateTime.wYear - 1900;
			end_tm.tm_isdst = -1;

			std::time_t start_time_t = std::mktime(&start_tm);
			std::time_t end_time_t = std::mktime(&end_tm);

			return end_time_t - start_time_t;
		}

	} //namespace core

}; //fw