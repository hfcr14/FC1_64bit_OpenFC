// XTTabCtrl.cpp : implementation of the CXTTabCtrl class.
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

#ifndef _XTP_INCLUDE_CONTROLS
#include "XTToolBarCtrl.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTCoolMenu.h"
#include "XTAccelManager.h"
#include "XTAccelKeyEdit.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTFrameImpl.h"
#include "XTFrameWnd.h"
#include "XTMDIFrameWnd.h"
#endif

#include "XTMemDC.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTResizeFormView.h"
#include "XTResizePropertyPage.h"
#include "XTResizePropertySheet.h"
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrl.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTTabCtrlButtons.h"
#include "XTTabCtrl.h"
#include "XTColorRef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int TABVIEW_BORDER = 3;


/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrlBase

CXTTabCtrlBase::CXTTabCtrlBase()
{
	m_bAutoCondensing = FALSE;
	m_bDisableXPMode = false;
	m_bXPBorder = true;
	m_bBoldFont = true;
	m_iEdge     = ::GetSystemMetrics(SM_CYEDGE);
	m_pTabCtrl  = NULL;  // for now 

	m_penBlack.CreatePen(PS_SOLID, 1, xtAfxData.clrBtnText);
	m_penWhite.CreatePen(PS_SOLID, 1, xtAfxData.clr3DHilight);
	m_penFace. CreatePen(PS_SOLID, 1, xtAfxData.clr3DFace);
	m_penText. CreatePen(PS_SOLID, 1, xtAfxData.clr3DShadow);
	
}

CXTTabCtrlBase::~CXTTabCtrlBase()
{
	m_penBlack.DeleteObject();
	m_penWhite.DeleteObject();
	m_penFace.DeleteObject();
	m_penText.DeleteObject();
	
	SAFE_DELETE(m_pNavBtns);
}

void CXTTabCtrlBase::ImplAttach(CTabCtrl *pTabCtrl)
{
	ASSERT(pTabCtrl != NULL);
	m_pTabCtrl = pTabCtrl;
	
	m_pNavBtns = new CXTTabCtrlButtons;
	ASSERT(m_pNavBtns != NULL);

	m_pNavBtns->Create(m_pTabCtrl);	
}

void CXTTabCtrlBase::ShowNavButtons(DWORD dwFlags)
{
	if ( m_pNavBtns )
	{
		m_pNavBtns->m_dwFlags = dwFlags;
		if (m_pNavBtns->ShowButtons())
		{
			m_pNavBtns->Refresh();
		}
	}
}

