// XTControlBar.cpp : implementation of the CXTControlBar class.
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
#include "XTFunctions.h"
#include "XTCallbacks.h"
#include "XTDockContext.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTSplitterDock.h"
#include "XTDockWindow.h"
#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTCoolMenu.h"
#include "XTMemDC.h"
#include "XTReBar.h"
#include "XTDockBar.h"
#include "XTPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTDockContextWndHook is used to undock toolbars that are on loan in the child frames

IMPLEMENT_DYNAMIC(CXTDockContextWndHook, CXTWndHook)

LRESULT CXTDockContextWndHook::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	bool bUnhook = false;
	if (message == WM_DESTROY && m_pBar && ::IsWindow(m_pBar->m_hWnd))
	{
		CFrameWnd* pFrame = m_pBar->GetParentFrame();
		if (m_pBar->m_pOriginalFrame && pFrame && m_pBar->m_pOriginalFrame != pFrame)
		{
			// the code below handles transferring the toolbar back to its original frame
			// when it was docked to another frame and is applicable when the toolbar is
			// not floating
			if (!m_pBar->IsFloating() && 
				!(m_pBar->m_pDockBar && m_pBar->m_pDockBar->m_bFloating))
			{
				//pFrame->ShowControlBar(this, false, false);
				pFrame->RemoveControlBar(m_pBar);
				m_pBar->m_pOriginalFrame->DockControlBar(m_pBar);
				m_pBar->m_pDockSite = m_pBar->m_pOriginalFrame;
				m_pBar->m_pDockContext->m_pDockSite = m_pBar->m_pOriginalFrame;
			}
			bUnhook = true;
		}
	}
	LRESULT result = CXTWndHook::WindowProc(message, wParam, lParam);
	if (bUnhook)
	{
		HookWindow((HWND)0);
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CXTControlBar
/////////////////////////////////////////////////////////////////////////////

CXTControlBar::CXTControlBar()
: m_pOriginalFrame(0)
, m_bEnableMultiFrameDocking(false)
, m_pHook(NULL)
, m_pHookParent(NULL)
{
	// no elements contained in the control bar yet
	m_nCount = 0;
	m_pData = NULL;

	// set up some default border spacings
	m_cxLeftBorder		=
	m_cxRightBorder 	= 6;
	m_cxDefaultGap		= 2;
	m_cyTopBorder		= 
	m_cyBottomBorder	= 1;
	m_bAutoDelete		= FALSE;
	m_hWndOwner 		= NULL;
	m_nStateFlags		= 0;
	m_pDockSite 		= NULL;
	m_pDockBar			= NULL;
	m_pDockContext		= NULL;
	m_dwStyle			= 0;
	m_dwDockStyle		= 0;
	m_nMRUWidth 		= 32767;
	m_bInReBar			= false;
	m_nPos				= 0;
	m_menuID			= 0;
	m_bp.sizeVert		= CSize(200,100);
	m_bp.sizeHorz		= CSize(200,100);
	m_bp.sizeFloat		= CSize(200,100);
	m_bp.sizeDefault	= CSize(200,100);
	m_bp.bUnique		= FALSE;
	m_bp.bMaximized 	= FALSE;
	m_bp.dwStyle		= CBRS_XT_DEFAULT;
	m_bContextMenus     = true;

    // create the fonts used by the toolbar.
    UpdateControlBarFonts();
}

CXTControlBar::~CXTControlBar()
{
	SAFE_DELETE( m_pHook );
	SAFE_DELETE( m_pHookParent );
}

IMPLEMENT_DYNAMIC(CXTControlBar, CControlBar)

BEGIN_MESSAGE_MAP(CXTControlBar, CControlBar)
	//{{AFX_MSG_MAP(CXTControlBar)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_ADDREBAR, OnAddRebar)
	ON_MESSAGE(XTWM_REBAR_SIZECHILD, OnRebarSizeChild)
	ON_MESSAGE(XTWM_QUERYVISUALIZE, OnQueryVisualize)
	ON_MESSAGE(XTWM_QUERYHOTSPOT, OnQueryHotSpot)
//	ON_MESSAGE(XTWM_DOCKTRACKINGDONE, OnDragTrackingDone)
END_MESSAGE_MAP()

// Tells if this control bar is allowed to dock to a specific frame
bool CXTControlBar::CanDock(CFrameWnd* /*pFrame*/)
{
	// There is no frame-based distinction by default, it is either on or off
	return m_bEnableMultiFrameDocking;
}

LRESULT CXTControlBar::OnDragTrackingDone(WPARAM, LPARAM)
{
	DockTrackingDone();
	return 0;
}

void CXTControlBar::DockTrackingDone()
{
	if (m_pHook)
	{
		m_pHook->HookWindow((HWND)0);

		CFrameWnd* pFrame = GetParentFrame();
		if (m_pOriginalFrame && pFrame && m_pOriginalFrame != pFrame)
		{
			m_pHook->HookWindow(pFrame);
		}
	}
}

void CXTControlBar::SetBorders(int cxLeft, int cyTop, int cxRight, int cyBottom)
{
	ASSERT(cxLeft >= 0);
	ASSERT(cyTop >= 0);
	ASSERT(cxRight >= 0);
	ASSERT(cyBottom >= 0);

	m_cxLeftBorder = cxLeft;
	m_cyTopBorder = cyTop;
	m_cxRightBorder = cxRight;
	m_cyBottomBorder = cyBottom;
}

LRESULT CXTControlBar::WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	ASSERT_VALID(this);

	LRESULT lResult;
	switch (nMsg)
	{
	case WM_NOTIFY:
	case WM_COMMAND:
	case WM_DRAWITEM:
	case WM_MEASUREITEM:
	case WM_DELETEITEM:
	case WM_COMPAREITEM:
	case WM_VKEYTOITEM:
	case WM_CHARTOITEM:
		// send these messages to the owner if not handled
		if (OnWndMsg(nMsg, wParam, lParam, &lResult))
			return lResult;
		else
		{
			// try owner next
			CWnd* pOwner = GetOwner();
			ASSERT_VALID(pOwner);

			lResult = pOwner->SendMessage(nMsg, wParam, lParam);

			// special case for TTN_NEEDTEXTA and TTN_NEEDTEXTW
			if(nMsg == WM_NOTIFY)
			{
				NMHDR* pNMHDR = (NMHDR*)lParam;
				if (pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW)
				{
					TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
					TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;

					if (pNMHDR->code == TTN_NEEDTEXTA)
					{
						if (pTTTA->hinst == 0 && (!pTTTA->lpszText || !*pTTTA->lpszText))
						{
							// not handled by owner, so let bar itself handle it
							lResult = CWnd::WindowProc(nMsg, wParam, lParam);
						}
					}
					else if (pNMHDR->code == TTN_NEEDTEXTW)
					{
						if (pTTTW->hinst == 0 && (!pTTTW->lpszText || !*pTTTW->lpszText))
						{
							// not handled by owner, so let bar itself handle it
							lResult = CWnd::WindowProc(nMsg, wParam, lParam);
						}
					}
				}
			}
			return lResult;
		}
	}

	// otherwise, just handle in default way
	lResult = CWnd::WindowProc(nMsg, wParam, lParam);
	return lResult;
}

