// sidb.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif

static AFX_EXTENSION_MODULE sidbDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("sidb.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(sidbDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(sidbDLL);

    //initalize static structures of the class
    sidb::SISQLObject::init();

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("sidb.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(sidbDLL);
	}
	return 1;   // ok
}



//void SIDB_DLLEXPORT SIDBSetLogPointer(fw::debug::Logger* pLog)
//{
//  fw::debug::LogInit(pLog);
//}


const std::string sqlBEGIN_TRANSACTION("BEGIN TRANSACTION");
const std::string sqlCOMMIT("COMMIT");
const std::string sqlROLLBACK("ROLLBACK");
