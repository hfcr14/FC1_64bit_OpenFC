// XTButton.cpp : implementation of the CXTButton class.
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
#include "XTButton.h"
#include "XTMemDC.h"
#include "XTHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int BS_XT_FLAT_ANY = (BS_XT_FLAT | BS_XT_SEMIFLAT | BS_XT_XPFLAT);

//#define __XPMSWORD__

/////////////////////////////////////////////////////////////////////////////
// CXTButton
/////////////////////////////////////////////////////////////////////////////
CXTButton::CXTButton()
{
	m_pFont           = NULL;
	m_hBitmap		  = NULL;
	m_hBitmapMono	  = NULL;
	m_hIcon			  = NULL;
	m_hIconPushed	  = NULL;
	m_sizeImage		  = CSize(0,0);
	m_ptImage		  = CPoint(0,0);
	m_ptText		  = CPoint(0,0);
	m_nBorderGap	  = 4;
	m_nImageGap		  = 8;
	m_dwxStyle        = BS_XT_FLAT;
	m_bUserPosition	  = FALSE;
	m_bLBtnDown		  = FALSE;
	m_bAltColor		  = FALSE;
	m_bPainted		  = FALSE;
	m_bChecked        = FALSE;
    m_bHilite         = FALSE;
    m_bXPFUserColors  = FALSE;
	m_clrBtnText      = ::GetSysColor(COLOR_BTNTEXT);
	m_clr3DFace       = ::GetSysColor(COLOR_3DFACE);
	m_clr3DHilight    = ::GetSysColor(COLOR_3DHILIGHT);
	m_clr3DShadow     = ::GetSysColor(COLOR_3DSHADOW);
	m_dwInitSignature = 1;
}

CXTButton::~CXTButton()
{
	CleanUpGDI();
}

IMPLEMENT_DYNAMIC(CXTButton, CButton)

BEGIN_MESSAGE_MAP(CXTButton, CButton)
	//{{AFX_MSG_MAP(CXTButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

void CXTButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
    NoteButtonDepressed(TRUE);
	CButton::OnLButtonDown(nFlags, point);
}

void CXTButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
    NoteButtonDepressed(FALSE);
	CButton::OnLButtonUp(nFlags, point);
}

void CXTButton::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		CRect rc;
		GetWindowRect(rc);

		CPoint pt;
		GetCursorPos(&pt);

		if (m_bLBtnDown == TRUE || (!rc.PtInRect(pt)))
		{
			KillTimer(1);

            m_bHilite = FALSE;

            if (m_bPainted == TRUE) {
				InvalidateRect (NULL);
			}

			m_bPainted = FALSE;
			return;
		}

		// On mouse over, show raised button.
		else if ((GetXButtonStyle() & BS_XT_FLAT_ANY) && !m_bPainted)
		{
            m_bHilite = TRUE;
			InvalidateRect(NULL);
			m_bPainted = true;
		}
	}
	
	CButton::OnTimer(nIDEvent);
}

void CXTButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS != NULL);

	// define some temporary variables.
	CDC*  pDC	    = CDC::FromHandle( lpDIS->hDC );
	CRect rcItem    = lpDIS->rcItem;
	int   nState    = lpDIS->itemState;
	int   nSavedDC  = pDC->SaveDC();
	bool  bSelected = ((nState & ODS_SELECTED) == ODS_SELECTED);

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	// Paint the background.
	COLORREF clrBackground = GetColorFace();
	pDC->FillSolidRect(rcItem, clrBackground);

    DWORD dwxStyle = GetXButtonStyle();
	if (dwxStyle & BS_XT_HILITEPRESSED)
	{
		if (m_bChecked)
		{
			if (!rcItem.PtInRect(pt)) {
				_xtAfxDrawShadedRect(pDC, rcItem);
			}
		}
		else if (bSelected) {
			_xtAfxDrawShadedRect(pDC, rcItem);
		}
	}

	if( dwxStyle & BS_XT_FLAT )
	{
        if (m_bHilite  ||  bSelected)
        {
        	pDC->Draw3dRect( rcItem, 
                bSelected ? m_clr3DShadow  : m_clr3DHilight,
    			bSelected ? m_clr3DHilight : m_clr3DShadow );
        }
	}
    else if (dwxStyle & BS_XT_XPFLAT)
    {
#ifdef __XPMSWORD__
         pDC -> Draw3dRect ( rcItem, GetSysColor ( COLOR_3DSHADOW ), GetSysColor ( COLOR_3DSHADOW ) );
#endif
        if (m_bHilite  ||  bSelected)
        {
            COLORREF clrXPFBorder, clrXPFPressed, clrXPFHighlight;
            if (m_bXPFUserColors)
            {
                clrXPFBorder    = m_clrXPFBorder;
                clrXPFPressed   = m_clrXPFPressed;
                clrXPFHighlight = m_clrXPFHighlight;
            }
            else
            {
                clrXPFBorder    = xtAfxData.clrHighlight;
                clrXPFPressed   = xtAfxData.clrXPHighlightPushed;
                clrXPFHighlight = xtAfxData.clrXPHighlight;
            }
            pDC->Draw3dRect( rcItem, clrXPFBorder, clrXPFBorder );
            CRect rcInner = rcItem;
            rcInner.DeflateRect(1, 1);
            // fill inside
			clrBackground = (bSelected) ?  clrXPFPressed : clrXPFHighlight;
            pDC->FillSolidRect(rcInner, clrBackground);
        }
    }
	else if (dwxStyle & BS_XT_SEMIFLAT) 
    {
		pDC->Draw3dRect( rcItem, 
            bSelected ? xtAfxData.clr3DShadow  : xtAfxData.clr3DHilight,
			bSelected ? xtAfxData.clr3DHilight : xtAfxData.clr3DShadow);
	}
	else // normal
	{
		pDC->Draw3dRect( rcItem, bSelected ? xtAfxData.clr3DDkShadow : m_clr3DHilight,
			bSelected ? m_clr3DHilight : xtAfxData.clr3DDkShadow);
		rcItem.DeflateRect(1,1);
		pDC->Draw3dRect( rcItem, bSelected ? m_clr3DShadow : GetColorFace(),
			bSelected ? GetColorFace() : m_clr3DShadow);
	}

	// Set the background mode to transparent.
    CXTContextBkModeHandler modeHandler(pDC, TRANSPARENT);
    CXTContextBkColorHandler backHandler(pDC, clrBackground);
	
	// draw the button text, icon or bitmap.
	DrawButtonText(pDC, nState, rcItem);
	DrawButtonIcon(pDC, nState, rcItem);
	DrawButtonBitmap(pDC, nState, rcItem);

	// Draw the focus rect if style is set and we have focus.
	if ((GetXButtonStyle() & BS_XT_SHOWFOCUS) && (nState & ODS_FOCUS))
	{
#ifdef __XPMSWORD__
      if ( dwxStyle & BS_XT_XPFLAT )
      {
         if ( ! ( m_bHilite || bSelected ) )
         {
            COLORREF    border   = m_bXPFUserColors ? m_clrXPFBorder : xtAfxData.clrHighlight;
            pDC -> Draw3dRect ( rcItem, border, border );
            rcItem.DeflateRect ( 1, 1 );
            pDC -> Draw3dRect ( rcItem, border, border );
         }
      }
      else
      {
#endif
		CXTContextTextColorHandler foreHandler(pDC, RGB( 0,0,0));
		rcItem.DeflateRect(2,2);
		pDC->DrawFocusRect(rcItem);
#ifdef __XPMSWORD__
      }
#endif
	}

	pDC->RestoreDC(nSavedDC);
}

CString CXTButton::GetButtonText(BOOL bRemoveAmpersand)
{
	CString strText;
	GetWindowText(strText);
	
	if (bRemoveAmpersand) {
		_xtAfxStripMnemonics(strText);
	}

	return strText;
}

