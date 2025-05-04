// XTCaption.cpp : implementation of the CXTCaption class.
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
#include "XTButton.h"
#include "XTCaption.h"
#include "XTCaptionPopupWnd.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTCaptionButton
/////////////////////////////////////////////////////////////////////////////

BOOL CXTCaptionButton::SetButtonStyle(COLORREF clrFace, COLORREF clrText)
{
	SetColorFace(clrFace);
	SetColorText(clrText);
	SetColorShadow(::GetSysColor(COLOR_3DDKSHADOW));

    DWORD dwXStyle = GetXButtonStyle();
    DWORD dwXStyleNew = dwXStyle & ~(BS_XT_XPFLAT | BS_XT_FLAT);
    
    if (xtAfxData.bXPMode) {
        dwXStyleNew |= BS_XT_XPFLAT;
    }
    else {
        dwXStyleNew |= BS_XT_FLAT;
    }

    if (dwXStyleNew != dwXStyle) {
        SetXButtonStyle(dwXStyleNew);
    }

	m_bAltColor = TRUE;

	return TRUE;
}

void CXTCaptionButton::DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem)
{
	CXTCaption* pWndParent = (CXTCaption*)GetParent();
	ASSERT_VALID(pWndParent);

	if ((pWndParent->m_dwExStyle & CPWS_EX_CLOSEBUTTON) == 0)
	{
		// Get the window text, return if empty.
		CString strWindowText; GetWindowText(strWindowText);
		if( strWindowText.IsEmpty( ))
			return;

		CXTCaption* pParent = (CXTCaption*)GetParent();
		ASSERT_VALID(pParent);

		CRect rcClient;
		GetWindowRect(&rcClient);
		pParent->ScreenToClient(&rcClient);

		CRect rcParent;
		pParent->GetClientRect(&rcParent);
		rcClient.left = rcParent.left + 9 - pParent->m_nBorder;

		// Set the text color and select the button font.
		pDC->SetTextColor( m_clrBtnText );
		CFont *oldFont = pDC->SelectObject( pParent->GetFont() );
		
		// get the height of the text to be displayed.
		int cy = pDC->DrawText( strWindowText, -1, rcClient, DT_SINGLELINE | DT_CALCRECT );
		rcClient.top = (rcItem.Height()-cy)/2;

		// draw the text, and select the original font.
		pDC->DrawState( rcClient.TopLeft(), strWindowText.GetLength(), strWindowText, 
			( nState & ODS_DISABLED )?DSS_DISABLED:DSS_NORMAL, TRUE, strWindowText.GetLength(), (HBRUSH)NULL );
		
		// Set up pen to use for drawing the triangle
		CPen pen(PS_SOLID, 1, m_clrBtnText);
		CPen *pOldPen = pDC->SelectObject( &pen );
		
		// get the client rect.
		CRect rcWnd;
		GetClientRect(&rcWnd);

		// initialize coordinates for triangle.
		int x1 = rcWnd.right-11;
		int x2 = rcWnd.right-6;
		int y1 = rcWnd.bottom-7;
		int y2 = rcWnd.bottom-5;

		// draw the triangle.
		pDC->MoveTo( x1, y1 );
		pDC->LineTo( x2, y1 );
		pDC->MoveTo( x1+1, y1+1 );
		pDC->LineTo( x2-1, y1+1 );
		pDC->SetPixel( x1+2, y2, m_clrBtnText );

		// restore old pen and font.
		pDC->SelectObject( oldFont );
		pDC->SelectObject( pOldPen );
	}
}

