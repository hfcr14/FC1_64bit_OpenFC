// CXTSearchOptionsCtrl : implementation file
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
#include "XTMemDC.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeFormView.h"
#include "XTSearchOptionsCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTSearchOptionsCtrl

CXTSearchOptionsCtrl::CXTSearchOptionsCtrl()
{
	m_strExpandLabel	= _T("");
	m_strContractLabel	= _T("");
	m_iMinSize          = ::GetSystemMetrics(SM_CYMENU);
	m_iMaxSize			= 0;
	m_bExpanded			= true;
	m_dwInitSignature   = 1;
}

CXTSearchOptionsCtrl::~CXTSearchOptionsCtrl()
{
}

IMPLEMENT_DYNAMIC(CXTSearchOptionsCtrl, CStatic)

BEGIN_MESSAGE_MAP(CXTSearchOptionsCtrl, CStatic)
	//{{AFX_MSG_MAP(CXTSearchOptionsCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTSearchOptionsCtrl message handlers

void CXTSearchOptionsCtrl::AddControl(CWnd* pWndCtrl)
{
	ASSERT_VALID(pWndCtrl); // must be valid.
	m_arHideCtrls.Add(pWndCtrl);
}

void CXTSearchOptionsCtrl::MoveControl(CWnd* pWndCtrl)
{
	ASSERT_VALID(pWndCtrl); // must be valid.
	m_arMoveCtrls.Add(pWndCtrl);
}

void CXTSearchOptionsCtrl::SetLabelText(LPCTSTR lpszExpand, LPCTSTR lpszContract)
{
	m_strExpandLabel   = lpszExpand;
	m_strContractLabel = lpszContract;
}

void CXTSearchOptionsCtrl::Expand()
{
	// set our expanded flag to true.
	m_bExpanded = true;
	
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	// notify owner of expanding, this is sent prior to our
	// adjusting any controls to allow the parent to prepare
	// itself for resizing.

	if (pOwner != NULL)
	{
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XT_ITEMEXPANDING), (LPARAM)m_hWnd );
	}

	// expand this window.
	CRect rect;
	GetWindowRect(&rect);
	GetParent()->ScreenToClient(&rect);
	rect.bottom = rect.top + m_iMaxSize;
	MoveWindow(&rect);

	// show each of our windows in the hide controls array.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		pWndCtrl->ShowWindow(SW_SHOW);
	}

	// move each of the controls in our move controls array.
	for (iIndex = 0; iIndex < m_arMoveCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arMoveCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		
		CWnd* pParentWnd = pWndCtrl->GetParent();
		ASSERT_VALID(pParentWnd);

		CRect rc;
		pWndCtrl->GetWindowRect(&rc);
		pParentWnd->ScreenToClient(&rc);

		int iOffset = m_iMaxSize-m_iMinSize;
		rc.top += iOffset;
		rc.bottom += iOffset;
		
		// move the control and repaint it.
		pWndCtrl->MoveWindow(&rc);
		pWndCtrl->InvalidateRect(NULL);
	}

	// notify owner that expand has completed.

	if (pOwner != NULL)
	{
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XT_ITEMEXPAND), (LPARAM)m_hWnd );
	}
}

void CXTSearchOptionsCtrl::Contract()
{
	// set our expanded flag to false.
	m_bExpanded = false;
	
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	// notify owner of contracting, this is sent prior to our
	// adjusting any controls to allow the parent to prepare
	// itself for resizing.

	if (pOwner != NULL)
	{
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XT_ITEMCONTRACTING), (LPARAM)m_hWnd );
	}

	// contract this window.
	CRect rect;
	GetWindowRect(&rect);

	if (m_iMaxSize == 0)
		m_iMaxSize = rect.Height();

	GetParent()->ScreenToClient(&rect);
	rect.bottom = rect.top + m_iMinSize;
	MoveWindow(&rect);

	// hide each of our windows in the hide controls array.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		pWndCtrl->ShowWindow(SW_HIDE);
	}

	// move each of the controls in our move controls array.
	for (iIndex = 0; iIndex < m_arMoveCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arMoveCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		
		CWnd* pParentWnd = pWndCtrl->GetParent();
		ASSERT_VALID(pParentWnd);
		
		CRect rc;
		pWndCtrl->GetWindowRect(&rc);
		pParentWnd->ScreenToClient(&rc);
		
		int iOffset = m_iMaxSize-m_iMinSize;
		rc.top -= iOffset;
		rc.bottom -= iOffset;
		
		// move the control and repaint it.
		pWndCtrl->MoveWindow(&rc);
		pWndCtrl->InvalidateRect(NULL);
	}

	// notify owner that contracting has completed.

	if (pOwner != NULL)
	{
		pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			SON_XT_ITEMCONTRACT), (LPARAM)m_hWnd );
	}
}

