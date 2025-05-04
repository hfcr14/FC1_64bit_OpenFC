// XTTreeCtrl.cpp : implementation file
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
#include "XTTreeCtrl.h"
#include "XTMemDC.h"
#include <windowsx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef MST_TIMER_PERIOD
#define MST_TIMER_PERIOD	75
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTTreeCtrl

CXTTreeCtrl::CXTTreeCtrl()
: m_hSelect(NULL)
, m_bMultiSelect(false)
, m_bBandLabel(true)
, m_bActionDone(false)
, m_bTreeNotify(false)
, m_bOkToEdit(true)
{

}

CXTTreeCtrl::~CXTTreeCtrl()
{
}

IMPLEMENT_DYNAMIC(CXTTreeCtrl, CTreeCtrl)

BEGIN_MESSAGE_MAP(CXTTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CXTTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT_EX(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTTreeCtrl message handlers

void CXTTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
    // If multiselect control, process possible left 
    // click drag selection.
    if ( m_bMultiSelect )
    {
		UINT nHitFlags = 0;
		
		HTREEITEM hItemHit = HitTest(point, &nHitFlags);
		HTREEITEM hItemSel = GetSelectedItem();
		
		// if expanding/contracting call base class.
		if ( ( nHitFlags & TVHT_ONITEMBUTTON ) == TVHT_ONITEMBUTTON )
		{
			CTreeCtrl::OnLButtonDown( nFlags, point );
			return;
		}
		
		if ( GetStyle() & TVS_EDITLABELS )
		{
			if( !( nFlags & ( MK_CONTROL|MK_SHIFT ) ) && ( nHitFlags & TVHT_ONITEMLABEL ) )
			{
				if ( m_bOkToEdit && hItemHit == hItemSel )
				{
					SelectAll(FALSE);
					SelectItem(hItemHit);
					
					CTreeCtrl::OnLButtonDown( nFlags, point );
					return;
				}
			}

			if ( ( nHitFlags & TVHT_ONITEM ) == 0 ) {
				m_bOkToEdit = false;
			}

			else if ( GetFocusedItem() == hItemSel ) {
				m_bOkToEdit = true;
			}
		}

		OnButtonDown(TRUE, nFlags, point);
    }

    else
	{
        CTreeCtrl::OnLButtonDown( nFlags, point );
    }
}

void CXTTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER(nFlags);

	// hittest to get the tree item under the cursor
	// and select it.
	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);
	if (hItem != NULL && (uFlags & TVHT_ONITEM) != 0)
	{
		// if the item is not selected, clear previous
		// selections and select the item under cursor.
		if (!IsSelected(hItem))
		{
			SelectAll(FALSE);
			SelectItem(hItem);
		}
	}
	else
	{
		// clear previous selections.
		SelectAll(FALSE);
	}

	// get the owner of the tree control.
    HWND hWnd = GetOwner()->m_hWnd;

    if (::IsWindow(hWnd))
	{
		// construct a NMHDR struct...
		NMHDR mHDR;
		mHDR.hwndFrom = m_hWnd;
		mHDR.code     = NM_RCLICK;
		mHDR.idFrom   = GetDlgCtrlID();

		// and send a WM_NOTIFY message to our owner.
		SendNotify( &mHDR );
	}
}

void CXTTreeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_bMultiSelect)
	{
		//'emulated' selected items will remain greyed
		// if application gets covered
		HTREEITEM hItem = GetFirstSelectedItem();
		while (hItem)
		{
			RECT rect;
			GetItemRect(hItem, &rect, TRUE);
			InvalidateRect(&rect);
			hItem = GetNextSelectedItem(hItem);
		}
	}

	CTreeCtrl::OnSetFocus(pOldWnd);
}

void CXTTreeCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CTreeCtrl::OnKillFocus(pNewWnd);
	
	if (m_bMultiSelect)
	{
		//'emulated' selected items may not get
		// refreshed to grey
		HTREEITEM hItem = GetFirstSelectedItem();
		while (hItem)
		{
			RECT rect;
			GetItemRect(hItem, &rect, TRUE);
			InvalidateRect(&rect);
			hItem = GetNextSelectedItem(hItem);
		}
	}
}

void CXTTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (!m_bMultiSelect) {
		CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
		return;
	}
		
	BOOL bCtrl  = (::GetKeyState(VK_CONTROL) < 0);
	BOOL bShift = (::GetKeyState(VK_SHIFT)   < 0);
	BOOL bDir   = FALSE;
	UINT uCount = GetSelectedCount();

	HTREEITEM hSel = NULL;
	switch (nChar)
	{
	case VK_UP:
		bDir = TRUE;
	case VK_DOWN:
		hSel = GetSelectedItem();
		if (!m_hSelect) {
			m_hSelect = hSel;
		}
		if (!bCtrl && !bShift)
		{
			m_hSelect = NULL;	//reset

			if (uCount > 1) {
				SelectAll(FALSE);
			}
			else {
				SetItemState(GetFocusedItem(), TVIS_FOCUSED|TVIS_SELECTED,
					TVIS_FOCUSED|TVIS_SELECTED);
			}
		}
		break;
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	if (!hSel || (!bCtrl && !bShift)) {
		return;
	}

	HTREEITEM hNext = bDir ? GetPrevVisibleItem(hSel) : GetNextVisibleItem(hSel);
	if (!hNext) {
		hNext = hSel;
	}
	if (bShift) {
		SelectItems(m_hSelect, hNext, TRUE);
	}
	else if (bCtrl) {
		SetItemState(hNext, TVIS_FOCUSED, TVIS_FOCUSED);
	}
}

