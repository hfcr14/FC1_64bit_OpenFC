// XTToolBarPopupWnd.cpp : implementation file
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
#include "XTToolBarCtrl.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTToolBarPopupWnd.h"
#include "XTMemDC.h"
#include "XTWndShadow.h"
#include "XTHelpers.h"
#include "XTCoolMenu.h"

#include <windowsx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CXTPopupWndToolbar, CXTToolBar)

/////////////////////////////////////////////////////////////////////////////
// CXTPopupWndToolbar helper used to paint background correctly for toolbar popup
//
COLORREF CXTPopupWndToolbar::GetBackgroundColor() const
{
	return (xtAfxData.bXPMode) ? xtAfxData.clrXPMenuTextBack : xtAfxData.clr3DFace;
}

bool CXTPopupWndToolbar::IsFillToolBarClientRect()
{
	return true;
}

IMPLEMENT_DYNAMIC(CXTToolBarPopupWndHook, CXTWndHook)

/////////////////////////////////////////////////////////////////////////////
// CXTToolBarPopupWndHook is used to hook a controlling frame to figure out when ids 
// it time to close the popup
//

CXTToolBarPopupWndHook::CXTToolBarPopupWndHook() 
: m_pPopup(0)
, m_bCreated(false)
{ }

// Filters messages as required to synch up the hidden toolbar popup
LRESULT CXTToolBarPopupWndHook::WindowProc(UINT uMSG, WPARAM wParam, LPARAM lParam)
{
	switch (uMSG)
	{
		case WM_NCACTIVATE:
			// fake showing inactive caption bar
			wParam = true;
			break;

		case WM_COMMAND:
		{
			UINT uCmdID = GET_WM_COMMAND_ID(wParam, lParam);

			// close popup before executing any uCmdID
			if (m_pPopup && m_bCreated && m_pPopup->m_wndToolBar.m_hWnd)
			{
				// skip commands that are child IDs of the toolbar being displayed
				if (!m_pPopup->m_wndToolBar.GetDlgItem(uCmdID))
				{
					// discard sender's handle since we are probably destroying the sender
					::PostMessage(m_hWnd, uMSG, uCmdID, 0);
					m_pPopup->UpdateToolbarIcon(uCmdID);
					m_pPopup->DestroyWindow();
					return 0;
				}
			}
			break;
		}
	}

	return CXTWndHook::WindowProc(uMSG, wParam, lParam);
};

/////////////////////////////////////////////////////////////////////////////
// CXTToolBarPopupWnd

CXTToolBarPopupWnd::CXTToolBarPopupWnd(int& iCmdID) : m_iCmdID(iCmdID)
{
	m_uToolbarID    = 0;
	m_pWndParent    = 0;
	m_hook.m_pPopup = this;

	m_rcExclude.SetRectEmpty();
}

CXTToolBarPopupWnd::~CXTToolBarPopupWnd()
{

}

IMPLEMENT_DYNAMIC(CXTToolBarPopupWnd, CWnd)

BEGIN_MESSAGE_MAP(CXTToolBarPopupWnd, CWnd)
	//{{AFX_MSG_MAP(CXTToolBarPopupWnd)
	ON_WM_ACTIVATE()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTToolBarPopupWnd message handlers