void CXTControlBar::OnWindowPosChanging(LPWINDOWPOS lpWndPos)
{
	// WINBUG: We call DefWindowProc here instead of CWnd::OnWindowPosChanging
	//	(which calls CWnd::Default, which calls through the super wndproc)
	//	because certain control bars that are system implemented (such as
	//	CToolBar with TBSTYLE_FLAT) do not implement WM_WINDOWPOSCHANGING
	//	correctly, causing repaint problems.  This code bypasses that whole
	//	mess.
	::DefWindowProc(m_hWnd, WM_WINDOWPOSCHANGING, 0, (LPARAM)lpWndPos);

	if (lpWndPos->flags & SWP_NOSIZE)
		return;

	// invalidate borders on the right
	CRect rect;
	GetWindowRect(&rect);
	CSize sizePrev = rect.Size();
	int cx = lpWndPos->cx;
	int cy = lpWndPos->cy;
	if (cx != sizePrev.cx && (m_dwStyle & CBRS_BORDER_RIGHT))
	{
		rect.SetRect(cx-afxData.cxBorder2, 0, cx, cy);
		InvalidateRect(&rect);
		rect.SetRect(sizePrev.cx-afxData.cxBorder2, 0, sizePrev.cx, cy);
		InvalidateRect(&rect);
	}

	// invalidate borders on the bottom
	if (cy != sizePrev.cy && (m_dwStyle & CBRS_BORDER_BOTTOM))
	{
		rect.SetRect(0, cy-afxData.cyBorder2, cx, cy);
		InvalidateRect(&rect);
		rect.SetRect(0, sizePrev.cy-afxData.cyBorder2, cx, sizePrev.cy);
		InvalidateRect(&rect);
	}
}

void CXTControlBar::EraseNonClient()
{
	// Get window DC that is clipped to the non-client area
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rc;
	GetWindowRect(rc);
	ScreenToClient(rc);
	rectClient.OffsetRect(-rc.left, -rc.top);
	dc.ExcludeClipRect(rectClient);
	rc.OffsetRect(-rc.left, -rc.top);

	// Paint to a memory device context to help
	// eliminate screen flicker...
	CXTMemDC memDC(&dc, rc, GetBackgroundColor());

	// and draw the borders in non-client area...
	DrawBorders(&memDC, rc);

	// and paint parts not drawn...
	memDC.FillSolidRect(rc, GetBackgroundColor());

	// and draw gripper in non-client area.
	DrawGripper(&memDC, rc);

	if ( xtAfxData.bXPMode )
	{
        // draw rounded corners for xp mode.
        if ( IsKindOf(RUNTIME_CLASS( CXTToolBar ) ) )
        {
            memDC.SetPixel( CPoint( rc.left,rc.top ),
                xtAfxData.clr3DFace );
            memDC.SetPixel( CPoint( rc.left,rc.bottom-1 ),
                xtAfxData.clr3DFace );
            memDC.SetPixel( CPoint( rc.right-1,rc.top ),
                xtAfxData.clr3DFace );
            memDC.SetPixel( CPoint( rc.right-1,rc.bottom-1 ),
                xtAfxData.clr3DFace );
        }
	}
}

void CXTControlBar::OnPaint() 
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient, GetBackgroundColor());
		
	// erase background now
	if (IsVisible())
		DoPaint(&memDC);	   // delegate to paint 
}

BOOL CXTControlBar::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}

void CXTControlBar::DoPaint(CDC* pDC)
{
	UNREFERENCED_PARAMETER(pDC);
	ASSERT_VALID(this);
}