void CXTButton::DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem)
{
	// Get the window text, return if empty.
	CString strText = GetButtonText(TRUE);

	if( !strText.IsEmpty() )
	{
		CFont *oldFont = pDC->SelectObject(GetFont());
		CSize sizeText = pDC->GetTextExtent(strText);
		CRect rcText = rcItem;
		rcText.DeflateRect(2,0);
		CPoint point = rcText.TopLeft();

		if (m_bUserPosition) {
			point = m_ptText;
		}
		else
		{
			OffsetPoint(point, sizeText);
			point.x = m_nBorderGap;
			
			DWORD dwStyle = GetStyle();
			if (GetXButtonStyle() & BS_XT_TWOROWS) {
				point.y += (m_sizeImage.cy/2)+(m_nImageGap/2);
			} 

			// horz. alignment
			if ((dwStyle & BS_CENTER) == BS_CENTER	||
				(dwStyle & BS_CENTER) == 0)
			{
				if (!(GetXButtonStyle() & BS_XT_TWOROWS))
				{
					point.x += m_sizeImage.cx;
				}
				else
				{
//					point.x += m_nBorderGap;
				}
				point.x += (rcItem.right - point.x - sizeText.cx - m_nBorderGap) / 2;
			}
			else if (dwStyle & BS_LEFT)
			{
				if (!(GetXButtonStyle() & BS_XT_TWOROWS))
				{
					point.x += m_sizeImage.cx + m_nImageGap;
				}
			}
			else // right
			{
				point.x = rcItem.right - m_nBorderGap - sizeText.cx;
			}
		}

		// if the button is selected.
		if( nState & ODS_SELECTED )
		{
			if ( ! ( GetXButtonStyle () & BS_XT_XPFLAT ) ) {
				point.Offset(1,1);
			}
		}

		// Set the draw state flags.
		DWORD dwFlags = DST_PREFIXTEXT;
		if( nState & ODS_DISABLED ) {
			pDC->SetTextColor(xtAfxData.clrGrayText);
		}
		else
		{
			dwFlags |= DSS_NORMAL;
			if ( ( GetXButtonStyle () & BS_XT_XPFLAT ) && ( nState & ODS_SELECTED ) ) {
				pDC -> SetTextColor( xtAfxData.clrXPHighlightText );
			}
			else {
				pDC->SetTextColor(m_clrBtnText);
			}
		}

		// draw the text, and select the original font.
		strText = GetButtonText(FALSE);
        pDC->SetTextAlign(TA_LEFT);
		pDC->DrawState( point, sizeText, strText, dwFlags, TRUE, strText.GetLength(), (HBRUSH)NULL );
		pDC->SelectObject( oldFont );
	}
}

CPoint CXTButton::CalculateImagePosition(CDC* pDC, UINT nState, 
    CRect& rcItem, bool bHasPushedImage)
{
	// Get the window text, return if empty.
	CString strText  = GetButtonText(TRUE);
	CSize   sizeText = pDC->GetTextExtent(strText);
	CPoint  point    = rcItem.TopLeft();

	DWORD dwStyle = GetStyle();
	
	if (m_bUserPosition) {
		point = m_ptImage;
	}
	else
	{
		OffsetPoint(point, m_sizeImage);
        point.x = m_nBorderGap;

        if (GetXButtonStyle() & BS_XT_TWOROWS) 
        {
            if (!strText.IsEmpty())
            {
				point.y -= ((sizeText.cy/2)+(m_nImageGap/2));
            }
			
            // horz. alignment - only for two-row mode, otherwise always
            // on left edge
            if ((dwStyle & BS_CENTER) == BS_CENTER  ||
                (dwStyle & BS_CENTER) == 0)
            {
                point.x = rcItem.left +
                    __max(0,((rcItem.Width()-m_sizeImage.cx)/2));
            }
            else if (dwStyle & BS_RIGHT)
            {
				point.x = rcItem.right - m_nBorderGap - m_sizeImage.cx;
            }
            else // left
            {
                // already left aligned
            }
		}
		else
		{
			if (strText.IsEmpty())
			{
				if ((dwStyle & BS_CENTER) == BS_CENTER)
	            {
					int cx = rcItem.Width();
					if (cx >= m_sizeImage.cx)
					{
						point.x = (cx - m_sizeImage.cx)/2;
					}
				}
				if  ((dwStyle & BS_VCENTER) == BS_VCENTER)
				{
					int cy = rcItem.Height();
					if (cy >= m_sizeImage.cy)
					{
						point.y = (cy - m_sizeImage.cy)/2;
					}
				}
			}
		}
	}

	// if the button is selected.
	if( nState & ODS_SELECTED )
	{
		if( !bHasPushedImage ) 
        {
			point.Offset(1,1);
		}
	}

    return point;
}

