// XTReBar.cpp : implementation of the CXTReBar class.
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
#include "XTFunctions.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTReBar.h"
#include "XTReBarCtrl.h"
#include "XTRegistryManager.h"
#include "XTRebarBandInfo.h"
#include "XTToolBar.h"

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CXTReBar

BEGIN_MESSAGE_MAP(CXTReBar, CXTControlBar)
	//{{AFX_MSG_MAP(CXTReBar)
	ON_WM_NCCREATE()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(RBN_HEIGHTCHANGE, OnHeightChange)
	ON_WM_NCPAINT()
	ON_WM_NCCALCSIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(RB_SHOWBAND, OnShowBand)
	ON_MESSAGE_VOID(WM_RECALCPARENT, OnRecalcParent)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(RBN_ENDDRAG, OnHeightChange)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(RBN_CHILDSIZE, OnChildSize)
	ON_MESSAGE(XTWM_IDEALSIZECHANGED, OnIdealSizeChanged)
END_MESSAGE_MAP()

CXTReBar::CXTReBar()
: m_iBandCount(-1)
{
	SetBorders();
}

void CXTReBar::OnRecalcParent()
{
	CFrameWnd* pFrameWnd = GetParentFrame();
	ASSERT(pFrameWnd != NULL);
	pFrameWnd->RecalcLayout();
}

void CXTReBar::OnHeightChange(NMHDR* /*pNMHDR*/, LRESULT* pResult)
{
	// gives us access to protected member m_bInRecalcLayout.
	class CFriendFrameWnd : public CFrameWnd
	{
		friend class CXTReBar;
	};

	// does the CXTReBar have a frame?
	CFriendFrameWnd* pFrameWnd = (CFriendFrameWnd*)GetParentFrame();
	if (pFrameWnd != NULL)
	{
		// it does -- tell it to recalc its layout
		if (!pFrameWnd->m_bInRecalcLayout) {
			pFrameWnd->RecalcLayout();
		}
		else {
			PostMessage(WM_RECALCPARENT);
		}
	}
	*pResult = 0;
}

void CXTReBar::OnChildSize(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMREBARCHILDSIZE* sizeInfo = (NMREBARCHILDSIZE*)pNMHDR;
	XT_REBARBANDINFO info(RBBIM_CHILD | RBBIM_IDEALSIZE | RBBIM_CHILDSIZE);
	GetReBarCtrl().GetBandInfo(sizeInfo->uBand, &info);
	bool bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	XT_REBARSIZECHILDINFO param;
	param.pBandInfo = &info;
	param.pSizeInfo = sizeInfo;
	::SendMessage(info.hwndChild, XTWM_REBAR_SIZECHILD, bHorz, (LPARAM)(LPVOID)&param);
	*pResult = 0;
}

LRESULT CXTReBar::OnShowBand(WPARAM wParam, LPARAM)
{
	LRESULT lResult = Default();
	if (lResult)
	{
		// keep window visible state in sync with band visible state
		XT_REBARBANDINFO rbBand(RBBIM_CHILD|RBBIM_STYLE);
		VERIFY(DefWindowProc(RB_GETBANDINFO, wParam, (LPARAM)&rbBand));
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(rbBand.hwndChild));
		BOOL bWindowVisible;
		if (pBar != NULL)
			bWindowVisible = pBar->IsVisible();
		else
			bWindowVisible =  (::GetWindowLong(rbBand.hwndChild, GWL_STYLE) & WS_VISIBLE) != 0;
		BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
		if (bWindowVisible != bBandVisible)
			VERIFY(::ShowWindow(rbBand.hwndChild, bBandVisible ? SW_SHOW : SW_HIDE));
	}
	return lResult;
}

