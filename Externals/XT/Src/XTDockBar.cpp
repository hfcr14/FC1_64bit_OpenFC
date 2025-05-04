// XTDockBar.cpp : implementation of the CXTDockBar class.
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
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTDockWindow.h"
#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTCoolMenu.h"
#include "XTMemDC.h"
#include "XTReBar.h"
#include "XTPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const int XT_TRACK_BORDER_SIZE	= 6;
/////////////////////////////////////////////////////////////////////////////
// CXTDockBar
/////////////////////////////////////////////////////////////////////////////
CXTDockBar::CXTDockBar()
{
	m_bTracking		 = FALSE;
	m_nTrackSplitter = -1;
	m_dwXTStyle		 = CBRS_XT_NONFLAT;
}

CXTDockBar::~CXTDockBar()
{
	DeleteSplitters();
}

IMPLEMENT_DYNAMIC(CXTDockBar, CDockBar)

BEGIN_MESSAGE_MAP(CXTDockBar, CDockBar)
	//{{AFX_MSG_MAP(CXTDockBar)
	ON_WM_NCHITTEST()
	ON_WM_SETCURSOR()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_WM_NCPAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CXTDockWindow* CXTDockBar::GetSafeDockWindow(CControlBar* pBar)
{
	// if this is not a valid pointer, then return NULL...
	if (pBar == NULL) {
		return NULL;
	}

	// if this is not a valid window return NULL...
	HWND hWnd = pBar->GetSafeHwnd();
	if ((hWnd == NULL) || !::IsWindow(hWnd) || !pBar->IsVisible()) {
		return NULL;
	}

	// if the control bar is not a docking window, return NULL...
	if (CXTDockWindow::IsDockWindow(pBar) == FALSE) {
		return NULL;
	}

	// if all checks out, then return the control bar that was 
	// originally passed in as a CXTDockWindow object...
	return (CXTDockWindow*)pBar;
}

// tells if toolbar at a given position is the last in its row
bool CXTDockBar::IsLastBarInRow(int nPos)
{
    if (m_arrBars.GetSize())
    {
        if (nPos < m_arrBars.GetSize() - 1)	        
        {
            if (m_arrBars[nPos] && !m_arrBars[nPos + 1])
            {
                // a bar is followed by a separator,
                // last bar in the row
                return true;
            }
        }
    }
    return false;
}

// Safe test for number of docked control bars.  Derived
// from CDockBar::GetDockedVisibleCount, with an additional test
// for the window being created (IsVisible() ASSERT's if m_hWnd of
// the control bar is NULL)
int CXTDockBar::GetDockedVisibleCount() const
{
	int nCount = 0;
	for (int i = 0; i < m_arrBars.GetSize(); i++)
	{
		CControlBar* pBar = STATIC_DOWNCAST(CControlBar, (CObject*)GetDockedControlBar(i));
		if (pBar != NULL && pBar->GetSafeHwnd() && pBar->IsVisible())
			nCount++;
	}
	return nCount;
}