void CXTTabCtrlBase::PaintButtons()
{
	if ( m_pNavBtns )
	{
		const bool bVertical = (m_pTabCtrl->GetStyle() & TCS_VERTICAL) == TCS_VERTICAL;
		const bool bMultiLine = bVertical || ((m_pTabCtrl->GetStyle() & TCS_MULTILINE) == TCS_MULTILINE);
		//const bool bTop = !bVertical && ((m_pTabCtrl->GetStyle() & TCS_BOTTOM) == 0);

		if (xtAfxData.bXPMode && !bVertical && !bMultiLine /*&& bTop*/ && !m_bAutoCondensing)
		{
			m_pNavBtns->SubclassTabButtons(m_pTabCtrl->GetDlgItem(1));
			m_pNavBtns->ShowButtons(TRUE);			
		}
		else
		{
			m_pNavBtns->ShowButtons(FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrlBase message handlers

// needed in order to access protected members
class CWnd_Friendly : public CWnd
{
	friend class CXTTabCtrlBase;
};

BOOL CXTTabCtrlBase::OnEraseBkgndImpl(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTTabCtrlBase::OnPaintImpl()
{
	CPaintDC dc(m_pTabCtrl); // device context for painting

	// Create a memory device context to help reduce
	// screen flicker.
	CRect rcClient;
	m_pTabCtrl->GetClientRect(&rcClient);

	CXTMemDC memDC(&dc, rcClient);

    if (m_bDisableXPMode == false && xtAfxData.bXPMode)
	{
		if (m_pTabCtrl->GetItemCount() != 0)
		{
			// Save the device context state.
			int nSavedDC = memDC.SaveDC();
			
			// Get the current tab selection.
			int iCurSel = m_pTabCtrl->GetCurSel();
			
			// get the selected tab item rect.
			CRect rcItem;
			m_pTabCtrl->GetItemRect(iCurSel, &rcItem);
			
			DWORD dwStyle = m_pTabCtrl->GetStyle();
			
			// Vertical tabs
			if (dwStyle & TCS_VERTICAL)
			{
				// Right
				if (dwStyle & TCS_RIGHT) {
					OnDrawRight(&memDC, rcClient, rcItem);
				}
				// Left
				else {
					OnDrawLeft(&memDC, rcClient, rcItem);
				}
			}
			
			// Horizontal tabs
			else
			{
				// Bottom
				if (dwStyle & TCS_BOTTOM) {
					OnDrawBottom(&memDC, rcClient, rcItem);
				}
				// Top
				else {
					OnDrawTop(&memDC, rcClient, rcItem);
				}
			}
			
			PaintButtons();

			if (m_bXPBorder)
			{
				CRect rcChild;
				GetChildRect(rcChild);
				rcChild.InflateRect(1,1);
				
				memDC.Draw3dRect(&rcChild,
					xtAfxData.clr3DShadow, xtAfxData.clr3DShadow);
			}
			
			memDC.RestoreDC(nSavedDC);
		}
		else
		{
			PaintButtons();
		}
	}
	else
	{
		PaintButtons();
		((CWnd_Friendly *)m_pTabCtrl)->DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	}
}

void CXTTabCtrlBase::OnSettingChangeImpl_Post(UINT uFlags, LPCTSTR lpszSection) 
{
	UNUSED_ALWAYS(uFlags);
	UNUSED_ALWAYS(lpszSection);

	// Update system metrics.
	m_iEdge = ::GetSystemMetrics(SM_CYEDGE);
}

void CXTTabCtrlBase::OnSysColorChangeImpl_Post() 
{
	// Update pen colors
	m_penBlack.DeleteObject();
	m_penBlack.CreatePen(PS_SOLID, 1, xtAfxData.clrBtnText);

	m_penWhite.DeleteObject();
	m_penWhite.CreatePen(PS_SOLID, 1, xtAfxData.clr3DHilight);

	m_penFace.DeleteObject();
	m_penFace.CreatePen(PS_SOLID, 1, xtAfxData.clr3DFace);

	m_penText.DeleteObject();
	m_penText.CreatePen(PS_SOLID, 1, xtAfxData.clr3DShadow);
}

void CXTTabCtrlBase::OnDrawBottom(CDC *pDC, CRect rcClient, CRect rcItem)
{
	// Make sure the correct font is selected.
	pDC->SelectObject(&xtAfxData.font);

	CRect rcHeight;
	m_pTabCtrl->GetItemRect(-1, &rcHeight);
	// repaint the tab area.
	rcClient.top = rcHeight.top - 1;
	pDC->FillSolidRect(rcClient, xtAfxData.clrXPTabInactiveBack);

   	// draw a line to separate tab area.
	pDC->SelectObject(&m_penBlack);

	pDC->MoveTo(rcClient.left, rcClient.top - 1);
	pDC->LineTo(rcClient.right, rcClient.top - 1);

	// paint the selected tab background.
	pDC->FillSolidRect(&rcItem, xtAfxData.clr3DFace);

	// draw the selected tab.
	pDC->MoveTo(rcItem.right, rcItem.top+1);
	pDC->LineTo(rcItem.right, rcItem.bottom-1);
	pDC->MoveTo(rcItem.right, rcItem.bottom-1);
	pDC->LineTo(rcItem.left,  rcItem.bottom-1);
	
	pDC->SelectObject(&m_penWhite);
	pDC->MoveTo(rcItem.left, rcItem.top);
	pDC->LineTo(rcItem.left, rcItem.bottom);
	
	pDC->SelectObject(&m_penFace);
	pDC->MoveTo(rcItem.right-1, rcItem.bottom-2);
	pDC->LineTo(rcItem.left,    rcItem.bottom-2);

	// Get the current tab selection.
	int iCurSel = m_pTabCtrl->GetCurSel();

	// draw remaining tabs
	int iItem;
	for (iItem = 0; iItem < m_pTabCtrl->GetItemCount(); ++iItem)
	{
		m_pTabCtrl->GetItemRect(iItem, &rcItem);

		TCHAR szLabel[256];
		TC_ITEM tci;
		tci.mask       = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText    = szLabel;     
		tci.cchTextMax = _countof(szLabel);    	

		if (!m_pTabCtrl->GetItem(iItem, &tci )) {
			continue;
		}

		pDC->SetBkMode(TRANSPARENT);

		CRect rcText(rcItem);
		
		// Draw any associated icons.
		CImageList* pImageList = m_pTabCtrl->GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			CPoint point = rcItem.TopLeft();
			point.x += 2;

			point.Offset(m_iEdge, m_iEdge);
			pImageList->Draw(pDC, tci.iImage, point, ILD_TRANSPARENT);

			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect rcImage(info.rcImage);
			rcText.left += rcImage.Width() + m_iEdge;
		}

		// Set the font for the tab label.
		if (iItem == iCurSel)
		{
			pDC->SetTextColor(xtAfxData.clrBtnText);
			pDC->SelectObject(m_bBoldFont ? &xtAfxData.fontBold : &xtAfxData.font);
		}
		else
		{
			pDC->SetTextColor(xtAfxData.clrXPTabInactiveText);
			pDC->SelectObject(&xtAfxData.font);
		}

		// Draw the tab label.
		pDC->DrawText(szLabel, rcText, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		if (iItem == iCurSel || iItem == iCurSel-1) {
			continue;
		}

		// Draw the tab separator.
		pDC->SelectObject(&m_penText);
		pDC->MoveTo(rcItem.right, rcItem.top+4);
		pDC->LineTo(rcItem.right, rcItem.bottom-2);
	}
}

void CXTTabCtrlBase::OnDrawTop(CDC *pDC, CRect rcClient, CRect rcItem)
{
	// Make sure the correct font is selected.
	pDC->SelectObject(&xtAfxData.font);

	int iBottom = rcClient.top + rcItem.Height();

	// repaint the tab area.
	CRect rcHeight;
	m_pTabCtrl->GetItemRect(-1, &rcHeight);
	rcClient.bottom = rcHeight.bottom;
	pDC->FillSolidRect(rcClient, xtAfxData.clrXPTabInactiveBack);

   	// draw a line to separate tab area.
	pDC->SelectObject(&m_penWhite);
	pDC->MoveTo(rcClient.left,  iBottom);
	pDC->LineTo(rcClient.right, iBottom);

	// paint the selected tab background.
	pDC->FillSolidRect(&rcItem, xtAfxData.clr3DFace);

	// draw the selected tab.
	pDC->MoveTo(rcItem.left, rcItem.top);
	pDC->LineTo(rcItem.left, iBottom + 1);
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.left,  rcItem.top);
	
	pDC->SelectObject(&m_penBlack);
	pDC->MoveTo(rcItem.right, iBottom-1);
	pDC->LineTo(rcItem.right, rcItem.top-1);
	
	pDC->SelectObject(&m_penFace);
	pDC->MoveTo(rcItem.left+1, iBottom);
	pDC->LineTo(rcItem.right, iBottom);

	// Get the current tab selection.
	int iCurSel = m_pTabCtrl->GetCurSel();

	// draw remaining tabs
	int iItem;
	for (iItem = 0; iItem < m_pTabCtrl->GetItemCount(); ++iItem)
	{
		m_pTabCtrl->GetItemRect(iItem, &rcItem);

		TCHAR szLabel[256];
		TC_ITEM tci;
		tci.mask       = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText    = szLabel;     
		tci.cchTextMax = _countof(szLabel);    	

		if (!m_pTabCtrl->GetItem(iItem, &tci )) {
			continue;
		}

		pDC->SetBkMode(TRANSPARENT);

		CRect rcText(rcItem);
		
		// Draw any associated icons.
		CImageList* pImageList = m_pTabCtrl->GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			CPoint point = rcItem.TopLeft();
			point.x += 2;
			point.Offset(m_iEdge, m_iEdge);
			pImageList->Draw(pDC, tci.iImage, point, ILD_TRANSPARENT);

			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect rcImage(info.rcImage);
			rcText.left += rcImage.Width() + m_iEdge;
		}

		// Set the font for the tab label.
		if (iItem == iCurSel)
		{
			pDC->SetTextColor(xtAfxData.clrBtnText);
			pDC->SelectObject(m_bBoldFont ? &xtAfxData.fontBold : &xtAfxData.font);
		}
		else
		{
			pDC->SetTextColor(xtAfxData.clrXPTabInactiveText);
			pDC->SelectObject(&xtAfxData.font);
		}

		// Draw the tab label.
		pDC->DrawText(szLabel, rcText, DT_SINGLELINE|DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS);

		if (iItem == iCurSel || iItem == iCurSel-1) {
			continue;
		}

		// Draw the tab separator.
		pDC->SelectObject(&m_penText);
		pDC->MoveTo(rcItem.right, rcItem.top+3);
		pDC->LineTo(rcItem.right, rcItem.bottom-3);
	}
}

void CXTTabCtrlBase::OnDrawLeft(CDC *pDC, CRect rcClient, CRect rcItem)
{
	// repaint the tab area.
	rcClient.right = rcItem.right;
	pDC->FillSolidRect(rcClient, xtAfxData.clrXPTabInactiveBack);

   	// draw a line to separate tab area.
	pDC->SelectObject(&m_penWhite);
	pDC->MoveTo(rcClient.right, rcClient.top);
	pDC->LineTo(rcClient.right, rcClient.bottom);

	// paint the selected tab background.
	pDC->FillSolidRect(&rcItem, xtAfxData.clr3DFace);

	// draw the selected tab.
	pDC->MoveTo(rcItem.left, rcItem.top);
	pDC->LineTo(rcItem.left, rcItem.bottom);
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.left,  rcItem.top);
	
	pDC->SelectObject(&m_penBlack);
	pDC->MoveTo(rcItem.left, rcItem.bottom);
	pDC->LineTo(rcItem.right, rcItem.bottom);
	
	pDC->SelectObject(&m_penFace);
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.right, rcItem.bottom);

	// Get the current tab selection.
	int iCurSel = m_pTabCtrl->GetCurSel();

	// draw remaining tabs
	int iItem;
	for (iItem = 0; iItem < m_pTabCtrl->GetItemCount(); ++iItem)
	{
		m_pTabCtrl->GetItemRect(iItem, &rcItem);

		TCHAR szLabel[256];
		TC_ITEM tci;
		tci.mask       = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText    = szLabel;     
		tci.cchTextMax = _countof(szLabel);    	

		if (!m_pTabCtrl->GetItem(iItem, &tci )) {
			continue;
		}

		pDC->SetBkMode(TRANSPARENT);

		CRect rcText(rcItem);
		
		CPoint point(rcItem.TopLeft());
		point.Offset(m_iEdge, m_iEdge);

		// Draw any associated icons.
		CImageList* pImageList = m_pTabCtrl->GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			pImageList->Draw(pDC, tci.iImage, point, ILD_TRANSPARENT);

			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect rcImage(info.rcImage);
			rcText.top += rcImage.Height() + m_iEdge;
		}

		// Set the font for the tab label.
		if (iItem == iCurSel)
		{
			pDC->SetTextColor(xtAfxData.clrBtnText);
			pDC->SelectObject(m_bBoldFont ? &xtAfxData.fontVCaptBold : &xtAfxData.fontVCapt);
		}
		else
		{
			pDC->SetTextColor(xtAfxData.clrXPTabInactiveText);
			pDC->SelectObject(&xtAfxData.fontVCapt);
		}

		CSize size = pDC->GetTextExtent(szLabel);

		int x = point.x;
		int y = rcText.bottom-((rcText.Height()-size.cx)/2);

		// Draw the tab label.
		pDC->ExtTextOut(x, y, ETO_CLIPPED, rcText, szLabel, NULL);

		if (iItem == iCurSel || iItem == iCurSel-1) {
			continue;
		}

		// Draw the tab separator.
		pDC->SelectObject(&m_penText);
		pDC->MoveTo(rcItem.left +2, rcItem.bottom);
		pDC->LineTo(rcItem.right-2, rcItem.bottom);
	}
}

