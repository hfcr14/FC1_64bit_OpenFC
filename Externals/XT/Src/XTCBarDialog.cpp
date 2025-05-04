// XTCBarDialog.cpp : implementation file
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
#include "XTDefines.h"
#include "XTResource.h"
#include "XTGlobal.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTCoolMenu.h"
#include "XTStatusBar.h"
#include "XTControlBar.h"
#include "XTMenuBar.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTCBarDialog.h"
#include "XTOSVersionInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTCBarDialog dialog

CXTCBarDialog::CXTCBarDialog()
: CXTResizeDialog()
{
	m_nIDTracking     = 0;
	m_nIDLastMessage  = 0;
	xtAfxData.bControlBarMenus = FALSE;
}

CXTCBarDialog::CXTCBarDialog(UINT nIDTemplate, CWnd* pParentWnd/*=NULL*/, const UINT nFlags)
: CXTResizeDialog(nIDTemplate, pParentWnd, nFlags)
{
	m_nIDTracking     = 0;
	m_nIDLastMessage  = 0;
	xtAfxData.bControlBarMenus = FALSE;
}

void CXTCBarDialog::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTCBarDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CXTCBarDialog, CXTResizeDialog)

BEGIN_MESSAGE_MAP(CXTCBarDialog, CXTResizeDialog)
	//{{AFX_MSG_MAP(CXTCBarDialog)
	ON_WM_INITMENUPOPUP()
	ON_WM_ENTERIDLE()
	ON_WM_MENUSELECT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_MESSAGE(WM_POPMESSAGESTRING, OnPopMessageString)
	// turning on and off standard frame gadgetry
	ON_UPDATE_COMMAND_UI(ID_VIEW_STATUS_BAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_STATUS_BAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_REBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_REBAR, OnBarCheck)
	// turning on and off standard mode indicators
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_CAPS, OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_NUM, OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_SCRL, OnUpdateKeyIndicator)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_KANA, OnUpdateKeyIndicator)
	// toolbar "tooltip" notification
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTCBarDialog message handlers

void CXTCBarDialog::RecalcBarLayout()
{
	// We need to resize the dialog to make room for control bars.
	// First, figure out how big the control bars are.
	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 
				   0, reposQuery, rcClientNow);
	
	// Now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.
	CPoint ptOffset(rcClientNow.left - rcClientStart.left,
					rcClientNow.top - rcClientStart.top); 

	CRect  rcChild;					
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild)
	{                               
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);

		if (pwndChild->IsKindOf(RUNTIME_CLASS(CControlBar))) {
			m_listControlBars.AddHead(pwndChild);
		}

		pwndChild = pwndChild->GetNextWindow();
	}

	// Adjust the dialog window dimensions
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	
	// And position the control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	// post message for initial message string
	PostMessage(WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
}

BOOL CXTCBarDialog::PreTranslateMessage(MSG* pMsg) 
{
	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	CXTMenuBar* pMenuBar = GetMenuBar();
	if (pMenuBar != NULL) 
	{
		if (pMenuBar->TranslateFrameMessage(pMsg))
		{
			return TRUE;
		}
	}
	
	// don't allow VK_ESCAPE to the dialog.
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_ESCAPE))
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}

	return FALSE;
}

void CXTCBarDialog::InstallCoolMenus(const UINT* nIDToolBars, int nSize)
{
	xtAfxData.bCoolMenuCompatMode = TRUE;
	m_hookMenu.Install((CFrameWnd*)this);
	m_hookMenu.LoadToolbars(nIDToolBars, nSize);
}

void CXTCBarDialog::InstallCoolMenus(const UINT nIDToolBar)
{
	InstallCoolMenus(&nIDToolBar, 1);
}

/////////////////////////////////////////////////////////////////////////////
// CXTCBarDialog::OnInitMenuPopup
//		OnInitMenuPopup updates the state of items on a popup menu.  
//
//  	This code is based on CFrameWnd::OnInitMenuPopup.  We assume the
//		application does not support context sensitive help.

