// XTTabCtrlButtons.cpp : implementation of the CXTTabCtrlButtons class.
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
#include "XTDrawHelpers.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTColorRef.h"
#include "XTTabCtrlButtons.h"
#include "XTTabCtrl.h"

#define DB_LEFT        0x0002
#define DB_RIGHT       0x0004
#define DB_CROSS       0x0008

#define DB_ENABLED     0x0000
#define DB_DISABLED    0x0100
#define DB_PRESSED     0x1000
#define DB_OVER        0x4000

#define DB_ISENABLED(Style)     (!((Style)&DB_DISABLED))
#define DB_ISPRESSED(Style)     (((Style)&(DB_PRESSED))!=0)
#define DB_ISOVER(Style)        (((Style)&DB_OVER)==DB_OVER)

void CXTTabCtrlButton::SetEnable(BOOL bEnable)		
{
	if (!bEnable) m_wStyle |= DB_DISABLED; else m_wStyle &= ~DB_DISABLED;
}

void CXTTabCtrlButton::Draw (CDC* pDC, COLORREF clrButton)
{
	ASSERT (pDC != NULL);
    CRect rc (m_Rect);
	DWORD wStyle = m_wStyle;
	
	if (DB_ISENABLED(wStyle))
	{
		if (DB_ISPRESSED(wStyle))
			pDC->Draw3dRect(rc, RGB(0, 0, 0), GetSysColor(0x16));		
		else if (DB_ISOVER(wStyle))
			pDC->Draw3dRect(rc, GetSysColor(0x16), RGB(0, 0, 0));		
	
		if (DB_ISPRESSED(wStyle) ) 
		{
			rc.OffsetRect(1, 1);
		}
	}
	
	CXTPenDC pen(*pDC, clrButton);
	CXTBrushDC brush(*pDC, clrButton);
	if (wStyle & DB_CROSS)
	{
		pDC->MoveTo(rc.left + 3, rc.top + 3); pDC->LineTo(rc.right - 4, rc.bottom - 5);
		pDC->MoveTo(rc.left + 4, rc.top + 3); pDC->LineTo(rc.right - 3, rc.bottom - 5);
		
		pDC->MoveTo(rc.left + 3, rc.bottom  - 6); pDC->LineTo(rc.right - 4, rc.top + 2);
		pDC->MoveTo(rc.left + 4, rc.bottom  - 6); pDC->LineTo(rc.right - 3, rc.top + 2);
	}		
	else if (wStyle & DB_LEFT)
	{
		pDC->MoveTo(rc.right - 5, rc.top + 2); 
		pDC->LineTo(rc.right - 9, rc.top + 6);
		pDC->LineTo(rc.right - 5, rc.top + 10);
		pDC->LineTo(rc.right - 5, rc.top + 2);
		if (DB_ISENABLED(wStyle))
			pDC->FloodFill(rc.right - 6, rc.top + 6, clrButton);
		
	}
	else if (wStyle & DB_RIGHT)
	{
		pDC->MoveTo(rc.left + 4, rc.top + 2); 
		pDC->LineTo(rc.left + 8, rc.top + 6);
		pDC->LineTo(rc.left + 4, rc.top + 10);
		pDC->LineTo(rc.left + 4, rc.top + 2);
		
		if (DB_ISENABLED(wStyle))
			pDC->FloodFill(rc.left + 5, rc.top + 6, clrButton);
	}
}

DWORD CXTTabCtrlButton::Click (CXTTabCtrlButtons* pWnd, CPoint pt, BOOL bRepeat)
{
    if ( (::GetCapture() != NULL ) || !DB_ISENABLED(m_wStyle))
	{
		return FALSE;
	}

    pWnd->SetCapture();
    ASSERT(pWnd == CWnd::GetCapture());

    BOOL  bDown = FALSE, bClick = FALSE;
    CRect rcBtn(m_Rect);

	int nTimer = (bRepeat == FALSE)? 0: SetTimer(pWnd->GetSafeHwnd(), 1, 150, NULL);

    while ( TRUE )
    {		
		if (bRepeat && DB_ISENABLED(m_wStyle))
		{
			ASSERT(m_pTabCtrl);
			
			int nCount = m_pTabCtrl->GetItemCount();
			
			CRect rc;
			m_pTabCtrl->GetItemRect(-1, rc);
			TCHITTESTINFO hi = {{5, rc.CenterPoint().y}};
			int nFirst = m_pTabCtrl->HitTest(&hi);
			
			if (m_wStyle & DB_LEFT)
			{
				if (nFirst > 0)
					m_pTabCtrl->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nFirst - 1), 0);
			} 
			else if (nCount > 0)
			{
				m_pTabCtrl->SendMessage(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, nFirst + 1), 0);
			}

			m_pTabCtrl->Invalidate(FALSE);
			pWnd->Invalidate(FALSE);
		}
		
        BOOL bNewDown = ::PtInRect (rcBtn, pt) != 0;
        if ( bNewDown != bDown )
        {
            bDown = bNewDown;
			if (bDown) m_wStyle = m_wStyle | DB_PRESSED; else m_wStyle &= ~DB_PRESSED;

			pWnd->InvalidateRect (m_Rect, FALSE);
        }
        MSG msg;

		// assess current state
		// pump WM_PAINT first for better feedback
		while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
				break;
			DispatchMessage(&msg);
		}

        VERIFY(::GetMessage(&msg, NULL, 0, 0));

        if ( CWnd::GetCapture() != pWnd )
        {
            DispatchMessage (&msg);
            goto ExitLoop;
        }
        switch ( msg.message )
        {
            case WM_MOUSEMOVE:
                pt = CPoint((short signed)LOWORD(msg.lParam), (short signed)HIWORD(msg.lParam));
                break;

            case WM_LBUTTONUP:
                bClick = bDown;
                goto ExitLoop;

            case WM_KEYDOWN:
                if ( msg.wParam != VK_ESCAPE )
                    break;
            case WM_CANCELMODE:
            case WM_RBUTTONDOWN:
                goto ExitLoop;

			default:
                DispatchMessage (&msg);
                break;
        }
    }
