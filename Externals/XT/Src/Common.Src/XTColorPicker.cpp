// XTColorPicker.cpp : implementation of the CXTColorPicker class.
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
#include "XTButton.h"
#include "XTColorRef.h"
#include "XTColorSelectorCtrl.h"
#include "XTColorPopup.h"
#include "XTColorPicker.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_XT_SHOWPOPUP  (WM_APP + 01)

/////////////////////////////////////////////////////////////////////////////
// DDX Routines

_XT_EXT_CLASS void AFXAPI DDX_XTColorPicker(CDataExchange *pDX, int nIDC, COLORREF& value)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT (hWndCtrl != NULL);                

    CXTColorPicker* pColorPicker = (CXTColorPicker*) CWnd::FromHandle(hWndCtrl);
    if (pDX->m_bSaveAndValidate) {
        value = pColorPicker->GetColor();
    }
	else {
        pColorPicker->SetColor(value);
    }
}

/////////////////////////////////////////////////////////////////////////////
// CXTColorPicker

CXTColorPicker::CXTColorPicker()
: m_dwPopup(CPS_XT_EXTENDED|CPS_XT_MORECOLORS)
, m_clrSelected(CLR_DEFAULT)
, m_clrDefault(CLR_DEFAULT)
, m_bShowText(FALSE)
, m_bColorText(FALSE)
, m_bPopupActive(FALSE)
, m_bInitControl(true)
{

}

CXTColorPicker::~CXTColorPicker()
{

}

IMPLEMENT_DYNAMIC(CXTColorPicker, CButton)

BEGIN_MESSAGE_MAP(CXTColorPicker, CButton)
	//{{AFX_MSG_MAP(CXTColorPicker)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_MESSAGE(CPN_XT_SELCHANGE, OnSelChange)
    ON_MESSAGE(CPN_XT_DROPDOWN, OnDropDown)
    ON_MESSAGE(CPN_XT_CLOSEUP, OnCloseUp)
    ON_MESSAGE(CPN_XT_SELENDOK, OnSelEndOK)
    ON_MESSAGE(CPN_XT_SELENDCANCEL, OnSelEndCancel)
    ON_MESSAGE(CPN_XT_SELNOFILL, OnSelNoFill)
    ON_MESSAGE_VOID(WM_XT_SHOWPOPUP, OnShowPopup)
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorPicker message handlers

LRESULT CXTColorPicker::OnSelChange(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		// Notify of selection change.
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XT_SELCHANGE), (LPARAM)m_hWnd );
	}

	return 0;
}

LRESULT CXTColorPicker::OnDropDown(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup activation.
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XT_DROPDOWN), (LPARAM)m_hWnd );
	}

	return 0;
}

LRESULT CXTColorPicker::OnCloseUp(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);
	m_bPopupActive = FALSE;

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup close up.
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XT_CLOSEUP), (LPARAM)m_hWnd );
	}

	Invalidate();
	return 0;
}

LRESULT CXTColorPicker::OnSelEndOK(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		SetColor((COLORREF)wParam);

		// Notify of popup close up.
		// NB: doing so may destroy the picker so lets copy off 
		// the window handle/control ID for safety
		HWND hWndSender = m_hWnd;
		int nControlID = GetDlgCtrlID();
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(nControlID,
			CPN_XT_CLOSEUP), (LPARAM)hWndSender);

		// Notify of selection made.
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(nControlID,
			CPN_XT_SELENDOK), (LPARAM)hWndSender);
	}
	
	return 0;
}

LRESULT CXTColorPicker::OnSelEndCancel(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	UNUSED_ALWAYS(lParam);

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		// Notify of popup close up.
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XT_CLOSEUP), (LPARAM)m_hWnd );

		// Notify of cancel.
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XT_SELENDCANCEL), (LPARAM)m_hWnd );
	}
	
	return 0;
}

LRESULT CXTColorPicker::OnSelNoFill(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	if (pOwner != NULL)
	{
		SetColor((COLORREF)wParam);
		
		// Notify of no fill press.
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			CPN_XT_SELNOFILL), (LPARAM)m_hWnd );
	}
	
	return 0;
}

LRESULT CXTColorPicker::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	
	if (m_bInitControl)
	{
		m_bInitControl = false;
		
		// Set the font, and change the style to BS_OWNERDRAW.
		ModifyStyle(0, BS_OWNERDRAW);
		SetFont(&xtAfxData.font);
	}
	
	return 0;
}

void CXTColorPicker::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	
	// Initialize the control.
	PostMessage(XTWM_INITIAL_UPDATE);
}

int CXTColorPicker::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	SendMessage(XTWM_INITIAL_UPDATE);
	
	return 0;
}