void CXTCaptionButton::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem)
{
	CXTCaption* pWndParent = (CXTCaption*)GetParent();
	ASSERT_VALID(pWndParent);

	if ((pWndParent->m_dwExStyle & CPWS_EX_CLOSEBUTTON) == CPWS_EX_CLOSEBUTTON)
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
			DWORD dwFlags = DST_ICON|DSS_MONO;
			if( nState & ODS_DISABLED )
			{
				dwFlags |= DSS_DISABLED;
			}
			else
			{
				dwFlags |= DSS_NORMAL;
			}

			COLORREF crText = xtAfxData.clrMenuText;
			
			if (xtAfxData.bXPMode)
			{
				if (nState & ODS_SELECTED)
				{
					crText = xtAfxData.clrXPHighlightText;
					point.Offset(-1,-1);
				}
				else
				{
					crText = xtAfxData.clrMenuText;
				}
			}

			CBrush brush(crText);

			// draw the icon associated with this button.
			pDC->DrawState( point, m_sizeImage, hIcon, dwFlags, (HBRUSH)brush );
		}
	}
	else
	{
		CXTButton::DrawButtonIcon(pDC, nState, rcItem);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTCaption

CXTCaption::CXTCaption()
{
	m_bUserColors   = false;
    m_strCaption    = _T("");
    m_hIcon         = NULL;
    m_pChildWnd     = NULL;
	m_pParentView   = NULL;
	m_pSplitterWnd  = NULL;
    m_pPopupWnd     = NULL;
    m_nOffset       = 0;
	m_nBorder       = 0;
	m_sizeIcon.cx   = __min(xtAfxData.cxSmIcon, 16);
    m_sizeIcon.cy   = __min(xtAfxData.cySmIcon, 16);
	m_dwExStyle     = 0L;
	m_dwTextStyle   = DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER;
	
	m_clrText   = ::GetSysColor(COLOR_BTNTEXT);
	m_clrBorder = ::GetSysColor(COLOR_3DFACE);
	m_clrFace   = ::GetSysColor(COLOR_3DFACE);
}

CXTCaption::~CXTCaption()
{

}

IMPLEMENT_DYNAMIC(CXTCaption, CStatic)

BEGIN_MESSAGE_MAP(CXTCaption, CStatic)
	//{{AFX_MSG_MAP(CXTCaption)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP

	ON_BN_CLICKED(XT_IDC_BTN_CLOSE, OnCaptButton)
	ON_MESSAGE_VOID(CPWN_XT_PUSHPINBUTTON, OnPushPinButton)
	ON_MESSAGE_VOID(CPWN_XT_PUSHPINCANCEL, OnPushPinCancel)

END_MESSAGE_MAP()

void CXTCaption::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient);

	// If the caption button is indeed a valid window and
	// is visible, exclude it from painting...
	if (::IsWindow(m_btnCaption.m_hWnd) &&
		m_btnCaption.IsWindowVisible())
	{
		CRect rcButton;
		m_btnCaption.GetWindowRect(&rcButton);
		ScreenToClient(&rcButton);
		memDC.ExcludeClipRect(&rcButton);
	}

	// draw the background, text and icon.
	DrawCaptionBack( &memDC, rectClient );
	DrawCaptionText( &memDC, rectClient );
	DrawCaptionIcon( &memDC, rectClient );
}