void CXTCBarDialog::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	UNREFERENCED_PARAMETER(nIndex);

	if (!bSysMenu)
	{
		ASSERT(pPopupMenu != NULL);
		
		// check the enabled state of various menu items
		CCmdUI state;        
		state.m_pMenu = pPopupMenu;
		ASSERT(state.m_pOther == NULL);
		
		state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
		for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
			 state.m_nIndex++)
		{
			state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
			if (state.m_nID == 0)
				continue; // menu separator or invalid cmd - ignore it
				
			ASSERT(state.m_pOther == NULL);
			ASSERT(state.m_pMenu != NULL);
			if (state.m_nID == (UINT)-1)
			{
				// possibly a popup menu, route to first item of that popup
				state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
				if (state.m_pSubMenu == NULL ||
				    (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				    state.m_nID == (UINT)-1)
				{				    			 
			        continue; // first item of popup can't be routed to
			    }
			    state.DoUpdate(this, FALSE);  // popups are never auto disabled
			}
			else
			{
				// normal menu item
				// Auto enable/disable if command is _not_ a system command
				state.m_pSubMenu = NULL;
				state.DoUpdate(this, state.m_nID < 0xF000);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTCBarDialog::OnEnterIdle
//		OnEnterIdle updates the status bar when there's nothing better to do.
//  	This code is based on CFrameWnd::OnEnterIdle.

void CXTCBarDialog::OnEnterIdle(UINT nWhy, CWnd* pWho) 
{
	UNREFERENCED_PARAMETER(pWho);
	
	if (nWhy != MSGF_MENU || m_nIDTracking == m_nIDLastMessage)
		return;

	OnSetMessageString(m_nIDTracking);
	ASSERT(m_nIDTracking == m_nIDLastMessage);		
}

/////////////////////////////////////////////////////////////////////////////
// CModelessMain::OnMenuSelect
//		OnMenuSelect updates the status bar message, based on the state
//		of the dialog menu.  
//
//  	This code is based on CFrameWnd::OnMenuSelect.  We assume the
//		application does not support context sensitive help.

void CXTCBarDialog::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu) 
{
	UNREFERENCED_PARAMETER(hSysMenu);

	// set the tracking state
	if (nFlags == 0xFFFF)
	{
		// cancel menu operation (go back to idle now)
		m_nIDTracking = AFX_IDS_IDLEMESSAGE;
		OnSetMessageString(m_nIDTracking);	// set string now
		ASSERT(m_nIDTracking == m_nIDLastMessage);
	}
	else if (nItemID == 0 ||
			 nFlags & (MF_SEPARATOR|MF_POPUP|MF_MENUBREAK|MF_MENUBARBREAK))
	{
		// nothing should be displayed
		m_nIDTracking = 0;
	}
	else if (nItemID >= 0xF000 && nItemID < 0xF1F0)
	{
		// special string table entries for system commands	
		m_nIDTracking = ID_COMMAND_FROM_SC(nItemID);
		ASSERT(m_nIDTracking >= AFX_IDS_SCFIRST &&
			   m_nIDTracking < AFX_IDS_SCFIRST + 31);
	}
	else
	{
		// track on idle
		m_nIDTracking = nItemID;
	}		
}

LRESULT CXTCBarDialog::OnPopMessageString(WPARAM wParam, LPARAM lParam)
{
	if (m_nFlags & WF_NOPOPMSG)
		return 0;

	return SendMessage(WM_SETMESSAGESTRING, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CModelessMain::OnSetMessageString
//		OnSetMessageString updates the status bar text.  
//
//	  	This code is based on CFrameWnd::OnSetMessageString.  We assume
//	  	a string ID is always passed in wParam.

LRESULT CXTCBarDialog::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	
	UINT    nIDMsg = (UINT)wParam;
	CString strMsg;
	
	CWnd* pMessageBar = GetMessageBar();
	if (pMessageBar != NULL && nIDMsg)
	{
		if (strMsg.LoadString(nIDMsg) != 0)
			pMessageBar->SetWindowText(strMsg);
		else
			TRACE1("Warning: no message line prompt for ID %x%04X\n", nIDMsg);
	}   
	
	UINT nIDLast     = m_nIDLastMessage;
	m_nIDLastMessage = nIDMsg;
	m_nIDTracking    = nIDMsg;
	return nIDLast;  
}

CXTStatusBar* CXTCBarDialog::GetMessageBar()
{
	return (CXTStatusBar*)GetDescendantWindow(AFX_IDW_STATUS_BAR, TRUE);
}

CXTMenuBar* CXTCBarDialog::GetMenuBar()
{
	return (CXTMenuBar*)GetDescendantWindow(AFX_IDW_MENUBAR, TRUE);
}

void CXTCBarDialog::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
	ASSERT(ID_VIEW_REBAR == AFX_IDW_REBAR);

	CControlBar* pBar = (CControlBar*)GetDescendantWindow(pCmdUI->m_nID, TRUE);
	if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(CControlBar)))
	{
		pCmdUI->SetCheck((pBar->GetStyle() & WS_VISIBLE) != 0);
		return;
	}
	pCmdUI->ContinueRouting();
}