BOOL CXTTreeCtrl::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (!m_bMultiSelect)
		return FALSE;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
	
	// TODO: Add your control notification handler code here
	if ((pNMTreeView->action == TVE_COLLAPSE) || (pNMTreeView->action == TVE_COLLAPSERESET))
	{
		// clear selection of children, it would be confusing otherwise
		// - the notifications can be over-ridden to stop the de-selection
		// if required. 
		// Unfortunately, the parent window can't over-ride this functionality
		// because MFC gives this class first crack.  So if changes are required
		// a derived class will have to be used..
		ASSERT(pNMTreeView->itemNew.hItem);

		// if a descendent item has focus the parent will get selected as a
		// consequence of collapsing the tree, so preserve
		// (if the parent was already selected it will gain focus, but
		// that's acceptable)
		BOOL bWasSel = IsSelected(pNMTreeView->itemNew.hItem);
		BOOL bWasFocus = SelectChildren(pNMTreeView->itemNew.hItem, FALSE, TRUE);
		
		if (bWasFocus && !bWasSel) {
			CTreeCtrl::SelectItem(NULL);	//stop parent from gaining selection
		}
	}
	
	return FALSE;	//pass to parent
}

HTREEITEM CXTTreeCtrl::GetNextItem( HTREEITEM hItem )
{
	HTREEITEM hti = NULL;
	
	if (ItemHasChildren(hItem)) {
		hti = GetChildItem(hItem);
	}
	
	if (hti == NULL)
	{
		while ((hti = GetNextSiblingItem(hItem)) == NULL)
		{
			if ((hItem = GetParentItem(hItem)) == NULL)
				return NULL;
		}
	}

	return hti;
}

HTREEITEM CXTTreeCtrl::GetPrevItem( HTREEITEM hItem )
{
	HTREEITEM hti = NULL;
	
	hti = GetPrevSiblingItem(hItem);
	if( hti == NULL ) {
		hti = GetParentItem(hItem);
	}
	else {
		hti = GetLastItem(hti);
	}

	return hti;
}

HTREEITEM CXTTreeCtrl::GetLastItem( HTREEITEM hItem )
{
	// Temporary used variable
	HTREEITEM htiNext;

	// Get the last item at the top level
	if( hItem == NULL ) {
		hItem = GetRootItem();
	}
	
	while( ItemHasChildren( hItem ) != NULL )
	{
		// Find the last child of hItem
		htiNext = GetChildItem( hItem );
		while( htiNext != NULL )
		{
			hItem = htiNext;
			htiNext = GetNextSiblingItem( htiNext );
		}
	}
	
	return hItem;
	
}

HTREEITEM CXTTreeCtrl::FindItemInBranch(LPCTSTR lpszSearch, BOOL bCaseSensitive /*= FALSE*/, 
	BOOL bWholeWord /*= FALSE*/, HTREEITEM htiItem /*= NULL*/)
{
    HTREEITEM htiFound = NULL;

    if (!ItemHasChildren(htiItem))
        return NULL;

	CString strSearch = lpszSearch;
	int iLen = strSearch.GetLength();
	if (iLen == 0) {
		return NULL;
	}

	if (!bCaseSensitive) {
		strSearch.MakeLower();
	}

    HTREEITEM htiChild = GetChildItem(htiItem);
    while (htiChild != NULL)
    {
        if (ItemHasChildren(htiChild))
        {
            htiFound = FindItemInBranch(lpszSearch, bCaseSensitive, bWholeWord, htiChild);
            if (htiFound != NULL)
                return htiFound;
        }
        
		CString strItemText = GetItemText( htiChild );
		if (!bCaseSensitive) {
			strItemText.MakeLower();
		}

		int iIndex;
		while ((iIndex = strItemText.Find( strSearch )) != -1)
		{
			// Search string found
			if (bWholeWord)
			{
				// Check preceding char
				if (iIndex != 0)
				{
					if( isalpha(strItemText[iIndex-1]) || strItemText[iIndex-1] == '_' )
					{
						// Not whole word
						strItemText = strItemText.Right(strItemText.GetLength() -
							iIndex - iLen );
						continue;
					}
				}

				// Check succeeding char
				if (strItemText.GetLength() > iIndex + iLen &&
				   (isalpha(strItemText[iIndex+iLen]) ||
				   (strItemText[iIndex+iLen] == '_')))
				{
					// Not whole word
					strItemText = strItemText.Right( strItemText.GetLength() -
						iIndex - strSearch.GetLength() );
					continue;
				}
			}
			
			if (IsFindValid(htiChild)) {
				return htiChild;
			}

			else {
				break;
			}
		}

        htiChild = GetNextSiblingItem(htiChild);
    }

    return NULL;
}