BOOL CXTCaption::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CXTCaption::DrawCaptionBack(CDC* pDC, CRect& rcItem)
{
	CRect rc = rcItem;

	// Set the background mode to transparent.
	pDC->SetBkMode( TRANSPARENT );

	// repaint the background.
	pDC->FillSolidRect( rc, m_clrBorder );

	// draw the caption border.
	if (m_dwExStyle & CPWS_EX_GROOVE_EDGE)
	{
		pDC->Draw3dRect(rc, xtAfxData.clr3DShadow, xtAfxData.clr3DHilight);
		rc.DeflateRect(1,1);
		pDC->Draw3dRect(rc, xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
		
		if (m_nBorder == 0) {
			rc.DeflateRect(1,1);
		}
	}

	else
	if (m_dwExStyle & CPWS_EX_RAISED_EDGE)
	{
		pDC->Draw3dRect(rc, xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
		
		if (m_nBorder == 0) {
			rc.DeflateRect(1,1);
		}
	}

	// draw the caption banner border.
	rc.DeflateRect( m_nBorder, m_nBorder );
	pDC->FillSolidRect(rc, m_clrFace);
}

void CXTCaption::DrawCaptionText(CDC* pDC, CRect& rcItem)
{
	// if the caption text is empty, return.
	if (m_strCaption.IsEmpty()) {
		return;
	}

	CRect rc = rcItem;
	rc.OffsetRect( 10,0 );

	// Set the text color and select the caption font.
	pDC->SetTextColor( m_clrText );
	CFont* oldFont = pDC->SelectObject( GetFont() );

	// get the height of the text to be displayed.
    int cy = pDC->DrawText( m_strCaption, -1, rc, DT_SINGLELINE|DT_CALCRECT );
	rc.top = (rcItem.Height()-cy)/2;
	rc.bottom = rc.top+cy;
	rc.right  = rcItem.right-(m_sizeIcon.cx + m_nBorder);

	// draw the text, and select the original font.
	pDC->DrawText( m_strCaption, rc, m_dwTextStyle );
	pDC->SelectObject( oldFont );
}

void CXTCaption::DrawCaptionIcon(CDC* pDC, CRect& rcItem)
{
	// If no icon was defined, just return.
	if( m_hIcon == NULL )
		return; 
	
	CRect rc = rcItem;
	rc.left = ( rc.right - m_sizeIcon.cx )-( m_nBorder + 2 ); 
	
	// stop the icon at the left hand border
	if (rc.left < m_nBorder) {
		rc.left = m_nBorder;
	}
	
	rc.top = ( rcItem.Height() - m_sizeIcon.cy )/2;
	rc.right  = rc.left + m_sizeIcon.cx;
	rc.bottom = rc.top  + m_sizeIcon.cy;
	
	// Only draw the icon if it will fit into the space between the borders
	if (rc.right < rcItem.right - m_nBorder) { 
		// draw the icon associated with the caption.
		::DrawIconEx( pDC->GetSafeHdc(), rc.left, rc.top, m_hIcon,
			rc.Width(), rc.Height(), NULL, (HBRUSH)NULL, DI_NORMAL); 
	}
}

BOOL CXTCaption::Create(CWnd* pParentWnd, LPCTSTR lpszWindowName, DWORD dwExStyle, DWORD dwStyle, const CRect& rect, UINT nID) 
{
	// Let the base class create the control.
	if( !CStatic::Create( NULL, dwStyle|WS_CLIPCHILDREN, rect, pParentWnd, nID ))
	{
		TRACE(_T("Unable to create caption.\n"));
		return FALSE;
	}

	SetFont(&xtAfxData.font);

	// save the style.
	m_dwExStyle = (dwExStyle & (CPWS_EX_GROOVE_EDGE | CPWS_EX_RAISED_EDGE | CPWS_EX_CLOSEBUTTON));
	ModifyStyleEx( 0, (dwExStyle & ~m_dwExStyle));

	// Save the window text.
	m_strCaption = lpszWindowName;

	if ((m_dwExStyle & CPWS_EX_CLOSEBUTTON) == CPWS_EX_CLOSEBUTTON)
	{
		// Create the image list used by frame buttons.
		m_ilButton.Create (XT_IDB_BTN_OUT, 16, 1, RGB(255,0,255));

		// Create the caption's close button.
		if (!m_btnCaption.Create(NULL, WS_VISIBLE|WS_CHILD|BS_ICON|BS_OWNERDRAW|BS_CENTER|BS_VCENTER,
			CRect(0,0,0,0), this, XT_IDC_BTN_CLOSE))
		{
			TRACE0( "Unable to create caption button.\n" );
			return -1;
		}

		// set the icon for the caption's close button
		m_btnCaption.SetIcon( CSize( 16, 15 ), m_ilButton.ExtractIcon( 2 ) );
		
		// set the style for the caption's close button
		if (xtAfxData.bXPMode) {
			m_btnCaption.SetXButtonStyle(
				(m_btnCaption.GetXButtonStyle() | BS_XT_XPFLAT) & ~BS_XT_FLAT);
		}
		else {
			m_btnCaption.SetXButtonStyle(
				(m_btnCaption.GetXButtonStyle() | BS_XT_FLAT) & ~BS_XT_XPFLAT);
		}

		SetWindowPos(NULL, 0,0,0,0,
			SWP_NOSIZE|SWP_NOMOVE|SWP_FRAMECHANGED);
	}
	else
	{
		// Create the button to be used with child window.
		if( !m_btnCaption.Create( m_strCaption, BS_ICON|BS_OWNERDRAW|BS_VCENTER,
			CRect(0,0,0,0), this, XT_IDC_BTN_CLOSE ))
		{
			TRACE0( "Unable to create caption button.\n" );
			return FALSE;
		}
	}

	// set the button font and colors.
	m_btnCaption.SetButtonStyle( m_clrFace, m_clrText );

	return TRUE;
}

void CXTCaption::ModifyCaptionStyle(int nBorderSize, CFont* pFont, LPCTSTR lpszWindText, HICON hIcon)
{
	ASSERT(::IsWindow(m_hWnd));

	if (nBorderSize != 0) {
		m_nBorder = nBorderSize;
	}

	if (pFont != NULL) {
		SetFont(pFont);
	}

	if (lpszWindText != NULL) {
		m_strCaption = lpszWindText;
	}

	if (hIcon != NULL) {
		m_hIcon	= hIcon;
	}

	Invalidate();
}

void CXTCaption::SetChildWindow(CWnd* pChild, CWnd* pNotifyWnd)
{
	ASSERT_VALID (pChild);		// must be valid.
	ASSERT_VALID (pNotifyWnd);	// must be valid.

	// save window pointers, and show caption button.
	m_pChildWnd  = pChild;
	SetOwner(pNotifyWnd);
	
	m_pParentView  = pChild->GetParent();
	ASSERT_KINDOF(CView, m_pParentView);

	m_pSplitterWnd = m_pParentView->GetParent();
	ASSERT_KINDOF(CSplitterWnd, m_pSplitterWnd);

	// get the size of the child and parent windows.
	m_pChildWnd   ->GetClientRect (&m_rcChild);
	m_pParentView ->GetClientRect (&m_rcParent);
	m_pSplitterWnd->GetClientRect (&m_rcSplitter);

	// save the size of the childs caption area.
	m_nOffset = (m_rcParent.Height() - m_rcChild.Height());

	// make the caption button visible.
	m_btnCaption.ShowWindow(SW_SHOW);
	m_btnCaption.UpdateWindow();

	// inflate the size of the parent to add a border, this will
	// also be the size of the popup window.
	int cx = ::GetSystemMetrics (SM_CXEDGE)*2;
	int cy = ::GetSystemMetrics (SM_CYEDGE)*2;
	m_rcParent.InflateRect(cx,cy);
}

void CXTCaption::KillChildWindow()
{
	m_btnCaption.SetState(FALSE);
	m_btnCaption.ModifyStyle( WS_VISIBLE, 0 );
	m_pPopupWnd->DestroyWindow();
	SAFE_DELETE( m_pPopupWnd );
}

void CXTCaption::OnCaptButton() 
{
	if ((m_dwExStyle & CPWS_EX_CLOSEBUTTON) == CPWS_EX_CLOSEBUTTON)
	{
		CWnd* pOwner = GetOwner();
		ASSERT_VALID(pOwner);

		if (pOwner)
		{
			// Notify of selection change.
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				BN_CLICKED), (LPARAM)m_hWnd );
		}
	}
	else
	{
		CRect rc (m_rcParent);
		m_pParentView->ClientToScreen (&rc);

		// get the current size of the splitter window.
		CRect rcSplitter;
		m_pSplitterWnd->GetClientRect(&rcSplitter);

		// check to see if the size has changed.
		if (rcSplitter.Height() > m_rcSplitter.Height()) {
			rc.bottom += (rcSplitter.Height() - m_rcSplitter.Height());
		}
		else if (m_rcSplitter.Height() > rcSplitter.Height()) {
			rc.bottom -= (m_rcSplitter.Height() - rcSplitter.Height());
		}

		// free any previously allocated memory and create a new popup window.
		SAFE_DELETE (m_pPopupWnd);
		m_pPopupWnd = new CXTCaptionPopupWnd;
		m_pPopupWnd->Create (rc, this, m_pChildWnd);

		// show the child window.
		m_pChildWnd->ShowWindow (SW_SHOW);
		m_pChildWnd->UpdateWindow ();

		// press the caption button.
		m_btnCaption.SetState (TRUE);
		Invalidate();
	}
}

