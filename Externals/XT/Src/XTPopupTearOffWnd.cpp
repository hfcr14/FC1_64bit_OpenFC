// XTPopupTearOffWnd.cpp : implementation file
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
#include "XTToolBarCtrl.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTMemDC.h"
#include "XTWndShadow.h"
#include "XTHelpers.h"
#include "XTWndHook.h"
#include "XTColorSelectorCtrl.h"
#include "XTDrawHelpers.h"
#include "XTToolBarPopupWnd.h"
#include "XTPopupTearOffWnd.h"
#include "XTControlBar.h"
#include "XTCallbacks.h"
#include "XTDockContext.h"
#include "XTCoolMenu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class _XT_EXT_CLASS CXTPopupTearOffWnd::CXTPopupTearOffWndWndHook : public CXTWndHook
{

public:
	CXTPopupTearOffWndWndHook(CXTPopupTearOffWnd* pWnd)
	{
		m_pTearOffWnd = pWnd;

	}
	~CXTPopupTearOffWndWndHook()
	{
		if (m_pTearOffWnd)
		{
			m_pTearOffWnd->m_pHook = 0;
		}

	}
	void DetachWnd()
	{
		m_pTearOffWnd = 0;
	}
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCACTIVATE)
		{
			// fake showing inactive caption bar
			wParam = true;
		}
		if (message == WM_COMMAND)
		{
			m_pTearOffWnd->OnHookedCommand(wParam, lParam);			
		}

		return CXTWndHook::WindowProc(message, wParam, lParam);
	}
private:
	CXTPopupTearOffWnd* m_pTearOffWnd;

};

/////////////////////////////////////////////////////////////////////////////
// CXTPopupTearOffWnd

CXTPopupTearOffWnd::CXTPopupTearOffWnd(int iCmdID) : m_iCmdID(iCmdID)
{
	m_pChild = 0;
	m_rcExclude.SetRectEmpty();
	m_pWndParent = 0;
	m_bTearOff = TRUE;
	
	m_bPressed = m_bHighlighted = FALSE;
}

CXTPopupTearOffWnd::~CXTPopupTearOffWnd()
{
}

#define TEAROFF_CY 11


BOOL CXTPopupTearOffWnd::_Create(CXTToolBar* pWndParent, CWnd* pChild, CXTControlBar* pControlBar)
{
	m_pWndParent = pWndParent;
	ASSERT(m_pWndParent != 0); // must be valid.
	
	m_pControlBar = (CXTControlBar*)pControlBar;
	ASSERT(m_pControlBar != 0);
	
	m_pChild = pChild;
	ASSERT(m_pChild != 0);

	int iIndex = m_pWndParent->CommandToIndex(m_iCmdID);;
	m_pWndParent->SetDroppedItem(iIndex, true);
	
	CRect rect;
	m_pWndParent->GetItemRect(iIndex, &rect);
	m_pWndParent->ClientToScreen(&rect);
	m_rcExclude = rect;
	
	// register the wnd class.
	CString wndClass = ::AfxRegisterWndClass(
		CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW, 0, 0);
	
	
	CRect rcChild;
	m_pChild->GetWindowRect(&rcChild);
	CSize sizeExtent = rcChild.Size();
	
	// call the base class for creation.
	if( !CWnd::CreateEx( NULL, wndClass, _T(""), WS_POPUP|WS_DLGFRAME|WS_CLIPCHILDREN,
		CRect(0, 0, 0, 0), m_pWndParent, 0, NULL ))
	{
		TRACE0( "Failed to create tear-off popup window.\n" );
		return FALSE;
	}
	
	// figure out what window rect we need to accommodate this client
	CRect rcBorders(0,0,100,100);
	SendMessage(WM_NCCALCSIZE, FALSE, (LPARAM)(LPVOID)&rcBorders);
	
	sizeExtent.cx += 100 - rcBorders.Width();
	sizeExtent.cy += 100 - rcBorders.Height();
	
	// resize/show the window the window
	int cx = sizeExtent.cx + 4;
	int cy = sizeExtent.cy + 4;
	
	CRect rcClient(2, 2, cx - 4, cy - 4);
	if (!xtAfxData.bXPMode) rcClient.DeflateRect(0, 0, 4, 4);

	if (m_bTearOff)
	{
		rcClient.OffsetRect(0, TEAROFF_CY);
		cy += TEAROFF_CY;
		
	}
	
	CPoint pt = m_pWndParent->ComputePopupTrackPoint(rect, CSize(cx, cy));
	
	m_pChild->SetParent(this);
	m_pChild->MoveWindow(rcClient);	
	m_pChild->SetFocus();
	
	SetWindowPos(0, pt.x, pt.y, cx, cy,
		SWP_NOZORDER | SWP_SHOWWINDOW);
	
	m_pChild->ShowWindow(SW_SHOW);
	
	
	
	return TRUE;	
	


}

