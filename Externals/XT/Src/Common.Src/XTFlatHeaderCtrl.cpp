// XTFlatHeaderCtrl.cpp : implementation of the CXTFlatHeaderCtrl class.
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
#include "XTFlatHeaderCtrl.h"
#include "XTMemDC.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTHeaderCtrl

CXTHeaderCtrl::CXTHeaderCtrl()
{
	m_iMinSize  = 0;
	m_bRTL      = DetermineRTL();
	m_bAutoSize = false;
}

CXTHeaderCtrl::~CXTHeaderCtrl()
{

}

BEGIN_MESSAGE_MAP(CXTHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CXTHeaderCtrl)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT_EX(HDN_ITEMCHANGING, OnItemchanging)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(CXTHeaderCtrl, CHeaderCtrl)

/////////////////////////////////////////////////////////////////////////////
// CXTHeaderCtrl message handlers

void CXTHeaderCtrl::ApplyFieldWidths(int iNewWidth)
{
	CListCtrl* pListCtrl = ( CListCtrl* )GetParent();
	ASSERT_VALID( pListCtrl );

	int iCount = GetItemCount();

	int iWidth = iNewWidth/(iCount - m_arFrozenCols.GetSize());

	int iItem;
	for ( iItem = 0; iItem < iCount; iItem++ )
	{
		if (IsColFrozen( iItem ))
			continue;

		if ( iWidth > m_iMinSize )
			pListCtrl->SetColumnWidth( iItem, iWidth );
		
		iNewWidth -= iWidth;
	}
}

void CXTHeaderCtrl::FitFieldWidths(int iNewWidth)
{
	if( iNewWidth==0 )
	{
		CRect rc;
		GetWindowRect( &rc );
		iNewWidth = rc.Width() - ::GetSystemMetrics( SM_CXVSCROLL ) - GetFrozenColWidth();
	}

	ApplyFieldWidths( iNewWidth );
}

void CXTHeaderCtrl::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if ( m_bAutoSize )
	{
		int iCount = GetItemCount();
		
		if ( iCount > 0 )
		{
			// get the current window size
			CRect rc;
			GetWindowRect( &rc );
			
			// see if the window size is changing
			if( rc.Width()!=lpwndpos->cx )
			{
				// window size changing, so resize columns to fit
				int iScrollBarWidth = ::GetSystemMetrics(SM_CXVSCROLL);
				int iWidth = lpwndpos->cx - iScrollBarWidth - GetFrozenColWidth();
				
				FitFieldWidths( iWidth );
			}
		}
	}
	else
	{
		CHeaderCtrl::OnWindowPosChanging(lpwndpos);
	}
}

void CXTHeaderCtrl::EnableAutoSize(bool bEnable/*=true*/)
{
	m_bAutoSize = bEnable;
}

void CXTHeaderCtrl::FreezeColumn(int iCol)
{
	m_arFrozenCols.Add(iCol);
}

void CXTHeaderCtrl::ThawColumn(int iCol)
{
	int i;
	for (i = 0; i < m_arFrozenCols.GetSize(); ++i)
	{
		if (m_arFrozenCols.GetAt(i) == (UINT)iCol)
		{
			m_arFrozenCols.RemoveAt(i);
			break;
		}
	}
}

BOOL CXTHeaderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ( m_arFrozenCols.GetSize() )
	{
		CPoint pt;
		::GetCursorPos(&pt);

		CPoint ptClient = pt;
		ScreenToClient(&ptClient);

		HDHITTESTINFO hti;
		::ZeroMemory(&hti, sizeof(hti));
		hti.pt.x = ptClient.x;
		hti.pt.y = ptClient.y;

		int nIndex = (int)::SendMessage(GetSafeHwnd(),
			HDM_HITTEST, 0L, (LPARAM)&hti);

		if ( nIndex > -1 )
		{
			// if we are over one of the frozen columns, we can stop
			if ( IsColFrozen( nIndex ) )
			{
				return TRUE;
			}

			else
			{
				// determine if the current index is adjacent to a frozen column index.
				// if columns are resized by dragging to the right, test for the frozen column on the left.
				// if columns are resized by dragging to the left, test for the frozen column on the right.

				int iAdjIndex = nIndex + (m_bRTL ? 1 : -1);
				if ((iAdjIndex > -1) && IsColFrozen( iAdjIndex ))
				{
					CRect r;
					Header_GetItemRect(m_hWnd, nIndex, &r);
					int nMidPoint = (r.left + (r.Width()/2));
					
					// if columns resize to the right and the point is the right half of the header item...
					if (!m_bRTL && (ptClient.x <= nMidPoint))
					{
						return TRUE;
					}
					
					// if columns resize to the left and the point is the left half of the header item...
					else if (m_bRTL && (ptClient.x >= nMidPoint))
					{
						return TRUE;
					}
				}
			}
		}
	}

	return CHeaderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CXTHeaderCtrl::OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY* pHDN = ( HD_NOTIFY* )pNMHDR;
	ASSERT( pHDN != NULL );

	LRESULT lResult = FALSE;

    // if sizing is disabled for this column, keep the user from resizing
	if ( m_arFrozenCols.GetSize() && IsColFrozen( (UINT)pHDN->iItem) )
		lResult = TRUE;

    // if the column is smaller than the minimum size, keep the user from resizing
    if (pHDN->pitem->mask & HDI_WIDTH && pHDN->pitem->cxy < m_iMinSize) 
	     lResult = TRUE;

	*pResult = lResult;

	return *pResult;
}

