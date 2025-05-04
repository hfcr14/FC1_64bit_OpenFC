// XTMaskEdit.cpp : implementation of the CXTMaskEdit class.
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
#include "XTMemDC.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTMaskEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTMaskEdit
/////////////////////////////////////////////////////////////////////////////

static const TCHAR CH_PROMPT = '_';

CXTMaskEdit::CXTMaskEdit( )
	: m_nStartChar( 0 )
	, m_nEndChar( 0 )
	, m_bOverType( true )
	, m_bUseMask( true )
	, m_bRedo( false )
	, m_bModified( false )
	, m_strWindowText( _T( "" ) )
	, m_strMask( _T( "" ) )
	, m_strLiteral( _T( "" ) )
	, m_strDefault( _T( "" ) )
	, m_strUndoBuffer( _T( "" ) )
	, m_chPrompt( CH_PROMPT )
{
	m_bFlatLook = FALSE;
}

CXTMaskEdit::~CXTMaskEdit( )
{

}

IMPLEMENT_DYNAMIC(CXTMaskEdit, CXTFlatEdit)

BEGIN_MESSAGE_MAP(CXTMaskEdit, CXTFlatEdit)
	//{{AFX_MSG_MAP(CXTMaskEdit)
	ON_WM_KEYDOWN( )
	ON_WM_CHAR( )
	ON_WM_LBUTTONDOWN( )
	ON_WM_SETFOCUS( )
	ON_WM_LBUTTONDBLCLK( )
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP( )

bool CXTMaskEdit::CorrectPosition(int& iPos, bool bForward/*=true*/, bool bBeep/*=true*/)
{
	GetWindowText( m_strWindowText );
	int iLen = m_strWindowText.GetLength( );
	
	if ( iPos == iLen )
	{
		if ( bBeep )
		{
			::MessageBeep( ( UINT )-1 );
		}
		return false;
	}
	
	if ( PosInRange( iPos ) == false )
		return false;

	if ( m_strLiteral.GetAt( iPos ) != m_chPrompt )
	{
		int i;
		if ( bForward )
		{
			for ( i = iPos; i < iLen; ++i )
			{
				if ( m_strLiteral.GetAt( i ) == m_chPrompt )
				{
					iPos = i;
					break;
				}
			}
		}
		else
		{
			for ( i = iPos; i >= 0; --i )
			{
				if ( m_strLiteral.GetAt( i ) == m_chPrompt )
				{
					iPos = i;
					break;
				}
			}
			
			if ( i == -1 )
			{
				iPos++;
				
				if ( bBeep )
				{
					::MessageBeep( ( UINT )-1 );
				}
				return false;
			}
		}
		
		if ( i == iLen )
		{
			if ( bBeep )
			{
				::MessageBeep( ( UINT )-1 );
			}
			return false;
		}
	}
	return true;
}

CString CXTMaskEdit::GetMaskedText(int iPos/*=0*/)
{
	int iWndLen = m_strWindowText.GetLength( );
	int iLitlen = m_strLiteral.GetLength( );

	ASSERT( iWndLen ==iLitlen ); // must be equal in length

	CString strBuffer;
	int i;
	for ( i = iPos; i < iWndLen; ++i )
	{
		if ( m_strLiteral.GetAt( i ) == m_chPrompt )
		{
			strBuffer += m_strWindowText.GetAt( i );
		}
	}

	return strBuffer;
}

void CXTMaskEdit::DeleteCharAt(int iPos)
{
	if ( PosInRange( iPos ) == false )
		return;

	CString strBuffer = GetMaskedText( iPos+1 );
	strBuffer += m_chPrompt;

	int x = 0;
	int iLen = strBuffer.GetLength( );
	int i;
	for (i = iPos; i < m_strLiteral.GetLength( ); ++i )
	{
		if ( ( m_strLiteral.GetAt( i ) == m_chPrompt ) )
		{
			m_strWindowText.SetAt( i, strBuffer.GetAt( x ) );
			++x;

			if ( x == iLen )
				break;
		}
	}
}

