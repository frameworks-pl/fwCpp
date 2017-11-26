#include "gtest.h"
#include <list>
#include <afxwin.h>
#include <atlstr.h>
#include <src/fw/core/fwdatetime.h>

TEST(DateTimeUtils, calculateSpanBetweenDates)
{
	CString sStart = _T("2017-11-26 12:00:00");
	CString sEnd = _T("2017-11-26 12:00:30");

	SYSTEMTIME startST;
	SYSTEMTIME endST;
	fw::core::DateTime::stringToTime(sStart, _T("Y-M-D H:i:s"), &startST);
	fw::core::DateTime::stringToTime(sEnd, _T("Y-M-D H:i:s"), &endST);

	int iSeconds = fw::core::DateTime::getTimeSpanAsSeconds(startST, endST);

	ASSERT_EQ(30, iSeconds);

}