void CXTControlBar::DrawBorders(CDC* pDC, CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	DWORD dwStyle = m_dwStyle;
	if (!(dwStyle & CBRS_BORDER_ANY))
		return;

	COLORREF clr3DShadow  = xtAfxData.clr3DShadow;
	COLORREF clr3DHilight = xtAfxData.clr3DHilight;

	if (xtAfxData.bXPMode && !IsKindOf(RUNTIME_CLASS(CXTReBar)))
	{
		clr3DShadow  = xtAfxData.clr3DFace;
		clr3DHilight = xtAfxData.clr3DFace;
	}
	
	// prepare for dark lines
	ASSERT(rect.top == 0 && rect.left == 0);
	CRect rect1, rect2;
	rect1 = rect;
	rect2 = rect;
	COLORREF clr = clr3DShadow;
	
	// draw dark line one pixel back/up
	if (dwStyle & CBRS_BORDER_3D)
	{
		rect1.right  -= CX_BORDER;
		rect1.bottom -= CY_BORDER;
	}
	if (dwStyle & CBRS_BORDER_TOP)
	{
		rect2.top += afxData.cyBorder2;
	}
	if (dwStyle & CBRS_BORDER_BOTTOM)
	{
		rect2.bottom -= afxData.cyBorder2;
	}
	
	// draw left and top
	if (dwStyle & CBRS_BORDER_LEFT)
	{
		pDC->FillSolidRect(0, rect2.top-1, CX_BORDER, rect2.Height()+1, clr);
	}
	if (dwStyle & CBRS_BORDER_TOP)
	{
		pDC->FillSolidRect(0, 0, rect.right-1, CY_BORDER, clr);
	}
	
	// draw right and bottom
	if (dwStyle & CBRS_BORDER_RIGHT)
	{
		pDC->FillSolidRect(rect1.right, rect2.top-1, -CX_BORDER, rect2.Height()+1, clr);
	}
	if (dwStyle & CBRS_BORDER_BOTTOM)
	{
		pDC->FillSolidRect(0, rect1.bottom, rect.right-1, -CY_BORDER, clr);
	}
	
	if (dwStyle & CBRS_BORDER_3D)
	{
		// prepare for highlight lines
		clr = clr3DHilight;

		// draw left and top
		if (dwStyle & CBRS_BORDER_LEFT)
		{
			pDC->FillSolidRect(1, rect2.top, CX_BORDER, rect2.Height(), clr);
		}
		if (dwStyle & CBRS_BORDER_TOP)
		{
			pDC->FillSolidRect(1, 1, rect.right-3, CY_BORDER, clr);
		}
		
		// draw right and bottom
		if (dwStyle & CBRS_BORDER_RIGHT)
		{
			pDC->FillSolidRect(rect.right, rect2.top-2, -CX_BORDER, rect2.Height()+3, clr);
		}
		if (dwStyle & CBRS_BORDER_BOTTOM)
		{
			pDC->FillSolidRect(0, rect.bottom, rect.right, -CY_BORDER, clr);
		}
	}
	if(InRebar())
	{
		if (dwStyle & CBRS_BORDER_LEFT)
		{
			rect.left += afxData.cxBorder2;
		}
		if (dwStyle & CBRS_BORDER_TOP)
		{
			rect.top += afxData.cyBorder2;
		}
		if (dwStyle & CBRS_BORDER_RIGHT)
		{
			rect.right -= afxData.cxBorder2;
		}
		if (dwStyle & CBRS_BORDER_BOTTOM)
		{
			rect.bottom -= afxData.cyBorder2;
		}
	}
	else
	{
		if(dwStyle & CBRS_BORDER_RIGHT)
		{
			pDC->FillSolidRect(rect.right-1, 0, rect.right, rect.bottom, clr3DShadow);
			rect.right--;
		}
		if(dwStyle & CBRS_BORDER_BOTTOM)
		{
			pDC->FillSolidRect(0, rect.bottom-1, rect.right, rect.bottom, clr3DShadow);
			rect.bottom--;
		}
		if(dwStyle & CBRS_BORDER_TOP)
		{
			pDC->FillSolidRect(0, 0, rect.right, 1, clr3DHilight);
			rect.top++;
		}
		if(dwStyle & CBRS_BORDER_LEFT)
		{
			pDC->FillSolidRect(0, 0, 1, rect.bottom, clr3DHilight);
			rect.left++;
		}
	}
}

#define CX_GRIPPER			3
#define CY_GRIPPER			3
#define CX_BORDER_GRIPPER	2
#define CY_BORDER_GRIPPER	2