CSize CXTDockBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ASSERT_VALID(this);
	
	InvalidateRect(NULL, FALSE);
	
	CSize sizeFixed = CControlBar::CalcFixedLayout(bStretch, bHorz);
	BOOL bTopLeft = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_LEFT);
		// get max size
	CSize sizeMax;
	if (!m_rectLayout.IsRectEmpty())
    {
		sizeMax = m_rectLayout.Size();
    }
	else
	{
		CRect rectFrame;
		m_pDockSite->GetClientRect(&rectFrame);
		sizeMax = rectFrame.Size();
    }

	if (!bHorz)
	{
		// Kludge: for reason unknown, MFC reports layout rect taller than it actually should be
		// so bottom of the control bar gets obscured.
		// Below is the fix that seems to circumvent that.
		sizeMax.cy -= 2;
	}

	const int nWidthMax = bHorz ? sizeMax.cx : sizeMax.cy;
	const int NormalcxBorder2 = GetSystemMetrics(SM_CYBORDER)*2;
	const int NormalcyBorder2 = 2;
	const int AltcxBorder2 = 0;
	const int AltcyBorder2 = 0;
	int cxBorder2 = 0;
	int cyBorder2 = 0;
	
	// prepare for layout
	AFX_SIZEPARENTPARAMS layout;
	layout.hDWP = m_bLayoutQuery ? NULL : ::BeginDeferWindowPos(m_arrBars.GetSize());
	CPoint pt(0,-1);
	if (m_dwStyle & CBRS_ALIGN_RIGHT)
		pt.x = -1;
	int nWidth = 0;
	
	BOOL bWrapped = FALSE;
	
	// destroy splitters before create new ones
	DeleteSplitters();
	
	// variables used for size bars
	BOOL bSplitterBetweenRows = FALSE;
	BOOL bFirstRow			  = TRUE;
	BOOL bFirstBarInRow		  = TRUE;
	int nFirstBarInRow		  = -1;
	
	BOOL bBarSizeable		= FALSE;
	BOOL bPrevBarSizeable	= FALSE;
	CControlBar *pPrevBar	= NULL;
	DWORD dwBarStyle		= CBRS_BORDER_ANY;
	DWORD dwPrevBarStyle	= CBRS_BORDER_ANY;
	DWORD dwBarInitStyle	= CBRS_BORDER_ANY;

	int nSizeBarsInRow		= 0;
	int nSizeBarsRight		= 0;
	int nFirstSplitterInRow = 0;

	// layout all the control bars
	int nPos;
	for (nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			if ( (pBar != NULL) && pBar->GetSafeHwnd() )
			{
				if (pBar->IsVisible())
				{
					// Is Bar sizeable?
					bBarSizeable = CXTDockWindow::IsDockWindow(pBar);

					// initial bar style:
					dwBarStyle = pBar->GetBarStyle() & ~CBRS_BORDER_ANY;
					
					if (bFirstBarInRow)
						// New Row
					{
						nSizeBarsInRow = nSizeBarsRight = GetRowSizeBars(nPos);
						
						// in bottom or right DockBars the Spliter must be added in 
						// function of size bars of this row, not previous
						if (!bTopLeft)
							bSplitterBetweenRows = (nSizeBarsInRow > 0) ? TRUE : FALSE;

						if (bSplitterBetweenRows)
						{
							// add a Splitter between this row
							if (bHorz)
							{
								// horz bar: adds a horz Spliter
								AddSplitter(CRect(pt.x, pt.y, 0, pt.y + XT_TRACK_BORDER_SIZE), XT_SPLITTER_HORZ, TRUE, bTopLeft ? nFirstBarInRow : nPos);
								pt.y += XT_TRACK_BORDER_SIZE;
								if (nFirstBarInRow != -1)
									RemoveRowBorders(nFirstBarInRow, CBRS_BORDER_BOTTOM);
								dwBarInitStyle &= ~CBRS_BORDER_TOP;
							}
							else
							{
								// vert bar: adds a vert Splitter
								AddSplitter(CRect(pt.x, pt.y, pt.x + XT_TRACK_BORDER_SIZE, 0), XT_SPLITTER_VERT, TRUE, bTopLeft ? nFirstBarInRow : nPos);
								pt.x += XT_TRACK_BORDER_SIZE + 1;
								if (nFirstBarInRow != -1)
									RemoveRowBorders(nFirstBarInRow, CBRS_BORDER_RIGHT);
								dwBarInitStyle &= ~CBRS_BORDER_LEFT;
							}
						}
	
						// if any changes inside the row ...
						if (nWidthMax != GetRowFixedWidth(nPos) + GetRowSizedWidth(nPos) ||
								FindNewBar(nPos) != NULL)
							AdjustRowSizes(nPos); //... adjust row size

						// initial border style
						dwBarStyle |= dwBarInitStyle;
						
						// remove border of first bar
						if (pBar->IsKindOf(RUNTIME_CLASS(CXTDockWindow)))
						{
							// only in horz ones.
							if (bHorz)
								dwBarStyle &= ~CBRS_BORDER_LEFT;	// left on horz bar
						}
						
						// used for last bar
						nFirstBarInRow = nPos;
						// next row isn't the first in row
						bFirstBarInRow = FALSE;
					} 

					else
					{
						if(CXTMenuBar::IsMenuBar(pBar))
						{
							// add a NULL (row break)
							m_arrBars.InsertAt(nFirstBarInRow, (CObject*)NULL);
							// move menubar to the first position of the row
							m_arrBars.InsertAt(nFirstBarInRow, (CObject*)pBar);
							m_arrBars.RemoveAt(nPos+2);
							m_pDockSite->DelayRecalcLayout();
							break;
						}

						// initial border style
						dwBarStyle |= dwBarInitStyle;
						// check if a Splitter inside row is needed
						if ((bPrevBarSizeable && nSizeBarsRight > 0) || 
							(bBarSizeable && (nSizeBarsInRow > nSizeBarsRight)))
						{
							// adds a splitter inside a row
							// add a Splitter between this row
							if (bHorz)
							{
								// horz bar: adds a vert Spliter
								AddSplitter(CRect(pt.x, pt.y, pt.x + XT_TRACK_BORDER_SIZE, 0), XT_SPLITTER_VERT, FALSE, nPos);
								pt.x += XT_TRACK_BORDER_SIZE;
								
								// remove borders
								dwBarStyle &= ~CBRS_BORDER_LEFT;	// left on current bar
								dwPrevBarStyle &= ~CBRS_BORDER_RIGHT;	// right on previous one
							}
							else
							{
								// vert bar: adds a horz Splitter
								AddSplitter(CRect(pt.x, pt.y, 0, pt.y + XT_TRACK_BORDER_SIZE), XT_SPLITTER_HORZ, FALSE, nPos);
								pt.y += XT_TRACK_BORDER_SIZE;
								// remove borders
								dwBarStyle &= ~CBRS_BORDER_TOP;	// top on current bar
								dwPrevBarStyle &= ~CBRS_BORDER_BOTTOM;	// bottom on previous one
							}
						}
					} // if (bFirstBarInRow)

					bPrevBarSizeable = bBarSizeable;
					if (pPrevBar != NULL && (CXTDockWindow::IsDockWindow(pPrevBar) || !(GetXTBarStyle() & CBRS_XT_FLAT)))
						pPrevBar->SetBarStyle(dwPrevBarStyle); //set bar style
					pPrevBar = pBar;

					// end code needed to add splitters
					/////////////////////////////////////
					
					if (bBarSizeable)
					{
						// one lest on the right
						nSizeBarsRight--;
						if (nSizeBarsInRow == 1)
							((CXTDockWindow *) pBar)->m_bp.bUnique = TRUE;
						else
							((CXTDockWindow *) pBar)->m_bp.bUnique = FALSE;
					}
					
					if(pBar->IsKindOf(RUNTIME_CLASS(CXTControlBar)))
					{
						cxBorder2=AltcxBorder2;
						cyBorder2=AltcyBorder2;
					}
					else
					{
						cxBorder2=NormalcxBorder2;
						cyBorder2=NormalcyBorder2;
					}
					
					// get ideal rect for bar
					CSize sizeBar;
					if (!bBarSizeable)
					{
						DWORD dwMode = 0;
						if ((pBar->m_dwStyle & CBRS_SIZE_DYNAMIC) &&
							(pBar->m_dwStyle & CBRS_FLOATING))
							dwMode |= LM_HORZ | LM_MRUWIDTH;
						else if (pBar->m_dwStyle & CBRS_ORIENT_HORZ)
							dwMode |= LM_HORZ | LM_HORZDOCK;
						else
							dwMode |=  LM_VERTDOCK;
						
						sizeBar = pBar->CalcDynamicLayout(-1, dwMode);
					}
					else
					{
						sizeBar.cx = ((CXTDockWindow *) pBar)->GetVertSize();
						sizeBar.cy = ((CXTDockWindow *) pBar)->GetHorzSize();
					}
					
					CRect rect(pt, sizeBar);
					
					// get current rect for bar
					CRect rectBar;
					pBar->GetWindowRect(&rectBar);
					ScreenToClient(&rectBar);
					
					if (bHorz)
					{
						// Offset Calculated Rect out to Actual
						if (rectBar.left > rect.left && !m_bFloating 
							// added to not allow offsets when there are size bars
							&& nSizeBarsInRow == 0)
						{
							rect.OffsetRect(rectBar.left - rect.left, 0);
							// add left border
							dwBarStyle |= CBRS_BORDER_LEFT;
						}
						
						// If ControlBar goes off the right, then right justify
						if (rect.right > sizeMax.cx && !m_bFloating)
						{
							int x = rect.Width() - cxBorder2;
							x = __max(sizeMax.cx - x, pt.x);
							rect.OffsetRect(x - rect.left, 0);
						}
						
						// If ControlBar has been wrapped, then left justify
						if (bWrapped)
						{
							bWrapped = FALSE;
							rect.OffsetRect(-(rect.left + cxBorder2), 0);
						}
						
						// If ControlBar is completely invisible, then wrap it
						if ((rect.left >= (sizeMax.cx - cxBorder2)) &&
							(nPos > 0) && (m_arrBars[nPos - 1] != NULL))
						{
							m_arrBars.InsertAt(nPos, (CObject*)NULL);
							pBar = NULL; pVoid = NULL;
							bWrapped = TRUE;
							m_pDockSite->DelayRecalcLayout();
						}
						
						if (!bWrapped)
						{
                            // check to see if this one is the last toolbar in the row.
                            // if so, expand its width to whatever is available in the client area
                            // and tell it to show/hide its buttons to fit the width
                            if (CXTToolBar* toolbar = DYNAMIC_DOWNCAST(CXTToolBar, pBar))
                            {
								if (toolbar->IsExpansionButton())
								{
									if (IsLastBarInRow(nPos))
									{
										if (rect.right > sizeMax.cx)
										{
											// compact toolbar horizontally
											rect.right = rect.left +
												toolbar->CompactButtons(rect.Width(), sizeMax.cx - rect.left, true);
										}
										else
										if (rect.right <= sizeMax.cx)
										{
											// expand toolbar horizontally
											rect.right = rect.left +
												toolbar->ExpandButtons(rect.Width(), sizeMax.cx - rect.left, true);
										}
									}
									else
									{
										// reset any hidden buttons
										rect.right = rect.left + 
												toolbar->ExpandButtons(rect.Width(), 32767, true);
									}
								}
                            }
						    if (rect != rectBar)
							{
								if (!m_bLayoutQuery &&
									!(pBar->m_dwStyle & CBRS_FLOATING))
								{
									pBar->m_pDockContext->m_rectMRUDockPos = rect;
								}
								AfxRepositionWindow(&layout, pBar->m_hWnd, &rect);
							}
							pt.x = rect.left + sizeBar.cx - cxBorder2;
							nWidth = __max(nWidth, sizeBar.cy);
						}
					}
					
					else
					{
						// Offset Calculated Rect out to Actual
						if (rectBar.top > rect.top && !m_bFloating						
							// added to not allow offsets when there are size bars
							&& nSizeBarsInRow == 0)
						{
							rect.OffsetRect(0, rectBar.top - rect.top);
							// add top border
							dwBarStyle |= CBRS_BORDER_TOP;
						}
						
						// If ControlBar goes off the bottom, then bottom justify
						if (rect.bottom > sizeMax.cy && !m_bFloating)
						{
							int y = rect.Height() - cyBorder2;
							y = __max(sizeMax.cy - y, pt.y);
							rect.OffsetRect(0, y - rect.top);
						}
						
						// If ControlBar has been wrapped, then top justify
						if (bWrapped)
						{
							bWrapped = FALSE;
							rect.OffsetRect(0, -(rect.top + cyBorder2));
						}
						
						// If ControlBar is completely invisible, then wrap it
						else if ((rect.top >= (sizeMax.cy - cyBorder2)) &&
							(nPos > 0) && (m_arrBars[nPos - 1] != NULL))
						{
							m_arrBars.InsertAt(nPos, (CObject*)NULL);
							pBar = NULL; pVoid = NULL;
							bWrapped = TRUE;
							m_pDockSite->DelayRecalcLayout();
						}
						
						if (!bWrapped)
						{
                            // check to see if this one is the last toolbar in the column.
                            // if so, expand its height to whatever is available in the client area
                            // and tell it to show/hide its buttons to fit the height
                            if (CXTToolBar* toolbar = DYNAMIC_DOWNCAST(CXTToolBar, pBar))
                            {
                                if (toolbar->IsExpansionButton())
                                {
									if (IsLastBarInRow(nPos))
									{
										if (rect.bottom > sizeMax.cy)
										{
											// compact toolbar horizontally
											rect.bottom = rect.top +
												toolbar->CompactButtons(rect.Height(), sizeMax.cy - rect.top, false);
										}
										else
										if (rect.bottom <= sizeMax.cy)
										{
											// expand toolbar horizontally
											rect.bottom = rect.top + 
												toolbar->ExpandButtons(rect.Height(), sizeMax.cy - rect.top, false);
										}
									}
									else
									{
										rect.bottom = rect.top + 
												toolbar->ExpandButtons(rect.Height(), 32767, false);
									}
                                }
                            }
							if (rect != rectBar)
							{
								if (!m_bLayoutQuery &&
									!(pBar->m_dwStyle & CBRS_FLOATING))
								{
									pBar->m_pDockContext->m_rectMRUDockPos = rect;
								}
								AfxRepositionWindow(&layout, pBar->m_hWnd, &rect);
							}
							pt.y = rect.top + sizeBar.cy - cyBorder2;
							nWidth = __max(nWidth, sizeBar.cx);
						}
					} // if (bHorz)
				} // if (pBar->IsVisible())

				if (!bWrapped)
				{
					// handle any delay/show hide for the bar
					pBar->RecalcDelayShow(&layout);
				}
				dwPrevBarStyle = dwBarStyle;
			}  // if (pBar != NULL)

		} // if (pVoid != NULL)

		else if (nWidth != 0)
		{
			// end of row because pBar == NULL
			if (bHorz)
			{
				// remove right border of last bar
				if (pt.x >= sizeMax.cx && pPrevBar->IsKindOf(RUNTIME_CLASS(CXTDockWindow)))
					dwPrevBarStyle &= ~CBRS_BORDER_RIGHT;

				if (pPrevBar != NULL && (CXTDockWindow::IsDockWindow(pPrevBar) || !(GetXTBarStyle() & CBRS_XT_FLAT)))
					//set bar style
					pPrevBar->SetBarStyle(dwPrevBarStyle);
				
				// remove borders of first row
				if (bFirstRow)
					RemoveRowBorders(nFirstBarInRow, CBRS_BORDER_TOP, TRUE);
				
				pt.y += nWidth - cyBorder2;
				sizeFixed.cx = __max(sizeFixed.cx, pt.x);
				sizeFixed.cy = __max(sizeFixed.cy, pt.y);
				pt.x = -cxBorder2;
				
				// adjust length of splitters inside row (vert ones)
				SetSplittersLength(nFirstSplitterInRow, XT_SPLITTER_VERT, pt.y);
			}

			else
			{
				BOOL bBtmBorder = (m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_BOTTOM) == NULL);

				// remove the bottom border of the last bar
				if (nSizeBarsInRow != 0 && bBtmBorder)
					dwPrevBarStyle &= ~CBRS_BORDER_BOTTOM;

				if (pPrevBar != NULL && (CXTDockWindow::IsDockWindow(pPrevBar) || !(GetXTBarStyle() & CBRS_XT_FLAT)))
					pPrevBar->SetBarStyle(dwPrevBarStyle); //set bar style
				
				if (bFirstRow)
					RemoveRowBorders(nFirstBarInRow, CBRS_BORDER_LEFT, TRUE);

				pt.x += nWidth - cxBorder2;
				sizeFixed.cx = __max(sizeFixed.cx, pt.x);
				sizeFixed.cy = __max(sizeFixed.cy, pt.y);
				pt.y = -cyBorder2 - 1;

				// adjust length of splitters inside row (horz ones)
				SetSplittersLength(nFirstSplitterInRow, XT_SPLITTER_HORZ, pt.x);
			}

			nWidth = 0;
			bFirstRow = FALSE;
			
			dwBarInitStyle = CBRS_BORDER_ANY;
			
			// the next bar is the first in the row
			bFirstBarInRow = TRUE;
			pPrevBar = NULL;
			
			// check if must add a splitter between rows
			bSplitterBetweenRows = nSizeBarsInRow > 0 ? TRUE : FALSE;
			nFirstSplitterInRow = m_arrSplitters.GetSize();
		} // if (pBar == NULL && pVoid == NULL && nWidth != 0)
	} // for ( nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	
	if (nFirstBarInRow != -1)
	{
		if(nSizeBarsInRow > 0)
		{
			// adds the last splitter (only in top or left dock bars)
			if (bTopLeft)
			{
				// add a Splitter between this row
				if (bHorz)
				{
					// horz bar: adds a horz Spliter
					AddSplitter(CRect(pt.x, pt.y, 0, pt.y + XT_TRACK_BORDER_SIZE), XT_SPLITTER_HORZ, TRUE, nFirstBarInRow);
					sizeFixed.cy += XT_TRACK_BORDER_SIZE - 1;
					RemoveRowBorders(nFirstBarInRow, CBRS_BORDER_BOTTOM);
				}
				else
				{
					// vert bar: adds a vert Splitter
					AddSplitter(CRect(pt.x, pt.y, pt.x + XT_TRACK_BORDER_SIZE, 0), XT_SPLITTER_VERT, TRUE, nFirstBarInRow);
					sizeFixed.cx += XT_TRACK_BORDER_SIZE - 1;
					RemoveRowBorders(nFirstBarInRow, CBRS_BORDER_RIGHT);
				}
			}
			else
			{
				// remove last bar borders
				if (bHorz)
					RemoveRowBorders(nFirstBarInRow, CBRS_BORDER_BOTTOM, TRUE);
				else
					RemoveRowBorders(nFirstBarInRow, CBRS_BORDER_RIGHT, TRUE);
			}
		}
		if (bHorz)
			sizeFixed.cy ++;
		else if (bTopLeft)
			sizeFixed.cx --;
	}
	if (!m_bLayoutQuery)
	{
		// move and resize all the windows at once!
		if (layout.hDWP == NULL || !::EndDeferWindowPos(layout.hDWP))
			TRACE0("Warning: DeferWindowPos failed - low system resources.\n");
	}
	
	// adjust the length of inter-rows Splitters
	if (bHorz)
		SetSplittersLength(0, XT_SPLITTER_HORZ, sizeMax.cx);
	else
		SetSplittersLength(0, XT_SPLITTER_VERT, sizeMax.cy);

	return sizeFixed;
}

