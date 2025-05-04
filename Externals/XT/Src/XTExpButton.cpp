// XTExpButton.cpp : implementation file
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
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResource.h"
#include "XTToolBarCtrl.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTButton.h"
#include "XTExpMenuWnd.h"
#include "XTExpButton.h"
#include "XTCoolMenu.h"
#include "XTHelpers.h"
#include "XTWndShadow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable : 4310) // disable level 4 warning

IMPLEMENT_DYNAMIC(CXTExpButton, CXTButton)

BEGIN_MESSAGE_MAP(CXTExpButton, CXTButton)
	//{{AFX_MSG_MAP(CXTExpButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CANCELMODE()
	ON_WM_PAINT()
	ON_COMMAND(XT_IDC_CUSTOMIZE, OnCustomize)
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(XTWM_POPUPCLOSED, OnPopupClosed)
	ON_UPDATE_COMMAND_UI_REFLECT(OnUpdateCustomize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTExpButton

CXTExpButton::CXTExpButton()
: m_pParent(0)
, m_popup(0)
{
}

CXTExpButton::~CXTExpButton()
{
}

BOOL CXTExpButton::Create(CXTToolBar* pParent, UINT nID)
{
	CRect rc;
	rc.SetRectEmpty();
	if (!CXTButton::Create(_T(""), 
		 WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | BS_PUSHBUTTON | BS_FLAT | BS_OWNERDRAW, 
		 rc, pParent, nID))
	{
		return false;
	}
	m_pParent = pParent;
	return true;
}

void CXTExpButton::DrawBits(CDC* pDC, const CRect& rcItem, UINT nState, int cx, int cy, bool isChevron, const WORD* bits)
{
	CBitmap bitmap;
	bitmap.CreateBitmap(8, cy, 1, 1, bits);

	// Set the draw state flags.
	DWORD dwFlags = DST_BITMAP;
	if( nState & ODS_DISABLED )
	{
		dwFlags |= DSS_DISABLED;
	}
	else
	{
		dwFlags |= DSS_NORMAL;
	}

	// compute the origin
	CPoint pt;
	if (m_pParent->IsChevronHorizontal())
	{
		pt.x = (rcItem.Width() - cx) / 2;
		pt.y = (isChevron)
				? (rcItem.top + 2)
				: (rcItem.bottom - cy - 2);
	}
	else
	{
		pt.y = (rcItem.Height() - cy) / 2;
		pt.x = (isChevron)
				? (rcItem.right - cx - 2)
				: (rcItem.left + 2);

	}
	if ((nState & ODS_SELECTED) && !xtAfxData.bXPMode)
	{
		pt.x++;
		pt.y++;
	}

	// draw the icon associated with this button.
	pDC->DrawState(pt, CSize(cx, cy), bitmap, dwFlags, (HBRUSH)NULL );
}

void CXTExpButton::DrawButtonBitmap(CDC* pDC, UINT nState, CRect& rcItem)
{
	if (m_pParent->IsChevronHorizontal())
	{
		if (m_pParent->IsExpansionChevron())
		{
			// Horizontal chevron
			// 0011 0011  0 33
			// 1001 1001  1 99
			// 1100 1100  2 CC
			// 1001 1001  3 99
			// 0011 0011  4 33
			static const WORD bitsHorz[5] = {0x33, 0x99, 0xCC, 0x99, 0x33};
			DrawBits(pDC, rcItem, nState, 8, 5, true, bitsHorz);
		}

		if (m_pParent->IsExpansionCustomizes())
		{		
			// down arrow
			// 0000 0111  0 07
			// 1000 1111  1 8F
			// 1101 1111  2 DF
			static const WORD bitsDown[3] = {0x07, 0x8F, 0xDF};
			DrawBits(pDC, rcItem, nState, 5, 3, false, bitsDown);
		}
	}
	else
	{
		if (m_pParent->IsExpansionChevron())
		{
			// Vertical chevron
			// 1110 1110 0 EE
			// 1110 0100 1 E4
			// 1111 0001 2 F1
			// 1111 1011 3 FB
			// 1110 1110 4 EE
			// 1110 0100 5 E4
			// 1111 0001 6 F1
			// 1111 1011 7 FB
			static const WORD bitsVert[8] = {0xEE, 0xE4, 0xF1, 0xFB, 0xEE, 0xE4, 0xF1, 0xFB};
			DrawBits(pDC, rcItem, nState, 8, 8, true, bitsVert);
		}

		if (m_pParent->IsExpansionCustomizes())
		{		
			// Right arrow
			// 1101 1111  0 DF
			// 1001 1111  1 9F
			// 0001 1111  2 1F
			// 1001 1111  3 9F
			// 1101 1111  4 DF
			static const WORD bitsRight[5] = {0xDF, 0x9F, 0x1F, 0x9F, 0xDF};
			DrawBits(pDC, rcItem, nState, 3, 5, false, bitsRight);
		}
	}
}

void CXTExpButton::PostNcDestroy()
{
	delete this;
}


DWORD CXTExpButton::GetXButtonStyle()
{
	DWORD style = CXTButton::GetXButtonStyle() & ~DWORD(BS_XT_XPFLAT | BS_XT_FLAT);

	style |= (xtAfxData.bXPMode) ? BS_XT_XPFLAT : BS_XT_FLAT;

	return style;
}

COLORREF CXTExpButton::GetColorFace()
{
	return m_pParent->GetBackgroundColor();
}


void CXTExpButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXTButton::OnLButtonDown(nFlags, point);
	if (!m_popup)
	{
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);

		// validate visible part
		CWnd* pParent = GetParent();
		while (pParent)
		{
			CRect rectParent;
			pParent->GetClientRect(&rectParent);
			pParent->ClientToScreen(&rectParent);
			rc &= rectParent;
			pParent = pParent->GetParent();
		}

		// outline is drawn inside the rect so use adjusted right/bottom
		// borders for proper align of the popup
		///rc.right --;
		//rc.bottom--;
		m_popup = new CXTExpMenuWnd(this, XTWM_POPUPCLOSED);
		if (!m_popup->Create(rc, m_pParent))
		{
			// PostNcDestroy is called automatically
			m_popup = 0;
		}
		RedrawWindow();
	}
}

void CXTExpButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	CXTButton::OnLButtonUp(nFlags, point);
}

void CXTExpButton::OnCancelMode()
{
	CXTButton::OnCancelMode();

	// close popup if active
	if (m_popup)
	{
		m_popup->DestroyWindow();
	}
}

void CXTExpButton::OnPopupClosed()
{
	m_popup = 0;
	Invalidate();
}

void CXTExpButton::OnPaint()
{
	if (m_popup)
	{
		if (xtAfxData.bXPMode)
		{
			CPaintDC dc(this); // device context for painting
			CDC* pDC = &dc;

			CRect rc;
			GetClientRect(&rc);
			pDC->FillSolidRect(rc, m_pParent->GetBackgroundColor());
			DrawButtonBitmap(pDC, 0, rc);
			CPen pen(PS_SOLID, 1, xtAfxData.clrXPMenuBorder);
			CXTContextObjectHandler<CPen> penHandler(pDC, &pen);
			pDC->MoveTo(rc.left, rc.top);
			pDC->LineTo(rc.right-1, rc.top);
			pDC->LineTo(rc.right-1, rc.bottom - 1);
			pDC->LineTo(rc.left, rc.bottom  - 1);
			pDC->LineTo(rc.left, rc.top);

			if (xtAfxData.bMenuShadows)
			{	
				//rc.OffsetRect(4, 4);
				//CXTWndShadow::ComputeShadow(pDC, rc, 100);
			}
		}
		else
		{
			CXTButton::OnPaint();
		}
	}
	else
	{
		CXTButton::OnPaint();
	}
}

void CXTExpButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (m_popup)
	{
		lpDrawItemStruct->itemState |= ODS_SELECTED;
	}
	CXTButton::DrawItem(lpDrawItemStruct);
}

void CXTExpButton::OnCustomize() 
{
	m_pParent->Customize();
}

void CXTExpButton::OnUpdateCustomize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pParent->IsExpansionChevron() ||
				   m_pParent->IsExpansionCustomizes());
}
