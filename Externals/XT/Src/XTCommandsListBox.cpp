// XTCommandsListBox.cpp : implementation of the CXTCommandsListBox class.
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
#include "XTCommandsListBox.h"
#include "XTDelegate.h"
#include "XTCustomizeAPI.h"
#include "XTMemDC.h"
#include "XTSkins.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTCommandsListBox
/////////////////////////////////////////////////////////////////////////////

CXTCommandsListBox::CXTCommandsListBox(CXTCommandsListBox::ISite* site)
: m_site(site)
, m_sizeIcon(22,22)
{
}

CXTCommandsListBox::~CXTCommandsListBox()
{

}

BEGIN_MESSAGE_MAP(CXTCommandsListBox, CListBox)
	//{{AFX_MSG_MAP(CXTCommandsListBox)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTCommandsListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	if ( IXTCommandsListBoxProxy* proxy = IXTSkin::GetInstance( )->GetCommandsListBoxProxy( ) )
	{
		proxy->OnDrawItem(lpDIS, this);
		return;
	}

	CDC*   pDC	      = CDC::FromHandle(lpDIS->hDC);
	CRect  rcItem     = lpDIS->rcItem;
	int    pos        = int(lpDIS->itemData);
	UINT   nCmdID     = m_site->GetItemCommand(pos);
	
	int nSavedDC = pDC->SaveDC();

	// Fill the item's background color first...
	pDC->FillSolidRect(&rcItem, xtAfxData.clr3DFace);

	CRect rcHilite(rcItem);
	rcHilite.left = m_sizeIcon.cx+1;
	
	COLORREF crWindowText = xtAfxData.clrWindowText;
	pDC->SelectObject(GetFont());
	
	// Paint the color item in the color requested
	if( lpDIS->itemState & ODS_SELECTED )
	{
		CBrush brush( xtAfxData.clrHighlight );
		pDC->FillRect(rcHilite, &brush);
		crWindowText = xtAfxData.clrHighlightText;
		brush.DeleteObject();
	}

	pDC->SetTextColor(crWindowText);

	rcHilite.InflateRect(-2, -2);

	// If item has focus, draw the focus rect.
	if ((lpDIS->itemAction & ODA_FOCUS) && (lpDIS->itemState & ODS_FOCUS))
		pDC->DrawFocusRect(&rcHilite);

	pDC->SetBkMode(TRANSPARENT);

	// Get and display item text.
	CString strItemText;
	GetText(lpDIS->itemID, strItemText);
	
	// Calculate the text size first.
	CRect rcText(lpDIS->rcItem);
	pDC->DrawText(strItemText, -1, &rcText,
		DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_CALCRECT);

	// Make necessary adjustments to the text size.
	rcText.top    = rcItem.top;
	rcText.bottom = rcItem.top + rcItem.Height();
	rcText.left	  = m_sizeIcon.cx + 6;
	rcText.right  = lpDIS->rcItem.right-m_sizeIcon.cx;
	
	// Draw the text.
	pDC->DrawText(strItemText, -1, &rcText,
		DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	// popup menu	
	if (nCmdID == 0)
	{
		CRect rcIcon(lpDIS->rcItem);
		rcIcon.left = rcIcon.right-11;

		CPen pen(PS_SOLID, 1, crWindowText);
		pDC->SelectObject(&pen);

		int x = rcIcon.left;
		int y = rcIcon.top +6;

		pDC->MoveTo(x, y  );
		pDC->LineTo(x, y+9);
		pDC->MoveTo(x+1, y+1);
		pDC->LineTo(x+1, y+8);
		pDC->MoveTo(x+2, y+2);
		pDC->LineTo(x+2, y+7);
		pDC->MoveTo(x+3, y+3);
		pDC->LineTo(x+3, y+6);
		pDC->MoveTo(x+4, y+4);
		pDC->LineTo(x+4, y+5);
	}
	else 
	{
		HICON hCommandIcon, hDisabledIcon, hHotIcon;
		if (m_site->GetCommandIcon(nCmdID, hCommandIcon, hDisabledIcon, hHotIcon))
		{
			CXTIconHandle icon(hCommandIcon);
			CXTIconHandle iconDisabled(hDisabledIcon);
			CXTIconHandle iconHot(hHotIcon);

			CRect rcIcon(lpDIS->rcItem);
			rcIcon.right = m_sizeIcon.cx;
			
			if (lpDIS->itemState & ODS_SELECTED)
			{
				// if a "hot" image list is defined, use it instead of normal list.
				if ( hHotIcon ) {
					icon = hHotIcon;
				}
				
				pDC->Draw3dRect(rcIcon, xtAfxData.clr3DHilight,
					xtAfxData.clr3DShadow);
			}		

			CSize iconExtent = icon.GetExtent();
			CSize desiredExtent = rcIcon.Size();
			if (desiredExtent.cx > xtAfxData.cxSmIcon ||
				desiredExtent.cy > xtAfxData.cySmIcon)
			{
				desiredExtent = CSize(xtAfxData.cxSmIcon, xtAfxData.cySmIcon);
			}
			if (iconExtent.cx > desiredExtent.cx || 
				iconExtent.cy > desiredExtent.cy)
			{
				icon = icon.ScaleToFit(desiredExtent);
				iconExtent = icon.GetExtent();
			}

			CPoint ptIcon;
			ptIcon.x = (rcIcon.left + rcIcon.right - iconExtent.cx) / 2;
			ptIcon.y = (rcIcon.top + rcIcon.bottom - iconExtent.cy) / 2;
			
			pDC->DrawState(ptIcon, iconExtent, icon,
				DST_ICON | DSS_NORMAL, (HBRUSH)NULL );		
		}
	}

	pDC->RestoreDC(nSavedDC);
}

void CXTCommandsListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = m_sizeIcon.cy;
}

void CXTCommandsListBox::OnPaint() 
{
	if ( IXTCommandsListBoxProxy* proxy = IXTSkin::GetInstance( )->GetCommandsListBoxProxy( ) )
	{
		proxy->OnPaint(this);
		return;
	}

	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to reduce screen flicker.
	CXTMemDC memDC(&dc, rectClient, xtAfxData.clr3DFace);

	// Let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

BOOL CXTCommandsListBox::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CXTCommandsListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bOutside;
	int nIndex = ( int )ItemFromPoint( point, bOutside );
	int posDrag = -1;
	if (!bOutside)
	{
		if ( SetCurSel( nIndex ) != LB_ERR )
		{
			// Already selected, start drag operation
			posDrag = ( int )GetItemData( nIndex );
			
			// since we are eating the message, if the selection has changed, notify the owner.
			CWnd* pWndOwner = GetOwner();
			if ( pWndOwner )
			{
				SetFocus();
				
				pWndOwner->PostMessage( WM_COMMAND, MAKEWPARAM( GetDlgCtrlID( ),
					LBN_SELCHANGE ), ( LPARAM )m_hWnd );
			}
		}
		else
		{
			// process as a click first
			CListBox::OnLButtonDown(nFlags, point);
			
			// DragDetect freezes for a bit so force window update now
			RedrawWindow(NULL, NULL, 
				RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);

			// check to see if drag has started
			if (::DragDetect(m_hWnd, point))
			{
				posDrag = ( int )GetItemData( nIndex );
			}
		}
	}
	if (posDrag >= 0)
	{
		m_site->DragNDrop(posDrag);
	}
}
