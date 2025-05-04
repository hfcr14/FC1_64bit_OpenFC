// XTBrowseEdit.cpp : implementation of the CXTBrowseEdit class.
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
#include "XTButton.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTBrowseEdit.h"
#include "XTBrowseDialog.h"
#include "XTPriv.h"
#include "XTOSVersionInfo.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CX_BUTTON = ( ::GetSystemMetrics( SM_CXHTHUMB ) + 
                      ( ::GetSystemMetrics( SM_CXEDGE ) * 2 ) );

/////////////////////////////////////////////////////////////////////////////

class CXTBrowseBtn : public CXTButton
{
	DECLARE_DYNAMIC(CXTBrowseBtn)

public:

	CXTBrowseBtn()
	{
        m_nBorderGap = 0;    
        m_nImageGap  = 0;
        m_dwBStyle   = NULL;
        m_pEditWnd   = NULL;
        m_dwxStyle   = BS_XT_SHOWFOCUS;
        
        // Load the image list used by control bar frame buttons.
        HIMAGELIST hImageList = ImageList_LoadBitmap( xtAfxData.hInstance, 
            MAKEINTRESOURCE( XT_IDB_SCROLL_ARW ), 11, 1, RGB( 0xFF,0x00,0xFF ) );
        
        if ( hImageList != NULL )
        {
            m_hIconHEnabled = ImageList_ExtractIcon(
                hInstance, hImageList, 4);
            ASSERT(m_hIconHEnabled != NULL);
            
            // Free image list.
            ImageList_Destroy(hImageList);
            hImageList = NULL;
        }
	}
	
    virtual ~CXTBrowseBtn()
	{
	}

protected:
	
	DWORD   		m_dwBStyle;		 // Search type
	UINT			m_nID;			 // Resource id for push button
	UINT			m_nMenu;		 // Popup menu id
	UINT			m_nSubMenuIndx;	 // Index of popup submenu.
	CXTBrowseEdit*	m_pEditWnd;		 // Parent edit control.
    CXTIconHandle   m_hIconHEnabled; // Horz button icon enabled

public:

    BOOL CreateBrowseButton(UINT nID)
    {
        XT_MANAGE_STATE;  // Switch to toolkit resources/module state
        
        CString string;
        string.LoadString(XT_IDS_DOT);

        DWORD dwStyle = (m_pEditWnd->GetStyle() & WS_VISIBLE) | 
            WS_CHILD | WS_TABSTOP | BS_CENTER | BS_VCENTER;

        if( !CXTButton::Create(string, dwStyle, CRect(0,0,0,0),
			m_pEditWnd->GetParent(), nID))
        {
            TRACE0( "Failed to create browse edit button.\n" );
            return FALSE;
        }
        
        // Set the font for this control.
        SetFont(&xtAfxData.font);
        ModifyStyle(0, BS_OWNERDRAW);
        
        // set the tab order and enable the window based on parent's state.
        SetWindowPos( m_pEditWnd, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE );
        EnableWindow( m_pEditWnd->IsWindowEnabled( ));

        return TRUE;
    }

    BOOL CreatePopupButton(UINT nID)
    {
        DWORD dwStyle = (m_pEditWnd->GetStyle() & WS_VISIBLE) | 
            WS_CHILD | WS_TABSTOP | BS_CENTER | BS_VCENTER | BS_ICON;

        if( !CXTButton::Create(NULL, dwStyle, CRect(0,0,0,0),
			m_pEditWnd->GetParent(), nID))
        {
            TRACE0( "Failed to create browse edit button.\n" );
            return FALSE;
        }
        
        // set the arrow icon that is displayed on the button.
        SetIcon(CSize(10,10), m_hIconHEnabled);
        ModifyStyle(0, BS_OWNERDRAW);
        
        // set the tab order and enable the window based on parent's state.
        SetWindowPos( m_pEditWnd, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE );
        EnableWindow( m_pEditWnd->IsWindowEnabled( ));

        return TRUE;
    }
    