bool CXTHeaderCtrl::IsColFrozen(int iCol)
{
	int iIndex;
	for (iIndex = 0; iIndex < m_arFrozenCols.GetSize(); ++iIndex )
	{
		if ( m_arFrozenCols.GetAt(iIndex) == (UINT)iCol )
		{
			return true;
		}
	}
	return false;
}

bool CXTHeaderCtrl::DetermineRTL()
{
	CWindowDC dc(NULL);
	
	// determine if columns are resized by dragging them right (most languages) or
	// left (RTL languages like Arabic & Hebrew).
	
	UINT nAlign = dc.GetTextAlign();
	ASSERT(nAlign != GDI_ERROR);
	
	// will only be true for RTL languages, text is laid out right to left and 
	// columns resize to the left
	if ((nAlign != GDI_ERROR) && (nAlign & TA_RTLREADING)) {
		return true;
	}

	return false;
}

int CXTHeaderCtrl::GetFrozenColWidth()
{
	CListCtrl* pListCtrl = ( CListCtrl* )GetParent();
	ASSERT_VALID( pListCtrl );

	int iFrozenWidth = 0;

	int iIndex;
	for (iIndex = 0; iIndex < m_arFrozenCols.GetSize(); ++iIndex )
	{
		int iCol = m_arFrozenCols.GetAt(iIndex);
		iFrozenWidth += pListCtrl->GetColumnWidth(iCol);
	}

	return iFrozenWidth;
}

void CXTHeaderCtrl::SetMinSize(int iMinSize)
{
	m_iMinSize = iMinSize;
}

/////////////////////////////////////////////////////////////////////////////
// CXTFlatHeaderCtrl
/////////////////////////////////////////////////////////////////////////////
CXTFlatHeaderCtrl::CXTFlatHeaderCtrl()
{
	m_bEnableMenus = TRUE;
    m_bLBtnDown    = FALSE;
    m_bAscending   = -1;
    m_bSortArrow   = FALSE;
    m_pt           = CPoint(0,0);
    m_nSortedCol   = -1;
    m_nOffset      = 6;
    m_nPos         = 0;
    m_popupMenuID  = 0;
	m_bInitControl = true;
}

CXTFlatHeaderCtrl::~CXTFlatHeaderCtrl()
{

}

IMPLEMENT_DYNAMIC(CXTFlatHeaderCtrl, CXTHeaderCtrl)

BEGIN_MESSAGE_MAP(CXTFlatHeaderCtrl, CXTHeaderCtrl)
	//{{AFX_MSG_MAP(CXTFlatHeaderCtrl)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_COMMAND(XT_IDC_SORTASC, OnSortAsc)
	ON_COMMAND(XT_IDC_SORTDSC, OnSortDsc)
	ON_COMMAND(XT_IDC_ALIGNLEFT, OnAlignLeft)
	ON_COMMAND(XT_IDC_ALIGNCENTER, OnAlignCenter)
	ON_COMMAND(XT_IDC_ALIGNRIGHT, OnAlignRight)
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

void CXTFlatHeaderCtrl::InitializeHeader(BOOL bBoldFont)
{
	SetFont(bBoldFont ? &xtAfxData.fontBold : &xtAfxData.font);
	Invalidate();
}

