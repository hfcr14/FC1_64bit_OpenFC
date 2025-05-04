// XTColorDialog.cpp : implementation file
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
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTFunctions.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTColorPageCustom.h"
#include "XTColorPageStandard.h"
#include "XTColorDialog.h"
#include "XTMemDC.h"
#include "XTPriv.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTColorDialog

IMPLEMENT_DYNAMIC(CXTColorDialog, CPropertySheet)

CXTColorDialog::CXTColorDialog(COLORREF clrNew, COLORREF clrCurrent, 
    DWORD dwFlags/*=0L*/, CWnd* pWndParent/*=NULL*/)
	 : CPropertySheet(XT_IDS_COLOR_CAPTION, pWndParent)
{
	m_psh.dwFlags = (m_psh.dwFlags & ~PSH_HASHELP)|PSH_NOAPPLYNOW;

	m_clrNew     = clrNew;
	m_clrCurrent = clrCurrent;
	m_dwStyle    = dwFlags;

	AddPage(new CXTColorPageStandard(this));
	AddPage(new CXTColorPageCustom(this));

    RevertResourceContext();
}

CXTColorDialog::~CXTColorDialog()
{
	int iPage;
	for (iPage = 0; iPage < GetPageCount(); ++iPage)
	{
		CPropertyPage* pPage = GetPage(iPage);
		SAFE_DELETE (pPage);
	}
}


BEGIN_MESSAGE_MAP(CXTColorDialog, CPropertySheet)
	//{{AFX_MSG_MAP(CXTColorDialog)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTColorDialog message handlers

