// XTEditListBox.cpp : implementation of the CXTEditListBox class.
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
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTBrowseEdit.h"
#include "XTMemDC.h"
#include "XTPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTListBox
/////////////////////////////////////////////////////////////////////////////
CXTListBox::CXTListBox()
{
	m_bAutoFont    = true;
	m_bInitControl = true;
}

CXTListBox::~CXTListBox()
{
	
}

IMPLEMENT_DYNAMIC(CXTListBox, CListBox)

BEGIN_MESSAGE_MAP(CXTListBox, CListBox)
	//{{AFX_MSG_MAP(CXTListBox)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

LRESULT CXTListBox::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);
	
	if (m_bInitControl)
	{
		m_bInitControl = false;
		
		// Set the font, and change the style to LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS.
		ModifyStyle(0, LBS_OWNERDRAWVARIABLE|LBS_HASSTRINGS);
		
		if (m_bAutoFont) {
			SetFont(&xtAfxData.font);
		}
	}
	
	return 0;
}

void CXTListBox::PreSubclassWindow() 
{
	CListBox::PreSubclassWindow();

	// Initialize the control.
	PostMessage(XTWM_INITIAL_UPDATE);
}

int CXTListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	SendMessage(XTWM_INITIAL_UPDATE);
	
	return 0;
}

void CXTListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC	 = CDC::FromHandle(lpDIS->hDC);
	CRect rcItem = lpDIS->rcItem;

	if ((lpDIS->itemID >= 0) && (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		int iSavedDC = pDC->SaveDC();

		COLORREF clrWindow = IsWindowEnabled() ? xtAfxData.clrWindow : xtAfxData.clr3DFace;
		COLORREF clrWindowText = IsWindowEnabled() ? xtAfxData.clrWindowText : xtAfxData.clrGrayText;

		if ((lpDIS->itemState & ODS_SELECTED) != 0)
		{
			clrWindow = IsWindowEnabled() ? xtAfxData.clrHighlight  : xtAfxData.clrGrayText;
			clrWindowText = xtAfxData.clrHighlightText;
		}

		// set the text and text background colors, then repaint the item.
		pDC->SetBkColor(clrWindow);
		pDC->SetTextColor(clrWindowText);
		pDC->FillSolidRect(&rcItem, clrWindow);

		CString strText;
		GetText(lpDIS->itemID, strText);

		rcItem.left += 2;
		pDC->DrawText(strText, &rcItem,
			DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);

		pDC->RestoreDC(iSavedDC);
	}
	if (lpDIS->itemAction & ODA_FOCUS)
		pDC->DrawFocusRect(&lpDIS->rcItem);
}

void CXTListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS) 
{
	lpMIS->itemHeight = ::GetSystemMetrics(SM_CYMENU)-2;
}

BOOL CXTListBox::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTListBox::OnPaint() 
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect r;
	GetClientRect(&r);

	// determine the background color.
	COLORREF clrWindow = IsWindowEnabled() ?
		xtAfxData.clrWindow : xtAfxData.clr3DFace;

	// Paint to a memory device context to reduce screen flicker.
	CXTMemDC memDC(&dc, r, clrWindow);

	// Let the window do its default painting...
	CListBox::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

void CXTListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Turn off redraw while scrolling, this will cause paint problems
	// with our control because we are painting off screen to reduce
	// flicker.  The default implementation assumes that WM_ERASEBKGND
	// has repainted the background, which in our case does not happen.

	SetRedraw(false);
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
	SetRedraw(true);

	// repaint the list box
	Invalidate();
	UpdateWindow();

	// repaint the scroll bar.
	::SetWindowPos(m_hWnd, NULL, 0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
}

void CXTListBox::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// Turn off redraw while scrolling, this will cause paint problems
	// with our control because we are painting off screen to reduce
	// flicker.  The default implementation assumes that WM_ERASEBKGND
	// has repainted the background, which in our case does not happen.

	SetRedraw(false);
	CListBox::OnHScroll(nSBCode, nPos, pScrollBar);
	SetRedraw(true);

	// repaint the list box
	Invalidate();
	UpdateWindow();

	// repaint the scroll bar.
	::SetWindowPos(m_hWnd, NULL, 0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
}

//////////////////////////////////////////////////////////////////////
// CXTFontEnum
//////////////////////////////////////////////////////////////////////

const int BMW = 16;
const int BMH = 20;

CXTFontEnum::CXTFontEnum()
{
	Init();
}

CXTFontEnum::~CXTFontEnum()
{

}

CXTFontEnum& CXTFontEnum::Get()
{
	static CXTFontEnum fontEnum;
	return fontEnum;
}