void CXTMaskEdit::InsertCharAt(int iPos, TCHAR nChar)
{
	if ( PosInRange( iPos ) == false )
	{
		return;
	}

	UINT uChar = nChar;
	if ( !CheckChar( uChar, false ) )
	{
		return;
	}

	CString strBuffer = GetMaskedText( iPos );

	CString strTemp = nChar;
	int i, x = 0, iLen = strBuffer.GetLength( );
	for ( i = 1; i < strBuffer.GetLength(); ++i )
	{
		strTemp += strBuffer.GetAt( i-1 );
	}
	strBuffer = strTemp;

	for ( i = iPos; i < m_strWindowText.GetLength( ); ++i )
	{
		if ( m_strLiteral.GetAt( i ) == m_chPrompt )
		{
			m_strWindowText.SetAt( i, strBuffer[ x ] );
			++x;

			if ( x == iLen )
				break;
		}
	}
}

void CXTMaskEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if ( CanUseMask( ) == false )
    {
        CXTFlatEdit::OnKeyDown( nChar, nRepCnt, nFlags ); // default processing.
        return;
    }

	GetWindowText( m_strWindowText ); // refresh
	GetSel( m_nStartChar, m_nEndChar );

	if ( m_strMask.IsEmpty( ) )
	{
		CXTFlatEdit::OnKeyDown( nChar, nRepCnt, nFlags ); // default processing.
		return;
	}

	bool bShift = (::GetKeyState(VK_SHIFT) < 0);
	
	switch ( nChar )
	{
	case VK_UP:
	case VK_LEFT:
		{
			CXTFlatEdit::OnKeyDown( nChar, nRepCnt, nFlags );
			GetSel( m_nStartChar, m_nEndChar );

			int iStartChar = m_nStartChar;
			CorrectPosition( iStartChar, false, false );

			if ( m_nStartChar < iStartChar )
			{
				m_nStartChar = iStartChar;

				if ( !bShift )
					m_nEndChar = iStartChar;

				SetSel( m_nStartChar, m_nEndChar );
			}
		}
		return;

	case VK_DOWN:
	case VK_RIGHT:
		{
			CXTFlatEdit::OnKeyDown( nChar, nRepCnt, nFlags );
			GetSel( m_nStartChar, m_nEndChar );

			int iEndChar = m_nEndChar;
			CorrectPosition( iEndChar, true, false );

			if ( m_nEndChar > iEndChar )
			{
				m_nEndChar = iEndChar;

				if ( !bShift )
					m_nStartChar = iEndChar;

				SetSel( m_nStartChar, m_nEndChar );
			}
		}
		return;

	case VK_HOME:
		{
			CXTFlatEdit::OnKeyDown( nChar, nRepCnt, nFlags );
			GetSel( m_nStartChar, m_nEndChar );

			int iStartChar = m_nStartChar;
			CorrectPosition( iStartChar, true, false );

			if ( m_nStartChar < iStartChar )
			{
				m_nStartChar = iStartChar;
				
				if ( bShift == false )
				{
					m_nEndChar = m_nStartChar;
				}
				SetSel( m_nStartChar, m_nEndChar );
			}
		}
		return;

	case VK_END:
		{
			CXTFlatEdit::OnKeyDown( nChar, nRepCnt, nFlags );
			GetSel( m_nStartChar, m_nEndChar );

			int iEndChar = m_nEndChar;
			CorrectPosition( iEndChar, true, false );

			if ( m_nEndChar > iEndChar )
			{
				m_nEndChar = iEndChar;

				if ( !bShift )
					m_nStartChar = iEndChar;

				SetSel( m_nStartChar, m_nEndChar );
			}
		}
		return;

	case VK_INSERT:
		{
			if ( bShift )
			{
				MaskPaste( );
			}
			else
			{
				m_bOverType = !m_bOverType; // set the type-over flag
			}
		}
		return;

	case VK_DELETE:
		{
			if ( m_nStartChar == m_nEndChar )
			{
				if ( CorrectPosition( m_nStartChar ) )
				{
					DeleteCharAt( m_nStartChar );

					m_bRedo = false; 
					m_bModified = true;
					SetWindowText( m_strWindowText );

					m_nEndChar = m_nStartChar;
					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			else
			{
				MaskClear( );
			}
		}
		return;

	case VK_SPACE:
		{
			GetWindowText( m_strWindowText ); // refresh

			if ( CorrectPosition( m_nStartChar ) )
			{
				if ( m_nStartChar < m_strLiteral.GetLength( ) )
				{
					if ( ProcessMask( nChar, m_nStartChar ) )
					{
						if ( CanOverType( ) )
						{
							m_strWindowText.SetAt( m_nStartChar, ' ' );
						}
						else
						{
							InsertCharAt( m_nStartChar, ( TCHAR )nChar );
						}
					}
					else
					{
						m_strWindowText.SetAt( m_nStartChar, m_chPrompt );
					}

					m_bRedo = false; 
					m_bModified = true;
					SetWindowText( m_strWindowText );
					
					m_nStartChar++;
					m_nEndChar = m_nStartChar;
					SetSel( m_nStartChar, m_nEndChar );
				}
				else
				{
					::MessageBeep( ( UINT )-1 );
				}
			}
		}
		return;

	case VK_BACK:
		{
			GetWindowText( m_strWindowText ); // refresh

			if ( ( m_nStartChar > 0 ) && 
				 ( m_nStartChar <= m_strLiteral.GetLength( ) ) )
			{
				m_nStartChar--;

				if ( CorrectPosition( m_nStartChar, false ) )
				{
					TCHAR ch = m_chPrompt;
					
					// get the masked literal representation.
					if ( !m_strDefault.IsEmpty( ) )
					{
						ch = m_strDefault.GetAt( m_nStartChar );
					}

					m_strWindowText.SetAt( m_nStartChar, ch );

					m_bRedo = false; 
					m_bModified = true;
					SetWindowText( m_strWindowText );

					m_nEndChar = m_nStartChar;
					SetSel( m_nStartChar, m_nEndChar );
				}
			}
			else 
			{
				::MessageBeep( ( UINT )-1 );
			}
		}
		return;
	}
	
	CXTFlatEdit::OnKeyDown( nChar, nRepCnt, nFlags );
}

void CXTMaskEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if ( CanUseMask( ) == false )
    {
        CXTFlatEdit::OnChar(nChar, nRepCnt, nFlags);
        return;
    }

	switch ( nChar )
	{
	case VK_UP:
	case VK_LEFT:
	case VK_DOWN:
	case VK_RIGHT:
	case VK_HOME:
	case VK_END:
	case VK_DELETE:
	case VK_SPACE:
	case VK_BACK:
		return; // handled in WM_KEYDOWN
	}

	if ( CheckChar( nChar ) == false )
	{
		return;
	}
	
	if ( CanUseMask( ) )
	{
		if ( _istprint( ( TCHAR )nChar ) )
		{
			int iLen = m_strLiteral.GetLength( );
			if ( m_nStartChar >= iLen )
			{
				::MessageBeep( ( UINT )-1 );
				return;
			}

			if ( m_nEndChar >= iLen )
				m_nEndChar = iLen-1;
			
			if ( m_nStartChar != m_nEndChar )
			{
				int i;
				for ( i = m_nStartChar; i < m_nEndChar; ++i )
				{
					if ( m_strLiteral.GetAt( i ) == m_chPrompt )
					{
						DeleteCharAt( m_nStartChar );
					}
				}
			}
			
			SetWindowText( m_strWindowText );

			if ( CorrectPosition( m_nStartChar ) )
			{
				if ( CanOverType( ) )
				{
					if ( m_nStartChar != m_nEndChar )
					{
						InsertCharAt( m_nStartChar, ( TCHAR )nChar );
					}
					else
					{
						m_strWindowText.SetAt( m_nStartChar, ( TCHAR )nChar );
					}
				}
				else
				{
					InsertCharAt( m_nStartChar, ( TCHAR )nChar );
				}
			}

			m_bRedo = false; 
			m_bModified = true;
			SetWindowText( m_strWindowText );

			if (m_nStartChar < iLen )
				m_nStartChar++;
			
			if ( m_nStartChar < iLen )
				CorrectPosition( m_nStartChar );

			m_nEndChar = m_nStartChar;
			SetSel( m_nStartChar, m_nEndChar );

			return;
		}
	}

	CXTFlatEdit::OnChar(nChar, nRepCnt, nFlags);

	// update the window text string.
	GetWindowText( m_strWindowText );
}