HTREEITEM CXTTreeCtrl::FindItem(LPCTSTR lpszSearch, BOOL bCaseSensitive /*= FALSE*/, 
	BOOL bDownDir /*= TRUE*/, BOOL bWholeWord /*= FALSE*/, HTREEITEM hItem /*= NULL*/)
{
	CString str = lpszSearch;

	int lenSearchStr = str.GetLength();
	if (lenSearchStr == 0) {
		return NULL;
	}

	// For the first pass, set the current item equal to the selection
	HTREEITEM htiSel = hItem ? hItem : GetSelectedItem();
	HTREEITEM htiCur = htiSel;
	CString strSearch = str;

	// make sure it ends if we started with no selection
	if ((htiCur == NULL) && (htiSel != NULL))
	{
		if (bDownDir) {
			htiCur = GetRootItem();
		}
		else {
			htiCur = GetLastItem(NULL);
		}
	}

	if (!bCaseSensitive) {
		strSearch.MakeLower();
	}

	// For the first pass only, we check to see if it 
	// is the item we're looking for.
	BOOL bFirstPass = TRUE;

	while( htiCur && (htiCur != htiSel || bFirstPass) )
	{
		bFirstPass = FALSE;

		CString strItemText = GetItemText( htiCur );
		if (!bCaseSensitive) {
			strItemText.MakeLower();
		}

		int iIndex;
		while ((iIndex = strItemText.Find( strSearch )) != -1)
		{
			// Search string found
			if (bWholeWord)
			{
				// Check preceding char
				if (iIndex != 0)
				{
					if( isalpha(strItemText[iIndex-1]) || strItemText[iIndex-1] == '_' )
					{
						// Not whole word
						strItemText = strItemText.Right(strItemText.GetLength() -
							iIndex - lenSearchStr );
						continue;
					}
				}

				// Check succeeding char
				if (strItemText.GetLength() > iIndex + lenSearchStr &&
				   (isalpha(strItemText[iIndex+lenSearchStr]) ||
				   (strItemText[iIndex+lenSearchStr] == '_')))
				{
					// Not whole word
					strItemText = strItemText.Right( strItemText.GetLength() -
						iIndex - strSearch.GetLength() );
					continue;
				}
			}
			
			if (IsFindValid(htiCur)) {
				return htiCur;
			}
			else {
				break;
			}
		}

		htiCur = bDownDir ? GetNextItem( htiCur ) : GetPrevItem( htiCur );
		if( htiCur == NULL )
		{
			if (bDownDir) {
				htiCur = GetRootItem();
			}
			else {
				htiCur = GetLastItem( NULL );
			}
		}
	}

	return NULL;
}

BOOL CXTTreeCtrl::IsFindValid( HTREEITEM )
{
	return TRUE;
}

void CXTTreeCtrl::SetItemFont(HTREEITEM hItem, LOGFONT& logfont)
{
	XT_CLRFONT cf;
	if (!m_mapColorFont.Lookup(hItem, cf)) {
		cf.color = (COLORREF)-1;
	}

	cf.logfont = logfont;
	m_mapColorFont[hItem] = cf;
}

BOOL CXTTreeCtrl::GetItemFont(HTREEITEM hItem, LOGFONT * plogfont)
{
	XT_CLRFONT cf;
	if (!m_mapColorFont.Lookup(hItem, cf)) {
		return FALSE;
	}

	if (cf.logfont.lfFaceName[0] == '\0') {
		return FALSE;
	}

	*plogfont = cf.logfont;
	
	return TRUE;

}

void CXTTreeCtrl::SetItemBold(HTREEITEM hItem, BOOL bBold)
{
	SetItemState( hItem, bBold ? TVIS_BOLD: 0, TVIS_BOLD );
	InvalidateRect(NULL);
}

BOOL CXTTreeCtrl::GetItemBold(HTREEITEM hItem)
{
	return GetItemState( hItem, TVIS_BOLD ) & TVIS_BOLD;
}

void CXTTreeCtrl::SetItemColor(HTREEITEM hItem, COLORREF color)
{
	XT_CLRFONT cf;
	if (!m_mapColorFont.Lookup( hItem, cf)) {
		cf.logfont.lfFaceName[0] = '\0';
	}

	cf.color = color;
	m_mapColorFont[hItem] = cf;
	InvalidateRect(NULL);
}

COLORREF CXTTreeCtrl::GetItemColor(HTREEITEM hItem)
{
	XT_CLRFONT cf;
	if (!m_mapColorFont.Lookup(hItem, cf)) {
		return (COLORREF)-1;
	}
	return cf.color;
}

BOOL CXTTreeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}

void CXTTreeCtrl::DoPaint()
{
	CClientDC dc(this);

	// Get the client rect.
	CRect rcClient;
	GetClientRect(&rcClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rcClient, xtAfxData.clrWindow);

	// Now let the window do its default painting...
	CTreeCtrl::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	
	int nSavedDC = memDC.SaveDC();

	// check to see if the tree has focus.
	bool bHasFocus = (GetFocus() == this);
	
	// get the visible count.
	HTREEITEM hItem = GetFirstVisibleItem();
	int iVisibleCount = GetVisibleCount()+1;

	while (hItem && iVisibleCount--)
	{
		// get the tree item rect.
		CRect rcItem;
		GetItemRect(hItem, &rcItem, TRUE);

		// refresh the background.
		CRect rcFill = rcItem;
		rcFill.right = rcClient.right;
		memDC.FillSolidRect(&rcFill, xtAfxData.clrWindow);

		// get the state of the tree item.
		UINT uState = GetItemState(hItem, TVIS_SELECTED|TVIS_FOCUSED);
        BOOL bFocused   = ((uState & TVIS_FOCUSED)     == TVIS_FOCUSED);
        BOOL bSelected  = ((uState & TVIS_SELECTED)    == TVIS_SELECTED);
		BOOL bHilighted = ((uState & TVIS_DROPHILITED) == TVIS_DROPHILITED);

		// define the background and text colors.
		COLORREF crBack = (bSelected|bHilighted) ? 
			(bHasFocus ? xtAfxData.clrHighlight : xtAfxData.clr3DFace) : xtAfxData.clrWindow;
		
		COLORREF crText = (bSelected|bHilighted) ? 
			(bHasFocus ? xtAfxData.clrHighlightText : xtAfxData.clrBtnText) : xtAfxData.clrWindowText;

		// create the LOGFONT used by the tree item.
		LOGFONT logfont;
		GetFont()->GetLogFont(&logfont);
		
		XT_CLRFONT cf;
		if (m_mapColorFont.Lookup(hItem, cf))
		{
			if (cf.color != (COLORREF)-1 && !bSelected) {
				crText = cf.color;
			}
			if (cf.logfont.lfFaceName[0] != '\0') {
				logfont = cf.logfont;
			}
		}

		if (GetItemBold(hItem)) {
			logfont.lfWeight = FW_BOLD;
		}

		// create a CFont object from the LOGFONT structure and
		// select it into the current device context.
		CFont fontDC;
		fontDC.CreateFontIndirect(&logfont);
		CFont* pOldFont = memDC.SelectObject(&fontDC);

		// get the text for the tree item and determine its size.
		CString strItem = GetItemText(hItem);
		CSize sizeText = memDC.GetTextExtent(strItem);

		// if the text is wider than the tree item label, adjust accordingly.
		if (rcItem.Width() < sizeText.cx) {
			rcItem.right = rcItem.left + sizeText.cx + 2;
		}

		// set the font foreground and background colors.
		memDC.SetBkColor(crBack);
		memDC.SetTextColor(crText);

		// draw the label background if selected.
		if (bSelected) {
			memDC.FillSolidRect(&rcItem, crBack);
		}

		// draw the text and restore the device context.
		memDC.DrawText(strItem, &rcItem, DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_NOPREFIX);
		memDC.SelectObject( pOldFont );
		fontDC.DeleteObject();

		// draw the focus rect.
		if ( bFocused && !GetEditControl( ) )
		{
			memDC.SetTextColor(xtAfxData.clrWindowText);
			memDC.DrawFocusRect(&rcItem);
		}

		// move to the next visible item.
		hItem = GetNextVisibleItem( hItem );
	}
	
	memDC.RestoreDC(nSavedDC);
}

