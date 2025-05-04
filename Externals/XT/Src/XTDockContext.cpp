// XTDockContext.cpp : implementation of the CXTDockContext class.
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
#include "XTDefines.h"
#include "XTWndHook.h"
#include "XTGlobal.h"
#include "XTCallbacks.h"
#include "XTSplitterDock.h"
#include "XTDockContext.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTDockWindow.h"
#include "XTDockBar.h"
#include "XTMiniDockFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef LAYOUT_LTR
#define LAYOUT_LTR	0x00000000
#endif // LAYOUT_LTR

#ifndef LAYOUT_RTL
#define LAYOUT_RTL	0x00000001
#endif // LAYOUT_RTL

#define HORZF(dw) (dw & CBRS_ORIENT_HORZ)
#define VERTF(dw) (dw & CBRS_ORIENT_VERT)

// This constant is used to expand an empty dock bar rectangle (i.e. the one that currently
// has nothing docked). This gives us some sort of marging to dock a toolbar to a dock bar
// since the operation works from current cursor position.
static const int DOCKING_MARGIN = 4;

/////////////////////////////////////////////////////////////////////////////
// CXTDockContext
/////////////////////////////////////////////////////////////////////////////

CXTDockContext::CXTDockContext(CControlBar* pBar) 
: CDockContext(pBar)
, m_ptOrig(0)
, m_bFullWindowDrag(FALSE)
, m_bRecalcPending(TRUE)
{
    m_bDragging = FALSE;

	// For reasons unknown, CDockContext constructor does not initialize
	// y coordinate of m_ptMRUFloatPos structure. the line below circumvents 
	// around that
	m_ptMRUFloatPos.y = CW_USEDEFAULT;

	// same for this
	m_rectMRUDockPos.right = 0;
	m_rectMRUDockPos.bottom = 0;

}

CXTDockContext::~CXTDockContext()
{

}

// adds a listener for drag state change notifications
void CXTDockContext::AddTrackStateListener(CObject* listener, LISTENERCB cb)
{
	m_trackStateListeners.Add(listener, cb);
}

// removes a listener
void CXTDockContext::RemoveListener(CObject* listener)
{
	m_trackStateListeners.Remove(listener);
}

void CXTDockContext::AdjustRectangle(CRect& rect, CPoint pt)
{
	int nXOffset = (pt.x < rect.left) ? (pt.x - rect.left) :
		(pt.x > rect.right) ? (pt.x - rect.right) : 0;
	int nYOffset = (pt.y < rect.top) ? (pt.y - rect.top) :
		(pt.y > rect.bottom) ? (pt.y - rect.bottom) : 0;
	rect.OffsetRect(nXOffset, nYOffset);
}

