// XTColorPopup.cpp : implementation file
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
#include "XTMemDC.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTColorSelectorCtrl.h"
#include "XTWndShadow.h"
#include "XTColorPopup.h"
#include "XTColorPageCustom.h"
#include "XTColorPageStandard.h"
#include "XTColorDialog.h"
#include "XTWndHook.h"
#include "XTHelpers.h"
#include "XTPriv.h"

#include "XTWndHook.h"

#ifndef _XTP_INCLUDE_CONTROLS
#include "XTDelegate.h"
#include "XTControlbar.h"
#include "XTToolBar.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTColorPopup

CXTColorPopup::CXTColorPopup(BOOL bAutoDelete/*=FALSE*/, LPARAM callerParam /*= 0*/)
: m_callerParam(callerParam)
{
	m_bAutoDelete	= bAutoDelete;
	m_bDisplayShadow= TRUE;
}

CXTColorPopup::~CXTColorPopup()
{

}

BEGIN_MESSAGE_MAP(CXTColorPopup, CXTColorSelectorCtrl)
	//{{AFX_MSG_MAP(CXTColorPopup)
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorPopup message handlers

BOOL CXTColorPopup::Create(CRect& rect, CWnd* pParentWnd, DWORD dwPopup, COLORREF clrColor, 
						   COLORREF clrDefault/*=CLR_DEFAULT*/) 
{
	SetBorders(4, 4, xtAfxData.bXPMode? 6: 10, xtAfxData.bXPMode? 6: 10);
	m_rcExclude = rect;

#ifndef _XTP_INCLUDE_CONTROLS
	if (xtAfxData.bXPMode && DYNAMIC_DOWNCAST(CXTToolBar, pParentWnd))rect.bottom--;
#endif

	return CXTColorSelectorCtrl::Create(rect, pParentWnd, dwPopup | WS_POPUP, clrColor, clrDefault);
}

void CXTColorPopup::PostNcDestroy() 
{
	if (m_bAutoDelete) {
		delete this;
	}
}

void CXTColorPopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// If we lose activation (other than to the color dialog) post
	// a close message...

	// TSF ÄNDERUNG 19.04.02
	if (nState == WA_INACTIVE && !IsColorDlgVisible()) {
		PostMessage(WM_CLOSE);
	}
}

void CXTColorPopup::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// Inform the parent that we are closing up.
	m_pParentWnd->SendMessage(CPN_XT_CLOSEUP,
		(WPARAM)m_clrColor, m_callerParam);
	
#ifndef _XTP_INCLUDE_CONTROLS
	CXTCoolMenu::m_iMenuBarAlign = 0;

	
	if (DYNAMIC_DOWNCAST(CXTToolBar, m_pParentWnd))
	{
		int iIndex = ((CXTToolBar*)m_pParentWnd)->CommandToIndex(m_callerParam);	
		((CXTToolBar*)m_pParentWnd)->SetDroppedItem(iIndex, false);
		m_pParentWnd->Invalidate(FALSE);
	}
#endif
}


int CXTColorPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state
	
	if (CXTColorSelectorCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	

	if (xtAfxData.bXPMode)
	{
		ModifyStyle(WS_DLGFRAME | WS_THICKFRAME | WS_BORDER, 0, SWP_FRAMECHANGED);
		ModifyStyleEx(WS_EX_STATICEDGE|WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME, 0, SWP_FRAMECHANGED);
	} else
	{
		ModifyStyle(0, WS_DLGFRAME, SWP_FRAMECHANGED);
	}

	if (xtAfxData.bXPMode && xtAfxData.bMenuShadows && m_bDisplayShadow)
	{
		XTShadowsManager()->SetShadow(m_hWnd, m_rcExclude);
	
#ifndef _XTP_INCLUDE_CONTROLS
		if (CXTCoolMenu::m_iMenuBarAlign && !m_rcExclude.IsRectEmpty())
		{
			XTShadowsManager()->SetShadow(m_rcExclude, m_hWnd);
		}
#endif
	}
	
	SetCapture();
	
	m_pParentWnd->SendMessage(CPN_XT_DROPDOWN,
		(WPARAM)m_clrColor, m_callerParam);
	
	return 0;
}

// Adds a window to send color picker notifications to.
void CXTColorPopup::AddListener(HWND hwndListener)
{
	m_listeners.Add(hwndListener);
}

void CXTColorPopup::EndSelection(int nCurSel)
{
	ReleaseCapture();
	
	_EndSelection(nCurSel, m_callerParam);

	PostMessage(WM_CLOSE);
	
}

#if _MSC_VER > 1200 //MFC 7.0
void CXTColorPopup::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CWnd::OnActivateApp(bActive, dwThreadID);
	
	// If deactivating app, cancel this selection
	// TSF ÄNDERUNG 19.04.02
	if (!bActive && !IsColorDlgVisible()) {
		EndSelection(-1);
	}
}
#else
void CXTColorPopup::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CWnd::OnActivateApp(bActive, hTask);
	
	// If deactivating app, cancel this selection
	// TSF ÄNDERUNG 19.04.02
	if (!bActive && !IsColorDlgVisible()) {
		EndSelection(-1);
	}
}
#endif //MFC 7.0

void CXTColorPopup::OnKillFocus(CWnd* pNewWnd) 
{
	if (pNewWnd != this)
	{
		CWnd::OnKillFocus(pNewWnd);
		ReleaseCapture();
	}
}

void CXTColorPopup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rc;
	GetClientRect(&rc);

	if (!rc.PtInRect(point) && !IsColorDlgVisible()) 
	{
		EndSelection(-1);
		return;
	}
	CXTColorSelectorCtrl::OnLButtonDown(nFlags, point);
}

BOOL CXTColorPopup::PreTranslateMessage(MSG* pMsg) 
{

	// TSF ÄNDERUNG 19.04.02
	if (GetCapture()->GetSafeHwnd() != m_hWnd && !IsColorDlgVisible())
		SetCapture(); 
	
	// TSF ÄNDERUNG 03.08.01 - if the ESCAPE is pressed, the window must be closed
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )
		EndSelection(-1);
	
	return CXTColorSelectorCtrl::PreTranslateMessage(pMsg);
}


void CXTColorPopup::OnNcPaint() 
{
	if (xtAfxData.bXPMode)
	{
		CWindowDC dc(this);
		const COLORREF clrFrame = xtAfxData.clrXPMenuBorder;
		CRect rc;
		GetWindowRect(&rc);
		int cx = rc.Width(); // for right border is exclusive
		int cy = rc.Height() ; // see above 	   
		dc.Draw3dRect(0, 0, cx, cy, clrFrame, clrFrame);
	
	#ifndef _XTP_INCLUDE_CONTROLS
		if (!m_rcExclude.IsRectEmpty())
			::DrawAdjacentBorder(&dc, xtAfxData.clrXPBarFace, m_rcExclude, rc);
	#endif

	}
	else
	{
		Default();
	}
}

void CXTColorPopup::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (xtAfxData.bXPMode)
	{
		// adjust non-client area for border space
		lpncsp->rgrc[0].left += 1;
		lpncsp->rgrc[0].top += 1;
		lpncsp->rgrc[0].right += -1;
		lpncsp->rgrc[0].bottom += -1;
	}
	else
	{	 
		CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}