int CXTFlatHeaderCtrl::SetSortImage(int nCol, BOOL bAsc)
{
	int nPrevCol = m_nSortedCol;
	
	m_nSortedCol = nCol;
	m_bAscending = bAsc;
	
	// Change the item to owner drawn
	HD_ITEM hditem;
	
	hditem.mask = HDI_BITMAP | HDI_FORMAT;
	GetItem( nCol, &hditem );
	if (hditem.hbm == NULL)
	{
		hditem.fmt |= HDF_OWNERDRAW;
		SetItem( nCol, &hditem );
		Invalidate();
	}
	
	return nPrevCol;
}

void CXTFlatHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);

	// define some temporary variables.
	CDC*	pDC			= CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect	rcItem		= lpDrawItemStruct->rcItem;
	CRect	rcIcon		= lpDrawItemStruct->rcItem;
    int 	nState		= lpDrawItemStruct->itemState;
	int		nItemID		= lpDrawItemStruct->itemID;

	CWnd* pParentWnd = GetParent();
	BOOL bNoSortHeader = ((pParentWnd->GetStyle() & LVS_NOSORTHEADER) == LVS_NOSORTHEADER);

	// save the current device context so we
	// can restore it when we lose scope.
	int nSavedDC = pDC->SaveDC();

	// Paint the background.
	pDC->FillSolidRect(rcItem, xtAfxData.clr3DFace);
	pDC->SetBkMode(TRANSPARENT);
	
	// Set clipping region to limit drawing within column
	CRgn rgn;
	rgn.CreateRectRgnIndirect( &rcItem );
	pDC->SelectObject( &rgn );
	rgn.DeleteObject();

	// Get the column text and format
	TCHAR buf[256];
	HD_ITEM hditem;
	
	hditem.mask = HDI_TEXT | HDI_FORMAT;
	hditem.pszText = buf;
	hditem.cchTextMax = 255;
	
	GetItem( nItemID, &hditem );
	
	// Determine format for drawing column label
	UINT uFormat = DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP 
		| DT_VCENTER | DT_END_ELLIPSIS ;
	
	if( hditem.fmt & HDF_CENTER) {
		uFormat |= DT_CENTER;
	}
	else if( hditem.fmt & HDF_RIGHT) {
		uFormat |= DT_RIGHT;
	}
	else {
		uFormat |= DT_LEFT;
	}
	
	// Adjust the rect if the mouse button is pressed on it
	if (nState == ODS_SELECTED && !bNoSortHeader)
	{
		rcItem.left++;
		rcItem.top += 2;
		rcItem.right++;
	}

	// Adjust the rect further if Sort arrow is to be displayed
	if (nItemID == m_nSortedCol && m_bSortArrow) {
		rcItem.right -= 3 * m_nOffset;
	}
	
	rcItem.left += m_nOffset;
	rcItem.right -= m_nOffset;

	// Draw column label
	if( rcItem.left < rcItem.right )
	{
		pDC->SelectObject(GetFont());
		pDC->DrawText(buf,-1,rcItem, uFormat);
	}

	// Draw the Sort arrow
	if(( nItemID == m_nSortedCol ) && m_bSortArrow && !bNoSortHeader)
	{
		// Set up pens to use for drawing the triangle
		CPen penLite(PS_SOLID, 1, xtAfxData.clr3DHilight);
		CPen penShad(PS_SOLID, 1, xtAfxData.clr3DShadow);
		pDC->SelectObject(&penLite);
		
		if (m_bAscending)
		{
			// Draw triangle pointing upwards
			pDC->MoveTo( rcIcon.right - 2*m_nOffset, m_nOffset-1);
			pDC->LineTo( rcIcon.right - 3*m_nOffset/2, rcIcon.bottom - m_nOffset );
			pDC->LineTo( rcIcon.right - 5*m_nOffset/2-2, rcIcon.bottom - m_nOffset );
			pDC->MoveTo( rcIcon.right - 5*m_nOffset/2-1, rcIcon.bottom - m_nOffset-1 );
			
			pDC->SelectObject( &penShad );
			pDC->LineTo( rcIcon.right - 2*m_nOffset, m_nOffset-2);
		}
		else
		{
			// Draw triangle pointing downwards
			pDC->MoveTo( rcIcon.right - 3*m_nOffset/2, m_nOffset-1);
			pDC->LineTo( rcIcon.right - 2*m_nOffset-1, rcIcon.bottom - m_nOffset + 1 );
			pDC->MoveTo( rcIcon.right - 2*m_nOffset-1, rcIcon.bottom - m_nOffset );
			
			pDC->SelectObject( &penShad );
			pDC->LineTo( rcIcon.right - 5*m_nOffset/2-1, m_nOffset -1 );
			pDC->LineTo( rcIcon.right - 3*m_nOffset/2, m_nOffset -1);
		}
	}

	// Restore the original device context.
	pDC->RestoreDC(nSavedDC);
}