void CXTDockContext::StartDrag(CPoint pt)
{
	ASSERT_VALID(m_pBar);
	m_bDragging = TRUE;
    CPoint ptOrig = pt;
    BOOL bFloatingOrig = m_pBar->IsFloating();

    UINT uMRUDockIDOrig = m_uMRUDockID ;
	CRect rectMRUDockPosOrig = m_rectMRUDockPos;
    DWORD dwMRUFloatStyleOrig = m_dwMRUFloatStyle;
	CPoint ptMRUFloatPosOrig  = m_ptMRUFloatPos;

	InitLoop();

	if (m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
	{
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptOrig = m_ptLast = pt;

		CSize sizeHorz = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(0, LM_VERTDOCK);
		CSize sizeFloat = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);
		
		m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectDragVert = CRect(rect.TopLeft(), sizeVert);
		
		// calculate frame dragging rectangle
		m_rectFrameDragHorz = CRect(rect.TopLeft(), sizeFloat);
		m_rectFrameDragVert = CRect(rect.TopLeft(), sizeFloat);
		
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		
		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}
	
	else if (m_pBar->m_dwStyle & CBRS_SIZE_FIXED)
	{
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptOrig = m_ptLast = pt;

		CSize sizeHorz = m_pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
		CSize sizeVert = m_pBar->CalcDynamicLayout(-1, LM_VERTDOCK);
		
		m_rectFrameDragHorz = m_rectDragHorz = CRect(rect.TopLeft(), sizeHorz);
		m_rectFrameDragVert = m_rectDragVert = CRect(rect.TopLeft(), sizeVert);
		
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}
	
	else
	{
		CRect rect;
		m_pBar->GetWindowRect(rect);
		m_ptOrig = m_ptLast = pt;

		BOOL bHorz = HORZF(m_dwStyle);
		DWORD dwMode = !bHorz ? (LM_HORZ | LM_HORZDOCK) : LM_VERTDOCK;
		CSize size = m_pBar->CalcDynamicLayout(-1, dwMode);
		
		if (bHorz)
		{
			m_rectDragHorz = rect;
			m_rectDragVert = CRect(CPoint(pt.x - rect.Height()/2, rect.top), size);
		}
		
		else 
		{
			m_rectDragVert = rect;
			m_rectDragHorz = CRect(CPoint(rect.left, pt.y - rect.Width()/2), size);
		}
		
		m_rectFrameDragHorz = m_rectDragHorz;
		m_rectFrameDragVert = m_rectDragVert;
		
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragHorz);
		CMiniFrameWnd::CalcBorders(&m_rectFrameDragVert);
		m_rectFrameDragHorz.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
		m_rectFrameDragVert.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	}

	AdjustRectangle(m_rectDragHorz, pt);
	AdjustRectangle(m_rectDragVert, pt);
	AdjustRectangle(m_rectFrameDragHorz, pt);
	AdjustRectangle(m_rectFrameDragVert, pt);

	if (!m_pBar->IsFloating())
	{
		// adjust the floating frame rectangle such that cursor position matches 
		// the control bar hot spot
		CSize extent = m_rectFrameDragHorz.Size();
		if (m_pBar->SendMessage(XTWM_QUERYHOTSPOT, 0, (LPARAM)(LPVOID)&extent))
		{
			// control bar defines hot spot location, pin the rect to it
			CPoint ptHotSpot = m_rectFrameDragHorz.TopLeft() + extent;
			CSize offset = pt - ptHotSpot;
			m_rectFrameDragHorz.OffsetRect(offset);
		}
	}

    m_dwOverDockStyle = CanDock();

    CRect rectDragHorzOrig = m_rectDragHorz;
	CRect rectFrameDragHorzOrig = m_rectFrameDragHorz;
	CRect rectDragVertOrig = m_rectDragVert;
	CRect rectFrameDragVertOrig = m_rectFrameDragVert;
    DWORD dwOrigDockStyle = m_dwOverDockStyle;

    // force cursor to update
    CDockBar* pDockBarOrig = m_pBar->m_pDockBar;
    m_pBar->PostMessage(WM_SETCURSOR, (WPARAM)m_pBar->m_hWnd, 0);

	Move(pt); 
    if (Track())
    {
        // recover original docking position if the bar was undocked
        BOOL bFloating = m_pBar->IsFloating();
        if (bFloating && !bFloatingOrig)
        {
            m_uMRUDockID = uMRUDockIDOrig;
		    m_rectMRUDockPos = rectMRUDockPosOrig;		
        }
        if (!bFloating && bFloatingOrig)
        {
            m_dwMRUFloatStyle = dwMRUFloatStyleOrig;
	        m_ptMRUFloatPos = ptMRUFloatPosOrig;
        }
    }
    else
    {
        // drag has been cancelled
	    if (m_bFullWindowDrag)
        {
            // restore original position
            m_rectDragHorz = rectDragHorzOrig;
	        m_rectFrameDragHorz = rectFrameDragHorzOrig;
	        m_rectDragVert = rectDragVertOrig; 
	        m_rectFrameDragVert = rectFrameDragVertOrig;
            if (bFloatingOrig)
            {
                m_dwOverDockStyle =  0;
                EndDrag();
            }
            else
            {
	            m_dwOverDockStyle =  dwOrigDockStyle;
                CRect rect = (m_dwOverDockStyle & CBRS_ORIENT_VERT) ?
			                    m_rectDragVert : m_rectDragHorz;		
		        pDockBarOrig->DockControlBar(m_pBar, &rect);            
            }
		    m_pDockSite->RecalcLayout(TRUE);
        }
    }
	m_bDragging = FALSE;
}

DWORD CXTDockContext::CanDock()
{
	BOOL bStyleHorz;
	DWORD dwDock = 0; 
	DWORD dwCurr = 0; 
	
	ASSERT(m_dwStyle != 0);
	ASSERT_KINDOF(CFrameWnd, m_pDockSite);

	bStyleHorz = HORZF(m_dwStyle);
	bStyleHorz = m_bFlip ? !bStyleHorz : bStyleHorz;
	
	if (bStyleHorz && HORZF(m_dwDockStyle))
	{
		dwDock = CanDock(m_rectDragHorz,
			m_dwDockStyle & ~CBRS_ORIENT_VERT);
	}
	
	else if (VERTF(m_dwDockStyle))
	{
		dwDock = CanDock(m_rectDragVert,
			m_dwDockStyle & ~CBRS_ORIENT_HORZ);
	}
	
	if (!m_bFlip)
	{
		if (dwDock == 0 && HORZF(m_dwDockStyle))
		{
			dwCurr = CanDock(m_rectDragVert,
				m_dwDockStyle & ~CBRS_ORIENT_VERT);
			dwDock = CanDock(m_rectDragHorz,
				m_dwDockStyle & ~CBRS_ORIENT_VERT);
			dwDock = (dwDock == dwCurr) ? dwDock : 0;
		}
		
		if (dwDock == 0 && VERTF(m_dwDockStyle))
		{
			dwCurr = CanDock(m_rectDragHorz,
				m_dwDockStyle & ~CBRS_ORIENT_HORZ);
			dwDock = CanDock(m_rectDragVert,
				m_dwDockStyle & ~CBRS_ORIENT_HORZ);
			dwDock = (dwDock == dwCurr) ? dwDock : 0;
		}
	}
	
	return dwDock;
}