void CXTTabCtrlBase::OnDrawRight(CDC *pDC, CRect rcClient, CRect rcItem)
{
	// repaint the tab area.
	rcClient.left = rcItem.left;
	pDC->FillSolidRect(rcClient, xtAfxData.clrXPTabInactiveBack);

   	// draw a line to separate tab area.
	pDC->SelectObject(&m_penBlack);
	pDC->MoveTo(rcClient.left, rcClient.top);
	pDC->LineTo(rcClient.left, rcClient.bottom);

	// paint the selected tab background.
	pDC->FillSolidRect(&rcItem, xtAfxData.clr3DFace);

	// draw the selected tab.
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.right, rcItem.bottom+1);
	pDC->MoveTo(rcItem.left, rcItem.bottom);
	pDC->LineTo(rcItem.right, rcItem.bottom);
	
	pDC->SelectObject(&m_penWhite);
	pDC->MoveTo(rcItem.right, rcItem.top);
	pDC->LineTo(rcItem.left,  rcItem.top);
	
	pDC->SelectObject(&m_penFace);
	pDC->MoveTo(rcItem.left, rcItem.top);
	pDC->LineTo(rcItem.left, rcItem.bottom);

	// Get the current tab selection.
	int iCurSel = m_pTabCtrl->GetCurSel();

	// draw remaining tabs
	int iItem;
	for (iItem = 0; iItem < m_pTabCtrl->GetItemCount(); ++iItem)
	{
		m_pTabCtrl->GetItemRect(iItem, &rcItem);

		TCHAR szLabel[256];
		TC_ITEM tci;
		tci.mask       = TCIF_TEXT|TCIF_IMAGE;
		tci.pszText    = szLabel;     
		tci.cchTextMax = _countof(szLabel);    	

		if (!m_pTabCtrl->GetItem(iItem, &tci )) {
			continue;
		}

		pDC->SetBkMode(TRANSPARENT);

		CRect rcText(rcItem);
		
		CPoint point(rcItem.TopLeft());
		point.Offset(m_iEdge, m_iEdge);

		// Draw any associated icons.
		CImageList* pImageList = m_pTabCtrl->GetImageList();
		if (pImageList && tci.iImage >= 0)
		{
			pImageList->Draw(pDC, tci.iImage, point, ILD_TRANSPARENT);

			IMAGEINFO info;
			pImageList->GetImageInfo(tci.iImage, &info);
			CRect rcImage(info.rcImage);
			rcText.top += rcImage.Height() + m_iEdge;
		}

		// Set the font for the tab label.
		if (iItem == iCurSel)
		{
			pDC->SetTextColor(xtAfxData.clrBtnText);
			pDC->SelectObject(m_bBoldFont ? &xtAfxData.fontVCaptBold : &xtAfxData.fontVCapt);
		}
		else
		{
			pDC->SetTextColor(xtAfxData.clrXPTabInactiveText);
			pDC->SelectObject(&xtAfxData.fontVCapt);
		}

		CSize size = pDC->GetTextExtent(szLabel);

		int x = point.x;
		int y = rcText.bottom-((rcText.Height()-size.cx)/2);

		// Draw the tab label.
		pDC->ExtTextOut(x, y, ETO_CLIPPED, rcText, szLabel, NULL);

		if (iItem == iCurSel || iItem == iCurSel-1) {
			continue;
		}

		// Draw the tab separator.
		pDC->SelectObject(&m_penText);
		pDC->MoveTo(rcItem.left +2, rcItem.bottom);
		pDC->LineTo(rcItem.right-2, rcItem.bottom);
	}
}

void CXTTabCtrlBase::OnAddPadding(CXTString& strLabelText)
{
	if (!strLabelText.IsEmpty())
	{
		CClientDC dc(NULL);
		int iSaveDC = dc.SaveDC();

		CFont* pfontNormal = NULL;
		CFont* pfontSelect = NULL;

		if (m_pTabCtrl->GetStyle() & TCS_VERTICAL)
		{
			pfontNormal = &xtAfxData.fontVCaptR;
			pfontSelect = &xtAfxData.fontVCaptRBold;
		}
		else
		{
			pfontNormal = &xtAfxData.fontVCapt;
			pfontSelect = &xtAfxData.fontVCaptBold;
		}

		dc.SelectObject(pfontSelect);
		CSize sizeSelect = dc.GetTextExtent(strLabelText);
		
		dc.SelectObject(pfontNormal);
		CSize sizeNormal = dc.GetTextExtent(strLabelText);

		bool bFront = true;

		while (sizeNormal.cx < sizeSelect.cx)
		{
			if (bFront)
			{
				strLabelText.Insert(0, _T(' '));
				bFront = false;
			}
			else
			{
				strLabelText += _T(' ');
				bFront = true;
			}
			sizeNormal = dc.GetTextExtent(strLabelText);
		}

		dc.RestoreDC(iSaveDC);
	}
}

void CXTTabCtrlBase::GetChildRect(CRect& rcChild) const
{
	ASSERT_VALID(m_pTabCtrl);

	// Get the selected tab index.
	int nCurSel = m_pTabCtrl->GetCurSel();
	if (nCurSel == -1)
	{
		rcChild.SetRectEmpty();
		return;
	}

	DWORD dwStyle = m_pTabCtrl->GetStyle();
	
	CRect rcTab;
	m_pTabCtrl->GetItemRect(nCurSel, &rcTab);
	m_pTabCtrl->GetClientRect(&rcChild);
	
	int cy = rcTab.Height() * m_pTabCtrl->GetRowCount();
	int cx = rcTab.Width () * m_pTabCtrl->GetRowCount();
	
	// vertical tabs
	if (dwStyle & TCS_VERTICAL)
	{
		// Right
		if (dwStyle & TCS_RIGHT)
		{
			rcChild.top    += TABVIEW_BORDER;
			rcChild.left   += TABVIEW_BORDER;
			rcChild.right  -= cx + 5;
			rcChild.bottom -= TABVIEW_BORDER;
		}
		
		// Left
		else
		{
			rcChild.top    += TABVIEW_BORDER;
			rcChild.left   += cx + 5;
			rcChild.right  -= TABVIEW_BORDER;
			rcChild.bottom -= TABVIEW_BORDER;
		}
	}
	
	// horizontal tabs
	else
	{
		// Bottom
		if (dwStyle & TCS_BOTTOM)
		{
			rcChild.top    += TABVIEW_BORDER;
			rcChild.left   += TABVIEW_BORDER;
			rcChild.right  -= TABVIEW_BORDER;
			rcChild.bottom -= cy + 5;
		}
		
		// Top
		else
		{
			rcChild.top    += cy + 5;
			rcChild.left   += TABVIEW_BORDER;
			rcChild.right  -= TABVIEW_BORDER;
			rcChild.bottom -= TABVIEW_BORDER;
		}
	}
}


/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrlBaseEx

CXTTabCtrlBaseEx::CXTTabCtrlBaseEx()
{
	m_bBoldFont       = false;
	m_bInitialUpdate  = FALSE;
	m_pParentWnd      = NULL;
	m_pParentFrame    = NULL;
	m_popupMenuID     = 0;
	m_nPos		      = 0;
	m_pLastActiveView = NULL;
	m_dwInitSignature = 1;
	m_nOldIndex       = -1;
	m_bInitialUpdatePending = false;

	// force VC++ to generate the following template functions:
	if (GetAutoCondense()) // should always be false!
	{
		// this block will never be executed, but the compiler can't know that
		SetAutoCondense(FALSE);
		OnPaintImpl();
		CreateTabView(NULL, NULL, NULL);
	}
}

CXTTabCtrlBaseEx::~CXTTabCtrlBaseEx()
{
}

void CXTTabCtrlBaseEx::OnDestroyImpl_Pre() 
{
	// Cleanup 
	while (!m_tcbItems.IsEmpty())
	{
		XT_TCB_ITEM *pMember = m_tcbItems.RemoveHead();
		if (::IsWindow(pMember->pWnd->m_hWnd))
			pMember->pWnd->PostMessage(WM_CLOSE);
		SAFE_DELETE(pMember);
	}
}

