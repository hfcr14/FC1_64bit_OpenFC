// XTPagerCtrl.cpp : implementation of the CXTPagerCtrl class.
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
#include "XTPagerCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPagerCtrl
/////////////////////////////////////////////////////////////////////////////
CXTPagerCtrl::CXTPagerCtrl()
{

}

CXTPagerCtrl::~CXTPagerCtrl()
{

}

IMPLEMENT_DYNAMIC(CXTPagerCtrl, CWnd)

BEGIN_MESSAGE_MAP(CXTPagerCtrl, CWnd)
	//{{AFX_MSG_MAP(CXTPagerCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTPagerCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	ASSERT_VALID(pParentWnd);   // must have a parent

	// initialize common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_PAGESCROLLER_CLASS;
	VERIFY(InitCommonControlsEx(&icex));

	CWnd* pWnd = this;
	return pWnd->Create(WC_PAGESCROLLER, NULL, dwStyle | WS_CLIPCHILDREN, rect, pParentWnd, nID);
}