void CXTControlBar::DrawGripper(CDC* pDC, const CRect& rect)
{
	if ((m_dwStyle & (CBRS_GRIPPER|CBRS_FLOATING)) == CBRS_GRIPPER)
	{
		// draw the gripper in the border
		if (m_dwStyle & CBRS_ORIENT_HORZ)
		{
			if (GetXTBarStyle() & CBRS_XT_TWOGRIP)
			{
				// Draw the first gripper.
				pDC->Draw3dRect( rect.left+2, rect.top+2, 3, rect.Height()-4,
					xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
				pDC->SetPixel( rect.left+2, rect.top+2+rect.Height()-5,
					xtAfxData.clr3DHilight);
				
				// Draw the second gripper.
				pDC->Draw3dRect( rect.left+5, rect.top+2, 3, rect.Height()-4,
					xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
				pDC->SetPixel( rect.left+5, rect.top+2+rect.Height()-5,
					xtAfxData.clr3DHilight);
			}
			else
			{
				if (xtAfxData.bXPMode)
				{
					CPen pen(PS_SOLID, 1, xtAfxData.clrXPGripper);
					CPen* pOldPen = pDC->SelectObject(&pen);

					int cy = rect.Height();

					int y = rect.top  + 4;
					int x = rect.left + 2;

					int i;
					for (i = 0; i < cy-9; i += 2, y += 2)
					{
						pDC->MoveTo(x,	   y);
						pDC->LineTo(x + 3, y);
					}

					pDC->SelectObject(pOldPen);
				}
				else
				{
					pDC->Draw3dRect(rect.left+CX_BORDER_GRIPPER,
						rect.top+m_cyTopBorder+2,
						CX_GRIPPER, rect.Height()-m_cyTopBorder-m_cyBottomBorder-4,
						xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
				}
			}
		}

		else
		{
			if (GetXTBarStyle() & CBRS_XT_TWOGRIP)
			{
				// Draw the first gripper.
				pDC->Draw3dRect( rect.left+2, rect.top+2, rect.Width()-4, 3,
					xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
				pDC->SetPixel( rect.left+2+rect.Width()-5, rect.top+2,
					xtAfxData.clr3DHilight);
				
				// Draw the second gripper.
				pDC->Draw3dRect( rect.left+2, rect.top+5, rect.Width()-4, 3,
					xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
				pDC->SetPixel( rect.left+2+rect.Width()-5, rect.top+5,
					xtAfxData.clr3DHilight);
			}
			else
			{
				if (xtAfxData.bXPMode)
				{
					CPen pen(PS_SOLID, 1, xtAfxData.clrXPGripper);
					CPen* pOldPen = pDC->SelectObject(&pen);
					
					int cx = rect.Width();
					
					int y = rect.top  + 2;
					int x = rect.left + 4;
					
					int i;
					for (i = 0; i < cx-9; i += 2, x += 2)
					{
						pDC->MoveTo(x, y);
						pDC->LineTo(x, y+3);
					}

					pDC->SelectObject(pOldPen);
				}
				else
				{
					pDC->Draw3dRect(rect.left+m_cyTopBorder+2,
						rect.top+CY_BORDER_GRIPPER,
						rect.Width()-m_cyTopBorder-m_cyBottomBorder-4, CY_GRIPPER,
						xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
				}
			}
		}
	}
}

void CXTControlBar::CalcInsideRect(CRect& rect, BOOL bHorz) const
{
	ASSERT_VALID(this);
	DWORD dwStyle = m_dwStyle;

	if (dwStyle & CBRS_BORDER_LEFT)
		rect.left += afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_TOP)
		rect.top += afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_RIGHT)
		rect.right -= afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect.bottom -= afxData.cyBorder2;

	// inset the top and bottom.
	if (bHorz)
	{
		rect.left += m_cxLeftBorder;
		rect.top += m_cyTopBorder;
		rect.right -= m_cxRightBorder;
		rect.bottom -= m_cyBottomBorder;
		if ((m_dwStyle & (CBRS_GRIPPER|CBRS_FLOATING)) == CBRS_GRIPPER)
			rect.left += CX_BORDER_GRIPPER+CX_GRIPPER+CX_BORDER_GRIPPER;
	}
	else
	{
		rect.left += m_cyTopBorder;
		rect.top += m_cxLeftBorder;
		rect.right -= m_cyBottomBorder;
		rect.bottom -= m_cxRightBorder;
		if ((m_dwStyle & (CBRS_GRIPPER|CBRS_FLOATING)) == CBRS_GRIPPER)
			rect.top += CY_BORDER_GRIPPER+CY_GRIPPER+CY_BORDER_GRIPPER;
	}
}

void CXTControlBar::EnableDocking(DWORD dwDockStyle)
{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);
	// CBRS_SIZE_DYNAMIC toolbar cannot have the CBRS_FLOAT_MULTI style
	ASSERT(((dwDockStyle & CBRS_FLOAT_MULTI) == 0) || ((m_dwStyle & CBRS_SIZE_DYNAMIC) == 0));

	ASSERT(m_pDockContext == NULL);
	m_dwDockStyle = dwDockStyle;

	if (m_pDockContext == NULL)
		m_pDockContext = new CXTDockContext(this); // always use CXTDockContext 

	if (m_hWndOwner == NULL)
		m_hWndOwner = ::GetParent(m_hWnd);
}

void CXTControlBar::EnableDockingEx(DWORD dwDockStyle, DWORD dwFlatStyle)
{
	EnableDocking(dwDockStyle);
	SetXTBarStyle(GetXTBarStyle()|dwFlatStyle);
}

void CXTControlBar::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ASSERT_VALID(this);

	// context menus turned off, call default implementation.
	if (!m_bContextMenus || !xtAfxData.bControlBarMenus)
	{
		CControlBar::OnRButtonDown(nFlags, point);
	}
	else
	{
		// Convert the cursor location to screen coordinates...
		CPoint pt = point;
		ClientToScreen(&pt);
		
		// No menu resource was defined, create default...
		if (m_menuID == 0)
		{
			if (m_pDockSite != NULL)
			{
				// Get a pointer to the parent frame, if floating make sure we get a
				// pointer to the top frame.
				CFrameWnd* pDockSite = m_pDockSite;
				if ((m_dwStyle & (CBRS_GRIPPER|CBRS_FLOATING)) == CBRS_GRIPPER)
				{
					if (pDockSite->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
						pDockSite = pDockSite->GetParentFrame();
				}
				
				if (pDockSite->IsFrameWnd())
				{
#ifdef _DEBUG
					// Check to see if the control bar's ID is valid, this will assert if
					// an incorrect ID was used during creation.
					_xtAfxAssertValidBarIDs(pDockSite);
#endif
					VERIFY(OnPopupMenu(pt, pDockSite));
					
					pDockSite->RecalcLayout();
				}
			}
		}
		else
		{
			CXTMenu menu;
			menu.LoadMenu(m_menuID);
			CXTMenu* pSubMenu = menu.GetSubMenu(m_nPos);
			
			CWnd* pWndPopupOwner = this;
			while (pWndPopupOwner->GetStyle() & WS_CHILD)
				pWndPopupOwner = pWndPopupOwner->GetParent();
			
			pSubMenu->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_HORIZONTAL,
				pt.x, pt.y, pWndPopupOwner );
		}
	}
}

BOOL CXTControlBar::OnPopupMenu(CPoint& point, CFrameWnd* pFrameWnd)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	ASSERT_VALID(this);
	BOOL bFound = FALSE;

	CXTMenu menu;
	menu.CreatePopupMenu();

#ifdef _DEBUG
	// Check to see if the control bar's ID is valid, this will assert if
	// an incorrect ID was used during creation.
	_xtAfxAssertValidBarIDs(pFrameWnd);
#endif
	
	// create a list based on the frames control bar list. Filter out all
	// control bars that we are not interested in. The remaining bars will
	// be used to create our popup menu.
	CPtrList listBars;
	POSITION pos;
	for (pos = pFrameWnd->m_listControlBars.GetHeadPosition(); pos!=NULL; pFrameWnd->m_listControlBars.GetNext(pos))
	{
		CControlBar* pBar = (CControlBar*)pFrameWnd->m_listControlBars.GetAt(pos);
		ASSERT_VALID(pBar);

		if (!pBar->IsKindOf(RUNTIME_CLASS(CXTMenuBar)) &&
			!pBar->IsKindOf(RUNTIME_CLASS(CXTReBar)) &&
#if _MSC_VER >= 1200 // MFC 6.0
			!pBar->IsKindOf(RUNTIME_CLASS(CReBar)) &&
#endif // _MSC_VER > 1200
			(pBar->IsKindOf(RUNTIME_CLASS(CXTControlBar)) ||
			 pBar->IsKindOf(RUNTIME_CLASS(CStatusBar))))
		{
			listBars.AddHead(pBar);
		}
	}

	POSITION pos1, pos2;

	// add docking windows...
	for ( pos1 = listBars.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		CXTDockWindow* pBar = (CXTDockWindow*)listBars.GetNext( pos1 );
		if (pBar && pBar->IsKindOf(RUNTIME_CLASS(CXTDockWindow)))
		{
			if (!pBar->HasContextMenu())
				continue;
			
			// define the style for the menu item.
			int nStyle = MF_STRING|MF_ENABLED;
			if (pBar->IsWindowVisible()) {
				nStyle |= MF_CHECKED;
			}

			// define the text for the menu item.
			CString strMenuText;
			pBar->GetWindowText(strMenuText);
			if (strMenuText.IsEmpty()) {
				strMenuText.LoadString(XT_IDS_DOCKWND);
			}

			// append the menu item, remove it from the pointer list and set
			// the found flag to true.
			menu.AppendMenu(nStyle, pBar->GetDlgCtrlID(), strMenuText);
			listBars.RemoveAt( pos2 );
			bFound = TRUE;
		}
	}

	// if dock windows were found, add a separator.
	if (bFound) {
		menu.AppendMenu(MF_SEPARATOR);
	}

	bool bCustomize = false;

	// add toolbars...
	for ( pos1 = listBars.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		CXTToolBar* pBar = (CXTToolBar*)listBars.GetNext( pos1 );
		if (pBar && pBar->IsKindOf(RUNTIME_CLASS(CXTToolBar)))
		{
			if (!pBar->HasContextMenu())
				continue;
			
			// define the style for the menu item.
			int nStyle = MF_STRING|MF_ENABLED;
			if (pBar->IsWindowVisible()) {
				nStyle |= MF_CHECKED;
			}
			
			// define the text for the menu item.
			CString strMenuText;
			pBar->GetWindowText(strMenuText);
			if (strMenuText.IsEmpty()) {
				strMenuText.LoadString(XT_IDS_TOOLBAR);
			}
			
			// append the menu item, remove it from the pointer list and set
			// the found flag to true.
			menu.AppendMenu(nStyle, pBar->GetDlgCtrlID(), strMenuText);
			listBars.RemoveAt( pos2 );
			bFound = TRUE;

			if ( pBar == this )
			{
				bCustomize = pBar->IsCustomizable();
			}
		}
	}

	// add remaining bars other than status bar.
	for ( pos1 = listBars.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		CXTControlBar* pBar = (CXTControlBar*)listBars.GetNext( pos1 );
		if (pBar && pBar->IsKindOf(RUNTIME_CLASS(CXTControlBar)))
		{
			if (!pBar->HasContextMenu())
				continue;
			
			// define the style for the menu item.
			int nStyle = MF_STRING|MF_ENABLED;
			if (pBar->IsWindowVisible()) {
				nStyle |= MF_CHECKED;
			}

			// define the text for the menu item.
			CString strMenuText;
			pBar->GetWindowText(strMenuText);
			if (strMenuText.IsEmpty()) {
				strMenuText.LoadString(XT_IDS_CONTROLBAR);
			}
			
			// append the menu item, remove it from the pointer list and set
			// the found flag to true.
			menu.AppendMenu(nStyle, pBar->GetDlgCtrlID(), strMenuText);
			listBars.RemoveAt( pos2 );
			bFound = TRUE;
		}
	}
	
	// status bar...
	for ( pos1 = listBars.GetHeadPosition(); ( pos2 = pos1 ) != NULL; )
	{
		CControlBar* pBar = (CControlBar*)listBars.GetNext( pos1 );
		if (pBar && pBar->IsKindOf(RUNTIME_CLASS(CStatusBar)))
		{
			// define the style for the menu item.
			int nStyle = MF_STRING|MF_ENABLED;
			if (pBar->IsWindowVisible()) {
				nStyle |= MF_CHECKED;
			}

			// define the text for the menu item.
			CString strMenuText;
			strMenuText.LoadString(XT_IDS_STATUSBAR);

			// append the menu item, remove it from the pointer list and set
			// the found flag to true.
			menu.AppendMenu(nStyle, pBar->GetDlgCtrlID(), strMenuText);
			listBars.RemoveAt( pos2 );
			bFound = TRUE;
		}
	}

	// add customize menu
	if (bCustomize)
	{
		// if dock windows were found, add a separator.
		if (bFound) {
			menu.AppendMenu(MF_SEPARATOR);
		}

		CString strMenuText;
		strMenuText.LoadString( XT_IDS_COLOR_CUST );
		menu.AppendMenu( MF_STRING|MF_ENABLED, XT_IDC_CUSTOMIZE, strMenuText );
	}

	// If any of the items were found, display the popup menu...
	if (bFound)
	{
		menu.TrackPopupMenu(
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_HORIZONTAL,
			point.x, point.y, pFrameWnd );
	}

	return TRUE;
}

BOOL CXTControlBar::ModifyXTBarStyle(DWORD dwRemove, DWORD dwAdd, BOOL bRedraw)
{
	SetXTBarStyle((m_bp.dwStyle & ~dwRemove) | dwAdd);

	// Force child size recalc.
	SetWindowPos (NULL, 0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);

	if (bRedraw) {
		Invalidate();
	}

	CXTDockBar* pDockBar = (CXTDockBar*)m_pDockBar;
	if (pDockBar && pDockBar->IsKindOf(RUNTIME_CLASS(CXTDockBar)))
	{
		pDockBar->SetXTBarStyle(m_bp.dwStyle);

		if (bRedraw) {
			pDockBar->Invalidate();
		}
	}

	return TRUE;
}

void CXTControlBar::SetXTBarStyle(DWORD dwStyle)
{
	if (m_bp.dwStyle != dwStyle) {
		m_bp.dwStyle = dwStyle;
	}
}

DWORD CXTControlBar::GetXTBarStyle()
{
	return m_bp.dwStyle;
}

void CXTControlBar::OnNcPaint() 
{
	EraseNonClient();
}

int CXTControlBar::OnCreate(LPCREATESTRUCT lpCS) 
{
	if (CControlBar::OnCreate(lpCS) == -1)
		return -1;
	
#ifdef _DEBUG
	// The following call will check that the control bar ID is in the valid
	// range of IDs.  It specifically does not check for the uniqueness of 
	// the ID: Although unique IDs are mandatory for permanent toolbars, they
	// are not mandatory for temporary toolbars.  Since there is no way to
	// distinguish between temporary and permanent toolbars at this point,
	// uniqueness is not checked.
	if (GetDlgCtrlID() != 0) 
	{
        // In some instances, the ID is still not set at this point - we
        // will only check when the ID is actually set.  If it isn't set,
        // it will be picked up by checks elsewhere.
		AssertValidCtrlID(false);
	}
#endif //_DEBUG

	// Set the font for the control bar.
	SetFont(&xtAfxData.font);
	
	// remember the frame the control bar was originally created and hook it
	// since closing that may not be the parent of the control bar when it gets
	// destroyed
	m_pOriginalFrame = GetParentFrame();

	return 0;
}

COLORREF CXTControlBar::GetBackgroundColor() const
{
	return xtAfxData.clr3DFace;
}

bool CXTControlBar::ShowMoveCursor() const
{
	CPoint point;
	::GetCursorPos( &point );
	ScreenToClient( &point );
	
	CRect rcWnd;
	GetWindowRect( &rcWnd );
	ScreenToClient( &rcWnd );

	CRect rcClient;
	GetClientRect( &rcClient );

	if ( ( m_dwStyle & CBRS_GRIPPER ) == CBRS_GRIPPER )
	{
		if ( m_dwStyle & CBRS_ORIENT_HORZ ) {
			rcWnd.right = rcClient.left;
		}
		else {
			rcWnd.bottom = rcClient.top;
		}
		
		return ( rcWnd.PtInRect( point ) == TRUE );
	}

	return false;
}

BOOL CXTControlBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if ( !m_bInReBar )
    {
        bool bDragging = ( m_pDockContext && m_pDockContext->m_bDragging );

        if ( bDragging || ShowMoveCursor( ) )
        {
            ::SetCursor( xtAfxData.hcurMove );
            return TRUE;
        }
    }
    
    return CControlBar::OnSetCursor(pWnd, nHitTest, message);
}