DWORD CXTDockContext::CanDock(CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar)
{
	dwDockStyle &= CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI;
	if (ppDockBar != NULL)
	{
		*ppDockBar = NULL;
	}

	DWORD dwResult;
	if (CanDock(m_pDockSite, dwResult, rect, dwDockStyle, ppDockBar))
	{
		return dwResult;
	}

	if (CFrameWnd* pOther = GetAlternateSite())
	{
		if (CanDock(pOther, dwResult, rect, dwDockStyle, ppDockBar))
		{
			DeferRecalcLayout(m_pDockSite);
			DeferRecalcLayout(pOther);
			m_pDockSite = pOther;
			return dwResult;
		}
	}
	return 0;
}


void CXTDockContext::DeferRecalcLayout(CFrameWnd* pFrame)
{
	pFrame->DelayRecalcLayout(FALSE);
	m_arrFramesPendingRecalc.Add(pFrame);
}

// Figure out an alternative frame the toolbar can be docked to
CFrameWnd* CXTDockContext::GetAlternateSite()
{
	if (m_pDockSite && m_pBar && m_pBar->IsKindOf(RUNTIME_CLASS(CXTControlBar)))
	{
		CXTControlBar* pBar = (CXTControlBar*)m_pBar;
		// Check if any owned popup would pick up this			
		//	- loop through all popups and filter out a CMainFrame's
		// 	- check to see if we can dock the toolbar to a main frame
		CPoint ptCursor;
		if (::GetCursorPos(&ptCursor))
		{								
			if (HWND hMainWnd = ::AfxGetMainWnd()->GetSafeHwnd())
			{
				HWND hWnd = ::GetWindow(hMainWnd, GW_HWNDFIRST);
				while (hWnd)
				{
					if (CWnd* pWnd = CWnd::FromHandlePermanent(hWnd))
					{
						// we skip mini frames - you can only redock to a (sibling) main frame
						if (pWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)) && 
							!pWnd->IsKindOf(RUNTIME_CLASS(CXTMiniDockFrameWnd)))
						{
							CFrameWnd* pFrame = (CFrameWnd*)pWnd;
							CRect rectWindow;
							pFrame->GetWindowRect(&rectWindow);
							if (rectWindow.PtInRect(ptCursor))
							{
								if (pFrame == m_pDockSite)
								{
									// still the same top level frame, fall out
									// to logic defining child frame
									break;
								}

																
								// found a popup that can be the target of 
								// this docking operation, return it
								// if the control bar allows redocking
								// otherwise return 0 (no suitable target)
								bool bCanRedock = pBar->CanDock(pFrame);
								return  (bCanRedock) ? pFrame : 0;
							}
						}
					}
					hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);					
				}
			}
		}

		// check to see if MDI frame exists, if so use it as an alternate
		CFrameWnd* pOther = m_pDockSite->GetActiveFrame();
		if (pOther != m_pDockSite && pBar->CanDock(pOther))
		{
			return pOther;
		}

		// check to see if a parent frame exists, use it if so
		pOther = m_pDockSite->GetParentFrame();
		if (pOther && pOther != m_pDockSite && pBar->CanDock(pOther))
		{
			return pOther;
		}
	}

	// cannot find any other frames for docking
	return 0;
}

bool CXTDockContext::CanDock(CFrameWnd* pFrame, DWORD& dwResult, CRect rect, DWORD dwDockStyle, CDockBar** ppDockBar)
{
	ASSERT_VALID(pFrame);
	
	POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
	CPoint ptCursor;
	BOOL bPtCursor = ::GetCursorPos(&ptCursor);
	while (pos != NULL)
	{
		CXTDockBar* pDockBar = (CXTDockBar*)pFrame->m_listControlBars.GetNext(pos);

		if (pDockBar->IsDockBar() && pDockBar->IsWindowVisible() &&
			(pDockBar->m_dwStyle & dwDockStyle & CBRS_ALIGN_ANY) &&
			(!pDockBar->m_bFloating ||
				(dwDockStyle & pDockBar->m_dwStyle & CBRS_FLOAT_MULTI)))
		{
			CRect rectBar;
			pDockBar->GetWindowRect(&rectBar);
			if (rectBar.Width() == 0)
			{
				rectBar.right += DOCKING_MARGIN;
				rectBar.left -= DOCKING_MARGIN;
			}
			if (rectBar.Height() == 0)
			{
				rectBar.bottom += DOCKING_MARGIN;
				rectBar.top -= DOCKING_MARGIN;
			}
			CRect rectParent;
			if (CWnd* pWndParent = pDockBar->GetParent())
			{
				pWndParent->GetClientRect(&rectParent);
				pWndParent->ClientToScreen(&rectParent);

				CRect rectCommon;
				if (rectCommon.IntersectRect(rectParent, rectBar))
				{
					rectBar = rectCommon;
				}
				
				BOOL bHit = (bPtCursor) 
								? rectBar.PtInRect(ptCursor)
								: rectBar.IntersectRect(rectBar, rect);
				if (bHit) 
				{
					if (ppDockBar != NULL)
						*ppDockBar = pDockBar;
					dwResult = pDockBar->m_dwStyle & dwDockStyle;
					return true;
				}
			}
		}
	}
	return false;
}