void CXTTabCtrlBaseEx::OnSelchangeImpl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED_ALWAYS(pNMHDR);

	*pResult = 0;
	OnSelChange();

	// turn redraw back on, and update the tab control.
	m_pTabCtrl->SetRedraw(true);
	m_pTabCtrl->Invalidate();
	m_pTabCtrl->UpdateWindow();
}
					 
// OnSelchange() was needed for API compatibility
void CXTTabCtrlBaseEx::OnSelChange() 
{
	// Get the index to the newly selected tab.
	int nIndex = m_pTabCtrl->GetCurSel();
	if(nIndex == -1) {
		return;
	}

	// Get a pointer to the selected view.
	if (m_nOldIndex != -1)
	{
		CWnd* pOldView = GetView(m_nOldIndex);
		if (pOldView != NULL)
		{
			pOldView->ShowWindow(SW_HIDE);
			pOldView->EnableWindow(FALSE);

			if (m_pParentFrame && pOldView == m_pParentFrame->GetActiveView())
			{
				if (m_pLastActiveView && ::IsWindow(m_pLastActiveView->m_hWnd)) {
					m_pParentFrame->SetActiveView(m_pLastActiveView);
				}
				else {
					m_pParentFrame->SetActiveView(NULL, FALSE);
					m_pLastActiveView = NULL;
				}
			}
		}
	}

	// Get a pointer to the selected view.
	CWnd* pView = GetView(nIndex);
	if (pView != NULL)
	{
		// If this is a CView object then set the active view
		// the parent frame.
		if (pView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			if (m_pParentFrame && m_pParentFrame->IsChild(pView))
			{
				// Save a pointer to the view if it is not a child of the
				// tab control, we will use this rather than setting the view
				// to NULL for the frame window.
				CView* pActiveView = m_pParentFrame->GetActiveView();
				if (pActiveView && pActiveView->GetParent() != m_pTabCtrl)
				{
					m_pLastActiveView = pActiveView;
				}
				m_pParentFrame->SetActiveView((CView*)pView);
			}
		}

		ActivateView(pView);
	}

	// Get a pointer to the owner window...
	CWnd* pWndOwner = m_pTabCtrl->GetOwner();
	ASSERT_VALID(pWndOwner);
	
	// and pass along this message.
	pWndOwner->SendMessage(TCN_XT_SELCHANGE,
		m_pTabCtrl->GetDlgCtrlID(), (LPARAM)m_pTabCtrl);
}

void CXTTabCtrlBaseEx::OnSelchangingImpl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED_ALWAYS(pNMHDR);
	*pResult = 0;

	// turn off redraw, this will eliminate that nasty flickering
	// you see when you switch tabs.
	m_pTabCtrl->SetRedraw(false);

	OnSelChanging();
}

// OnSelchanging was needed for API compatibility
void CXTTabCtrlBaseEx::OnSelChanging() 
{
	// Get the index to the selected tab, we will
	// need this later to hide old view.
	m_nOldIndex = m_pTabCtrl->GetCurSel();
	if (m_nOldIndex == -1) {
		return;
	}

	// Get a pointer to the owner window...
	CWnd* pWndOwner = m_pTabCtrl->GetOwner();
	ASSERT_VALID(pWndOwner);
	
	// and pass along this message.
	pWndOwner->SendMessage(TCN_XT_SELCHANGING,
		m_pTabCtrl->GetDlgCtrlID(), (LPARAM)m_pTabCtrl);
}

void CXTTabCtrlBaseEx::OnWindowPosChangedImpl_Pre(WINDOWPOS FAR* lpwndpos) 
{
	if( lpwndpos->flags & SWP_HIDEWINDOW )
	{
		POSITION pos;
		for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
		{
			CWnd* pView = m_tcbItems.GetAt(pos)->pWnd;
			ASSERT_VALID(pView);

			if (m_pParentFrame && m_pParentFrame->GetActiveView() == pView)
			{
				// To avoid main window freezing, we must deactivate the view,
				// because it's not visible now.

				if (m_pLastActiveView && ::IsWindow(m_pLastActiveView->m_hWnd)) {
					m_pParentFrame->SetActiveView(m_pLastActiveView);
				}
				else {
					m_pParentFrame->SetActiveView(NULL, FALSE);
					m_pLastActiveView = NULL;
				}
			}
		}
	}
	else
	{
		// resize all child views.
		POSITION pos;
		for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos)) {
			ResizeTabView(m_tcbItems.GetAt(pos)->pWnd);
		}
	}
}

void CXTTabCtrlBaseEx::OnWindowPosChangedImpl_Post(WINDOWPOS FAR* lpwndpos) 
{
	UNUSED_ALWAYS(lpwndpos);
	if (m_bAutoCondensing)
	{
		Condense();
	}
}

BOOL CXTTabCtrlBaseEx::ModifyTabStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags)
{
	if (!m_pTabCtrl->ModifyStyle(dwRemove, dwAdd, nFlags))
		return FALSE;

	// Set the font for the tab control.
	InitializeFont();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrlBaseEx tooltip related functions

CToolTipCtrl* CXTTabCtrlBaseEx::GetTips()
{
#if _MSC_VER < 1200 // MFC 5.0
	return m_pTabCtrl->GetTooltips();
#else
	return m_pTabCtrl->GetToolTips();
#endif // _MSC_VER < 1200
}

void CXTTabCtrlBaseEx::SetTips(CToolTipCtrl* pWndTip)
{
#if _MSC_VER < 1200 // MFC 5.0
	m_pTabCtrl->SetTooltips(pWndTip);
#else
	m_pTabCtrl->SetToolTips(pWndTip);
#endif // _MSC_VER < 1200
}

void CXTTabCtrlBaseEx::AddToolTip(UINT nIDTab, LPCTSTR lpszText)
{
	// Get the tool tips for the tab control.
	CToolTipCtrl* pToolTips = GetTips();

	if (pToolTips != NULL)
	{
		// Add the tool tip.
		if (nIDTab == 0)
		{
			pToolTips->AddTool(m_pTabCtrl, lpszText, NULL, nIDTab);
		}
		else
		{
			pToolTips->AddTool(m_pTabCtrl, lpszText, CRect(0,0,0,0), nIDTab);
		}
	}
}

void CXTTabCtrlBaseEx::UpdateToolTip(int nIDTab, LPCTSTR lpszText)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(( nIDTab >= 0 ) && (m_pTabCtrl->GetItemCount() > nIDTab));

	// Find the view we want to change the tooltip for and
	// reset its tooltip text
	POSITION pos = m_tcbItems.FindIndex(nIDTab);
	if (pos != NULL)
	{
		// Get the private data for the view
		XT_TCB_ITEM *pMember = m_tcbItems.GetAt(pos);
		ASSERT(pMember);

		// Reset the tooltip text to the new value
		pMember->szToolTipLabel = lpszText;
		
		// Get the tool tips for the tab control.
		CToolTipCtrl* pToolTips = GetTips();

		if (pToolTips != NULL)
		{
			// Update the tooltip for the view
			pToolTips->UpdateTipText(pMember->szToolTipLabel,
				m_pTabCtrl, pMember->uiToolTipId);
		}
	}
}

void CXTTabCtrlBaseEx::UpdateToolTip(CRuntimeClass *pViewClass, LPCTSTR lpszText)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
	
	int nIndex=0;
	POSITION pos;
	for (pos=m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		// Get the private data for the view
		XT_TCB_ITEM *pMember = m_tcbItems.GetAt(pos);
		ASSERT(pMember);

		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			UpdateToolTip(nIndex, lpszText);
			return;
		}
		nIndex++;
	}
}

void CXTTabCtrlBaseEx::ResetToolTips()
{
	// Get the tool tips for the tab control.
	CToolTipCtrl* pToolTips = GetTips();

	if (pToolTips != NULL)
	{
		int nIndex = 0;

		POSITION pos;
		for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
		{
			// Get the next item in the list.
			XT_TCB_ITEM* pMember = m_tcbItems.GetAt( pos );
			
			// Update the tooltip for the view
			pMember->uiToolTipId = nIndex;

			pToolTips->UpdateTipText(pMember->szToolTipLabel,
				m_pTabCtrl, nIndex);

			 ++nIndex;
		}
	}
}

