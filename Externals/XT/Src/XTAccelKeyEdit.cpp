// XTAccelKeyEdit.cpp : implementation of the CXTAccelKeyEdit class.
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

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTAccelKeyEdit.h"
#include "XTAccelManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTAccelKeyEdit

CXTAccelKeyEdit::CXTAccelKeyEdit()
{
	Reset( );
}

CXTAccelKeyEdit::~CXTAccelKeyEdit()
{
}

BEGIN_MESSAGE_MAP(CXTAccelKeyEdit, CEdit)
	//{{AFX_MSG_MAP(CXTAccelKeyEdit)
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTAccelKeyEdit message handlers

BOOL CXTAccelKeyEdit::PreTranslateMessage (MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYUP   || pMsg->message == WM_SYSKEYUP   ||
	     pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN )
	{
		bool bKeyDown = ( pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN );

		if ( bKeyDown && IsKeyDefined( ) )
		{
			Reset( );
		}

		switch ( pMsg->wParam )
		{
		case VK_SHIFT:
			{
				if ( !IsKeyDefined( ) )
				{
					m_bShift = bKeyDown;
				}
			}
			break;

		case VK_CONTROL:
			{
				if ( !IsKeyDefined( ) )
				{
					m_bCtrl = bKeyDown;
				}
			}
			break;

		case VK_MENU:
			{
				if ( !IsKeyDefined( ) )
				{
					m_bAlt = bKeyDown;
				}
			}
			break;
			
		case VK_BACK:
			{
				if ( bKeyDown )
				{
					Reset( );
				}
			}
			return TRUE;

		default:
			{
				if ( !IsKeyDefined( ) )
				{
					if ( bKeyDown )
					{
						m_bKeyDefined     = true;
						m_nVirtualKeyCode = ( WORD )pMsg->wParam;
					}
				}
			}
			break;
		}

		UpdateKeyString();
		return TRUE;
	}
	
    return CEdit::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////////////////////
//
void CXTAccelKeyEdit::UpdateKeyString()
{
    CString strKey;

    if ( m_bCtrl ) {
        strKey = _T( "Ctrl+" );
	}
    if ( m_bAlt ) {
        strKey += _T( "Alt+" );
    }
    if ( m_bShift ) {
        strKey += _T( "Shift+" );
    }
    
	CString strKeyName = CXTAccelManager::GetVirtualKeyString( m_nVirtualKeyCode );

	if ( !strKeyName.IsEmpty( ) ) {
		strKey += strKeyName;
	}

	// get the current edit text.
	CString strText;
	GetWindowText( strText );

	// if it does not equal new string update.
	if ( strText != strKey )
	{
		SetWindowText( strKey );

		// get the current selection.
		int iStartChar, iEndChar;
		GetSel( iStartChar, iEndChar );

		// if it does not equal the new selection, update.
		int iSel = strKey.GetLength( );
		if ( iSel != iStartChar && iSel != iEndChar )
		{
			SetSel( iSel,iSel );
		}
	}
}

////////////////////////////////////////////////////////////////////////
//
void CXTAccelKeyEdit::Reset()
{
    m_nVirtualKeyCode = 0;

    m_bCtrl        = false;
    m_bAlt         = false;
    m_bShift       = false;
	m_bKeyDefined  = false;

	if ( ::IsWindow( m_hWnd ) )
	{
		SetWindowText( _T( "" ) );
	}
}

////////////////////////////////////////////////////////////////////////
//
BOOL CXTAccelKeyEdit::GetKeyInfo(WORD& nVirtualKeyCode, BOOL& bCtrl, BOOL& bAlt, BOOL& bShift)
{
	if ( IsKeyDefined( ) )
	{
		nVirtualKeyCode = m_nVirtualKeyCode;
		bAlt            = m_bAlt;
		bCtrl           = m_bCtrl;
		bShift          = m_bShift;
		return TRUE;
	}
	return FALSE;
}


void CXTAccelKeyEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	if ( !IsKeyDefined( ) )
	{
		Reset( );
	}
}

void CXTAccelKeyEdit::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER(nFlags);
	UNREFERENCED_PARAMETER(point);

	// Do not allow right mouse click
	
}