void CXTColorPicker::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct != NULL);

	// define some temporary variables.
	CDC*	pDC			= CDC::FromHandle( lpDrawItemStruct->hDC );
	CRect	rcItem		= lpDrawItemStruct->rcItem;
    int 	nState		= lpDrawItemStruct->itemState;
	int     nSavedDC    = pDC->SaveDC();
	DWORD	dwState		= DFCS_BUTTONPUSH|DFCS_ADJUSTRECT;

	// set display flags based on state.
	if (m_bPopupActive)
		nState |= ODS_SELECTED|ODS_FOCUS;

	if (nState & ODS_SELECTED)
		dwState |= DFCS_PUSHED;

	if (nState & ODS_DISABLED)
		dwState |= DFCS_INACTIVE;

	// draw the button borders
	pDC->DrawFrameControl(&rcItem,  DFC_BUTTON, dwState);

	// draw the button arrow.
	CRect rcArrow(rcItem);
	rcArrow.left = rcArrow.Width()-xtAfxData.cxHThumb;

	if (nState & ODS_SELECTED) {
		rcArrow.OffsetRect(1,1);
	}

	dwState = DFCS_SCROLLDOWN;

	if (nState & ODS_DISABLED) {
		dwState |= DFCS_INACTIVE;
	}

    pDC->DrawFrameControl(&rcArrow, DFC_SCROLL, dwState);

	// Cover up thick border from DrawFrameControl, we only want the arrow.
	pDC->Draw3dRect( rcArrow, xtAfxData.clr3DFace, xtAfxData.clr3DFace);
	rcArrow.DeflateRect(1,1);
	pDC->Draw3dRect( rcArrow, xtAfxData.clr3DFace, xtAfxData.clr3DFace);

	// draw the focus rect.
	if (nState & ODS_FOCUS) {
		pDC->DrawFocusRect( rcItem );
	}

	// draw the separator line.
	CRect rect(rcArrow.left-1, rcArrow.top+2, rcArrow.left+1, rcArrow.bottom-2);
	pDC->Draw3dRect( rect, xtAfxData.clr3DShadow, xtAfxData.clr3DHilight );

	// draw the color box.
	if((nState & ODS_SELECTED) || (GetCheck() == 1))
		rcItem.OffsetRect( 1,1 );
	
	rcItem.DeflateRect( 2, 2 );
	rcItem.right = rcArrow.left-4;

	COLORREF clrRect = ( nState & ODS_DISABLED ) ? xtAfxData.clr3DFace : GetColor( );

	if ( IsTextShown( ) && IsTextInColor( ) )
	{
		clrRect = GetContrastColor( );
	}

	// Construct it, then initialize
    CBrush newBrush;
	if( newBrush.CreateSolidBrush( clrRect ) )
	{
		// Select it into the device context
        // Save the old brush at the same time
		pDC->SelectObject( &newBrush );

		// Set the border color for the display rect.
		CPen newPen;
		if( newPen.CreatePen( PS_SOLID, 1, (nState & ODS_DISABLED)?
			xtAfxData.clr3DShadow:RGB(0,0,0)) )
		{
			// draw the color rect.
			pDC->SelectObject( &newPen );
			pDC->Rectangle( rcItem );
		}
	}
	
    // Draw the window text (if any)
	if ( IsTextShown( ) )
	{
		CString strText;
		GetWindowText( strText );
		if ( strText.GetLength( ) )
		{
			if ( nState & ODS_DISABLED )
			{
				pDC->SetTextColor( xtAfxData.clr3DShadow );
			}
			else
			{
				if ( IsTextInColor( ) )
				{
					pDC->SetTextColor( GetColor( ) );
				}
				else
				{
					pDC->SetTextColor( GetContrastColor( ) );
				}
			}
			
			// Draw the text
			pDC->SetBkMode( TRANSPARENT );
			pDC->SelectObject( GetFont( ) );
			pDC->DrawText( strText, rcItem, DT_CENTER|DT_SINGLELINE|DT_VCENTER );
		}
	}

	// restore the original DC.
	pDC->RestoreDC(nSavedDC);
}

void CXTColorPicker::SetColor(COLORREF clr)
{
	if ( clr != GetColor( ) )
	{
		m_clrSelected = clr;
		RedrawWindow();
	}
}

COLORREF CXTColorPicker::GetContrastColor() const
{
	double Intensity = CXTColorRef( GetColor( ) ).getIntensity( );

	if ( Intensity > 128 ) {
		return CXTColorRef::BLACK;
	}
	else {
		return CXTColorRef::WHITE;
	}
}

void CXTColorPicker::OnPaint() 
{
	CPaintDC dc(this);
	CRect rectClient;
	GetClientRect(&rectClient);
	CXTMemDC memDC(&dc, rectClient);
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

BOOL CXTColorPicker::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CXTColorPicker::ShowPopupWindow()
{
	m_bPopupActive = TRUE;
	
	// Make sure that we have input focus.
	if (GetFocus() != this) {
		SetFocus();
	}

	// Post a message instead of displaying right away - this will take care
	// of asynchronous focusing issues
	PostMessage(WM_XT_SHOWPOPUP);
}

void CXTColorPicker::OnShowPopup()
{
	// Get the size of the picker button.
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	// Create the color popup window.
	CXTColorPopup *pColorPopup = new CXTColorPopup(TRUE);
	pColorPopup->Create(rcWindow, this, m_dwPopup, GetColor(), GetDefaultColor());
	pColorPopup->SetFocus();
}

BOOL CXTColorPicker::PreTranslateMessage(MSG* pMsg) 
{
	if (!m_bPopupActive)
	{
		if ( pMsg->message == WM_LBUTTONDOWN)
		{
			ShowPopupWindow();
			// in this case the message is not "swallowed" so the button will
			// get it and display itself in a recessed state
		}
		else if (pMsg->message == WM_KEYDOWN &&
			 (pMsg->wParam == VK_RETURN  || 
			  pMsg->wParam == VK_SPACE   ||
			  pMsg->wParam == VK_DOWN) )
		{
			ShowPopupWindow();
			return TRUE;  // swallow message
		}
	}
	
	return CButton::PreTranslateMessage(pMsg);
}
