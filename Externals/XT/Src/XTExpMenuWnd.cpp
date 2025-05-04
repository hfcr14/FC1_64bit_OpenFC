// XTExpMenuWnd.cpp : implementation file
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
#include <windowsx.h>
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTExpMenuWnd.h"
#include "XTMemDC.h"
#include "XTWndShadow.h"
#include "XTHelpers.h"
#include "XTResource.h"
#include "XTToolBarCtrl.h"
#include "XTColorSelectorCtrl.h"
#include "XTColorPopup.h"
#include "XTPriv.h"

#pragma warning( disable : 4706 ) 
#include <multimon.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CXTExpMenuWnd, CWnd)
	//{{AFX_MSG_MAP(CXTExpMenuWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_ACTIVATE()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_COMMAND(XT_IDC_CUSTOMIZE, OnCustomize)
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CXTHiddenButtonsWndHook, CXTWndHook)
IMPLEMENT_DYNAMIC(CXTExpMenuWnd, CWnd)
IMPLEMENT_DYNAMIC(CXTHiddenButtonsToolbar, CXTToolBar)


static COLORREF GetBackgroundColor()
{
	return (xtAfxData.bXPMode) ? xtAfxData.clrXPMenuTextBack : xtAfxData.clr3DFace;
}

COLORREF CXTHiddenButtonsToolbar::GetBackgroundColor() const
{
	return ::GetBackgroundColor();
}

bool CXTHiddenButtonsToolbar::IsFillToolBarClientRect()
{
	return true;
}

CXTColorPopup* CXTHiddenButtonsToolbar::CreateNewColorPopup(XT_DROPDOWNBUTTON* pDropButton, CRect rect)
{
	CXTColorPopup* pColorPopup = CXTToolBar::CreateNewColorPopup(pDropButton, rect);
	if (m_pPopup)
	{
		pColorPopup->AddListener(m_pPopup->m_pParent->m_hWnd);
	}
	return pColorPopup;
}


/////////////////////////////////////////////////////////////////////////////
// CXTHiddenButtonsWndHook is used to hook a controlling frame
// to figure out when ids it time to close the popup
//
// Filters messages as required to synch up the hidden toolbar popup
LRESULT CXTHiddenButtonsWndHook::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_NCACTIVATE:
			// fake showing inactive caption bar
			wParam = true;
			break;

		case WM_COMMAND:
		{
			UINT command = GET_WM_COMMAND_ID(wParam, lParam);

			// close popup before executing any command
			if (m_pPopup && m_pPopup->m_toolbar.m_hWnd)
			{
				// skip commands that are child IDs of the toolbar being displayed
				if (!m_pPopup->m_toolbar.GetDlgItem(command))
				{
					// discard sender's handle since we are probably destroying the sender
					::PostMessage(m_hWnd, message, command, 0);
					m_pPopup->DestroyWindow();
					return 0;
				}
			}
			break;
		}
		case CPN_XT_CLOSEUP:
			m_pPopup->PostMessage(WM_CLOSE);
			break;

		case CPN_XT_SELENDOK:

			break;
	}

	return CXTWndHook::WindowProc(message, wParam, lParam);
};


/////////////////////////////////////////////////////////////////////////////
// CXTExpMenuWnd

CXTExpMenuWnd::CXTExpMenuWnd(CWnd* pWndNotify, UINT nDestroyedMsg)
: m_pParent(0)
, m_nDestroyedMsg(nDestroyedMsg)
, m_pWndNotify(pWndNotify)
, m_bDestroying(false)
{
	m_toolbar.m_pPopup = this;
	m_hook.m_pPopup = this;
	m_exclusionRect.SetRectEmpty();
}

CXTExpMenuWnd::~CXTExpMenuWnd()
{
}

