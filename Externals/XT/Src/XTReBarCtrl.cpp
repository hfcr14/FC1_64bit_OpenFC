// XTReBarCtrl.cpp : implementation of the CXTReBarCtrl class.
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
#include "XTReBarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTReBarCtrl
/////////////////////////////////////////////////////////////////////////////
CXTReBarCtrl::CXTReBarCtrl()
{

}

IMPLEMENT_DYNAMIC(CXTReBarCtrl, CWnd)

BOOL CXTReBarCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                        UINT nID)
{
	// initialize common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_COOL_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));

	CWnd* pWnd = this;
	return pWnd->Create(REBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);
}

CImageList* CXTReBarCtrl::GetImageList() const
{
  REBARINFO rbi;
  rbi.cbSize = sizeof(rbi);
  rbi.fMask = RBIM_IMAGELIST;
  return GetBarInfo(&rbi) ? CImageList::FromHandle(rbi.himl) : NULL;
}

BOOL CXTReBarCtrl::SetImageList(CImageList* pImageList)
{
  REBARINFO rbi;
  rbi.cbSize = sizeof(rbi);
  rbi.fMask = RBIM_IMAGELIST;
  rbi.himl = (HIMAGELIST)pImageList->GetSafeHandle();
  return SetBarInfo(&rbi);
}

BOOL CXTReBarCtrl::GetColorScheme(COLORSCHEME* lpcs)
{
  lpcs->dwSize = sizeof(COLORSCHEME);
  return SendMessage(RB_GETCOLORSCHEME, 0, (LPARAM)lpcs);
}

void CXTReBarCtrl::SetColorScheme(const COLORSCHEME* lpcs)
{
  ((COLORSCHEME*)lpcs)->dwSize = sizeof(COLORSCHEME);
  SendMessage(RB_SETCOLORSCHEME, 0, (LPARAM)lpcs);
}

