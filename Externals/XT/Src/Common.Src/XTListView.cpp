// XTListView.cpp : implementation of the CXTListView class.
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
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrl.h"
#include "XTListView.h"
#include "XTMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTListView
/////////////////////////////////////////////////////////////////////////////
CXTListView::CXTListView()
{
	ImplAttach(&GetListCtrl());
}

CXTListView::~CXTListView()
{

}

IMPLEMENT_DYNCREATE(CXTListView, CListView)

BEGIN_MESSAGE_MAP(CXTListView, CListView)
	//{{AFX_MSG_MAP(CXTListView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTListView::OnDraw( CDC* pDC )
{
	UNUSED_ALWAYS( pDC );
}

#ifdef _DEBUG
void CXTListView::AssertValid() const
{
	CListView::AssertValid();
}

void CXTListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

void CXTListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
}

void CXTListView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnCustomDrawImpl(pNMHDR, pResult);
}

BOOL CXTListView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return OnNotifyImpl(wParam, lParam, pResult);
}

void CXTListView::OnDestroy() 
{
	OnDestroyImpl_Pre();
	CListView::OnDestroy();
}

BOOL CXTListView::OnEraseBkgnd(CDC* pDC) 
{
	return OnEraseBkgndImpl(pDC);
}

void CXTListView::OnPaint() 
{
	OnPaintImpl();
}

int CXTListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return OnCreateImpl_Post(lpCreateStruct);
}

void CXTListView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CListView::OnSettingChange(uFlags, lpszSection);
	OnSettingChangeImpl_Post(uFlags, lpszSection);
}
