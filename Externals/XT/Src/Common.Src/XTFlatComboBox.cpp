// XTFlatComboBox.cpp : implementation of the CXTFlatComboBox class.
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
#include "XTResource.h"
#include "XTMemDC.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"

#ifndef _XTP_INCLUDE_CONTROLS
#include "XTSkins.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EVENT_TIMER   1000

// Returns: true if the window under the mouse cursor is pWnd.
// Summary: used to determine if the window directly under the mouse cursor is 
//          the object pointed to by pWnd.
static bool WndUnderCursor(CWnd* pWnd)
{
    CPoint pt;
    ::GetCursorPos(&pt);

    if (pWnd == CWnd::WindowFromPoint(pt))
    {
        return true;
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////
// CXTFlatComboBox
/////////////////////////////////////////////////////////////////////////////
CXTFlatComboBox::CXTFlatComboBox()
{
	m_bDisableAC   = FALSE;
    m_bFlatLook    = TRUE;
    m_bLBtnDown    = FALSE;
    m_bPainted     = FALSE;
    m_bHasFocus    = FALSE;
    m_bAutoComp    = FALSE;
}

CXTFlatComboBox::~CXTFlatComboBox()
{

}

IMPLEMENT_DYNAMIC(CXTFlatComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CXTFlatComboBox, CComboBox)
    //{{AFX_MSG_MAP(CXTFlatComboBox)
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_CONTROL_REFLECT(CBN_SETFOCUS, OnSetFocus)
    ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillFocus)
	ON_WM_SETCURSOR()
    ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
    ON_CONTROL_REFLECT(CBN_CLOSEUP, OnEndSel)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
END_MESSAGE_MAP()

BOOL CXTFlatComboBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if (message == WM_MOUSEMOVE)
    {
        if (IsFlat())
        {
            CRect rcWindow;
            
            GetWindowRect (&rcWindow);
            
            if (rcWindow.PtInRect(GetCurrentMessage()->pt))
            {
                SetTimer( EVENT_TIMER, 10, NULL );
                OnTimer( EVENT_TIMER );
            }
        }
        else
        {
            KillTimer( EVENT_TIMER );
        }
    }
	
	return CComboBox::OnSetCursor(pWnd, nHitTest, message);
}

void CXTFlatComboBox::DisableFlatLook(BOOL bDisable)
{
    m_bFlatLook = !bDisable;

    if (m_bFlatLook)
    {
        ModifyStyle( WS_BORDER, 0 );
        ModifyStyleEx( WS_EX_CLIENTEDGE|WS_EX_STATICEDGE, 0 );

        SetTimer( EVENT_TIMER, 10, NULL );
        OnTimer( EVENT_TIMER );
    }
    else
    {
        ModifyStyle( 0, WS_BORDER );
        ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

        KillTimer( EVENT_TIMER );
    }

    SetWindowPos(NULL, 0,0,0,0,
        SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
}

void CXTFlatComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if (IsFlat()) {
        m_bLBtnDown = TRUE;
    }

    CComboBox::OnLButtonDown(nFlags, point);
}

void CXTFlatComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (IsFlat())
	{
        m_bLBtnDown = FALSE;
        Invalidate();
    }

    CComboBox::OnLButtonUp(nFlags, point);
}

void CXTFlatComboBox::OnTimer(UINT_PTR nIDEvent)
{
    if (EVENT_TIMER == nIDEvent)
    {
        if (IsFlat())
        {
            CClientDC dc(this);
            
            // OnLButtonDown, show PRESSED.
            if (m_bLBtnDown==TRUE)
            {
                KillTimer( EVENT_TIMER );
                
                if (m_bPainted == TRUE)
                {
                    DrawCombo(&dc, PRESSED);
                    m_bPainted = FALSE;
                }
            }
            
            // If mouse leaves, show flat.
            else if (!PointInRect() && !m_bHasFocus)
            {
                KillTimer( EVENT_TIMER );
                
                if (m_bPainted == TRUE)
                {
                    DrawCombo(&dc, NORMAL);
                    m_bPainted = FALSE;
                }
                
            }
            
            // On mouse over, show RAISED.
            else
            {
                if (m_bPainted == TRUE)
                {
                    return;
                }
                else
                {
                    m_bPainted = TRUE;
                    DrawCombo(&dc, RAISED);
                }
            }
        }
        else
        {
            KillTimer( EVENT_TIMER );
        }
    }
    else
    {
        CComboBox::OnTimer(nIDEvent);
    }
}