BOOL CXTToolBarPopupWnd::Create(CXTToolBar* pWndParent, UINT uToolbarID, int iNumCols/*=5*/)
{
	m_pWndParent = pWndParent;
	ASSERT(m_pWndParent != 0); // must be valid.

	m_uToolbarID = uToolbarID;
	ASSERT(m_uToolbarID != 0);

	int iIndex = m_pWndParent->CommandToIndex(m_iCmdID);
	m_pWndParent->SetDroppedItem(iIndex, true);

	CRect rect;
	m_pWndParent->GetItemRect(iIndex, &rect);
	m_pWndParent->ClientToScreen(&rect);

	// register the wnd class.
	CString wndClass = ::AfxRegisterWndClass(
		CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW, 0, 0);

	m_rcExclude = rect;

	// call the base class for creation.
	if( !CWnd::CreateEx( NULL, wndClass, _T(""), WS_POPUP|WS_DLGFRAME,
		CRect(0,0,0,0), m_pWndParent, 0, NULL ))
	{
		TRACE0( "Failed to create toolbar extension popup window.\n" );
		return FALSE;
	}

	// figure out the preferred width of the toolbar (6 normal buttons by default)
	CSize sizeButton(m_pWndParent->GetToolBarCtrl().GetButtonSize());
	int preferredWidth = __min(250, sizeButton.cx * iNumCols);

	// calc the size but do not commit just yet - actually that will be
	// done later when resizing this window
	CSize sizeExtent = m_wndToolBar.CalcDynamicLayout(preferredWidth, LM_HORZ );

	// figure out what window rect we need to accommodate this client
	CRect rcBorders(0,0,100,100);
	SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)(LPVOID)&rcBorders);


	sizeExtent.cx += 100 - rcBorders.Width();
	sizeExtent.cy += 100 - rcBorders.Height();

	// resize/show the window the window
	int cx = sizeExtent.cx + 2;
	int cy = sizeExtent.cy + 3;

	CPoint pt = m_pWndParent->ComputePopupTrackPoint(rect, CSize(cx, cy));

	SetWindowPos(0, pt.x, pt.y, cx, cy,
		SWP_SHOWWINDOW | SWP_NOZORDER);

	m_wndToolBar.OnUpdateCmdUI(m_pWndParent->GetDockingFrame(), true);

	// Set the font and control ID.
	SetFont(&xtAfxData.font);

	m_hook.m_bCreated = true;

	return TRUE;
}

int CXTToolBarPopupWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_pWndParent) {
		m_hook.HookWindow(m_pWndParent->GetOwner());
	}

	if (xtAfxData.bXPMode)
	{
		ModifyStyle(WS_DLGFRAME | WS_THICKFRAME | WS_BORDER, 0, SWP_FRAMECHANGED);
		ModifyStyleEx(WS_EX_STATICEDGE|WS_EX_WINDOWEDGE|WS_EX_DLGMODALFRAME, 0, SWP_FRAMECHANGED);
		if (xtAfxData.bMenuShadows)
		{
			//new CXTWndShadow(m_hWnd);
			XTShadowsManager()->SetShadow(m_hWnd, m_rcExclude);
			if (CXTCoolMenu::m_iMenuBarAlign && !m_rcExclude.IsRectEmpty())
			{
				XTShadowsManager()->SetShadow(m_rcExclude, m_hWnd);
			}

		}
	}

	DWORD dwStyleExtra = m_pWndParent->GetToolbarStyle() & (TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);

	// create the toolbar
	if (!m_wndToolBar.CreateEx(this, dwStyleExtra, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | 
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOATING | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(m_uToolbarID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CSize sizeButton, sizeBitmap;
	m_pWndParent->GetSizes(sizeButton, sizeBitmap);

	VERIFY(m_wndToolBar.SetBitmapSize(sizeBitmap));
	VERIFY(m_wndToolBar.SetButtonSize(sizeButton));

	// copy extended style
	m_wndToolBar.SetExtendedStyle(m_pWndParent->GetExtendedStyle());
	m_wndToolBar.SetOwner(m_pWndParent->GetOwner());
	
	return 0;
}

void CXTToolBarPopupWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	// TODO: Add your message handler code here
	if (nState == WA_INACTIVE)
	{
		PostMessage(WM_CLOSE);
	}
}

void CXTToolBarPopupWnd::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CWnd::OnWindowPosChanged(lpwndpos);

	if (lpwndpos->cx > 0 && lpwndpos->cy > 0)
	{
		int width = lpwndpos->cx;
		CRect rcBorders(0,0,100,100);
		SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)(LPVOID)&rcBorders);
		width  -= 100 - rcBorders.Width();

		CSize sizeExtent1 = m_wndToolBar.CalcDynamicLayout(width, LM_HORZ | LM_COMMIT);
		m_wndToolBar.SetWindowPos(0, 1, 1, width, sizeExtent1.cy,
			SWP_NOACTIVATE | SWP_NOZORDER);
	}
}