bool CXTFontEnum::DoesFontExist(CString& strFaceName)
{
	POSITION pos;
	for ( pos = m_listFonts.GetHeadPosition( ); pos; m_listFonts.GetNext( pos ) )
	{
		XT_LOGFONT& lf = m_listFonts.GetAt( pos );
		if ( strFaceName.CompareNoCase( lf.lfFaceName ) == 0 )
		{
			strFaceName = lf.lfFaceName; // adjust case if necessary
			return true;
		}
	}
	return false;
}

XT_LOGFONT* CXTFontEnum::GetLogFont(CString strFaceName)
{
	POSITION pos;
	for ( pos = m_listFonts.GetHeadPosition( ); pos; m_listFonts.GetNext( pos ) )
	{
		XT_LOGFONT& lf = m_listFonts.GetAt( pos );
		if ( strFaceName == lf.lfFaceName )
		{
			return &lf;
		}
	}
	return NULL;
}

bool CXTFontEnum::AddFont(const LOGFONT* pLF, DWORD dwType)
{
	// See if the font name has already been added.
	CXTString strFaceName = pLF->lfFaceName;
	strFaceName.Remove( '@' );

	if ( DoesFontExist( strFaceName ) != 0 )
	{
		return false;
	}

	LOGFONT lfGlobal;
	xtAfxData.font.GetLogFont( &lfGlobal );

	// Save the XT_LOGFONT struct.
	XT_LOGFONT lf;
	::CopyMemory( ( void* )&lf, ( const void* )pLF, sizeof( XT_LOGFONT ) );

	lf.dwType   = dwType;
	lf.lfHeight = -( ::GetSystemMetrics( SM_CYVTHUMB )+1 );
	lf.lfWidth  = 0;

	m_listFonts.AddTail( lf );

	return true;
}

BOOL CALLBACK CXTFontEnum::EnumFontFamExProc(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX* lpntm, DWORD dwFontType, LPARAM lParam)
{
	UNREFERENCED_PARAMETER( lpntm );
	
	CXTFontEnum* pFontEnum = reinterpret_cast< CXTFontEnum* >( lParam );
	if ( pFontEnum != NULL )
	{
		if ( pFontEnum->AddFont( &pelf->elfLogFont, dwFontType ) )
		{
			// TODO:
		}
	}
	
	return TRUE;
}

int CXTFontEnum::GetMaxWidth()
{
	CWindowDC dc( NULL );
	CFont* p = dc.SelectObject( &xtAfxData.font );

	int iMaxWidth = 0;

	POSITION pos;
	for ( pos = m_listFonts.GetHeadPosition( ); pos; m_listFonts.GetNext( pos ) )
	{
		XT_LOGFONT& lf = m_listFonts.GetAt( pos );
	
		iMaxWidth = __max( iMaxWidth, 
			dc.GetTextExtent( lf.lfFaceName ).cx );
	}

	dc.SelectObject( p );
	return iMaxWidth;
}