void CXTReBar::SaveState(LPCTSTR lpszSection)
{
	// get a reference to the rebar control.
	CXTReBarCtrl& rebarCtrl = GetReBarCtrl();

	// get the number of bands found in the rebar.
	int nCount = rebarCtrl.GetBandCount();
	
	XT_REBARBANDINFO info(RBBIM_SIZE | RBBIM_ID | RBBIM_STYLE);

	CXTRegistryManager regManager;
	
	CString str;
	int iBand;
	for (iBand = 0; iBand < nCount; iBand++)
	{
		rebarCtrl.GetBandInfo(iBand, &info);

		str.Format(_T("%d.wID"), iBand);
		regManager.WriteProfileInt(lpszSection, str, info.wID);

		str.Format(_T("%d.cx"), iBand);
		regManager.WriteProfileInt(lpszSection, str, info.cx);

		str.Format(_T("%d.iIndex"), iBand);
		regManager.WriteProfileInt(lpszSection, str, iBand);

		str.Format(_T("%d.bBreak"), iBand);
		regManager.WriteProfileInt(lpszSection, str, ( ( info.fStyle & RBBS_BREAK ) == RBBS_BREAK ) );
	}

	SaveCustomBars(lpszSection);
}

void CXTReBar::LoadState(LPCTSTR lpszSection)
{
	LoadCustomBars(lpszSection);

	// get a reference to the rebar control.
	CXTReBarCtrl& rebarCtrl = GetReBarCtrl();

	// get the number of bands stored in the registry.
	CXTRegistryManager regManager;
	UINT nCount = rebarCtrl.GetBandCount( );
	
	XT_REBARBANDINFO info(RBBIM_SIZE | RBBIM_ID | RBBIM_STYLE);
	
	CString str;
	UINT iBand;
	for (iBand = 0; iBand < nCount; iBand++)
	{
		str.Format(_T("%d.wID"), iBand);
		UINT wID = regManager.GetProfileInt(lpszSection, str, 0);
		
		str.Format(_T("%d.cx"), iBand);
		UINT cx = regManager.GetProfileInt(lpszSection, str, 0);
		
		str.Format(_T("%d.iIndex"), iBand);
		int iSavedIndex = regManager.GetProfileInt(lpszSection, str, 0);
		
		str.Format(_T("%d.bBreak"), iBand);
		BOOL bBreak = regManager.GetProfileInt(lpszSection, str, 0);
		
		// if the band was moved, the index will not match, so get the 
		// actual index for this band from wID.
		int iActualIndex = rebarCtrl.IDToIndex(wID);

		// update the band info.
		rebarCtrl.GetBandInfo(iActualIndex, &info);
		info.cx     = cx;
		info.wID    = wID;
		info.fStyle = bBreak ? ( info.fStyle | RBBS_BREAK ) : ( info.fStyle & ~RBBS_BREAK );
		rebarCtrl.SetBandInfo(iActualIndex, &info);
		
		// if the band was moved, send it to its saved location.
		if (iActualIndex != iSavedIndex)
		{
			rebarCtrl.MoveBand(iActualIndex, iSavedIndex);
		}
	}
}

BOOL CXTReBar::AddBar(CWnd* pBar, REBARBANDINFO* pRBBI)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pBar != NULL);
	ASSERT(::IsWindow(pBar->m_hWnd));
	
	pRBBI->cbSize = sizeof(REBARBANDINFO);
	pRBBI->fMask |= RBBIM_CHILD;
	pRBBI->hwndChild = pBar->m_hWnd;

	CSize size;
	bool bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	if (!pBar->SendMessage(XTWM_ADDREBAR, bHorz, (LPARAM)(LPVOID)pRBBI))
	{
		if (CControlBar* pTemp = DYNAMIC_DOWNCAST(CControlBar, pBar))
		{
			size = pTemp->CalcFixedLayout(FALSE, bHorz);
			pRBBI->fMask |= RBBIM_CHILDSIZE;
			pRBBI->cxMinChild = size.cx;
			pRBBI->cyMinChild = size.cy;
		}
		else
		{
			CRect rect;
			pBar->GetWindowRect(&rect);
			size = rect.Size();
		}
	}
	
	//WINBUG: COMCTL32.DLL is off by 4 pixels in its sizing logic.  Whatever
	//  is specified as the minimum size, the system rebar will allow that band
	//  to be 4 actual pixels smaller!  That's why we add 4 to the size here.
	ASSERT(xtAfxData.iComCtlVersion != -1);
	if (xtAfxData.iComCtlVersion < VERSION_IE401)
	{
		pRBBI->cxMinChild += 4;
	}

	// make sure each band has a unique id.
    pRBBI->fMask |= RBBIM_ID;
	pRBBI->wID    = ++m_iBandCount;
	
	BOOL bResult = (BOOL)DefWindowProc(RB_INSERTBAND, (WPARAM)-1, (LPARAM)pRBBI);

	CFrameWnd* pFrameWnd = GetParentFrame();
	if (pFrameWnd != NULL) {
		pBar->SetOwner(pFrameWnd);
		pFrameWnd->DelayRecalcLayout();
	}

	return bResult;
}