bool CXTMaskEdit::CheckChar(UINT& nChar, bool bBeep/*=true*/)
{
	// do not use mask
	if ( !CanUseMask( ) )
	{
		return false;
	}
	
	// control character, OK
	if ( !_istprint( ( TCHAR )nChar ) )
	{
		return true;
	}
	
	// advance to the first character input position.
	int i;
	for ( i = m_nStartChar; i < m_strLiteral.GetLength( ); ++i )
	{
		if ( m_strLiteral.GetAt( i ) == m_chPrompt )
		{
			SetSel( i, i );
			break;
		}
	}
	
	// make sure the string is not longer than the mask
	if ( i >= m_strMask.GetLength( ) )
	{
		if ( bBeep )
		{
			:: MessageBeep( ( UINT )-1 );
		}
		return false;
	}
	
	if ( !ProcessMask( nChar, i ) )
	{
		if ( bBeep )
		{
			:: MessageBeep( ( UINT )-1 );
		}
		return false;
	}

	return true;
}

bool CXTMaskEdit::ProcessMask(UINT& nChar, int nEndPos)
{
	// check the key against the mask
	switch ( m_strMask.GetAt( nEndPos ) )
	{
    case '0':		// digit only //completely changed this
		{
			if ( _istdigit( ( TCHAR )nChar ) ) {
				return true;
			}
			break;
		}
    case '9':		// digit or space
		{
			if ( _istdigit( ( TCHAR )nChar ) ) {
				return true;
			}
			if ( nChar == ' ' ) {
				return true;
			}
			break;
		}
    case '#':		// digit or space or '+' or '-'
		{
			if ( _istdigit( ( TCHAR )nChar ) ) {
				return true;
			}
			if ( nChar == ' ' || nChar == '-' || nChar == '+' ) {
				return true;
			}
			break;
		}
    case 'L':		// alpha only
		{
			if ( _istalpha( ( TCHAR )nChar ) ) {
				return true;
			}
			break;
		}
    case '?':		// alpha or space
		{
			if ( _istalpha( ( TCHAR )nChar ) ) {
				return true;
			}
			if ( nChar == ' ' ) {
				return true;
			}
			break;
		}
    case 'A':		// alpha numeric only
		{
			if ( _istalnum( ( TCHAR )nChar ) ) {
				return true;
			}
			break;
		}
    case 'a':		// alpha numeric or space
		{
			if ( _istalnum( ( TCHAR )nChar ) ) {
				return true;
			}
			if ( nChar == ' ' ) {
				return true;
			}
			break;
		}
    case '&':		// all print character only
		{
			if ( _istprint( ( TCHAR )nChar ) ) {
				return true;
			}
			break;
		}
    case 'H':		// hex digit
		{
			if ( _istxdigit( ( TCHAR )nChar ) ) {
				return true;
			}
			break;
		}
	case 'X':		// hex digit or space
		{
			if ( _istxdigit( ( TCHAR )nChar ) ) {
				return true;
			}
			if ( nChar == ' ' ) {
				return true;
			}
			break;
		}
	case '>':
		{
			if ( _istalpha( ( TCHAR )nChar ) )
			{
				if ( _istlower( ( TCHAR )nChar ) )
				{
					nChar = _toupper( ( TCHAR )nChar );
				}
				return true;
			}
			break;
		}
	case '<':
		{
			if ( _istalpha( ( TCHAR )nChar ) )
			{
				if ( _istupper( ( TCHAR )nChar ) )
				{
					nChar = _tolower( ( TCHAR )nChar );
				}
				return true;
			}
			break;
		}
	}

	return false;
}

