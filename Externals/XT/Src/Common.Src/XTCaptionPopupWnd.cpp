// XTCaptionPopupWnd.cpp : implementation of the CXTCaptionPopupWnd class.
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
#include "XTPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTCaptionPopupWnd
/////////////////////////////////////////////////////////////////////////////
CXTCaptionPopupWnd::CXTCaptionPopupWnd()
{
	m_pChildWnd		= NULL;
	m_pChildParent	= NULL;
	m_pParentWnd	= NULL;
}

CXTCaptionPopupWnd::~CXTCaptionPopupWnd()
{
	DestroyWindow();
}

IMPLEMENT_DYNAMIC(CXTCaptionPopupWnd, CWnd)

BEGIN_MESSAGE_MAP(CXTCaptionPopupWnd, CWnd)
	//{{AFX_MSG_MAP(CXTCaptionPopupWnd)
	ON_WM_DESTROY()
	ON_BN_CLICKED(XT_IDC_BTN_CLOSE, OnCaptButton)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTCaptionPopupWnd::Create(const CRect& rect, CWnd* pParentWnd, CWnd* pChildWnd) 
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	ASSERT(pParentWnd != NULL); // must be valid.
	m_pParentWnd = pParentWnd;

	ASSERT(pChildWnd  != NULL); // must be valid.
	m_pChildWnd = pChildWnd;

	// register the wnd class.
	CString wndClass = ::AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,
		0, ::GetSysColorBrush(COLOR_BTNFACE));

	// define the styles for this window.
	DWORD dwStyleEx = WS_EX_TOOLWINDOW;
	DWORD dwStyle   = WS_VISIBLE|WS_POPUP|WS_DLGFRAME;

	// call the base class for creation.
    if( !CWnd::CreateEx( dwStyleEx, wndClass, _T(""), dwStyle, rect.left, rect.top,
		rect.Width(), rect.Height(), pParentWnd->GetSafeHwnd(), 0, NULL ))
	{
		TRACE0( "Failed to create caption popdown window.\n" );
		return FALSE;
	}

	// set the child windows parent to this popup window, save
	// the previous parent, since we need to reset this when the
	// window is destroyed.
	m_pChildParent = m_pChildWnd->SetParent(this);
	ASSERT(m_pChildParent != NULL);

	// define the caption and client rect.
	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(2,2);
	CRect rcCapt( rcClient );

	// move the window to fit the client area of this window.
	rcClient.top += 19;
	m_pChildWnd->MoveWindow(rcClient);

	// define the size for the push pin caption.
	rcCapt.bottom = rcCapt.top + 19;

	// Create the caption.
	CString string;
	string.LoadString(XT_IDS_CLICKKEEPOPEN);

	if( !m_Caption.Create( this, string, CPWS_EX_RAISED_EDGE,
		WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE, rcCapt ))
	{
		TRACE0( "Unable to create caption.\n" );
		return FALSE;
	}

	// define the size for the caption button.
	CRect rcButton( rcCapt );
	rcButton.DeflateRect(2,2);
	rcButton.left = rcButton.right - 16;

	// Create the caption button.
	if( !m_CaptionButton.Create( NULL, WS_VISIBLE|BS_ICON|BS_OWNERDRAW|BS_CENTER|BS_VCENTER,
		rcButton, this, XT_IDC_BTN_CLOSE ))
	{
		TRACE0( "Unable to create caption button.\n" );
		return FALSE;
	}

	CImageList imageList;
	imageList.Create(XT_IDB_BTN_OUT, 16, 1, RGB(255,0,255));
	
	// Associate the button icon
	m_hIconOff = imageList.ExtractIcon(0);
	m_hIconOn  = imageList.ExtractIcon(1);
	m_CaptionButton.SetIcon(CSize(16,15), m_hIconOff, m_hIconOn);
	imageList.DeleteImageList();

    if (xtAfxData.bXPMode)
    {
        m_CaptionButton.SetXButtonStyle(
            (m_CaptionButton.GetXButtonStyle() | BS_XT_XPFLAT) & ~BS_XT_FLAT);
    }

	// start the timer after everything has been initialized.
	SetTimer(1, 10, NULL);

	return TRUE;
}

void CXTCaptionPopupWnd::OnDestroy() 
{
	if ( ( m_pChildWnd    && ::IsWindow( m_pChildWnd->m_hWnd ) ) &&
	     ( m_pChildParent && ::IsWindow( m_pChildParent->m_hWnd ) ) )
	{
		// set the child window's parent back it's original parent.
		m_pChildWnd->SetParent( m_pChildParent );
	}

	CWnd::OnDestroy();
}

void CXTCaptionPopupWnd::OnCaptButton()
{
	// Notify that push pin button was pressed.
	m_pParentWnd->SendMessage(CPWN_XT_PUSHPINBUTTON);
}

void CXTCaptionPopupWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	rc.DeflateRect(1,1);

	// draw a border for the client area.
	dc.Draw3dRect( rc, xtAfxData.clr3DShadow,
		xtAfxData.clr3DHilight);

	// update the caption as well.
	m_Caption.Invalidate();
}

void CXTCaptionPopupWnd::OnTimer(UINT_PTR nIDEvent)
{
	UNUSED_ALWAYS( nIDEvent );

	// get a handle to the window which currently
	// has the input focus...
	HWND hWnd_HasFocus = ::GetFocus( );

	// get a handle to the caption, caption button, child window
	// and popup window...
	HWND hWnd_Caption		= m_Caption.GetSafeHwnd();
	HWND hWnd_CaptionButton = m_CaptionButton.GetSafeHwnd();
	HWND hWnd_Popup			= GetSafeHwnd();
	HWND hWnd_PopupChild	= m_pChildWnd->GetSafeHwnd();

	// check to see if the window that has input focus is part
	// of the caption popup window, if not, send a message
	// to the parent control.
	if(( hWnd_HasFocus != hWnd_Caption ) &&
	   ( hWnd_HasFocus != hWnd_CaptionButton ) &&
	   ( hWnd_HasFocus != hWnd_Popup ) &&
	   ( hWnd_HasFocus != hWnd_PopupChild ))
	{
		if ( m_pParentWnd && ::IsWindow( m_pParentWnd->m_hWnd ) )
		{
			// Notify that popup window has lost input focus.
			m_pParentWnd->SendMessage( CPWN_XT_PUSHPINCANCEL );
		}
		else
		{
			SendMessage( WM_CLOSE );
		}
	}
}