ExitLoop:
    ReleaseCapture();

	if (nTimer) KillTimer(*pWnd, nTimer);

	m_wStyle &= ~DB_PRESSED;
	
	CheckForMouseOver (pWnd, pt);
	pWnd->InvalidateRect (m_Rect, FALSE);

    return bClick;
}

void CXTTabCtrlButton::CheckForMouseOver (CWnd* pWnd, CPoint pt)
{
	static BOOL bTimer = FALSE;
	
	if (!PtInRect (pt) && DB_ISOVER(m_wStyle))
	{
		m_wStyle &= ~DB_OVER;
		pWnd->InvalidateRect (m_Rect, FALSE);
	} 

	if (!DB_ISOVER(m_wStyle) && PtInRect (pt) && DB_ISENABLED(m_wStyle) )
	{
		TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT), TME_LEAVE, *pWnd, 0};
		_TrackMouseEvent(&tme);
		m_wStyle |= DB_OVER;

		pWnd->InvalidateRect (m_Rect, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrlButtons

CXTTabCtrlButtons::CXTTabCtrlButtons()
	: m_btnClose(DB_CROSS), m_btnLeft(DB_LEFT), m_btnRight(DB_RIGHT)
{
	m_dwFlags = XT_SHOW_ARROWS;
	m_bShow = FALSE; 
	m_hwndSubclassed = 0;
	m_hwndPrimary = 0; 
}

CXTTabCtrlButtons::~CXTTabCtrlButtons()
{
}

BEGIN_MESSAGE_MAP(CXTTabCtrlButtons, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

void CXTTabCtrlButtons::OnPaint()
{
	CPaintDC dcPaint(this);
	CXTClientRect rc(m_pTabCtrl);
	CXTClientRect rcClient(this);
	CXTBufferDC dc(dcPaint, rc);

	int nCount = m_pTabCtrl->GetItemCount();
	if (!m_bShow || nCount == 0)
	{
		CWnd::DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);
		return;
	}

	CRect rcItem;
	m_pTabCtrl->GetItemRect(-1, &rcItem);

	
	dc.FillSolidRect(rcClient, CXTColorRef::GetColor(XPCOLOR_TAB_INACTIVE_BACK));
	if ((m_pTabCtrl->GetStyle() & TCS_BOTTOM) != 0)
	{
		dc.FillSolidRect(0, 0, rcClient.right, 1, xtAfxData.clrBtnText);

	} else
	{
		dc.FillSolidRect(0, rcClient.bottom - 1, rcClient.right, 1, xtAfxData.clr3DHilight);
	}
	COLORREF clrText = CXTColorRef::GetColor(XPCOLOR_TAB_INACTIVE_TEXT);
	
	int nTop = (rcItem.Height() - 13) / 2;
	int nRight = rcClient.right - 5;
	
	if (m_dwFlags & XT_SHOW_CLOSE)
	{	
		m_btnClose.SetRect(CRect(nRight - 14, nTop, nRight, nTop + 15));
		m_btnClose.Draw(&dc, clrText);
		nRight -= 14;
	}
	else
		m_btnClose.SetRect(CXTEmptyRect());

	if (m_dwFlags & XT_SHOW_ARROWS)
	{
		ASSERT(m_pTabCtrl);
		CXTClientRect rcClient(m_pTabCtrl);
		BOOL bEnableRight = FALSE, bEnableLeft = FALSE;
		if (nCount > 0)
		{
			CRect rcItem;
			m_pTabCtrl->GetItemRect(nCount - 1, &rcItem);
			bEnableRight = rcItem.right > rc.Width() - GetWidth();

			m_pTabCtrl->GetItemRect(0, &rcItem);
			bEnableLeft = rcItem.left < 0;
		}
		
		m_btnRight.SetRect(CRect(nRight - 14, nTop, nRight, nTop + 15));
		m_btnRight.SetEnable(bEnableRight);
		m_btnRight.Draw(&dc, clrText);

		m_btnLeft.SetRect(CRect(nRight - 28, nTop, nRight -14, nTop + 15));
		m_btnLeft.SetEnable(bEnableLeft);
		m_btnLeft.Draw(&dc, clrText);
	}
	else 
	{
		m_btnRight.SetRect(CXTEmptyRect());
		m_btnLeft.SetRect(CXTEmptyRect());
	}
}
BOOL CXTTabCtrlButtons::OnEraseBkgnd(CDC* )
{
	return TRUE;
}

int CXTTabCtrlButtons::GetWidth()
{
	return 10 + (m_dwFlags & XT_SHOW_CLOSE? 14: 0) + (m_dwFlags & XT_SHOW_ARROWS? 28: 0);
}

BOOL CXTTabCtrlButtons::Create(CTabCtrl* pParent)
{
	m_pTabCtrl = m_btnRight.m_pTabCtrl = m_btnClose.m_pTabCtrl = m_btnLeft.m_pTabCtrl = pParent;

	return TRUE;
}

CRect CXTTabCtrlButtons::AdjustRect()
{
	CRect rcItem;
	m_pTabCtrl->GetItemRect(-1, &rcItem);

	CXTClientRect rc(m_pTabCtrl);
	rc.left = rc.right - GetWidth();
	rc.bottom = rcItem.bottom + 1;
	
	rc.top = rcItem.top + ((m_pTabCtrl->GetStyle() & TCS_BOTTOM) != 0? -2: 0);
	
	return rc;
}

void CXTTabCtrlButtons::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bShow)
	{
		if (m_btnClose.PtInRect(point) && m_btnClose.Click(this, point))
		{
			m_pTabCtrl->SendMessage( WM_COMMAND, XT_IDC_TAB_CLOSE );
		}
		else if (m_btnLeft.PtInRect(point))
		{
			m_btnLeft.Click(this, point, TRUE);
		}
		else if (m_btnRight.PtInRect(point))
		{
			m_btnRight.Click(this, point, TRUE);
		}
	}
	else
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
}

