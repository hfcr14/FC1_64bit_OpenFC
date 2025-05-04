// XTDockColorSelector.cpp : implementation file
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
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTFunctions.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTToolBarPopupWnd.h"
#include "XTMemDC.h"
#include "XTWndShadow.h"
#include "XTHelpers.h"

#include "XTColorSelectorCtrl.h"
#include "XTDockWindow.h"
#include "XTDockColorSelector.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTDockColorSelector::CXTDockColorSelector()
{

}

CXTDockColorSelector::~CXTDockColorSelector()
{

}

BOOL CXTDockColorSelector::Create(CWnd* pParentWnd, UINT nID, LPCTSTR lpszCaption)
{
	return CXTDockWindow::Create(pParentWnd, nID, lpszCaption, CSize(152, 152), CBRS_SIZE_FIXED|CBRS_FLOATING);
}

IMPLEMENT_DYNAMIC(CXTDockColorSelector, CXTDockWindow)

BEGIN_MESSAGE_MAP(CXTDockColorSelector, CXTDockWindow)
	//{{AFX_MSG_MAP(CDockWinEx1)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_COMMAND(XT_IDC_BTN_CLOSE, OnButtonClose)
	ON_UPDATE_COMMAND_UI(XT_IDC_BTN_CLOSE, OnUpdateButtonClose)
	ON_MESSAGE(CPN_XT_SELENDOK, OnSelEndOK)
	ON_MESSAGE(XTWM_QUERYVISUALIZE, OnQueryVisualize)
END_MESSAGE_MAP()

void CXTDockColorSelector::OnButtonClose()
{
	GetDockingFrame()->ShowControlBar(this,FALSE,FALSE);
}


void CXTDockColorSelector::OnUpdateButtonClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

/////////////////////////////////////////////////////////////////////////////
// CXTDockColorSelector message handlers

int CXTDockColorSelector::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTDockWindow::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Create the tree control
	m_wndColors.Create(CRect(0, 0, 0, 0), this, CPS_XT_EXTENDED|CPS_XT_MORECOLORS, 0);
	m_wndColors.AddListener(GetOwner()->m_hWnd);
	
	SetDockWindowStyle(CBRS_SIZE_FIXED|CBRS_FLOAT_MULTI);
	
	// Set the child for the docking window.
	SetChild (&m_wndColors);
	
	return 0;
}

LRESULT CXTDockColorSelector::OnSelEndOK(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);
	
	m_wndColors.SelectColor((COLORREF)wParam);
	
	return 0;
}

LRESULT CXTDockColorSelector::OnQueryVisualize(WPARAM, LPARAM)
{
	return 1;
}

HBRUSH CXTDockColorSelector::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CXTDockWindow::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd == &m_wndColors)
	{
		pDC->SetBkColor( xtAfxData.bXPMode? xtAfxData.clrXPBarFace: xtAfxData.clr3DFace);
	}
	
	return hbr;
}