void CXTButton::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem)
{
	// get the handle to the icon (if any) associated
	// with this button.
	HICON hIcon = m_hIcon;
	if (hIcon)
	{
        CPoint point = CalculateImagePosition(pDC, nState, rcItem,
            (m_hIconPushed != NULL));

		if( m_hIconPushed && (nState & ODS_SELECTED) ) 
        {
			hIcon = m_hIconPushed;
		}

		// Set the draw state flags.
		DWORD dwFlags = DST_ICON;
		if( nState & ODS_DISABLED ) {
			dwFlags |= DSS_DISABLED;
		} else {
			dwFlags |= DSS_NORMAL;
		}

		// draw the icon associated with this button.
		pDC->DrawState( point, m_sizeImage, hIcon, dwFlags, (HBRUSH)NULL );
	}
}

void CXTButton::DrawButtonBitmap(CDC* pDC, UINT nState, CRect& rcItem)
{
	// get the handle to the icon (if any) associated
	// with this button.
	HBITMAP hBitmap = m_hBitmap;

	if (nState & ODS_DISABLED) {
		hBitmap = m_hBitmapMono;
	}

	if (hBitmap)
	{
        CPoint point = CalculateImagePosition(pDC, nState, rcItem, false);

		// Set the draw state flags.
		DWORD dwFlags = DST_BITMAP;
		if( nState & ODS_DISABLED ) {
			dwFlags |= DSS_DISABLED;
		} else {
			dwFlags |= DSS_NORMAL;
		}

		// draw the icon associated with this button.
		pDC->DrawState( point, m_sizeImage, hBitmap, dwFlags, (HBRUSH)NULL );
	}
}

void CXTButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	POINT pt;

	::GetCursorPos(&pt);
	::MapWindowPoints(NULL,(HWND)(m_hWnd),&pt,1);
	::SendMessage((HWND)(m_hWnd),WM_LBUTTONDOWN,0,MAKELPARAM(pt.x,pt.y)); 
	
	CButton::OnLButtonDblClk(nFlags, point);
}

void CXTButton::OnSysColorChange() 
{
	CButton::OnSysColorChange();

	if(m_bAltColor == FALSE)
	{
		m_clr3DFace     = ::GetSysColor(COLOR_3DFACE);
		m_clr3DShadow   = ::GetSysColor(COLOR_3DSHADOW);
		m_clr3DHilight  = ::GetSysColor(COLOR_3DHILIGHT);
		m_clrBtnText    = ::GetSysColor(COLOR_BTNTEXT);
	}
}

LRESULT CXTButton::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	
	if ((DWORD)lParam == m_dwInitSignature)
	{
		// note init completed
		m_dwInitSignature = 0;

		// Set the font, and change the style to BS_OWNERDRAW.
		ModifyStyle(0, BS_OWNERDRAW);
		
		// Check to see if the font is initialized, if not set it to the
		// default font used by the toolkit.
		if (m_pFont == NULL || m_pFont->GetSafeHandle() == NULL)
		{
			m_pFont = &xtAfxData.font;
		}

		SetFont(m_pFont);
	}
	
	return 0;
}

void CXTButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	
	// Initialize the control.
	DeferInitialUpdate();
}

