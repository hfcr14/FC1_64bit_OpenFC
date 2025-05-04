// XTResize.cpp: implementation of the CXTResize class.
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

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef OBM_SIZE
#define OBM_SIZE 32766
#endif

#define ENTRY_WINDOWPLACEMENT _T("WindowPlacement")

#define IDC_SIZEICON 0x7FFF

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTResize::CXTResize(CWnd* pwnd, const UINT nFlags) : 
	m_pwnd(pwnd),
	m_nFlagsXX(nFlags),
	m_szMin(0,0),
	m_szMax(0,0),
	m_rcWindow(0,0,0,0),
	m_strSection(_T(""))
{

}

CXTResize::~CXTResize()
{
	RemoveAllControls();
}

//////////////////////////////////////////////////////////////////////

BOOL CXTResize::RemoveResize(const UINT nID)
{
	// search for an item with the given id
	int iItem;
	for (iItem = m_arrItems.GetSize(); iItem--; )
	{
		CWnd* pWnd = m_arrItems[iItem]->m_pwnd;
		ASSERT_VALID(pWnd);

		if (pWnd->GetDlgCtrlID() == (int) nID)
		{
			CXTItem* pItem = m_arrItems[iItem];
			if (pItem != NULL)
			{
				m_arrItems.RemoveAt(iItem);
				SAFE_DELETE( pItem );
				return TRUE;
			}
		}
	}

	return FALSE;
}

void CXTResize::RemoveAllControls()
{
	// free allocated memory
	int iIndex;
	for (iIndex = 0; iIndex < m_arrItems.GetSize(); ++iIndex)
	{
		CXTItem* pItem = m_arrItems.GetAt(iIndex);
		SAFE_DELETE( pItem );
	}

	// empty array
	m_arrItems.RemoveAll();
}

static bool _IsGroupBox(HWND hWnd)
{
	TCHAR szClass[8];
	::GetClassName( hWnd, szClass, 8 );
	
	if ( _tcsicmp( szClass, _T( "Button" ) ) == 0 )
	{
		DWORD dwStyle = ::GetWindowLong( hWnd, GWL_STYLE );
		if ( ( dwStyle & ( BS_GROUPBOX|WS_TABSTOP ) ) == BS_GROUPBOX )
		{
			return true;
		}
	}

	return false;
}

void CXTResize::SetResize(const UINT nID, const HWND hWnd, const XT_RESIZERECT& rrcSizing)
{
	CXTItem* pItem = 0;
	
	// search for an item with the given id
	int i;
	for ( i = m_arrItems.GetSize(); i--; )
	{
		CWnd* pWnd = m_arrItems[i]->m_pwnd;
		if (pWnd && ::IsWindow( pWnd->m_hWnd ) && ( pWnd->GetDlgCtrlID( ) == ( int )nID ) )
		{
			pItem = m_arrItems[i];
		}
	}

	// if we didn't find it then create one
	if (pItem == 0)
	{
		HWND hWndChild = hWnd;
		if ( hWndChild == NULL )
		{
			CWnd* pWnd = m_pwnd->GetDlgItem( nID );
			if ( pWnd &&  ::IsWindow( pWnd->m_hWnd ) )
			{
				hWndChild = pWnd->m_hWnd;
			}
		}

		bool bDelete = false;

		CWnd* pWnd = m_pwnd->FromHandlePermanent( hWndChild );
		if ( pWnd == NULL )
		{
			// there is no permanent window, create one
			pWnd = new CWnd; // CXTItem handles the delete
			pWnd->Attach( hWndChild );
		
			bDelete = true;
		}

		CRect rcWindow;
		pWnd->GetWindowRect( rcWindow );
		m_pwnd->ScreenToClient( rcWindow );

		pItem = new CXTItem( pWnd, rrcSizing, rcWindow, bDelete );
		m_arrItems.Add( pItem );

		if ( !HasFlag( SZ_NOTRANSPARENTGROUP ) )
		{
			// if the control is a group box, then make it transparent
			if ( _IsGroupBox( hWndChild ) )
			{
				CWnd::ModifyStyleEx( hWndChild, 0, WS_EX_TRANSPARENT,
					SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE );
			}
		} 
	}
	else
	{
		// the item already exists, just add the new sizing option
		pItem->m_rrcSizing += rrcSizing;
	}
	// we should only allow sizing within the rect {0,0,1,1}
	ASSERT((CXTResizeRect(0,0,1,1) & pItem->m_rrcSizing) == pItem->m_rrcSizing);
}