void CXTTreeCtrl::OnPaint() 
{
	CPaintDC dc(this);
	DoPaint(); // perform custom paint.
}

HTREEITEM CXTTreeCtrl::GetPrevSelectedItem(HTREEITEM hItem)
{
	for (hItem = GetPrevVisibleItem(hItem);
	     hItem != NULL;
		 hItem = GetPrevVisibleItem(hItem))
	{
		 if (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED) {
			return hItem;
		 }
	}

	return NULL;
}

UINT CXTTreeCtrl::GetSelectedCount() const
{
	UINT nCount = 0;
	HTREEITEM hItem = GetFirstSelectedItem();
	while (hItem)
	{
		nCount++;
		hItem = GetNextSelectedItem(hItem);
	}
	return nCount;
}

BOOL CXTTreeCtrl::EnableMultiSelect(BOOL bMultiSelect)
{
	BOOL bReturn = m_bMultiSelect;
	m_bMultiSelect = bMultiSelect;

	if (!m_bMultiSelect)
	{
		HTREEITEM hItem = GetSelectedItem();
		if (hItem && !IsSelected(hItem)) {
			hItem = NULL;
		}
		SelectAllIgnore(FALSE, hItem);
		if (hItem) {
			SelectItem(hItem);
		}
	}

	return bReturn;
}

BOOL CXTTreeCtrl::SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask)
{
	ASSERT(hItem);

	if (!m_bMultiSelect) {
		return CTreeCtrl::SetItemState(hItem, nState, nStateMask);
	}

	HTREEITEM hFocus  = GetSelectedItem();		      // current focus
	BOOL bWasFocus    = (hFocus == hItem);
	BOOL bFocusWasSel = hFocus && IsSelected(hFocus); // selection state of current focus
	BOOL bWasSel      = IsSelected(hItem);		      // select state of acting item

	UINT nS  = (nState & ~TVIS_FOCUSED);
	UINT nSM = (nStateMask & ~TVIS_FOCUSED);

	if (nStateMask & TVIS_FOCUSED)
	{
		//wanted to affect focus
		if (nState & TVIS_FOCUSED)
		{
			//wanted to set focus
			if (!bWasFocus && bFocusWasSel)
			{
				//because SelectItem would de-select the current 'real' selection
				// (the one with focus), need to make the tree ctrl think there is
				// no 'real' selection but still keep the the old item selected
				//it has to be done before the SelectItem call because
				// otherwise the TVN_SELCHANGING/ED notification handlers
				// wouldn't be able to get the proper list of selected items
				CTreeCtrl::SelectItem(NULL); //will cause notify, but can be taken as focus change
				CTreeCtrl::SetItemState(hFocus, TVIS_SELECTED, TVIS_SELECTED);
				UpdateWindow();
			}

			if (!CTreeCtrl::SelectItem(hItem)) { //set focus (will consequently select, if not already focused)
				return FALSE;
			}
			if (nStateMask & TVIS_SELECTED)
			{
				//wanted to affect select state
				if (nState & TVIS_SELECTED)
				{
					//wanted to select, already done if wasn't focused
					if (!bWasFocus || bFocusWasSel)
					{
						nS &= ~TVIS_SELECTED;
						nSM &= ~TVIS_SELECTED;
					}
				}
				//else wanted to clear, base call will do that
			}
			else
			{
				//didn't want to affect select state
				if (!bWasSel)
				{
					//it wasn't previously selected, let base clear (correct)
					nS &= ~TVIS_SELECTED;
					nSM |= TVIS_SELECTED;
				}
				//else was already selected, no harm done
			}
		}
		else
		{
			//wanted to clear focus
			if (bWasFocus)
			{
				//it had the focus
				CTreeCtrl::SelectItem(NULL);	//clear focus
				if (!(nStateMask & TVIS_SELECTED))
				{
					//didn't want to affect select state
					if (bWasSel)
					{
						//it was selected, so restore
						ASSERT( !(nS & TVIS_SELECTED) );
						ASSERT( !(nSM & TVIS_SELECTED) );
						//set state here, to avoid double-notify
						CTreeCtrl::SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
						//let base do other states
					}
				}
				else if (nState & TVIS_SELECTED)
				{
					//wanted to select (but clear focus)
					if (bWasSel)
					{
						//if was selected, restore
						CTreeCtrl::SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
					}
					//don't want to notify, default did it
					nS &= ~TVIS_SELECTED;
					nSM &= ~TVIS_SELECTED;
				}
			}
		}
	}

	if (!nSM) {
		return TRUE;	//no other states to alter
	}

	if (nSM & TVIS_SELECTED)
	{
		//still need to alter selection state
		NMTREEVIEW nmtv;
		nmtv.hdr.hwndFrom  = m_hWnd;
		nmtv.hdr.idFrom    = ::GetDlgCtrlID(m_hWnd);
		nmtv.hdr.code      = TVN_SELCHANGING;
		nmtv.itemOld.mask  = nmtv.itemNew.mask = 0;
		nmtv.itemOld.hItem = nmtv.itemNew.hItem = NULL;
		
		TVITEM& item       = (nS & TVIS_SELECTED) ? nmtv.itemNew : nmtv.itemOld;
		item.mask          = TVIF_HANDLE|TVIF_PARAM;
		item.hItem         = hItem;
		item.lParam        = GetItemData(hItem);
	
		if (SendNotify(&nmtv.hdr)) {
			return FALSE;	//sel-changing stopped
		}
		
		VERIFY( CTreeCtrl::SetItemState(hItem, nS, nSM) );
		nmtv.hdr.code = TVN_SELCHANGED;
		SendNotify(&nmtv.hdr);
		nS &= ~TVIS_SELECTED;
		nSM &= ~TVIS_SELECTED;
	}
	
	if (!nSM) {
		return TRUE;
	}

	return CTreeCtrl::SetItemState(hItem, nS, nSM);
}