int CXTDockBar::InsertEx(CControlBar* pBarIns, CRect rect, CPoint ptMid)
{
	ASSERT_VALID(this);
	ASSERT(pBarIns != NULL);

	int nPos = 0;
	int nPosInsAfter = 0;
	int nWidth = 0;
	int nTotalWidth = 0;
	BOOL bHorz = m_dwStyle & CBRS_ORIENT_HORZ;

	for (nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		if (pBar != NULL && pBar->IsVisible())
		{
			CRect rectBar;
			pBar->GetWindowRect(&rectBar);
			ScreenToClient(&rectBar);
			nWidth = XTMAX(nWidth,
				bHorz ? rectBar.Size().cy : rectBar.Size().cx - 1);
			if (bHorz ? rect.left > rectBar.left : rect.top > rectBar.top)
				nPosInsAfter = nPos;
		}
		else // end of row because pBar == NULL
		{
            // MFCBUG: update the cumulative row thickness only when a new row 
			// is detected this code also handles invisible toolbars and cases 
			// of end-of-row for blank rows
            if (pBar == NULL && (nPos == 0 || nWidth > 0))
            {
                nTotalWidth += nWidth - afxData.cyBorder2;
                nWidth = 0;
            }
			if ((bHorz ? ptMid.y : ptMid.x) < nTotalWidth)
			{
				if (nPos == 0) // first section
					m_arrBars.InsertAt(nPosInsAfter+1, (CObject*)NULL);
				m_arrBars.InsertAt(nPosInsAfter+1, pBarIns);
				return nPosInsAfter+1;
			}
            // MFCBUG: if pBar is NULL update nPosInsAfter.
            if (pBar == NULL)
			{
                nPosInsAfter = nPos;
			}
		}
	}

	// create a new row
	m_arrBars.InsertAt(nPosInsAfter+1, (CObject*)NULL);
	m_arrBars.InsertAt(nPosInsAfter+1, pBarIns);

	return nPosInsAfter+1;
}