#ifdef _DEBUG
void CXTControlBar::AssertValidCtrlID(bool bCheckUnique /*= true*/)
{
	AssertValidCtrlID(this, bCheckUnique);
}

#define XT_MESSAGE_TEXT	\
	"Your control bar ID is out of range.  All control bar ID's\n"	\
	"should fall between 59500 and 59647.  See Codejock Software\n" \
	"Knowledge Base article 004.002 for more details."

void CXTControlBar::AssertValidCtrlID(CControlBar *pBar, bool bCheckUnique /*= true*/)
{
	if ( !pBar->m_pDockSite || !pBar->m_pDockSite->IsFrameWnd( ) )
		return;
	
	const int iCtrlID = pBar->GetDlgCtrlID();
	
	// If this asserts you are using the old ID range XT_IDW_CONTROLBAR_MIN 
	// (17000) to XT_IDW_CONTROLBAR_MAX (17500).  Your control ID's must fall 
	// between AFX_IDW_CONTROLBAR_FIRST (59392) and AFX_IDW_CONTROLBAR_LAST 
	// (59647), this is a mandatory requirement.  
	//
	// If your existing control bar numbers begin at 17000, change them to 
	// begin at 59500 instead.  We suggest that you begin with 59500 and 
	// increment from there to avoid conflicts with MFC constants, for example 
	// in your resource.h file you would define:
	// 
	// #define IDR_TOOLBAR_1 59500
	// #define IDR_TOOLBAR_2 59501
	// #define IDR_DOCKWND_1 59502
	// #define IDR_DOCKWND_2 59503
	
	BOOL bContinue = !( ( iCtrlID >= 17000 ) && ( iCtrlID <= 17500 ) );
	XT_ASSERT_MSG( bContinue, XT_MESSAGE_TEXT );
	
	if ( bContinue == false )
	{
		return;
	}
	
	// If this asserts, your control ID is out of range.  All control bar ID's 
	// must fall between the range of AFX_IDW_CONTROLBAR_FIRST ( 59392 ) and 
	// AFX_IDW_CONTROLBAR_LAST (59647), this is a mandatory requirement.
	//
	// The control bar ID is the last parameter to the control bar creation 
	// function.  For example, you usually have the following code in 
	// CMainFrame::OnCreate():
	//
	// m_wndToolbar.CreateEx(this, ..., AFX_IDW_TOOLBAR);
	//
	// You must ensure that the last parameter is in the valid range.  We suggest
	// that you begin with 59500 and increment from there to avoid conflicts with
	// MFC constants, for example in your resource.h file you would define:
	// 
	// #define IDR_TOOLBAR_1 59500
	// #define IDR_TOOLBAR_2 59501
	// #define IDR_DOCKWND_1 59502
	// #define IDR_DOCKWND_2 59503
	
	bContinue = ( iCtrlID >= AFX_IDW_CONTROLBAR_FIRST ) && ( iCtrlID <= AFX_IDW_CONTROLBAR_LAST );
	XT_ASSERT_MSG( bContinue, XT_MESSAGE_TEXT );
	
	if ( bCheckUnique == false || bContinue == false )
	{
		return;
	}
	
	// check that ID is unique
	POSITION pos;
	for (pos = pBar->m_pDockSite->m_listControlBars.GetHeadPosition(); 
	pos != NULL; 
	pBar->m_pDockSite->m_listControlBars.GetNext(pos))
	{
		CControlBar* pOtherBar = DYNAMIC_DOWNCAST(CControlBar, 
			(CObject *)pBar->m_pDockSite->m_listControlBars.GetAt(pos));
		ASSERT_VALID(pOtherBar);
		
		if (pOtherBar == pBar)
			continue;
		
		// If this asserts your control bar ID is already in use.  All control bar ID's 
		// must be unique, this is a mandatory requirement.
		//
		// Please note that all toolbars are created by default with the ID AFX_IDW_TOOLBAR, 
		// which is a default parameter to the toolbar's Create() function.  You can probably 
		// locate a call such as m_wndToolbar.Create(this) in your CMainFrame::OnCreate() 
		// function.  You must ensure that for each toolbar you are passing a unique ID in 
		// the last parameter, for example:
		//
		// m_wndToolbar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP, AFX_IDW_TOOLBAR + 100);
		//
		// We suggest that you begin with 59500 and increment from there to avoid conflicts with
		// MFC constants, for example in your resource.h file you would define:
		// 
		// #define IDR_TOOLBAR_1 59500
		// #define IDR_TOOLBAR_2 59501
		// #define IDR_DOCKWND_1 59502
		// #define IDR_DOCKWND_2 59503
		
		bContinue = pOtherBar->GetDlgCtrlID() != iCtrlID;
		XT_ASSERT_MSG( bContinue, XT_MESSAGE_TEXT );
		
		if ( bContinue == false )
		{
			return;
		}
	}
}
#endif //_DEBUG

