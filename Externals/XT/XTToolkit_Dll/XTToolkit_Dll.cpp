// XTToolkit_Dll.cpp : Defines the initialization routines for the DLL.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "..\Src\XTPriv.h"

#include <afxdllx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static AFX_EXTENSION_MODULE extensionDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("XTTOOLKIT_DLL.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(extensionDLL, hInstance))
			return 0;

#if !defined(XT_INIT_BY_REGULAR_DLL)
		// Create a new CDynLinkLibrary for this app.
		new CDynLinkLibrary(extensionDLL);

		CXTManageState::SetModuleInfo(extensionDLL.hResource, 
			extensionDLL.hModule);

		// Add other initialization here.
		xtAfxData.InitResources(extensionDLL.hResource);

#endif // !defined(XT_INIT_BY_REGULAR_DLL)
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("XTTOOLKIT_DLL.DLL Terminating!\n");
		// Terminate the library before destructors are called
		AfxTermExtensionModule(extensionDLL);
	}
	return 1;   // ok
}

#if defined(XT_INIT_BY_REGULAR_DLL)
// Exported DLL initialization is run in context of application
// or Regular DLL.
_XT_EXT_CLASS void WINAPI InitXtremeExtDLL()
{
	// Create a new CDynLinkLibrary for this app.
	new CDynLinkLibrary(extensionDLL);
	
	CXTManageState::SetModuleInfo(extensionDLL.hResource, 
		extensionDLL.hModule);

	// Add other initialization here.
	xtAfxData.InitResources(extensionDLL.hResource);
}
#endif // defined(XT_INIT_BY_REGULAR_DLL)