UINT CXTTreeCtrl::GetItemState(HTREEITEM hItem, UINT nStateMask) const
{
	UINT n = CTreeCtrl::GetItemState(hItem, nStateMask & ~TVIS_FOCUSED);
	if (nStateMask & TVIS_FOCUSED)
	{
		if (GetSelectedItem() == hItem) {
			n |= TVIS_FOCUSED;
		}
	}
	return n;
}

BOOL CXTTreeCtrl::SelectItem(HTREEITEM hItem)
{
	if (m_bMultiSelect)
	{
		return SetItemState(hItem,
			TVIS_SELECTED|TVIS_FOCUSED, TVIS_SELECTED|TVIS_FOCUSED);
	}
	else
	{
		return CTreeCtrl::SelectItem(hItem);
	}
}

BOOL CXTTreeCtrl::FocusItem(HTREEITEM hItem)
{
	ASSERT(m_bMultiSelect);

	BOOL bRet = FALSE;
	if (hItem) {
		bRet = SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED);
	}
	else
	{
		hItem = CTreeCtrl::GetSelectedItem();
		if (hItem) {
			bRet = SetItemState(hItem, 0, TVIS_FOCUSED);
		}
	}
	return bRet;
}

LRESULT CXTTreeCtrl::SendNotify(LPNMHDR pNMHDR)
{
	CWnd* pWndOwner = GetOwner();
	if (!pWndOwner)
	{
		return 0;
	}

	bool bTreeNotify = m_bTreeNotify;

	LRESULT lResult = ::SendMessage( pWndOwner->m_hWnd, WM_NOTIFY,
		( WPARAM )pNMHDR->idFrom, ( LPARAM )pNMHDR );

	m_bTreeNotify = bTreeNotify;

	return lResult;
}

HTREEITEM CXTTreeCtrl::GetFirstSelectedItem() const
{
	HTREEITEM hItem = GetRootItem();
	while (hItem)
	{
		if (IsSelected(hItem)) {
			break;
		}
		hItem = GetNextVisibleItem(hItem);
	}
	return hItem;
}

HTREEITEM CXTTreeCtrl::GetNextSelectedItem(HTREEITEM hItem) const
{
	hItem = GetNextVisibleItem(hItem);
	while (hItem)
	{
		if (IsSelected(hItem)) {
			break;
		}
		hItem = GetNextVisibleItem(hItem);
	}
	return hItem;
}

void CXTTreeCtrl::SelectAll(BOOL bSelect /*= TRUE*/)
{
//	SetFocus();

	bSelect = !!bSelect;	//ensure 0 or 1
	UINT nState = bSelect ? TVIS_SELECTED : 0;
	HTREEITEM hItem = GetRootItem();
	while (hItem)
	{
		if (IsSelected(hItem) != bSelect) {
			SetItemState(hItem, nState, TVIS_SELECTED);
		}
		hItem = GetNextVisibleItem(hItem);
	}
}

void CXTTreeCtrl::SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore)
{
	//special case to avoid multiple notifications for
	// the same item
	bSelect = !!bSelect;	//ensure 0 or 1
	UINT nState = bSelect ? TVIS_SELECTED : 0;
	HTREEITEM hItem = GetRootItem();
	while (hItem)
	{
		if (hItem != hIgnore)
		{
			if (IsSelected(hItem) != bSelect) {
				SetItemState(hItem, nState, TVIS_SELECTED);
			}
		}
		hItem = GetNextVisibleItem(hItem);
	}
}

