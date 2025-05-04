// XTStatusBar.cpp : implementation file
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
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTStatusBar.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTStatusBar

CXTStatusBar::CXTStatusBar()
{
	// TODO: add construction code here.
}

CXTStatusBar::~CXTStatusBar()
{
	// TODO: add destruction code here.
	int i;
	for ( i = 0; i < m_arControls.GetSize(); i++ )
	{
		if( m_arControls[i]->pWnd && ::IsWindow(m_arControls[i]->pWnd->m_hWnd) ) {
			m_arControls[i]->pWnd->CloseWindow();
		}
		
		if( m_arControls[i]->bAutoDelete ) {
			SAFE_DELETE (m_arControls[i]->pWnd);
		}
		
		SAFE_DELETE (m_arControls[i]);
	}
}

IMPLEMENT_DYNAMIC(CXTStatusBar, CStatusBar)

BEGIN_MESSAGE_MAP(CXTStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CXTStatusBar)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_IDLEUPDATECMDUI, OnIdleUpdateCmdUI)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTStatusBar message handlers

BOOL CXTStatusBar::GetPaneInfoEx(int nIndex, XT_STATUSPANE* pSBP)
{
	if( nIndex < m_nCount  && nIndex >= 0 )
	{
		GetPaneInfo (nIndex,  pSBP->nID, pSBP->nStyle, pSBP->cxText);
		GetPaneText (nIndex , pSBP->strText);
		return TRUE;
	}
	return FALSE;
}

BOOL CXTStatusBar::SetPaneInfoEx(int nIndex, XT_STATUSPANE* pSBP)
{
	if( nIndex < m_nCount  && nIndex >= 0 )
	{
		SetPaneInfo (nIndex, pSBP->nID, pSBP->nStyle, pSBP->cxText);
		SetPaneText (nIndex, pSBP->strText);
		return TRUE;
	}
	return FALSE;
}

void CXTStatusBar::SetPaneWidth(int nIndex, int cxText)
{
	XT_STATUSPANE sbp;
	GetPaneInfoEx(nIndex, &sbp);
	sbp.cxText = cxText;
	SetPaneInfoEx(nIndex, &sbp);
}

BOOL CXTStatusBar::RemoveIndicator(UINT nID)
{
	if (CommandToIndex(nID) == -1)
	{
		TRACE1("WARNING: Pane ID %d was not found in the status bar.\n", nID);
		return FALSE;
	}

	// initialize an array of pane pointers minus the
	// one to be removed.
	CArray<XT_STATUSPANE*,XT_STATUSPANE*> sbpArray;
	int nIndex;
	for (nIndex = 0; nIndex < m_nCount; ++nIndex)
	{
		XT_STATUSPANE* pOldSBP = GetPanePtr(nIndex);
		
		if (pOldSBP->nID == nID)
			continue;

		XT_STATUSPANE* pNewSBP = new XT_STATUSPANE;

		pNewSBP->cxText  = pOldSBP->cxText;
		pNewSBP->nFlags  = pOldSBP->nFlags;
		pNewSBP->nID     = pOldSBP->nID;
		pNewSBP->nStyle  = pOldSBP->nStyle;
		pNewSBP->strText = pOldSBP->strText;
		sbpArray.Add(pNewSBP);
	}

	// allocate a new indicator array and initialize the value for each.
	UINT* lpIDArray = new UINT[sbpArray.GetSize()];
	for (nIndex = 0; nIndex < sbpArray.GetSize(); ++nIndex) {
		lpIDArray[nIndex] = sbpArray[nIndex]->nID;
	}

	// set the indicators and free all allocated memory.
	SetIndicators(lpIDArray, sbpArray.GetSize());
	for (nIndex = 0; nIndex < sbpArray.GetSize(); ++nIndex)
	{
		XT_STATUSPANE* pSBP = sbpArray[nIndex];
		SetPaneInfoEx(nIndex, pSBP);
		SAFE_DELETE (pSBP);
	}

	int i;
	for ( i = 0; i < m_arControls.GetSize(); i++ )
	{
		if (m_arControls[i]->nID == nID)
		{
			if( m_arControls[i]->pWnd && ::IsWindow(m_arControls[i]->pWnd->m_hWnd) ) {
				m_arControls[i]->pWnd->DestroyWindow();
			}
			
			if( m_arControls[i]->bAutoDelete ) {
				SAFE_DELETE (m_arControls[i]->pWnd);
			}
			
			SAFE_DELETE (m_arControls[i]);
			m_arControls.RemoveAt(i);
			
			break;
		}
	}

	sbpArray.RemoveAll();
	SAFE_DELETE_AR (lpIDArray);
	PositionControls();

	return TRUE;
}