void CXTDockBar::DockControlBar(CControlBar* pBar, LPCRECT lpRect)
{
	if (CXTDockWindow::IsDockWindow(pBar))
	{
		((CXTDockWindow *)pBar)->ModifyXTBarStyle(0, CBRS_XT_NEWDOCKED, FALSE);
	}

	ASSERT_VALID(this);
	ASSERT_VALID(pBar);
	ASSERT_KINDOF(CControlBar, pBar);

	CRect rectBar;
	pBar->GetWindowRect(&rectBar);
	if (pBar->m_pDockBar == this && (lpRect == NULL || rectBar == *lpRect))
	{
		// already docked and no change in position
		return;
	}

	// set CBRS_FLOAT_MULTI style if docking bar has it
	if (m_bFloating && (pBar->m_dwDockStyle & CBRS_FLOAT_MULTI))
		m_dwStyle |= CBRS_FLOAT_MULTI;

	m_dwStyle &= ~(CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);
	m_dwStyle |= pBar->m_dwStyle & (CBRS_SIZE_FIXED | CBRS_SIZE_DYNAMIC);

	if (!(m_dwStyle & CBRS_FLOAT_MULTI))
	{
		TCHAR szTitle[_MAX_PATH];
		pBar->GetWindowText(szTitle, _countof(szTitle));
		AfxSetWindowText(m_hWnd, szTitle);
	}

	// align correctly and turn on all borders
	DWORD dwStyle = pBar->GetBarStyle();
	dwStyle &= ~(CBRS_ALIGN_ANY);
	dwStyle |=  (m_dwStyle & CBRS_ALIGN_ANY) | CBRS_BORDER_ANY;

	if (m_bFloating)
		dwStyle |= CBRS_FLOATING;
	else
		dwStyle &= ~CBRS_FLOATING;

	pBar->SetBarStyle(dwStyle);

	// hide first if changing to a new docking site to avoid flashing
	BOOL bShow = FALSE;
	if (pBar->m_pDockBar != this && pBar->IsWindowVisible())
	{
		pBar->SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_HIDEWINDOW);
		bShow = TRUE;
	}

	int nPos = -1;
	if (lpRect != NULL)
	{
		// insert into appropriate row
		CRect rect(lpRect);
		ScreenToClient(&rect);
		CPoint ptMid(rect.left + rect.Width()/2, rect.top + rect.Height()/2);
		nPos = InsertEx(pBar, rect, ptMid);

		// position at requested position
		pBar->SetWindowPos(NULL, rect.left, rect.top, rect.Width(),
			rect.Height(), SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS);
	}
	else
	{
		// always add on current row, then create new one
		m_arrBars.Add(pBar);
		m_arrBars.Add(NULL);

		// align off the edge initially
		pBar->SetWindowPos(NULL, -afxData.cxBorder2, -afxData.cyBorder2, 0, 0,
			SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_NOCOPYBITS);
	}

	// attach it to the docking site
	if (pBar->GetParent() != this)
		pBar->SetParent(this);
	if (pBar->m_pDockBar == this)
		pBar->m_pDockBar->RemoveControlBar(pBar, nPos);
	else if (pBar->m_pDockBar != NULL)
		pBar->m_pDockBar->RemoveControlBar(pBar, -1, m_bFloating && !pBar->m_pDockBar->m_bFloating);
	pBar->m_pDockBar = this;

	if (bShow)
	{
		ASSERT(!pBar->IsWindowVisible());
		pBar->SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
	}

	// remove any place holder for pBar in this dockbar
	RemovePlaceHolder(pBar);

	// get parent frame for recalc layout
	CFrameWnd* pFrameWnd = GetDockingFrame();
	pFrameWnd->DelayRecalcLayout();
}

int CXTDockBar::GetRowSizeBars(int nInitPos, CControlBar *pNewBar)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	int nSizeBars = 0;
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			if (pBar != NULL && pBar->GetSafeHwnd() && pBar->IsVisible())
			{
				// if is a size bar
				if (CXTDockWindow::IsDockWindow(pBar) && pBar != pNewBar)
					// inc. count
					nSizeBars++;
			}
		}
		else
			//end of row
			break;
	}
	return nSizeBars;
}

void CXTDockBar::AddSplitter(CRect rcSplitter, int nType, BOOL bInterRow, int nPos)
{
    CXTSplitterDock * pSplitter;
	if (bInterRow)
    {
        pSplitter = new CXTSplitterDock(this, rcSplitter, nType, nPos);
    }
	else
    {
		pSplitter = new CXTSplitterRowDock(this, rcSplitter, nType, nPos);
    }
    m_arrSplitters.Add(pSplitter);
}

void CXTDockBar::DeleteSplitters()
{
	int iItem;
    for (iItem = 0; iItem < m_arrSplitters.GetSize(); iItem++)
	{
        CXTSplitterDock* pSplitter = m_arrSplitters.GetAt(iItem);
		SAFE_DELETE( pSplitter );
	}
    m_arrSplitters.RemoveAll();
}

void CXTDockBar::SetSplittersLength(int nStart, int nType, int nLength)
{
    ASSERT(nType == XT_SPLITTER_VERT || nType == XT_SPLITTER_HORZ);
    ASSERT(nStart >= 0 && nStart <= m_arrSplitters.GetSize());
	
	int i;
	for (i = nStart; i < m_arrSplitters.GetSize(); i++)
    {
        CXTSplitterDock * pItem = m_arrSplitters[i];
        if (pItem->m_nType == nType)
			pItem->SetLength(nLength);
	}
}