BOOL CXTToolBarPopupWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient, (xtAfxData.bXPMode) ? xtAfxData.clrXPMenuTextBack : xtAfxData.clr3DFace);
	return CWnd::OnEraseBkgnd(pDC);
}

void CXTToolBarPopupWnd::PostNcDestroy() 
{
	delete this;
}

CXTIconHandle CXTToolBarPopupWnd::GetSelectedIcon(UINT uCmdID)
{
	// Get the index of the selected command.
	int iIndex = m_wndToolBar.CommandToIndex(uCmdID);
	if (iIndex == -1)
		return 0;

	// Get the image list for the popup toolbar.
	CImageList* pImageList = m_wndToolBar.GetToolBarCtrl().GetImageList();
	ASSERT(pImageList != NULL);
	

	// Get the index of the image associated with the command.
	TBBUTTON tb;
	m_wndToolBar.GetButton(iIndex, &tb);
	m_iCmdID = tb.idCommand;
	
	// Return the icon handle.
	return pImageList->ExtractIcon(tb.iBitmap);
}

void CXTToolBarPopupWnd::UpdateToolbarIcon(UINT uCmdID)
{
	// Get the parent toolbars image list.
	CImageList* pImageList = m_pWndParent->GetToolBarCtrl().GetImageList();
	ASSERT(pImageList != NULL);
	
	// Get the index of the command that activated us.
	int iIndex = m_pWndParent->CommandToIndex(m_iCmdID);
	ASSERT(iIndex != -1);
	
	// Get the index of the image associated with the command.
	UINT nID; UINT nStyle; int iImage;
	m_pWndParent->GetButtonInfo(iIndex, nID, nStyle, iImage);

	// Update the command id of the drop down button.
	XT_DROPDOWNBUTTON* pDDButton = m_pWndParent->FindDropDownButton(nID);
	ASSERT(pDDButton != NULL);

	CXTIconHandle hIcon = GetSelectedIcon(uCmdID);
	if (hIcon == 0)
		return;
	// Get the newly selected icon and update the toolbar's image list.
	pImageList->Replace( iImage, hIcon);

	nID = m_iCmdID;
	pDDButton->idButton = m_iCmdID;
	m_pWndParent->SetButtonInfo(iIndex, nID, nStyle, iImage);

	// Redraw the parent toolbar.
	m_pWndParent->Invalidate();
	m_pWndParent->UpdateWindow();
}

void CXTToolBarPopupWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (xtAfxData.bXPMode)
	{
		// adjust non-client area for border space
		lpncsp->rgrc[0].left	+= 1;
		lpncsp->rgrc[0].top		+= 1;
		lpncsp->rgrc[0].right	-= 1;
		lpncsp->rgrc[0].bottom	-= 1;
	}
	else
	{
		CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}

void CXTToolBarPopupWnd::OnNcPaint() 
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
		if (!m_rcExclude.IsRectEmpty())
		{
			//rc.bottom--;
			::DrawAdjacentBorder(&dc, xtAfxData.clrXPBarFace, m_rcExclude, rc);
		}
	}

	Default();
}

void CXTToolBarPopupWnd::OnDestroy() 
{
	m_hook.HookWindow((HWND)0);
	m_hook.m_pPopup = 0;

	CXTToolBar* pWndParent = m_pWndParent;

	CWnd::OnDestroy();

	int iIndex = m_pWndParent->CommandToIndex(m_iCmdID);
	pWndParent->SetDroppedItem(iIndex, false);
	pWndParent->Invalidate();
	pWndParent->UpdateWindow();
}