void CXTTreeCtrl::SelectItems(HTREEITEM hFirst, HTREEITEM hLast, BOOL bOnly /*= TRUE*/)
{
	//locate (and select) either first or last
	// (so order is arbitrary)
	HTREEITEM hItem = GetRootItem();
	while (hItem)
	{
		if ((hItem == hFirst) || (hItem == hLast))
		{
			if (hFirst != hLast)
			{
				if (!IsSelected(hItem)) {
					SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
				}
				hItem = GetNextVisibleItem(hItem);
			}
			break;
		}

		if (bOnly && IsSelected(hItem)) {
			SetItemState(hItem, 0, TVIS_SELECTED);
		}

		hItem = GetNextVisibleItem(hItem);
	}
	
	//select rest of range
	while (hItem)
	{
		if (!IsSelected(hItem)) {
			SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
		}
		if ((hItem == hFirst) || (hItem == hLast))
		{
			hItem = GetNextVisibleItem(hItem);
			break;
		}
		hItem = GetNextVisibleItem(hItem);
	}
	if (!bOnly) {
		return;
	}
	while (hItem)
	{
		if (IsSelected(hItem)) {
			SetItemState(hItem, 0, TVIS_SELECTED);
		}
		hItem = GetNextVisibleItem(hItem);
	}
}

BOOL CXTTreeCtrl::OnButtonDown(BOOL bLeft, UINT nFlags, CPoint point)
{
	UINT      nHF   = 0;
	HTREEITEM hItem = NULL;

	BOOL bBase = !m_bMultiSelect;
	if (!bBase)
	{
		hItem = HitTest(point, &nHF);
		if (hItem)
		{
			//base always handles expanding items
			//(doesn't really mean much to right button, but pass anyway)
			bBase = (nHF & (TVHT_ONITEMBUTTON/*|TVHT_ONITEMINDENT*/));
			if (!bBase && bLeft && (GetStyle() & TVS_CHECKBOXES))
			{
				//when the tree has check-boxes, the default handler makes
				// a quick selection of the clicked item, then re-selects
				// the previously selected item - to cause a sel-changed
				// notification.  Fortunately it doesn't affect the multi-
				// selection, so can pass on.
				bBase = (nHF & TVHT_ONITEMSTATEICON);

#ifdef _MST_MULTI_CHECK
				//Use the above define if you want all selected items to
				// be checked the same when any one of them is checked
				// - Interestingly this doesn't happen in the listview control
				//  (LVS_EX_CHECKBOXES)
				if (bBase)
				{
					//the default selection notification would mess
					// the multi-selection up, so generate the notification
					// manually
					// (anyway, this is smoother than the selection flicker
					//  the default gives)
					NMTREEVIEW nmtv;
#ifdef TVN_CHKCHANGE
					nmtv.hdr.code = TVN_CHKCHANGE;
#else
					nmtv.hdr.code = TVN_SELCHANGED;
#endif
					nmtv.hdr.hwndFrom = m_hWnd;
					nmtv.hdr.idFrom = ::GetDlgCtrlID(m_hWnd);
					nmtv.itemOld.hItem = NULL;
					nmtv.itemNew.mask = TVIF_HANDLE|TVIF_PARAM;

					BOOL bChk = !GetCheck(hItem);
					if (IsSelected(hItem))
					{
						HTREEITEM h = GetFirstSelectedItem();
						while (h)
						{
							if (!GetCheck(h) == bChk) //! to ensure 0 or 1
							{
								SetCheck(h, bChk);
#ifdef TVN_CHKCHANGE
								//only send multiple check-change
								// notifications (not sel-changed)
								if (h != hItem) //clicked item will be done last
								{
									nmtv.itemNew.hItem = h;
									nmtv.itemNew.lParam = GetItemData(h);
									SendNotify(&nmtv.hdr);
								}
#endif
							}
							h = GetNextSelectedItem(h);
						}
					}
					else {
						SetCheck(hItem, bChk);
					}
					//notify clicked item
					nmtv.itemNew.hItem = hItem;
					nmtv.itemNew.lParam = GetItemData(hItem);
					SendNotify(&nmtv.hdr);
					return TRUE;
				}
#endif
			}
		}
	}

    if (bBase)
		return TRUE;

	if (!hItem || (nHF & (TVHT_ONITEMRIGHT|TVHT_NOWHERE|TVHT_ONITEMINDENT)))
	{
		//clicked in space, do rubber-banding
		DoBanding(nFlags, point);
		return TRUE;
	}

	ASSERT(nHF & (TVHT_ONITEM|TVHT_ONITEMSTATEICON));	//nothing else left

	DoPreSelection(hItem, bLeft, nFlags);
	DoAction(hItem, bLeft, nFlags, point);

	return m_bActionDone;  // as set in doaction
}

void CXTTreeCtrl::DoPreSelection(HTREEITEM hItem, BOOL bLeft, UINT nFlags)
{
	if (bLeft)
	{
		//if shift key down, select immediately
		if ((nFlags & MK_SHIFT))
		{
			if (!m_hSelect) {
				m_hSelect = GetSelectedItem();	//focus
			}
			SelectItems(m_hSelect, hItem, !(nFlags & MK_CONTROL));
			SetItemState(hItem, TVIS_FOCUSED, TVIS_FOCUSED);	//focus changes to last clicked
		}
		else
		{
			if (!(nFlags & MK_CONTROL))
			{
				//if ctrl was down, then the selection is delayed until
				// mouse up, otherwise select the one item
				if (!IsSelected(hItem)) {
					SelectAllIgnore(FALSE, hItem);
				}
				SetItemState(hItem, TVIS_SELECTED|TVIS_FOCUSED, TVIS_SELECTED|TVIS_FOCUSED);
			}
			m_hSelect = NULL;	//reset when a non-shift operation occurs
		}
		return;
	}

	//right mouse
	if (nFlags & (MK_CONTROL|MK_SHIFT))
	{
		if (!(nFlags & MK_SHIFT)) {
			m_hSelect = hItem;
		}
		return;		//do nothing if shift or ctrl
	}
	if (!IsSelected(hItem)) {
		SelectAllIgnore(FALSE, hItem);
	}
	SetItemState(hItem, TVIS_SELECTED|TVIS_FOCUSED, TVIS_SELECTED|TVIS_FOCUSED);
}