void CXTDockBar::DrawSplitters(CDC *dc)
{
	// layout all the control bars
	int nPos;
	for (nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
	{
		CXTControlBar* pBar = (CXTControlBar*)GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if ((pVoid != NULL) && (pBar != NULL))
		{
			int i;
			for (i = 0; i < m_arrSplitters.GetSize(); i++)
				m_arrSplitters[i]->Draw(dc);
		}
	}
}

void CXTDockBar::DoPaint(CDC *pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	CDockBar::DoPaint(pDC);
	DrawSplitters(pDC);
}

// Adjust the Bars of the Row in which pNewBar is to match the CDockBar width
// If bNewBar is TRUE the size of pNewBar is not modified
// Other considerations:
//	- The fixed bars (not CXTDockWindows ones) are intended to be full shown
//	- The sized bars size are changed to accommodate the new bar or Dock bar size, their min. width is
//	taken from CXTDockWindow::GetMinExt().
//  - pNewBar width is only modified if fixed width plus min sized width is larger than DockBar

void CXTDockBar::AdjustRowSizes(int nInitPos)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());

	// find if there is a new docked Bar
	CControlBar *pNewBar = FindNewBar(nInitPos);
	if (pNewBar)
		// remove NEWDOCK style
		((CXTDockWindow *)pNewBar)->ModifyXTBarStyle(CBRS_XT_NEWDOCKED, 0, FALSE);

	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	
	// obtain maximized bar.
	CXTDockWindow *pMaxBar = IsRowMaximized(nInitPos);
	if (pMaxBar)	
		Normalize(pMaxBar);
	else 
		Normalize(FindUniqueBar(nInitPos));
	int nMaxHeight  = GetRowMaxHeight(nInitPos, NULL);

	// get row size info each of thes functions loop all bar in the row, better 
	// performance if we fusion all functions in one
	int nSizedBars = GetRowSizeBars(nInitPos);
	
	// if there is no sized bars...
	if (nSizedBars == 0)
		return; //...nothing to do

	// new row width
	int nNewRowWidth;
	if (!m_rectLayout.IsRectEmpty())
	{
		nNewRowWidth = bHorz ? m_rectLayout.Width() : m_rectLayout.Height();
	}
	else
	{
		CRect rectAvail;
		GetClientRect(&rectAvail);
		nNewRowWidth = bHorz ? rectAvail.Width() : rectAvail.Height();
	}

	// if there is only one sized bar...
	if (nSizedBars == 1)
	{
		CControlBar* pBar = FindUniqueBar(nInitPos);
		if (pBar != NULL)
		{
			if (bHorz)
				((CXTDockWindow *) pBar)->m_bp.sizeHorz.cy = 
					((CXTDockWindow *) pBar)->m_bp.sizeDefault.cy = nMaxHeight;
			else
				((CXTDockWindow *) pBar)->m_bp.sizeVert.cx = 
					((CXTDockWindow *) pBar)->m_bp.sizeDefault.cx = nMaxHeight;

			// removes fixed part
			nNewRowWidth -= GetRowFixedWidth(nInitPos);
			((CXTDockWindow *) pBar)->Maximize(nNewRowWidth);
		}
		return;
	}

	int nSizedWidth = GetRowSizedWidth(nInitPos, pNewBar);	
	int nFixedWidth = GetRowFixedWidth(nInitPos, pNewBar);	

	// width increment (new - old)
	int nIncWidth = (nNewRowWidth - (nFixedWidth + nSizedWidth));
	
	// increment on new bar
	int nPos;
	
	CSize sizeNewBar;
	if (pNewBar)
	{
		sizeNewBar = pNewBar->CalcDynamicLayout(-1, bHorz ? LM_HORZDOCK | LM_HORZ : LM_VERTDOCK);
		// decrease the new bar width	
		nIncWidth -= bHorz ? sizeNewBar.cx : sizeNewBar.cy;

		// if the new bar is only sized one...
		if (nSizedBars == 1)
		{
			// .. all the increment is for this bar
			// nNewIncWidth = nIncWidth;
			// It's not necessary the loop
			goto AdjustNewBar;
		}
		else
			// no consider the new one
			nSizedBars--;
	}
	
	// adjust bars size
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			if (pBar != NULL && pBar->GetSafeHwnd() && pBar->IsVisible() && CXTDockWindow::IsDockWindow(pBar) &&
				!(pBar == pNewBar))
			{
				int nNewWidth;
				int nBarIncWidth;
				CSize sizeBar;
				if (bHorz)
				{
					sizeBar = pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
					nBarIncWidth = nIncWidth/nSizedBars;
					if (nIncWidth < 0)
						nBarIncWidth += __min(0, nIncWidth - nBarIncWidth + (GetRowSizedWidth(nPos + 1, pNewBar) - GetRowMinSizedWidth(nPos + 1, pNewBar)));
					nNewWidth = __max(sizeBar.cx + nBarIncWidth, ((CXTDockWindow *) pBar)->GetMinExt());
					nIncWidth -= (nNewWidth - sizeBar.cx);
					sizeBar.cx = nNewWidth;
					sizeBar.cy = nMaxHeight;
				}
				else
				{
					sizeBar = pBar->CalcDynamicLayout(-1, LM_VERTDOCK);
					nBarIncWidth = nIncWidth/nSizedBars;
					if (nIncWidth < 0)
						nBarIncWidth += __min(0, nIncWidth - nBarIncWidth + (GetRowSizedWidth(nPos + 1, pNewBar) - GetRowMinSizedWidth(nPos + 1, pNewBar)));
					nNewWidth = __max(sizeBar.cy + nBarIncWidth, ((CXTDockWindow *) pBar)->GetMinExt());
					nIncWidth -= (nNewWidth - sizeBar.cy);
					sizeBar.cx = nMaxHeight;
					sizeBar.cy = nNewWidth;
				}
				((CXTDockWindow *) pBar)->SetNormalSize(sizeBar);
				pBar->SetWindowPos(NULL, 0, 0, sizeBar.cx, sizeBar.cy, SWP_NOMOVE | SWP_NOZORDER);
				nSizedBars--;
			}
		}
		else
		{
			break;
		}
	} 

// adjust new bar size
AdjustNewBar:

	if (pNewBar)// && CXTDockWindow::IsDockWindow (pNewBar))
	{
		// the new bar absorbs the remaining increment
		int nNewWidth;
		if (bHorz)
		{
			nNewWidth = __max(sizeNewBar.cx + nIncWidth, ((CXTDockWindow *) pNewBar)->GetMinExt());
			nIncWidth -= (nNewWidth - sizeNewBar.cx);
			sizeNewBar.cx = nNewWidth;
			sizeNewBar.cy = nMaxHeight;
		}
		else
		{
			nNewWidth = __max(sizeNewBar.cy + nIncWidth, ((CXTDockWindow *) pNewBar)->GetMinExt());
			nIncWidth -= (nNewWidth - sizeNewBar.cy);
			sizeNewBar.cx = nMaxHeight;
			sizeNewBar.cy = nNewWidth;
		}
		((CXTDockWindow *) pNewBar)->SetNormalSize(sizeNewBar);
		pNewBar->SetWindowPos(NULL, 0, 0, sizeNewBar.cx, sizeNewBar.cy, SWP_NOMOVE | SWP_NOZORDER);
	}
	
	// if the row was maximized...
	if (pMaxBar)
	{
		// ...maximize...
		if (pNewBar /*&& CXTDockWindow::IsDockWindow (pNewBar)*/)
			// ... the new bar or...
			Maximize((CXTDockWindow *)pNewBar);
		else
			// ...the previous maximized bar
			Maximize(pMaxBar);
	}
}