BOOL CXTTabCtrlBaseEx::EnableToolTipsImpl(BOOL bEnable)
{
	// Get the tool tips for the tab control.
	CToolTipCtrl* pToolTips = GetTips();

	if (pToolTips != NULL)
	{
		pToolTips->Activate(bEnable);
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrlBaseEx tab view methods.

CWnd* CXTTabCtrlBaseEx::CreateTabView(CRuntimeClass *pViewClass, CDocument *pDocument, CCreateContext *pContext)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pContext || pViewClass != NULL);
	ASSERT(pContext || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		// client if possible.
		contextT.m_pCurrentDoc	   = pDocument;
		contextT.m_pNewViewClass   = pViewClass;
		contextT.m_pLastView	   = NULL;
		contextT.m_pCurrentFrame   = NULL;
		contextT.m_pNewDocTemplate = NULL;

		if (pDocument)
		{
			contextT.m_pNewDocTemplate = pDocument->GetDocTemplate();
		}

		pContext = &contextT;
	}

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pContext->m_pNewViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" );
		// Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL
		
	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL); // not yet created.

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;

        if (m_bDisableXPMode == false && xtAfxData.bXPMode) {
		dwStyle &= ~WS_BORDER;
	}

	int nTab = m_tcbItems.GetCount();

	// Create with the right size (wrong position)
	CRect rect(0,0,0,0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, m_pTabCtrl, (AFX_IDW_PANE_FIRST + nTab), pContext))
	{
		TRACE0( "Warning: couldn't create client tab for view.\n" );
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}
	ASSERT((int)_AfxGetDlgCtrlID(pWnd->m_hWnd) == (AFX_IDW_PANE_FIRST + nTab));

	// send initial notification message for the window just created
	// only if our initialization has been completed
	if (m_bInitialUpdate && !m_bInitialUpdatePending) 
	{
		pWnd->SendMessage(WM_INITIALUPDATE);
	}

	pWnd->SetOwner(m_pParentWnd);
	return pWnd;
}

BOOL CXTTabCtrlBaseEx::AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CDocument* pDoc/*=NULL*/,
	CCreateContext *pContext/*=NULL*/, int iIndex/*=-1*/, int iIconIndex/*=-1*/)
{	
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pContext || pViewClass);
	ASSERT(pContext || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
	
	// Create the view to be associate with tab.
	CWnd* pWnd = CreateTabView(pViewClass, pDoc, pContext);
	if (pWnd == NULL) // tabs can be a CWnd, but are usually CViews
		return FALSE;

	return AddControl(lpszLabel, (CView *)pWnd, iIndex, iIconIndex);
}

BOOL CXTTabCtrlBaseEx::AddView(LPCTSTR lpszLabel, CView* pView, int iIndex/*=-1*/, int iIconIndex/*=-1*/)
{
	ASSERT(pView != NULL);
	ASSERT(pView->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CView)));
	return AddControl(lpszLabel, pView, iIndex, iIconIndex);
}

BOOL CXTTabCtrlBaseEx::AddControl(LPCTSTR lpszLabel, CWnd* pWnd, int iIndex/*=-1*/, int iIconIndex/*=-1*/)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT_VALID(pWnd);
	ASSERT(pWnd != NULL);
	ASSERT(lpszLabel != NULL);
	ASSERT(iIndex >= -1);
	ASSERT(iIndex <= m_pTabCtrl->GetItemCount());

    pWnd->SetParent(m_pTabCtrl);
	
	// Allocate a new XT_TCB_ITEM object.
	XT_TCB_ITEM* pMember = new XT_TCB_ITEM;

	if (pMember == NULL)
		return FALSE;

	pMember->pWnd			= pWnd;
	pMember->szTabLabel		= lpszLabel;
	pMember->szToolTipLabel = lpszLabel;
	pMember->uiToolTipId	= m_pTabCtrl->GetItemCount();

	// If no index was specified add the view tab 
	// to the end.
	if (iIndex == -1) {
		iIndex = m_tcbItems.GetCount();
	}

	// Add the new XT_TCB_ITEM to the tab item list.
	const POSITION pos = m_tcbItems.FindIndex(iIndex);
	if (pos != NULL)
	{
		m_tcbItems.InsertBefore(pos, pMember);
	}
	else
	{
		ASSERT(iIndex == m_pTabCtrl->GetItemCount());
		m_tcbItems.AddTail(pMember);
	}

	// Insert the new tab item.
	TC_ITEM tci;
	tci.mask	= TCIF_TEXT | TCIF_IMAGE;
	tci.pszText = (LPTSTR)lpszLabel;

	if (iIconIndex != -1) {
		tci.iImage = iIconIndex;
	}
	else {
		tci.iImage = iIndex;
	}
	
	m_pTabCtrl->InsertItem(iIndex, &tci);

	// Create the ToolTip for the tab.
	AddToolTip(pMember->uiToolTipId, lpszLabel);

	// Since the tool tip count has changed,
	// re-initialize the tooltips.
	ResetToolTips();

	// hide/show views after insetion
	SetActiveView(iIndex);

	return TRUE;
}

BOOL CXTTabCtrlBaseEx::UpdateTabLabel(int iTab, XT_TCB_ITEM* pMember, LPCTSTR lpszLabel)
{
    // Text length cannot be longer than 255 characters.
    ASSERT(_tcslen(lpszLabel) < 255);

    CString strLabel = lpszLabel;
    if (strLabel.GetLength() > 255) {
        strLabel = strLabel.Left(255);
    }

    // if the string is empty return FALSE.
    else if (strLabel.IsEmpty()) {
        return FALSE;
    }

    // if XT_TCB_ITEM pointer is NULL, return FALSE.
    if (pMember == NULL)
        return FALSE;
    
    TCHAR szText[256];

    TC_ITEM tci;
    tci.mask       = TCIF_TEXT;
    tci.cchTextMax = _countof(szText);
    tci.pszText    = szText;
    
    // get the tab control item and update the text.
    m_pTabCtrl->GetItem(iTab, &tci);
    _tcscpy(szText, strLabel);
    m_pTabCtrl->SetItem(iTab, &tci);
    
    // Get the tool tips for the tab control and update
    // the tip text.
    CToolTipCtrl* pToolTips = GetTips();
    if (pToolTips == NULL)
        return FALSE;
    
    pToolTips->UpdateTipText(strLabel, m_pTabCtrl, iTab);
    
    // update the XT_TCB_ITEM item label and tip text.
    pMember->szTabLabel     = strLabel;
    pMember->szToolTipLabel = strLabel;

    // update other information
    pMember->szCondensedLabel = strLabel;
    
    return TRUE;
}

BOOL CXTTabCtrlBaseEx::SetTabText(int iTab, LPCTSTR lpszLabel)
{
	// Get the item at the specified index.
	POSITION pos = m_tcbItems.FindIndex (iTab);
	if (pos == NULL)
		return FALSE;

	return UpdateTabLabel (iTab, m_tcbItems.GetAt(pos), lpszLabel);
}

BOOL CXTTabCtrlBaseEx::SetTabText(CWnd* pView, LPCTSTR lpszLabel)
{
	int iTab = 0;

	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		// Get the next item in the list.
		XT_TCB_ITEM* pMember = m_tcbItems.GetAt( pos );

		if (pView == pMember->pWnd)
			return UpdateTabLabel (iTab, m_tcbItems.GetAt(pos), lpszLabel);

		++iTab;
	}

	return FALSE;
}

BOOL CXTTabCtrlBaseEx::SetTabText(CRuntimeClass *pViewClass, LPCTSTR lpszLabel)
{
	int iTab = 0;

	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		// Get the next item in the list.
		XT_TCB_ITEM* pMember = m_tcbItems.GetAt( pos );

		if (pMember->pWnd->IsKindOf(pViewClass))
			return UpdateTabLabel (iTab, m_tcbItems.GetAt(pos), lpszLabel);

		++iTab;
	}

	return FALSE;
}

CWnd* CXTTabCtrlBaseEx::NextView()
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(m_tcbItems.GetCount() > 0);

	// get the current selection and increment by 1.
	int iIndex = m_pTabCtrl->GetCurSel();
	iIndex++;

	// if the new index is greater than or equal to 
	// the tab count, set the index to 0.
	if (iIndex > m_pTabCtrl->GetItemCount()-1)
		iIndex = 0;

	// set the active view and return a pointer to
	// the newly activated view.
	SetActiveView(iIndex);
	
	return GetActiveView();
}

