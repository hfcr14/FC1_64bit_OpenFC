// XTOutlookBar.cpp : implementation of the CXTOutlookBar class.
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
#include "XTOutlookBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTOutlookBar
/////////////////////////////////////////////////////////////////////////////

CXTOutlookBar::CXTOutlookBar()
{
	m_bUserColors = false;
    m_bHilight = true;
    m_clrBack  = xtAfxData.bXPMode ? xtAfxData.clrXPBarFace : xtAfxData.clr3DShadow;
    m_clrText  = xtAfxData.bXPMode ? xtAfxData.clrBtnText  : xtAfxData.clrWindow;
	m_cxIcon   = ::GetSystemMetrics(SM_CXICON);
	m_cyIcon   = ::GetSystemMetrics(SM_CYICON);
}

CXTOutlookBar::~CXTOutlookBar()
{
	while(!m_arContentItems.IsEmpty())
	{
		XT_CONTENT_ITEM* pContentItems = m_arContentItems.RemoveHead();
		delete pContentItems;
	}
}

IMPLEMENT_DYNAMIC(CXTOutlookBar, CListBox)

BEGIN_MESSAGE_MAP(CXTOutlookBar, CListBox)
	//{{AFX_MSG_MAP(CXTOutlookBar)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Checks mouse movement, sets point member, sets timer.

void CXTOutlookBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_point = point;
	SetTimer (1, 10, NULL);
	CListBox::OnMouseMove(nFlags, point);
}

void CXTOutlookBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CListBox::OnLButtonUp(nFlags, point);

	CWnd* pWndOwner = GetOwner();
	ASSERT(pWndOwner);

	pWndOwner->SendMessage(XTWM_OUTBAR_NOTIFY,
        (LPARAM)GetCurSel(), (WPARAM)GetDlgCtrlID());
}

// Sets the highlight flag based on cursor position.

void CXTOutlookBar::OnTimer(UINT_PTR /*nIDEvent*/)
{
    BOOL   bOutSide;
    CRect  rItem;
	CRect  rWindow;
	CPoint pt;
    int    nIndex;

    // Get the rect of the current window,
	// get the current cursor position.
	GetWindowRect (rWindow);
    GetCursorPos (&pt);

    // If the mouse is in the rect of this window,
	// set highlight flag to true, get the item that
	// the mouse is over.
	if (rWindow.PtInRect (pt))
	{
		m_bHilight = true;
		nIndex = ItemFromPoint (m_point, bOutSide);

		if (nIndex != GetCurSel() && !bOutSide)
		{
			SetCurSel( nIndex );
		}
    }

    // If the mouse is not in the rect of
	// this window, kill the timer, set highlight flag
	// to false, and return.
	else
	{
		m_bHilight = false;

        if (::GetKeyState(VK_LBUTTON) < 0)
		{
          SetCurSel (-1);
        }

        KillTimer(1);
	}

	Invalidate();
	UpdateWindow();
}

// Initial draw routine, draws button on mouse over,
// on mouse press, and on mouse out.

void CXTOutlookBar::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	// Define temporary variables.
    CDC*  pDC    = CDC::FromHandle (lpDIS->hDC);
	CRect rcIcon = lpDIS->rcItem;
	CRect rcText = lpDIS->rcItem;

	CPoint point;
	point.x = (rcIcon.Width()-m_cxIcon)/2;
	point.y = (rcIcon.top + 15);

	// Get the menu item.
	XT_CONTENT_ITEM* pContentItems = GetMenuItem(lpDIS->itemID);
	ASSERT(pContentItems);

	// save the current device context.
	int nSavedDC = pDC->SaveDC();

    // set the text foreground and background colors.
	pDC->SetTextColor (GetTextColor());
	pDC->SetBkColor (GetBackColor());

	// repaint the background.
	pDC->FillSolidRect (rcIcon, GetBackColor());
	pDC->SetBkMode (TRANSPARENT);

	// select the font used by the button text.
	pDC->SelectObject(GetFont());

	// define the text rect.
	rcText.DeflateRect (7,52,7,0);

    rcIcon.top    += 13;
    rcIcon.left    = (rcIcon.Width()-(m_cxIcon+4))/2;
    rcIcon.right   = rcIcon.left+(m_cxIcon+4);
    rcIcon.bottom -= 14;
    
    COLORREF clrShadow = xtAfxData.bXPMode ?
        xtAfxData.clrHighlight : xtAfxData.clr3DDkShadow;
    
    COLORREF clrHilite = xtAfxData.bXPMode ?
        xtAfxData.clrHighlight : xtAfxData.clr3DHilight;
    
	if (lpDIS->itemState & ODS_SELECTED && m_bHilight)
	{
		COLORREF clrHighlightDark = xtAfxData.bXPMode ?
			xtAfxData.clrXPHighlightPushed : GetBackColor();
		
		COLORREF clrHighlightLite = xtAfxData.bXPMode ?
			xtAfxData.clrXPHighlightChecked : GetBackColor();

        if (::GetKeyState(VK_LBUTTON) < 0)
        {
            pDC->FillSolidRect(&rcIcon, clrHighlightDark);
            pDC->Draw3dRect(&rcIcon, clrShadow, clrHilite);
        }
        else
        {
            pDC->FillSolidRect(&rcIcon, clrHighlightLite);
            pDC->Draw3dRect(&rcIcon, clrHilite, clrShadow);
        }
	}

	// draw the icon and the text associated with the button.
    pDC->DrawIcon(point, pContentItems->m_hIcon);
    pDC->DrawText(pContentItems->m_strText, rcText,
		DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);

	// restore the device context and free the icon.
	pDC->RestoreDC (nSavedDC);
}