BOOL CXTColorDialog::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO: Add your specialized code here
	SetWindowPos(NULL, 0, 0, 329, 341,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	GetTabControl()->SetWindowPos(NULL, 0, 0, 226, 280,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	CWnd* pWnd1 = GetDlgItem(IDOK);
	if (pWnd1 != NULL){
		pWnd1->SetWindowPos(NULL, 243, 29, 75, 22,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}
	
	CWnd* pWnd2 = GetDlgItem(IDCANCEL);
	if (pWnd2 != NULL){
		pWnd2->SetWindowPos(NULL, 243, 58, 75, 22,
			SWP_NOZORDER | SWP_NOACTIVATE);
	}

	SetActivePage(0);

	return bResult;
}

void CXTColorDialog::OnPaint() 
{
    XT_MANAGE_STATE;

	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(xtAfxData.clrWindowText);
	CFont* pOldFont = dc.SelectObject(&xtAfxData.font);

    // We have to put the Hex title above the hex box
	
	CRect rcText = rc;
	CString strText;

	if (m_dwStyle & CPS_XT_SHOWHEXVALUE)
	{
	
	    rcText.right  = 317; 
	    rcText.left   = 243;
	    rcText.bottom = 180 - 18;
	    rcText.top    = 162 - 18;
        
	    strText.LoadString(XT_IDS_HEX);
	    dc.DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }
    
    // We are defining the box in the right bottom corner where the new/current color
    // rectangles  will reside. 

	rc.right  -= 9;
	rc.left    = rc.right - 68;
	rc.bottom -= 32;
	rc.top     = rc.bottom - 66;

	if (m_dwStyle & CPS_XT_SHOW3DSELECTION)
	{
		rc.InflateRect(3,3);
		dc.Draw3dRect(&rc, xtAfxData.clr3DHilight, xtAfxData.clr3DDkShadow);
		rc.DeflateRect(3,3);
		dc.Draw3dRect(&rc, xtAfxData.clr3DShadow, xtAfxData.clr3DHilight);
	}
	else {
		dc.Draw3dRect(&rc, xtAfxData.clrWindowFrame, xtAfxData.clrWindowFrame);
	}

	rcText = rc;
	rcText.top -= 22;
	rcText.bottom = rcText.top + 22;

	strText.LoadString(XT_IDS_NEW);
	dc.DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	rcText = rc;
	rcText.top =  rcText.bottom;
	rcText.bottom = rcText.top + 22;

	strText.LoadString(XT_IDS_CURRENT);
	dc.DrawText(strText, rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	dc.SelectObject(pOldFont);

	rc.DeflateRect(1,1);

	int cy = rc.Height()/2;

	m_rcNew = rc;
	m_rcNew.bottom = rc.top + cy;
	dc.FillSolidRect(&m_rcNew, m_clrNew);
	
	m_rcCurrent = rc;
	m_rcCurrent.top = rc.bottom - cy;
	dc.FillSolidRect(&m_rcCurrent, m_clrCurrent);
}

void CXTColorDialog::SetNewColor(COLORREF clr, BOOL bNotify/*=TRUE*/)
{
	m_clrNew = clr;

	if (IsWindowVisible())
	{
		CClientDC dc(this);
		dc.FillSolidRect(&m_rcNew, m_clrNew);

		if (m_dwStyle & CPS_XT_SHOWHEXVALUE) {
			m_editHex.SetWindowText(RGBtoHex(m_clrNew));
		}
	}

	if (bNotify && GetTabControl() && GetTabControl()->GetSafeHwnd())
	{
		int iPage;
		for (iPage = 0; iPage < GetPageCount(); ++iPage)
		{
			GetPage(iPage)->SendMessage(WM_XT_UPDATECOLOR,
				(WPARAM)(COLORREF)m_clrNew);
		}
	}
}

void CXTColorDialog::SetCurrentColor(COLORREF clr)
{
	m_clrCurrent = clr;

	if (IsWindowVisible())
	{
		CClientDC dc(this);
		dc.FillSolidRect(&m_rcCurrent, m_clrCurrent);
	}
}

BOOL CXTColorDialog::CopyToClipboard(CString strText)
{
	if (::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();

		HGLOBAL hGlobalBuff = ::GlobalAlloc(GMEM_MOVEABLE, strText.GetLength()+1);

		TCHAR* szBuffer = (TCHAR*)::GlobalLock(hGlobalBuff);

		_tcscpy(szBuffer, strText);
		::GlobalUnlock(hGlobalBuff);

		if (::SetClipboardData(CF_TEXT, hGlobalBuff) == NULL)
			return FALSE;

		::CloseClipboard();
		return TRUE;
	}

	return FALSE;
}

CString CXTColorDialog::RGBtoHex(COLORREF clr) const
{
	int r = GetRValue(clr);
	int g = GetGValue(clr);
	int b = GetBValue(clr);

	CString strHex;

	if (r < 16 && g < 16 && b < 16) {
		strHex.Format(_T("Hex: 0%X0%X0%X"), r, g, b);
	}
	else if (r < 16 && g < 16) {
		strHex.Format(_T("Hex: 0%X0%X%X"), r, g, b);
	}
	else if (r < 16 && b < 16) {
		strHex.Format(_T("Hex: 0%X%X0%X"), r, g, b);
	}
	else if (g < 16 && b < 16) {
		strHex.Format(_T("Hex: %X0%X0%X"), r, g, b);
	}
	else if (r < 16) {
		strHex.Format(_T("Hex: 0%X%X%X"), r, g, b);
	}
	else if (g < 16) {
		strHex.Format(_T("Hex: %X0%X%X"), r, g, b);
	}
	else if (b < 16) {
		strHex.Format(_T("Hex: %X%X0%X"), r, g, b);
	}
	else {
		strHex.Format(_T("Hex: %X%X%X"), r, g, b);
	}

	return strHex;
}

int CXTColorDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_dwStyle & CPS_XT_SHOWHEXVALUE)
	{
		CRect rect;

		rect.right  = 317; 
		rect.left   = 243;
		rect.bottom = 180;
		rect.top    = 162;

		if (!m_editHex.CreateEx(WS_EX_STATICEDGE, _T("edit"), RGBtoHex(m_clrNew),
			WS_CHILD|WS_VISIBLE|ES_READONLY, rect, this, AFX_IDC_CHANGE))
		{
			TRACE0("Failed to create edit control.\n");
			return -1;
		}

		// Set the font and mask for the edit control.
		m_editHex.SetFont (&xtAfxData.font);
	}

	((CXTColorPageStandard*)GetPage(0))->SetColor(m_clrNew);

	return 0;
}