void CXTCaption::OnPushPinButton()
{
	KillChildWindow();
	
	// get the current size of the child's parent window and
	// adjust the size of the child accordingly.
	m_pParentView->GetClientRect (&m_rcParent);
	m_rcChild      = m_rcParent;
	m_rcChild.top += m_nOffset;

	// move the child window to its correct location.
	m_pChildWnd->MoveWindow (m_rcChild);

	CWnd* pNotifyWnd = GetOwner();
	ASSERT_VALID(pNotifyWnd);

	if (pNotifyWnd != NULL && ::IsWindow(pNotifyWnd->m_hWnd))
	{
        // Notify that push pin button was pressed.
		pNotifyWnd->SendMessage(CPWN_XT_PUSHPINBUTTON);
		Invalidate();
	}
}

void CXTCaption::OnPushPinCancel()
{
	m_btnCaption.SetState(FALSE);
	m_pPopupWnd->DestroyWindow();

	SAFE_DELETE (m_pPopupWnd);

	CWnd* pNotifyWnd = GetOwner();
	ASSERT_VALID(pNotifyWnd);

	if (pNotifyWnd != NULL && ::IsWindow(pNotifyWnd->m_hWnd))
	{
		// Notify that popup window has lost input focus.
		pNotifyWnd->SendMessage(CPWN_XT_PUSHPINCANCEL);
		Invalidate();
	}
}