BOOL CXTPopupTearOffWnd::Create(CXTToolBar* pWndParent, CXTControlBar* pControlBar)
{
	CWnd* pChild  = CloneChild(pControlBar);
	ASSERT(pChild != 0);

	return _Create(pWndParent, pChild, pControlBar);

}


BEGIN_MESSAGE_MAP(CXTPopupTearOffWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPopupTearOffWnd)
	ON_WM_CREATE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CAPTURECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPopupTearOffWnd message handlers

int CXTPopupTearOffWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pWndParent) 
	{
		m_pHook = new CXTPopupTearOffWndWndHook(this);		
		m_pHook->HookWindow(m_pWndParent->GetOwner());
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

	
	return 0;
}

void CXTPopupTearOffWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
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

void CXTPopupTearOffWnd::OnNcPaint() 
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
			::DrawAdjacentBorder(&dc, xtAfxData.clrXPBarFace, m_rcExclude, rc);
		}
	}

	Default();
}

void CXTPopupTearOffWnd::PostNcDestroy() 
{	
	delete this;
}

BOOL CXTPopupTearOffWnd::OnEraseBkgnd(CDC* ) 
{
	return TRUE;
}

void CXTPopupTearOffWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	CXTMemDC memDC(&dc, rc);
	
	memDC.FillSolidRect(rc, (xtAfxData.bXPMode) ? xtAfxData.clrXPMenuTextBack : xtAfxData.clr3DFace);	

	if (m_bTearOff)
	{
		if (!m_bHighlighted)
		{
			COLORREF crFill = xtAfxData.bXPMode ? xtAfxData.clrXPBarFace : xtAfxData.clrInActiveCaption;
			memDC.FillSolidRect(1, 3, rc.right - 2, 7, crFill);
		
			if ( xtAfxData.bXPMode )
			{
				CXTPenDC pen(memDC, xtAfxData.clrXPGripper);
				memDC.MoveTo(rc.right/2 - 17, 4); memDC.LineTo(rc.right/2 + 17, 4);
				memDC.MoveTo(rc.right/2 - 17, 6); memDC.LineTo(rc.right/2 + 17, 6);
				memDC.MoveTo(rc.right/2 - 17, 8); memDC.LineTo(rc.right/2 + 17, 8);
			}
		}
		else
		{
			COLORREF crFill = xtAfxData.bXPMode ? xtAfxData.clrXPHighlight : xtAfxData.clrActiveCaption;
			memDC.FillSolidRect(1, 3, rc.right - 2, 7, crFill);

			if ( xtAfxData.bXPMode )
			{
				memDC.Draw3dRect(1, 3, rc.right - 2, 7, xtAfxData.clrXPHighlightBorder, xtAfxData.clrXPHighlightBorder);
				
				CXTPenDC pen(memDC, xtAfxData.clrXPHighlightText);
				memDC.MoveTo(rc.right/2 - 17, 5); memDC.LineTo(rc.right/2 + 17, 5);
				memDC.MoveTo(rc.right/2 - 17, 7); memDC.LineTo(rc.right/2 + 17, 7);
			}

		}
		// TODO: Default style. 
		
	}
}


void CXTPopupTearOffWnd::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)
	{
		PostMessage(WM_CLOSE);
	}
}

void CXTPopupTearOffWnd::OnDestroy() 
{
	CXTToolBar* pWndParent = m_pWndParent;
	int iIndex = m_pWndParent->CommandToIndex(m_iCmdID);

	if (m_pHook)
	{
		((CXTPopupTearOffWndWndHook*)m_pHook)->DetachWnd();
		m_pHook->HookWindow((HWND)0);
		delete m_pHook;
		m_pHook = 0;
	}
	
	if (m_pChild)
	{	
		m_pChild->DestroyWindow();
		delete m_pChild;
		m_pChild = 0;
	}


	CWnd::OnDestroy();

	
	pWndParent->SetDroppedItem(iIndex, false);
	pWndParent->Invalidate();
	pWndParent->UpdateWindow();
	
}