void CXTMaskEdit::SetEditMask(LPCTSTR lpszMask, LPCTSTR lpszLiteral, LPCTSTR lpszDefault/*=NULL*/)
{
	ASSERT(lpszMask);
	ASSERT(lpszLiteral);

	// initialize the mask for the control.
	m_strMask    = lpszMask;
	m_strLiteral = lpszLiteral;
	
	if ( lpszDefault == NULL )
	{
		m_strWindowText = lpszLiteral;
		m_strDefault    = lpszLiteral;
	}
	else
	{
		m_strWindowText = lpszDefault;
		m_strDefault    = lpszDefault;
	}

	// set the window text for the control.
	m_bRedo = false; 
	m_bModified = false;
	SetWindowText( m_strWindowText );

	m_strUndoBuffer = m_strWindowText;

	// initialize the font used by the control.
	SetFont( &xtAfxData.font );
}

void CXTMaskEdit::SetPromptChar(TCHAR ch)
{
	m_strLiteral.Replace( m_chPrompt, ch );
	m_strWindowText.Replace( m_chPrompt, ch );

	m_chPrompt = ch;
	SetWindowText( m_strWindowText );
}

void CXTMaskEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CXTFlatEdit::OnLButtonDown(nFlags, point);

    if ( CanUseMask( ) == false )
    {
        return;
    }

	GetSel( m_nStartChar, m_nEndChar );
	CorrectPosition( m_nStartChar, true, false );

	if ( ::GetKeyState( VK_SHIFT ) < 0 )
		m_nEndChar = m_nStartChar;
		
	SetSel( m_nStartChar, m_nEndChar );
}

void CXTMaskEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CXTFlatEdit::OnLButtonDblClk(nFlags, point);

    if ( CanUseMask( ) == false )
    {
        return;
    }

	GetSel( m_nStartChar, m_nEndChar );
	CorrectPosition( m_nStartChar, true, false );
	CorrectPosition( m_nEndChar, true, false );

	SetSel( m_nStartChar, m_nEndChar );
}

void CXTMaskEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CXTFlatEdit::OnSetFocus(pOldWnd);

    if ( CanUseMask( ) == false )
    {
        return;
    }

	GetSel( m_nStartChar, m_nEndChar );
	CorrectPosition( m_nStartChar, true, false );
	
	m_nEndChar = m_nStartChar;
	SetSel( m_nStartChar, m_nEndChar );
}

bool CXTMaskEdit::CopyToClipboard(LPCTSTR lpszText)
{
    if ( !OpenClipboard( ) ) 
        return false;

    ::EmptyClipboard( ); 

	int iLen = _tcslen( lpszText ) + 1;

	HGLOBAL hglbCopy = ::GlobalAlloc( GMEM_MOVEABLE, iLen );

	if ( hglbCopy == NULL )
	{ 
		::CloseClipboard( );
		return false;
	}

	LPTSTR lptstrCopy = ( TCHAR* )::GlobalLock( hglbCopy );
	memcpy( lptstrCopy, lpszText, iLen );
	::GlobalUnlock( hglbCopy ); 

	::SetClipboardData( CF_TEXT, hglbCopy ); 
    
	if ( !::CloseClipboard( ) )
		return false;

	return true;
}

void CXTMaskEdit::MaskCut()
{
	MaskCopy( );
	MaskClear( );
	
	m_bRedo = false; 
	m_bModified = true;
	SetWindowText( m_strWindowText );

	CorrectPosition( m_nStartChar );
	m_nEndChar = m_nStartChar;
	SetSel( m_nStartChar, m_nEndChar );
}

void CXTMaskEdit::MaskCopy()
{
	GetSel( m_nStartChar, m_nEndChar );

	CString strBuffer;
	int i;
	for ( i = m_nStartChar; i < m_nEndChar; ++i )
	{
		if ( m_strLiteral.GetAt( i ) == m_chPrompt )
		{
			if ( m_strWindowText.GetAt( i ) == m_chPrompt )
				continue;
			
			strBuffer += m_strWindowText.GetAt( i );
		}
	}

	CopyToClipboard( strBuffer );
}

