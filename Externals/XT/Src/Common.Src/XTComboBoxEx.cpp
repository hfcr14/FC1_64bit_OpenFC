// XTComboBoxEx.cpp : implementation of the CXTComboBoxEx class.
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
#include "XTComboBoxEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTComboBoxEx
/////////////////////////////////////////////////////////////////////////////
BOOL CXTComboBoxEx::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd,
	UINT nID)
{
  // initialize common controls
#if _MSC_VER < 1200 // MFC 5.0
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_USEREX_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));
#else
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTL_USEREX_REG));
#endif // _MSC_VER < 1200

	CWnd* pWnd = this;
	return pWnd->Create(WC_COMBOBOXEX, NULL, dwStyle, rect, pParentWnd, nID);
}

int CXTComboBoxEx::DeleteItem(int iIndex)
{
	ASSERT(::IsWindow(m_hWnd));
	return (int) ::SendMessage(m_hWnd, CBEM_DELETEITEM, (WPARAM) iIndex, 0);
}

BOOL CXTComboBoxEx::GetItem(COMBOBOXEXITEM* pCBItem)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pCBItem != NULL);
	ASSERT(AfxIsValidAddress(pCBItem, sizeof(COMBOBOXEXITEM)));
	
	return (int) ::SendMessage(m_hWnd, CBEM_GETITEM, 0, (LPARAM) pCBItem);
}

int CXTComboBoxEx::InsertItem(const COMBOBOXEXITEM* pCBItem)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pCBItem != NULL);
	ASSERT(AfxIsValidAddress(pCBItem, sizeof(COMBOBOXEXITEM), FALSE));
	
	return (int) ::SendMessage(m_hWnd, CBEM_INSERTITEM, 0, (LPARAM) pCBItem);
}

int CXTComboBoxEx::InsertItem(int iItem, UINT nStringID, int iIndent, int iImage, int iSelectedImage, UINT mask)
{
	CString strItem;
	strItem.LoadString(nStringID);
	return InsertItem(iItem, strItem, iIndent, iImage, iSelectedImage, mask);
}

int CXTComboBoxEx::InsertItem(int iItem, LPCTSTR lpszItem, int iIndent, int iImage, int iSelectedImage, UINT mask)
{
	COMBOBOXEXITEM cbItem;

	cbItem.mask				= mask;
	cbItem.iItem			= iItem;
	cbItem.pszText			= const_cast<LPTSTR>(lpszItem);
	cbItem.iImage			= iImage;
	cbItem.iSelectedImage	= iSelectedImage;
	cbItem.iIndent			= iIndent;
	
	return InsertItem(&cbItem);
}

BOOL CXTComboBoxEx::SetItem(const COMBOBOXEXITEM* pCBItem)
{
	ASSERT(::IsWindow(m_hWnd));
	ASSERT(pCBItem != NULL);
	ASSERT(AfxIsValidAddress(pCBItem, sizeof(COMBOBOXEXITEM), FALSE));
	
	return (int) ::SendMessage(m_hWnd, CBEM_SETITEM, 0, (LPARAM) pCBItem);
}

CXTComboBoxEx::~CXTComboBoxEx()
{
	DestroyWindow();
}

IMPLEMENT_DYNAMIC(CXTComboBoxEx, CComboBox)