BOOL CXTFlatComboBox::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTFlatComboBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Get the client rect.
	CRect r;
	GetClientRect(&r);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, r, xtAfxData.clrWindow);

	// Now let the window do its default painting...
	CComboBox::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	RenderContent( &memDC );
}

// a helper for rendering the control appearance
void CXTFlatComboBox::RenderContent(CDC* pDC)
{
    if ( IsFlat() )
    {
        if ( m_bHasFocus )
		{
            DrawCombo( pDC, RAISED );
        }
        else if ( PointInRect( ) )
		{
            DrawCombo( pDC, WndUnderCursor( this ) ? RAISED : NORMAL );
        }
        else
		{
            DrawCombo( pDC, NORMAL );
        }
    }
}

void CXTFlatComboBox::DrawCombo(CDC* pDC, STATE eState)
{
#ifndef _XTP_INCLUDE_CONTROLS
    // check to see if we can delegate drawing to a proxy
	if (IXTFlatComboProxy* proxy = IXTSkin::GetInstance()->GetFlatComboProxy())
	{
		proxy->DrawCombo( this, pDC, ( eState != NORMAL ) );
		return;
	}
#endif

    CRect rcItem;
    GetClientRect( &rcItem );

    // Cover up dark 3D shadow.
	if ( eState == NORMAL )
	{
		pDC->Draw3dRect( rcItem,
			xtAfxData.clr3DFace,
			xtAfxData.clr3DFace );
	}
	else
	{
		pDC->Draw3dRect( rcItem,
			xtAfxData.clr3DShadow,
			xtAfxData.clr3DHilight );
	}

    rcItem.DeflateRect( 1,1 );
    
    if ( !IsWindowEnabled( ) )
	{
        pDC->Draw3dRect( rcItem,
			xtAfxData.clr3DHilight,
			xtAfxData.clr3DHilight );
    }
    else
	{
        pDC->Draw3dRect( rcItem,
			xtAfxData.clr3DFace,
			xtAfxData.clr3DFace );
    }

    // Cover up dark 3D shadow on drop arrow.
    rcItem.DeflateRect( 1,1 );
    rcItem.left = rcItem.right-xtAfxData.cxHThumb;
    pDC->Draw3dRect( rcItem,
		xtAfxData.clr3DFace,
		xtAfxData.clr3DFace );
    
    // Cover up NORMAL 3D shadow on drop arrow.
    rcItem.DeflateRect( 1,1 );
    pDC->Draw3dRect( rcItem,
		xtAfxData.clr3DFace,
		xtAfxData.clr3DFace );
    
    if ( IsWindowEnabled( ) )
	{
		rcItem.InflateRect( 0,1 );
		switch ( eState )
		{
		case NORMAL:
			pDC->Draw3dRect( rcItem,
				xtAfxData.clr3DHilight,
				xtAfxData.clr3DHilight );
			rcItem.left -= 1;
			pDC->Draw3dRect( rcItem,
				xtAfxData.clr3DHilight,
				xtAfxData.clr3DHilight );
			break;

		case RAISED:
			pDC->Draw3dRect( rcItem,
				xtAfxData.clr3DHilight,
				xtAfxData.clr3DShadow );
			break;

		case PRESSED:
			rcItem.OffsetRect( 1,1 );
			pDC->Draw3dRect( rcItem,
				xtAfxData.clr3DShadow,
				xtAfxData.clr3DHilight );
			break;
		}
    }
}

void CXTFlatComboBox::OnSetFocus()
{
    if (IsFlat())
	{
        m_bHasFocus = TRUE;
        Invalidate();
    }
}

void CXTFlatComboBox::OnKillFocus() 
{
    if (IsFlat())
	{
        m_bHasFocus = FALSE;
        Invalidate();
    }
}