BOOL CXTStatusBar::AddIndicator(UINT nID, int nIndex)
{
	if (nIndex < 0 || nIndex > m_nCount)
	{
		TRACE0("WARNING: Pane index out of range.\n");
		return FALSE;
	}

	if (CommandToIndex(nID) != -1)
	{
		TRACE1("WARNING: Pane ID %d already exists in the status bar.\n", nID);
		return FALSE;
	}

	// initialize an array of pane pointers minus the
	// one to be removed.
	CArray<XT_STATUSPANE*,XT_STATUSPANE*> sbpArray;
	int iIndex;
	for (iIndex = 0; iIndex < m_nCount+1; ++iIndex)
	{
		XT_STATUSPANE* pNewSBP = new XT_STATUSPANE;

		if (iIndex == nIndex)
		{
			pNewSBP->cxText = 0;
			pNewSBP->nID    = nID;
			pNewSBP->nStyle = SBPS_NORMAL;
		}
		else
		{
			int idx = iIndex;
			if (iIndex > nIndex)
				--idx;

			XT_STATUSPANE* pOldSBP = GetPanePtr(idx);
			pNewSBP->cxText  = pOldSBP->cxText;
			pNewSBP->nFlags  = pOldSBP->nFlags;
			pNewSBP->nID     = pOldSBP->nID;
			pNewSBP->nStyle  = pOldSBP->nStyle;
			pNewSBP->strText = pOldSBP->strText;
		}

		sbpArray.Add(pNewSBP);
	}

	// allocate a new indicator array and initialize the value for each.
	UINT* lpIDArray = new UINT[sbpArray.GetSize()];
	for (iIndex = 0; iIndex < sbpArray.GetSize(); ++iIndex) {
		lpIDArray[iIndex] = sbpArray[iIndex]->nID;
	}

	// set the indicators and free all allocated memory.
	SetIndicators(lpIDArray, sbpArray.GetSize());
	for (iIndex = 0; iIndex < sbpArray.GetSize(); ++iIndex)
	{
		XT_STATUSPANE* pSBP = sbpArray[iIndex];
		SetPaneInfoEx(iIndex, pSBP);
		SAFE_DELETE (pSBP);
	}

	sbpArray.RemoveAll();
	SAFE_DELETE_AR (lpIDArray);
	PositionControls();

	return TRUE;
}

void CXTStatusBar::PositionControls()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int cx = ::GetSystemMetrics(SM_CXEDGE);

	int i;
	for (i = 0; i < m_arControls.GetSize(); i++ )
	{
		int   iIndex  = CommandToIndex(m_arControls[i]->nID);
        DWORD dwStyle = GetPaneStyle(iIndex);
		CWnd* pWnd    = m_arControls[i]->pWnd;

		CRect rcPane;
		GetItemRect(iIndex, &rcPane);
		
		// WINBUG: CStatusBar::GetItemRect(...) does not return the correct size 
		// of the last item in the status bar in most cases, so we need to calculate 
		// the width ourself...
		
		if (iIndex == (m_nCount-1))
		{
			if ((dwStyle & SBPS_STRETCH ) == 0)
			{
				UINT nID;
				UINT nStyle;
				int  cxWidth;
				GetPaneInfo(iIndex, nID, nStyle, cxWidth);
				
				rcPane.right = rcPane.left + cxWidth + (cx*3);
			}
			else
			{
				rcPane.right = rcClient.right;
				
				if ((GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP) {
					rcPane.right -= (xtAfxData.cxSmIcon + cx);
				}
			}
		}

		if ((GetPaneStyle (iIndex) & SBPS_NOBORDERS) == 0) {
			rcPane.DeflateRect(cx,cx);
		}

		if (pWnd && ::IsWindow(pWnd->m_hWnd))
			pWnd->MoveWindow(rcPane);
	}
}

