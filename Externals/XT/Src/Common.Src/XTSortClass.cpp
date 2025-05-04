// XTSortClass.cpp : implementation of the CXTSortClass class.
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
#include "XTSortClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct XT_SORT_ITEM
{
	XT_SORT_ITEM(const DWORD dwData, const CString &strItemText) {
		m_dwData = dwData;
		m_strItemText = strItemText;
	}
	DWORD	m_dwData;
	CString m_strItemText;
};

//////////////////////////////////////////////////////////////////////
// CXTSortClass
//
CXTSortClass::CXTSortClass(CListCtrl* pListCtrl, const int nCol)
{
	ASSERT(pListCtrl);
	m_pListCtrl = pListCtrl;
	int nCount = m_pListCtrl->GetItemCount();
	
	// Replace Item data with pointer to XT_SORT_ITEM structure
	int i;
	for (i = 0; i < nCount; i++)
	{
		DWORD dwData = m_pListCtrl->GetItemData(i); // save current data to restore it later
		CString strItemText = m_pListCtrl->GetItemText(i, nCol); 
		m_pListCtrl->SetItemData(i, (DWORD) new XT_SORT_ITEM(dwData, strItemText));
	}
}

CXTSortClass::~CXTSortClass()
{
	ASSERT(m_pListCtrl);
	int nCount = m_pListCtrl->GetItemCount();

	int i;
	for (i = 0; i < nCount; i++)
	{
		XT_SORT_ITEM* pItem = (XT_SORT_ITEM*)m_pListCtrl->GetItemData(i);
		ASSERT(pItem);
		m_pListCtrl->SetItemData(i, pItem->m_dwData);
		delete pItem;
	}
}

void CXTSortClass::Sort(bool bAsc, XT_DATA_TYPE eType)
{
	long lParamSort = eType;
	
	// if lParamSort positive - ascending sort order, negative - descending
	if (!bAsc) {
		lParamSort *= -1; 
	}
	
	m_pListCtrl->SortItems(Compare, lParamSort);
}

int CALLBACK CXTSortClass::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	XT_SORT_ITEM* item1 = (XT_SORT_ITEM*)lParam1;
	XT_SORT_ITEM* item2 = (XT_SORT_ITEM*)lParam2;

	ASSERT(item1 && item2);

	// Restore data type and sort order from lParamSort
	// if lParamSort positive - ascending sort order, negative - descending
	short sOrder = lParamSort < 0 ? (short) -1 : (short) 1; 
	
	XT_DATA_TYPE eType  = (XT_DATA_TYPE) (lParamSort * sOrder); // get rid of sign
	
	// declare typed buffers
	COleDateTime t1, t2;
	
	switch (eType)
	{
	case DT_INT:
		return ( _ttol( item1->m_strItemText ) - _ttol( item2->m_strItemText )) * sOrder;

	case DT_DEC:
#ifdef _UNICODE
		{
			char szText1[ 256 ];		
			char szText2[ 256 ];		
		
			::WideCharToMultiByte( CP_ACP, 0, item1->m_strItemText, -1, szText1,
				255, NULL, NULL );
			
			::WideCharToMultiByte( CP_ACP, 0, item2->m_strItemText, -1, szText2,
				255, NULL, NULL );

			return ( atof( szText1 ) < atof( szText2 ) ? -1 : 1) * sOrder;
		}
#else
		return ( atof ( item1->m_strItemText ) < atof( item2->m_strItemText ) ? -1 : 1) * sOrder;
#endif
		
	case DT_DATETIME:
		if( t1.ParseDateTime( item1->m_strItemText ) && t2.ParseDateTime( item2->m_strItemText )) {
			return (t1 < t2 ? -1 : 1 ) * sOrder;
		}
		return 0;

	case DT_STRING:
		return item1->m_strItemText.CompareNoCase( item2->m_strItemText ) * sOrder;
		
	default:
		ASSERT("Error: attempt to sort a column without type.");
		return 0;
	}
}

