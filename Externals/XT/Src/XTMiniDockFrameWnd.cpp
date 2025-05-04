// XTMiniDockFrameWnd.cpp : implementation of the CXTMiniDockFrameWnd class.
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
#include "XTGlobal.h"
#include "XTResource.h"
#include "XTMiniDockFrameWnd.h"
#include "XTSplitterDock.h"
#include "XTCallbacks.h"
#include "XTDockContext.h"
#include "XTDockBar.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTDockWindow.h"
#include "XTMemDC.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTExpMenuWnd.h"
#include "XTPriv.h"
#include "XTCustomizeAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTFrameButton - helper
/////////////////////////////////////////////////////////////////////////////

class CXTFrameButton
{
public:
	CXTFrameButton(CXTMiniDockFrameWnd* pParentFrame, CXTControlBar* pBar, UINT uStyle=0)
	{
		m_bIsHidden    = false;
		m_bIsHot       = false;
		m_bIsPushed    = false;
		m_bIsPainted   = false;
		m_uStyle       = uStyle;

		m_pParentFrame = pParentFrame;
		ASSERT_VALID(m_pParentFrame);

		m_pToolBar = (CXTToolBar*)pBar;
		ASSERT_VALID(m_pToolBar);
	}

	void SetHot(bool bIsHot) {
		m_bIsHot = bIsHot;
	}
	bool IsHot() {
		return m_bIsHot;
	}
	void SetPushed(bool bIsPushed) {
		m_bIsPushed = bIsPushed;
	}
	bool IsPushed() {
		return m_bIsPushed;
	}
	void SetPainted(bool bIsPainted) {
		m_bIsPainted = bIsPainted;
	}
	bool IsPainted() {
		return m_bIsPainted;
	}
	UINT GetButtonStyle() {
		return m_uStyle;
	}
	bool NeedsRepaint() {
		return m_bIsPainted || m_bIsHot;
	}
	void SetNormal() {
		m_bIsPainted = m_bIsPushed = m_bIsHot = false;
	}
	bool IsNormal() {
		return !m_bIsPainted && !m_bIsPushed && !m_bIsHot;
	}
	void SetToolBar(CXTToolBar* pToolBar) {
		m_pToolBar = pToolBar;
	}
	CXTToolBar* GetToolBar() {
		return m_pToolBar;
	}
	bool IsHidden() {
		return m_bIsHidden;
	}
	CRect GetButtonRect(LPRECT lpRect=NULL)
	{
		int cx = ::GetSystemMetrics(SM_CXEDGE);
		int cy = ::GetSystemMetrics(SM_CYEDGE);

		CRect r1;
		if (lpRect == NULL) {
			m_pParentFrame->GetWindowRect(r1);
		}
		else {
			r1.CopyRect(lpRect);
		}

        CRect r2 = r1;
		if (m_uStyle == DFCS_CAPTIONCLOSE)
		{
			r2.right  -= cx;
			r2.top    += cy;
			r2.bottom  = r2.top   + (xtAfxData.cySmIcon-1);
			r2.left    = r2.right - xtAfxData.cxSmIcon;
		}
		else
		{
			r2.right  -= cx + xtAfxData.cxSmIcon;

            if (!xtAfxData.bXPMode)
                r2.right += 2;

			r2.top    += cy;
			r2.bottom  = r2.top   + (xtAfxData.cySmIcon-1);
			r2.left    = r2.right - xtAfxData.cxSmIcon;
		}

        if (!xtAfxData.bXPMode)
            r2.DeflateRect(2,2);
        
        if (r2.left < r1.left + cx*2)
            m_bIsHidden = true;
		else
			m_bIsHidden = false;
        
		return r2;
	}

protected:

	bool                 m_bIsHidden;
	bool				 m_bIsHot;
	bool                 m_bIsPushed;
	bool				 m_bIsPainted;
	UINT                 m_uStyle;
	CXTMiniDockFrameWnd* m_pParentFrame;
	CXTToolBar*			 m_pToolBar;
};

/////////////////////////////////////////////////////////////////////////////
// CXTMiniDockFrameWnd
/////////////////////////////////////////////////////////////////////////////