// This class ensures proper state cleanup after Track() exits
class CXTTrackDockContextCleanup
{
    CXTDockContext* m_pCtx;
    HWND m_hWndFocus;
    BOOL m_bReset;
public:
    CXTTrackDockContextCleanup(CXTDockContext* pCtx)
        : m_pCtx(pCtx), m_hWndFocus(::GetFocus()), m_bReset(TRUE)
    {
#if 0
        if (m_hWndFocus)
        {
            m_pCtx->m_pBar->SetFocus();
        }
#endif
		m_pCtx->m_bTracking = true;
		m_pCtx->m_trackStateListeners.Notify(pCtx);
    }

    ~CXTTrackDockContextCleanup()
    {    
		m_pCtx->m_bTracking = false;
		m_pCtx->m_trackStateListeners.Notify(m_pCtx);

        m_pCtx->CancelLoop();
        m_pCtx->m_pDockSite->RecalcLayout(TRUE);  
        m_pCtx->m_bRecalcPending = true;
#if 0
        if (m_hWndFocus && ::IsWindow(m_hWndFocus))
        {
		    ::SetFocus(m_hWndFocus);
        }
#endif
    }
}; 

// helper to lock/unlock main window for updates
class CXTWindowUpdateLock
{
    CWnd* m_pWnd;
public:
    CXTWindowUpdateLock(CWnd* pWnd, bool bLock)
        : m_pWnd((bLock) ? pWnd : NULL)
    {
        if (m_pWnd)
        {
            m_pWnd->LockWindowUpdate();
        }		
    }

    ~CXTWindowUpdateLock()
    {
        if (m_pWnd)
        {        
            m_pWnd->UnlockWindowUpdate();
        }
    }
};


BOOL CXTDockContext::Track()
{
    CXTTrackDockContextCleanup trackCleanup(this);
    
	if (::GetCapture() != NULL)
		return FALSE;
    
	m_pBar->SetCapture();
	ASSERT(m_pBar == CWnd::GetCapture());
	
    CPoint ptLastMouse;
    bool   bEatMosueMove = false;
	while (CWnd::GetCapture() == m_pBar)
	{
		MSG msg;
        
        // pump WM_PAINT first for better feedback
	    while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
	    {
		    if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
			    break;
		    DispatchMessage(&msg);
	    }

        // get a message
		if (!::GetMessage(&msg, NULL, 0, 0))
		{
			AfxPostQuitMessage(msg.wParam);
			break;
		}

        if (CWnd::GetCapture() != m_pBar)
        {
            // capture was stolen while repainting
            break;
        }

        // eat mouse move messages that are at the same mouse position
        if (msg.message != WM_MOUSEMOVE)
        {
            bEatMosueMove = false;
        }
        else
        {
            // terminate loop if we happen to get mouse move with button up
            if (!(msg.wParam & MK_LBUTTON))
            {
                // mouse move with button up - eventually, the capture was silently stolen
                // terminate the loop
                break;
            }
            if (bEatMosueMove && msg.pt.x == ptLastMouse.x && msg.pt.y == ptLastMouse.y )
            {
                continue;
            }
            ptLastMouse = msg.pt;
            bEatMosueMove = true;
        }

        // lock updates -only if full window drag and toolbar is not floating since the latter
        // is more efficient without locking
        CXTWindowUpdateLock lockUpdate(m_pDockSite, m_bFullWindowDrag && !m_pBar->IsFloating());
		switch (msg.message)
		{
		case WM_LBUTTONUP:
			if (m_bDragging)
            {
				EndDrag();
            }
			else
            {
				EndResize();
            }
			return TRUE;
		case WM_MOUSEMOVE:
			if (m_bDragging)
            {
				Move(msg.pt);
            }
			else
            {
				Stretch(msg.pt);
            }
			break;
		case WM_KEYUP:
			if (m_bDragging)
				OnKey((int)msg.wParam, FALSE);
			break;
		case WM_KEYDOWN:
			if (m_bDragging)
				OnKey((int)msg.wParam, TRUE);
			if (msg.wParam == VK_ESCAPE)
			{
				return FALSE;
			}
			break;
		case WM_RBUTTONDOWN:
			return FALSE;
		
			// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}        

		// flush pending layout recalc
		FlushRecalcLayout(m_pDockSite);
		int i;
		for (i = 0; i < m_arrFramesPendingRecalc.GetSize(); i++)
		{
			FlushRecalcLayout(m_arrFramesPendingRecalc[i]);
		}
		m_arrFramesPendingRecalc.RemoveAll();
	}
	
	return FALSE;
}

void CXTDockContext::FlushRecalcLayout(CFrameWnd* pFrame)
{
    if (pFrame->m_nIdleFlags & CFrameWnd::idleLayout)
    {
        pFrame->RecalcLayout(FALSE);
        pFrame->m_nIdleFlags &= ~CFrameWnd::idleLayout;
    }
}
	
void CXTDockContext::EndDrag()
{
	if (m_dwOverDockStyle != 0)
	{
		CXTDockBar* pDockBar = GetDockBar(m_dwOverDockStyle);
		if( pDockBar == NULL )
			return; 
		
		if (m_pBar->m_pDockBar != pDockBar && CXTDockWindow::IsDockWindow(m_pBar) && 
			((CXTDockWindow *) m_pBar)->IsMaximized())
			((CXTDockBar *) m_pBar->m_pDockBar)->Normalize((CXTDockWindow *) m_pBar);
		
		CRect rect = (m_dwOverDockStyle & CBRS_ORIENT_VERT) ?
			m_rectDragVert : m_rectDragHorz;
		
		UINT uID = _AfxGetDlgCtrlID(pDockBar->m_hWnd);
		m_uMRUDockID = uID;
		m_rectMRUDockPos = rect;
		pDockBar->ScreenToClient(&m_rectMRUDockPos);
        
		pDockBar->DockControlBar(m_pBar, &rect);
		m_pDockSite->RecalcLayout( FALSE );
		m_bRecalcPending = true;
	}
	else 
	{
		if (CXTDockWindow::IsDockWindow(m_pBar) && ((CXTDockWindow *) m_pBar)->IsMaximized())
        {
		    ((CXTDockBar *) m_pBar->m_pDockBar)->Normalize((CXTDockWindow *) m_pBar);
        }

		if ((m_dwStyle & CBRS_SIZE_DYNAMIC) || (HORZF(m_dwStyle) && !m_bFlip) ||
            (VERTF(m_dwStyle) && m_bFlip))
		{
	        FloatControlBar(CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI));
		}
		else 
		{
		    FloatControlBar(CBRS_ALIGN_LEFT | (m_dwDockStyle & CBRS_FLOAT_MULTI));
		}
		if (CXTDockWindow::IsDockWindow(m_pBar))
        {
			((CXTDockWindow*)m_pBar)->SetFloatSize();
        }
		if (!m_bRecalcPending)
		{
            m_pDockSite->m_nIdleFlags &= ~CFrameWnd::idleLayout;
			m_bRecalcPending = false;
		}
	}    
}