CWnd* CXTTabCtrlBaseEx::PrevView()
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(m_tcbItems.GetCount() > 0);

	// get the current selection and increment by 1.
	int iIndex = m_pTabCtrl->GetCurSel();
	iIndex--;

	// if the new index is greater than or equal to 
	// the tab count, set the index to 0.
	if (iIndex < 0)
		iIndex = m_pTabCtrl->GetItemCount()-1;

	// set the active view and return a pointer to
	// the newly activated view.
	SetActiveView(iIndex);
	
	
	return GetActiveView();
}

BOOL CXTTabCtrlBaseEx::PreTranslateMessageImpl(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		bool bShift = (::GetKeyState(VK_SHIFT) < 0);
		bool bCtrl  = (::GetKeyState(VK_CONTROL) < 0);
		
		if (bCtrl && (pMsg->wParam == VK_TAB) || (pMsg->wParam == VK_F6))
		{
			if (bShift)
			{
				PrevView();
				m_pTabCtrl->SetFocus();
			}
			else
			{
				NextView();
				m_pTabCtrl->SetFocus();
			}

			return TRUE;
		}
	}

	return FALSE; // Allow messages to be passed to base class
}

CWnd* CXTTabCtrlBaseEx::GetView(int nView)
{
	ASSERT_VALID(m_pTabCtrl);
	
	if (nView > -1 && nView < m_pTabCtrl->GetItemCount())
	{
		POSITION pos = m_tcbItems.FindIndex(nView);
		if (pos != NULL)
		{
			CWnd* pWnd = m_tcbItems.GetAt(pos)->pWnd;
			ASSERT_VALID(pWnd);

			if (pWnd && ::IsWindow(pWnd->m_hWnd)) {
				return pWnd;
			}
		}
	}

	return NULL;
}

CWnd* CXTTabCtrlBaseEx::GetView(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
	
	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		CWnd* pWnd = m_tcbItems.GetAt(pos)->pWnd;
		ASSERT_VALID(pWnd);

		if (pWnd && ::IsWindow(pWnd->m_hWnd) && pWnd->IsKindOf(pViewClass)) {
			return pWnd;
		}
	}

	return NULL;
}

void CXTTabCtrlBaseEx::ActivateView(CWnd* pNewView)
{
	ASSERT_VALID(pNewView); // must be valid

	// Hide all views.
	int iIndex;
	for (iIndex = 0; iIndex < m_tcbItems.GetCount(); ++iIndex)
	{
		POSITION pos = m_tcbItems.FindIndex(iIndex);
		if (pos != NULL)
		{
			XT_TCB_ITEM* pTCBItem = m_tcbItems.GetAt(pos);
			ASSERT(pTCBItem != NULL);

			if (pTCBItem && pTCBItem->pWnd)
			{
				pTCBItem->pWnd->ShowWindow(SW_HIDE);
				pTCBItem->pWnd->EnableWindow(FALSE);
			}
		}
	}

	////////// ULF 2003-08-07
	if (m_pParentFrame && m_pParentFrame->IsChild(pNewView))
	{
		CView* pActiveView = m_pParentFrame->GetActiveView();
		if(pActiveView && pActiveView->GetParent() == m_pTabCtrl)
		{
			if(pActiveView != pNewView)
			{
				// Make the frame window aware of the active view (for message routing)
				m_pParentFrame->SetActiveView((CView*)pNewView);
			}
		}
	}
	/////////
	
	// Show and enable the window.
	pNewView->ShowWindow(SW_SHOW);
	pNewView->EnableWindow(TRUE);
	pNewView->SetFocus();

	// Resize the window.
	ResizeTabView(pNewView);
}

void CXTTabCtrlBaseEx::SetActiveView(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
	
	// Walk through the tabs
	int iIndex;
	for (iIndex = 0; iIndex < m_tcbItems.GetCount(); ++iIndex)
	{
		POSITION pos = m_tcbItems.FindIndex(iIndex);
		if (pos != NULL)
		{
			XT_TCB_ITEM* pTCBItem = m_tcbItems.GetAt(pos);
			ASSERT(pTCBItem != NULL);
			
			if (pTCBItem && pTCBItem->pWnd->IsKindOf(pViewClass))
			{
				// Select new the tab
				m_pTabCtrl->SetCurSel(iIndex);
				
				// now active the view and give it focus.
				ActivateView(pTCBItem->pWnd);
				break;
			}
		}
	}
}

void CXTTabCtrlBaseEx::SetActiveView(CWnd* pTabView)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pTabView != NULL);

	// Walk through the tabs
	int iIndex;
	for (iIndex = 0; iIndex < m_tcbItems.GetCount(); ++iIndex)
	{
		POSITION pos = m_tcbItems.FindIndex(iIndex);
		if (pos != NULL)
		{
			XT_TCB_ITEM* pTCBItem = m_tcbItems.GetAt(pos);
			ASSERT(pTCBItem != NULL);

			// Does the tab view match the requested view?
			if (pTCBItem && pTCBItem->pWnd == pTabView)
			{
				// Select new the tab
				m_pTabCtrl->SetCurSel(iIndex);

				// now active the view and give it focus.
				ActivateView(pTCBItem->pWnd);
				break;
			}
		}
	}
}

void CXTTabCtrlBaseEx::SetActiveView(int nActiveTab)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(nActiveTab >= 0);
	
	// Find the item at the specified index.
	POSITION pos = m_tcbItems.FindIndex(nActiveTab);
	if (pos != NULL)
	{
		XT_TCB_ITEM* pTCBItem = m_tcbItems.GetAt(pos);
		ASSERT(pTCBItem != NULL);
		
		if (pTCBItem && pTCBItem->pWnd != NULL)
		{
			ASSERT_VALID(pTCBItem->pWnd);

			// Select new the tab
			m_pTabCtrl->SetCurSel(nActiveTab);
			
			// now active the view and give it focus.
			ActivateView(pTCBItem->pWnd);
		}
	}
}

void CXTTabCtrlBaseEx::RemoveListItem(POSITION pos, BOOL bDestroyWnd/*=TRUE*/)
{
	if( pos != NULL )
	{
		// remove the page from internal list
		XT_TCB_ITEM *pMember = m_tcbItems.GetAt(pos);
		ASSERT(pMember);

		// And delete the member window, freeing our stored
		// values relating to it
		if (pMember->pWnd != NULL)
		{
			if (bDestroyWnd)
			{
				pMember->pWnd->DestroyWindow();
				pMember->pWnd = NULL;
			}
			else
			{
				pMember->pWnd->ShowWindow(SW_HIDE);
			}
		}

		// Remove from list and free memory.
		m_tcbItems.RemoveAt(pos);
		SAFE_DELETE(pMember);
	}
}

void CXTTabCtrlBaseEx::DeleteView(int nView, BOOL bDestroyWnd/*=TRUE*/)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(( nView >= 0 ) && (m_pTabCtrl->GetItemCount() > nView));

	// Now find the view we want to delete and remove it
	POSITION pos = m_tcbItems.FindIndex(nView);
	if (pos != NULL)
	{
		CWnd* pView = m_tcbItems.GetAt(pos)->pWnd;

		// Ensure that we get no dangling pointers
		if (m_pParentFrame && m_pParentFrame->GetActiveView() == pView)
		{
			if (m_pLastActiveView && ::IsWindow(m_pLastActiveView->m_hWnd)) {
				m_pParentFrame->SetActiveView(m_pLastActiveView);
			}
			else {
				m_pParentFrame->SetActiveView(NULL, FALSE);
				m_pLastActiveView = NULL;
			}
		}

		// Remove item from list, and free memory.
		RemoveListItem( pos, bDestroyWnd );

		// Remove it from the tab control
  	    m_pTabCtrl->DeleteItem(nView);

		int nCount = m_pTabCtrl->GetItemCount();

		if (nCount > 0)
		{
			// Finally, if we have just deleted the active view, reset the
			// active tab to be the first view in the list
			if (nView == 0) {
				SetActiveView(nView);
			}
			else if (nView >= nCount) {
				SetActiveView(nCount-1);
			}
			else {
				SetActiveView(nView-1);
			}

			// Reset the tooltips for the views we have left...
			ResetToolTips();
		}
	}
}

