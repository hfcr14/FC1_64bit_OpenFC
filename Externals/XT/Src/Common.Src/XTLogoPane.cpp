// XTLogoPane.cpp : implementation of the CXTLogoPane class.
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
#include "XTLogoPane.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTBasePane
/////////////////////////////////////////////////////////////////////////////
CXTBasePane::CXTBasePane()
{
}

CXTBasePane::~CXTBasePane()
{
}

IMPLEMENT_DYNCREATE(CXTBasePane, CWnd)

BEGIN_MESSAGE_MAP(CXTBasePane, CWnd)
	//{{AFX_MSG_MAP(CXTBasePane)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTBasePane::OnPaint() 
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rcClient;
	GetClientRect(&rcClient);

	CXTMemDC memDC(&dc, rcClient);

	// Draw the logo text
	DoPaint(&memDC, rcClient);
}

BOOL CXTBasePane::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

BOOL CXTBasePane::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd, DWORD dwStyle/*=WS_CHILD|WS_VISIBLE*/, UINT nID/*=0xffff*/) 
{
	CString wndClass = ::AfxRegisterWndClass(CS_BYTEALIGNCLIENT);

	if(!CWnd::Create(wndClass, lpszWindowName, dwStyle,
		CRect(0,0,0,0), pParentWnd, nID)) {
		return FALSE;
	}

	m_strWindowText = lpszWindowName;

	return TRUE;
}

void CXTBasePane::DoPaint(CDC* pDC, CRect& rcClient)
{
	UNREFERENCED_PARAMETER(pDC);
	UNREFERENCED_PARAMETER(rcClient);

	// TODO: handle paint operations in derived class
}

/////////////////////////////////////////////////////////////////////////////
// CXTLogoPane
/////////////////////////////////////////////////////////////////////////////
CXTLogoPane::CXTLogoPane()
{
	XT_LOGFONT lf;
	xtAfxData.fontBold.GetLogFont(&lf);
	SetLogoFont(lf);
}

CXTLogoPane::~CXTLogoPane()
{
	m_Font.DeleteObject();
}

BEGIN_MESSAGE_MAP(CXTLogoPane, CXTBasePane)
	//{{AFX_MSG_MAP(CXTLogoPane)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTLogoPane::SetLogoFont(LPCTSTR lpszFontName, int nHeight/*=24*/,
	int nWeight/*=FW_BOLD*/, BOOL bItalic/*=TRUE*/, BOOL bUnderline/*=FALSE*/)
{
	if(m_Font.m_hObject) {
		m_Font.DeleteObject();
	}

	m_Font.CreateFont(nHeight, 0, 0, 0, nWeight, (BYTE)bItalic, 
		(BYTE)bUnderline, 0,0,0,0,0,0, lpszFontName);
	SetTextSize();
}

void CXTLogoPane::SetLogoFont(LOGFONT& logFont)
{
	if(m_Font.m_hObject) {
		m_Font.DeleteObject();
	}

	m_Font.CreateFontIndirect(&logFont);
	SetTextSize();
}

void CXTLogoPane::SetTextSize()
{
	CWindowDC dc(NULL);
	CFont* pOldFont = dc.SelectObject(&m_Font);
	m_sizeText = dc.GetTextExtent(m_strWindowText);
	dc.SelectObject(pOldFont);
}

void CXTLogoPane::DoPaint(CDC* pDC, CRect& rcClient)
{
	// Save DC state.
	int iSavedDC = pDC->SaveDC();

	// Determine the size of the logo text.
	CSize sizeText = pDC->GetTextExtent(m_strWindowText);

	// Calculate the top "y" location.
	int y = (rcClient.Height()-sizeText.cy)/2;

	// Resize the rect to reflect the actual text size.
	rcClient.top = y;
	rcClient.bottom = rcClient.top + sizeText.cy;

	// Set the text color.
	pDC->SetTextColor(xtAfxData.clr3DShadow);

	// Draw text
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_Font);
	pDC->DrawText(m_strWindowText, &rcClient,
		DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS | DT_SINGLELINE);

	// Restore DC.
	pDC->RestoreDC(iSavedDC);
}

/////////////////////////////////////////////////////////////////////////////
// CXTIconPane
/////////////////////////////////////////////////////////////////////////////

CXTIconPane::CXTIconPane()
{
	m_hIcon = NULL;
	m_sizeIcon.cx = xtAfxData.cxSmIcon;
	m_sizeIcon.cy = xtAfxData.cySmIcon;
}

CXTIconPane::~CXTIconPane()
{
}

BEGIN_MESSAGE_MAP(CXTIconPane, CXTBasePane)
	//{{AFX_MSG_MAP(CXTIconPane)
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTIconPane::DoPaint(CDC* pDC, CRect& rcClient)
{
	if (m_hIcon)
	{
		// determine the x-y location of the icon
        CPoint point;
		point.x = (rcClient.Width() -m_sizeIcon.cx)/2;
		point.y = (rcClient.Height()-m_sizeIcon.cy)/2;

		// draw the icon associated with this pane.
		if (IsWindowEnabled())
		{
			pDC->DrawState(point, m_sizeIcon, m_hIcon,
				DST_ICON | DSS_NORMAL, (HBRUSH)NULL);
		}
		else
		{
			if (xtAfxData.bXPMode)
			{
				CBrush brush( xtAfxData.clr3DShadow );
				pDC->DrawState(point, m_sizeIcon, m_hIcon,
					DST_ICON | DSS_MONO, (HBRUSH)brush);
			}
			else
			{
				pDC->DrawState(point, m_sizeIcon, m_hIcon,
					DST_ICON | DSS_DISABLED, (HBRUSH)NULL);
			}
		}
	}
}

void CXTIconPane::SetPaneIcon(LPCTSTR lpszIconID)
{
	// Load the image for the icon pane.
	m_hIcon = ( HICON )::LoadImage( AfxGetInstanceHandle( ), lpszIconID, IMAGE_ICON,
		xtAfxData.cxSmIcon, xtAfxData.cySmIcon, LR_DEFAULTCOLOR );
}

void CXTIconPane::SetPaneIcon(int nIconID)
{
	SetPaneIcon(MAKEINTRESOURCE(nIconID));
}

void CXTIconPane::OnEnable(BOOL bEnable) 
{
	CXTBasePane::OnEnable(bEnable);
	
	// Refresh the icon to indicate state.
	Invalidate();
	UpdateWindow();
}