    virtual BOOL Create(CXTBrowseEdit* pEditWnd, DWORD dwBStyle, UINT nID, UINT nMenu=-1, UINT nSubMenuIndx=-1)
    {
        XT_MANAGE_STATE;  // Switch to toolkit resources/module state
        
        ASSERT_VALID(pEditWnd);				 // Must be valid
        ASSERT_VALID(pEditWnd->GetParent());
        
        // Set the pointers to the edit and edit parent windows.
        m_pEditWnd = pEditWnd;
        
        // Set the browse style and menu resource 
        // ID information (if any).
        m_dwBStyle		= dwBStyle;
        m_nMenu			= nMenu;
        m_nSubMenuIndx	= nSubMenuIndx;
        
        switch (m_dwBStyle & (BES_XT_CHOOSEDIR | BES_XT_CHOOSEFILE | BES_XT_POPUPMENU))
        {
        // If this is a browse for directory or file control.
        case BES_XT_CHOOSEDIR:
        case BES_XT_CHOOSEFILE:
            CreateBrowseButton(nID);
			break;

        // If this is a popup menu control.
        case BES_XT_POPUPMENU:
            CreatePopupButton(nID);
			break;

        // Style was not defined return FALSE.
		default:
			return FALSE;
        }

        return TRUE;
	}

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if ( pMsg->message == WM_KEYDOWN ) // activate if space or enter key pressed...
		{
			if ( pMsg->wParam == VK_RETURN ||
				 pMsg->wParam == VK_SPACE )
			{
				OnClicked();
				return TRUE; // eat message...
			}
		}

		return CXTButton::PreTranslateMessage(pMsg);
	}

	afx_msg void OnKillFocus(CWnd* pNewWnd)
	{
		CXTButton::OnKillFocus(pNewWnd);
		
		// NOTE: Future bug fix should come here -
		// for this is a slave of a 'temporary' edit box, we should return the
		// focus to that box (i.e. in a CXTBrowseDialog).
		/*
		if (pNewWnd != m_pEditWnd)
		{
			m_pEditWnd->SetFocus();
		}
		*/
	}
    
    afx_msg void OnClicked()
    {
        m_pEditWnd->OnBrowse();
    }
    
	DECLARE_MESSAGE_MAP()

	friend class CXTBrowseEdit;
};

IMPLEMENT_DYNAMIC(CXTBrowseBtn, CXTButton)

BEGIN_MESSAGE_MAP(CXTBrowseBtn, CXTButton)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTBrowseEdit class
/////////////////////////////////////////////////////////////////////////////

CXTBrowseEdit::CXTBrowseEdit()
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state
	
    m_nGap            = ::GetSystemMetrics(SM_CXEDGE);
	m_dwBStyle        = BES_XT_CHOOSEDIR;
	m_nMenu	          = 0;
	m_nSubMenuIndx	  = 0;
	m_bOpenFileDialog = TRUE;
	m_strFileName	  = _T("");
	m_dwFlags         = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_EXPLORER;
	m_pParentWnd	  = NULL;
	m_pFileDialog	  = NULL;
	m_pBrowseBtn      = NULL;
	m_dwInitSignature = 1;
	m_bBrowsing       = false;

	m_strDefExt.LoadString(XT_IDS_DEFEXT);
	m_strFilter.LoadString(XT_IDS_DEFFILT);
	m_strTitle.LoadString(XT_IDS_DEFTITLE);
}

CXTBrowseEdit::~CXTBrowseEdit()
{
    SAFE_DELETE( m_pBrowseBtn );
}

IMPLEMENT_DYNAMIC(CXTBrowseEdit, CXTEdit)

BEGIN_MESSAGE_MAP(CXTBrowseEdit, CXTEdit)
	//{{AFX_MSG_MAP(CXTBrowseEdit)
	ON_WM_ENABLE()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitControl)
END_MESSAGE_MAP()

void CXTBrowseEdit::OnEnable(BOOL bEnable) 
{
	CXTEdit::OnEnable(bEnable);

	if (m_pBrowseBtn && ::IsWindow(m_pBrowseBtn->m_hWnd))
	{
		m_pBrowseBtn->EnableWindow(bEnable);
	}
}

void CXTBrowseEdit::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CXTEdit::OnShowWindow( bShow, nStatus );
	
	if (m_pBrowseBtn && ::IsWindow(m_pBrowseBtn->m_hWnd))
	{
		m_pBrowseBtn->ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	}
}

LRESULT CXTBrowseEdit::OnInitControl(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	
	if ((DWORD)lParam == m_dwInitSignature)
	{
		// note init completed
		m_dwInitSignature = 0;

		// Create the browse button associated with the browse
		// edit control.
		if (m_dwBStyle & (BES_XT_CHOOSEDIR|BES_XT_CHOOSEFILE|BES_XT_POPUPMENU))
		{
            m_pBrowseBtn = new CXTBrowseBtn;
            ASSERT(m_pBrowseBtn != NULL);

			VERIFY(m_pBrowseBtn->Create(this, m_dwBStyle, 
				XT_IDC_BTN_BROWSE, m_nMenu, m_nSubMenuIndx));

            // move browse button.
            PositionBrowseButton(true);
		}
		
		SetFont(&xtAfxData.font);
		m_pParentWnd = this;
	}

	return 0;
}

