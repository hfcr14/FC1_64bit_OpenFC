// XTMDIFrameWnd.cpp : implementation of the CXTMDIFrameWnd class.
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
#include "XTRegistryManager.h"
#include "XTMemDC.h"
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
#include "XTMDIFrameWnd.h"
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTDockWindow.h"
#include "XTMiniDockFrameWnd.h"
#include "XTDockState.h"
#include "XTWindowList.h"
#include "XTMDIChildWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTMDIFrameWnd
/////////////////////////////////////////////////////////////////////////////
CXTMDIFrameWnd::CXTMDIFrameWnd()
{

}

CXTMDIFrameWnd::~CXTMDIFrameWnd()
{

}

IMPLEMENT_DYNCREATE(CXTMDIFrameWnd, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CXTMDIFrameWnd, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CXTMDIFrameWnd)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_INITMENUPOPUP()
	//}}AFX_MSG_MAP
	ON_COMMAND(XT_IDC_CUSTOMIZE, OnCustomizeBar)
	ON_COMMAND(AFX_IDM_FIRST_MDICHILD+9, OnWindowList)
	ON_COMMAND_EX_RANGE(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnBarCheck)
	ON_UPDATE_COMMAND_UI_RANGE(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, OnUpdateControlBarMenu)
	ON_COMMAND_EX_RANGE(XT_IDC_TOOLSMANAGER_MIN, XT_IDC_TOOLSMANAGER_MAX, OnToolsManager)
	ON_UPDATE_COMMAND_UI_RANGE(XT_IDC_TOOLSMANAGER_MIN, XT_IDC_TOOLSMANAGER_MAX, OnUpdateToolsManager)
	ON_UPDATE_COMMAND_UI(XT_IDC_CHEVRON, OnUpdateChevron)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTMDIFrameWnd

CXTCoolMenu* CXTMDIFrameWnd::GetCoolMenu()
{
    ASSERT(::IsWindow(m_hWnd)); return CXTFrameImpl::GetCoolMenuImpl(this);
}

CXTMenuBar* CXTMDIFrameWnd::GetMenuBar()
{
	ASSERT(::IsWindow(m_hWnd)); return CXTFrameImpl::GetMenuBarImpl(this);
}

void CXTMDIFrameWnd::InstallCoolMenus(const UINT* nIDToolBars, int nSize)
{
	CXTFrameImpl::InstallCoolMenusImpl(nIDToolBars, nSize, this);
}

void CXTMDIFrameWnd::InstallCoolMenus(const UINT nIDToolBar)
{
	CXTFrameImpl::InstallCoolMenusImpl(nIDToolBar, this);
}

void CXTMDIFrameWnd::LoadBarState(LPCTSTR lpszProfileName)
{
    CXTFrameImpl::LoadBarStateImpl(lpszProfileName, this);
}

void CXTMDIFrameWnd::SaveBarState(LPCTSTR lpszProfileName) const
{
    CXTFrameImpl::SaveBarStateImpl(lpszProfileName, (CFrameWnd*)this);
}

void CXTMDIFrameWnd::EnableDocking(DWORD dwDockStyle)
{
    CXTFrameImpl::EnableDockingImpl(dwDockStyle, this);
}

void CXTMDIFrameWnd::EnableDockingEx(DWORD dwDockStyle, DWORD dwFlatStyle)
{
    CXTFrameImpl::EnableDockingExImpl(dwDockStyle, dwFlatStyle, this);
}

void CXTMDIFrameWnd::DockControlBar(CControlBar* pBar, CDockBar* pDockBar, LPCRECT lpRect)
{
    CXTFrameImpl::DockControlBarImpl(pBar, pDockBar, lpRect, this);
}

void CXTMDIFrameWnd::DockControlBar(CControlBar* pBar, UINT nDockBarID, LPCRECT lpRect)
{
    CXTFrameImpl::DockControlBarImpl(pBar, nDockBarID, lpRect, this);
}

void CXTMDIFrameWnd::DockControlBarLeftOf(CControlBar* pBar, CControlBar* pBarLeftOf)
{
    CXTFrameImpl::DockControlBarLeftOfImpl(pBar, pBarLeftOf, this);
}

CMenu* CXTMDIFrameWnd::GetMenu() const
{
    return CXTFrameImpl::GetMenuImpl((CFrameWnd*)this);
}

BOOL CXTMDIFrameWnd::HideMenuItem(UINT nCommandID)
{
	return CXTFrameImpl::HideMenuItemImpl(nCommandID, this);
}

BOOL CXTMDIFrameWnd::HideMenuItem(LPCTSTR lpszItem)
{
	return CXTFrameImpl::HideMenuItemImpl(lpszItem, this);
}

