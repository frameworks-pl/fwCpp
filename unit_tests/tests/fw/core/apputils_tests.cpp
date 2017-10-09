#include "gtest.h"
#include <list>
#include <afxwin.h>
#include <atlstr.h>
#include <src/fw/core/fwapputils.h>
#include <src/fw/core/fwfileutils.h>

TEST(AppUtils, guidComparionTest)
{
	CString sGUID1 = _T("aeb18f9d-a60a-4e81-b535-076f1668f44f");
	CString sGUID2 = _T("AEB18F9D-A60A-4E81-B535-076F1668F44F");
    
	ASSERT_TRUE(fw::core::AppUtils::compareGUIDs(sGUID1, sGUID2));
	
}