void CXTTabCtrlBaseEx::DeleteView(CRuntimeClass *pViewClass, BOOL bDestroyWnd/*=TRUE*/)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	int nView = 0;
	POSITION pos;
	for ( pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext( pos ))
	{
		CWnd* pView = m_tcbItems.GetAt(pos)->pWnd;
		ASSERT_VALID(pView);

		if (pView->IsKindOf(pViewClass))
		{
			DeleteView(nView, bDestroyWnd);
			return;
		}
		nView++;
	}
}

void CXTTabCtrlBaseEx::DeleteView(CWnd* pView, BOOL bDestroyWnd/*=TRUE*/)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pView != NULL);
	ASSERT(AfxIsValidAddress(pView, sizeof(CWnd), FALSE));

	int nView = 0;
	POSITION pos;
	for ( pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext( pos ))
	{
		if (m_tcbItems.GetAt(pos)->pWnd == pView)
		{
			DeleteView(nView, bDestroyWnd);
			return;
		}
		nView++;
	}
}

LPCTSTR CXTTabCtrlBaseEx::GetViewName(int nView)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(( nView >= 0 ) && ( m_pTabCtrl->GetItemCount() > nView ));
	
	if( nView != -1 )
	{
		POSITION pos = m_tcbItems.FindIndex(nView);
		if(pos != NULL) {
			return m_tcbItems.GetAt(pos)->szTabLabel;
		}
	}
	
	return NULL;
}

LPCTSTR CXTTabCtrlBaseEx::GetViewName(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(m_pTabCtrl);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
	
	POSITION pos;
	for ( pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext( pos ))
	{
		XT_TCB_ITEM *pMember = m_tcbItems.GetAt( pos );
		ASSERT(pMember);

		if( pMember->pWnd->IsKindOf( pViewClass ))
		{
			return (LPCTSTR)pMember->szTabLabel;
		}
	}
	return NULL;
}

