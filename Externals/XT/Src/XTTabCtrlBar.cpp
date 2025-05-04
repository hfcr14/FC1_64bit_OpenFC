// XTTabCtrlBar.cpp : implementation of the CXTTabCtrlBar class.
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
#include "XTWndHook.h"
#include "XTTabCtrlButtons.h"
#include "XTTabCtrl.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTDockWindow.h"
#include "XTTabCtrlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrlBar

IMPLEMENT_DYNAMIC(CXTTabCtrlBar, CXTDockWindow)

BEGIN_MESSAGE_MAP(CXTTabCtrlBar, CXTDockWindow)
	//{{AFX_MSG_MAP(CXTTabCtrlBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(TCN_XT_SELCHANGE, OnTabSelChange)
	ON_MESSAGE(TCN_XT_SELCHANGING, OnTabSelChanging)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrlBar construction/destruction

CXTTabCtrlBar::CXTTabCtrlBar()
{
    // by default, the tabs will be set to auto-condense, to imitate the
    // popular Visual C++ behavior
    GetTabCtrl().SetAutoCondense(TRUE);
    GetTabCtrl().SendInitialUpdate(TRUE);
}

CXTTabCtrlBar::~CXTTabCtrlBar()
{
}

/////////////////////////////////////////////////////////////////////////////
// CXTTabView message handlers

int CXTTabCtrlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTDockWindow::OnCreate(lpCreateStruct) == -1)
		return -1;

	DWORD dwStyle = TCS_BOTTOM|TCS_TOOLTIPS|WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN;

	// Create the tab control used by the control bar.
	if (!GetTabCtrl().Create(dwStyle, CRect(0,0,0,0),
		this, AFX_IDC_TAB_CONTROL))
	{
		TRACE0( "Unable to create CXTTabCtrlBar.\n" );
		return -1;
	}

	// Associate the child window.
	SetChild(&GetTabCtrl());

	return 0;
}

LRESULT CXTTabCtrlBar::OnTabSelChange(WPARAM wParam, LPARAM lParam)
{
	OnTabSelChange((int)wParam, (CXTTabCtrl*)lParam);
	return 0;
}

LRESULT CXTTabCtrlBar::OnTabSelChanging(WPARAM wParam, LPARAM lParam)
{
	OnTabSelChanging((int)wParam, (CXTTabCtrl*)lParam);
	return 0;
}

void CXTTabCtrlBar::OnTabSelChange(int nIDCtrl, CXTTabCtrl* pTabCtrl)
{
	UNUSED_ALWAYS(nIDCtrl);

	// TODO: override in derived class to handle TCN_SELCHANGE 

	// Get a pointer to the owner window...
	CWnd* pWndOwner = GetOwner();
	ASSERT_VALID(pWndOwner);
	
	// and pass along this message.
	pWndOwner->SendMessage(TCN_XT_SELCHANGE,
		GetDlgCtrlID(), (LPARAM)pTabCtrl);
}

void CXTTabCtrlBar::OnTabSelChanging(int nIDCtrl, CXTTabCtrl* pTabCtrl)
{
	UNUSED_ALWAYS(nIDCtrl);
	
	// TODO: override in derived class to handle TCN_SELCHANGING 

	// Get a pointer to the owner window...
	CWnd* pWndOwner = GetOwner();
	ASSERT_VALID(pWndOwner);
	
	// and pass along this message.
	pWndOwner->SendMessage(TCN_XT_SELCHANGING,
		GetDlgCtrlID(), (LPARAM)pTabCtrl);

}

void CXTTabCtrlBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	// call the base class first.
	CXTDockWindow::OnUpdateCmdUI(pTarget, bDisableIfNoHndler);

	// If the parents active view belongs to us and we are either
	// inactive or floating, we need to set the frames active view to NULL.
	// We set the active view to NULL when floating because the view's
	// parent changes to CMiniDockFrameWnd and will case the main frame
	// to freeze until the control bar is re-docked.

	CView* pActiveView = m_pParentFrame->GetActiveView();
	if (pActiveView && IsChild(pActiveView) && (IsFloating() || !m_bActive))
	{
		CView* pView = GetTabCtrl().GetLastKnownChildView();
		if (pView && ::IsWindow(pView->m_hWnd))
        {
			m_pParentFrame->SetActiveView(pView);
		}
		else
        {
			m_pParentFrame->SetActiveView(NULL, FALSE);
		}
	}
}

#define XT_MSG_TEXT	"Directly using a CView object as a child of a CXTTabCtrlBar may have\nundesired results such as messages not correctly routing within the\nframework, see KB article \"005.007 - HOW TO: Use views with docking\nwindows \" for more details.\n\nIf you would like to use a CView derived class with a CXTTabCtrlBar\nobject, please use the CreateDockingView(...) and AddControl(...) methods\ninstead, for example:\n\nAddControl(m_strSolutionExplorer, CreateDockingView(\n   RUNTIME_CLASS(CFrameWnd), RUNTIME_CLASS(CTabFileView), NULL));"

BOOL CXTTabCtrlBar::AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CDocument* pDoc, CCreateContext *pContext, int iIndex, int iIconIndex)
{
	// AddView(...) depreciated see KB article "005.007 - HOW TO: Use views with docking windows" for more details.
    XT_ASSERT_MSG( FALSE, XT_MSG_TEXT );
    return GetTabCtrl().AddView(lpszLabel, pViewClass, pDoc, pContext, iIndex, iIconIndex);
}

BOOL CXTTabCtrlBar::AddView(LPCTSTR lpszLabel, CView* pView, int iIndex, int iIconIndex)
{
	// AddView(...) depreciated see KB article "005.007 - HOW TO: Use views with docking windows" for more details.
    XT_ASSERT_MSG( FALSE, XT_MSG_TEXT );
    return GetTabCtrl().AddView(lpszLabel, pView, iIndex, iIconIndex);
}
