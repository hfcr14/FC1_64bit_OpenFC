// XTOleIPFrameWnd.cpp : implementation of the CXTOleIPFrameWnd class.
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
#include "XTOleIPFrameWnd.h"
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTDockWindow.h"
#include "XTMiniDockFrameWnd.h"
#include "XTDockState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTOleIPFrameWnd
/////////////////////////////////////////////////////////////////////////////

CXTOleIPFrameWnd::CXTOleIPFrameWnd()
{

}

CXTOleIPFrameWnd::~CXTOleIPFrameWnd()
{

}

IMPLEMENT_DYNCREATE(CXTOleIPFrameWnd, COleIPFrameWnd)

BEGIN_MESSAGE_MAP(CXTOleIPFrameWnd, COleIPFrameWnd)
	//{{AFX_MSG_MAP(CXTOleIPFrameWnd)
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
// CXTOleIPFrameWnd

CXTCoolMenu* CXTOleIPFrameWnd::GetCoolMenu()
{
    ASSERT(::IsWindow(m_hWnd));
    return CXTFrameImpl::GetCoolMenuImpl(this);
}

CXTMenuBar* CXTOleIPFrameWnd::GetMenuBar()
{
	ASSERT(::IsWindow(m_hWnd));
    return CXTFrameImpl::GetMenuBarImpl(this);
}

void CXTOleIPFrameWnd::InstallCoolMenus(const UINT* nIDToolBars, int nSize)
{
	CXTFrameImpl::InstallCoolMenusImpl(nIDToolBars, nSize, this);
}

void CXTOleIPFrameWnd::InstallCoolMenus(const UINT nIDToolBar)
{
	CXTFrameImpl::InstallCoolMenusImpl(nIDToolBar, this);
}

void CXTOleIPFrameWnd::LoadBarState(LPCTSTR lpszProfileName)
{
    CXTFrameImpl::LoadBarStateImpl(lpszProfileName, this);
}

void CXTOleIPFrameWnd::SaveBarState(LPCTSTR lpszProfileName) const
{
    CXTFrameImpl::SaveBarStateImpl(lpszProfileName, (CFrameWnd*)this);
}

void CXTOleIPFrameWnd::EnableDocking(DWORD dwDockStyle)
{
    CXTFrameImpl::EnableDockingImpl(dwDockStyle, this);
}

void CXTOleIPFrameWnd::EnableDockingEx(DWORD dwDockStyle, DWORD dwFlatStyle)
{
    CXTFrameImpl::EnableDockingExImpl(dwDockStyle, dwFlatStyle, this);
}

void CXTOleIPFrameWnd::DockControlBar(CControlBar* pBar, CDockBar* pDockBar, LPCRECT lpRect)
{
    CXTFrameImpl::DockControlBarImpl(pBar, pDockBar, lpRect, this);
}

void CXTOleIPFrameWnd::DockControlBar(CControlBar* pBar, UINT nDockBarID, LPCRECT lpRect)
{
    CXTFrameImpl::DockControlBarImpl(pBar, nDockBarID, lpRect, this);
}

void CXTOleIPFrameWnd::DockControlBarLeftOf(CControlBar* pBar, CControlBar* pBarLeftOf)
{
    CXTFrameImpl::DockControlBarLeftOfImpl(pBar, pBarLeftOf, this);
}

CMenu* CXTOleIPFrameWnd::GetMenu() const
{
    return CXTFrameImpl::GetMenuImpl((CFrameWnd*)this);
}

BOOL CXTOleIPFrameWnd::HideMenuItem(UINT nCommandID)
{
	return CXTFrameImpl::HideMenuItemImpl(nCommandID, this);
}

BOOL CXTOleIPFrameWnd::HideMenuItem(LPCTSTR lpszItem)
{
	return CXTFrameImpl::HideMenuItemImpl(lpszItem, this);
}

BOOL CXTOleIPFrameWnd::HideMenuItems(const UINT* nCommandIDs, int nSize)
{
	return CXTFrameImpl::HideMenuItemsImpl(nCommandIDs, nSize, this);
}

BOOL CXTOleIPFrameWnd::HideMenuItems(const LPCTSTR lpszItems, int nSize)
{
	return CXTFrameImpl::HideMenuItemsImpl(lpszItems, nSize, this);
}

void CXTOleIPFrameWnd::DrawMenuBarX()
{
    COleIPFrameWnd::DrawMenuBar(); CXTFrameImpl::DrawMenuBarImpl(this);
}

void CXTOleIPFrameWnd::SetDockState(const CXTDockState& state)
{
    CXTFrameImpl::SetDockStateImpl(state, this);
}

void CXTOleIPFrameWnd::GetDockState(CXTDockState& state) const
{
    CXTFrameImpl::GetDockStateImpl(state, (CFrameWnd*)this);
}

BOOL CXTOleIPFrameWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return COleIPFrameWnd::PreCreateWindow(cs);
}

BOOL CXTOleIPFrameWnd::PreCreateWindow(CREATESTRUCT& cs, UINT uIcon) 
{
    if (!COleIPFrameWnd::PreCreateWindow(cs))
        return FALSE;
    
    return CXTFrameImpl::PreCreateWindowImpl(cs, uIcon, this);
}

void CXTOleIPFrameWnd::OnSysColorChange() 
{
	COleIPFrameWnd::OnSysColorChange();
    CXTFrameImpl::OnSysColorChangeImpl(this);
}

void CXTOleIPFrameWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	COleIPFrameWnd::OnSettingChange(uFlags, lpszSection);
    CXTFrameImpl::OnSettingChangeImpl(uFlags, lpszSection, this);
}

void CXTOleIPFrameWnd::OnCustomizeBar()
{
    CXTFrameImpl::OnCustomizeBarImpl(this);
}

BOOL CXTOleIPFrameWnd::PreTranslateMessage(MSG* pMsg) 
{
    if (CXTFrameImpl::PreTranslateMessageImpl(pMsg, this))
        return TRUE;

	return COleIPFrameWnd::PreTranslateMessage(pMsg);
}

void CXTOleIPFrameWnd::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	// Initialize menu accelerators
    OnInitMenuPopupImpl(pPopupMenu, nIndex, bSysMenu);

	COleIPFrameWnd::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
}

bool CXTOleIPFrameWnd::InitAccelManager()
{
	return InitAccelManagerImpl( this );
}

bool CXTOleIPFrameWnd::InitToolsManager(int iNormalIndex, int iWindowIndex/*=-1*/, int iArgPopupMenu/*=0*/, int iDirPopupMenu/*=0*/)
{
	return InitToolsManagerImpl( this, iNormalIndex, iWindowIndex, iArgPopupMenu, iDirPopupMenu );
}

bool CXTOleIPFrameWnd::InitOptionsManager()
{
	return InitOptionsManagerImpl( this );
}

BOOL CXTOleIPFrameWnd::OnToolsManager(UINT nID)
{
	return OnToolsManagerImpl( nID );
}

void CXTOleIPFrameWnd::OnUpdateToolsManager(CCmdUI* pCmdUI)
{
	OnUpdateToolsManagerImpl( pCmdUI );
}

void CXTOleIPFrameWnd::OnUpdateChevron(CCmdUI* pCmdUI)
{
	OnUpdateChevronImpl( pCmdUI );
}