void CXTDockContext::FloatControlBar(DWORD dwFloatStyle)
{
    m_ptMRUFloatPos = m_rectFrameDragHorz.TopLeft();
    m_dwMRUFloatStyle = dwFloatStyle;

    // the code below checks to see if we can simply get the floating toolbar window moving
    // NB:  - no layout recalc is issued
    //      - WS_EX_DLGMODALFRAME enables copying underlying bits
    //      - SWP_NOSENDCHANGING ignores recalcs incurred by repositioning the window
    CControlBar* pBar = m_pBar;
    DWORD dwStyle =  m_dwMRUFloatStyle;
    CPoint point = m_ptMRUFloatPos;
    if (pBar->m_pDockSite != NULL && pBar->m_pDockBar != NULL)
	{
		CDockBar* pDockBar = pBar->m_pDockBar;
		ASSERT_KINDOF(CDockBar, pDockBar);
		if (pDockBar->m_bFloating && pDockBar->GetDockedCount() == 1 &&
			(dwStyle & pDockBar->m_dwStyle & CBRS_ALIGN_ANY) != 0)
		{
			CMiniDockFrameWnd* pDockFrame =
				(CMiniDockFrameWnd*)pDockBar->GetParent();
			ASSERT(pDockFrame != NULL);
			ASSERT_KINDOF(CMiniDockFrameWnd, pDockFrame);
            pDockFrame->ModifyStyleEx(0, WS_EX_DLGMODALFRAME);
			pDockFrame->SetWindowPos(NULL, point.x, point.y, 0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOSENDCHANGING);
			return;
		}
	}

    DWORD dwMRUFloatStyle = m_dwMRUFloatStyle;
	if (pBar->m_dwStyle & CBRS_SIZE_DYNAMIC)
	{
		dwMRUFloatStyle |= CBRS_SIZE_DYNAMIC;
	}
	CMiniDockFrameWnd* pDockFrame = m_pDockSite->CreateFloatingFrame(dwMRUFloatStyle);	
	ASSERT(pDockFrame != NULL);
	pDockFrame->SetWindowPos(NULL, point.x, point.y, 0, 0,
		SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	if (pDockFrame->m_hWndOwner == NULL)
		pDockFrame->m_hWndOwner = pBar->m_hWnd;

	CDockBar* pDockBar = (CDockBar*)pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
	ASSERT(pDockBar != NULL);
	ASSERT_KINDOF(CDockBar, pDockBar);

	pDockBar->DockControlBar(pBar);
	pDockFrame->RecalcLayout(TRUE);
	if (GetWindowLong(pBar->m_hWnd, GWL_STYLE) & WS_VISIBLE)
	{
		pDockFrame->ShowWindow(SW_SHOWNA);
		pDockFrame->UpdateWindow();
	}
}

CXTDockBar* CXTDockContext::GetDockBar(DWORD dwOverDockStyle)
{
	CDockBar* pBar;
	if (HORZF(dwOverDockStyle))
	{
		ASSERT_KINDOF(CFrameWnd, m_pDockSite);
		CanDock(m_rectDragHorz,	dwOverDockStyle & ~CBRS_ORIENT_VERT, &pBar);
		return DYNAMIC_DOWNCAST(CXTDockBar, pBar);
	}
	
	if (VERTF(dwOverDockStyle))
	{
		CanDock(m_rectDragVert,	dwOverDockStyle & ~CBRS_ORIENT_HORZ, &pBar);
		return DYNAMIC_DOWNCAST(CXTDockBar, pBar);
	}
	
	return NULL;
}

void CXTDockContext::ToggleDocking()
{
	if (m_pBar->IsFloating())
	{
		// Dock it only if is allowed to be docked
		if (m_pBar->m_dwDockStyle & CBRS_ALIGN_ANY)
		{
			ASSERT((m_uMRUDockID >= AFX_IDW_DOCKBAR_TOP && 
				m_uMRUDockID <= AFX_IDW_DOCKBAR_BOTTOM) ||
				m_uMRUDockID == 0);

			CRect rect = m_rectMRUDockPos;
			CXTDockBar* pDockBar = NULL;
			if (m_uMRUDockID != 0)
			{
				pDockBar = (CXTDockBar*)m_pDockSite->GetControlBar(m_uMRUDockID);
				pDockBar->ClientToScreen(&rect);
			}

			// dock it at the specified position, RecalcLayout will snap
			if (CXTDockWindow::IsDockWindow(m_pBar))
				((CXTDockWindow *)m_pBar)->ModifyXTBarStyle(0, CBRS_XT_NEWDOCKED, FALSE);
			if (pDockBar)
			{
				pDockBar->DockControlBar(m_pBar, &rect);
			}
			else
			{
				m_pDockSite->ReDockControlBar(m_pBar, pDockBar, &rect);
			}
			m_pDockSite->RecalcLayout();
		}
	}
	else
	{
		CPoint ptFloat = m_ptMRUFloatPos;
		if (ptFloat.x < 0 || ptFloat.y < 0)
		{
			ptFloat = m_rectMRUDockPos.TopLeft();
			m_pBar->GetParent()->ClientToScreen(&ptFloat);
		}
		m_rectFrameDragHorz = CRect(ptFloat, m_rectFrameDragHorz.Size());
		if (CXTDockWindow::IsDockWindow(m_pBar) && ((CXTDockWindow *) m_pBar)->IsMaximized())
			((CXTDockBar *) m_pBar->m_pDockBar)->Normalize((CXTDockWindow *) m_pBar);
		
		FloatControlBar(CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI));
		
		if (CXTDockWindow::IsDockWindow(m_pBar))
			((CXTDockWindow*)m_pBar)->SetFloatSize();
	}
}