void CXTFlatHeaderCtrl::DrawBorders(CDC* pDC, CRect& rect)
{
	// save the current device context so we
	// can restore it when we lose scope.
	int nSavedDC = pDC->SaveDC();

	// Draw flat style border around entire header.
	CRect rc(rect);
	rc.InflateRect(0,0,1,0);
	pDC->Draw3dRect(rc, xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);

	// Cover up thick 3D border.
	rc.DeflateRect(1,1,0,1);
	pDC->Draw3dRect(rc, xtAfxData.clr3DFace, xtAfxData.clr3DFace);

	// Create the pens for further cover-up.
	CPen penLite(PS_SOLID, 1, xtAfxData.clr3DHilight);
	CPen penShad(PS_SOLID, 1, xtAfxData.clr3DShadow);
	CPen penFace(PS_SOLID, 1, xtAfxData.clr3DFace);
	pDC->SelectObject( &penLite );
	
	// Set up the header item struct.
	HD_ITEM hdi;
	memset (&hdi, 0, sizeof(HD_ITEM));
	hdi.fmt  = HDF_STRING | HDF_LEFT | HDF_OWNERDRAW;
	hdi.mask = HDI_WIDTH | HDI_TEXT | HDI_FORMAT | HDI_ORDER;
	int cx = 0;
	
    long j = GetItemCount();
    long i = 0;
	
    long * ItemOrders = new long[j];
    for ( i=0; i<j; i++ )
	{
		GetItem(i, &hdi);
		ItemOrders[hdi.iOrder] = i;
    }
	
	hdi.fmt  = HDF_STRING | HDF_LEFT | HDF_OWNERDRAW;
	hdi.mask = HDI_WIDTH | HDI_TEXT | HDI_FORMAT;

	// For each header item found, do further cover up.
	for (i = 0; i < j; ++i)
	{
		GetItem( ItemOrders[i], &hdi);
		cx += hdi.cxy;

		pDC->SelectObject(&penShad);
		pDC->MoveTo(cx, 2);
		pDC->LineTo(cx, rc.bottom-1);

		pDC->SelectObject(&penLite);
		pDC->MoveTo(cx+1, 2);
		pDC->LineTo(cx+1, rc.bottom-1);

		pDC->SelectObject(&penFace);
		pDC->MoveTo(cx-1, 2);
		pDC->LineTo(cx-1, rc.bottom-1);

		pDC->SelectObject(&penFace);
		pDC->MoveTo(cx-2, 2);
		pDC->LineTo(cx-2, rc.bottom-1);
	}
	
	delete [] ItemOrders;

	if (m_bLBtnDown == FALSE)
	{
		pDC->SelectObject(&penShad);
		pDC->MoveTo(rect.right-1, rect.top);
		pDC->LineTo(rect.right-1, rect.bottom);
	}
	
	// Cleanup.
	pDC->RestoreDC(nSavedDC);
}

void CXTFlatHeaderCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt = m_pt = point;
	ClientToScreen(&pt);

	// If no sort headers are defined for the parent control or popup menus
	// has been disabled, call the base class and return.
	CWnd* pParentWnd = GetParent();
	if (pParentWnd->GetStyle() & LVS_NOSORTHEADER || m_bEnableMenus == FALSE)
	{
		CXTHeaderCtrl::OnRButtonDown(nFlags, point);
		return;
	}

	// No menu was defined use default
	if (!m_popupMenuID)
	{
		// Get the index to the header item under the cursor.
		int iIndex = HitTest(m_pt);

		if (iIndex != -1)
		{
			XT_MANAGE_STATE;  // Switch to toolkit resources/module state

			CXTCoolMenu	coolMenu;
			coolMenu.HookWindow(this);
			coolMenu.LoadToolbar(XT_IDR_TBAR_HDR);

			CXTMenu menu;
			VERIFY(menu.LoadMenu(XT_IDM_POPUP));
			
			CXTMenu* pPopup = menu.GetSubMenu(2);
			ASSERT(pPopup != NULL);

			if (m_nSortedCol == iIndex && m_bAscending == TRUE)
				pPopup->CheckMenuItem(XT_IDC_SORTASC, MF_CHECKED | MF_BYCOMMAND);

			else if (m_nSortedCol == iIndex && m_bAscending == FALSE)
				pPopup->CheckMenuItem(XT_IDC_SORTDSC, MF_CHECKED | MF_BYCOMMAND);

			CWnd* pParentWnd = GetParent();
			if (pParentWnd && (
				pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
				pParentWnd->IsKindOf(RUNTIME_CLASS(CListView))))
			{
				LVCOLUMN lvc;
				lvc.mask = LVCF_FMT;
				
				ListView_GetColumn(pParentWnd->m_hWnd, iIndex, &lvc);
				
				switch (lvc.fmt & LVCFMT_JUSTIFYMASK)
				{
				case LVCFMT_LEFT:
					pPopup->CheckMenuItem(XT_IDC_ALIGNLEFT, MF_CHECKED | MF_BYCOMMAND);
					break;
				case LVCFMT_CENTER:
					pPopup->CheckMenuItem(XT_IDC_ALIGNCENTER, MF_CHECKED | MF_BYCOMMAND);
					break;
				case LVCFMT_RIGHT:
					pPopup->CheckMenuItem(XT_IDC_ALIGNRIGHT, MF_CHECKED | MF_BYCOMMAND);
					break;
				}
			}
		
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
				pt.x, pt.y, this);
		}
	}
	else
	{
		CMenu menu;
		VERIFY(menu.LoadMenu(m_popupMenuID));

		CMenu* pPopup = menu.GetSubMenu(m_nPos);
		ASSERT(pPopup != NULL);
		
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			pt.x, pt.y, GetOwner());
	}
}

void CXTFlatHeaderCtrl::SetMenuID(UINT popupMenuID, int nPos)
{
	m_popupMenuID  = popupMenuID;
	m_nPos		   = nPos;
}

#if _MSC_VER < 1200 // MFC 5.0
BOOL CXTFlatHeaderCtrl::GetOrderArray(LPINT piArray, int iCount /* = -1 */)
{
	ASSERT(::IsWindow(m_hWnd));

	// if -1 was passed, find the count ourselves

	int nCount = iCount;
	if (nCount == -1)
	{
		nCount = GetItemCount();

		if (nCount == -1)
			return FALSE;
	}

	ASSERT(AfxIsValidAddress(piArray, iCount * sizeof(int)));

	return (BOOL) ::SendMessage(m_hWnd, HDM_GETORDERARRAY,
		(WPARAM) iCount, (LPARAM) piArray);
}
#endif // _MSC_VER < 1200

LRESULT CXTFlatHeaderCtrl::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (m_bInitControl)
	{
		m_bInitControl = false;

		CWnd* pParentWnd = GetParent();
		ASSERT_VALID(pParentWnd);
		pParentWnd->ModifyStyle(0, WS_CLIPCHILDREN);
	}

	return 0;
}

void CXTFlatHeaderCtrl::PreSubclassWindow() 
{
	CXTHeaderCtrl::PreSubclassWindow();

	// Initialize the control.
	PostMessage(XTWM_INITIAL_UPDATE);
}

int CXTFlatHeaderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTHeaderCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	SendMessage(XTWM_INITIAL_UPDATE);

	return 0;
}

void CXTFlatHeaderCtrl::OnPaint() 
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient);

	// and let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	// draw the flat borders.
	DrawBorders(&memDC, rectClient);
}

BOOL CXTFlatHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CXTFlatHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	CXTHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CXTFlatHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = FALSE;
	CXTHeaderCtrl::OnLButtonUp(nFlags, point);
}

void CXTFlatHeaderCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CXTHeaderCtrl::OnWindowPosChanged(lpwndpos);
	Invalidate();
}

int CXTFlatHeaderCtrl::HitTest( CPoint pt ) const
{
	int iItem;
	for (iItem = 0; iItem < GetItemCount(); ++iItem)
	{
		CRect r;
		Header_GetItemRect(m_hWnd, iItem, &r);
		
		if (r.PtInRect(pt))
			return iItem;
	}

	return -1;
}

