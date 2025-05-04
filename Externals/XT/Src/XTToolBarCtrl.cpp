// XTToolBarCtrl.cpp : implementation of the CXTToolBarCtrl class.
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
#include "XTToolBarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTToolBarCtrl
/////////////////////////////////////////////////////////////////////////////
CXTToolBarCtrl::~CXTToolBarCtrl()
{
  DestroyWindow();
}

CXTToolBarCtrl::CXTToolBarCtrl()
{
  // initialize common controls
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(icex);
  icex.dwICC = ICC_BAR_CLASSES;
  VERIFY(InitCommonControlsEx(&icex));
}

IMPLEMENT_DYNAMIC(CXTToolBarCtrl, CToolBarCtrl)

BEGIN_MESSAGE_MAP(CXTToolBarCtrl, CToolBarCtrl)
  //{{AFX_MSG_MAP(CXTToolBarCtrl)
  ON_WM_CREATE()
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTToolBarCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
                            UINT nID)
{
  CWnd* pWnd = this;
  BOOL bCreated = pWnd->Create(TOOLBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID);

  return bCreated;
}

int CXTToolBarCtrl::OnCreate(LPCREATESTRUCT lpcs)
{
  if (CWnd::OnCreate(lpcs) == -1)
    return -1;
  SetButtonStructSize(sizeof(TBBUTTON));
  return 0;
}

HRESULT CXTToolBarCtrl::GetDropTarget(IDropTarget** ppDropTarget) const
{
  ASSERT(::IsWindow(m_hWnd));
  ASSERT(ppDropTarget);
  return (HRESULT) ::SendMessage(m_hWnd, TB_GETOBJECT, (WPARAM)&IID_IDropTarget, (LPARAM)ppDropTarget);
}

#define XT_MESSAGE_TEXT	\
	"CXTToolBarCtrl::SetDisabledImageList( pImageList ) is DEPRECATED,\n"	\
	"use CXTToolBar::SetImageList( hImageList, FALSE ) to set the\n" \
	"disabled image list for your toolbar."

CImageList* CXTToolBarCtrl::SetDisabledImageList(CImageList* pImageList)
{
	UNREFERENCED_PARAMETER( pImageList );

	//
	// DEPRECATED: use CXTToolBar::SetImageList( pImageList->GetSafeHandle( ), FALSE );
	//
	XT_ASSERT_MSG( FALSE, XT_MESSAGE_TEXT );

	return NULL;
}
