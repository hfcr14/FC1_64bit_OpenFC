// XTResizePropertyPage.cpp: implementation of the CXTResizePropertyPage class.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//--------------------------------------------------------------------
// Permission granted, based on the resizable classes created by Torben B. Haagh.
// http://www.codeguru.com/dialog/torbenResizeDialog.shtml
//--------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizePropertyPage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CXTResizePropertyPage

IMPLEMENT_DYNCREATE(CXTResizePropertyPage, CPropertyPage)

BEGIN_MESSAGE_MAP(CXTResizePropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CXTResizePropertyPage)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTResizePropertyPage construction/destruction

CXTResizePropertyPage::CXTResizePropertyPage(const UINT nTemplate, const UINT nCaption, const UINT nFlags) : 
	CPropertyPage(nTemplate, nCaption),
	CXTResize(this, nFlags | SZ_NOSIZEICON)
{
	m_nDialogID = nTemplate;
}

BOOL CXTResizePropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CXTResize::Init();

	EnableToolTips();

	return TRUE;
}

void CXTResizePropertyPage::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTResize::GetMinMaxInfo(pMMI);
}

void CXTResizePropertyPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
	CXTResize::Size();
}

void CXTResizePropertyPage::OnDestroy() 
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty()) {
		SavePlacement(m_strSection);
	}

	CPropertyPage::OnDestroy();
}
