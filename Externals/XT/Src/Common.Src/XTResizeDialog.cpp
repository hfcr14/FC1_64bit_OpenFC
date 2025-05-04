// XTResizeDialog.cpp: implementation of the CXTResizeDialog class.
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
#include "XTResizeDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CXTResizeDialog

IMPLEMENT_DYNCREATE(CXTResizeDialog, CDialog)

BEGIN_MESSAGE_MAP(CXTResizeDialog, CDialog)
	//{{AFX_MSG_MAP(CXTResizeDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTResizeDialog construction/destruction

CXTResizeDialog::CXTResizeDialog()
	: CXTResize(NULL, 0)
{
}

CXTResizeDialog::CXTResizeDialog(const UINT nID, CWnd* pParent, const UINT nFlags) : 
	CDialog(nID, pParent),
	CXTResize(this, nFlags)
{
	m_nDialogID = nID;
}

BOOL CXTResizeDialog::OnInitDialog() 
{
	// Only show the resize grip if the frame is a resizing frame
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwStyle & WS_THICKFRAME) == 0) {
		SetFlag(SZ_NOSIZEICON);
	}

	CDialog::OnInitDialog();
	CXTResize::Init();

	// Enable tool tips for this dialog
	EnableToolTips();

	return TRUE;
}

void CXTResizeDialog::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CXTResize::GetMinMaxInfo(pMMI);
}

void CXTResizeDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CXTResize::Size();
}

void CXTResizeDialog::OnDestroy() 
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty()) {
		SavePlacement(m_strSection);
	}

	CDialog::OnDestroy();
}