int CXTDockBar::GetFirstBarInRow(CControlBar *pBar)
{
	int nPos = FindBar(pBar);

	//bar not found
	if (nPos < 0)
		return -1;
	
	while (nPos >= 0)
	{
		if (m_arrBars[nPos] == NULL)
			return (nPos + 1);
		nPos --;
	}
    return 0;
}

int CXTDockBar::GetRowMaxHeight(int nInitPos, CControlBar *pNewBar)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	int nMaxHeight = 0;
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			if (pBar != NULL && pBar->GetSafeHwnd() && pBar->IsVisible() && pBar != pNewBar)
			{
				if (bHorz)
				{
					CSize sizeBar = pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
					nMaxHeight = __max(nMaxHeight, sizeBar.cy);
				}
				else
				{
					CSize sizeBar = pBar->CalcDynamicLayout(-1, LM_VERTDOCK);
					nMaxHeight = __max(nMaxHeight, sizeBar.cx);
				}
			}
		}
		else
			// end of row
			break;
	}
	// end of bars
	return nMaxHeight;
}

int CXTDockBar::GetRowMinHeight(int nInitPos, CControlBar *pNewBar)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	int nMinHeight = 2;	// min Height of a sized bar
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			if (pBar != NULL && pBar->GetSafeHwnd() && pBar->IsVisible() && pBar != pNewBar && !CXTDockWindow::IsDockWindow(pBar))
			{
				if (bHorz)
				{
					CSize sizeBar = pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
					nMinHeight = __max(nMinHeight, sizeBar.cy);
				}
				else
				{
					CSize sizeBar = pBar->CalcDynamicLayout(-1, LM_VERTDOCK);
					nMinHeight = __max(nMinHeight, sizeBar.cx);
				}
			}
		}
		else
			// end of row
			break;
	}
	// end of bars
	return nMinHeight;
}

int CXTDockBar::GetRowSizedWidth(int nInitPos, CControlBar *pNewBar)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	
	int nSizedWidth = 0;	
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			CXTDockWindow* pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL && pDockWindow != pNewBar)
			{
				if (bHorz) {
					nSizedWidth += pDockWindow->GetVertSize();
				}
				else {
					nSizedWidth += pDockWindow->GetHorzSize();
				}
			} 
		}
		else
			// end of row
			break;
	}
	
	// end of bars
	return nSizedWidth;
}

int CXTDockBar::GetRowMinSizedWidth(int nInitPos, CControlBar *pNewBar)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	int nSizedWidth = 0;	
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			CXTDockWindow* pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL && pDockWindow != pNewBar)
			{
				nSizedWidth += pDockWindow->GetMinExt();
			} 
		}
		else
			// end of row
			break;
	}
	// end of bars
	return nSizedWidth;
}

// return size of fixed bar plus Splitters 

int CXTDockBar::GetRowFixedWidth(int nInitPos, CControlBar *pNewBar)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	
	CControlBar *pBar = GetDockedControlBar(nInitPos);

	// if no control bars were found return 0.
	int nFirstBar = GetFirstBarInRow(pBar);
	if (nFirstBar == -1)
		return 0;

	int nFixedWidth = 0;	
	BOOL bPrevBarSizeable = FALSE;
	BOOL bBarSizeable = FALSE;
	
	int nSizeBars = GetRowSizeBars(nFirstBar);
	int nSizeBarsRight = nSizeBars;
	int nPos;
	for (nPos = nFirstBar; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			if (pBar != NULL && pBar->GetSafeHwnd() && pBar->IsVisible())
			{
				// Is Bar sizeable?
				bBarSizeable = CXTDockWindow::IsDockWindow(pBar);
				
				// check if a Splitter inside row is needed
				if ((bBarSizeable && (nSizeBars > nSizeBarsRight) && nPos >= nInitPos) ||
					(!bBarSizeable && bPrevBarSizeable && nSizeBarsRight > 0 && nPos >= nInitPos))
					
				{
					// adds a splitter to FizedSize
					nFixedWidth += XT_TRACK_BORDER_SIZE;
				} 
				if (pBar != pNewBar && !bBarSizeable && nPos >= nInitPos)
				{
					if (bHorz)
					{
						CSize sizeBar = pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
						nFixedWidth += sizeBar.cx;
					}
					else
					{
						CSize sizeBar = pBar->CalcDynamicLayout(-1, LM_VERTDOCK);
						nFixedWidth += sizeBar.cy;
					}
				}
				if (bBarSizeable)
					nSizeBarsRight--;
				bPrevBarSizeable  = bBarSizeable;
			} 
		}
		else
			// end of row
			break;
	}
	// end of bars
	return nFixedWidth;
}

CControlBar *CXTDockBar::FindNewBar(int nInitPos)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			CXTDockWindow* pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL && pDockWindow->GetXTBarStyle() & CBRS_XT_NEWDOCKED)
			{
				return pDockWindow;
			} 
		}
		else
			// end of row
			break;
	}
	// New bar not found
	return NULL;
}

CXTDockWindow *CXTDockBar::FindUniqueBar(int nInitPos)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			CXTDockWindow* pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL)
			{
				return pDockWindow;
			} 
		}
		else
			// end of row
			break;
	}
	// New bar not found
	return NULL;
}

LRESULT CXTDockBar::OnNcHitTest(CPoint point) 
{
	HitTest(point);
	return (m_nTrackSplitter != -1)?HTBORDER:CDockBar::OnNcHitTest(point);
}

void CXTDockBar::HitTest(const CPoint & point)
{
	::SetCursor(afxData.hcurArrow);

	// If another window has capture, don't
	// even bother...
	if (GetCapture() != NULL)
		return;

	CPoint pointClient = point;
	ScreenToClient(&pointClient);
	m_nTrackSplitter = -1;
	
	// Check all the Splitters
	int nSplitter;
	for (nSplitter = 0; nSplitter < m_arrSplitters.GetSize(); nSplitter++)
	{
		// check splitter
		if (m_arrSplitters[nSplitter]->HitTest(pointClient))
		{
			if (m_arrSplitters[nSplitter]->IsHorz()) {
				::SetCursor(xtAfxData.hcurVSplitBar);
			}
			else {
				::SetCursor(xtAfxData.hcurHSplitBar);
			}

			m_nTrackSplitter = nSplitter;
			break;
		}
	}
}

BOOL CXTDockBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest != HTCLIENT)
		return FALSE;

	return CDockBar::OnSetCursor(pWnd, nHitTest, message);
}

void CXTDockBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
    if ((m_nTrackSplitter != -1) && (m_arrSplitters[m_nTrackSplitter]->StartTrack(point)))
    {
        m_hWndFocus = ::SetFocus(m_hWnd);
        m_bTracking = TRUE;

        // fix: Alexander Kirichenko <alexk@ProximaSoftware.ru> call SetCapture() 
        // after calling SetFocus(m_hWnd), not before.
        SetCapture();
    }
	else
		CDockBar::OnNcLButtonDown(nHitTest, point);
}

void CXTDockBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bTracking && (m_nTrackSplitter != -1))
		m_arrSplitters[m_nTrackSplitter]->Move(point);
	else
		CDockBar::OnMouseMove(nFlags, point);
}

void CXTDockBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bTracking)
	{
		m_bTracking = FALSE;
		ReleaseCapture();
		
        if (m_hWndFocus) {
			::SetFocus(m_hWndFocus);
        }
        
		m_arrSplitters[m_nTrackSplitter]->EndTrack(point);
	} 
	else 
		CDockBar::OnLButtonUp(nFlags, point);
}

