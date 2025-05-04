// XTListCtrl.cpp : implementation of the CXTListCtrl class.
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
#include "XTMemDC.h"
#include "XTPriv.h"
#include "XTRegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CXTListCtrlBase::CXTListCtrlBase()
{
	m_bAutoFont     = TRUE;
	m_nMinColWidth	= 0;
	m_nMaxColWidth	= ::GetSystemMetrics(SM_CXFULLSCREEN);
	m_nSortedCol	= -1;
	m_bAscending	= true;
	m_bAutoSave		= false;	
	m_bNoColSizing	= false;
	m_clrText		= ::GetSysColor(COLOR_WINDOWTEXT);
	m_clrTextBk 	= ::GetSysColor(COLOR_WINDOW);
}

CXTListCtrlBase::~CXTListCtrlBase()
{
}

void CXTListCtrlBase::ImplAttach(CListCtrl *pListCtrl)
{
	m_pListCtrl = (CListCtrl_Friendly *)pListCtrl;
}

bool CXTListCtrlBase::MoveRow(int nFrom, int nTo)
{
	// Can't move to the same place, or from or to a negative index
	if ( ( nFrom == nTo ) || ( nFrom < 0 ) || ( nTo < 0 ) )
	{
		return false;
	}
	
	// First Copy the row to the new location
	int iRemove = CopyRow( nFrom, nTo );
	if ( iRemove != -1 )
	{
		return ( m_pListCtrl->DeleteItem( iRemove ) == TRUE );
	}

	return false;
}

int CXTListCtrlBase::CopyRow(int nFrom, int nTo)
{
	// Can't move to the same place, or from or to a negative index
	if ( ( nFrom == nTo ) || ( nFrom < 0 ) || ( nTo < 0 ) )
	{
		return -1;
	}

	//Copy the row to the new index
	TCHAR szLabel[ 256 ];
	
	LV_ITEM lvi;
	::ZeroMemory( &lvi, sizeof( LV_ITEM ) );
	
	lvi.mask       = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.pszText    = szLabel;
	lvi.iItem      = nFrom;
	lvi.cchTextMax = _countof( szLabel );
	m_pListCtrl->GetItem( &lvi );
	
	lvi.iItem = nTo;
	m_pListCtrl->InsertItem( &lvi );

	// If row has been inserted before original
	// increment the original
	if ( nFrom > nTo )
	{
		nFrom++;
	}

	//Loop through subitems
	int i;
	for ( i = 1; i < GetColumnCount( ); i++ )
	{
		CString strLabel =  m_pListCtrl->GetItemText( nFrom, i );
		if ( !strLabel.IsEmpty( ) )
		{
			m_pListCtrl->SetItemText(
				nTo, i, strLabel );
		}
	}

	return nFrom;
}