void CXTMaskEdit::MaskPaste()
{
	GetSel( m_nStartChar, m_nEndChar );
	MaskClear( );
    
	if ( !OpenClipboard( ) ) 
        return; 
	
	LPTSTR lptstrPaste = NULL;
    HGLOBAL hglbPaste = ::GetClipboardData( CF_TEXT ); 
    if ( hglbPaste != NULL ) 
    { 
        lptstrPaste = ( TCHAR* )::GlobalLock( hglbPaste ); 
        if ( lptstrPaste != NULL ) 
        { 
            ::GlobalUnlock( hglbPaste ); 
        } 
    } 
    
	::CloseClipboard();

	int i, x = m_nStartChar, iLen = _tcslen( lptstrPaste );

	for ( i = 0; i < iLen; ++i )
	{
		UINT ch = lptstrPaste[ i ];

		if ( CheckChar( ch, false ) )
		{
			InsertCharAt( x, ( TCHAR )ch );
			++x;

			if ( x == m_strWindowText.GetLength( ) )
				break;
		}
	}
	
	m_bRedo = false; 
	m_bModified = true;
	SetWindowText( m_strWindowText );

	m_nEndChar = m_nStartChar;
	SetSel( m_nStartChar, m_nEndChar );
}

void CXTMaskEdit::MaskClear()
{
	if ( m_nStartChar == m_nEndChar )
		return;

	// check to see if there is anything left to delete
	int iLength = m_strWindowText.GetLength( );
	
	int i;
	for ( i = m_nStartChar; i < iLength; ++i )
	{
		TCHAR ch1 = m_strLiteral.GetAt( i );
		TCHAR ch2 = m_strWindowText.GetAt( i );
		
		if ( ( ch1 == m_chPrompt ) && ( ch2 != m_chPrompt ) )
		{
			break;
		}
	}
	
	if ( i == iLength )
	{
		::MessageBeep( ( UINT )-1 );
		return;
	}
	
	for ( i = m_nStartChar; i < m_nEndChar; ++i )
	{
		if ( m_strLiteral.GetAt( i ) == m_chPrompt )
		{
			DeleteCharAt( m_nStartChar );
		}
	}
	
	m_bRedo = false; 
	m_bModified = true;
	SetWindowText( m_strWindowText );

	m_nEndChar = m_nStartChar;
	SetSel( m_nStartChar, m_nEndChar );
}

void CXTMaskEdit::MaskUndo( )
{
	GetSel( m_nStartChar, m_nEndChar );

	if ( m_bRedo )
	{
		m_bRedo = false; 
		m_bModified = true;
		SetWindowText( m_strRedoBuffer );
	}
	else
	{
		m_bRedo = true; 
		m_bModified = true;
		GetWindowText( m_strRedoBuffer );
		SetWindowText( m_strUndoBuffer );
	}

	GetWindowText( m_strWindowText );
	m_nEndChar = m_nStartChar;
	SetSel( m_nStartChar, m_nEndChar );
}

void CXTMaskEdit::MaskSelectAll()
{
	m_nStartChar = 0;
	CorrectPosition( m_nStartChar );
	SetSel(m_nStartChar, -1);
}

void CXTMaskEdit::OnEditCut( ) 
{
    if ( CanUseMask( ) == false )
    {
        CEdit::Cut();
    }
    else
    {
	    MaskCut( );
    }
}

void CXTMaskEdit::OnEditCopy( ) 
{
    if ( CanUseMask( ) == false )
    {
        CEdit::Copy();
    }
    else
    {
	    MaskCopy( );
    }
}

void CXTMaskEdit::OnEditPaste( ) 
{
    if ( CanUseMask( ) == false )
    {
        CEdit::Paste();
    }
    else
    {
    	MaskPaste( );
    }
}

