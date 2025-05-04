// XTSplitterDock.cpp : implementation of the CXTSplitterDock class.
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
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTDockWindow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

bool CXTSplitterDock::m_bDisableTracking = false;

//////////////////////////////////////////////////////////////////////
// CXTSplitterDock Class
//////////////////////////////////////////////////////////////////////
CXTSplitterDock::CXTSplitterDock(CXTDockBar *pBar, const CRect &rcCurrent,const int nType, const int nPos)
{
	ASSERT(pBar->IsKindOf(RUNTIME_CLASS(CXTDockBar)));
	m_pDockBar = pBar;
	m_rcCurrent = rcCurrent;
	m_nType = nType;
	m_nControlBar = nPos;
}

CXTSplitterDock::~CXTSplitterDock()
{
}

void CXTSplitterDock::Draw(CDC *pDC)
{
	// Repaint the background...
	pDC->FillSolidRect(m_rcCurrent, xtAfxData.clr3DFace);

	int x  = m_rcCurrent.left;
	int y  = m_rcCurrent.top;
	int cx = m_rcCurrent.Width();
	int cy = m_rcCurrent.Height();

	// and draw the splitter.
	if (m_nType == XT_SPLITTER_VERT)
	{
		// If the borders are not flat...
		if ((m_pDockBar->GetXTBarStyle() & CBRS_XT_BORDERS_FLAT) == 0 && !xtAfxData.bXPMode)
		{
			// Draw the top and bottom edges.
			pDC->FillSolidRect(x, y,      cx-1, 1, xtAfxData.clr3DHilight);
			pDC->FillSolidRect(x, y+cy-1, cx-1, 1, xtAfxData.clr3DShadow);

			pDC->FillSolidRect(x,      y, 1, cy, xtAfxData.clr3DHilight);
			pDC->FillSolidRect(x+cx-2, y, 1, cy, xtAfxData.clr3DShadow);
			
			// If the semi flat style is not used, draw thick devstudio style borders.
			if ((m_pDockBar->GetXTBarStyle() & CBRS_XT_SEMIFLAT) == 0) {
				pDC->FillSolidRect(x+cx-1, y, 1, cy, xtAfxData.clrWindowFrame);
			}
		}
	}
	else
	{
		// If the borders are not flat...
		if ((m_pDockBar->GetXTBarStyle() & CBRS_XT_BORDERS_FLAT) == 0 && !xtAfxData.bXPMode)
		{
			// Draw the left and right edges.
			pDC->FillSolidRect(x,      y, 1, cy-1, xtAfxData.clr3DHilight);
			pDC->FillSolidRect(x+cx-1, y, 1, cy-1, xtAfxData.clr3DShadow);
			
			pDC->FillSolidRect(x, y,      cx-1, 1, xtAfxData.clr3DHilight);
			pDC->FillSolidRect(x, y+cy-2, cx-1, 1, xtAfxData.clr3DShadow);
			
			// If the semi flat style is not used, draw thick devstudio style borders.
			if ((m_pDockBar->GetXTBarStyle() & CBRS_XT_SEMIFLAT) == 0) {
				pDC->FillSolidRect(x, y+cy-1, cx, 1, xtAfxData.clrWindowFrame);
			}
		}
	}
}

bool CXTSplitterDock::OnInvertTracker()
{
	if (m_bDisableTracking)
		return false;

	CFrameWnd* pFrameWnd = m_pDockBar->GetDockingFrame();
	ASSERT_VALID(pFrameWnd);
    
    if (!pFrameWnd)
        return false;

    if (m_rcTrack.IsRectEmpty())
    {
        CWnd* pParentWnd = m_pDockBar->GetParent();
        ASSERT_VALID(pParentWnd);

        pParentWnd->Invalidate();
        return false;
    }

    CDC* pDC = pFrameWnd->GetDC();

	// if we have flat borders, set the tracker to solid black,
	// otherwise use a halftone brush...
	CBrush* pBrush = NULL;

	if ((m_pDockBar->GetXTBarStyle() & CBRS_XT_BORDERS_FLAT) || xtAfxData.bXPMode) {
		pBrush = CBrush::FromHandle((HBRUSH)::GetStockObject(WHITE_BRUSH));
	}
	else {
		pBrush = CDC::GetHalftoneBrush();
	}

	CBrush* pOldBrush = pDC->SelectObject(pBrush);
	
	// deflate the width of the tracker by 1 pixel for a better look.
	CRect rect(m_rcTrack);
	if(rect.Width() < rect.Height()) {
		rect.DeflateRect(1,0);
	}
    else {
		rect.DeflateRect(0,1);
	}
	
	// draw the tracker, restore the device context and free GDI resources...
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
	pDC->SelectObject(pOldBrush);
	pFrameWnd->ReleaseDC(pDC);

    return true;
}

