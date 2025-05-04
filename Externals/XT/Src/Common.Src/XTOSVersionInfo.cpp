// XTOSVersionInfo.cpp: implementation of the CXTOSVersionInfo class.
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

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTOSVersionInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTOSVersionInfo::CXTOSVersionInfo()
{
	// zero memory and set struct size.
	::ZeroMemory((OSVERSIONINFO*)this, sizeof(OSVERSIONINFO));
	dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	// get the Windows OS version information.
	::GetVersionEx(this);
}

CXTOSVersionInfo& CXTOSVersionInfo::Get()
{
    static CXTOSVersionInfo singleton;
    return singleton;
}