void CXTFontEnum::Init(CDC* pDC/*=NULL*/, BYTE nCharSet/*=DEFAULT_CHARSET*/)
{
	m_listFonts.RemoveAll();

	// Enumerate all styles of all fonts for the ANSI character set
	XT_LOGFONT lf;
	lf.lfFaceName[ 0 ] = '\0';
	lf.lfCharSet = nCharSet;
	
	if ( pDC == NULL || pDC->m_hDC == NULL )
	{
		CWindowDC dc( NULL );
		::EnumFontFamiliesEx( dc.m_hDC, &lf,
			( FONTENUMPROC )EnumFontFamExProc, ( LPARAM )this, 0 );
	}
	else
	{
		::EnumFontFamiliesEx( pDC->m_hDC, &lf,
			( FONTENUMPROC )EnumFontFamExProc, ( LPARAM )this, 0 );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTFontListBox
/////////////////////////////////////////////////////////////////////////////

CXTFontListBox::CXTFontListBox()
{
	m_strSymbol = _T( "AbCdEfGhIj" );
	m_eStyle    = XT_FLB_NAME_SAMPLE;

	VERIFY( m_ilFontType.Create(
		XT_IDB_FONTTYPE, BMW, 1, RGB( 255,0,255 ) ) );
}

CXTFontListBox::~CXTFontListBox()
{
}

BEGIN_MESSAGE_MAP(CXTFontListBox, CXTListBox)
	//{{AFX_MSG_MAP(CXTFontListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTFontListBox message handlers

void CXTFontListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	if ( lpDIS->itemID == -1 )
	{
		return;
	}

	CDC*  pDC	   = CDC::FromHandle( lpDIS->hDC );
	CRect rcItem   = lpDIS->rcItem;
	int   nState   = lpDIS->itemState;
	int   nAction  = lpDIS->itemAction;
	int   nItem    = lpDIS->itemID;
	int   nIndexDC = pDC->SaveDC( );

	CString strFontName;
	GetText( nItem, strFontName );

	XT_LOGFONT* pLF = ( XT_LOGFONT* )GetItemData( nItem  );
	if ( pLF == NULL )
	{
		return;
	}

	// highlight the background if selected.
	if ( nState & ODS_SELECTED ) 
	{
		if ( ( nAction & ODA_FOCUS ) && ( nState & ODS_FOCUS ) )
		{
			pDC->FillSolidRect( rcItem, xtAfxData.clrHighlight );
			pDC->SetTextColor( xtAfxData.clrHighlightText );
			pDC->DrawFocusRect( &rcItem );
		}
		else
		{
			pDC->FillSolidRect( rcItem, xtAfxData.clr3DFace );
			pDC->SetTextColor( xtAfxData.clrWindowText );
		}
	}
	else
	{
		pDC->FillSolidRect( rcItem, xtAfxData.clrWindow );
		pDC->SetTextColor( xtAfxData.clrWindowText );
	}

	// true type or device flag set by SetItemData
	if ( pLF->dwType & TRUETYPE_FONTTYPE )
	{
		m_ilFontType.Draw( pDC, 1, rcItem.TopLeft(), ILD_NORMAL );
	}

	// Define the size of the text area to draw
	CRect rcText( rcItem );
	rcText.left += BMW + 6;

	pDC->SetBkMode( TRANSPARENT );

	if ( ( m_eStyle == XT_FLB_BOTH ) || ( m_eStyle == XT_FLB_NAME_GUI ) )
	{
		// Draw the text item.
		pDC->SelectObject( &xtAfxData.font );
		pDC->DrawText( strFontName, rcText, DT_VCENTER | DT_SINGLELINE );

		rcText.left = rcText.left + CXTFontEnum::Get().GetMaxWidth() + 5;
	}

	if ( ( m_eStyle == XT_FLB_BOTH ) || ( m_eStyle == XT_FLB_NAME_SAMPLE ) )
	{
		if ( ( m_eStyle == XT_FLB_NAME_SAMPLE ) && ( pLF->lfCharSet == SYMBOL_CHARSET ) )
		{
			pDC->SelectObject( &xtAfxData.font );
			pDC->DrawText( strFontName, rcText, DT_VCENTER | DT_SINGLELINE );

			int iOffset = pDC->GetTextExtent( strFontName ).cx + 5;
			rcText.left += iOffset;

			strFontName = m_strSymbol;
		}

		// Create the font to be displayed and initialize the device context.
		CFont font;
		font.CreateFontIndirect( pLF );
		
		// Draw the text item.
		pDC->SelectObject( &font );
		pDC->DrawText( strFontName, rcText, DT_VCENTER | DT_SINGLELINE );
	}

	// Restore the original device context.
	pDC->RestoreDC( nIndexDC );
}

void CXTFontListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS) 
{
	ASSERT( lpMIS->CtlType == ODT_LISTBOX );
	
	lpMIS->itemHeight = BMH;
	lpMIS->itemWidth  = 0;
}

int CXTFontListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS) 
{
	ASSERT( lpCIS->CtlType == ODT_LISTBOX );

	int iItem1 = ( int )( WORD )lpCIS->itemID1;
	if ( iItem1 == -1 )
	{
		return -1;
	}

	CString strItem1;
	GetText( iItem1, strItem1 );

	int iItem2 = ( int )( WORD )lpCIS->itemID2;
	if ( iItem2 == -1 )
	{
		return -1;
	}

	CString strItem2;
	GetText( iItem2, strItem2 );

	return strItem1.Collate( strItem2 );
}

void CXTFontListBox::InitControl()
{
	CXTFontList& fontList = CXTFontEnum::Get().GetFontList();

	POSITION pos;
	for ( pos = fontList.GetHeadPosition( ); pos; fontList.GetNext( pos ) )
	{
		XT_LOGFONT& lf = fontList.GetAt( pos );
		int iIndex = AddString( lf.lfFaceName );

		SetItemData( iIndex, ( DWORD_PTR )&lf );
	}
}

bool CXTFontListBox::GetSelFont(XT_LOGFONT& lf)
{
	int iCurSel = GetCurSel( );
	if ( iCurSel == LB_ERR )
	{
		return false;
	}

	XT_LOGFONT* pLF = ( XT_LOGFONT* )GetItemData( iCurSel );
	if ( pLF == NULL )
	{
		return false;
	}

	::CopyMemory( ( void* )&lf, ( const void* )pLF, sizeof( XT_LOGFONT ) );
	
	return true;
}

bool CXTFontListBox::GetSelFont(CString& strFaceName)
{
	XT_LOGFONT lf;
	if ( GetSelFont( lf ) )
	{
		strFaceName = lf.lfFaceName;
		return true;
	}
	return false;
}