void CXTFlatHeaderCtrl::SendNotify(int iIndex)
{
	CWnd* pParentWnd = GetParent();
	if (pParentWnd && (
		pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
		pParentWnd->IsKindOf(RUNTIME_CLASS(CListView))))
	{
		CWnd* pOwnerWnd = pParentWnd->GetOwner();
		if (pOwnerWnd)
		{
			TCHAR  lpBuffer[256];
			
			HDITEM hdi;
			hdi.mask       = HDI_TEXT|HDI_BITMAP|HDI_FORMAT|HDI_IMAGE|HDI_LPARAM|HDI_ORDER|HDI_WIDTH;
			hdi.pszText    = lpBuffer;
			hdi.cchTextMax = 256;
			
			GetItem(iIndex, &hdi);
			
			NMHEADER nmh;
			nmh.hdr.hwndFrom = m_hWnd;
			nmh.hdr.idFrom   = GetDlgCtrlID();
			nmh.hdr.code     = HDN_ITEMCLICK;
			nmh.iItem        = iIndex;
			nmh.iButton      = 1;
			nmh.pitem        = &hdi;
			
			// send message to the parent's owner window.
			pOwnerWnd->SendMessage(WM_NOTIFY, 
				(WPARAM)(int)nmh.hdr.idFrom, (LPARAM)(NMHEADER*)&nmh);

			// then forward to the descendants.
			pOwnerWnd->SendMessageToDescendants(WM_NOTIFY, 
				(WPARAM)(int)nmh.hdr.idFrom, (LPARAM)(NMHEADER*)&nmh);
		}
	}
}

void CXTFlatHeaderCtrl::OnSortAsc()
{
	int iIndex = HitTest(m_pt);
	if (iIndex != -1)
	{
		if (m_nSortedCol != iIndex || m_bAscending == FALSE)
		{
			m_bAscending = TRUE;
			m_nSortedCol = iIndex;
			SendNotify(iIndex);
		}
	}
}

void CXTFlatHeaderCtrl::OnSortDsc()
{
	int iIndex = HitTest(m_pt);
	if (iIndex != -1)
	{
		if (m_nSortedCol != iIndex || m_bAscending == TRUE)
		{
			m_bAscending = FALSE;
			m_nSortedCol = iIndex;
			SendNotify(iIndex);
		}
	}
}

BOOL CXTFlatHeaderCtrl::SetAlingment(int nFlag)
{
	int iIndex = HitTest(m_pt);
	
	if (iIndex != -1)
	{
		CWnd* pParentWnd = GetParent();
		if (pParentWnd && (
			pParentWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) ||
			pParentWnd->IsKindOf(RUNTIME_CLASS(CListView))))
		{
			LVCOLUMN lvc;
			lvc.mask = LVCF_FMT;
			
			ListView_GetColumn(pParentWnd->m_hWnd, iIndex, &lvc);
			lvc.fmt &= ~LVCFMT_JUSTIFYMASK;
			lvc.fmt |= nFlag;
			ListView_SetColumn(pParentWnd->m_hWnd, iIndex, &lvc);
			ListView_SetWorkAreas(pParentWnd->m_hWnd, 0, NULL);
		}
	}

	return FALSE;
}

void CXTFlatHeaderCtrl::OnAlignLeft()
{
	SetAlingment(LVCFMT_LEFT);
}

void CXTFlatHeaderCtrl::OnAlignCenter()
{
	SetAlingment(LVCFMT_CENTER);
}

void CXTFlatHeaderCtrl::OnAlignRight()
{
	SetAlingment(LVCFMT_RIGHT);
}

void CXTFlatHeaderCtrl::SetBitmap(int iCol, UINT uBitmapID, DWORD dwRemove)
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage (AfxGetResourceHandle(),
		MAKEINTRESOURCE(uBitmapID), IMAGE_BITMAP, 0, 0, LR_LOADMAP3DCOLORS);
	ASSERT(hBitmap != NULL);

	if (hBitmap != NULL)
	{
		HD_ITEM hdi;
		hdi.mask = HDI_FORMAT;
		
		GetItem (iCol, &hdi);
		hdi.mask = HDI_BITMAP | HDI_FORMAT;
		hdi.fmt |= HDF_BITMAP;
		hdi.hbm  = hBitmap;

		if (dwRemove != NULL)
			hdi.fmt &= ~dwRemove;

		SetItem (iCol, &hdi);
	}
}