BOOL CXTFlatComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// Make sure that the keystrokes continue to the edit control.
	if ( pMsg->message == WM_KEYDOWN || pMsg->message==WM_KEYUP )
	{
		// if tab, return or escape key, just use default.
		switch ( pMsg->wParam )
		{
		case VK_DELETE:
		case VK_BACK:
			{
				if ( m_bAutoComp )
				{
					m_bDisableAC = ( pMsg->message == WM_KEYDOWN );
				}
				break;
			}
		case VK_TAB:
		case VK_RETURN:
		case VK_ESCAPE:
			{
				return CComboBox::PreTranslateMessage( pMsg );
			}
		}

		// If the combo box has an edit control, don't allow
		// the framework to process accelerators, let the edit
		// control handle it instead.  GetEditSel() will return
		// CB_ERR if there is no edit control present...

		if ( GetEditSel( ) != CB_ERR )
		{
			::TranslateMessage( pMsg );
			::DispatchMessage( pMsg );

			return TRUE;
		}
	}

    return CComboBox::PreTranslateMessage(pMsg);
}

void CXTFlatComboBox::OnEditUpdate() 
{
    // if we are not to auto update the text, get outta here
    if (m_bAutoComp)
	{
		if (m_bDisableAC)
		{
			Default();
		}
		else
		{
			// Get the text in the edit box
			CString strItemTyped;
			GetWindowText(strItemTyped);
			int nLength = strItemTyped.GetLength();

			if (nLength >= 1)
			{
				// Currently selected range
				DWORD dwCurSel = GetEditSel();
				WORD dStart = LOWORD(dwCurSel);
				WORD dEnd   = HIWORD(dwCurSel);
				
				// Search for, and select in, and string in the combo box that is prefixed
				// by the text in the edit box
				if (SelectString(-1, strItemTyped) == CB_ERR)
				{
					SetWindowText(strItemTyped);     // No text selected, so restore what was there before
					if (dwCurSel != CB_ERR) {
						SetEditSel(dStart, dEnd);   //restore cursor postion
					}
				}
				
				// Set the text selection as the additional text that we have added
				if (dEnd < nLength && dwCurSel != CB_ERR) {
					SetEditSel(dStart, dEnd);
				}
				else {
					SetEditSel(nLength, -1);
				}
			}
		}
	}
}

void CXTFlatComboBox::OnEndSel() 
{
    if (IsFlat())
    {
        Invalidate();
    }
}

LRESULT CXTFlatComboBox::OnPrintClient(WPARAM wp, LPARAM )
{
    LRESULT lResult = Default();

    CDC* pDC = CDC::FromHandle((HDC)wp);
    RenderContent(pDC);

    return lResult;
}

/////////////////////////////////////////////////////////////////////////////
// CXTEdit
/////////////////////////////////////////////////////////////////////////////

CXTEdit::CXTEdit()
: m_bInitInCreate(false)
{
}

CXTEdit::~CXTEdit( )
{
}

IMPLEMENT_DYNAMIC(CXTEdit, CEdit)

BEGIN_MESSAGE_MAP(CXTEdit, CEdit)
	//{{AFX_MSG_MAP(CXTEdit)
	ON_WM_CREATE( )
	ON_WM_CONTEXTMENU( )
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_WM_ERASEBKGND( )
	ON_WM_PAINT( )
	ON_WM_INITMENUPOPUP( )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// CXTEdit message handlers

void CXTEdit::Init()
{
	if ( ::IsWindow( m_hWnd ) )
	{
		m_coolMenu.HookWindow( m_hWnd );
		m_coolMenu.LoadToolbar( XT_IDR_TBAR_HEXEDIT );
		
		SetFont( &xtAfxData.font );
	}
}

int CXTEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Hook the cool menu
	if ( m_bInitInCreate )
	{
		Init();
	}

	return 0;
}

void CXTEdit::PreSubclassWindow( ) 
{
	CEdit::PreSubclassWindow( );

	// Hook the cool menu
	if ( !m_bInitInCreate )
	{
		Init();
	}
}

void CXTEdit::OnContextMenu(CWnd*, CPoint point)
{
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		
		point = rect.TopLeft( );
		point.Offset(5, 5);
	}
	
	CMenu menu;
	VERIFY(menu.LoadMenu(XT_IDM_POPUP) );
	
	CMenu* pPopup = menu.GetSubMenu(1);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	