void CXTMaskEdit::OnEditClear( ) 
{
    if ( CanUseMask( ) == false )
    {
        CEdit::Clear();
    }
    else
    {
     	MaskClear( );
    }
}

void CXTMaskEdit::OnEditUndo( ) 
{
    if ( CanUseMask( ) == false )
    {
        CEdit::Undo();
    }
    else
    {
    	MaskUndo( );
    }
}

void CXTMaskEdit::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
    if ( CanUseMask( ) == false )
    {
        pCmdUI->Enable( CanUndo( ) );
    }
    else
    {
        pCmdUI->Enable( m_bModified );
    }
}

void CXTMaskEdit::OnEditSelectAll( ) 
{
	MaskSelectAll( );
}

BOOL CXTMaskEdit::PreTranslateMessage(MSG* pMsg) 
{
    if ( CanUseMask( ) == false )
    {
        return CXTFlatEdit::PreTranslateMessage(pMsg);
    }

	// intercept Ctrl+C (copy), Ctrl+V (paste), Ctrl+X (cut) and Ctrl+Z (undo)
	// before CEdit base class gets ahold of them.

	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( ::GetKeyState( VK_SUBTRACT ) < 0 )
		{
			OnChar( '-', 1, 1 );
			return TRUE;
		}
		
		if ( ::GetKeyState( VK_ADD ) < 0 )
		{
			OnChar( '+', 1, 1 );
			return TRUE;
		}
		
		if ( ::GetKeyState( VK_CONTROL ) < 0 )
		{
			switch ( pMsg->wParam )
			{
			case 'X':
			case 'x':
				{
					MaskCut( );
					return TRUE;
				}
				
			case 'C':
			case 'c':
				{
					MaskCopy( );
					return TRUE;
				}
				
			case 'V':
			case 'v':
				{
					MaskPaste( );
					return TRUE;
				}
				
			case 'Z':
			case 'z':
				{
					MaskUndo( );
					return TRUE;
				}
			}
		}
		else
		{
			switch ( pMsg->wParam )
			{
			case '-':
				{
					OnChar( '-', 1, 1 );
					return TRUE;
				}

			case '+':
				{
					OnChar( '+', 1, 1 );
					return TRUE;
				}
			}
		}
	}

	return CXTFlatEdit::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// CXTDateEdit class
/////////////////////////////////////////////////////////////////////////////

CXTDateEdit::CXTDateEdit( )
{
	m_bUseMask		= true;
	m_strMask		= _T("00/00/0000");
	m_strLiteral	= _T("__/__/____");
}

COleDateTime CXTDateEdit::ReadOleDateTime(LPCTSTR lpszData)
{
	COleDateTime dt;
	dt.ParseDateTime(lpszData);
	return dt;
}

IMPLEMENT_DYNAMIC(CXTDateEdit, CXTMaskEdit)

void CXTDateEdit::FormatOleDateTime(CString &strData, COleDateTime dt)
{
	strData = dt.Format( _T("%d/%m/%Y") );
}

void CXTDateEdit::SetDateTime(COleDateTime& dt)
{
	CString strText;
	FormatOleDateTime(strText, dt);
	m_strWindowText = m_strDefault = strText;
	SetWindowText(strText);
}

void CXTDateEdit::SetDateTime(CString strDate)
{
	m_strWindowText = m_strDefault = strDate;
	SetWindowText(strDate);
}

COleDateTime CXTDateEdit::GetDateTime( )
{
	CString strText;
	GetWindowText(strText);
	return ReadOleDateTime(strText);
}

CString CXTDateEdit::GetWindowDateTime( )
{
	CString strText;
	GetWindowText(strText);
	return strText;
}