void CXTTreeCtrl::DoAction(HTREEITEM hItem, BOOL bLeft, UINT nFlags, CPoint point)
{
    m_bActionDone = false;
	::SetCapture(m_hWnd);
	ASSERT(::GetCapture() == m_hWnd);

	MSG msg;
	UINT nDone = 0;
	CPoint pt;
	CSize sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));

	while (!nDone && ::GetMessage(&msg, NULL, 0, 0))
	{
		if (::GetCapture() != m_hWnd) {
			break;
		}
		switch (msg.message)
		{
			case WM_MOUSEMOVE:
				pt.x = GET_X_LPARAM(msg.lParam);
				pt.y = GET_Y_LPARAM(msg.lParam);
				if ((abs(pt.x - point.x) > sizeDrag.cx) ||
				   ((abs(pt.y - point.y) > sizeDrag.cy))) {
					nDone = 2;
				}
				//because we exit loop, button up will still be dispatched
				// which means WM_CONTEXTMENU will be sent after TVN_BEGINRDRAG
				// - this is the same behaviour as original tree
				break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				nDone = 1;
				break;

			default:
				::DispatchMessage(&msg);
				break;
		}

	}

	::ReleaseCapture();
	ASSERT(::GetCapture() != m_hWnd);

	//construct tree notification info
	NMTREEVIEW nmtv;
	nmtv.hdr.hwndFrom   = m_hWnd;
	nmtv.hdr.idFrom     = ::GetDlgCtrlID(m_hWnd);
	nmtv.itemNew.mask   = TVIF_HANDLE|TVIF_PARAM;
	nmtv.itemNew.hItem  = hItem;
	nmtv.itemNew.lParam = GetItemData(hItem);
	DWORD dwStyle = GetStyle();

	if (nDone == 1)
	{
		//click
		if (!(nFlags & MK_SHIFT) && bLeft)
		{
			UINT nState = TVIS_SELECTED;
			if ((nFlags & MK_CONTROL)) {
				nState ^= (GetItemState(hItem, TVIS_SELECTED) & TVIS_SELECTED);
			}
			else {
				SelectAllIgnore(FALSE, hItem);
			}
			SetItemState(hItem, TVIS_FOCUSED|nState, TVIS_FOCUSED|TVIS_SELECTED);
		}
		if (::GetFocus() != m_hWnd) {
			::SetFocus(m_hWnd);
		}

		nmtv.hdr.code = bLeft ? NM_CLICK : NM_RCLICK;
		SendNotify(&nmtv.hdr);
        m_bActionDone = true;
	}
	else if (nDone == 2)
	{
		//drag
		SetItemState(hItem, TVIS_FOCUSED|TVIS_SELECTED, TVIS_FOCUSED|TVIS_SELECTED);
		if (!(dwStyle & TVS_DISABLEDRAGDROP))
		{
			nmtv.hdr.code = bLeft ? TVN_BEGINDRAG : TVN_BEGINRDRAG;
			nmtv.ptDrag = point;
			SendNotify(&nmtv.hdr);
		}
        m_bActionDone = true;
	}
}

void CXTTreeCtrl::DoBanding(UINT nFlags, CPoint point)
{
	if (::GetFocus() != m_hWnd) {
		::SetFocus(m_hWnd);
	}

	::SetCapture(m_hWnd);

	CTypedPtrList<CPtrList, HTREEITEM> list;
	if (nFlags & (MK_SHIFT|MK_CONTROL)) {
		GetSelectedList(list);
	}

	CClientDC dc(this);
	CRect rectCli;
	GetClientRect(&rectCli);

	MSG msg;
	BOOL bDone = FALSE;
	CPoint pt;
	CSize sizeDrag(::GetSystemMetrics(SM_CXDRAG), ::GetSystemMetrics(SM_CYDRAG));
	BOOL bDrag = FALSE;
	CSize sizeEdge(1, 1);

	UINT nTimer = SetTimer(1, MST_TIMER_PERIOD, NULL);	//for scroll
	CPoint ptScr(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));
	CRect rect(0, 0, 0, 0);
	UINT h = 0;
	HTREEITEM hItem = GetRootItem();
	if (hItem)
	{
		GetItemRect(hItem, &rect, FALSE);
		ptScr.y *= (h = rect.Height());		//this assumes equal height items
	}
	point += ptScr;

	while (!bDone && ::GetMessage(&msg, NULL, 0, 0))
	{
		if (::GetCapture() != m_hWnd) {
			break;
		}

		switch (msg.message)
		{
			case WM_TIMER:
				pt = msg.pt;
				ScreenToClient(&pt);
				if (rectCli.PtInRect(pt))
				{
					::DispatchMessage(&msg);
					break;
				}
				msg.lParam = MAKELPARAM(pt.x, pt.y);
				//fall through to mousemove

			case WM_MOUSEMOVE:
				pt.x = GET_X_LPARAM(msg.lParam);
				pt.y = GET_Y_LPARAM(msg.lParam);
				if (!bDrag)
				{
					if ((abs(pt.x - point.x) <= sizeDrag.cx) &&
						((abs(pt.y - point.y) <= sizeDrag.cy))) {
						break;
					}
					bDrag = TRUE;
					if (!(nFlags & (MK_CONTROL|MK_SHIFT))) {
						SelectAll(FALSE);
					}
					UpdateWindow();
					rect.SetRect(point, point);
					dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);
				}

				dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);	//delete

				if (pt.y < rectCli.top) {
					::SendMessage(m_hWnd, WM_VSCROLL, SB_LINEUP, 0);
				}
				else if (pt.y >= rectCli.bottom) {
					::SendMessage(m_hWnd, WM_VSCROLL, SB_LINEDOWN, 0);
				}
				if (pt.x < rectCli.left) {
					::SendMessage(m_hWnd, WM_HSCROLL, SB_LINELEFT, 0);
				}
				else if (pt.x >= rectCli.right) {
					::SendMessage(m_hWnd, WM_HSCROLL, SB_LINERIGHT, 0);
				}

				ptScr = point;
				ptScr.x -= GetScrollPos(SB_HORZ);
				ptScr.y -= GetScrollPos(SB_VERT) * h;
				rect.SetRect(ptScr, pt);
				rect.NormalizeRect();
				UpdateSelectionForRect(rect, nFlags, list);
				dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);	//draw
				break;

			case WM_LBUTTONUP:
			case WM_RBUTTONUP:
				bDone = TRUE;
				break;

			case WM_KEYDOWN:
				if (LOWORD(msg.wParam) == VK_ESCAPE)
				{
					SelectAll(FALSE);
					bDone = TRUE;
					break;
				}
				//dispatch

			default:
				::DispatchMessage(&msg);
				break;
		}
	}
	KillTimer(nTimer);
	::ReleaseCapture();

	if (bDrag) {
		dc.DrawDragRect(rect, sizeEdge, NULL, sizeEdge);
	}
	else {
		if (!(nFlags & (MK_CONTROL|MK_SHIFT))) {
			SelectAll(FALSE);
		}
	}
}