void CXTResize::Init()
{
	if(m_pwnd == NULL)
		return;

	// clear the control list when initializing.  The reason for this is we may have a
	// static window that calls Init() multiple times for example CDialog::DoModal().
	RemoveAllControls();

	// get the dialog size
	m_pwnd->GetWindowRect(m_rcInitWindow);
	m_rcWindow = m_rcInitWindow;

	if (!HasFlag(SZ_NOMINSIZE)) 
	{
		// set minimum size to current window size
		m_szMin.cx = m_rcWindow.Width(); 
		m_szMin.cy = m_rcWindow.Height(); 
	}

	// set the clip children style to prevent flickering
	if (!HasFlag(SZ_NOCLIPCHILDREN)) 
	{
		m_pwnd->ModifyStyle(0, WS_CLIPCHILDREN);
	}

	// add the size icon
	if (!HasFlag(SZ_NOSIZEICON))
	{
		// Create the size icon if it already doesn't exist.
		if (!m_scSizeIcon.GetSafeHwnd())
			m_scSizeIcon.Create(0, WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, CRect(0,0,16,16), m_pwnd, IDC_SIZEICON);

		CRect rcIcon, rcClient;
		m_pwnd->GetClientRect(rcClient);

		m_scSizeIcon.GetClientRect(rcIcon);
		m_scSizeIcon.SetWindowPos(&CWnd::wndTop, rcClient.right-rcIcon.Width(),
			rcClient.bottom-rcIcon.Height(), 0, 0, SWP_NOSIZE);

		SetResize(IDC_SIZEICON, SZ_REPOS(1));
	}
}

void CXTResize::Size()
{
	if (m_arrItems.GetSize() > 0)
	{
		CRect rcWindow;
		m_pwnd->GetWindowRect(rcWindow);

		int dx = rcWindow.Width()  - m_rcWindow.Width();
		int dy = rcWindow.Height() - m_rcWindow.Height();

		BOOL bDefered = FALSE;
		HDWP hDWP = ::BeginDeferWindowPos(m_arrItems.GetSize());
		
		int i;
		for (i = 0; i < m_arrItems.GetSize(); i++)
		{
			if (Defer(hDWP, m_arrItems[i], dx, dy))
				bDefered = TRUE;
		}

		if (bDefered)
			::EndDeferWindowPos(hDWP);

		for (i = 0; i < m_arrItems.GetSize(); i++)
		{
			CWnd* pWnd = m_arrItems[i]->m_pwnd;
			if (pWnd && _IsGroupBox(pWnd->m_hWnd))
			{
				pWnd->Invalidate();
			}
		}

		m_rcWindow = rcWindow;
	}
}

void CXTResize::Reset()
{
	int iSize = m_arrItems.GetSize();
	int i;
	for (i = 0; i < iSize; i++)
	{
		CXTItem* pItem = m_arrItems[i];
		pItem->m_bInitialSize = FALSE;
		pItem->m_rrcWindow = pItem->m_rrcInitWindow;
	}
}

BOOL CXTResize::Defer(HDWP& hDWP, CXTItem* pItem, int dx, int dy)
{
	HWND hWnd = pItem->m_pwnd->m_hWnd;
	if (::IsWindow(hWnd))
	{
		if (!pItem->m_bInitialSize)
		{
			CRect rcWindow;
			m_pwnd->GetWindowRect(rcWindow);

			dx = rcWindow.Width()  - m_rcInitWindow.Width();
			dy = rcWindow.Height() - m_rcInitWindow.Height();

			pItem->m_bInitialSize = TRUE;
		}

		CXTResizeRect rrcItem = pItem->m_rrcWindow;

		rrcItem.left   += dx * pItem->m_rrcSizing.left;
		rrcItem.top    += dy * pItem->m_rrcSizing.top;
		rrcItem.right  += dx * pItem->m_rrcSizing.right;
		rrcItem.bottom += dy * pItem->m_rrcSizing.bottom;
	
		if (rrcItem != pItem->m_rrcWindow)
		{
			int x  = (int) rrcItem.left;
			int y  = (int) rrcItem.top;
			int cx = (int) rrcItem.Width();
			int cy = (int) rrcItem.Height();

			// Set positioning flags
			UINT uFlags = SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOOWNERZORDER;

			CRect rOld;
			CRect rNew = rrcItem;

			// get the size of the dialog item in client coordinates.
			pItem->m_pwnd->GetWindowRect(&rOld);
			m_pwnd->ScreenToClient(&rOld);

			// if the x-y coordinates have not changed, there is no reason
			// to move the dialog item.
			if (rNew.TopLeft() == rOld.TopLeft())
				uFlags |= SWP_NOMOVE;

			// if the cx-cy size has not changed, there is no reason to
			// size the dialog item.  If size has changed, don't
			// copy bits of the client area (i.e. have them invalidated/redrawn)
			if (rNew.Size() == rOld.Size())
				uFlags |= SWP_NOSIZE;
			else
				uFlags |= SWP_NOCOPYBITS;
			
			hDWP = ::DeferWindowPos(hDWP, hWnd, 0, x, y, cx, cy, uFlags);
			if (hDWP ==  NULL)
			{
				TRACE(_T("DeferWindowPos failed for ID %i\n"), GetDlgCtrlID(hWnd));
				return FALSE;
			}
			pItem->m_rrcWindow = rrcItem;
		}
	}
	return TRUE;
}

