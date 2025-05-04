// XTFrameWnd.cpp : implementation of the CXTFrameWnd class.
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
#include "XTMemDC.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTCoolMenu.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTResizeFormView.h"
#include "XTResizePropertyPage.h"
#include "XTResizePropertySheet.h"
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
#include "XTFrameWnd.h"
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTDockWindow.h"
#include "XTMiniDockFrameWnd.h"
#include "XTCheckListBox.h"
#include "XTTipWindow.h"
#include "XTCommandsListBox.h"
#include "XTCustomizeAPI.h"
#include "XTCallbacks.h"
#include "XTFlatComboBox.h"
#include "XTBrowseEdit.h"
#include "XTCustomizePage.h"
#include "XTCustomizeSheet.h"
#include "XTRegistryManager.h"
#include "XTDockState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTFrameWnd
/////////////////////////////////////////////////////////////////////////////
CXTFrameWnd::CXTFrameWnd()
{

}

CXTFrameWnd::~CXTFrameWnd()
{

}

IMPLEMENT_DYNCREATE(CXTFrameWnd, CFrameWnd)

BEGIN_MESSAGE_MAP(CXTFrameWnd, CFrameWnd)
	//{{AFX_MSG_MAP(CXTFrameWnd)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
	ON_COMMAND(XT_IDC_CUSTOMIZE, OnCustomizeBar)
	ON_COMMAND_EX_RANGE(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnBarCheck)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnUpdateControlBarMenu)
	ON_COMMAND_EX_RANGE(XT_IDC_TOOLSMANAGER_MIN, XT_IDC_TOOLSMANAGER_MAX, OnToolsManager)
	ON_UPDATE_COMMAND_UI_RANGE(XT_IDC_TOOLSMANAGER_MIN, XT_IDC_TOOLSMANAGER_MAX, OnUpdateToolsManager)
	ON_UPDATE_COMMAND_UI(XT_IDC_CHEVRON, OnUpdateChevron)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTFrameWnd

CXTCoolMenu* CXTFrameWnd::GetCoolMenu()
{
    ASSERT(::IsWindow(m_hWnd)); return CXTFrameImpl::GetCoolMenuImpl(this);
}

CXTMenuBar* CXTFrameWnd::GetMenuBar()
{
	ASSERT(::IsWindow(m_hWnd)); return CXTFrameImpl::GetMenuBarImpl(this);
}

void CXTFrameWnd::InstallCoolMenus(const UINT* nIDToolBars, int nSize)
{
	CXTFrameImpl::InstallCoolMenusImpl(nIDToolBars, nSize, this);
}

void CXTFrameWnd::InstallCoolMenus(const UINT nIDToolBar)
{
	CXTFrameImpl::InstallCoolMenusImpl(nIDToolBar, this);
}

void CXTFrameWnd::LoadBarState(LPCTSTR lpszProfileName)
{
    CXTFrameImpl::LoadBarStateImpl(lpszProfileName, this);
}

void CXTFrameWnd::SaveBarState(LPCTSTR lpszProfileName) const
{
    CXTFrameImpl::SaveBarStateImpl(lpszProfileName, (CFrameWnd*)this);
}

void CXTFrameWnd::EnableDocking(DWORD dwDockStyle)
{
    CXTFrameImpl::EnableDockingImpl(dwDockStyle, this);
}

void CXTFrameWnd::EnableDockingEx(DWORD dwDockStyle, DWORD dwFlatStyle)
{
    CXTFrameImpl::EnableDockingExImpl(dwDockStyle, dwFlatStyle, this);
}

void CXTFrameWnd::DockControlBar(CControlBar* pBar, CDockBar* pDockBar, LPCRECT lpRect)
{
    CXTFrameImpl::DockControlBarImpl(pBar, pDockBar, lpRect, this);
}

void CXTFrameWnd::DockControlBar(CControlBar* pBar, UINT nDockBarID, LPCRECT lpRect)
{
    CXTFrameImpl::DockControlBarImpl(pBar, nDockBarID, lpRect, this);
}

void CXTFrameWnd::DockControlBarLeftOf(CControlBar* pBar, CControlBar* pBarLeftOf)
{
    CXTFrameImpl::DockControlBarLeftOfImpl(pBar, pBarLeftOf, this);
}

CMenu* CXTFrameWnd::GetMenu() const
{
    return CXTFrameImpl::GetMenuImpl((CFrameWnd*)this);
}

BOOL CXTFrameWnd::HideMenuItem(UINT nCommandID)
{
	return CXTFrameImpl::HideMenuItemImpl(nCommandID, this);
}