bool CXTFontListBox::SetSelFont(XT_LOGFONT& lf)
{
	return SetSelFont( lf.lfFaceName );
}

bool CXTFontListBox::SetSelFont(CString strFaceName)
{
	int iCount = GetCount( );
	int i;
	for ( i = 0; i < iCount; ++i )
	{
		XT_LOGFONT* pLF = ( XT_LOGFONT* )GetItemData( i );
		if ( pLF != NULL )
		{
			if ( strFaceName.CompareNoCase( pLF->lfFaceName ) == 0 )
			{
				SetCurSel( i );
				return true;
			}
		}
	}

	return false;
}
/////////////////////////////////////////////////////////////////////////////
// CXTEditListBox
/////////////////////////////////////////////////////////////////////////////

const int BTN_OFFSET = 22;

CXTEditListBox::CXTEditListBox()
: m_bEnableEdit(true)
{
	m_pItemEdit    = NULL;
	m_nIndex       = -1;
	m_bNewItem	   = FALSE;
	m_bInitControl = true;

	SetListEditStyle(XT_IDS_DIRECTORIES, LBS_XT_CHOOSEDIR);
}

CXTEditListBox::~CXTEditListBox()
{
}

IMPLEMENT_DYNAMIC(CXTEditListBox, CXTListBox)

BEGIN_MESSAGE_MAP(CXTEditListBox, CXTListBox)
	//{{AFX_MSG_MAP(CXTEditListBox)
	ON_LBN_XT_LABELEDITEND(XT_IDC_LBOX_EDIT, OnEndLabelEdit)
	ON_LBN_XT_NEWITEM(XT_IDC_GROUP_EDIT, OnNewItem)
	ON_LBN_XT_DELETEITEM(XT_IDC_GROUP_EDIT, OnDeleteItem)
	ON_LBN_XT_MOVEITEMUP(XT_IDC_GROUP_EDIT, OnMoveItemUp)
	ON_LBN_XT_MOVEITEMDOWN(XT_IDC_GROUP_EDIT, OnMoveItemDown)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCMBUTTONDOWN()
	ON_WM_NCRBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	ON_LBN_XT_LABELEDITCANCEL(XT_IDC_LBOX_EDIT, OnEndLabelEdit)
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

void CXTEditListBox::OnEndLabelEdit()
{
	// Get a pointer to the owner window.
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	if (m_bNewItem)
	{
		// Delete the temp string.
		DeleteString( m_nIndex );

		// If the text string is empty, then send a cancel message.
		if (m_strItemText.IsEmpty())
		{
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				LBN_XT_LABELEDITCANCEL), (LPARAM)m_hWnd );
		}

		// The user has entered text, send the new item message
		// and insert the new string.
		else
		{
			m_nIndex = AddString( m_strItemText );
			// NB: SetTopIndex below causes some confusion if you have editing session, change content
            // and then double click on another item - the result looks like misplaced 
            // edit control
			//SetTopIndex( m_nIndex );
	

			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(),
				LBN_XT_NEWITEM ), (LPARAM)m_hWnd );
		}
	}
	else
	{
		// set the index to the current selection.
		m_nIndex = GetCurSel();

		// Get the text for the selected item.
		CString strItemText;
		GetText(m_nIndex, strItemText);

		// If the new string is the same as the old string,
		// then send a cancel message.
		if (strItemText == m_strItemText)
		{
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				LBN_XT_LABELEDITCANCEL), (LPARAM)m_hWnd );
		}

		// The user has updated the text string, delete the 
		// existing string and replace it with the new text.
		else
		{
			DWORD dwData = GetItemData( m_nIndex );
			DeleteString( m_nIndex );
			InsertString( m_nIndex, m_strItemText );
			SetItemData( m_nIndex, dwData);
			
            // NB: SetTopIndex below causes some confusion if you have editing session, change content
            // and then double click on another item - the result looks like misplaced 
            // edit control
			//SetTopIndex( m_nIndex );
	
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
				LBN_XT_LABELEDITEND), (LPARAM)m_hWnd );
		}
	}

	// Set the focus back to the listbox.
	SetCurSel( m_nIndex );
	SetFocus();

	m_pItemEdit = NULL;
}

// This member function will enable editing for the a list box item.
void CXTEditListBox::EditItem(int iItem)
{
	SetFocus();
	SetCurSel(iItem);
	m_nIndex = iItem;
	EditListItem(FALSE);
}