void CXTBrowseEdit::PreSubclassWindow() 
{
	CXTEdit::PreSubclassWindow();

	// Initialize the control.
	DeferInitialUpdate();
}

void CXTBrowseEdit::DeferInitialUpdate()
{
	if (m_dwInitSignature > 0)
	{
		PostMessage(XTWM_INITIAL_UPDATE, 0, ++m_dwInitSignature);
	}
}

int CXTBrowseEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	DeferInitialUpdate();
	
	return 0;
}

void CXTBrowseEdit::ChooseDirectory()
{
    // Get the current (if any) text that is displayed in
    // the edit control, this will be the initial path for the
    // browse dialog to start from.
    CString strPath;
    GetWindowText(strPath);
    
    // Instantiate a browse for folder dialog object
    CXTBrowseDialog browseDlg;
    browseDlg.SetOwner(GetParent()->GetSafeHwnd());
    browseDlg.SetTitle((TCHAR*)(LPCTSTR)m_strTitle);
    browseDlg.SetSelPath((TCHAR*)(LPCTSTR)strPath);
    
    if( browseDlg.DoModal() == IDOK ) {
        SetWindowText( browseDlg.GetSelPath() );
    }
}

void CXTBrowseEdit::ChooseFile()
{
    if ( m_pFileDialog )
    {
        if ( m_pFileDialog->DoModal( ) == IDOK )
            SetWindowText( m_pFileDialog->GetPathName( ) );
    }
    else
    {
        CFileDialog dlg( m_bOpenFileDialog, m_strDefExt, m_strFileName, 
            m_dwFlags, m_strFilter, m_pParentWnd );
        
        // Check to see if this is Windows 2000 or later, if so use the
        // Windows 2000 version of OPENFILENAME.

        if (CXTOSVersionInfo::Get().IsWin2KOrGreater() && sizeof(OPENFILENAME) < 88 && dlg.m_ofn.lStructSize < 88)
        {
            // Windows 2000 version of OPENFILENAME has three extra members,
            // this was copied from newer version of commdlg.h...

            struct OPENFILENAMEEX
            { 
                void*  pvReserved; // 4 bytes
                DWORD  dwReserved; // 4 bytes
                DWORD  FlagsEx;    // 4 bytes
            };

            dlg.m_ofn.lStructSize += sizeof(OPENFILENAMEEX); // should equal an additional 12 bytes;
        }
        
        if( dlg.DoModal() == IDOK )
        {
            SetWindowText( dlg.GetPathName( ));
        }
    }
}

void CXTBrowseEdit::PopupMenu()
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

    ASSERT(m_nMenu != 0);
    if (m_pBrowseBtn->GetSafeHwnd())
        m_pBrowseBtn->SetState(TRUE);
    
    CRect rect;
    m_pBrowseBtn->GetWindowRect(&rect);
    
    XT_UNDO_STATE; // loading a user resource...
    CXTMenu menu;
    VERIFY(menu.LoadMenu(m_nMenu));
    XT_REDO_STATE;
    
    CXTMenu* pPopup = menu.GetSubMenu(m_nSubMenuIndx);
    ASSERT(pPopup != NULL);
    
	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();

	CXTCoolMenu menuHook;
	menuHook.HookWindow( pWndPopupOwner );
	
    pPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
        rect.right, rect.top, pWndPopupOwner, &rect );
	
    // Return the button state to normal.
    if (m_pBrowseBtn->GetSafeHwnd())
        m_pBrowseBtn->SetState(FALSE);
}

void CXTBrowseEdit::OnBrowse()
{
	m_bBrowsing = true;

	// If this is a browse directory control.
    switch (m_dwBStyle & (BES_XT_CHOOSEDIR | BES_XT_CHOOSEFILE | BES_XT_POPUPMENU))
    {
    case BES_XT_CHOOSEDIR:
        ChooseDirectory();
        break;
        
    case BES_XT_CHOOSEFILE:
        ChooseFile();
        break;

    case BES_XT_POPUPMENU:
        PopupMenu();
        break;
    }

	SetSel(0,-1);
	SetFocus();

	m_bBrowsing = false;
}

void CXTBrowseEdit::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
    if (m_pBrowseBtn && ::IsWindow(m_pBrowseBtn->m_hWnd))
	{
        lpwndpos->cx -= CX_BUTTON + m_nGap;
	}

	CXTEdit::OnWindowPosChanging(lpwndpos);
}

void CXTBrowseEdit::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CXTEdit::OnWindowPosChanged(lpwndpos);
    
    // move browse button.
    PositionBrowseButton();
}