BOOL CXTReBar::AddBar(CWnd* pBar, LPCTSTR pszText, CBitmap* pbmp, DWORD dwStyle)
{
	XT_REBARBANDINFO rbBand(RBBIM_STYLE);
	rbBand.fStyle = dwStyle;
	if (pszText != NULL)
	{
		rbBand.fMask |= RBBIM_TEXT;
		rbBand.lpText = const_cast<LPTSTR>(pszText);
	}
	if (pbmp != NULL)
	{
		rbBand.fMask |= RBBIM_BACKGROUND;
		rbBand.hbmBack = (HBITMAP)*pbmp;
	}

	return AddBar(pBar, &rbBand);
}

BOOL CXTReBar::AddBar(CWnd* pBar, COLORREF clrFore, COLORREF clrBack, LPCTSTR pszText, DWORD dwStyle)
{
	XT_REBARBANDINFO rbBand(RBBIM_STYLE | RBBIM_COLORS);
	rbBand.fStyle = dwStyle;
	rbBand.clrFore = clrFore;
	rbBand.clrBack = clrBack;
	if (pszText != NULL)
	{
		rbBand.fMask |= RBBIM_TEXT;
		rbBand.lpText = const_cast<LPTSTR>(pszText);
	}

    return AddBar(pBar, &rbBand);
}

CSize CXTReBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// the union of the band rectangles is the total bounding rect
	int nCount = DefWindowProc(RB_GETBANDCOUNT, 0, 0);
	XT_REBARBANDINFO rbBand;	
	int nTemp;

	// sync up hidden state of the bands
	for (nTemp = nCount; nTemp--; )
	{
		rbBand.fMask = RBBIM_CHILD|RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, nTemp, (LPARAM)&rbBand));
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, CWnd::FromHandlePermanent(rbBand.hwndChild));
		BOOL bWindowVisible;
		if (pBar != NULL)
			bWindowVisible = pBar->IsVisible();
		else
			bWindowVisible =  (::GetWindowLong(rbBand.hwndChild, GWL_STYLE) & WS_VISIBLE) != 0;
		BOOL bBandVisible = (rbBand.fStyle & RBBS_HIDDEN) == 0;
		if (bWindowVisible != bBandVisible)
			VERIFY(DefWindowProc(RB_SHOWBAND, nTemp, bWindowVisible));
	}

	// determine bounding rect of all visible bands
	CRect rectBound; rectBound.SetRectEmpty();
	for (nTemp = nCount; nTemp--; )
	{
		rbBand.fMask = RBBIM_STYLE;
		VERIFY(DefWindowProc(RB_GETBANDINFO, nTemp, (LPARAM)&rbBand));
		if ((rbBand.fStyle & RBBS_HIDDEN) == 0)
		{
			CRect rect;
			VERIFY(DefWindowProc(RB_GETRECT, nTemp, (LPARAM)&rect));
			rectBound |= rect;
		}
	}

	// add borders as part of bounding rect
	if (!rectBound.IsRectEmpty())
	{
		CRect rect; rect.SetRectEmpty();
		CalcInsideRect(rect, bHorz);
		rectBound.right -= rect.Width();
		rectBound.bottom -= rect.Height();
	}

	return CSize((bHorz && bStretch) ? 32767 : rectBound.Width(),
				 (!bHorz && bStretch) ? 32767 : rectBound.Height());
}