BOOL CXTTabCtrlButtons::ShowButtons()
{
	return m_bShow;
}
void CXTTabCtrlButtons::ShowButtons(BOOL bShow)
{
	if (m_bShow != bShow)
	{
		m_bShow = bShow;

		if (m_hWnd && !m_bShow) ShowWindow(SW_HIDE);
	}
	if (m_bShow && m_hWnd)
	{
		Refresh();
	}
}

void CXTTabCtrlButtons::Refresh()
{
	if (!m_hWnd) return;
	
	if (m_bShow)
	{
		CRect rc = AdjustRect();
		SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(),
			m_pTabCtrl->GetItemCount() > 0? SWP_SHOWWINDOW: SWP_HIDEWINDOW);
		
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		OnMouseMove(0, point);
	}
	Invalidate(FALSE);
}

void CXTTabCtrlButtons::OnMouseMove(UINT , CPoint point)
{
	if (m_bShow)
	{
		m_btnClose.CheckForMouseOver(this, point);
		m_btnLeft.CheckForMouseOver(this, point);
		m_btnRight.CheckForMouseOver(this, point);
	}
}

void CXTTabCtrlButtons::OnMouseLeave()
{
	OnMouseMove(0,CPoint(-1, -1));
}

void CXTTabCtrlButtons::SubclassTabButtons(CWnd* pWnd)
{
	if (pWnd && pWnd->GetSafeHwnd() && m_hwndSubclassed != pWnd->GetSafeHwnd())
	{
		m_hwndSubclassed = pWnd->GetSafeHwnd();
		
		if (m_hWnd)
		{	
			HWND hWnd = UnsubclassWindow();
			if (hWnd == m_hwndPrimary)
			{
				::DestroyWindow(hWnd);
			}
		}
		SubclassWindow(*pWnd);
	} 
	else
	{
		if (!m_hWnd)
		{		
			VERIFY(CWnd::Create(AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW)),
				0, WS_CHILD|WS_VISIBLE, CXTEmptyRect(), m_pTabCtrl, 0));
			m_hwndPrimary = m_hWnd;
		}
	}
}

void CXTTabCtrlButtons::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CWnd::OnWindowPosChanged(lpwndpos);
	static int bOnWindowPosChanged = FALSE;

	if (m_bShow && !bOnWindowPosChanged)
	{
		if (m_pTabCtrl->GetItemCount() == 0)
			return;

		bOnWindowPosChanged = TRUE;
		CRect rc = AdjustRect();
		SetWindowPos(&CWnd::wndTop, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW );
		bOnWindowPosChanged = FALSE;
	}
}

void CXTTabCtrlButtons::SetButtonStyle(DWORD dwFlags)
{
    m_dwFlags = dwFlags;
}