int CXTDockBar::GetAvailableHeight()
{
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	
	// get client size of the Frame Window
	CRect rect;
	m_pDockSite->GetClientRect(&rect);
	
	int nHeight;
	if (bHorz)
	{
		nHeight = rect.Height();
		// decrements top and bottom dockbars height
		CXTDockBar *pBar = (CXTDockBar*)m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_TOP);
		if (pBar && pBar->IsVisible())
		{
			pBar->GetWindowRect(rect);
			nHeight -= rect.Height();
		}
		pBar = (CXTDockBar*)m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_BOTTOM);
		if (pBar && pBar->IsVisible())
		{
			pBar->GetWindowRect(rect);
			nHeight -= rect.Height();
		}
		// also decrements status bar height
		pBar = (CXTDockBar*)m_pDockSite->GetControlBar(AFX_IDW_STATUS_BAR);
		if (pBar && pBar->IsVisible())
		{
			pBar->GetWindowRect(rect);
			nHeight -= rect.Height();
		}
	}
	else
	{
		nHeight = rect.Width();
		// decrements left and right dockbars height
		CXTDockBar *pBar = (CXTDockBar*)m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_LEFT);
		if (pBar && pBar->IsVisible())
		{
			pBar->GetWindowRect(rect);
			nHeight -= rect.Width();
		}
		pBar = (CXTDockBar*)m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_RIGHT);
		if (pBar && pBar->IsVisible())
		{
			pBar->GetWindowRect(rect);
			nHeight -= rect.Width();
		}
	}
	return nHeight;
}

int CXTDockBar::SetRowHeight(int nInitPos, int nNewHeight)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	ASSERT(nNewHeight >= GetRowMinHeight(nInitPos));
	
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			CXTDockWindow* pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL)
			{
				CSize sizeBar;
				if (bHorz)
				{
					sizeBar = pDockWindow->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
					sizeBar.cy = nNewHeight;
				}
				else
				{
					sizeBar = pDockWindow->CalcDynamicLayout(-1, LM_VERTDOCK);
					sizeBar.cx = nNewHeight;
				}
				
				pDockWindow->SetNormalSize(sizeBar);
				//pDockWindow->SetWindowPos(NULL, 0, 0, sizeBar.cx, sizeBar.cy, SWP_NOMOVE | SWP_NOZORDER);
			}
		}
		else
			// end of row
			break;
	}
	// end of bars
	return nNewHeight;
}

int CXTDockBar::StretchRowLeft(int nInitPos, int nIncWidth)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	
	CControlBar *pBar = GetDockedControlBar(nInitPos);
	int nFirstBar = GetFirstBarInRow(pBar);
	int nSizedBars = GetRowSizeBars(nFirstBar) - GetRowSizeBars(nInitPos);

	// if there is no sized bars...
	if (nSizedBars == 0 || nIncWidth == 0 || nFirstBar == -1)
		//...nothing to do
		return nIncWidth;
	
	// adjust bars size
	int nPos;
	for (nPos = nInitPos - 1; nPos >= nFirstBar; nPos--)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			CXTDockWindow* pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL)
			{
				int nInc;
				CSize sizeBar;
				sizeBar.cx = pDockWindow->GetVertSize();
				sizeBar.cy = pDockWindow->GetHorzSize();
				if (bHorz)
				{
					nInc = __max(nIncWidth, pDockWindow->GetMinExt() - sizeBar.cx);
					sizeBar.cx += nInc;
				}
				else
				{
					nInc = __max(nIncWidth, pDockWindow->GetMinExt() - sizeBar.cy);
					sizeBar.cy += nInc;
				}
				
				pDockWindow->SetNormalSize(sizeBar);
				//pDockWindow->SetWindowPos(NULL, 0, 0, sizeBar.cx, sizeBar.cy, SWP_NOMOVE | SWP_NOZORDER);
				nIncWidth -= nInc;
			}
		}
	} 
	return nIncWidth;
}

int CXTDockBar::StretchRowRight(int nInitPos, int nIncWidth)
{
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	
	int nSizedBars = GetRowSizeBars(nInitPos);
	
	// if there is no sized bars...
	if (nSizedBars == 0 || nIncWidth == 0)
		//...nothing to do
		return nIncWidth;
	
	// adjust bars size
	int nPos;
	for (nPos = nInitPos ; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			CXTDockWindow* pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL)
			{
				int nInc;
				CSize sizeBar;
				sizeBar.cx = pDockWindow->GetVertSize();
				sizeBar.cy = pDockWindow->GetHorzSize();
				if (bHorz)
				{
					nInc = __max(nIncWidth, pDockWindow->GetMinExt() - sizeBar.cx);
					sizeBar.cx += nInc;
				}
				else
				{
					nInc = __max(nIncWidth, pDockWindow->GetMinExt() - sizeBar.cy);
					sizeBar.cy += nInc;
				}
				
				pDockWindow->SetNormalSize(sizeBar);
				//pDockWindow->SetWindowPos(NULL, 0, 0, sizeBar.cx, sizeBar.cy, SWP_NOMOVE | SWP_NOZORDER);
				nIncWidth -= nInc;
			}
		}
		else
			// end of row
			break;
	} 
	return nIncWidth;
}

void CXTDockBar::Maximize(CXTDockWindow* pMaxBar, BOOL bRecalcLayout)
{
	ASSERT(pMaxBar);
	int nMaxSize;
	if (pMaxBar->IsHorzDocked())
	{
		CSize sizeBar = pMaxBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
		nMaxSize = sizeBar.cx;
	}
	else
	{
		CSize sizeBar = pMaxBar->CalcDynamicLayout(-1, LM_VERTDOCK);
		nMaxSize = sizeBar.cy;
	}
	
	// if no control bars were found return.
	int nFirstBar = GetFirstBarInRow(pMaxBar);
	if (nFirstBar == -1)
		return;

	CXTDockWindow* pDockWindow = NULL;
	int nPos;
	for (nPos = nFirstBar; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL && pDockWindow != pMaxBar) {
				nMaxSize += pDockWindow->Minimize();
			}
		}
		else
			// end of row
			break;
	}
	pMaxBar->Maximize(nMaxSize);
	
	if (pDockWindow && bRecalcLayout)
		m_pDockSite->RecalcLayout();
}

void CXTDockBar::Normalize(CXTDockWindow *pMaxBar, BOOL bRecalcLayout)
{
	if (pMaxBar == NULL)
		return;

	// if no control bars were found return.
	int nFirstBar = GetFirstBarInRow(pMaxBar);
	if (nFirstBar == -1)
		return;

	CXTDockWindow* pDockWindow = NULL;
	int nPos;
	for (nPos = nFirstBar; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL) {
				pDockWindow->Normalize();
			}
		}
		else
			// end of row
			break;
	}
	if (pDockWindow && bRecalcLayout)
		m_pDockSite->RecalcLayout();
}

CXTDockWindow * CXTDockBar::IsRowMaximized(int nBar)
{
	// if no control bars were found return NULL.
	int nFirstBar = GetFirstBarInRow(GetDockedControlBar(nBar));
	if (nFirstBar == -1)
		return NULL;

	int nPos;
	for (nPos = nFirstBar; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];
		
		if (pVoid != NULL)
		{
			CXTDockWindow* pDockWindow = GetSafeDockWindow(pBar);
			if (pDockWindow != NULL)
			{
				if (pDockWindow->IsMaximized() && !pDockWindow->IsUnique()) {
					return pDockWindow;
				}
			}
		}
		else
			// end of row: not maximized
			return NULL;
	}
	// end of row: not maximized
	return NULL;
}