void CXTCaption::UpdateCaption(LPCTSTR lpszWindowText, HICON hIcon)
{
	ASSERT(::IsWindow(m_hWnd));
	
	CRect rcClient;
	GetClientRect(&rcClient);
	CDC* pDC = GetDC();
	
	DrawCaptionBack(pDC, rcClient);

	if (lpszWindowText != NULL)
	{
		m_strCaption = lpszWindowText;
		UpdateCaptionText(pDC, rcClient);
	}
	
	if (hIcon != NULL)
	{
		m_hIcon	= hIcon;
		DrawCaptionIcon( pDC, rcClient );
	}

	ReleaseDC(pDC);
}

void CXTCaption::UpdateCaptionText(CDC* pDC, CRect& rcItem)
{
	CFont* pOldFont = pDC->SelectObject(GetFont());
	
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	
	CSize size = pDC->GetTextExtent(m_strCaption, m_strCaption.GetLength());
	size.cx += tm.tmAveCharWidth+21;
	pDC->SelectObject(pOldFont);
	
	CRect rc( 0, 0, size.cx, rcItem.Height() );
	rc.DeflateRect(m_nBorder+1, m_nBorder+1);
	m_btnCaption.MoveWindow(rc);
	m_btnCaption.SetWindowText(m_strCaption);
	
	DrawCaptionText( pDC, rcItem );
}

void CXTCaption::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CStatic::OnWindowPosChanged(lpwndpos);
	
	if (::IsWindow(m_btnCaption.m_hWnd))
	{
		if ((m_dwExStyle & CPWS_EX_CLOSEBUTTON) == 0)
		{
			// Use a NULL window dc to get the size the caption button
			// should be using CDC::GetTextExtent(...).
			CWindowDC dc(NULL);
			CFont* pOldFont = dc.SelectObject(GetFont());

			TEXTMETRIC tm;
			dc.GetTextMetrics(&tm);

			CSize size = dc.GetTextExtent(m_strCaption, m_strCaption.GetLength());
			size.cx += tm.tmAveCharWidth+21;
			dc.SelectObject(pOldFont);
			
			CRect rc( 0, 0, size.cx, lpwndpos->cy );
			rc.DeflateRect(m_nBorder+1, m_nBorder+1);
			m_btnCaption.MoveWindow(rc);
		}
		else
		{
			m_btnCaption.MoveWindow( lpwndpos->cx-18, 2, 16, 15 );
		}
	}

	Invalidate();
	UpdateWindow();
}

void CXTCaption::SetCaptionColors(COLORREF clrBorder, COLORREF clrFace, COLORREF clrText)
{
	ASSERT(::IsWindow(m_hWnd));
	
	m_bUserColors = true;

	m_clrBorder = clrBorder;
	m_clrFace   = clrFace;
	m_clrText   = clrText;

	if (m_btnCaption.GetSafeHwnd())
	{
		m_btnCaption.SetColorFace(m_clrFace);
		m_btnCaption.SetColorText(m_clrText);
		m_btnCaption.SetColorShadow(::GetSysColor(COLOR_3DDKSHADOW));
	}
}

void CXTCaption::OnSysColorChange() 
{
	CStatic::OnSysColorChange();
	
	// update caption colors if not user defined.
	if (m_bUserColors == false)
	{
		m_clrText   = ::GetSysColor(COLOR_BTNTEXT);
		m_clrBorder = ::GetSysColor(COLOR_3DFACE);
		m_clrFace   = ::GetSysColor(COLOR_3DFACE);
		
		if (m_btnCaption.GetSafeHwnd())
		{
			m_btnCaption.SetColorFace(m_clrFace);
			m_btnCaption.SetColorText(m_clrText);
			m_btnCaption.SetColorShadow(::GetSysColor(COLOR_3DDKSHADOW));
		}
	}
}