BOOL CXTControlBar::SetStatusText(int nHit)
{
	if (!CControlBar::SetStatusText(nHit))
		return FALSE;

	// MFCBUG: If the control bar's owner does not have a status bar, the message
	// is eaten and the main window message prompt is not updated.  To fix this we 
	// will forward the message to the main window if our owner window does not 
	// have a status bar so the application's status text will be correctly updated.

	if (GetOwner()->GetDescendantWindow(AFX_IDW_STATUS_BAR, TRUE) == NULL)
	{
		if (nHit == -1) {
			AfxGetMainWnd()->SendMessage(WM_POPMESSAGESTRING, AFX_IDS_IDLEMESSAGE);
		}
		else {
			AfxGetMainWnd()->SendMessage(WM_SETMESSAGESTRING, nHit);
		}
	}
	
	return TRUE;
}

LRESULT CXTControlBar::OnAddRebar(WPARAM wp, LPARAM lp)
{
	m_bInReBar = true;
	return OnAddedToRebar((REBARBANDINFO*)lp, wp != 0);
}

bool CXTControlBar::OnAddedToRebar(REBARBANDINFO* pRBBI, bool bHorz)
{	
	CSize size = CalcFixedLayout(FALSE, bHorz);
	pRBBI->fMask |= RBBIM_CHILDSIZE;
	pRBBI->cxMinChild = size.cx;
	pRBBI->cyMinChild = size.cy;
	return true;	
}