//	while (pWndPopupOwner->GetStyle( ) & WS_CHILD)
//		pWndPopupOwner = pWndPopupOwner->GetParent( );
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
		point.x, point.y, pWndPopupOwner);
}

void CXTEdit::OnEditUndo( ) 
{
	Undo( );
}

void CXTEdit::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanUndo( ) );
}

void CXTEdit::OnEditCut( ) 
{
	Cut( );
}

void CXTEdit::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(SelectionMade( ) );
}

void CXTEdit::OnEditCopy( ) 
{
	Copy( );
}

void CXTEdit::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(SelectionMade( ) );
}

void CXTEdit::OnEditPaste( ) 
{
	Paste( );
}

void CXTEdit::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CanPaste( ) );
}

void CXTEdit::OnEditClear( ) 
{
	Clear( );
}

void CXTEdit::OnUpdateEditClear(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(SelectionMade( ) );
}

void CXTEdit::OnEditSelectAll( ) 
{
	SetSel(0, -1);
}

void CXTEdit::OnUpdateEditSelectAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(GetWindowTextLength( ) > 0);
}

BOOL CXTEdit::CanPaste( )
{
	return ::IsClipboardFormatAvailable(CF_TEXT);
}

BOOL CXTEdit::SelectionMade( )
{
	int nStartChar, nEndChar;
	GetSel( nStartChar, nEndChar );
	return (nStartChar < nEndChar);
}

BOOL CXTEdit::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return TRUE;
}

void CXTEdit::OnPaint( ) 
{
	CPaintDC dc(this); // device context for painting
	
	// Get the client rect.
	CRect r;
	GetClientRect(&r);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, r, xtAfxData.clrWindow);

	// Now let the window do its default painting...
	CEdit::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

void CXTEdit::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	UNREFERENCED_PARAMETER( nIndex );

	Default( );
	
	if (!bSysMenu)
	{
		ASSERT(pPopupMenu != NULL);
		
		// check the enabled state of various menu items
		CCmdUI state;        
		state.m_pMenu = pPopupMenu;
		ASSERT(state.m_pOther == NULL);
		
		state.m_nIndexMax = pPopupMenu->GetMenuItemCount( );
		for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
			 state.m_nIndex++)
		{
			state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
			if (state.m_nID == 0)
				continue; // menu separator or invalid cmd - ignore it
				
			ASSERT(state.m_pOther == NULL);
			ASSERT(state.m_pMenu != NULL);
			if (state.m_nID == (UINT)-1)
			{
				// possibly a popup menu, route to first item of that popup
				state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
				if (state.m_pSubMenu == NULL ||
				    (state.m_nID = state.m_pSubMenu->GetMenuItemID(0) ) == 0 ||
				    state.m_nID == (UINT)-1)
				{				    			 
			        continue; // first item of popup can't be routed to
			    }
			    state.DoUpdate(this, FALSE);  // popups are never auto disabled
			}
			else
			{
				// normal menu item
				// Auto enable/disable if command is _not_ a system command
				state.m_pSubMenu = NULL;
				state.DoUpdate(this, state.m_nID < 0xF000);
			}
		}
	}
}

BOOL CXTEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	m_bInitInCreate = true;

	if (!CEdit::Create(dwStyle, rect, pParentWnd, nID))
		return FALSE;		

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTFlatEdit
/////////////////////////////////////////////////////////////////////////////

CXTFlatEdit::CXTFlatEdit()
{
	m_bFlatLook = TRUE;
    m_bLBtnDown = FALSE;
    m_bHasFocus = FALSE;
}

CXTFlatEdit::~CXTFlatEdit()
{
}

IMPLEMENT_DYNAMIC(CXTFlatEdit, CXTEdit)

BEGIN_MESSAGE_MAP(CXTFlatEdit, CXTEdit)
	//{{AFX_MSG_MAP(CXTFlatEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTFlatEdit message handlers

BOOL CXTFlatEdit::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    if ( message == WM_MOUSEMOVE )
    {
        if (IsFlat())
        {
            CRect rcWindow;
            GetWindowRect( &rcWindow );
            if ( rcWindow.PtInRect( GetCurrentMessage( )->pt ) )
            {
                SetTimer( EVENT_TIMER, 10, NULL );
                OnTimer( EVENT_TIMER );
            }
        }
        else
        {
            KillTimer( EVENT_TIMER );
        }
    }
	
	return CXTEdit::OnSetCursor(pWnd, nHitTest, message);
}