BOOL CXTSearchOptionsCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTSearchOptionsCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);

	// exclude the windows in our "hide list" from paint routines.
	int iIndex;
	for (iIndex = 0; iIndex < m_arHideCtrls.GetSize(); ++iIndex)
	{
		CWnd* pWndCtrl = (CWnd*)m_arHideCtrls.GetAt(iIndex);
		ASSERT_VALID(pWndCtrl); // must be valid.
		
		CRect rcHide;
		pWndCtrl->GetWindowRect(&rcHide);
		ScreenToClient(&rcHide);

		dc.ExcludeClipRect(&rcHide);
	}

	// paint off screen.
	CXTMemDC memDC(&dc, rcClient, xtAfxData.clrWindow);
	
	// save the current memDC.
	int iSavedDC = memDC.SaveDC();
	
	// construct the font's attributes
	if (m_bExpanded)
	{
		memDC.SelectObject(&xtAfxData.font);
		memDC.SetBkColor(xtAfxData.clrHighlight);
		memDC.SetTextColor(xtAfxData.clrHighlightText);
		
		// if we are expanded, draw a border and fill caption.
		memDC.Draw3dRect(rcClient, xtAfxData.clrHighlight,
			xtAfxData.clrHighlight);
		
		CRect rcCaption(rcClient);
		rcCaption.bottom = rcCaption.top + m_iMinSize;
		rcCaption.top += 1;
		rcCaption.bottom -= 1;
		memDC.FillSolidRect(rcCaption, xtAfxData.clrHighlight);
	}
	else
	{
		memDC.SelectObject(&xtAfxData.fontULine);
		memDC.SetBkColor(xtAfxData.clrWindow);
		memDC.SetTextColor(xtAfxData.clrHighlight);
	}
	
	// get the size of the label's text.
	CSize size = memDC.GetTextExtent(m_bExpanded
		? m_strExpandLabel : m_strContractLabel);
	
	// construct the "hot" area used for mouse activation.
	GetWindowRect(&m_rcLabel);
	m_rcLabel.bottom = m_rcLabel.top + m_iMinSize;
	
	if (m_bExpanded)
		m_rcLabel.left += 3;
	else
		m_rcLabel.right = m_rcLabel.left + size.cx;
	
	// paint the label.
	ScreenToClient(&m_rcLabel);
	memDC.DrawText(m_bExpanded ? m_strExpandLabel : m_strContractLabel,
		&m_rcLabel, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	
	// restore the memDC.
	memDC.RestoreDC(iSavedDC);
}

void CXTSearchOptionsCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_rcLabel.PtInRect(point))
	{
		if (m_bExpanded) {
			Contract();
		}
		else {
			Expand();
		}
		
		Invalidate();
	}
	
	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CXTSearchOptionsCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// get the cursor position in client coordinates.
	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	// if the cursor is over the label set the hand cursor.
	if( m_rcLabel.PtInRect(point) )
	{
		::SetCursor( xtAfxData.hcurHand );
		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CXTSearchOptionsCtrl::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	
	if ((DWORD)lParam == m_dwInitSignature)
	{
		CRect rect;
		GetWindowRect(&rect);

		if (m_iMaxSize == 0)
			m_iMaxSize = rect.Height();

		// update display.
		Invalidate();
		UpdateWindow();
	}
	
	return 0;
}

void CXTSearchOptionsCtrl::PreSubclassWindow() 
{
	CStatic::PreSubclassWindow();
	
	// Initialize the control.
	DeferInitialUpdate();
}

void CXTSearchOptionsCtrl::DeferInitialUpdate()
{
	if (m_dwInitSignature > 0)
	{
		PostMessage(XTWM_INITIAL_UPDATE, 0, ++m_dwInitSignature);
	}
}

int CXTSearchOptionsCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	DeferInitialUpdate();
	
	return 0;
}

void CXTSearchOptionsCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CStatic::OnWindowPosChanged(lpwndpos);
	
	// Update the control whenever the window size or position has changed.
	Invalidate();
	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CXTSearchOptionsView

IMPLEMENT_DYNAMIC(CXTSearchOptionsView, CXTResizeFormView)

CXTSearchOptionsView::CXTSearchOptionsView(const UINT nID, const UINT nFlags) : 
	CXTResizeFormView(nID, nFlags)
{
	m_hBrush = NULL;
}

CXTSearchOptionsView::~CXTSearchOptionsView()
{
	if (m_hBrush != NULL)
		::DeleteObject(m_hBrush);
}

BEGIN_MESSAGE_MAP(CXTSearchOptionsView, CXTResizeFormView)
	//{{AFX_MSG_MAP(CXTSearchOptionsView)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTSearchOptionsView message handlers

HBRUSH CXTSearchOptionsView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nCtlColor);
	
	if (m_hBrush != NULL)
		::DeleteObject(m_hBrush);

	m_hBrush = ::CreateSolidBrush(xtAfxData.clrWindow);

	pDC->SetBkColor(xtAfxData.clrWindow);
	pDC->SetTextColor(xtAfxData.clrWindowText);

	return m_hBrush;
}