void CXTEditListBox::EditListItem(BOOL bNewItem)
{
	if (!m_bEnableEdit)
		return;

	// Turn off redraw until we are ready to
	// create the edit field.
	SetRedraw( FALSE );

	// flush the item text string.
	m_strItemText.Empty();

	// if this is a new item, add a temporary string
	// to the list box and set the selection to it. This
	// is where the in place edit box will appear.
	if ( bNewItem || GetCurSel() == LB_ERR )
	{
		m_nIndex = AddString(_T(""));
		SetCurSel( m_nIndex );
		m_bNewItem = TRUE;
	}

	// get the text for the currently selected
	// item and set the new flag to FALSE.
	else
	{
		m_bNewItem = FALSE;

        int iCount = GetCount( );
        m_nIndex = GetCurSel( );

        if ( m_nIndex >= iCount || iCount <= 0 )
        {
			m_nIndex = LB_ERR;
			SetCurSel( LB_ERR );
	
			// turn redraw back.
			SetRedraw( TRUE );
			Invalidate();

			return;
        }
		
        GetText( m_nIndex, m_strItemText );
	}

    // turn redraw back.
	SetRedraw( TRUE );
	Invalidate();

	// Get the size of the currently selected item.
	CRect rcItem;
	GetItemRect(m_nIndex, rcItem);
    rcItem.InflateRect( -2, 2 );

	// create the edit box.
	m_pItemEdit = new CXTItemEdit( this, rcItem,
		m_strItemText, m_dwLStyle, true );

	// if defined, set the filter for the item edit control.
	if ( !m_strFilter.IsEmpty() ) {
		m_pItemEdit->SetDlgFilter(m_strFilter);
	}
}

void CXTEditListBox::OnNewItem()
{
	EditListItem(TRUE);
}

void CXTEditListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);
	EditListItem(FALSE);
}

void CXTEditListBox::OnDeleteItem()
{
	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	// Send notification to owner that we are about to delete the item
	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_PREDELETEITEM), (LPARAM)m_hWnd );

	int nCurIndex = GetCurSel();	// Current index.

	// If there is a valid selection made...
	if (nCurIndex != LB_ERR)
	{
		// Remove the new current item.
		int nCount = DeleteString(nCurIndex);
		
		if ((nCount != LB_ERR) && (nCurIndex < nCount)) {
			SetCurSel(nCurIndex);
		}
		else {
			SetCurSel(nCurIndex-1);
		}
	}

	// Set the focus back to the listbox.
	SetFocus();

	// Send notification to owner.
	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_DELETEITEM), (LPARAM)m_hWnd );
}

void CXTEditListBox::OnMoveItemUp()
{
	m_nIndex = GetCurSel(); // Current index.
	int nNewIndex = m_nIndex-1; // New index.

	// If there is a valid selection made, and the
	// selection is not the first item and...
	if ((m_nIndex != LB_ERR) && (m_nIndex != 0))
	{
		// Save the string for the item we are
		// going to swap position with.
		CString strItem;
		GetText(nNewIndex, strItem);
		
		// Save any item data.
		DWORD dwNewData = GetItemData(nNewIndex);

		// Remove both the new and current
		// index items...
		DeleteString(nNewIndex);
		
		// then swap and re-insert them into
		// the list box.
		InsertString(m_nIndex, strItem);

		// Restore any item data.
		SetItemData(m_nIndex, dwNewData);
		
		// Select the item at the new index.
		SetCurSel(nNewIndex);
	}

	// Set the focus back to the listbox.
	SetFocus();

	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	// Send notification to owner.
	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_MOVEITEMUP), (LPARAM)m_hWnd );
}

void CXTEditListBox::OnMoveItemDown()
{
	m_nIndex = GetCurSel(); // Current index.
	int nNewIndex = m_nIndex+1; // New index.

	// If there is a valid selection made, and the
	// selection is not the last item and...
	if ((m_nIndex != LB_ERR) && (nNewIndex < GetCount()))
	{
		// Save the string for the current
		// selection.
		CString strCurItem;
		GetText(m_nIndex, strCurItem);
		
		// Save any item data.
		DWORD dwCurData = GetItemData(m_nIndex);

		// Remove both the new and current
		// index items...
		DeleteString(m_nIndex);
		
		// then swap and re-insert them into
		// the list box.
		InsertString(nNewIndex, strCurItem);

		// Restore any item data.
		SetItemData(nNewIndex, dwCurData);
		
		// Select the item at the new index.
		SetCurSel(nNewIndex);
	}

	// Set the focus back to the listbox.
	SetFocus();

	CWnd* pOwner = GetOwner();
	ASSERT(pOwner);

	// Send notification to owner.
	pOwner->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
		LBN_XT_MOVEITEMDOWN), (LPARAM)m_hWnd );
}

void CXTEditListBox::SetListEditStyle(UINT nTitle, DWORD dwLStyle/*=0x0*/)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	CString string;
	string.LoadString(nTitle);
	SetListEditStyle(string, dwLStyle);
}

