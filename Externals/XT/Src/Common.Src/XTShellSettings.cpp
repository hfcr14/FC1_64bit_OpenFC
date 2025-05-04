// XTShellSettings.cpp: implementation of the CXTShellSettings class.
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
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTShellSettings.h"

//////////////////////////////////////////////////////////////////////
// CXTShellSettings

CXTShellSettings::CXTShellSettings()
{
	::ZeroMemory((SHELLFLAGSTATE*)this, sizeof(SHELLFLAGSTATE));

	typedef void (WINAPI* _SHGET)(LPSHELLFLAGSTATE lpsfs, DWORD dwMask);
	_SHGET pfn = (_SHGET)::GetProcAddress(
		(HINSTANCE)CXTLoadLibrary(_T("shell32.dll")), "SHGetSettings");

	if(pfn != NULL) 
	{   
		pfn(this,
			SSF_DESKTOPHTML          | 
			SSF_DONTPRETTYPATH       | 
			SSF_DOUBLECLICKINWEBVIEW | 
			SSF_HIDEICONS            |
			SSF_MAPNETDRVBUTTON      | 
			SSF_NOCONFIRMRECYCLE     | 
			SSF_SHOWALLOBJECTS       | 
			SSF_SHOWATTRIBCOL        | 
			SSF_SHOWCOMPCOLOR        |
			SSF_SHOWEXTENSIONS       |
			SSF_SHOWINFOTIP          |
			SSF_SHOWSYSFILES         |
			SSF_WIN95CLASSIC );
	}
}

CXTShellSettings::~CXTShellSettings()
{
	
}

//////////////////////////////////////////////////////////////////////
// 