CXTMiniDockFrameWnd::CXTMiniDockFrameWnd()
: m_currentAppearance(APPEARANCE_UNDEFINED)
, m_bInitCompleted(false)
{
	m_pPopupWnd   = NULL;
	m_pPressedBtn = NULL;	

	// create the image list used by our mini-frame buttons
	m_imageList.Create(XT_IDB_BTN_FRAME, 10, 1, RGB(255,0,255));
}

CXTMiniDockFrameWnd::~CXTMiniDockFrameWnd()
{
	while (!m_arrButtons.IsEmpty())
	{
		CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.RemoveHead();
		SAFE_DELETE( pFrameBtn );
	}
}

IMPLEMENT_DYNCREATE(CXTMiniDockFrameWnd, CMiniDockFrameWnd)

BEGIN_MESSAGE_MAP(CXTMiniDockFrameWnd, CMiniDockFrameWnd)
	//{{AFX_MSG_MAP(CXTMiniDockFrameWnd)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_NCPAINT()
	ON_WM_NCACTIVATE()
    ON_MESSAGE_VOID(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	ON_COMMAND(XT_IDC_CUSTOMIZE, OnCustomize)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitMiniFrame)
	ON_MESSAGE_VOID(XTWM_POPUPCLOSED, OnPopupClosed)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

void CXTMiniDockFrameWnd::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	if (nHitTest == HTCAPTION)
	{
		CXTDockBar* pDockBar = GetXTDockBar();
		if (pDockBar && (pDockBar->m_dwStyle & CBRS_FLOAT_MULTI) == 0)
		{
			CXTControlBar* pBar = GetXTControlBar();
			if (pBar)
			{
				CXTDockContext* pDockContext = GetXTDockContext(pBar);
				if (pDockContext)
				{
					ActivateTopParent();
					pDockContext->ToggleDocking();
					return;
				}
			}
		}
	}
	
	CMiniDockFrameWnd::OnNcLButtonDblClk(nHitTest, point);
}

BOOL CXTMiniDockFrameWnd::Create(CWnd* pParent, DWORD dwBarStyle)
{
	// Let the base class do all the work.
	if (!CMiniDockFrameWnd::Create(pParent, dwBarStyle))
		return FALSE;

	m_bInitCompleted = false;
	PostMessage(XTWM_INITIAL_UPDATE);

	m_toolTip.Create(this);

	return TRUE;
}