LRESULT CXTControlBar::OnRebarSizeChild(WPARAM wp, LPARAM lp)
{
	OnRebarBandResized((XT_REBARSIZECHILDINFO*)lp, wp != 0);
	return 0;
}

void CXTControlBar::OnRebarBandResized(XT_REBARSIZECHILDINFO* /*pInfo*/, bool /*bHorz*/)
{
	// do nothing by default
}

LRESULT CXTControlBar::OnQueryVisualize(WPARAM, LPARAM)
{
	return IsFullWindowDrag();
}

bool CXTControlBar::IsFullWindowDrag()
{
	// by default, draw wire frame when dragging 
	return false;
}

bool CXTControlBar::GetHotSpot(LPSIZE /*pSize*/)
{
	return false;
}

LRESULT CXTControlBar::OnQueryHotSpot(WPARAM, LPARAM lp)
{
	return GetHotSpot(LPSIZE(LPVOID(lp)));
}

void CXTControlBar::UpdateControlBarFonts()
{
    CWindowDC dc(NULL);
    int iHeight = -::MulDiv(8, ::GetDeviceCaps(dc.m_hDC, LOGPIXELSY), 72);

    if (m_font.GetSafeHandle())
        m_font.DeleteObject();

    LOGFONT lf;
    xtAfxData.font.GetLogFont(&lf);
    lf.lfHeight = iHeight;
    m_font.CreateFontIndirect(&lf);

    if (m_fontBold.GetSafeHandle())
        m_fontBold.DeleteObject();

    xtAfxData.fontBold.GetLogFont(&lf);
    lf.lfHeight = iHeight;
    m_fontBold.CreateFontIndirect(&lf);

    if (m_fontVert.GetSafeHandle())
        m_fontVert.DeleteObject();

    xtAfxData.fontVCapt.GetLogFont(&lf);
    lf.lfHeight = iHeight;
    m_fontVert.CreateFontIndirect(&lf);
}