BOOL CXTPopupTearOffWnd::PtInTearOffRect(CPoint point)
{
	CXTClientRect rc(this);
	CRect rcTearOff(0, 3, rc.right - 1, 10);	
	
	return rcTearOff.PtInRect(point);	
}

void CXTPopupTearOffWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);
	
	if (m_bTearOff)
	{		
		BOOL bHighlighted = PtInTearOffRect(point);
		
		if (m_bHighlighted != bHighlighted)
		{		

			if (bHighlighted) 
			{
				m_bHighlighted = TRUE;			
				SetCapture();
				SetCursor(xtAfxData.hcurMove);
			}
			else 		
			{
				if(!m_bPressed) 
				{
					ReleaseCapture();
					m_bHighlighted = FALSE;
				}
				
			}
			Invalidate(FALSE);
		} 

		CXTClientRect rc(this);
		if (m_bPressed && (point.y < -10 || point.y > 25 || point.x < -15 || point.x > rc.right + 15))
		{
			OnStartDrag(point);
		}
	}	
	
}

void CXTPopupTearOffWnd::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_bPressed)
	{
		m_bPressed = FALSE;	
		Invalidate(FALSE);
	}
	if (m_bHighlighted)
	{
		m_bHighlighted = FALSE;	
		Invalidate(FALSE);
	}
	
	CWnd::OnCaptureChanged(pWnd);
}

void CXTPopupTearOffWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bTearOff)
	{
		if (PtInTearOffRect(point))
		{
			m_bPressed = TRUE;	

		}
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CXTPopupTearOffWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bPressed)
	{
		if (!PtInTearOffRect(point))
		{
			ReleaseCapture();
			m_bHighlighted = FALSE;
		}
		m_bPressed = FALSE;
		Invalidate(FALSE);
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CXTPopupTearOffWnd::OnHookedCommand(WPARAM , LPARAM )
{


}

void CXTPopupTearOffWnd::OnStartDrag(CPoint point)
{
	ASSERT(m_pControlBar);
	ASSERT(m_pControlBar->m_pDockContext);
	ASSERT(m_pControlBar->m_pDockSite);
	
	ReleaseCapture();
	
	ClientToScreen(&point);
	
	CXTClientRect rc(m_pControlBar);
	CPoint pt = point;
	pt.Offset(-rc.Width()/2, -10);	
	
	ShowWindow(SW_HIDE);
	
	m_pControlBar->m_pDockSite->FloatControlBar(m_pControlBar, pt, 0);	
	m_pControlBar->m_pDockSite->ShowControlBar(m_pControlBar, TRUE, FALSE);	
	
	if (m_pControlBar->m_pDockContext)
	{	
		m_pControlBar->m_pDockContext->StartDrag(point);
	}	
	
	
}


////////////////////////////////////////////////////////////////////////

CXTPopupColorTearOff::CXTPopupColorTearOff(int iCmdID)
	: CXTPopupTearOffWnd(iCmdID)
{
	
}


BEGIN_MESSAGE_MAP(CXTPopupColorTearOff, CXTPopupTearOffWnd)
	//{{AFX_MSG_MAP(CXTPopupColorTearOff)
	ON_WM_CTLCOLOR()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ACTIVATE()

	//}}AFX_MSG_MAP
	ON_MESSAGE(CPN_XT_SELENDOK, OnSelEndOK)
END_MESSAGE_MAP()



void CXTPopupColorTearOff::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE && !((CXTColorSelectorCtrl*)m_pChild)->IsColorDlgVisible())
	{
		PostMessage(WM_CLOSE);
	}
}

HBRUSH CXTPopupColorTearOff::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd == m_pChild)
	{
		pDC->SetBkColor( xtAfxData.bXPMode? xtAfxData.clrXPMenuTextBack: xtAfxData.clr3DFace);
	}	
	return hbr;
}