void CXTSplitterDock::SetLength(int nLength)
{
    if (m_nType == XT_SPLITTER_VERT)
        m_rcCurrent.bottom = nLength;
    else
        m_rcCurrent.right = nLength;
}

bool CXTSplitterDock::StartTrack(CPoint point)
{
	if (m_bDisableTracking)
		return false;

	m_rcTrack = m_rcCurrent;
	// transform current rect from DockBar client coordinates to FrameWnd ones
	m_pDockBar->ClientToScreen(&m_rcTrack);
	m_pDockBar->GetDockingFrame()->ScreenToClient(&m_rcTrack);
	// point to client coordinates
	m_pDockBar->ScreenToClient(&point);
	m_ptCurrent = point;
	// calculate initial and limits widths
	CalcMaxMinHeight();
	
    return OnInvertTracker();
}

bool CXTSplitterDock::Move(CPoint point)
{
	if (!OnInvertTracker())
        return false;

	CSize sizeOffset(0,0);
	switch (m_pDockBar->GetDlgCtrlID())
	{
	case AFX_IDW_DOCKBAR_TOP:
		sizeOffset.cy = point.y - m_ptCurrent.y;
		sizeOffset.cy = __min(sizeOffset.cy, m_nMaxHeight - m_nCurrentHeight);
		sizeOffset.cy = __max(sizeOffset.cy, m_nMinHeight - m_nCurrentHeight);
		m_nCurrentHeight += sizeOffset.cy;
		break;
	case AFX_IDW_DOCKBAR_BOTTOM:
		sizeOffset.cy = point.y - m_ptCurrent.y;
		sizeOffset.cy = -__min(-sizeOffset.cy, m_nMaxHeight - m_nCurrentHeight);
		sizeOffset.cy = -__max(-sizeOffset.cy, m_nMinHeight - m_nCurrentHeight);
		m_nCurrentHeight -= sizeOffset.cy;
		break;
	case AFX_IDW_DOCKBAR_LEFT:
		sizeOffset.cx = point.x - m_ptCurrent.x;
		sizeOffset.cx = __min(sizeOffset.cx, m_nMaxHeight - m_nCurrentHeight);
		sizeOffset.cx = __max(sizeOffset.cx, m_nMinHeight - m_nCurrentHeight);
		m_nCurrentHeight += sizeOffset.cx;
		break;
	case AFX_IDW_DOCKBAR_RIGHT:
		sizeOffset.cx = point.x - m_ptCurrent.x;
		sizeOffset.cx = -__min(-sizeOffset.cx, m_nMaxHeight - m_nCurrentHeight);
		sizeOffset.cx = -__max(-sizeOffset.cx, m_nMinHeight - m_nCurrentHeight);
		m_nCurrentHeight -= sizeOffset.cx;
		break;
	}
	
    m_rcTrack.OffsetRect(sizeOffset);
	m_ptCurrent.Offset(sizeOffset);
	
    return OnInvertTracker();
}

void CXTSplitterDock::CalcMaxMinHeight()
{
	// current row height = max row Height
	m_nCurrentHeight = m_nStartHeight = m_pDockBar->GetRowMaxHeight(m_nControlBar);
	m_nMinHeight = m_pDockBar->GetRowMinHeight(m_nControlBar);
	m_nMaxHeight = m_pDockBar->GetAvailableHeight() + m_nCurrentHeight;
}