void CXTDockBar::OnCancel()
{
	if(m_bTracking)
	{
		m_arrSplitters[m_nTrackSplitter]->OnInvertTracker();
		m_bTracking = FALSE;
		ReleaseCapture();
		if (m_hWndFocus)
			::SetFocus(m_hWndFocus);
	}
}

void CXTDockBar::RemoveRowBorders(int nInitPos, DWORD dwBarStyle, BOOL bOnlySized)
{
	ASSERT(dwBarStyle & CBRS_BORDER_ANY);
	ASSERT(nInitPos >= 0 && nInitPos < m_arrBars.GetSize());
	
	BOOL bHorz = m_dwStyle & (CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	
	int nMaxHeight = GetRowMaxHeight(nInitPos);	
	
	int nPos;
	for (nPos = nInitPos; nPos < m_arrBars.GetSize(); nPos++)
	{
		CControlBar* pBar = GetDockedControlBar(nPos);
		void* pVoid = m_arrBars[nPos];

		if (pVoid != NULL)
		{
			if (pBar != NULL && pBar->GetSafeHwnd() && pBar->IsVisible())
			{
				// remove border in sizeables bars
				if (CXTDockWindow::IsDockWindow(pBar))
				{
					pBar->SetBarStyle(pBar->GetBarStyle() & ~dwBarStyle);
				}
				// and in fixed bars with max height
				else if (!bOnlySized && !(GetXTBarStyle() & CBRS_XT_FLAT))
				{
					int nBarHeight;
					if (bHorz)
					{
						CSize sizeBar = pBar->CalcDynamicLayout(-1, LM_HORZ | LM_HORZDOCK);
						nBarHeight = sizeBar.cy;
					}
					else
					{
						CSize sizeBar = pBar->CalcDynamicLayout(-1, LM_VERTDOCK);
						nBarHeight = sizeBar.cx;
					}
					if (nBarHeight == nMaxHeight)
						pBar->SetBarStyle(pBar->GetBarStyle() & ~dwBarStyle);
				}
			}
		}
		else
			// end of row
			break;
	}
}

void CXTDockBar::OnPaint() 
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectWindow);

	// erase background now
	if (GetSafeHwnd() && IsVisible() && GetDockedVisibleCount() != 0)
		DoPaint(&memDC);       // delegate to paint helper
}

BOOL CXTDockBar::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CXTDockBar::OnRButtonDown(UINT /*nFlags*/, CPoint pt) 
{
	// If dock bar menus are disabled, return.
	if (!xtAfxData.bDockBarMenus) {
		return;
	}

	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	CPoint point = pt;
	ClientToScreen(&point);

	ASSERT_VALID(this);
	BOOL bFound = FALSE;

	CXTMenu menu;
	menu.CreatePopupMenu();

	// Make sure the CMainFrame is always the owner window.
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT_VALID(pFrameWnd);
	
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

			if ( bCustomize == false )
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
}

void CXTDockBar::OnNcPaint() 
{
	// get window DC that is clipped to the non-client area
	CWindowDC dc(this);
	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	dc.ExcludeClipRect(rectClient);

	// draw borders in non-client area
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
	DrawNcBorders(&dc, rectWindow);

	// erase parts not drawn
	dc.IntersectClipRect(rectWindow);
	SendMessage(WM_ERASEBKGND, (WPARAM)dc.m_hDC);

	// draw gripper in non-client area
	DrawGripper(&dc, rectWindow);
}

#if _MSC_VER < 1200 // MFC 5.0

#define CX_GRIPPER  3
#define CY_GRIPPER  3
#define CX_BORDER_GRIPPER 2
#define CY_BORDER_GRIPPER 2

void CXTDockBar::DrawGripper(CDC* pDC, const CRect& rect)
{
	// only draw the gripper if not floating and gripper is specified
	if ((m_dwStyle & (CBRS_GRIPPER|CBRS_FLOATING)) == CBRS_GRIPPER)
	{
		// draw the gripper in the border
		if (m_dwStyle & CBRS_ORIENT_HORZ)
		{
			pDC->Draw3dRect(rect.left+CX_BORDER_GRIPPER,
				rect.top+m_cyTopBorder,
				CX_GRIPPER, rect.Height()-m_cyTopBorder-m_cyBottomBorder,
				afxData.clrBtnHilite, afxData.clrBtnShadow);
		}
		else
		{
			pDC->Draw3dRect(rect.left+m_cyTopBorder,
				rect.top+CY_BORDER_GRIPPER,
				rect.Width()-m_cyTopBorder-m_cyBottomBorder, CY_GRIPPER,
				afxData.clrBtnHilite, afxData.clrBtnShadow);
		}
	}
}

#endif //_MSC_VER < 1200

void CXTDockBar::DrawNcBorders(CDC* pDC, CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	DWORD dwStyle = m_dwStyle;
	if (!(dwStyle & CBRS_BORDER_ANY))
		return;

	// prepare for dark lines
	ASSERT(rect.top == 0 && rect.left == 0);
	CRect rect1, rect2;
	rect1 = rect;
	rect2 = rect;

	COLORREF clr = xtAfxData.bXPMode ? xtAfxData.clr3DFace : afxData.clrBtnShadow;

	// draw dark line one pixel back/up
	if (dwStyle & CBRS_BORDER_3D)
	{
		rect1.right -= CX_BORDER;
		rect1.bottom -= CY_BORDER;
	}
	if (dwStyle & CBRS_BORDER_TOP)
		rect2.top += afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect2.bottom -= afxData.cyBorder2;

	// draw left and top
	if (dwStyle & CBRS_BORDER_LEFT)
		pDC->FillSolidRect(0, rect2.top, CX_BORDER, rect2.Height(), clr);
	if (dwStyle & CBRS_BORDER_TOP)
		pDC->FillSolidRect(0, 0, rect.right, CY_BORDER, clr);

	// draw right and bottom
	if (dwStyle & CBRS_BORDER_RIGHT)
		pDC->FillSolidRect(rect1.right, rect2.top, -CX_BORDER, rect2.Height(), clr);
	if (dwStyle & CBRS_BORDER_BOTTOM)
		pDC->FillSolidRect(0, rect1.bottom, rect.right, -CY_BORDER, clr);

	if (dwStyle & CBRS_BORDER_3D)
	{
		// prepare for highlight lines
		clr = xtAfxData.bXPMode ? xtAfxData.clr3DFace : afxData.clrBtnHilite;

		// draw left and top
		if (dwStyle & CBRS_BORDER_LEFT)
			pDC->FillSolidRect(1, rect2.top, CX_BORDER, rect2.Height(), clr);
		if (dwStyle & CBRS_BORDER_TOP)
			pDC->FillSolidRect(0, 1, rect.right, CY_BORDER, clr);

		// draw right and bottom
		if (dwStyle & CBRS_BORDER_RIGHT)
			pDC->FillSolidRect(rect.right, rect2.top, -CX_BORDER, rect2.Height(), clr);
		if (dwStyle & CBRS_BORDER_BOTTOM)
			pDC->FillSolidRect(0, rect.bottom, rect.right, -CY_BORDER, clr);
	}

	if (dwStyle & CBRS_BORDER_LEFT)
		rect.left += afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_TOP)
		rect.top += afxData.cyBorder2;
	if (dwStyle & CBRS_BORDER_RIGHT)
		rect.right -= afxData.cxBorder2;
	if (dwStyle & CBRS_BORDER_BOTTOM)
		rect.bottom -= afxData.cyBorder2;
}