#define m_rectRequestedSize     m_rectDragHorz
#define m_rectActualSize        m_rectDragVert
#define m_rectActualFrameSize   m_rectFrameDragHorz
#define m_rectFrameBorders      m_rectFrameDragVert

void CXTDockContext::StartResize(int nHitTest, CPoint pt)
{
    ASSERT_VALID(m_pBar);
    ASSERT(m_pBar->m_dwStyle & CBRS_SIZE_DYNAMIC);
    m_bDragging = FALSE;
    CPoint ptOriginal = pt;
	
    InitLoop();
	
    // GetWindowRect returns screen coordinates(not mirrored)
    // So if the desktop is mirrored then turn off mirroring
    // for the desktop dc so that we draw correct focus rect
#if _MSC_VER >= 1200 // MFC 6.0
    if (m_pDC && m_pDC->GetLayout() & LAYOUT_RTL)
        m_pDC->SetLayout(LAYOUT_LTR);
#endif // _MSC_VER >= 1200

    // get true bar size (including borders)
    CRect rect;
    m_pBar->GetWindowRect(rect);
    m_ptLast = pt;
    m_nHitTest = nHitTest;
	
    CSize size = m_pBar->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH);
    m_rectRequestedSize = CRect(rect.TopLeft(), size);
    m_rectActualSize = CRect(rect.TopLeft(), size);
    m_rectActualFrameSize = CRect(rect.TopLeft(), size);
	
    // calculate frame rectangle
    CMiniFrameWnd::CalcBorders(&m_rectActualFrameSize);
    m_rectActualFrameSize.InflateRect(-afxData.cxBorder2, -afxData.cyBorder2);
	
    m_rectFrameBorders = CRect(CPoint(0,0),
        m_rectActualFrameSize.Size() - m_rectActualSize.Size());
	
    // initialize tracking state and enter tracking loop
    m_dwOverDockStyle = 0;
    Stretch(pt);   // call it here to handle special keys
    if (!Track() && m_bFullWindowDrag)
    {
        // restore original size
        Stretch(ptOriginal);
    }
}