void CXTControlBar::EnableMultiFrameDocking(bool bEnable)
{
	ASSERT(::IsWindow(m_hWnd));
	m_bEnableMultiFrameDocking = bEnable;

	// clean up any previous hooking.
	if (m_pHook != NULL)
	{
		m_pHook->HookWindow((HWND)0);
		SAFE_DELETE( m_pHook );
	}

	if (m_pHookParent != NULL)
	{
		m_pHookParent->HookWindow((HWND)0);
		SAFE_DELETE( m_pHookParent );
	}
	
	// install the parent hooks.
	if (m_bEnableMultiFrameDocking)
	{
		m_pHook = new CXTDockContextWndHook(this);
		ASSERT(m_pHook != NULL);
		
		m_pHookParent = new CXTDockContextWndHook(this);
		ASSERT(m_pHookParent != NULL);
		m_pHookParent->HookWindow(m_pOriginalFrame);
	}
}

void CXTControlBar::OnDestroy() 
{
	// Check to see if any frames that we are owned by do not
	// have an active view that is owned by us, if so set the 
	// frames active view to NULL to avoid dangling pointers...

	// Check the top level frame for the application.
	CFrameWnd* pMainFrame = GetTopLevelFrame();
	if (pMainFrame && pMainFrame->IsFrameWnd())
	{
		CView* pActiveView = pMainFrame->GetActiveView();
		if (pActiveView && IsChild(pActiveView))
		{
			pMainFrame->SetActiveView(NULL);
		}
	}

	// Check the parent frame of the control bar
	CFrameWnd* pParentFrame = GetParentFrame();
	if (pParentFrame && pParentFrame->IsFrameWnd())
	{
		// don't bother if we have previously checked this, or if this is a floating frame
		if ((pMainFrame != pParentFrame) && !pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd)))
		{
			CView* pActiveView = pParentFrame->GetActiveView();
			if (pActiveView && IsChild(pActiveView))
			{
				pParentFrame->SetActiveView(NULL);
			}
		}
	}

	// Check the original frame that we were created with.
	if (m_pOriginalFrame && m_pOriginalFrame->IsFrameWnd())
	{
		// don't bother if we have previously checked this
		if ((m_pOriginalFrame != pMainFrame) && (m_pOriginalFrame != pParentFrame))
		{
			CView* pActiveView = m_pOriginalFrame->GetActiveView();
			if (pActiveView && IsChild(pActiveView))
			{
				m_pOriginalFrame->SetActiveView(NULL);
			}
		}
	}

	CControlBar::OnDestroy();
}

bool CXTControlBar::HasCmdHandlers(CWnd* pWnd)
{
    UNREFERENCED_PARAMETER(pWnd);
	return false; // handled in derived class.
}

CDialog* CXTControlBar::GetParentDialog() const
{
	// no Window attached
	if ( GetSafeHwnd( ) == NULL )
	{
		return NULL;
	}

	ASSERT_VALID(this);

	// start with one parent up
	CWnd* pParentWnd = GetParent( );
	while ( pParentWnd != NULL )
	{
		if ( pParentWnd->IsKindOf( RUNTIME_CLASS( CDialog ) ) )
		{
			return ( CDialog* )pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent( );
	}
	return NULL;
}

CXTDockWindow* CXTControlBar::GetParentDockWnd() const
{
	// no Window attached
	if ( GetSafeHwnd( ) == NULL )
	{
		return NULL;
	}

	ASSERT_VALID(this);

	// start with one parent up
	CWnd* pParentWnd = GetParent( );
	while ( pParentWnd != NULL )
	{
		if ( pParentWnd->IsKindOf( RUNTIME_CLASS( CXTDockWindow ) ) )
		{
			return ( CXTDockWindow* )pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent( );
	}
	return NULL;
}

LRESULT CXTControlBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER( lParam );

	// handle delay hide/show
	bool bVisible = ( ( GetStyle( ) & WS_VISIBLE ) == WS_VISIBLE );
	
	UINT swpFlags = 0;
	if ( ( m_nStateFlags & delayHide ) && bVisible ) {
		swpFlags = SWP_HIDEWINDOW;
	}
	else if ( ( m_nStateFlags & delayShow ) && !bVisible ) {
		swpFlags = SWP_SHOWWINDOW;
	}
	
	m_nStateFlags &= ~( delayShow | delayHide );
	if ( swpFlags != 0 )
	{
		SetWindowPos( NULL, 0, 0, 0, 0, swpFlags |
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
	}
	
	// the style must be visible and if it is docked
	// the dockbar style must also be visible
	if ( ( GetStyle( ) & WS_VISIBLE ) &&
		( m_pDockBar == NULL || ( m_pDockBar->GetStyle( ) & WS_VISIBLE ) ) )
	{
		// if we are owned by a dialog or docking window we need to
		// verify that the owner has command handlers defined.
		if ( GetParentDialog( ) || GetParentDockWnd( ) )
		{
			CFrameWnd* pTarget = ( CFrameWnd* )GetOwner( );
			if ( pTarget == NULL || !HasCmdHandlers( pTarget ) ) {
				pTarget = GetParentFrame( );
			}
			if ( pTarget != NULL ) {
				OnUpdateCmdUI( pTarget, ( BOOL )wParam );
			}
		}
		else
		{
			CFrameWnd* pTarget = ( CFrameWnd* )GetOwner( );
			if ( pTarget == NULL || !pTarget->IsFrameWnd( ) ) {
				pTarget = GetParentFrame( );
			}
			if ( pTarget != NULL ) {
				OnUpdateCmdUI( pTarget, ( BOOL )wParam );
			}
		}
	}
	
	return 0L;
}

void CXTControlBar::DelayShow(BOOL bShow)
{
	// call base, this probably changes visibility
	CControlBar::DelayShow(bShow);
	bool bIsVisible = IsVisible() != 0;
	// visibility changed, notify
	NoteBarVisible(bIsVisible);
}