void CXTTreeCtrl::UpdateSelectionForRect(LPCRECT pRect, UINT nFlags, CTypedPtrList<CPtrList, HTREEITEM>& list)
{
	CRect rect;
	BOOL bSel;
	POSITION pos;

	HTREEITEM hItem = GetRootItem();
	while (hItem)
	{
		bSel = IsSelected(hItem);
		GetItemRect(hItem, &rect, m_bBandLabel);
		if (rect.IntersectRect(rect, pRect))
		{
			//item in rect
			pos = list.Find(hItem);
			if (!bSel && !pos) {
				SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			}
			else if ((nFlags & MK_CONTROL) && pos) {
				SetItemState(hItem, 0, TVIS_SELECTED);
			}
			else if ((nFlags & MK_SHIFT) && pos) {
				list.RemoveAt(pos);		//if shift and in rect, don't lock anymore
			}
		}
		else
		{
			//item not in rect
			pos = list.Find(hItem);
			if (bSel && !pos) {
				SetItemState(hItem, 0, TVIS_SELECTED);
			}
			else if (pos) {
				SetItemState(hItem, TVIS_SELECTED, TVIS_SELECTED);
			}
		}
		hItem = GetNextVisibleItem(hItem);
	}
	UpdateWindow();
}

BOOL CXTTreeCtrl::SelectChildren(HTREEITEM hParent, BOOL bSelect /*= TRUE*/, BOOL bRecurse /*= TRUE*/)
{
	UINT nS = bSelect ? TVIS_SELECTED : 0;

	BOOL bFocusWasInHere = FALSE;

	HTREEITEM hItem = GetNextItem(hParent, TVGN_CHILD);
	while (hItem)
	{
		UINT nState = GetItemState(hItem, TVIS_SELECTED|TVIS_EXPANDED|TVIS_FOCUSED);
		if ((nState & TVIS_SELECTED) != nS) {
			SetItemState(hItem, nS, TVIS_SELECTED);
		}
		bFocusWasInHere |= (nState & TVIS_FOCUSED);
		if (bRecurse && (nState & TVIS_EXPANDED)) {
			bFocusWasInHere |= SelectChildren(hItem, bSelect, bRecurse);
		}
		hItem = GetNextSiblingItem(hItem);
	}
	return bFocusWasInHere;
}

void CXTTreeCtrl::GetSelectedList(CTypedPtrList<CPtrList, HTREEITEM>& list) const
{
	list.RemoveAll();

	HTREEITEM hItem = GetFirstSelectedItem();
	while (hItem)
	{
		list.AddTail(hItem);
		hItem = GetNextSelectedItem(hItem);
	}
}

BOOL CXTTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pNMHDR = (NMHDR*)lParam;
	
	switch(pNMHDR->code)
	{
	case TTN_SHOW:
		{
			// get a pointer to the tooltip control.
			HWND hWnd = TreeView_GetToolTips( m_hWnd );
			if ( hWnd != NULL )
			{
				// make sure the tooltip is at the top of the "Z" order, otherwise
				// it will appear behind popup windows....
				::SetWindowPos( hWnd, HWND_TOP, 0,0,0,0,
					SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
			}
		}
		break;
	}
	
	return CTreeCtrl::OnNotify(wParam, lParam, pResult);
}

BOOL CXTTreeCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// If the tree control has an edit control, don't allow
	// the framework to process keystrokes, let the edit
	// control handle it instead...
	
	CEdit* pEditCtrl = GetEditControl();
	if (pEditCtrl && ::IsWindow(pEditCtrl->m_hWnd))
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		
		return TRUE;
	}
	
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CXTTreeCtrl::OnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// Remove the tree item from the map.
	m_mapColorFont.RemoveKey(pNMTreeView->itemOld.hItem);
	
	*pResult = 0;
}