void CXTResize::GetMinMaxInfo(MINMAXINFO* pMMI)
{
	if (!m_rcWindow.IsRectNull())
	{
		if (HasFlag(SZ_NOHORISONTAL))
		{
			pMMI->ptMaxTrackSize.x = pMMI->ptMaxSize.x = m_rcWindow.Width();
		}
		if (HasFlag(SZ_NOVERTICAL))
		{
			pMMI->ptMaxTrackSize.y = pMMI->ptMaxSize.y = m_rcWindow.Height();
		}
	}
	if (m_szMin.cx != 0) pMMI->ptMinTrackSize.x = m_szMin.cx;
	if (m_szMin.cy != 0) pMMI->ptMinTrackSize.y = m_szMin.cy;
	if (m_szMax.cx != 0) pMMI->ptMaxTrackSize.x = m_szMax.cx;
	if (m_szMax.cy != 0) pMMI->ptMaxTrackSize.y = m_szMax.cy;
}

void CXTResize::AutoLoadPlacement(LPCTSTR pszSection)
{
	m_strSection = pszSection;
	ASSERT(!m_strSection.IsEmpty());
	LoadPlacement(m_strSection);
}

void CXTResize::LoadPlacement(LPCTSTR pszSection)
{
	UINT nBytes = 0;
	BYTE* pBytes = 0;
	AfxGetApp()->GetProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, &pBytes, &nBytes);
	if (nBytes == sizeof(WINDOWPLACEMENT))
	{
		m_pwnd->SetWindowPlacement((WINDOWPLACEMENT*) pBytes);
	}
	if (pBytes && nBytes) delete[] pBytes;

}

void CXTResize::SavePlacement(LPCTSTR pszSection)
{
	WINDOWPLACEMENT wp;
	m_pwnd->GetWindowPlacement(&wp);
	AfxGetApp()->WriteProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, (BYTE*) &wp, sizeof(wp));
}

void CXTResize::SetResize(XT_SIZING arr[])
{
	int i;
	for (i=0; i<sizeof(arr); i++)
	{
		XT_SIZING& sz = arr[i];
		SetResize(sz.id, sz.rrc.left, sz.rrc.top, sz.rrc.right, sz.rrc.bottom);
	}
}

void CXTResize::SetFlag(EFlags eFlag)
{
	m_nFlagsXX &= (eFlag^0xFFFFFFFF);
	m_nFlagsXX |= eFlag;
}

void CXTResize::SetResize(CWnd* pwnd, const CXTResizeRect& rrcSizing, CRect rcWindow)
{
	CXTItem *pItem = new CXTItem(pwnd, rrcSizing, rcWindow, FALSE);
	m_arrItems.Add(pItem);
}

//////////////////////////////////////////////////////////////////////
// CXTSizeIcon
//////////////////////////////////////////////////////////////////////

CXTSizeIcon::CXTSizeIcon()
{
	// Load the size cursor
	m_hCursor = ::LoadCursor(NULL, IDC_SIZENWSE);
}

BEGIN_MESSAGE_MAP(CXTSizeIcon, CStatic)
	//{{AFX_MSG_MAP(CXTSizeIcon)
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTSizeIcon::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER(pWnd);
	UNREFERENCED_PARAMETER(nHitTest);
	UNREFERENCED_PARAMETER(message);

	// Set the cursor to the size cursor.
	::SetCursor(m_hCursor);
	return TRUE;
}

int CXTSizeIcon::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Load the size bitmap.
	if(!m_bmSizeIcon.GetSafeHandle())
		m_bmSizeIcon.LoadOEMBitmap(OBM_SIZE);
	
	SetBitmap(m_bmSizeIcon);
	
	return 0;
}

void CXTSizeIcon::OnMouseMove(UINT nFlags, CPoint point) 
{
	CStatic::OnMouseMove(nFlags, point);

	// Set the cursor to the size cursor.
	if (::GetCursor() != m_hCursor) {
		::SetCursor(m_hCursor);
	}
}

void CXTSizeIcon::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CStatic::OnLButtonDown(nFlags, point);

	CPoint pt;
	ClientToScreen(&pt);

	GetParent()->SendMessage( WM_NCLBUTTONDOWN, HTBOTTOMRIGHT,
		MAKELPARAM(LOWORD(pt.x), HIWORD(pt.y)) );
}

//////////////////////////////////////////////////////////////////////
// CXTItem
//////////////////////////////////////////////////////////////////////

CXTItem::CXTItem(CWnd* pwnd, const CXTResizeRect& rrcSizing, CRect& rcWindow, BOOL bDeleteWnd)
{
	m_pwnd          = pwnd;
	m_rrcSizing	    = rrcSizing;
	m_rrcWindow     = rcWindow;
	m_rrcInitWindow = rcWindow;
	m_bDeleteWnd    = bDeleteWnd;
	m_bInitialSize  = FALSE;
}

CXTItem::~CXTItem()
{
	if (m_bDeleteWnd)
	{
		m_pwnd->Detach();
		_delete( m_pwnd );
	} 
}
