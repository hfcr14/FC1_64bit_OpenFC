// XTWindowPos.cpp : implementation of the CXTWindowPos class.
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
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTWindowPos.h"
#include "XTRegistryManager.h"
#include "XTPriv.h"

/////////////////////////////////////////////////////////////////////////////
// CXTWindowPos
/////////////////////////////////////////////////////////////////////////////
CXTWindowPos::CXTWindowPos()
{
	memset(this, 0, sizeof(CXTWindowPos));
	length	= sizeof(CXTWindowPos);
	showCmd	= SW_SHOW;
}

CXTWindowPos::~CXTWindowPos()
{

}

BOOL CXTWindowPos::SaveWindowPos(CWnd* pWnd, LPCTSTR lpszWndPos/*=NULL*/)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	ASSERT(pWnd); // must be valid.
	
	if (::IsWindow(pWnd->m_hWnd))
	{
		if (pWnd->GetWindowPlacement(this))
		{
			CString strSection;
			strSection.LoadString(XT_IDS_REG_SETTINGS);

			CString strEntry;
			strEntry.LoadString(XT_IDS_REG_WNDPOS);

			if (lpszWndPos == NULL)
			{
				// Make sure we don't pop up 
				// minimized the next time
				if (showCmd != SW_SHOWMAXIMIZED) {
					showCmd  = SW_SHOWNORMAL;
				}
			}
			
			if (lpszWndPos != NULL) {
				strEntry = lpszWndPos;
			}

			CXTRegistryManager regManager;
			regManager.WriteProfileBinary(strSection, strEntry,
				reinterpret_cast<LPBYTE>(this), sizeof(CXTWindowPos));

			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CXTWindowPos::LoadWindowPos(CWnd* pWnd, LPCTSTR lpszWndPos/*=NULL*/)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	ASSERT_VALID( pWnd ); // must be valid.

	if(::IsWindow(pWnd->m_hWnd))
	{
		if (LoadWindowPos(lpszWndPos))
		{
			pWnd->SetWindowPlacement(this);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTWindowPos::LoadWindowPos(LPCTSTR lpszWndPos/*=NULL*/)
{
	UINT nSize = 0;
	LPBYTE pbtData = 0;

	CString strSection;
	strSection.LoadString(XT_IDS_REG_SETTINGS);

	CString strEntry;
	strEntry.LoadString(XT_IDS_REG_WNDPOS);

	if (lpszWndPos != NULL) {
		strEntry = lpszWndPos;
	}

	CXTRegistryManager regManager;
	if (regManager.GetProfileBinary(strSection, strEntry,
		&pbtData, &nSize))
	{
		::CopyMemory((void*)&*this, (const void*)
			reinterpret_cast<CXTWindowPos*>(pbtData),
			(DWORD)sizeof(CXTWindowPos));
		delete [] pbtData;

		return TRUE;
	}

	return FALSE;
}