void CXTDockContext::Stretch(CPoint pt)
{
    CPoint ptOffset = pt - m_ptLast;
    CSize size;
	
    // offset all drag rects to new position
    if (m_nHitTest != HTTOP && m_nHitTest != HTBOTTOM)
    {
        if (m_nHitTest == HTLEFT || m_nHitTest == HTTOPLEFT || m_nHitTest == HTBOTTOMLEFT)
            m_rectRequestedSize.left += ptOffset.x;
        else
            m_rectRequestedSize.right += ptOffset.x;
        size = m_pBar->CalcDynamicLayout(m_rectRequestedSize.Width(), LM_HORZ);
    }
	
    if (m_nHitTest != HTLEFT && m_nHitTest != HTRIGHT)
    {
        if (m_nHitTest == HTTOP || m_nHitTest == HTTOPLEFT || m_nHitTest == HTTOPRIGHT)
            m_rectRequestedSize.top += ptOffset.y;
        else
            m_rectRequestedSize.bottom += ptOffset.y;
        size = m_pBar->CalcDynamicLayout(m_rectRequestedSize.Height(), LM_HORZ|LM_LENGTHY);
    }
	
    CRect rectDesk;
    HWND hWndDesk = ::GetDesktopWindow();
    ::GetWindowRect(hWndDesk, &rectDesk);
    CRect rectTemp = m_rectActualFrameSize;
	
    // left, top, top left
    if (m_nHitTest == HTLEFT || m_nHitTest == HTTOP || m_nHitTest == HTTOPLEFT)
    {
        rectTemp.left = rectTemp.right -
            (size.cx + m_rectFrameBorders.Width());
        rectTemp.top = rectTemp.bottom -
            (size.cy + m_rectFrameBorders.Height());
        CRect rect;
        if (rect.IntersectRect(rectDesk, rectTemp))
        {
            m_rectActualSize.left = m_rectActualSize.right - size.cx;
            m_rectActualSize.top = m_rectActualSize.bottom - size.cy;
            m_rectActualFrameSize.left = rectTemp.left;
            m_rectActualFrameSize.top = rectTemp.top;
        }
    } else if (m_nHitTest == HTTOPRIGHT) // top right
    {
        rectTemp.top = rectTemp.bottom -
            (size.cy + m_rectFrameBorders.Height());
        rectTemp.right = rectTemp.left +
            (size.cx + m_rectFrameBorders.Width());
        CRect rect;
        if (rect.IntersectRect(rectDesk, rectTemp))
        {
            m_rectActualSize.left = m_rectActualSize.right - size.cx;
            m_rectActualSize.bottom = m_rectActualSize.top + size.cy;
            m_rectActualFrameSize.right = rectTemp.right;
            m_rectActualFrameSize.top = rectTemp.top;
        }
    } else if (m_nHitTest == HTBOTTOMLEFT) // bottom left
    {
        rectTemp.bottom = rectTemp.top +
            (size.cy + m_rectFrameBorders.Height());
        rectTemp.left = rectTemp.right -
            (size.cx + m_rectFrameBorders.Width());
        CRect rect;
        if (rect.IntersectRect(rectDesk, rectTemp))
        {
            m_rectActualSize.right = m_rectActualSize.left + size.cx;
            m_rectActualSize.top = m_rectActualSize.bottom - size.cy;
            m_rectActualFrameSize.left = rectTemp.left;
            m_rectActualFrameSize.bottom = rectTemp.bottom;
        }
    } else// bottom, right, bottom right
    {
        rectTemp.right = rectTemp.left +
            (size.cx + m_rectFrameBorders.Width());
        rectTemp.bottom = rectTemp.top +
            (size.cy + m_rectFrameBorders.Height());
        CRect rect;
        if (rect.IntersectRect(rectDesk, rectTemp))
        {
            m_rectActualSize.right = m_rectActualSize.left + size.cx;
            m_rectActualSize.bottom = m_rectActualSize.top + size.cy;
            m_rectActualFrameSize.right = rectTemp.right;
            m_rectActualFrameSize.bottom = rectTemp.bottom;
        }
    }
    m_ptLast = pt;
	
    // update feedback
    DrawFocusRect();
}

void CXTDockContext::Move(CPoint pt)
{
	CPoint ptOffset = pt - m_ptLast;
    if (ptOffset.x != 0 || ptOffset.y != 0)
    {
	    // offset all drag rects to new position
	    m_rectDragHorz.OffsetRect(ptOffset);
	    m_rectFrameDragHorz.OffsetRect(ptOffset);
	    m_rectDragVert.OffsetRect(ptOffset);
	    m_rectFrameDragVert.OffsetRect(ptOffset);
	    m_ptLast = pt;

	    // if control key is down don't dock
	    m_dwOverDockStyle = m_bForceFrame ? 0 : CanDock();

	    // update feedback
	    DrawFocusRect();
    }
}