int CXTListCtrlBase::GetColumnCount()
{
	// if we are not in report mode return error.
	if( (GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return -1;

	// get a pointer to the header control, if NULL return error.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return -1;

	return pHeaderCtrl->GetItemCount();
}

void CXTListCtrlBase::OnCustomDrawImpl(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	*pResult = 0;
	
	switch (lplvcd->nmcd.dwDrawStage)
	{
	// Request prepaint notifications for each item.
	case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
		}
		break;

	// Requested notification
	case CDDS_ITEMPREPAINT:
		{
			int iItem = lplvcd->nmcd.dwItemSpec;

			if (iItem >= 0)
			{
				// Only change the background colour if the window is enabled, and
				// the item is not selected, otherwise use the system default.

				if (m_pListCtrl->IsWindowEnabled())
				{
					if (m_pListCtrl->GetItemState(iItem, LVIS_SELECTED) == 0)
					{
						lplvcd->clrText   = m_clrText;
						lplvcd->clrTextBk = m_clrTextBk;
					}
				}
			}

			*pResult = CDRF_DODEFAULT;
		}
		break;
	}
}

BOOL CXTListCtrlBase::OnNotifyImpl(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *pHDNotify = (HD_NOTIFY*)lParam;

	switch( pHDNotify->hdr.code )
	{
	case HDN_ITEMCLICKA:
	case HDN_ITEMCLICKW:
		{
			CXTFlatHeaderCtrl* pHeaderCtrl = GetFlatHeaderCtrl();
			if (pHeaderCtrl)
			{
				// left mouse button.
				if (pHDNotify->iButton == 0)
				{
					if (pHDNotify->iItem == m_nSortedCol) {
						m_bAscending = !m_bAscending;
					}
					else {
						m_bAscending = true;
					}
				}
				
				// right mouse button.
				else if (pHDNotify->iButton == 1) 	{
					m_bAscending = pHeaderCtrl->GetAscending() ? true : false;
				}
				
				else {
					break;
				}
				
				// set sort image for header.
				m_nSortedCol = pHDNotify->iItem;
				pHeaderCtrl->SetSortImage (m_nSortedCol, m_bAscending);
				
				// sort list.
				SortList (m_nSortedCol, m_bAscending);
			}
		}
		break;

	case HDN_ITEMCHANGINGA:
	case HDN_ITEMCHANGINGW:
	case HDN_ENDTRACK:
		{
			if (pHDNotify->pitem->mask & HDI_WIDTH)
			{
				if (pHDNotify->pitem->cxy < m_nMinColWidth)
					pHDNotify->pitem->cxy = m_nMinColWidth;
			}
		}
		break;

	case HDN_DIVIDERDBLCLICKA:
	case HDN_DIVIDERDBLCLICKW:
	case HDN_BEGINTRACKW:
	case HDN_BEGINTRACKA:
		{
			if (!m_bNoColSizing)
				break;

			*pResult = TRUE;
			return TRUE;
		}
	}

	return m_pListCtrl->CListCtrl::OnNotify(wParam, lParam, pResult);
}

bool CXTListCtrlBase::SortList(int nCol, bool bAscending)
{
#ifdef _DEBUG
	TRACE2("Column sort on column index %d, ascending=%d\n", nCol, bAscending);
#else
	UNUSED_ALWAYS( nCol );
	UNUSED_ALWAYS( bAscending );
#endif

	return true;
}

int CXTListCtrlBase::AddColumn(LPCTSTR lpszColHeading, int nWidth/*=-1*/, int nFormat/*=LVCFMT_LEFT*/)
{
	// if we are not in report mode return error.
	if( (GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return -1;

	// get a pointer to the header control, if NULL return error.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return -1;

	int iIndex = pHeaderCtrl->GetItemCount();
	if (nWidth == -1)
	{
		// Get the column width of the previous column from header control
		HD_ITEM hd_item;
		hd_item.mask = HDI_WIDTH;               //indicate that we want the width
		pHeaderCtrl->GetItem( iIndex - 1, &hd_item );
		nWidth = hd_item.cxy;
	}
	
	return m_pListCtrl->InsertColumn(iIndex, lpszColHeading, nFormat, nWidth, iIndex);
}

bool CXTListCtrlBase::BuildColumns(int nCols, int* nWidth, int* nColString)
{
	int i;
	for ( i = 0; i < nCols; ++i )
	{
		CString	strColumn;
		strColumn.LoadString (nColString[i]);

		if ( m_pListCtrl->InsertColumn(i, strColumn, LVCFMT_LEFT, nWidth[i], i) == -1 )
		{
			return false;
		}
	}

	return true;
}

bool CXTListCtrlBase::BuildColumns(int nCols, int* nWidth, CString* strColString)
{
	int i;
	for (i = 0; i < nCols; ++i)
	{
		if ( m_pListCtrl->InsertColumn(i, strColString[i], LVCFMT_LEFT, nWidth[i], i) == -1 )
		{
			return false;
		}
	}

	return true;
}

int CXTListCtrlBase::HitTestEx(CPoint& point, int* nCol) const
{
    int colnum = 0;
    int row = m_pListCtrl->HitTest( point, NULL );
    
    if( nCol ) *nCol = 0;
    
    // Make sure that the ListView is in LVS_REPORT
    if( (GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
        return row;
    
    // Get the top and bottom row visible
    row = m_pListCtrl->GetTopIndex();
    int bottom = row + m_pListCtrl->GetCountPerPage();
    if( bottom > m_pListCtrl->GetItemCount() )
        bottom = m_pListCtrl->GetItemCount();
    
    // get a pointer to the header control, if NULL return error.
    CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
    if (pHeaderCtrl == NULL)
        return -1;
    
    // Get the number of columns
    int nColumnCount = pHeaderCtrl->GetItemCount();
    
    // Loop through the visible rows
    for ( ;row <= bottom;row++)
    {
        // Get bounding rect of item and check whether point falls in it.
        CRect rect;
        m_pListCtrl->GetItemRect( row, &rect, LVIR_BOUNDS );
        if( rect.PtInRect(point) )
        {
            // Now find the column
            for ( colnum = 0; colnum < nColumnCount; colnum++ )
            {
                int colwidth = m_pListCtrl->GetColumnWidth(Header_OrderToIndex(pHeaderCtrl->m_hWnd, colnum)); 
                if( point.x >= rect.left
                    && point.x <= (rect.left + colwidth ) )
                {
                    if( nCol ) *nCol = colnum;
                    return row;
                }
                rect.left += colwidth;
            }
        }
    }
    return -1;
}

bool CXTListCtrlBase::SubclassHeader(bool bBoldFont/*=false*/)
{
	// if we are not in report mode return false.
	if( (GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return false;

	// header was already subclassed!
	if (::IsWindow(m_flatHeader.GetSafeHwnd()))
		return false;

	// Get the windows handle to the header control for the
	// list control then subclass the control.
	HWND hWndHeader = _xtGetHeaderCtrl()->GetSafeHwnd();
	if (!m_flatHeader.SubclassWindow (hWndHeader))
		return false;

	// finish header initialization.
	m_flatHeader.InitializeHeader(bBoldFont);
	return true;
}

void CXTListCtrlBase::AutoSaveColumns(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	m_bAutoSave = true;

	// initialize registry strings.
	if( lpszSection == NULL )
		m_strSection.LoadString(XT_IDS_REG_SETTINGS);
	else
		m_strSection = lpszSection;

	if( lpszEntry == NULL )
		m_strEntry.LoadString(XT_IDS_REG_COLINFO);
	else
		m_strEntry = lpszEntry;

	if( lpszDefault == NULL )
		m_strDefault = _T("");
	else
		m_strDefault = lpszDefault;

	LoadColumnWidths();
}

void CXTListCtrlBase::SaveColumnWidths()
{
	// if we are not in report mode return.
	if( (GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return;

	// get a pointer to the header control.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return;

	CString strValue;
	int i;
	for ( i = 0; i < pHeaderCtrl->GetItemCount(); ++i )
	{
		CString strTemp;
		strTemp.Format(_T("%d,"), m_pListCtrl->GetColumnWidth( i ));
		strValue += strTemp;
	}

	CXTRegistryManager regManager;
	regManager.WriteProfileString( m_strSection, m_strEntry, strValue );
}

void CXTListCtrlBase::LoadColumnWidths()
{
	// if we are not in report mode return.
	if( (GetWindowLong(m_pListCtrl->m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return;

	// get a pointer to the header control.
	CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
	if (pHeaderCtrl == NULL)
		return;

	int i;
	for ( i = 0; i < pHeaderCtrl->GetItemCount(); ++i )
		SetStoredWidth(i);
}

void CXTListCtrlBase::SetStoredWidth(int nCol)
{
	int nWidth = GetStoredWidth( nCol );
	m_pListCtrl->SetColumnWidth( nCol, nWidth );
}

int CXTListCtrlBase::GetStoredWidth(int nCol)
{
	// get the value from the registry.
	CXTRegistryManager regManager;
	CString strValue = regManager.GetProfileString(
		m_strSection, m_strEntry, m_strDefault );

	// extract the sub string to get the column width.
	CString strSubString;
	AfxExtractSubString( strSubString, strValue, nCol, _T(','));

	// return the width from the registry.
	return _ttoi( strSubString );
}

void CXTListCtrlBase::OnDestroyImpl_Pre() 
{
	if( m_bAutoSave ) SaveColumnWidths();
}

void CXTListCtrlBase::AutoSizeColumn(int nCol/*=-1*/)
{
	// Call this after your list control is filled
	m_pListCtrl->SetRedraw (FALSE);
	
	int nMinCol = nCol < 0 ? 0 : nCol;
	int nMaxCol = nCol < 0 ? GetColumnCount()-1 : nCol;
	
	for (nCol = nMinCol; nCol <= nMaxCol; nCol++)
	{
		m_pListCtrl->SetColumnWidth( nCol, LVSCW_AUTOSIZE );
		int wc1 = m_pListCtrl->GetColumnWidth( nCol );
		int wc2 = 0;

		// LVSCW_AUTOSIZE_USEHEADER doesn't always work as expected...

		CHeaderCtrl* pHeaderCtrl = _xtGetHeaderCtrl();
		if (pHeaderCtrl && ::IsWindow(pHeaderCtrl->m_hWnd))
		{
			CWindowDC dc(NULL);
			CFont* pOF = dc.SelectObject(pHeaderCtrl->GetFont());

			TCHAR szColText[256];

			LVCOLUMN lvc;
			lvc.mask = LVCF_TEXT;
			lvc.pszText = szColText;
			lvc.cchTextMax = _countof(szColText);

			ListView_GetColumn(m_pListCtrl->m_hWnd, nCol, &lvc);

			CSize size = dc.GetTextExtent(szColText);
			wc2 = size.cx;

			dc.SelectObject(pOF);
		}

		wc2 += 12; // add padding.
		
		CXTFlatHeaderCtrl* pFlatHeaderCtrl = GetFlatHeaderCtrl();
		if (pFlatHeaderCtrl && pFlatHeaderCtrl->HasSortArrow())
		{
			if (m_nSortedCol == nCol)
				wc2 += 24; // add padding.
		}

		int wc = __max( m_nMinColWidth, __max( wc1, wc2 ));
	
		if( wc > m_nMaxColWidth )
			wc = m_nMaxColWidth;
		
		// set the column width.
		m_pListCtrl->SetColumnWidth( nCol, wc );
	}
	
	m_pListCtrl->SetRedraw();
	m_pListCtrl->Invalidate();
}

BOOL CXTListCtrlBase::OnEraseBkgndImpl(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CXTListCtrlBase::OnPaintImpl() 
{
	// background is already filled in gray
	CPaintDC dc(m_pListCtrl);

	bool bReportView = ( ( ::GetWindowLong( m_pListCtrl->m_hWnd,
		GWL_STYLE ) & LVS_TYPEMASK ) == LVS_REPORT );

	// Get the client rect.
	CRect rectClient;
	m_pListCtrl->GetClientRect(&rectClient);

	// Exclude the header control from being re-painted.
	CHeaderCtrl* pWndHeader = NULL;
	if ( bReportView )
	{
		// Get a pointer to the header control.
		pWndHeader = _xtGetHeaderCtrl();
		if ( pWndHeader != NULL )
		{
			CRect rcHeader;
			pWndHeader->GetWindowRect( &rcHeader );
			m_pListCtrl->ScreenToClient( &rcHeader );
			dc.ExcludeClipRect( &rcHeader );
		}
	}

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient, GetBackColor());

	// Now let the window do its default painting...
	m_pListCtrl->CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	// Refresh the header control.
	if ( bReportView && pWndHeader )
	{
		pWndHeader->Invalidate();
		pWndHeader->UpdateWindow();
	}
}

void CXTListCtrlBase::PreSubclassWindowImpl_Post() 
{
	if (m_bAutoFont)
	{
		// Post a WM_SETFONT message to the window to set the font
		// after initialization.
		if (xtAfxData.bWin95) {
			m_pListCtrl->PostMessage(WM_SETFONT,
				(WPARAM)(HFONT)xtAfxData.font, MAKELPARAM(TRUE,0));
		}
		else {
			m_pListCtrl->SendMessage(WM_SETFONT,
				(WPARAM)(HFONT)xtAfxData.font, MAKELPARAM(TRUE,0));
		}
	}
}

int CXTListCtrlBase::OnCreateImpl_Post(LPCREATESTRUCT lpCreateStruct) 
{
	UNUSED_ALWAYS(lpCreateStruct);

	ASSERT(m_pListCtrl != NULL); // Make sure ImplAttach was called

	if (m_bAutoFont)
	{
		// Set the font for the window.
		m_pListCtrl->SetFont(&xtAfxData.font);
	}
	
	return 0;
}

void CXTListCtrlBase::OnSettingChangeImpl_Post(UINT uFlags, LPCTSTR lpszSection) 
{
	UNUSED_ALWAYS(uFlags);
	UNUSED_ALWAYS(lpszSection);

	m_nMaxColWidth = ::GetSystemMetrics(SM_CXFULLSCREEN);
}

/////////////////////////////////////////////////////////////////////////////
// CXTListCtrl
/////////////////////////////////////////////////////////////////////////////

CXTListCtrl::CXTListCtrl()
{
	ImplAttach(this);
	m_bInitControl = true;
}

CXTListCtrl::~CXTListCtrl()
{
}


IMPLEMENT_DYNAMIC(CXTListCtrl, CListCtrl)

BEGIN_MESSAGE_MAP(CXTListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CXTListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETTINGCHANGE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

void CXTListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	OnCustomDrawImpl(pNMHDR, pResult);
}

BOOL CXTListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	return OnNotifyImpl(wParam, lParam, pResult);
}

void CXTListCtrl::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CListCtrl::OnSettingChange(uFlags, lpszSection);
	OnSettingChangeImpl_Post(uFlags, lpszSection);
}

LRESULT CXTListCtrl::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (m_bInitControl)
	{
		m_bInitControl = false;
		
		if (m_bAutoFont)
		{
			SetFont(&xtAfxData.font);
		}
	}

	return 0;
}

void CXTListCtrl::PreSubclassWindow() 
{
	CListCtrl::PreSubclassWindow();

	// Initialize the control.
	PostMessage(XTWM_INITIAL_UPDATE);
}

int CXTListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	SendMessage(XTWM_INITIAL_UPDATE);
	
	return 0;
}

void CXTListCtrl::OnPaint() 
{
	OnPaintImpl();
}

BOOL CXTListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return OnEraseBkgndImpl(pDC);
}

void CXTListCtrl::OnDestroy()
{
	OnDestroyImpl_Pre();
	CListCtrl::OnDestroy();
}