LRESULT CXTMiniDockFrameWnd::OnInitMiniFrame(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	if (m_bInitCompleted)
	{
		// do not init twice
		return 0;
	}
	m_bInitCompleted = true;

	CXTControlBar* pBar = GetXTControlBar();
	if (!pBar) {
		return 0;
	}

	if (IsDockWindow(pBar))
	{
		// remove the MFS_4THICKFRAME style to allow diagonal resizing, and
        // set WS_CLIPCHILDREN and WS_CLIPSIBLINGS flags so child items and not painted over.
		ModifyStyle(MFS_4THICKFRAME, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
		SetFont (&xtAfxData.font);
	}

	else if (IsControlBar(pBar))
	{
		if (!IsMenubar(pBar))
		{
			// reset the button array.
			while (!m_arrButtons.IsEmpty())
			{
				CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.RemoveHead();
				SAFE_DELETE( pFrameBtn );
			}

			CXTFrameButton* pFrameBtn1 = new CXTFrameButton(this, pBar, DFCS_CAPTIONCLOSE);
			ASSERT(pFrameBtn1 != NULL);
			m_arrButtons.AddTail(pFrameBtn1);

			if (IsToolbar(pBar) && ((CXTToolBar*)pBar)->GetCustomBar())
			{
				CXTFrameButton* pFrameBtn2 = new CXTFrameButton(this, pBar, DFCS_SCROLLDOWN);
				ASSERT(pFrameBtn2 != NULL);
				m_arrButtons.AddTail(pFrameBtn2);
			}
        }
        
        // remove the close button.
        ModifyStyle(WS_SYSMENU, 0);
	}

    // remember most recent appearance
    m_currentAppearance = GetCurrentAppearance();

	// flush delayed layout
	RecalcLayout(true);

    // update interior and the frame
    if (IsWindowVisible())
    {
        Invalidate();
        SetWindowPos(NULL, 0,0,0,0,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
    }

	return 0;
}

void CXTMiniDockFrameWnd::DrawXPFrameControl(CDC* pDC, CRect& rc, CXTFrameButton* pFrameButton)
{
	CRect r = pFrameButton->GetButtonRect(&rc);

	// if the button is hidden ( falls outside of the mini-dock frame
	// window coordinates ) return.
	if (pFrameButton->IsHidden())
		return;

    int iSavedDC = pDC->SaveDC();

    COLORREF crBack = xtAfxData.bXPMode ? 
        xtAfxData.clr3DShadow : xtAfxData.clr3DFace;
	
    COLORREF crText = xtAfxData.bXPMode ? 
        xtAfxData.clrHighlightText : xtAfxData.clrMenuText;

	const bool bIsCloseBtn = (pFrameButton->GetButtonStyle() == DFCS_CAPTIONCLOSE);

	pFrameButton->SetPainted(false);

	if (pFrameButton->IsHot() && xtAfxData.bXPMode)
	{
		if (pFrameButton->IsPushed())
		{
			crBack = xtAfxData.clrXPHighlightPushed;
			crText = xtAfxData.clrXPHighlightText;
		}
		else
		{
			crBack = xtAfxData.clrXPHighlight;
			crText = xtAfxData.clrMenuText;
		}

		if (!bIsCloseBtn && m_pPopupWnd)
		{
			crBack = xtAfxData.clrXPBarFace;
			crText = xtAfxData.clrMenuText;
		}
	}

	pDC->FillSolidRect(&r, crBack);
    
    if (xtAfxData.bXPMode)
	{
		if (pFrameButton->IsHot())
		{
			COLORREF crBorder = xtAfxData.clrHighlight;

			if (!bIsCloseBtn && m_pPopupWnd)
				crBorder = xtAfxData.clrXPMenuBorder;

			pDC->Draw3dRect(&r, crBorder, crBorder);
			
			pFrameButton->SetPainted(true);
		}
	}
	else
    {
		if ((!bIsCloseBtn && m_pPopupWnd) || pFrameButton->IsPushed())
	    {
			r.InflateRect(1,1);
			pDC->Draw3dRect(&r,
				xtAfxData.clrWindowFrame, xtAfxData.clr3DFace);

			r.DeflateRect(1,1);
			pDC->Draw3dRect(&r,
				xtAfxData.clr3DShadow, xtAfxData.clr3DHilight);

			pFrameButton->SetPainted(true);
		}
		else
		{
			r.InflateRect(1,1);
			pDC->Draw3dRect(&r,
				xtAfxData.clr3DFace, xtAfxData.clrWindowFrame);

			r.DeflateRect(1,1);
			pDC->Draw3dRect(&r,
				xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
		}
    }

	CPoint pt = r.TopLeft();
	
	pt.x += (r.Size().cx-10)/2;
	pt.y += (r.Size().cy-10)/2;
	
	if (r.Size().cx <= 10) {
		pt.x = 0;
	}
	if (r.Size().cy <= 10) {
		pt.y = 0;
	}

	CBrush brush(crText);
				
	if (bIsCloseBtn)
	{
		CXTIconHandle hIcon( m_imageList.ExtractIcon( 2 ) );
		pDC->DrawState( pt, r.Size(), hIcon, DST_ICON|DSS_MONO, (HBRUSH)brush );
	}
	else
	{
		CXTIconHandle hIcon( m_imageList.ExtractIcon( 3 ) );
		pDC->DrawState( pt, r.Size(), hIcon, DST_ICON|DSS_MONO, (HBRUSH)brush );
	}

    pDC->RestoreDC(iSavedDC);
}

void CXTMiniDockFrameWnd::OnNcPaint()
{
	CXTControlBar* pBar = GetXTControlBar();
	if (!pBar)
	{
		Default();
		return;
	}

	if (IsControlBar(pBar) && !IsDockWindow(pBar))
	{
		CWindowDC dc(this);

		// get the size of the client area.
		CRect rectClient;
		GetClientRect(rectClient);

		// get the size of the window area.
		CRect rc;
		GetWindowRect(rc);

		// convert to client coordinates
		ScreenToClient(rc);

		// exclude the client area from paint operation.
		rectClient.OffsetRect(-rc.left, -rc.top);
		dc.ExcludeClipRect(rectClient);
		rc.OffsetRect(-rc.left, -rc.top);

		// Paint to a memory device context to help
		// eliminate screen flicker...
        CXTMemDC memDC(&dc, rc, xtAfxData.bXPMode ?
            xtAfxData.clrXPBarFace : xtAfxData.clr3DFace);

		// save the current state of the device context.
		int iSavedDC = memDC.SaveDC();

		// get the caption text for the toolbar.
		CString strText;
		pBar->GetWindowText(strText);

		// If the caption text is empty, use default
		if (strText.IsEmpty()) {
			strText.LoadString(XT_IDS_TOOLBAR);
		}

		int cx = ::GetSystemMetrics(SM_CXEDGE);
		int cy = ::GetSystemMetrics(SM_CYEDGE);

		CRect rcCapt = rc;
        if (xtAfxData.bXPMode)
        {
			memDC.Draw3dRect(&rc,
				xtAfxData.clr3DShadow, xtAfxData.clr3DShadow);
		    
			rc.DeflateRect(1,1);
			memDC.Draw3dRect(&rc,
				xtAfxData.clr3DShadow, xtAfxData.clr3DShadow);

			rc.DeflateRect(1,1);
		    memDC.SetPixel(rc.left,    rc.top,      xtAfxData.clr3DShadow);
		    memDC.SetPixel(rc.left,    rc.bottom-1, xtAfxData.clr3DShadow);
		    memDC.SetPixel(rc.right-1, rc.top,      xtAfxData.clr3DShadow);
		    memDC.SetPixel(rc.right-1, rc.bottom-1, xtAfxData.clr3DShadow);

			rcCapt = rc;
            rcCapt.DeflateRect(cy-1, cy-1);

            rcCapt.bottom = rcCapt.top + (xtAfxData.cySmIcon-1) + cy - 1;
            memDC.FillSolidRect(&rcCapt, xtAfxData.clr3DShadow);
        }
        else
        {
            cy += 1;
			
            memDC.DrawEdge(&rcCapt, EDGE_RAISED, BF_RECT);
			
            rcCapt.DeflateRect(cy, cy);
			
            rcCapt.bottom = rcCapt.top + (xtAfxData.cySmIcon-1) - cy +1;
            
			COLORREF clrLeftCapt = xtAfxData.clrActiveCaption;
			
			// query gradient info (usually TRUE for Win98/Win2k)
			BOOL bGradient = FALSE;
			::SystemParametersInfo(SPI_GETGRADIENTCAPTIONS,
				0, &bGradient, 0);
			
			// solid color
			if (bGradient == FALSE)
			{
				memDC.FillSolidRect(&rcCapt, clrLeftCapt);
			}
			else
			{
				// gradient from left to right or from bottom to top
				// get second gradient color (the right end)
				COLORREF clrRightCapt = xtAfxData.clrGradActiveCapt;
				
				// this will make 2^6 = 64 fountain steps
				int nShift = 6;
				int nSteps = 1 << nShift;
				
				int i;
				for (i = 0; i < nSteps; i++)
				{
					// do a little alpha blending
					int nRed   = (GetRValue(clrLeftCapt) * (nSteps - i) + GetRValue(clrRightCapt) * i) >> nShift;
					int nGreen = (GetGValue(clrLeftCapt) * (nSteps - i) + GetGValue(clrRightCapt) * i) >> nShift;
					int nBlue  = (GetBValue(clrLeftCapt) * (nSteps - i) + GetBValue(clrRightCapt) * i) >> nShift;
					
					// then paint with the resulting color
					CRect r = rcCapt;
					r.left  = rcCapt.left + ((i       * rcCapt.Width()) >> nShift);
					r.right = rcCapt.left + (((i + 1) * rcCapt.Width()) >> nShift);
					
					if (r.Width() > 0) {
						memDC.FillSolidRect(r, RGB(nRed, nGreen, nBlue));
					}
				}
			}
        }

		// set the font attributes for the caption.
		memDC.SelectObject(&xtAfxData.fontBold);
		memDC.SetTextColor(xtAfxData.clrActiveCaptText);
		memDC.SetBkMode(TRANSPARENT);

		// get the size required to display the caption.
		CSize sizeText = memDC.GetTextExtent(strText);

		// define the left side of the caption string.
        CRect rcText = rcCapt;
		rcText.left += cx;

		if (IsControlBar(pBar))
		{
			POSITION pos;
			for (pos = m_arrButtons.GetHeadPosition(); pos != NULL; m_arrButtons.GetNext(pos))
			{
				CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetAt(pos);
				ASSERT(pFrameBtn != NULL);

				CRect r = pFrameBtn->GetButtonRect();
				if (!pFrameBtn->IsHidden())
				{
					rcText.right -= (r.Width() + cx);
				}
			}
		}

		// paint the caption.
		memDC.DrawText(strText, &rcText, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		// draw the frame control buttons.
		if (IsControlBar(pBar))
		{
			POSITION pos;
			for (pos = m_arrButtons.GetHeadPosition(); pos != NULL; m_arrButtons.GetNext(pos))
			{
				CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetAt(pos);
				ASSERT(pFrameBtn != NULL);
				DrawXPFrameControl(&memDC, rc, pFrameBtn);
			}
		}

		// restore the device context.
		memDC.RestoreDC(iSavedDC);
	}
	else
	{
		Default();
	}
}

bool CXTMiniDockFrameWnd::IsValidBar(CControlBar* pBar) const
{
	if (!::IsWindow(m_hWnd) || !pBar || !pBar->IsKindOf(RUNTIME_CLASS(CControlBar))) 
	{
		return false;
	}
	
	ASSERT_VALID(pBar);
	return true;
}

bool CXTMiniDockFrameWnd::IsControlBar(CControlBar* pBar) const
{
	if (IsValidBar(pBar) && pBar->IsKindOf(RUNTIME_CLASS(CXTControlBar)))
	{
		return true;
	}

	return false;
}

bool CXTMiniDockFrameWnd::IsToolbar(CControlBar* pBar) const
{
	if (IsValidBar(pBar) && pBar->IsKindOf(RUNTIME_CLASS(CXTToolBar)))
	{
		return true;
	}

	return false;
}

bool CXTMiniDockFrameWnd::IsMenubar(CControlBar* pBar) const
{
	if (IsValidBar(pBar) && pBar->IsKindOf(RUNTIME_CLASS(CXTMenuBar)))
	{
		return true;
	}

	return false;
}

bool CXTMiniDockFrameWnd::IsDockWindow(CControlBar* pBar) const
{
	if (IsValidBar(pBar) && pBar->IsKindOf(RUNTIME_CLASS(CXTDockWindow)))
	{
		return true;
	}

	return false;
}

CXTDockContext* CXTMiniDockFrameWnd::GetXTDockContext(CControlBar* pBar) const
{
	if (IsControlBar(pBar))
	{
		return (CXTDockContext*)pBar->m_pDockContext;
	}

	return NULL;
}

CXTDockBar* CXTMiniDockFrameWnd::GetXTDockBar() const
{
	return (CXTDockBar*)&m_wndDockBar;
}

CXTControlBar* CXTMiniDockFrameWnd::GetXTControlBar() const
{
	CXTDockBar* pDockBar = GetXTDockBar();
	if (!pDockBar)
	{
		return NULL;
	}

	int nPos = 1;
	CControlBar* pBar = NULL;
	while (pBar == NULL && nPos < m_wndDockBar.m_arrBars.GetSize())
	{
		pBar = pDockBar->GetDockedControlBar(nPos++);
	}

	if (!IsValidBar(pBar))
	{
		return NULL;
	}

	ASSERT(pBar->m_pDockContext != NULL);
	return DYNAMIC_DOWNCAST(CXTControlBar, pBar);
}

BOOL CXTMiniDockFrameWnd::OnNcActivate(BOOL bActive)
{
	if ((GetStyle() & MFS_SYNCACTIVE) == 0)
	{
		// Turn WS_VISIBLE off before calling DefWindowProc,
		// so DefWindowProc won't paint and thereby cause flicker.
		DWORD dwStyle = GetStyle();
		if (dwStyle & WS_VISIBLE)
		{
			::SetWindowLong(m_hWnd, GWL_STYLE, (dwStyle & ~ WS_VISIBLE));
		}
		
		// do not call the base class because it will call Default()
		// and we may have changed bActive.
		::DefWindowProc(m_hWnd, WM_NCACTIVATE, bActive, 0L);
		
		if (dwStyle & WS_VISIBLE)
		{
			::SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
		}
		
		// At this point, nothing has happened (since WS_VISIBLE was off).
		// Now it's time to paint.
		
		// update state
		m_bActive = bActive;
		
		// paint non-client area (frame too)
		::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
		
		return TRUE;
	}
	else if(m_nFlags & WF_KEEPMINIACTIVE)
	{
		return FALSE;
	}
	return TRUE;
}

CXTMiniDockFrameWnd::APPEARANCE_STYLE CXTMiniDockFrameWnd::GetCurrentAppearance()
{
    return (xtAfxData.bXPMode) ? APPEARANCE_XPSTYLE : APPEARANCE_CLASSIC;
}

void CXTMiniDockFrameWnd::OnIdleUpdateCmdUI()
{
    CMiniDockFrameWnd::OnIdleUpdateCmdUI();

    // check to see if the frame has to change because the style has been switched
    if (m_currentAppearance != APPEARANCE_UNDEFINED)
    {
        if (m_currentAppearance != GetCurrentAppearance())
        {
            OnInitMiniFrame(0, 0);
        }
    }

	// check to see if the cursor is outside of the window area and refresh any
	// frame buttons that need to be repainted.
	{
		CRect r;
		GetWindowRect(&r);

		CPoint pt;
		GetCursorPos(&pt);

		if (!r.PtInRect(pt))
		{
			bool bNCPaint = false;
			
			POSITION pos;
			for (pos = m_arrButtons.GetHeadPosition(); pos != NULL; m_arrButtons.GetNext(pos))
			{
				CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetAt(pos);
				ASSERT(pFrameBtn != NULL);
				
				r = pFrameBtn->GetButtonRect();
				if (!r.PtInRect(pt) && pFrameBtn->IsPainted() && !m_pPopupWnd)
				{
					pFrameBtn->SetNormal();
					bNCPaint = true;
				}
			}		

			if (bNCPaint)
			{
				::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
			}
		}
	}
}

void CXTMiniDockFrameWnd::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (nHitTest == HTCAPTION)
	{
        m_pPressedBtn = NULL;
		bool bNCPaint = false;
		
		POSITION pos;
		for (pos = m_arrButtons.GetHeadPosition(); pos != NULL; m_arrButtons.GetNext(pos))
		{
			CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetAt(pos);
			ASSERT(pFrameBtn != NULL);
			
			CRect r = pFrameBtn->GetButtonRect();
			if (r.PtInRect(point))
			{
				if (!pFrameBtn->IsPushed() || pFrameBtn->IsHot())
				{
					pFrameBtn->SetPushed(true);
					bNCPaint = true;
					m_pPressedBtn = pFrameBtn;
				}
			}
			else
			{
				if (!pFrameBtn->IsNormal())
				{
					pFrameBtn->SetNormal();
					bNCPaint = true;
				}
			}

			CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();	
			if (connection && connection->IsCustMode())
				return;
			
			if (pFrameBtn->GetButtonStyle() == DFCS_SCROLLDOWN)
			{
				HWND hWnd = m_pPopupWnd->GetSafeHwnd();
				if (pFrameBtn->IsNormal() && ::IsWindow(hWnd))
					::SendMessage(hWnd, WM_CLOSE, 0, 0);
			}
		}
		
		if (bNCPaint)
		{
			::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
		}
        
        if (m_pPressedBtn == NULL)
        {
            ActivateTopParent();
            if ((m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
            {
                CXTControlBar* pBar = GetXTControlBar();
				if (pBar)
				{
					pBar->SetFocus();
					CXTDockContext* pDockContext = GetXTDockContext(pBar);
					if (pDockContext)
					{
						pDockContext->StartDrag(point);
					}
				}
            }
        }
        return;
    }
	else if (nHitTest >= HTSIZEFIRST && nHitTest <= HTSIZELAST)
	{
		ActivateTopParent();

		CXTControlBar* pBar = GetXTControlBar();
		if (pBar && IsDockWindow(pBar))
		{
			CXTDockContext* pDockContext = GetXTDockContext(pBar);
			if (pDockContext)
			{
	    		pDockContext->StartResize(nHitTest, point);
			}
            return;
		}
	}

    CMiniDockFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CXTMiniDockFrameWnd::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if (nHitTest == HTCAPTION && (m_wndDockBar.m_dwStyle & CBRS_FLOAT_MULTI) == 0)
	{
		CXTControlBar* pBar = GetXTControlBar();
		if (pBar && IsControlBar(pBar))
		{
			bool bNCPaint = false;

			POSITION pos;
			for (pos = m_arrButtons.GetHeadPosition(); pos != NULL; m_arrButtons.GetNext(pos))
			{
				CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetAt(pos);
				ASSERT(pFrameBtn != NULL);

				CRect r = pFrameBtn->GetButtonRect();
                if (r.PtInRect(point))
                {
                    if (pFrameBtn->IsPushed() && !pFrameBtn->IsHidden())
                    {
                        switch (pFrameBtn->GetButtonStyle())
                        {
                        case DFCS_CAPTIONCLOSE:
                            {
                                ::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
								pFrameBtn->SetNormal();
								m_pPressedBtn = NULL;
                            }
                            break;
                            
                        case DFCS_SCROLLDOWN:
                            {
								CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();	
								if (connection && connection->IsCustMode())
									break;

                                if (m_pPopupWnd)
                                    break;
                                
                                // outline is drawn inside the rect so use adjusted right/bottom
                                // borders for proper align of the popup
                                CRect rc = r;
                                if (xtAfxData.bXPMode)
                                {
                                    rc.right  -= 3;
                                    rc.left   -= 2;
                                    rc.bottom -= 1;
                                }
                                
                                m_pPopupWnd = new CXTExpMenuWnd(this, XTWM_POPUPCLOSED);
                                if (!m_pPopupWnd->Create(rc, (CXTToolBar*)pBar))
                                {
                                    // PostNcDestroy is called automatically
                                    m_pPopupWnd = 0;
                                }

								pFrameBtn->SetHot(true);
								pFrameBtn->SetPushed(false);
								bNCPaint = true;
							}
                            break;
                        }
                    }
                }
			}

			if (bNCPaint)
			{
				::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
			}
		}
	}
	
	CMiniDockFrameWnd::OnNcLButtonUp(nHitTest, point);
}

void CXTMiniDockFrameWnd::ActivateToolTips(CPoint point, bool bIsCloseButton)
{
	// don't display if tooltips are not allowed...
	CXTControlBar* pBar = GetXTControlBar();
	if (!pBar || (pBar->GetBarStyle() & CBRS_TOOLTIPS) == 0) {
		return;
	}

	if (::IsWindow(m_toolTip.m_hWnd))
	{
		XT_MANAGE_STATE;  // Switch to toolkit resources/module state

		::SendMessage(m_toolTip.m_hWnd, TTM_POP, 0, 0);
		
		MSG msg;
		msg.hwnd    = m_hWnd;
		msg.message = WM_MOUSEMOVE;
		msg.wParam  = 0;
		msg.lParam  = MAKELPARAM(point.x,point.y);
		msg.time    = 0;
		msg.pt      = point;
		
		if (m_toolTip.GetToolCount())
		{
			m_toolTip.DelTool(this);
			m_toolTip.Activate(false);
		}

		// If the tip text differs, update the tooltip control.
		if (bIsCloseButton)
		{
			m_toolTip.AddTool(this, XT_IDS_CLOSE);
			m_toolTip.Activate(true);
		}
		else
		{
			m_toolTip.AddTool(this, XT_IDS_TOOLBAR_OPTS);
			m_toolTip.Activate(true);
		}
		
		// Pass on to tooltip.
		m_toolTip.RelayEvent(&msg);
	}
}

void CXTMiniDockFrameWnd::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	if (!m_pPopupWnd)
	{
		bool bPtInRect = false;
		bool bNCPaint  = false;
		bool bNCLBtnDown = (::GetKeyState(VK_LBUTTON) < 0);
		
		POSITION pos;
		for (pos = m_arrButtons.GetHeadPosition(); pos != NULL; m_arrButtons.GetNext(pos))
		{
			CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetAt(pos);
			ASSERT(pFrameBtn != NULL);
			
			CRect r = pFrameBtn->GetButtonRect();
			if (r.PtInRect(point))
			{
				bPtInRect = true;
				
				if (bNCLBtnDown && m_pPressedBtn != pFrameBtn && !pFrameBtn->IsNormal())
				{
					pFrameBtn->SetNormal();
					bNCPaint = true;
				}
				else
				if ((bNCLBtnDown == false && pFrameBtn->IsPushed() == true ) ||
					(bNCLBtnDown == true  && pFrameBtn->IsPushed() == false))
				{
					pFrameBtn->SetPushed(bNCLBtnDown);
					bNCPaint = true;
				}
				else
				if (pFrameBtn->IsHot() == false)
				{
					pFrameBtn->SetHot(true);
					bNCPaint = true;

					bool bIsCloseButton = (pFrameBtn->GetButtonStyle() == DFCS_CAPTIONCLOSE);
					ActivateToolTips(point, bIsCloseButton);
				}
			}
			
			else
			{
				if (!pFrameBtn->IsNormal())
				{
					bNCPaint = true;
				}
				
				pFrameBtn->SetNormal();
			}
		}
		
		if (bNCPaint)
		{
			::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
		}

		if (!bPtInRect && ::IsWindow(m_toolTip.m_hWnd))
		{
			::SendMessage(m_toolTip.m_hWnd, TTM_POP, 0, 0);
		}
	}
	
	CMiniDockFrameWnd::OnNcMouseMove(nHitTest, point);
}

void CXTMiniDockFrameWnd::OnPopupClosed()
{
    bool bNCPaint = false;
    
    POSITION pos;
	for (pos = m_arrButtons.GetHeadPosition(); pos != NULL; m_arrButtons.GetNext(pos))
    {
        CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetAt(pos);
        ASSERT(pFrameBtn != NULL);
        
        if (pFrameBtn->IsPainted())
        {
            bNCPaint = true;
            pFrameBtn->SetNormal();
        }
    }		
    
    if (bNCPaint)
    {
        ::SendMessage(m_hWnd, WM_NCPAINT, 0, 0);
    }
    
    m_pPopupWnd = NULL;
	m_pPressedBtn = NULL;
}

void CXTMiniDockFrameWnd::OnCustomize() 
{
	POSITION pos;
	for (pos = m_arrButtons.GetHeadPosition(); pos != NULL; m_arrButtons.GetNext(pos))
	{
		CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetAt(pos);
		ASSERT(pFrameBtn != NULL);

		if (pFrameBtn->GetButtonStyle() == DFCS_SCROLLDOWN)
		{
			pFrameBtn->SetNormal();
			pFrameBtn->GetToolBar()->PostMessage(WM_COMMAND, XT_IDC_CUSTOMIZE);
			break;
		}
	}
}

LRESULT CXTMiniDockFrameWnd::OnSetText(WPARAM, LPARAM)
{
	// do default processing
	LRESULT ret = Default();

	// forcibly redraw the frame.
	// miniframe does something weird if you don't
	SetWindowPos(0,0,0,0,0,
		SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOMOVE | SWP_DRAWFRAME);

	return ret;
}

void CXTMiniDockFrameWnd::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType /*= adjustBorder*/)
{
	// compute min width of this frame
	int cx = xtAfxData.cxSmIcon;
	if (POSITION pos = m_arrButtons.GetHeadPosition())
	{
		CRect rc;
		GetWindowRect(rc);
		ScreenToClient(rc);
		rc.OffsetRect(-rc.left, -rc.top);
		do
		{
			CXTFrameButton* pFrameBtn = (CXTFrameButton*)m_arrButtons.GetNext(pos);			
			CRect r = pFrameBtn->GetButtonRect(&rc);
			cx += r.Width();
		}
		while(pos);
	}
	if (lpClientRect->right - lpClientRect->left < cx)
	{
		lpClientRect->right = lpClientRect->left + cx;
	}
	CMiniDockFrameWnd::CalcWindowRect(lpClientRect, nAdjustType);
}

BOOL CXTMiniDockFrameWnd::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;
	GetClientRect(&rect);
	CXTControlBar* pBar = GetXTControlBar();
	if (pBar)
	{
		pDC->FillSolidRect(rect, pBar->GetBackgroundColor());
		return true;
	}

	return CMiniDockFrameWnd::OnEraseBkgnd(pDC) ;
}

void CXTMiniDockFrameWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	if (!m_bInitCompleted)
	{
		// force update
		SendMessage(XTWM_INITIAL_UPDATE);
	}
	CMiniDockFrameWnd::OnShowWindow(bShow, nStatus);
}