// Sets item height

void CXTOutlookBar::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = m_cyIcon*2;
}

void CXTOutlookBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CListBox::OnWindowPosChanged(lpwndpos);
	InvalidateRect(NULL);
}

BOOL CXTOutlookBar::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// Make sure that the control is owner drawn.
	dwStyle |= LBS_OWNERDRAWVARIABLE | LBS_NOINTEGRALHEIGHT;

	if (!CListBox::Create(dwStyle, rect, pParentWnd, nID))
		return FALSE;

	// Set the font used by the Outlook bar.
	SetFont(CFont::FromHandle((HFONT)
		::GetStockObject(DEFAULT_GUI_FONT)));

	return TRUE;
}

int CXTOutlookBar::AddMenuItem(UINT nIconID, LPCTSTR lpszText)
{
	XT_CONTENT_ITEM* pContentItems = new XT_CONTENT_ITEM;
	ASSERT(pContentItems != NULL);

	pContentItems->m_nIndex  = AddString(lpszText);
	pContentItems->m_strText = lpszText;

	// Load the icon for the menu item.
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nIconID), RT_GROUP_ICON);
	pContentItems->m_hIcon = (HICON)::LoadImage(hInst, MAKEINTRESOURCE(nIconID), 
		IMAGE_ICON, m_cxIcon, m_cyIcon, LR_DEFAULTCOLOR);

	m_arContentItems.AddTail(pContentItems);

	return pContentItems->m_nIndex;
}

int CXTOutlookBar::InsertMenuItem(int iIndex, UINT nIconID, LPCTSTR lpszText)
{
	XT_CONTENT_ITEM* pContentItems = new XT_CONTENT_ITEM;
	ASSERT(pContentItems != NULL);

	pContentItems->m_nIndex  = InsertString(iIndex, lpszText);
	pContentItems->m_strText = lpszText;

	// Load the icon for the menu item.
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nIconID), RT_GROUP_ICON);
	pContentItems->m_hIcon = (HICON)::LoadImage(hInst, MAKEINTRESOURCE(nIconID), 
		IMAGE_ICON, m_cxIcon, m_cyIcon, LR_DEFAULTCOLOR);

	POSITION pos = m_arContentItems.FindIndex(pContentItems->m_nIndex);
	if (pos != NULL) {
		m_arContentItems.InsertBefore(pos, pContentItems);
	}
	else {
		m_arContentItems.AddTail(pContentItems);
	}

	return pContentItems->m_nIndex;
}

BOOL CXTOutlookBar::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}

void CXTOutlookBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	OnNoFlickerPaint(&dc);
}

void CXTOutlookBar::OnNoFlickerPaint(CDC* pDC)
{
	ASSERT(pDC != NULL);
	
	// Get the client area to paint.
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC; // memory device context.

    // Create the memory DC, set Map Mode
	memDC.CreateCompatibleDC(pDC);
	memDC.SetMapMode(pDC->GetMapMode());

    // Create a bitmap big enough to hold the window's image
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());

    // Select the bitmap into the memory DC
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// Repaint the background, this takes the place of WM_ERASEBKGND.
	memDC.FillSolidRect(rcClient, GetBackColor());

	// Let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	// Blt it
	pDC->BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
		&memDC, rcClient.left, rcClient.top, SRCCOPY);            

    // Select the original bitmap back in
	memDC.SelectObject(pOldBitmap);

	// Clean up
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

XT_CONTENT_ITEM* CXTOutlookBar::GetMenuItem(int iItem)
{
	// Get the menu item.
	POSITION pos = m_arContentItems.FindIndex(iItem);
	XT_CONTENT_ITEM* pContentItems = m_arContentItems.GetAt(pos);
	ASSERT(pContentItems);

	return pContentItems;
}

COLORREF CXTOutlookBar::GetBackColor()
{
	if (m_bUserColors) {
		return m_clrBack;
	}
	else {
		return xtAfxData.bXPMode ? xtAfxData.clrXPBarFace : xtAfxData.clr3DShadow;
	}
}

COLORREF CXTOutlookBar::GetTextColor()
{
	if (m_bUserColors) {
		return m_clrText;
	}
	else {
		return xtAfxData.bXPMode ? xtAfxData.clrBtnText  : xtAfxData.clrWindow;
	}
}