void CXTEditListBox::SetListEditStyle(LPCTSTR lpszTitle, DWORD dwLStyle/*=0x0*/)
{
	m_strTitle = lpszTitle;
	m_dwLStyle = dwLStyle;

	if (m_editGroup.GetSafeHwnd()) {
		m_editGroup.SetWindowText(m_strTitle);
	}
}

LRESULT CXTEditListBox::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	if (m_bInitControl)
	{
		m_bInitControl = false;

		// Create the edit toolbar group.
		if (HasToolbar())
		{
			VERIFY( CreateEditGroup( ));
		}
	}

	return 0;
}

void CXTEditListBox::PreSubclassWindow() 
{
	CXTListBox::PreSubclassWindow();

	// Initialize the control.
	PostMessage(XTWM_INITIAL_UPDATE);
}

int CXTEditListBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTListBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	SendMessage(XTWM_INITIAL_UPDATE);
	
	return 0;
}

BOOL CXTEditListBox::CreateEditGroup()
{
	// Create the edit button group that appears at the top of the
	// list edit control.
    DWORD dwStyle = (GetStyle() & WS_VISIBLE) | 
         WS_CHILD | WS_TABSTOP | SS_CENTERIMAGE | SS_LEFT;

	if( !m_editGroup.CreateEx( WS_EX_STATICEDGE, _T("Static"), m_strTitle, 
		dwStyle, CRect(0,0,0,0), GetParent(), XT_IDC_GROUP_EDIT ))
	{
		TRACE0( "Unable to create caption.\n" );
		return FALSE;
	}
	m_editGroup.Initialize(this);

	if (m_bAutoFont) {
		SetFont(&xtAfxData.font);
	}

    // move edit group (toolbar) control.
    PositionEditGroup(true);

	// enable the window depending on the list box enabled state.
    m_editGroup.EnableWindow( IsWindowEnabled() );

	return TRUE;
}

BOOL CXTEditListBox::PreTranslateMessage(MSG* pMsg)
{
	// Send message to edit window if active.
	if ( m_pItemEdit && ::IsWindow( m_pItemEdit->m_hWnd ) )
	{
		if ( pMsg->wParam == VK_TAB )
		{
			SetFocus( );
			return CWnd::PreTranslateMessage(pMsg);
		}
		
		return m_pItemEdit->PreTranslateMessage(pMsg);
	}

	if (GetCurSel() != LB_ERR)
	{
		if (pMsg->message == WM_KEYUP)
		{
			if (pMsg->wParam == VK_DELETE) {
				OnDeleteItem();
			}
			if(pMsg->wParam == VK_INSERT) {
				OnNewItem();
			}
		}
		
		if (pMsg->message == WM_SYSKEYUP)
		{
			if ((pMsg->wParam == VK_UP) && 
				(::GetKeyState(VK_MENU) < 0)) {
				OnMoveItemUp();
			}
			if ((pMsg->wParam == VK_DOWN) && 
				(::GetKeyState(VK_MENU) < 0)) {
				OnMoveItemDown();
			}
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CXTEditListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CXTListBox::OnLButtonDown(nFlags, point);

	// TODO: Add your message handler code here and/or call default
	m_nIndex = GetCurSel();

	CRect rcItem;
	if (GetItemRect(m_nIndex, rcItem) != LB_ERR)
	{
		if (!rcItem.PtInRect(point))
			SetCurSel(-1);
	}
	
}

void CXTEditListBox::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pItemEdit)
	{
		m_pItemEdit->DestroyWindow();
		m_pItemEdit = NULL;
	}

	CListBox::OnNcLButtonDown(nHitTest, point);
}

void CXTEditListBox::OnNcMButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pItemEdit)
	{
		m_pItemEdit->DestroyWindow();
		m_pItemEdit = NULL;
	}
	
	CListBox::OnNcMButtonDown(nHitTest, point);
}

void CXTEditListBox::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_pItemEdit)
	{
		m_pItemEdit->DestroyWindow();
		m_pItemEdit = NULL;
	}
	
	CListBox::OnNcRButtonDown(nHitTest, point);
}

void CXTEditListBox::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if (HasToolbar() && ::IsWindow(m_editGroup.m_hWnd))
	{
        lpwndpos->cy -= BTN_OFFSET;
		lpwndpos->y  += BTN_OFFSET;
	}

	CXTListBox::OnWindowPosChanging(lpwndpos);
}

void CXTEditListBox::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CXTListBox::OnWindowPosChanged(lpwndpos);
	
    // move edit group (toolbar) control.
    PositionEditGroup();
}