void CXTDockContext::DrawFocusRect(BOOL bRemoveRect)
{
    if (m_bFullWindowDrag)
    {
	    if (!bRemoveRect) 
        {
            if (m_bDragging)
			    EndDrag();
		    else
			    EndResize();
        }
        return;
    }

    ASSERT(m_pDC != NULL);

	// default to thin frame
	CSize size(CX_BORDER, CY_BORDER);

	// determine new rect and size
	CRect rect;
	CBrush* pWhiteBrush = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
	CBrush* pDitherBrush = CDC::GetHalftoneBrush();
	CBrush* pBrush = pWhiteBrush;

	CRect rectDock(0, 0, 0, 0);
	if (CDockBar *pDockBar = GetDockBar(m_dwOverDockStyle))
	{
		pDockBar->GetWindowRect(&rectDock);
	}

	if (HORZF(m_dwOverDockStyle))
	{
		rect = m_rectDragHorz;
		CPoint ptMid(rect.left + rect.Width()/2, rect.top + rect.Height()/2);
		// check if new row or not
		if (CXTDockWindow::IsDockWindow(m_pBar) && !rectDock.PtInRect(ptMid))
		{
			// new row: width equal to Dock bar width
			rect.left = rectDock.left;
			rect.right = rectDock.right;
		}
	}
	else if (VERTF(m_dwOverDockStyle))
	{
		rect = m_rectDragVert;
		CPoint ptMid(rect.left + rect.Width()/2, rect.top + rect.Height()/2);
		// check if new row or not
		if (CXTDockWindow::IsDockWindow(m_pBar) && !rectDock.PtInRect(ptMid))
		{
			// new row: width equal to Dock bar width
			rect.top = rectDock.top;
			rect.bottom = rectDock.bottom;
		}
	}
	else
	{
		// use thick frame instead
		size.cx = GetSystemMetrics(SM_CXFRAME) - CX_BORDER;
		size.cy = GetSystemMetrics(SM_CYFRAME) - CY_BORDER;
		if (m_bDragging == FALSE || (HORZF(m_dwStyle) && !m_bFlip) || (VERTF(m_dwStyle) && m_bFlip))
			rect = m_rectFrameDragHorz;
		else
			rect = m_rectFrameDragVert;
		pBrush = pDitherBrush;
	}
	if (bRemoveRect)
		size.cx = size.cy = 0;

	if ((HORZF(m_dwOverDockStyle) || VERTF(m_dwOverDockStyle)))
	{
		// looks better one pixel in (makes the bar look pushed down)
		rect.InflateRect(-CX_BORDER, -CY_BORDER);
	}

	// draw it and remember last size
	m_pDC->DrawDragRect(&rect, size, &m_rectLast, m_sizeLast,
		pBrush, m_bDitherLast ? pDitherBrush : pWhiteBrush);
	m_rectLast = rect;
	m_sizeLast = size;
	m_bDitherLast = (pBrush == pDitherBrush);
}

void CXTDockContext::OnKey(int nChar, BOOL bDown)
{
	if (nChar == VK_CONTROL)
		UpdateState(&m_bForceFrame, bDown);
	if (nChar == VK_SHIFT)
		UpdateState(&m_bFlip, bDown);
}

void CXTDockContext::UpdateState(BOOL* pFlag, BOOL bNewValue)
{
	if (*pFlag != bNewValue)
	{
		*pFlag = bNewValue;
		m_bFlip = (HORZF(m_dwDockStyle) && VERTF(m_dwDockStyle) && m_bFlip); // shift key
		m_dwOverDockStyle = (m_bForceFrame) ? 0 : CanDock();
		DrawFocusRect();
	}
}


void CXTDockContext::InitLoop()
{
	m_bFullWindowDrag = false;
	if (xtAfxData.bToolBarVisualize)
	{
		if (m_pBar->SendMessage(XTWM_QUERYVISUALIZE))
		{
			m_bFullWindowDrag = true;
		}
	}

    if (m_bFullWindowDrag)
    {
        // get styles from bar
	    m_dwDockStyle = m_pBar->m_dwDockStyle;
	    m_dwStyle = m_pBar->m_dwStyle & CBRS_ALIGN_ANY;
	    ASSERT(m_dwStyle != 0);

	    // initialize state
	    m_rectLast.SetRectEmpty();
	    m_sizeLast.cx = m_sizeLast.cy = 0;
	    m_bForceFrame = m_bFlip = m_bDitherLast = FALSE;

	    // not used
        m_pDC = NULL;

        return;
    }

    // do default
    CDockContext::InitLoop();
}

void CXTDockContext::CancelLoop()
{    
    if (m_bFullWindowDrag)
    {
        ReleaseCapture();
        return;
    }

    // do default
	CDockContext::CancelLoop();
}

void CXTDockContext::EndResize()
{
    m_pBar->CalcDynamicLayout(m_rectActualSize.Width(), LM_HORZ | LM_COMMIT);
	m_pDockSite->FloatControlBar(m_pBar, m_rectActualFrameSize.TopLeft(),
		CBRS_ALIGN_TOP | (m_dwDockStyle & CBRS_FLOAT_MULTI) | CBRS_SIZE_DYNAMIC);
}

