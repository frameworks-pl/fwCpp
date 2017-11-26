#ifndef DATETIME_H_INCLUDED
#define DATETIME_H_INCLUDED

#include "fwexception.h"


namespace fw {

	namespace core {


		class DateTime 
		{
			public:
				enum SymbolsEnum {
					YEAR_4DIGIT,
					YEAR_2DIGIT,
					MONTH_2DIGIT,
					MONTH_1DIGIT,
					DAY_2DIGIT,
					DAY_1DIGIT,
					HOUR24,
					HOUR12,
					HOUR12_2,
					MINUTE,
					SECOND,
					AM_PM,

					LAST
				};

				/**
				  @brief This enum is used by compareTimes
				  which uses CompareFileTime to do the job.
				  Value return by CompreFileTime corresponds to the enum
				*/
				enum TimeEqualityEnum {
					FIRST_LESS_THAN_SECOND = -1,
					FIRST_EQUAL_SECOND = 0,
					FIRST_GREATER_THAN_SECOND = 1
				};


				DateTime();

				/**
				  @brief Transforms the provided string to system time structure
				  based on the mask! It also works as a validation method (return false
				  if transoframtion of string to SYSTEMTIME fails).
				  Returns true on success.
				*/

				static bool stringToTime(const CString& pDateTime, const CString& pMask, SYSTEMTIME* pTimeStruct = NULL);
				/**
				  @brief Transforms the provided system time to the string
				  based on the  mask!
				  Returns formated date/time or empty string on error.
				*/
				static CString timeToString(const CString& pMask, const SYSTEMTIME& pTime);

				/**
				  @brief Checks if the provided system time is valid.
				  All values are in the appropriate range!
				*/
				static bool isValid(const SYSTEMTIME& pSysTime);

				/**
				  @brief Calculates the day of week based on year, month, day
				  values.
				  The result is correlated with the WeekDayEnum (which in turn
				  is correlated with enum used by CTime::GetDayOfWeek.
				*/
				static int getDayOfWeek(int pYear, int pMonth, int pDay);

				/**
				  @brief Returns the current date/time as the system time
				*/
				static SYSTEMTIME getCurrentDateTime();

				/**
				  @brief Compares two system dates and returns one of the TimeEqualityEnum
				  values.
				*/
				static int compareTimes(const SYSTEMTIME& pDateTime1, const SYSTEMTIME& pDateTime2);

				/**
				  @brief Adds the specified period of time to the provided date/time
				  and returns the resulting date/time
				*/
				static void addPeriod(SYSTEMTIME& pDateTime, const int& pDays, const int& pHours = 0, const int& pMinutes = 0, const int& pSeconds = 0);

				/**
					@brief Returns number of seconds that passed between first date
					and second date.
				*/
				static int getTimeSpanAsSeconds(const SYSTEMTIME& pStartDateTime, const SYSTEMTIME& pEndDateTime);

				//returns string representing the date using user system settings
				//or default system settings
				//bLong specifies if the date should be converted to long or short format
				static CString convertToSystemDateFormat(const SYSTEMTIME& pDateTime, bool bLong = false);

				//the same as convertToSystemDateFormat but deals with time only
				static CString convertToSystemTimeFormat(const SYSTEMTIME& pDateTime);

				//returns current system time mask
				static CString getSystemTimeMask();

				//returns current system date mask
				static CString getSystemDateMask(bool bShort = true);

				//tries to convert a string into a date/time using system settings
				//returns true if the function succeeded (if pSystemTime is not NULL it will
				//receive the result
				//bDatePart <=> parse date part, bTime <=> parse time part
				static bool stringToTimeUsingSystemSettings(const CString& pDateTimeString, bool bDatePart = true, bool bTimePart = true, SYSTEMTIME* pSystemTime = NULL, bool bAcceptIfConversionSucceeded = false, const CString& pDateTimeSeparator = _T(" "));

				//converts system date/time mask to somift's internal mask
				//returns false if conversion cannot be done
				static bool convertSystemMaskToSomiftMask(const CString& pSystemMask, CString& pSomiftMask);


			protected:
				static inline bool isWhiteSpace(const TCHAR& pSymbol);
				static bool extractSymbol(CString& pMask, CString& pDateTime, SYSTEMTIME* pTimeStruct = NULL);
				static const TCHAR mSymbols[DateTime::LAST];
				static const TCHAR* mSystemSymbols[DateTime::LAST];
				static inline bool extractPart(CString& pSource, CString& pTarget, const int& pCount);
				static void addLeadingZeros(CString& pValue, const int& pExpectedLength);


		};

	} //namespace core

}; //namespace fw



#endif //DATETIME_H_INCLUDED