void CXTEditListBox::PositionEditGroup(bool bSizeList/*=false*/)
{
	if (HasToolbar() && ::IsWindow(m_editGroup.m_hWnd))
	{
        CWnd* pParentWnd = GetParent();
        ASSERT_VALID(pParentWnd);

        CRect r;
        GetWindowRect(&r);
        pParentWnd->ScreenToClient(&r);

        if (bSizeList)
        {
            // call SetWindowPos, this causes WM_WINDOWPOSCHANGING to
            // get called which will adjust the size of the edit list box window.
            // WM_WINDOWPOSCHANGED is then called which moves the group edit control.
            
			::SetWindowPos( m_hWnd, NULL, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED );
        }
        else
        {
            // move the group edit control.
			r.top    -= BTN_OFFSET;
			r.bottom  = r.top + BTN_OFFSET;
			
			::SetWindowPos( m_editGroup.m_hWnd, NULL, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED );
		}
	}
}

void CXTEditListBox::OnEnable(BOOL bEnable) 
{
	CXTListBox::OnEnable(bEnable);
	
	if (HasToolbar() && ::IsWindow(m_editGroup.m_hWnd))
	{
		m_editGroup.EnableWindow(bEnable);
	}
}

void CXTEditListBox::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CXTListBox::OnShowWindow(bShow, nStatus);
	
	if (HasToolbar() && ::IsWindow(m_editGroup.m_hWnd))
	{
		m_editGroup.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTEditGroup
/////////////////////////////////////////////////////////////////////////////

CXTEditGroup::CXTEditGroup()
: m_bEnableEdit(true)
{
	m_sizeBtn = CSize(14,12);
	m_ImageList.Create(XT_IDB_LISTEDIT, 14, 1, RGB(255,0,255));
}

CXTEditGroup::~CXTEditGroup()
{
}

IMPLEMENT_DYNAMIC(CXTEditGroup, CStatic)

BEGIN_MESSAGE_MAP(CXTEditGroup, CStatic)
	//{{AFX_MSG_MAP(CXTEditGroup)
	ON_BN_CLICKED(XT_IDC_BTN_NEW, OnButtonNew)
	ON_BN_CLICKED(XT_IDC_BTN_DELETE, OnButtonDelete)
	ON_BN_CLICKED(XT_IDC_BTN_UP, OnButtonUp)
	ON_BN_CLICKED(XT_IDC_BTN_DOWN, OnButtonDown)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTEditGroup::PreTranslateMessage(MSG* pMsg) 
{
	// Let the ToolTip process this message.
	m_tooltip.RelayEvent(pMsg);
	return CStatic::PreTranslateMessage(pMsg);
}

void CXTEditGroup::MoveButtons()
{
	if (::IsWindow(m_hWnd))
	{
		// Set the initial size for the first button.
		CRect r;
		GetWindowRect(&r);
		ScreenToClient(&r);
		r.DeflateRect(1,1);
		r.left = r.right-BTN_OFFSET;

		if ( ::IsWindow( m_btnDown.m_hWnd ))
		{
			m_rClip[0] = r;
			::SetWindowPos( m_btnDown.m_hWnd, NULL, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED );

			r.OffsetRect(-BTN_OFFSET,0);
		}
		
		if ( ::IsWindow( m_btnUp.m_hWnd ))
		{
			m_rClip[1] = r;
			::SetWindowPos( m_btnUp.m_hWnd, NULL, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED );

			r.OffsetRect(-BTN_OFFSET,0);
		}
		
		if ( ::IsWindow( m_btnDelete.m_hWnd ))
		{
			m_rClip[2] = r;
			::SetWindowPos( m_btnDelete.m_hWnd, NULL, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED );

			r.OffsetRect(-BTN_OFFSET,0);
		}
		
		if ( ::IsWindow( m_btnNew.m_hWnd ))
		{
			m_rClip[3] = r;
			::SetWindowPos( m_btnNew.m_hWnd, NULL, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED );
		}

		if ( ::IsWindow( m_hWnd ))
		{
			Invalidate();
			UpdateWindow();
		}
	}
}

void CXTEditGroup::Initialize(CWnd* pNotifyWnd)
{
	ASSERT_VALID(pNotifyWnd); // Must be valid.
	m_pNotifyWnd = pNotifyWnd;

	// Set the style for the navigation buttons.
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|BS_ICON|BS_OWNERDRAW|BS_CENTER|BS_VCENTER;

	// Create the down button
	if (!m_btnDown.Create(NULL, dwStyle, CRect(0,0,0,0), this, XT_IDC_BTN_DOWN))
	{
		TRACE0("Unable to create button.\n");
		return;
	}

	// Create the up button
	if (!m_btnUp.Create(NULL, dwStyle, CRect(0,0,0,0), this, XT_IDC_BTN_UP))
	{
		TRACE0("Unable to create button.\n");
		return;
	}

	// Create the delete button
	if (!m_btnDelete.Create(NULL, dwStyle, CRect(0,0,0,0), this, XT_IDC_BTN_DELETE))
	{
		TRACE0("Unable to create button.\n");
		return;
	}

	// Create the new button
	if (!m_btnNew.Create(NULL, dwStyle, CRect(0,0,0,0), this, XT_IDC_BTN_NEW))
	{
		TRACE0("Unable to create button.\n");
		return;
	}

	// Set the icon for each of the navigation buttons.
	m_hIconDown = m_ImageList.ExtractIcon( 3 );
	m_btnDown.SetIcon( m_sizeBtn, m_hIconDown );

	m_hIconUp = m_ImageList.ExtractIcon( 2 );
	m_btnUp.SetIcon( m_sizeBtn, m_hIconUp );

	m_hIconDelete = m_ImageList.ExtractIcon( 1 );
	m_btnDelete.SetIcon( m_sizeBtn, m_hIconDelete );

	m_hIconNew = m_ImageList.ExtractIcon( 0 );
	m_btnNew.SetIcon( m_sizeBtn, m_hIconNew );

	// Move the buttons to their correct location.
	MoveButtons();
	
	// Create the ToolTip control.
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);

	// Add tooltips to group buttons.
	m_tooltip.AddTool(&m_btnNew,    XT_IDC_BTN_NEW);
	m_tooltip.AddTool(&m_btnDelete, XT_IDC_BTN_DELETE);
	m_tooltip.AddTool(&m_btnUp,     XT_IDC_BTN_UP);
	m_tooltip.AddTool(&m_btnDown,   XT_IDC_BTN_DOWN);

	m_btnNew.SetXButtonStyle(BS_XT_FLAT);
	m_btnDelete.SetXButtonStyle(BS_XT_FLAT);
	m_btnUp.SetXButtonStyle(BS_XT_FLAT);
	m_btnDown.SetXButtonStyle(BS_XT_FLAT);

	// Set the font for this window.
	SetFont(&xtAfxData.font);
}