void CXTFlatEdit::DisableFlatLook(BOOL bDisable)
{
    m_bFlatLook = !bDisable;

    if (m_bFlatLook)
    {
        ModifyStyle( WS_BORDER, 0 );
        ModifyStyleEx( WS_EX_CLIENTEDGE|WS_EX_STATICEDGE, 0 );

        SetTimer( EVENT_TIMER, 10, NULL );
        OnTimer( EVENT_TIMER );
    }
    else
    {
        ModifyStyle( 0, WS_BORDER );
        ModifyStyleEx( 0, WS_EX_CLIENTEDGE );

        KillTimer( EVENT_TIMER );
    }

    SetWindowPos(NULL, 0,0,0,0,
        SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
}

void CXTFlatEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if ( IsFlat() )
    {
        m_bLBtnDown = TRUE;
    }

	CXTEdit::OnLButtonDown(nFlags, point);
}

void CXTFlatEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if ( IsFlat() )
	{
        m_bLBtnDown = FALSE;
        Invalidate( );
    }

	CXTEdit::OnLButtonUp(nFlags, point);
}

void CXTFlatEdit::OnTimer(UINT_PTR nIDEvent)
{
    if (EVENT_TIMER == nIDEvent)
    {
        if (IsFlat())
        {
            CClientDC dc(this);
            
            if ( !PointInRect() && !m_bHasFocus )
            {
                KillTimer( EVENT_TIMER );
                DrawEdit( &dc, NORMAL );
            }
            else
            {
                DrawEdit( &dc, RAISED );
            }
        }
        else
        {
            KillTimer( EVENT_TIMER );
        }
    }
    else
    {
	    CXTEdit::OnTimer(nIDEvent);
    }
}

void CXTFlatEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Get the client rect.
	CRect r;
	GetClientRect(&r);
	
	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, r, xtAfxData.clrWindow);
	
	// Now let the window do its default painting...
	CEdit::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	
	RenderContent( &dc );
}

// a helper for rendering the control appearance
void CXTFlatEdit::RenderContent(CDC* pDC)
{
    if ( IsFlat() )
    {
        if ( m_bHasFocus )
		{
            DrawEdit( pDC, RAISED );
        }
        else if ( PointInRect( ) )
		{
            DrawEdit( pDC, WndUnderCursor( this ) ? RAISED : NORMAL );
        }
        else
		{
            DrawEdit( pDC, NORMAL );
        }
	}
}

void CXTFlatEdit::DrawEdit(CDC* pDC, STATE eState)
{
#ifndef _XTP_INCLUDE_CONTROLS
    // check to see if we can delegate drawing to a proxy
	if ( IXTFlatEditProxy* proxy = IXTSkin::GetInstance( )->GetFlatEditProxy( ) )
	{
		proxy->DrawEdit( this, pDC, ( eState == RAISED ) );
		return;
	}
#endif

	CRect rcClient;
	GetClientRect( &rcClient );

    bool bRaised = ( eState == RAISED || m_bHasFocus );
    
    rcClient.InflateRect( 1,1 );
    pDC->Draw3dRect( rcClient,
        IsWindowEnabled( ) ? xtAfxData.clr3DFace : xtAfxData.clrWindow,
        IsWindowEnabled( ) ? xtAfxData.clr3DFace : xtAfxData.clrWindow);
    
    rcClient.InflateRect( 1,1 );
    pDC->Draw3dRect( rcClient,
        bRaised ? xtAfxData.clr3DShadow  : xtAfxData.clr3DFace,
        bRaised ? xtAfxData.clr3DHilight : xtAfxData.clr3DFace );
}

void CXTFlatEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CXTEdit::OnSetFocus(pOldWnd);
	
    if ( IsFlat() )
	{
        m_bHasFocus = TRUE;
        Invalidate( );
    }
}

void CXTFlatEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CXTEdit::OnKillFocus(pNewWnd);
	
    if ( IsFlat() )
	{
        m_bHasFocus = FALSE;
        Invalidate( );
    }
}