void CXTButton::DeferInitialUpdate()
{
	if (m_dwInitSignature > 0)
	{
		PostMessage(XTWM_INITIAL_UPDATE, 0, ++m_dwInitSignature);
	}
}

int CXTButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	DeferInitialUpdate();
	
	return 0;
}

void CXTButton::ResetImage()
{
    if (m_sizeImage == CSize(0, 0)) return;
    CleanUpGDI();
    m_sizeImage = CSize(0,0);
    m_ptImage   = CPoint(0,0);
    m_ptText    = CPoint(0,0);
    Invalidate();
}

void CXTButton::CleanUpGDI()
{
    if (m_hIcon)
    {
        ::DestroyIcon(m_hIcon);
        m_hIcon = NULL;
    }
    if (m_hIconPushed)
    {
        ::DestroyIcon(m_hIconPushed);
        m_hIconPushed = NULL;
    }

    AfxDeleteObject((HGDIOBJ*)&m_hBitmap);
    AfxDeleteObject((HGDIOBJ*)&m_hBitmapMono);
    m_hBitmap = NULL;
    m_hBitmapMono = NULL;
}

BOOL CXTButton::SetIcon(CSize size, HICON hIcon, HICON hIconPushed/*=NULL*/, BOOL bRedraw/*=TRUE*/)
{
	// Save the image size and set the icon
	// handles to NULL.
	m_hIcon	      = hIcon;
	m_hIconPushed = hIconPushed;
	m_sizeImage	  = size;

	// Redraw the button.
	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}

	return TRUE;
}

BOOL CXTButton::SetIcon(CSize size, UINT nID, UINT nPushedID/*=0*/, BOOL bRedraw/*=TRUE*/)
{
	UNUSED_ALWAYS(bRedraw);
	return CXTButton::SetIcon(size, MAKEINTRESOURCE(nID),
		MAKEINTRESOURCE(nPushedID));
}

BOOL CXTButton::SetIcon(CSize size, LPCTSTR lpszID, LPCTSTR lpszPushedID/*=NULL*/, BOOL bRedraw/*=TRUE*/)
{
	// Free previous resources (if any).
	CleanUpGDI();

	// Find the resource for the normal icon and load the image.
	HINSTANCE hInst = AfxFindResourceHandle(lpszID, RT_GROUP_ICON);
	m_hIcon = (HICON)::LoadImage(hInst, lpszID, 
		IMAGE_ICON, size.cx, size.cy, LR_DEFAULTCOLOR);

	// Return false if the icon handle is NULL.
	if (!m_hIcon) {
		TRACE1( "Failed to load Icon resource %s.\n", lpszID );
		return FALSE;
	}

	// If we are using a pushed image as well...
	if (lpszPushedID) 
	{
		// Find the resource for the pushed icon and load the image.
		hInst = AfxFindResourceHandle(lpszPushedID, RT_GROUP_ICON);
		m_hIconPushed = (HICON)::LoadImage(hInst, lpszPushedID, 
			IMAGE_ICON, size.cx, size.cy, LR_DEFAULTCOLOR);

		// Return false if the icon handle is NULL.
		if (!m_hIconPushed) {
			TRACE0( "Failed to load Icon resource.\n" );
			return FALSE;
		}
	}

	// Save the image size.
	m_sizeImage = size;

	// Redraw the button.
	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}

	return TRUE;
}

BOOL CXTButton::SetBitmap(CSize size, UINT nID, BOOL bRedraw/*=TRUE*/)
{
	// Free previous resources (if any).
	CleanUpGDI();

	// Get the resource handle for the toolbar bitmap.
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(nID), RT_BITMAP);
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), RT_BITMAP);

	if (!hRsrc) {
		return FALSE;
	}

	AfxDeleteObject((HGDIOBJ*)&m_hBitmap);
	AfxDeleteObject((HGDIOBJ*)&m_hBitmapMono);

	// Load the normal bitmap.
	m_hBitmap = _xtAfxLoadSysColorBitmap(hInst, hRsrc, FALSE);
	
	// Load the disabled bitmap
	m_hBitmapMono = _xtAfxLoadSysColorBitmap(hInst, hRsrc, TRUE);

	// Return false if the bitmap handle is NULL.
	if (!m_hBitmap || !m_hBitmapMono)
	{
		TRACE1( "Failed to load Bitmap resource %d.\n", nID );
		return FALSE;
	}

	// Save the image size.
	m_sizeImage = size;

	// Redraw the button.
	if (GetSafeHwnd() && bRedraw)
	{
		Invalidate();
	}

	return TRUE;
}