bool CXTSplitterDock::EndTrack(CPoint point)
{
	if (m_bDisableTracking)
		return false;

    // last movement
    if ( !Move( point ) )
    {
        return false;
    }

    // hide track bar
    if ( !OnInvertTracker( ) )
    {
        return false;
    }

    if ( m_nStartHeight != m_nCurrentHeight )
    {
        SetNewHeight( );
        
        CFrameWnd* pFrameWnd = m_pDockBar->GetDockingFrame( );
        ASSERT_VALID( pFrameWnd );

        // NB: note that simply recalcing the layout may destroy this pointer.
        pFrameWnd->DelayRecalcLayout( );
    }

    return true;
}

void CXTSplitterDock::SetNewHeight()
{
	m_pDockBar->SetRowHeight(m_nControlBar, m_nCurrentHeight);
	// if the row was maximized...
	CXTDockWindow *pMaxBar = m_pDockBar->IsRowMaximized(m_nControlBar);
	if (pMaxBar)
		// ...continue
		m_pDockBar->Maximize(pMaxBar, FALSE);
}
//////////////////////////////////////////////////////////////////////
// CSplitterVertDock Class
//////////////////////////////////////////////////////////////////////
CXTSplitterRowDock::CXTSplitterRowDock(CXTDockBar *pBar, const CRect &rcCurrent,const int nType, const int nPos)
	: CXTSplitterDock(pBar, rcCurrent,nType, nPos)
{
}

void CXTSplitterRowDock::SetNewHeight()
{
	int nIncHeight = m_nCurrentHeight - m_nStartHeight;
	m_pDockBar->StretchRowLeft(m_nControlBar, nIncHeight);
	m_pDockBar->StretchRowRight(m_nControlBar, -nIncHeight);
	// if the row is maximized...
	CXTDockWindow *pMaxBar = m_pDockBar->IsRowMaximized(m_nControlBar);
	if (pMaxBar)
		// ...normalize it
		m_pDockBar->Normalize(pMaxBar, FALSE);
}

bool CXTSplitterRowDock::Move(CPoint point)
{
	if (!OnInvertTracker())
        return false;

	CSize sizeOffset(0,0);
	if (IsHorz())
	{
		sizeOffset.cy = point.y - m_ptCurrent.y;
		sizeOffset.cy = __min(sizeOffset.cy, m_nMaxHeight - m_nCurrentHeight);
		sizeOffset.cy = __max(sizeOffset.cy, m_nMinHeight - m_nCurrentHeight);
		m_nCurrentHeight += sizeOffset.cy;
	}
	else
	{
		sizeOffset.cx = point.x - m_ptCurrent.x;
		sizeOffset.cx = __min(sizeOffset.cx, m_nMaxHeight - m_nCurrentHeight);
		sizeOffset.cx = __max(sizeOffset.cx, m_nMinHeight - m_nCurrentHeight);
		m_nCurrentHeight += sizeOffset.cx;
	}
	m_rcTrack.OffsetRect(sizeOffset);
	m_ptCurrent.Offset(sizeOffset);
	
    return OnInvertTracker();
}

void CXTSplitterRowDock::CalcMaxMinHeight()
{
	// current row height = max row Height
	// Here the Heights vars. refer to positions of the splitter
	m_nCurrentHeight = m_nStartHeight = IsHorz() ? m_rcCurrent.top : m_rcCurrent.left;
	CControlBar *pBar = m_pDockBar->GetDockedControlBar(m_nControlBar);
	int nFirstBar = m_pDockBar->GetFirstBarInRow(pBar);
	m_nMinHeight = m_pDockBar->GetRowFixedWidth(nFirstBar) - m_pDockBar->GetRowFixedWidth(m_nControlBar) + 
		m_pDockBar->GetRowMinSizedWidth(nFirstBar) - m_pDockBar->GetRowMinSizedWidth(m_nControlBar);
	m_nMaxHeight = m_nCurrentHeight + m_pDockBar->GetRowSizedWidth(m_nControlBar) - m_pDockBar->GetRowMinSizedWidth(m_nControlBar);
}

