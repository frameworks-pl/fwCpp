// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#ifndef FWCOMMON_PCH_INCLUDED
#define FWCOMMON_PCH_INCLUDED

#define _USING_V110_SDK71_

#define _ATL_APARTMENT_THREADED
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS // some CString constructors will be explicit

// turns  off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>


#ifdef __linux
/*
** fix for gcc 3.4.3 build error "call of overloaded `abs(double)' is ambiguous"
*/
#include <cstdlib> //Definition of labs
#include <cmath> //Definition of fabs
static long int abs(long int _X)
{
    return (labs(_X));
}
static double abs(double _X)
{
    return (fabs(_X));
}

#endif //#ifdef __linux

////////////////////////////////////////////////////////////////////////////////
// TODO: verify this section
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxole.h>         // MFC OLE classes
//#include <afxodlgs.h>       // MFC OLE dialog classes
//#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

//#include <afxctl.h> //AfxConnectionAdvise and AfxConnectionUnadvise


#ifndef _AFX_NO_DB_SUPPORT
//#include <afxdb.h>          // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>         // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <windef.h>

//STL
#include <string>
#include <list>
#include <map>
#include <set>
#include <vector>

#include "fwcommon.h"

#endif //FWCOMMON_PCH_INCLUDED