BOOL CXTTabCtrlBaseEx::IsChildView(CWnd* pWnd)
{
	POSITION pos = m_tcbItems.GetHeadPosition();
	while (pos)
	{
		XT_TCB_ITEM *pItem = m_tcbItems.GetNext(pos);
		if (pItem && pItem->pWnd == pWnd) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTTabCtrlBaseEx::RemoveAllTabs(BOOL bDestroyWnd/*=TRUE*/)
{
	if (m_pParentFrame)
	{
		// If the parent frames active view belongs to
		// use set the active view to NULL.
		CView * pActiveView = m_pParentFrame->GetActiveView();
		if (IsChildView(pActiveView))
		{
			if (m_pLastActiveView && ::IsWindow(m_pLastActiveView->m_hWnd)) {
				m_pParentFrame->SetActiveView(m_pLastActiveView);
			}
			else {
				m_pParentFrame->SetActiveView(NULL, FALSE);
				m_pLastActiveView = NULL;
			}
		}
	}

	// Free memory and clear tab view list.
	while(!m_tcbItems.IsEmpty())
	{
		POSITION pos = m_tcbItems.FindIndex(0);
		if (pos != NULL) {
			RemoveListItem(pos, bDestroyWnd);
		}
	}

	// Remove all tabs from control.
	m_pTabCtrl->DeleteAllItems();

	return TRUE;
}

int CXTTabCtrlBaseEx::GetTabFromPoint(CPoint point)
{
	TCHITTESTINFO tch;
	memset(&tch, 0, sizeof(TCHITTESTINFO));

	if (point == CPoint(0,0))
	{
		CPoint pt;
		GetCursorPos(&pt);
		m_pTabCtrl->ScreenToClient(&pt);
		tch.pt = pt;
	}
	else {
		tch.pt = point;
	}
	
	return m_pTabCtrl->HitTest(&tch);
}

int CXTTabCtrlBaseEx::OnCreateImpl_Post(LPCREATESTRUCT lpCreateStruct) 
{
	UNUSED_ALWAYS(lpCreateStruct);
	// Initialize the tab control.
	m_pTabCtrl->PostMessage(XTWM_INITIAL_UPDATE, 0, ++m_dwInitSignature);
	m_bInitialUpdatePending = true;

	return 0;
}

void CXTTabCtrlBaseEx::PreSubclassWindowImpl_Post() 
{
	// Initialize the tab control.
	m_pTabCtrl->PostMessage(XTWM_INITIAL_UPDATE, 0, ++m_dwInitSignature);
	m_bInitialUpdatePending = true;
}

void CXTTabCtrlBaseEx::ResizeTabView(CWnd *pView)
{
	if (pView)
	{
		CRect rcChild;
		GetChildRect(rcChild);
		pView->MoveWindow(rcChild);
	}
}

void CXTTabCtrlBaseEx::InitializeFont()
{
	DWORD dwStyle = m_pTabCtrl->GetStyle();

	BOOL bVertical     = ((dwStyle & TCS_VERTICAL) == TCS_VERTICAL);
	BOOL bRightAligned = ((dwStyle & TCS_RIGHT   ) == TCS_RIGHT);

	// Set the font based on the tab controls orientation.
	HGDIOBJ hFont = (bVertical ? (bRightAligned ? xtAfxData.fontVCaptR.GetSafeHandle() :
		xtAfxData.fontVCapt.GetSafeHandle()) : xtAfxData.fontHCapt.GetSafeHandle());

	if (xtAfxData.bWin95) {
		m_pTabCtrl->PostMessage(WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	}
	else {
		m_pTabCtrl->SendMessage(WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
	}
}

BOOL CXTTabCtrlBaseEx::OnCmdMsgImpl_Pre(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	CWnd* pView = GetActiveView();
	if(pView)
	{
		if (pView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;
	}
	return FALSE;
}

LRESULT CXTTabCtrlBaseEx::OnInitializeImpl(WPARAM, LPARAM lp)
{
	if (DWORD(lp) == m_dwInitSignature)
	{
		if (m_bInitialUpdate)
		{
			// fire off initial updates
			// NB: this works only if WM_INITIALUPDATE has not been sent to us
			//     since its handler invalidates the signature
			m_pTabCtrl->SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);
		}

		// execute our own initialization sequence
		OnInitialUpdateImpl();
	}
	return 0;
}

void CXTTabCtrlBaseEx::OnInitialUpdateImpl()
{
	// invalidate pending initialization requests
	m_dwInitSignature++;
	m_bInitialUpdatePending = false;


	ASSERT(m_pTabCtrl != NULL);  // make sure ImplAttach was called

	// Set the font for the tab control.
	InitializeFont();

	// Get the pointer to the parent window.
	m_pParentWnd = m_pTabCtrl->GetParent();
	ASSERT_VALID(m_pParentWnd);

	// Get the pointer to the parent frame window.
	m_pParentFrame = m_pTabCtrl->GetParentFrame();
	if (m_pParentFrame)
	{
		ASSERT_VALID(m_pParentFrame);

		// If floating, get parent frame of the floating frame window.
		if (m_pParentFrame->IsKindOf(RUNTIME_CLASS(CMiniDockFrameWnd))) {
			m_pParentFrame = m_pParentFrame->GetParentFrame();
		}
	}

	// Resize all views.
	POSITION pos;
	for (pos = m_tcbItems.GetHeadPosition(); pos; m_tcbItems.GetNext(pos))
	{
		XT_TCB_ITEM* pTCBItem = m_tcbItems.GetAt(pos);
		ASSERT(pTCBItem != NULL);

		CWnd* pWnd = pTCBItem->pWnd;
		ASSERT_VALID(pWnd);

		ResizeTabView(pWnd);
	}

	if (m_bAutoCondensing)
	{
		Condense();
	}

	// make sure children are clipped.
	m_pTabCtrl->ModifyStyle(NULL, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);
}

void CXTTabCtrlBaseEx::OnRButtonDownImpl(UINT /*nFlags*/, CPoint point) 
{
	// No menu was defined...
	if (!m_popupMenuID) {
//		TRACE0( "Warning: No menu resource was associated with CXTTabCtrl.\n" ); 
		return;
	}

	CPoint pt = point;
	m_pTabCtrl->ClientToScreen(&pt);

	CMenu menu;
	VERIFY(menu.LoadMenu(m_popupMenuID));

	CMenu* pPopup = menu.GetSubMenu(m_nPos);
	ASSERT(pPopup != NULL);
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y,
		m_pParentFrame);
}


void CXTTabCtrlBaseEx::SetAutoCondense(BOOL bEnable)
{
	m_bAutoCondensing = bEnable;
	Condense();
}

BOOL CXTTabCtrlBaseEx::GetAutoCondense()
{
	return m_bAutoCondensing; 
}

int CXTTabCtrlBaseEx::CalculateTabWidth(CDC *pDC, CString& sLabel, bool bHasIcon)
{
	int len = pDC->GetTextExtent(sLabel).cx + 6 + 6;
	if (sLabel.GetLength() == 1) ++len;
	if (bHasIcon)
	{
		len += 16 + 6;
	}
	return len;
}

// used by tab condensation code
struct XT_ITEMANDWIDTH
{
	XT_TCB_ITEM *pItem;
	int          iWidth;
	bool         bHasIcon;
	bool         bAtMinLength;
	int          iItemIndex;
	CXTString    sNewLabel;
};

void CXTTabCtrlBaseEx::Condense()
{
	if (!m_pTabCtrl->m_hWnd  ||  m_pTabCtrl->GetItemCount() == 0)
	{
		return;
	}

	// create a device context with the tab control's font (used for label
	// widths calculations)
	CFont *pFont = m_pTabCtrl->GetFont();
	VERIFY(pFont != NULL);
	CDC *pDC = m_pTabCtrl->GetDC();
	CFont *pOldFont = pDC->SelectObject(pFont);

	int iTotalWidth = 0;
	CList <XT_ITEMANDWIDTH, XT_ITEMANDWIDTH&> lstItemsByWidth;

	// the formula for iMinTabWidth is based on empirical data
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	const int iMinTabWidth = tm.tmAveCharWidth * 6 + 12;

	// calculate length of tab labels and fill our tracking list
	POSITION p = m_tcbItems.GetHeadPosition();
	ASSERT(p != NULL);  // there should be items
	int idx = 0;
	TCITEM tci;
	tci.mask = TCIF_IMAGE;
	while (p)
	{
		m_pTabCtrl->GetItem(idx, &tci);
		XT_ITEMANDWIDTH iaw;
		iaw.bHasIcon = (tci.iImage >= 0);
		iaw.pItem = m_tcbItems.GetNext(p);
		iaw.iWidth = CalculateTabWidth(pDC, iaw.pItem->szTabLabel, iaw.bHasIcon);
		iaw.sNewLabel = iaw.pItem->szTabLabel;
		iaw.iItemIndex = idx;
		iaw.bAtMinLength = FALSE;
		++idx;
		lstItemsByWidth.AddTail(iaw);
		iTotalWidth += iaw.iWidth;
	}

	const bool bVertical = (m_pTabCtrl->GetStyle() & TCS_VERTICAL) == TCS_VERTICAL;
	const bool bMultiLine = 
		bVertical || ((m_pTabCtrl->GetStyle() & TCS_MULTILINE) == TCS_MULTILINE);

	CRect rc;
	m_pTabCtrl->GetClientRect(&rc);
	const int client_width = bVertical? rc.Height() : rc.Width();
	POSITION p2;

	if (client_width == 0)
	{
		pDC->SelectObject(pOldFont);    
		m_pTabCtrl->ReleaseDC(pDC);
		return;
	}

	if (m_bAutoCondensing)
	{
		iTotalWidth += 4; // 4 pixels extra for the selected tab
		if (bMultiLine) ++iTotalWidth;

		// start condensing labels until the total width fits the client area,
		// or until we can't condense any more
		while (iTotalWidth > client_width)
		{
			// look for widest item
			p = lstItemsByWidth.GetHeadPosition();
			int iWidest = -1;
			while (p)
			{
				const POSITION p3 = p;
				XT_ITEMANDWIDTH& iaw = lstItemsByWidth.GetNext(p);
				if (iaw.iWidth > iWidest  &&  !iaw.bAtMinLength)
				{
					iWidest = iaw.iWidth;
					p2 = p3;
				}
			}
			if (iWidest == -1)
			{
				break;
			}
			XT_ITEMANDWIDTH& iaw = lstItemsByWidth.GetNext(p2);
			const int iOldWidth = iaw.iWidth;
			CXTString& label = iaw.sNewLabel;
			
			// shorten name
			if (label.Right(3) != _T("..."))
			{
				label += _T("...");
			}
			do
			{
				if (label.GetLength() == 3	||	(iaw.iWidth <= iMinTabWidth))
				{
					iaw.bAtMinLength = true;
				}
				else
				{
					int iLen = label.GetLength( );
                    if ( ( iLen > 0 ) && ( ( iLen-4 ) >= 0 ) ) {
                        label.Delete( ( iLen-4 ), 1 );
                    }
				}
				iaw.iWidth = CalculateTabWidth(pDC, label, iaw.bHasIcon);
			} while (!iaw.bAtMinLength  &&  iaw.iWidth >= iOldWidth);
			iTotalWidth -= (iOldWidth - iaw.iWidth);
		}
	}

	// update actual tab labels based on new condensed labels
	tci.mask = TCIF_TEXT;
	p = lstItemsByWidth.GetHeadPosition();
	while (p)
	{
		XT_ITEMANDWIDTH& iaw = lstItemsByWidth.GetNext(p);
		// only change if actually different
		if (iaw.pItem->szCondensedLabel != iaw.sNewLabel)
		{
			iaw.pItem->szCondensedLabel = iaw.sNewLabel;
			tci.pszText = (TCHAR*)(LPCTSTR)iaw.sNewLabel;
			m_pTabCtrl->SetItem(iaw.iItemIndex, &tci);
		}
	}

	pDC->SelectObject(pOldFont);    
	m_pTabCtrl->ReleaseDC(pDC);
}


/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrl

IMPLEMENT_DYNAMIC(CXTTabCtrl, CTabCtrl)

BEGIN_MESSAGE_MAP(CXTTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CXTTabCtrl)
	//}}AFX_MSG_MAP
	//:Ignore
	MESSAGE_MAP_ENTRIES_CXTTabCtrlBaseEx
	//:End Ignore
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrl construction/destruction

CXTTabCtrl::CXTTabCtrl()
{
	ImplAttach(this);
}

CXTTabCtrl::~CXTTabCtrl()
{

}


/////////////////////////////////////////////////////////////////////////////
// CXTTabCtrl message handlers

void CXTTabCtrl::OnDestroy() 
{
	OnDestroyImpl_Pre();
	CTabCtrl::OnDestroy();
}

void CXTTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnSelchangeImpl(pNMHDR, pResult);
}

void CXTTabCtrl::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnSelchangingImpl(pNMHDR, pResult);
}

void CXTTabCtrl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	OnWindowPosChangedImpl_Pre(lpwndpos);
	CTabCtrl::OnWindowPosChanged(lpwndpos);
	OnWindowPosChangedImpl_Post(lpwndpos);
}

BOOL CXTTabCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (PreTranslateMessageImpl(pMsg))
		return TRUE;

	return CTabCtrl::PreTranslateMessage(pMsg);
}

int CXTTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return OnCreateImpl_Post(lpCreateStruct);
}

void CXTTabCtrl::PreSubclassWindow() 
{
	CTabCtrl::PreSubclassWindow();
	PreSubclassWindowImpl_Post();
}

void CXTTabCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	OnRButtonDownImpl(nFlags, point);
}

BOOL CXTTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return OnEraseBkgndImpl(pDC);
}

BOOL CXTTabCtrl::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if (OnCmdMsgImpl_Pre(nID, nCode, pExtra, pHandlerInfo))
	{
		return TRUE;
	}
	return CTabCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CXTTabCtrl::OnPaint() 
{
	OnPaintImpl();
}

void CXTTabCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CTabCtrl::OnSettingChange(uFlags, lpszSection);
	OnSettingChangeImpl_Post(uFlags, lpszSection);
}

void CXTTabCtrl::OnSysColorChange() 
{
	CTabCtrl::OnSysColorChange();
	OnSysColorChangeImpl_Post();
}

BOOL CXTTabCtrl::EnableToolTips(BOOL bEnable)
{
	return EnableToolTipsImpl(bEnable);
}