BOOL CXTCBarDialog::OnBarCheck(UINT nID)
{
	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
	ASSERT(ID_VIEW_REBAR == AFX_IDW_REBAR);

	CControlBar* pBar = (CControlBar*)GetDescendantWindow(nID, TRUE);
	if (pBar != NULL && pBar->IsKindOf(RUNTIME_CLASS(CControlBar)))
	{
		ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE);
		return TRUE;
	}
	return FALSE;
}

void CXTCBarDialog::ShowControlBar(CControlBar* pBar, BOOL bShow, BOOL bDelay)
{
	UNREFERENCED_PARAMETER(bDelay);

	pBar->ShowWindow( bShow ? SW_SHOWNA : SW_HIDE );
}

/////////////////////////////////////////////////////////////////////////////
// Support for standard status bar

void CXTCBarDialog::OnUpdateKeyIndicator(CCmdUI* pCmdUI)
{
	UINT nVK;
	UINT flag = 0x0001;

	switch (pCmdUI->m_nID)
	{
	case ID_INDICATOR_CAPS:
		nVK = VK_CAPITAL;
		break;

	case ID_INDICATOR_NUM:
		nVK = VK_NUMLOCK;
		break;

	case ID_INDICATOR_SCRL:
		nVK = VK_SCROLL;
		break;

	case ID_INDICATOR_KANA:
		nVK = VK_KANA;
		// WINBUG: Special case for Windows 3.x.  The wrong bit was toggled
		// in those systems so this must be special cased.  This is fixed
		// on systems whose version is 4.x or greater.
		if (!CXTOSVersionInfo::Get().IsWin95OrGreater())
			flag = 0x8000;
		break;

	default:
		TRACE1("Warning: OnUpdateKeyIndicator - unknown indicator 0x%04X.\n",
			pCmdUI->m_nID);
		pCmdUI->ContinueRouting();
		return; // not for us
	}

	pCmdUI->Enable(::GetKeyState(nVK) & flag);
		// enable static text based on toggled key state
	ASSERT(pCmdUI->m_bEnableChanged);
}

LRESULT CXTCBarDialog::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	SendMessageToDescendants(WM_IDLEUPDATECMDUI,
		(WPARAM)TRUE, 0, TRUE, TRUE);
	
	return 0;
}

void CXTCBarDialog::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);
	
	// And position the control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

BOOL CXTCBarDialog::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	TCHAR szFullText[256];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom is actually the HWND of the tool
		nID = _AfxGetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) // will be zero on a separator
	{
		// don't handle the message if no string resource found
		if (AfxLoadString(nID, szFullText) == 0)
			return FALSE;

		// this is the command id, not the button index
		AfxExtractSubString(strTipText, szFullText, 1, '\n');
	}
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		_mbstowcsz(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, _countof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, strTipText, _countof(pTTTW->szText));
#endif
	*pResult = 0;

	// bring the tooltip window above other popup windows
	::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,
		SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER);

	return TRUE;    // message was handled
}