BOOL CXTExpMenuWnd::Create(CRect rect, CXTToolBar* pParent)
{
	m_pParent = pParent;
	ASSERT(m_pParent != NULL); // must be valid.

	m_exclusionRect = rect;

	// register the wnd class.
	CString wndClass = ::AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW,
		0, 0);
	// call the base class for creation.
	if( !CWnd::CreateEx( NULL, wndClass, _T(""), WS_POPUP|WS_DLGFRAME,
		CRect(0,0,0,0), m_pParent, 0, NULL ))
	{
		TRACE0( "Failed to create toolbar extension popup window.\n" );
		return FALSE;
	}

	// figure out the preferred width of the toolbar (6 normal buttons by default)
	CSize sizeButton(m_pParent->GetToolBarCtrl().GetButtonSize());
	int preferredWidth = __min(250, sizeButton.cx * 6);

	// populate with buttons
	m_pParent->CopyHiddenButtons(&m_toolbar, preferredWidth);

	// calc the size but do not commit just yet - actually that will be
	// done later when resizing this window
	CSize extent = m_toolbar.CalcDynamicLayout(preferredWidth, LM_HORZ );
	if (m_toolbar.GetButtonCount() == 0)
	{
		extent = CSize(0);
	}
	if (m_pParent->IsExpansionCustomizes())
	{
		CSize extent1 = extent;
		CSize extent2 = m_toolbarCommands.CalcDynamicLayout(preferredWidth, LM_HORZ);
		extent = CSize(__max(extent1.cx, extent2.cx), extent1.cy + extent2.cy );
		int nCommandsWidth = extent.cx;
		int nCommandsHeight = ::GetSystemMetrics(SM_CYMENU);
		m_toolbarCommands.SetButtonSize(CSize(0, nCommandsHeight));
		m_toolbarCommands.GetToolBarCtrl().SetButtonWidth(nCommandsWidth, nCommandsWidth);
	}

	// figure out what window rect we need to accommodate this client
	CRect rectBorders(0,0,100,100);
	SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)(LPVOID)&rectBorders);
	extent.cx += 100 - rectBorders.Width();
	extent.cy += 100 - rectBorders.Height();

	
	m_toolbar.OnUpdateCmdUI(m_pParent->GetDockingFrame(), true);

	// Set the font and control ID.
	SetFont(&xtAfxData.font);

	// resize/show the window the window
	int nWidth = extent.cx + 6;
	int nHeight = extent.cy + 3;

	CPoint origin = m_pParent->ComputePopupTrackPoint(rect, CSize(nWidth, nHeight));

    if (m_pParent->IsFloating())
    {
        origin = CPoint(rect.right-nWidth+1, rect.bottom+1);

        m_exclusionRect.bottom += 2;
        m_exclusionRect.right  += 1;
    }

	else if (m_pParent->IsTopDocked()) {
		origin = CPoint(rect.right-nWidth, rect.bottom-1);
    }
	else if (m_pParent->IsBottomDocked()) {
		origin = CPoint(rect.right-nWidth, rect.top-nHeight+1);
    }
    else if (m_pParent->IsRightDocked()) {
		origin = CPoint(rect.left-nWidth+1, rect.bottom-nHeight);
	}
	else if (m_pParent->IsLeftDocked()) {
		origin = CPoint(rect.right-1, rect.bottom-nHeight);
	}

    POINT point;
    ::GetCursorPos( &point );
    
    HMONITOR hMonitor = ::MonitorFromPoint( point, MONITOR_DEFAULTTONULL );
    if ( hMonitor )
    {
        MONITORINFOEX info;
        info.cbSize = sizeof( MONITORINFOEX );
        if ( ::GetMonitorInfo( hMonitor, ( MONITORINFO* )&info ) )
        {
            if( origin.x < info.rcMonitor.left ) {
                origin.x = info.rcMonitor.left;
            }
            if( origin.y < info.rcMonitor.top ) {
                origin.y = info.rcMonitor.top;
            }
        }
    }

	SetWindowPos(0, origin.x, origin.y, nWidth, nHeight,
		SWP_SHOWWINDOW | SWP_NOZORDER);

	return TRUE;
}

void CXTExpMenuWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	// If we lose activation, post
	// a close message...
	if (nState == WA_INACTIVE)
	{
		DestroyWindow();
	}
}

void CXTExpMenuWnd::OnDestroy()
{
	m_hook.HookWindow(HWND(0));

	// relocate controls back to their original parent
	CArray<CWnd*, CWnd*> children;
	CWnd* pChild;
	for (pChild = m_toolbar.GetWindow(GW_CHILD);
			pChild;  pChild = pChild->GetNextWindow())
	{
		if (pChild->GetDlgCtrlID() != XT_IDC_CUSTOMIZE)
		{
			children.Add(pChild);
		}
	}

	int i;
	for (i = 0; i < children.GetSize(); i++)
	{
		children[i]->ShowWindow(SW_HIDE);
		children[i]->SetParent(m_pParent);
	}

	// notify parent that the window is closing
	if (m_nDestroyedMsg && m_pWndNotify)
	{
		m_pWndNotify->SendMessage(m_nDestroyedMsg);
	}

	CWnd::OnDestroy();
}


int CXTExpMenuWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pParent)
	{
		m_hook.HookWindow(m_pParent->GetOwner());
	}

	if (xtAfxData.bXPMode)
	{
		ModifyStyle(WS_DLGFRAME | WS_THICKFRAME | WS_BORDER, 0, SWP_FRAMECHANGED);
		ModifyStyleEx(WS_EX_STATICEDGE|WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME, 0, SWP_FRAMECHANGED);
		if (xtAfxData.bMenuShadows)
		{
			XTShadowsManager()->SetShadow(m_hWnd, m_exclusionRect);
			if (!m_exclusionRect.IsRectEmpty())
			{
				XTShadowsManager()->SetShadow(m_exclusionRect, m_hWnd);
			}
		}
	}

	 // create the toolbar
	DWORD dwStyleExtra = m_pParent->GetToolbarStyle() & (TBSTYLE_LIST | TBSTYLE_FLAT);
	CSize sizeButton, sizeBitmap;
	if (!m_toolbar.CreateEx(this, dwStyleExtra,
		   WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY |
		   CBRS_FLOATING | CBRS_SIZE_DYNAMIC ))
	{
		TRACE0("Failed to create extension toolbar.\n" );
		return FALSE;
	}

	m_pParent->GetSizes(sizeButton, sizeBitmap);
	//m_toolbar.SetSizes(sizeButton, sizeBitmap);
	VERIFY(m_toolbar.SetBitmapSize(sizeBitmap));
	VERIFY(m_toolbar.SetButtonSize(sizeButton));


	// copy extended style
	m_toolbar.SetExtendedStyle(m_pParent->GetExtendedStyle());
	m_toolbar.SetOwner(m_pParent->GetOwner());

	if (m_pParent->IsExpansionCustomizes())
	{
		DWORD style = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;
		if (m_pParent->IsExpansionChevron())
		{
			style |= CBRS_BORDER_TOP;
		}
		if (!m_toolbarCommands.CreateEx(this, TBSTYLE_LIST | TBSTYLE_FLAT, style))
		{
			TRACE0("Failed to create extension toolbar.\n" );
			return FALSE;
		}
		CString strCustomize;
		XT_MANAGE_STATE_BEGIN  // Switch to toolkit resources/module state
		VERIFY(strCustomize.LoadString(XT_IDS_COLOR_CUST));
		XT_MANAGE_STATE_END
		UINT commands[] = {XT_IDC_CUSTOMIZE};
		m_toolbarCommands.SetButtons(commands, _countof(commands));
		m_toolbarCommands.SetButtonText(0, strCustomize);
		m_toolbarCommands.ResetFreshenAfter();
	}
	return 0;
}

#if _MSC_VER > 1200 //MFC 7.0
void CXTExpMenuWnd::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CWnd::OnActivateApp(bActive, dwThreadID);
#else
void CXTExpMenuWnd::OnActivateApp(BOOL bActive, HTASK hTask)
{
	CWnd::OnActivateApp(bActive, hTask);
#endif //MFC 7.0
	// If deactivating app, close this
	if (!bActive)
	{
		DestroyWindow();
	}
}

void CXTExpMenuWnd::OnKillFocus(CWnd* pNewWnd)
{
	if (pNewWnd != this)
	{
		CWnd::OnKillFocus(pNewWnd);
		ReleaseCapture();
	}
}

void CXTExpMenuWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDown(nFlags, point);

}

void CXTExpMenuWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonUp(nFlags, point);

}

void CXTExpMenuWnd::OnNcPaint()
{
	if (xtAfxData.bXPMode)
	{
		CWindowDC dc(this);
		COLORREF clrFrame = xtAfxData.clrXPMenuBorder;
		CRect rc;
		GetWindowRect(&rc);
		int cx = rc.Width(); // for right border is exclusive
		int cy = rc.Height() ; // see above
		dc.Draw3dRect(0, 0, cx, cy, clrFrame, clrFrame);

		// find out if there is an overlap with popup menu tab, if so, draw it adjacent
		if (!m_exclusionRect.IsRectEmpty())
		{
			::DrawAdjacentBorder(&dc, xtAfxData.clrXPBarFace, m_exclusionRect, rc);
		}
	}
	else
	{
		Default();
	}
	Default();
}

void CXTExpMenuWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	if (xtAfxData.bXPMode)
	{
		// adjust non-client area for border space
		lpncsp->rgrc[0].left += 1;
		lpncsp->rgrc[0].top += 1;
		lpncsp->rgrc[0].right -= 1;
		lpncsp->rgrc[0].bottom -= 1;
	}
	else
	{
		CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}

BOOL CXTExpMenuWnd::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, ::GetBackgroundColor());
	return CWnd::OnEraseBkgnd(pDC);
}

void CXTExpMenuWnd::OnWindowPosChanged(WINDOWPOS FAR* wndPos)
{
	CWnd::OnWindowPosChanged(wndPos);

	if (wndPos->cx > 0 && wndPos->cy > 0)
	{
		int width = wndPos->cx;
		CRect rectBorders(0,0,100,100);
		SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)(LPVOID)&rectBorders);
		width  -= 100 - rectBorders.Width();

		CSize extent1(0);
		if (m_toolbar.GetButtonCount() > 0)
		{
			extent1 = m_toolbar.CalcDynamicLayout(width, LM_HORZ | LM_COMMIT);
		}
		m_toolbar.SetWindowPos(0, 1, 1, width, extent1.cy,
			SWP_NOACTIVATE | SWP_NOZORDER);

		if (m_pParent->IsExpansionCustomizes())
		{
			CSize extent2 = m_toolbarCommands.CalcDynamicLayout(width, LM_HORZ | LM_COMMIT);
			int y = (extent1.cy > 3) ? extent1.cy + 2 : 1;
			m_toolbarCommands.SetWindowPos(0, 1, y, width - 2, extent2.cy,
				SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}
}

void CXTExpMenuWnd::PostNcDestroy()
{
	delete this;
}

BOOL CXTExpMenuWnd::DestroyWindow()
{
	if (!m_bDestroying)
	{
		m_bDestroying = true;
		return CWnd::DestroyWindow();
	}
	else
	{
		return true;
	}
}

void CXTExpMenuWnd::OnCustomize()
{
	m_pWndNotify->PostMessage(WM_COMMAND, XT_IDC_CUSTOMIZE);
	DestroyWindow();
}