void CXTBrowseEdit::PositionBrowseButton(bool bSizeEdit/*=false*/)
{
    // if the browse button exists, move it to the correct
    // location in relation to the browse edit window.
    if (m_pBrowseBtn && ::IsWindow(m_pBrowseBtn->m_hWnd))
    {
        CWnd* pParentWnd = GetParent();
        ASSERT_VALID(pParentWnd);

        CRect r;
        GetWindowRect(&r);
        pParentWnd->ScreenToClient(&r);

        if (bSizeEdit)
        {
            // call MoveWindow, this causes WM_WINDOWPOSCHANGING to
            // get called which will adjust the size of the browse edit window.
            // WM_WINDOWPOSCHANGED is then called which moves the browse button.

			::SetWindowPos( m_hWnd, NULL, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED|SWP_NOREDRAW );
        }
        else
        {
            // move the browse button.
            r.right += CX_BUTTON + m_nGap;
            r.left   = r.right - CX_BUTTON;
            
			// set the window position and make sure tab order is correct.
			::SetWindowPos( m_pBrowseBtn->m_hWnd, m_hWnd, r.left, r.top,
				r.Width(), r.Height(), SWP_FRAMECHANGED|SWP_NOREDRAW );
        }

		// Refresh the parent window.
		pParentWnd->RedrawWindow( NULL, NULL,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );
    }
}

/////////////////////////////////////////////////////////////////////////////
// CXTItemEdit class
/////////////////////////////////////////////////////////////////////////////

CXTItemEdit::CXTItemEdit(CWnd* pParent, const CRect& rect, CString& strWindowText, DWORD dwBStyle/*=BES_XT_CHOOSEDIR*/, bool bAutoDelete/*=true*/)
: m_strWindowText(strWindowText)
{
	m_bInitInCreate = true;

	// Initialize defaults
    m_nGap        = 0;
	m_bAutoDelete = bAutoDelete;
	m_bEscapeKey  = false;
	m_bModified   = false;  
	m_dwBStyle    = dwBStyle;

	// Create the edit control.
	VERIFY(Create( WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL,
		rect, pParent, XT_IDC_LBOX_EDIT ));

	// Set the font, window text and select it.
	SetFont(&xtAfxData.font);
	SetWindowText(m_strWindowText);
	SetSel(0,-1);
	SetFocus();

	m_pParentWnd   = this;
	m_bClosePosted = false;
}

CXTItemEdit::~CXTItemEdit()
{
}

IMPLEMENT_DYNAMIC(CXTItemEdit, CXTBrowseEdit)

BEGIN_MESSAGE_MAP(CXTItemEdit, CXTBrowseEdit)
	//{{AFX_MSG_MAP(CXTItemEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTItemEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CXTBrowseEdit::OnKillFocus(pNewWnd);

	if (!m_pBrowseBtn->GetSafeHwnd() || m_pBrowseBtn != pNewWnd)
	{
		ASSERT_VALID(this);
		EndLabelEdit();
	}
}

void CXTItemEdit::PostNcDestroy() 
{
	if (m_bAutoDelete) {
		delete this;
	}
}

BOOL CXTItemEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		if ((pMsg->wParam == VK_RETURN) ||
			(pMsg->wParam == VK_ESCAPE))
		{
			if (pMsg->wParam == VK_ESCAPE) {
				m_bEscapeKey = true;
			}
			
			EndLabelEdit();
			return TRUE;
		}
	}

	return CXTBrowseEdit::PreTranslateMessage(pMsg);
}

void CXTItemEdit::EndLabelEdit()
{
	if (m_bClosePosted) return;

	if (::IsWindow( m_pBrowseBtn->GetSafeHwnd() ))
		m_pBrowseBtn->PostMessage( WM_CLOSE );
	
	if (::IsWindow( GetSafeHwnd() ))
		PostMessage( WM_CLOSE );

	m_bClosePosted = true;

	if (m_bModified == false)
	{
		CWnd* pOwner = GetOwner();
		ASSERT_VALID(pOwner);

		if (m_bEscapeKey == false)
		{
			m_bModified = true;

			// get the window text and pass it on to the
			// notification window...
			GetWindowText(m_strWindowText);

			// Notify of label edit.
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(),
				BEN_XT_LABELEDITEND), (LPARAM)m_hWnd );
		}
		else
		{
			// Notify of label edit.
			pOwner->SendMessage( WM_COMMAND, MAKEWPARAM( GetDlgCtrlID(),
				BEN_XT_LABELEDITCANCEL), (LPARAM)m_hWnd );
		}
	}
}

void CXTItemEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_RETURN) ||
		(nChar == VK_ESCAPE))
	{
		if (nChar == VK_ESCAPE)
			m_bEscapeKey = TRUE;

		EndLabelEdit();
		return;
	}
	
	CXTBrowseEdit::OnChar(nChar, nRepCnt, nFlags);
}