CSize CXTReBar::CalcDynamicLayout(int /*nLength*/, DWORD dwMode)
{
	if (dwMode & LM_HORZDOCK)
		ASSERT(dwMode & LM_HORZ);
	return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ);
}

BOOL CXTReBar::Create(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, UINT nID)
{
	ASSERT_VALID(pParentWnd);   // must have a parent
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

	// save the style
	m_dwStyle = (dwStyle & CBRS_ALL);
	if (nID == AFX_IDW_REBAR)
		m_dwStyle |= CBRS_HIDE_INPLACE;

	dwStyle &= ~CBRS_ALL;
	dwStyle |= CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE|RBS_VARHEIGHT;
	dwStyle |= dwCtrlStyle;

	// initialize common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_COOL_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));

	ASSERT(xtAfxData.iComCtlVersion != -1);

	// create the HWND
	CRect rect; rect.SetRectEmpty();
	if (!CWnd::Create(REBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	// Note: Parent must resize itself for control bar to be resized
	m_bInReBar = true;

	return TRUE;
}

void CXTReBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
	UpdateDialogControls(pTarget, bDisableIfNoHandler);
}

BOOL CXTReBar::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (!CXTControlBar::OnNcCreate(lpCreateStruct))
		return FALSE;

	// if the owner was set before the rebar was created, set it now
	if (m_hWndOwner != NULL)
		DefWindowProc(RB_SETPARENT, (WPARAM)m_hWndOwner, 0);

	return TRUE;
}

BOOL CXTReBar::OnEraseBkgnd(CDC* /*pDC*/)
{
	return (BOOL)Default();
}

void CXTReBar::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS* lpncsp)
{
	// calculate border space (will add to top/bottom, subtract from right/bottom)
	CRect rect; rect.SetRectEmpty();
	BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	CXTControlBar::CalcInsideRect(rect, bHorz);

	// adjust non-client area for border space
	lpncsp->rgrc[0].left += rect.left;
	lpncsp->rgrc[0].top += rect.top;
	lpncsp->rgrc[0].right += rect.right;
	lpncsp->rgrc[0].bottom += rect.bottom;
}

void CXTReBar::OnNcPaint()
{
	EraseNonClient();
}

void CXTReBar::OnPaint()
{
	Default();
}

LRESULT CXTReBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	
	HWND hWndChild = _xtAfxChildWindowFromPoint(m_hWnd, point);
	CWnd* pWnd = CWnd::FromHandlePermanent(hWndChild);
	if (pWnd == NULL)
		return CXTControlBar::OnToolHitTest(point, pTI);

	ASSERT(pWnd->m_hWnd == hWndChild);
	return pWnd->OnToolHitTest(point, pTI);
}

LRESULT CXTReBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// special handling for certain messages (forwarding to owner/parent)
	switch (message)
	{
	case WM_POPMESSAGESTRING:
	case WM_SETMESSAGESTRING:
		return GetOwner()->SendMessage(message, wParam, lParam);
	}
	return CXTControlBar::WindowProc(message, wParam, lParam);
}

LRESULT CXTReBar::OnIdealSizeChanged(WPARAM wParam, LPARAM lParam)
{
	HWND hwndChild = (HWND)wParam;
	CXTReBarCtrl& rebarCtrl = GetReBarCtrl();

	// get the number of bands found in the rebar.
	int nCount = rebarCtrl.GetBandCount();

	// find the band that holds this control and update its parameters
	int i;
	for (i = 0; i < nCount; i++)
	{
		XT_REBARBANDINFO bandInfo(RBBIM_CHILD | RBBIM_IDEALSIZE | RBBIM_CHILDSIZE);
		if (rebarCtrl.GetBandInfo(i, &bandInfo))
		{
			if (bandInfo.hwndChild == hwndChild)
			{
				bandInfo.fMask = RBBIM_IDEALSIZE | RBBIM_CHILDSIZE;
				bandInfo.cxIdeal = (UINT)lParam;
				rebarCtrl.SetBandInfo(i, &bandInfo);
				break;
			}
		}
		
	}
	return 0;
}

#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNAMIC(CXTReBar, CXTControlBar)