void CXTStatusBar::OnSize(UINT nType, int cx, int cy) 
{
	CStatusBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	PositionControls();
}

BOOL CXTStatusBar::AddControl(CWnd* pWnd, UINT nID, BOOL bAutoDelete)
{
	if (CommandToIndex (nID) == -1) {
		return FALSE;
	}

	XT_STATUSPANE_CTRL* pSBPCtrl = new XT_STATUSPANE_CTRL;
	pSBPCtrl->nID         = nID;
	pSBPCtrl->pWnd        = pWnd;
	pSBPCtrl->bAutoDelete = bAutoDelete;

	m_arControls.Add(pSBPCtrl);
	PositionControls();

	return TRUE;
}

XT_STATUSPANE_CTRL* CXTStatusBar::GetPaneControl(UINT nID)
{
	int i;
	for (i = 0; i < m_arControls.GetSize(); i++ )
	{
		if (nID == m_arControls[i]->nID)
			return m_arControls[i];
	}

	return NULL;
}

XT_STATUSPANE* CXTStatusBar::GetPanePtr(int nIndex) const
{
	ASSERT((nIndex >= 0 && nIndex < m_nCount) || m_nCount == 0);
	return ((XT_STATUSPANE*)m_pData) + nIndex;
}

int CXTStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatusBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the font for the status bar.
	SetFont(&xtAfxData.fontHCapt);
	
	return 0;
}

void CXTStatusBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);
	CXTMemDC memDC(&dc, rcClient);

    CStatusBar::DefWindowProc(WM_PAINT, (WPARAM)memDC.m_hDC, 0);

    if (xtAfxData.bXPMode)
    {
        int iIndex;
		for (iIndex = 0; iIndex < m_nCount; ++iIndex)
        {
            DWORD dwStyle = GetPaneStyle(iIndex);

            if ((dwStyle & SBPS_NOBORDERS) == 0)
            {
                CRect rcPane;
                GetItemRect(iIndex, &rcPane);

				// WINBUG: CStatusBar::GetItemRect(...) does not return the correct size 
				// of the last item in the status bar in most cases, so we need to calculate 
				// the width ourself...
				
				if (iIndex == (m_nCount-1))
				{
					int cx = ::GetSystemMetrics(SM_CXEDGE);

					if ((dwStyle & SBPS_STRETCH ) == 0)
					{
						UINT nID;
						UINT nStyle;
						int  cxWidth;
						GetPaneInfo(iIndex, nID, nStyle, cxWidth);

						rcPane.right = rcPane.left + cxWidth + (cx*3);
					}
					else
					{
						rcPane.right = rcClient.right;

						if ((GetStyle() & SBARS_SIZEGRIP) == SBARS_SIZEGRIP) {
							rcPane.right -= (xtAfxData.cxSmIcon + cx);
						}
					}
				}

				// Draw the xp style pane.
                memDC.Draw3dRect(&rcPane,
                    xtAfxData.clr3DShadow, xtAfxData.clr3DShadow);
            }
        }
    }
}

BOOL CXTStatusBar::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

LRESULT CXTStatusBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
{
	// handle delay hide/show
	BOOL bVis = GetStyle() & WS_VISIBLE;
	UINT swpFlags = 0;
	if ((m_nStateFlags & delayHide) && bVis)
		swpFlags = SWP_HIDEWINDOW;
	else if ((m_nStateFlags & delayShow) && !bVis)
		swpFlags = SWP_SHOWWINDOW;
	m_nStateFlags &= ~(delayShow|delayHide);
	if (swpFlags != 0)
	{
		SetWindowPos(NULL, 0, 0, 0, 0, swpFlags|
			SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
	}

	// the style must be visible and if it is docked
	// the dockbar style must also be visible
	if ((GetStyle() & WS_VISIBLE) &&
		(m_pDockBar == NULL || (m_pDockBar->GetStyle() & WS_VISIBLE)))
	{
		// MFCBUG: removed check for IsFrameWnd so we can update statusbar
		// for dialog based applications as well as doc/view apps.
		CFrameWnd* pTarget = (CFrameWnd*)GetOwner();
		if (pTarget == NULL)
			pTarget = (CFrameWnd*)GetParent();
		if (pTarget != NULL)
			OnUpdateCmdUI(pTarget, (BOOL)wParam);
	}
	return 0L;
}