BOOL CXTButton::SetTextAndImagePos(CPoint ptImage, CPoint ptText, BOOL bRedraw/*=TRUE*/)
{
	m_ptImage = ptImage;
	m_ptText  = ptText;

	// Redraw the button.
	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}

	m_bUserPosition = TRUE;
	return TRUE;
}

void CXTButton::SetAlternateColors(COLORREF clr3DFace, COLORREF clr3DHilight, COLORREF clr3DShadow, COLORREF clrBtnText)
{
	SetColorFace(clr3DFace);
	SetColorHilite(clr3DHilight);
	SetColorShadow(clr3DShadow);
	SetColorText(clrBtnText);
	m_bAltColor	= TRUE;
}

void CXTButton::SetXPFlatColors(COLORREF clrBorder, COLORREF clrHighlight,
    COLORREF clrPressed)
{
    m_clrXPFBorder    = clrBorder;
    m_clrXPFHighlight = clrHighlight;
    m_clrXPFPressed   = clrPressed;
    m_bXPFUserColors  = TRUE;
}


void CXTButton::SetColorFace(COLORREF clrFace)
{
	m_clr3DFace = clrFace;
}

void CXTButton::SetColorHilite(COLORREF clrHilite)
{
	m_clr3DHilight = clrHilite;
}

void CXTButton::SetColorShadow(COLORREF clrShadow)
{
	m_clr3DShadow = clrShadow;
}

void CXTButton::SetColorText(COLORREF clrText)
{
	m_clrBtnText = clrText;
}

DWORD CXTButton::SetXButtonStyle(DWORD dwxStyle, BOOL bRedraw/*=TRUE*/)
{
	// The BS_XT_CENTER style is obsolete, instead use
	// BS_CENTER style with CButton::Create(...), CButton::SetButtonStyle(...),
	// or using the resource editor.

	ASSERT((dwxStyle & BS_XT_CENTER) == 0);
	
	DWORD dwOldStyle = m_dwxStyle;
	m_dwxStyle = dwxStyle;

	// Redraw the button.
	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
	return dwOldStyle;
}

void CXTButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetXButtonStyle() & BS_XT_FLAT_ANY) {
		SetTimer(1, 10, NULL);
		OnTimer(1);
	}

	CButton::OnMouseMove(nFlags, point);
}

void CXTButton::OffsetPoint(CPoint& point, CSize size)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	if (GetXButtonStyle() & BS_XT_TWOROWS) {
        point.x = __max(0,((rcClient.Width()-size.cx)/2));
		point.y = __max(0,((rcClient.Height()-size.cy)/2));
	}
	else {
		point.x = m_nBorderGap;
		point.y = __max(0,((rcClient.Height()-size.cy)/2));
	}
}

void CXTButton::OnPaint() 
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient);

	// Now let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

BOOL CXTButton::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CXTButton::NoteButtonDepressed(BOOL bDepressed)
{
    if (m_bLBtnDown != bDepressed)
    {
        m_bLBtnDown = bDepressed;
        Invalidate();
    }
}

void CXTButton::OnDestroy()
{
	CButton::OnDestroy();
	
	// reset the initialization flag.
	m_dwInitSignature = 1;
}

CFont* CXTButton::SetFontEx(CFont* pFont)
{
	CFont* pOldFont = GetFont();

	if (pFont && pFont->GetSafeHandle())
	{
		m_pFont = pFont;
		if (::IsWindow(m_hWnd))
		{
			SetFont(m_pFont);
		}
	}

	return pOldFont;
}