bool CXTDateEdit::ProcessMask(UINT nChar, int nEndPos)
{
	// check the key against the mask
	switch ( m_strMask.GetAt( nEndPos ) )
	{
    case '0':		// digit only //completely changed this
		{
			bool bReturn = true;

			if ( _istdigit( ( TCHAR )nChar ) )
			{
				if ( nEndPos == 0 )
				{
					if ( nChar > '3' ) {
						bReturn = false;
					}
				}
				else if ( nEndPos == 1 )
				{
					if ( m_strWindowText.GetAt(0) == '3' )
					{
						if ( nChar > '1' ) {
							bReturn = false;
						}
					}
				}
				else if ( nEndPos == 3 )
				{
					if ( nChar > '1' ) {
						bReturn = false;
					}
				}
				else if ( nEndPos == 4 )
				{
					if ( m_strWindowText.GetAt(3) == '1' )
					{
						if ( nChar > '2') {
							bReturn = false;
						}
					}
				}
				return bReturn;
			}
			break;
		}
	}

	MessageBeep((UINT)-1);
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CXTTimeEdit class
/////////////////////////////////////////////////////////////////////////////

CXTTimeEdit::CXTTimeEdit( )
{
	m_bMilitary  = false;
    m_bUseMask   = true;
    m_strMask    = _T("00:00");
    m_strLiteral = _T("__:__");
    m_iHours     = 0;
    m_iMins      = 0;
}

IMPLEMENT_DYNAMIC(CXTTimeEdit, CXTDateEdit)

void CXTTimeEdit::FormatOleDateTime(CString &strData, COleDateTime dt)
{
	if ( dt.m_dt == 0 ) { 
		strData = _T("00:00"); 
	}
	else {
		strData = dt.Format( _T("%H:%M") ); 
	}
}

bool CXTTimeEdit::ProcessMask(UINT nChar, int nEndPos)
{
	// check the key against the mask
	switch ( m_strMask.GetAt( nEndPos ) )
	{
    case '0':		// digit only //completely changed this
		{
			bool bReturn = true;

			if ( _istdigit( ( TCHAR )nChar ) )
			{
				CString strHours;
				strHours.Format( _T( "%02d" ), m_iHours );

				CString strMins;
				strMins.Format( _T( "%02d" ), m_iMins );

				if ( nEndPos == 0 )
				{
					if(nChar > (UINT)strHours[0]) {
						bReturn = false;
					}
				}
				else if ( nEndPos == 1 )
				{
					if(m_strWindowText.GetAt(0) == strHours[0])
					{
						if ( nChar > (UINT)strHours[1] ) {
							bReturn = false;
						}
					}
				}
				else if ( nEndPos == 3 )
				{
					if ( nChar > (UINT)strMins[0] ) {
						bReturn = false;
					}
				}
				else if ( nEndPos == 4 )
				{
					if ( m_strWindowText.GetAt(3) == strMins[0] )
					{
						if ( nChar > (UINT)strMins[1] ) {
							bReturn = false;
						}
					}
				}
				return bReturn;
			}
			break;
		}
	}

	MessageBeep((UINT)-1);
	return false;
}

void CXTTimeEdit::SetHours(int nHours)
{
	m_iHours = nHours;

	CString strText;
	strText.Format( _T("%02d:%02d"), m_iHours, m_iMins );
	SetWindowText( strText );
}

void CXTTimeEdit::SetMins(int nMins)
{
	m_iMins = nMins;
	
	CString strText;
	strText.Format( _T("%02d:%02d"), m_iHours, m_iMins );
	SetWindowText( strText );
}

void CXTTimeEdit::SetTime(int nHours, int nMins)
{
	m_iHours = nHours;
	m_iMins  = nMins;
	
	CString strText;
	strText.Format( _T("%02d:%02d"), m_iHours, m_iMins );
	SetWindowText( strText );
}

/////////////////////////////////////////////////////////////////////////////
// DDX_ routines
/////////////////////////////////////////////////////////////////////////////

_XT_EXT_CLASS void AFXAPI DDX_XTOleDateTime(CDataExchange* pDX, int nIDC, CXTDateEdit& rControl, COleDateTime& rDateTime)
{
	DDX_Control( pDX, nIDC, rControl );
	
	if ( pDX->m_bSaveAndValidate ) {
		rDateTime = rControl.GetDateTime( );
	}
	else {
		rControl.SetDateTime( rDateTime );
	}
}
