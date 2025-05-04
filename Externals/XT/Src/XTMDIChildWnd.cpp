// XTMDIChildWnd.cpp : implementation of the CXTMDIChildWnd class.
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
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTCoolMenu.h"
#include "XTMemDC.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTResizeFormView.h"
#include "XTResizePropertyPage.h"
#include "XTResizePropertySheet.h"
#include "XTFlatComboBox.h"
#include "XTAccelManager.h"
#include "XTAccelKeyEdit.h"
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrl.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTFrameImpl.h"
#include "XTMDIChildWnd.h"
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTDockWindow.h"
#include "XTMiniDockFrameWnd.h"
#include "XTRegistryManager.h"
#include "XTMDIFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTMDIChildWnd
/////////////////////////////////////////////////////////////////////////////
CXTMDIChildWnd::CXTMDIChildWnd()
{

}

CXTMDIChildWnd::~CXTMDIChildWnd()
{

}

IMPLEMENT_DYNCREATE(CXTMDIChildWnd, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CXTMDIChildWnd, CMDIChildWnd)
	//{{AFX_MSG_MAP(CXTMDIChildWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(XT_IDC_CUSTOMIZE, OnCustomizeBar)
	ON_COMMAND_EX_RANGE(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnBarCheck)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnUpdateControlBarMenu)
	ON_UPDATE_COMMAND_UI(XT_IDC_CHEVRON, OnUpdateChevron)
END_MESSAGE_MAP()

void CXTMDIChildWnd::DockControlBarLeftOf(CControlBar* pBar, CControlBar* pBarLeftOf)
{
    CXTFrameImpl::DockControlBarLeftOfImpl(pBar, pBarLeftOf, this);
}

void CXTMDIChildWnd::EnableDocking(DWORD dwDockStyle)
{
    CXTFrameImpl::EnableDockingImpl(dwDockStyle, this);
}

void CXTMDIChildWnd::EnableDockingEx(DWORD dwDockStyle, DWORD dwFlatStyle)
{
    CXTFrameImpl::EnableDockingExImpl(dwDockStyle, dwFlatStyle, this);
}

void CXTMDIChildWnd::DockControlBar(CControlBar* pBar, CDockBar* pDockBar, LPCRECT lpRect)
{
    CXTFrameImpl::DockControlBarImpl(pBar, pDockBar, lpRect, this);
}

void CXTMDIChildWnd::DockControlBar(CControlBar* pBar, UINT nDockBarID, LPCRECT lpRect)
{
    CXTFrameImpl::DockControlBarImpl(pBar, nDockBarID, lpRect, this);
}

void CXTMDIChildWnd::LoadBarState(LPCTSTR lpszProfileName)
{
    CXTFrameImpl::LoadBarStateImpl(lpszProfileName, this);
}

void CXTMDIChildWnd::SaveBarState(LPCTSTR lpszProfileName) const
{
    CXTFrameImpl::SaveBarStateImpl(lpszProfileName, (CFrameWnd*)this);
}

BOOL CXTMDIChildWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CMDIChildWnd::PreCreateWindow(cs);
}

BOOL CXTMDIChildWnd::PreCreateWindow(CREATESTRUCT& cs, UINT uIcon) 
{
    if (!CMDIChildWnd::PreCreateWindow(cs))
        return FALSE;
    
    return CXTFrameImpl::PreCreateWindowImpl(cs, uIcon, this);
}

void CXTMDIChildWnd::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    // MFCBUG : MFC assumes that there always is an active
    // view set for the child frame, but in some cases 
    // this does not hold true, and the frame title gets set to
    // NULL.  To get around this problem, we will first set
    // the active view for the frame if it is not already set
    // before we call OnUpdateFrameTitle in the base class.

    CWnd* pChildWnd = GetWindow(GW_CHILD);
    while (pChildWnd)
    {
        if (pChildWnd->IsKindOf(RUNTIME_CLASS(CView)))
        {
            CView* pChildView = (CView*)pChildWnd;
            
            // If for some reason the active view is not set for
            // the child frame set it now.
            if ((GetActiveView() != pChildView))
            {
                SetActiveView(pChildView);
            }
            
            break;
        }
        pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
    }

    // now call the base class.
    CMDIChildWnd::OnUpdateFrameTitle(bAddToTitle);
}

int CXTMDIChildWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

    // Hook cool menus for MDI children.
    m_hookCoolMenus.HookWindow(this);

    return 0;
}

void CXTMDIChildWnd::OnCustomizeBar()
{
    CXTFrameImpl::OnCustomizeBarImpl((CFrameWnd*)AfxGetMainWnd());
}

void CXTMDIChildWnd::OnUpdateChevron(CCmdUI* pCmdUI)
{
	OnUpdateChevronImpl( pCmdUI );
}