BOOL CXTMDIFrameWnd::HideMenuItems(const UINT* nCommandIDs, int nSize)
{
	return CXTFrameImpl::HideMenuItemsImpl(nCommandIDs, nSize, this);
}

BOOL CXTMDIFrameWnd::HideMenuItems(const LPCTSTR lpszItems, int nSize)
{
	return CXTFrameImpl::HideMenuItemsImpl(lpszItems, nSize, this);
}

void CXTMDIFrameWnd::DrawMenuBarX()
{
    CMDIFrameWnd::DrawMenuBar(); CXTFrameImpl::DrawMenuBarImpl(this);
}

void CXTMDIFrameWnd::SetDockState(const CXTDockState& state)
{
    CXTFrameImpl::SetDockStateImpl(state, this);
}

void CXTMDIFrameWnd::GetDockState(CXTDockState& state) const
{
    CXTFrameImpl::GetDockStateImpl(state, (CFrameWnd*)this);
}

BOOL CXTMDIFrameWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CMDIFrameWnd::PreCreateWindow(cs);
}

BOOL CXTMDIFrameWnd::PreCreateWindow(CREATESTRUCT& cs, UINT uIcon) 
{
    if (!CMDIFrameWnd::PreCreateWindow(cs))
        return FALSE;
    
    return CXTFrameImpl::PreCreateWindowImpl(cs, uIcon, this);
}

void CXTMDIFrameWnd::OnSysColorChange() 
{
	CMDIFrameWnd::OnSysColorChange();
    CXTFrameImpl::OnSysColorChangeImpl(this);
}

void CXTMDIFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CMDIFrameWnd::OnSettingChange(uFlags, lpszSection);
    CXTFrameImpl::OnSettingChangeImpl(uFlags, lpszSection, this);
}

void CXTMDIFrameWnd::OnCustomizeBar()
{
    CXTFrameImpl::OnCustomizeBarImpl(this);
}

BOOL CXTMDIFrameWnd::PreTranslateMessage(MSG* pMsg) 
{
    if (CXTFrameImpl::PreTranslateMessageImpl(pMsg, this))
        return TRUE;

	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CXTMDIFrameWnd::OnWindowList()
{
	CXTWindowList dlg(this);
	
	// Try to locate the icon for the window by calling WM_GETICON
	// first, this will give us the correct icon if the user has called
	// SetIcon(...) for the child frame.
	HICON hIcon = (HICON)::SendMessage(m_hWnd, WM_GETICON, FALSE, 0);
	
	// If the icon returned is NULL, then try using GCL_HICONSM to get the
	// icon for the frame.
	if (hIcon == NULL) {
		hIcon = (HICON)::GetClassLong(m_hWnd, GCL_HICONSM);
	}

	// If the icon was found, set the icon for the window list dialog.
	if (hIcon != NULL) {
		dlg.SetWindowIcon(hIcon);
	}

	dlg.DoModal();
}

void CXTMDIFrameWnd::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	// Check to see if we have any active MDI windows, and check to see if it
	// is a CXTMDIChildWnd object, if it is then modify the windows menu...

	CXTMDIChildWnd* pMDIChildWnd = (CXTMDIChildWnd*)GetActiveFrame();
	if (pMDIChildWnd != NULL && pMDIChildWnd->IsKindOf(RUNTIME_CLASS(CXTMDIChildWnd)))
	{
		pMDIChildWnd->OnInitMenuPopupImpl(pPopupMenu, nIndex, bSysMenu);
	}

	// otherwise update the menu for the frame default.

	else
	{
		OnInitMenuPopupImpl(pPopupMenu, nIndex, bSysMenu);
	}

    CMDIFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

bool CXTMDIFrameWnd::InitAccelManager()
{
	return InitAccelManagerImpl( this );
}

bool CXTMDIFrameWnd::InitToolsManager(int iNormalIndex, int iWindowIndex/*=-1*/, int iArgPopupMenu/*=0*/, int iDirPopupMenu/*=0*/)
{
	return InitToolsManagerImpl( this, iNormalIndex, iWindowIndex, iArgPopupMenu, iDirPopupMenu );
}

bool CXTMDIFrameWnd::InitOptionsManager()
{
	return InitOptionsManagerImpl( this );
}

BOOL CXTMDIFrameWnd::OnToolsManager(UINT nID)
{
	return OnToolsManagerImpl( nID );
}

void CXTMDIFrameWnd::OnUpdateToolsManager(CCmdUI* pCmdUI)
{
	OnUpdateToolsManagerImpl( pCmdUI );
}

void CXTMDIFrameWnd::OnUpdateChevron(CCmdUI* pCmdUI)
{
	OnUpdateChevronImpl( pCmdUI );
}