void CXTEditGroup::OnButtonNew() 
{
	if (m_bEnableEdit)
	{
		m_pNotifyWnd->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			LBN_XT_NEWITEM), (LPARAM)m_hWnd );
	}
}

void CXTEditGroup::OnButtonDelete() 
{
	if (m_bEnableEdit)
	{
		m_pNotifyWnd->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			LBN_XT_DELETEITEM), (LPARAM)m_hWnd );
	}
}

void CXTEditGroup::OnButtonUp() 
{
	if (m_bEnableEdit)
	{
		m_pNotifyWnd->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			LBN_XT_MOVEITEMUP), (LPARAM)m_hWnd );
	}
}

void CXTEditGroup::OnButtonDown() 
{
	if (m_bEnableEdit)
	{
		m_pNotifyWnd->SendMessage( WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(),
			LBN_XT_MOVEITEMDOWN), (LPARAM)m_hWnd );
	}
}

void CXTEditGroup::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CStatic::OnWindowPosChanged(lpwndpos);
	
	// Move the buttons to their correct location.
	MoveButtons();
}

BOOL CXTEditGroup::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTEditGroup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Exclude the buttons from getting repainted.
	int i;
	for (i = 0; i < _countof(m_rClip); ++i)
		dc.ExcludeClipRect(&m_rClip[i]);
	
	CRect r;
	GetClientRect(&r);
	CXTMemDC memDC(&dc, r);

	CStatic::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

void CXTEditGroup::OnEnable(BOOL bEnable) 
{
	CStatic::OnEnable(bEnable);
	
	if (::IsWindow(m_btnDown.m_hWnd))
	{
		m_btnDown.EnableWindow(bEnable);
	}
	
	if (::IsWindow(m_btnUp.m_hWnd))
	{
		m_btnUp.EnableWindow(bEnable);
	}
	
	if (::IsWindow(m_btnDelete.m_hWnd))
	{
		m_btnDelete.EnableWindow(bEnable);
	}
	
	if (::IsWindow(m_btnNew.m_hWnd))
	{
		m_btnNew.EnableWindow(bEnable);
	}
}

void CXTEditGroup::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CStatic::OnShowWindow(bShow, nStatus);
	
	if (::IsWindow(m_btnDown.m_hWnd))
	{
		m_btnDown.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
	
	if (::IsWindow(m_btnUp.m_hWnd))
	{
		m_btnUp.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
	
	if (::IsWindow(m_btnDelete.m_hWnd))
	{
		m_btnDelete.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
	
	if (::IsWindow(m_btnNew.m_hWnd))
	{
		m_btnNew.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}