LRESULT CXTPopupColorTearOff::OnSelEndOK(WPARAM , LPARAM )
{
	PostMessage(WM_CLOSE);
	return 0;
}

int CXTPopupColorTearOff::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTPopupTearOffWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	((CXTColorSelectorCtrl*)m_pChild)->AddListener(m_hWnd);
	return 0;
}

CWnd* CXTPopupColorTearOff::CloneChild(CXTControlBar* pControlBar)
{
	CXTColorSelectorCtrl* pColors = new CXTColorSelectorCtrl();
	pColors->Create(CRect(0, 0, 0, 0), pControlBar->GetOwner(), CPS_XT_EXTENDED|CPS_XT_MORECOLORS, 0);
	return pColors;
}


////////////////////////////////////////////////////////////////////////

CXTPopupToolbarTearOff::CXTPopupToolbarTearOff(int& iCmdID)
: CXTPopupTearOffWnd(iCmdID), m_iCmd(iCmdID)
{
	
}

BEGIN_MESSAGE_MAP(CXTPopupToolbarTearOff, CXTPopupTearOffWnd)
//{{AFX_MSG_MAP(CXTPopupToolbarTearOff)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()




BOOL CXTPopupToolbarTearOff::Create(CXTToolBar* pWndParent, CXTToolBar* pWndChild, int iNumCols/*=5*/)
{
	m_pWndParent = pWndParent;
	
	CXTToolBar* pBar = (CXTToolBar*) CloneChild(pWndChild);
	
	ASSERT(pBar);
	if (!pBar) return FALSE;


	// figure out the preferred width of the toolbar (6 normal buttons by default)
	CSize sizeButton(pBar->GetToolBarCtrl().GetButtonSize());

	int preferredWidth = __min(250, sizeButton.cx * iNumCols);
	
	// calc the size but do not commit just yet - actually that will be
	// done later when resizing this window
	CSize sizeExtent = pBar->CalcDynamicLayout(preferredWidth, LM_HORZ |LM_COMMIT);
	
	pBar->MoveWindow(0, 0, sizeExtent.cx, sizeExtent.cy);

	pBar->OnUpdateCmdUI(m_pWndParent->m_pDockSite, true);
	
	return CXTPopupTearOffWnd::_Create(pWndParent, pBar, pWndChild);

}

void CXTPopupToolbarTearOff::OnHookedCommand(WPARAM wParam, LPARAM) 
{
	UINT uCmdID = LOWORD(wParam);

	if (m_pChild && !m_pChild->GetDlgItem(uCmdID))
	{
		PostMessage(WM_CLOSE);
	}	

}


CWnd* CXTPopupToolbarTearOff::CloneChild(CXTControlBar* pControlBar)
{
	
	CXTToolBar* pBar = new CXTPopupWndToolbar;

	CXTToolBar* pToolBar = DYNAMIC_DOWNCAST(CXTToolBar, pControlBar);
	ASSERT(pToolBar);

	DWORD dwStyleExtra = pToolBar->GetToolbarStyle() & (TBSTYLE_LIST | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT);
	
	// create the toolbar  
	if (!pBar->CreateEx(pToolBar->m_pDockSite, dwStyleExtra, WS_CHILD | CBRS_ALIGN_TOP | 
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_FLOATING | CBRS_SIZE_DYNAMIC) )//||	
	{
		TRACE0("Failed to create toolbar\n");
		return 0;      // fail to create
	}
	
	
	// clone buttons
	int nCount = pToolBar->GetButtonCount();
	pBar->SetButtons(0, nCount);
	int i;
	for (i = 0; i < nCount; i++)
	{
		TBBUTTON btn;
		pToolBar->GetButton(i, &btn);
		pBar->SetButton(i, &btn);
	}
	HIMAGELIST hl =pToolBar->GetImageList();
	pBar->SetImageList(hl);
	
	CSize sizeButton, sizeBitmap;
	pToolBar->GetSizes(sizeButton, sizeBitmap);
	
	VERIFY(pBar->SetBitmapSize(sizeBitmap));
	VERIFY(pBar->SetButtonSize(sizeButton));
	
	// copy extended style
	pBar->SetExtendedStyle(pToolBar->GetExtendedStyle());
	pBar->SetOwner(m_pWndParent->GetOwner());
	

	return pBar;
}