BOOL CXTFrameWnd::HideMenuItem(LPCTSTR lpszItem)
{
	return CXTFrameImpl::HideMenuItemImpl(lpszItem, this);
}

BOOL CXTFrameWnd::HideMenuItems(const UINT* nCommandIDs, int nSize)
{
	return CXTFrameImpl::HideMenuItemsImpl(nCommandIDs, nSize, this);
}

BOOL CXTFrameWnd::HideMenuItems(const LPCTSTR lpszItems, int nSize)
{
	return CXTFrameImpl::HideMenuItemsImpl(lpszItems, nSize, this);
}

void CXTFrameWnd::DrawMenuBarX()
{
    CFrameWnd::DrawMenuBar(); CXTFrameImpl::DrawMenuBarImpl(this);
}

void CXTFrameWnd::SetDockState(const CXTDockState& state)
{
    CXTFrameImpl::SetDockStateImpl(state, this);
}

void CXTFrameWnd::GetDockState(CXTDockState& state) const
{
    CXTFrameImpl::GetDockStateImpl(state, (CFrameWnd*)this);
}

BOOL CXTFrameWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CFrameWnd::PreCreateWindow(cs);
}

BOOL CXTFrameWnd::PreCreateWindow(CREATESTRUCT& cs, UINT uIcon) 
{
    if (!CFrameWnd::PreCreateWindow(cs))
        return FALSE;
    
    return CXTFrameImpl::PreCreateWindowImpl(cs, uIcon, this);
}

void CXTFrameWnd::OnSysColorChange() 
{
	CFrameWnd::OnSysColorChange();
    CXTFrameImpl::OnSysColorChangeImpl(this);
}

void CXTFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CFrameWnd::OnSettingChange(uFlags, lpszSection);
    CXTFrameImpl::OnSettingChangeImpl(uFlags, lpszSection, this);
}

void CXTFrameWnd::OnCustomizeBar()
{
    CXTFrameImpl::OnCustomizeBarImpl(this);
}

BOOL CXTFrameWnd::PreTranslateMessage(MSG* pMsg) 
{
    if (CXTFrameImpl::PreTranslateMessageImpl(pMsg, this))
        return TRUE;

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CXTFrameWnd::SwitchSDIView(CRuntimeClass *pViewClass, CDocument *pDocument/*=NULL*/, CCreateContext *pContext/*=NULL*/)
{
	// Get a pointer to the active view and set
	// the view's document.
	CView* pOldActiveView = GetActiveView();

	if (pDocument == NULL && pOldActiveView != NULL) {
		pDocument = pOldActiveView->GetDocument();
	}

	// Instantiate the create context for the new view.
	CCreateContext context;

	if (pContext == NULL)
	{
		context.m_pNewViewClass	  = pViewClass;
		context.m_pCurrentDoc	  = pDocument;
		context.m_pNewDocTemplate = pDocument ? pDocument->GetDocTemplate() : NULL;
		context.m_pLastView		  = pOldActiveView;
		context.m_pCurrentFrame	  = this;

		pContext = &context;
	}

	// Call the CFrameWnd::CreateView(...) method to create the new view.
	CView* pNewActiveView = DYNAMIC_DOWNCAST(CView, CreateView(pContext));
	ASSERT_KINDOF(CView, pNewActiveView);

	// Initialize the view and set the frames active
	// view to the one we just created.
	pNewActiveView->OnInitialUpdate();
	SetActiveView(pNewActiveView);
	RecalcLayout();

	delete pOldActiveView; 	
}

void CXTFrameWnd::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	// Initialize menu accelerators
    OnInitMenuPopupImpl(pPopupMenu, nIndex, bSysMenu);

    CFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

bool CXTFrameWnd::InitAccelManager()
{
	return InitAccelManagerImpl( this );
}

bool CXTFrameWnd::InitToolsManager(int iNormalIndex, int iWindowIndex/*=-1*/, int iArgPopupMenu/*=0*/, int iDirPopupMenu/*=0*/)
{
	return InitToolsManagerImpl( this, iNormalIndex, iWindowIndex, iArgPopupMenu, iDirPopupMenu );
}

bool CXTFrameWnd::InitOptionsManager()
{
	return InitOptionsManagerImpl( this );
}

BOOL CXTFrameWnd::OnToolsManager(UINT nID)
{
	return OnToolsManagerImpl( nID );
}

void CXTFrameWnd::OnUpdateToolsManager(CCmdUI* pCmdUI)
{
	OnUpdateToolsManagerImpl( pCmdUI );
}

void CXTFrameWnd::OnUpdateChevron(CCmdUI* pCmdUI)
{
	OnUpdateChevronImpl( pCmdUI );
}
