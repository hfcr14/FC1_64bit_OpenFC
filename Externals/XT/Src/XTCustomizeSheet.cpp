// XTCustomizeSheet.cpp : implementation of the CXTCustomizeSheet class.
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
#include "XTFunctions.h"
#include "XTResource.h"
#include "XTGlobal.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTCheckListBox.h"
#include "XTWndHook.h"
#include "XTTipWindow.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTCommandsListBox.h"
#include "XTCustomizeAPI.h"
#include "XTCallbacks.h"
#include "XTEditListBox.h"
#include "XTMemDC.h"
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrl.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTResizeFormView.h"
#include "XTResizePropertyPage.h"
#include "XTResizePropertySheet.h"
#include "XTAccelManager.h"
#include "XTAccelKeyEdit.h"
#include "XTBrowseEdit.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTCustomizePage.h"
#include "XTCustomizeSheet.h"
#include "XTRegistryManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

typedef CXTCustomizeConnection::ONNEWBAR ONNEWBAR;
typedef CXTCustomizeConnection::ONBARDESTROYED ONBARDESTROYED;


// this hook disables displaying window menu when the user right-clicks on the app's taskbar button
class CXTCustomizeSheetWndHook : public CXTWndHook
{
	bool& m_bAppActive;

public:

	CXTCustomizeSheetWndHook(bool& bAppActive)
		: m_bAppActive(bAppActive)
	{
		HookWindow(AfxGetMainWnd());
	}

	virtual ~CXTCustomizeSheetWndHook()
	{
		HookWindow((HWND)0);
	}

    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case 0x0313:
				// undocumented, sent to display a window menu at a given position (in lParam)
				// do not pass this to the frame to disable displaying window menu
				return 0;

			case WM_ACTIVATEAPP:
				m_bAppActive = wParam != 0;
				break;

		}
		return CXTWndHook::WindowProc(message, wParam, lParam);
	}
	
};

/////////////////////////////////////////////////////////////////////////////
// CXTCustomizeSheet
/////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4355) // OK here

CXTCustomizeSheet::CXTCustomizeSheet(CFrameWnd* pWndParent, DWORD dwCustStyle)
: CXTResizePropertySheet(XT_IDS_CUSTOMIZE, pWndParent)
, m_context(pWndParent)
, m_pPage1(0)
, m_pPage2(0)
, m_pPage3(0)
, m_pPage4(0)
, m_pPage5(0)
, m_bAppActive(true)
, m_pWndParent(pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().

	ASSERT( ( dwCustStyle & CUST_XT_DEFAULT ) != 0 );

	if ( dwCustStyle & CUST_XT_TOOLBARS )
	{
        m_pPage1 = new CXTCustToolBarPage(&m_context);
		AddPage(m_pPage1);
	}
	
	if ( dwCustStyle & CUST_XT_COMMANDS )
	{
        m_pPage2 = new CXTCustCommandsPage(&m_context);
		AddPage(m_pPage2);
	}

	if ( dwCustStyle & CUST_XT_KEYBOARD )
	{
        m_pPage3 = new CXTCustAccelerators(&m_context);
		AddPage(m_pPage3);
	}

	if ( dwCustStyle & CUST_XT_TOOLS )
	{
        m_pPage4 = new CXTCustTools(&m_context);
		AddPage(m_pPage4);
        SetActivePage(m_pPage4);
	}

	if ( dwCustStyle & CUST_XT_OPTIONS )
	{
        m_pPage5 = new CXTCustOptions(&m_context);
		AddPage(m_pPage5);
	}

	m_psh.dwFlags |= PSH_NOAPPLYNOW;

    RevertResourceContext();
}

CXTCustomizeSheet::~CXTCustomizeSheet()
{
    SAFE_DELETE( m_pPage1 );
    SAFE_DELETE( m_pPage2 );
    SAFE_DELETE( m_pPage3 );
    SAFE_DELETE( m_pPage4 );
    SAFE_DELETE( m_pPage5 );
}

IMPLEMENT_DYNAMIC(CXTCustomizeSheet, CXTResizePropertySheet)

BEGIN_MESSAGE_MAP(CXTCustomizeSheet, CXTResizePropertySheet)
	//{{AFX_MSG_MAP(CXTCustomizeSheet)
	ON_WM_DESTROY()
	ON_WM_NCACTIVATE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDCANCEL, OnCloseBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTCustomizeSheet::OnInitDialog() 
{
	// initialize the context
	SetCustMode(true);

	// collect info about known customizable bars
	m_map.RemoveAll();
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->GetCustomBarInfo(STORECUSTOMINFO(this, &CXTCustomizeSheet::StoreInfo));
	connection->OnNewBar.Add(this, &CXTCustomizeSheet::StoreInfo);
	connection->OnBarDestroyed.Add(this, &CXTCustomizeSheet::OnBarDestroyed);

	BOOL bResult = CXTResizePropertySheet::OnInitDialog();

	// Remove the OK button.
	CWnd* pWnd = GetDlgItem( IDOK );
	if ( pWnd && ::IsWindow( pWnd->m_hWnd ) )
	{
		pWnd->ShowWindow( SW_HIDE );
	}

	// Rename the cancel button to 'Close"
	pWnd = GetDlgItem( IDCANCEL );
	if ( pWnd && ::IsWindow( pWnd->m_hWnd ) )
	{
		CString string;
		string.LoadString( XT_IDS_CLOSE );
		pWnd->SetWindowText( string );
	}

	AutoLoadPlacement( _T( "CXTCustomizeSheet" ) );

    CXTRegistryManager regMgr;
    int iActivePage = regMgr.GetProfileInt(_T("Settings"), _T("Active Page"), 0);
    SetActivePage( iActivePage );

	return bResult;
}

void CXTCustomizeSheet::StoreInfo(CXTCustomControlBarInfo* pInfo)
{
	if (HWND hwnd = pInfo->m_pBar->GetSafeHwnd())
	{
		m_map.SetAt(hwnd, true);
	}
}

void CXTCustomizeSheet::OnBarDestroyed(CControlBar* pBar)
{
	if (HWND hwnd = pBar->GetSafeHwnd())
	{
		m_map.RemoveKey(hwnd);
	}
}

void CXTCustomizeSheet::SetCustMode(bool bMode)
{
	CXTCustomizeConnection::GetInstance()->SetCustMode(bMode);
}

void CXTCustomizeSheet::OnDestroy() 
{
	SetCustMode(false);
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->Remove(this);

	int iActivePage = GetActiveIndex( );

    CXTRegistryManager regMgr;
    regMgr.WriteProfileInt( _T( "Settings" ), _T( "Active Page" ), iActivePage );

	CXTResizePropertySheet::OnDestroy();
}

// A hook used in customization sheet to filter keyboard/mouse events
class CXTCustomizeHook
{
public:
	typedef CXTDelegate<bool, LPARAM> FILTER;

private:
	HHOOK m_hHook;
	FILTER Filter;

public:

	CXTCustomizeHook(const FILTER& Filter)
	: Filter(Filter)
	, m_hHook(0)
	{ }


	~CXTCustomizeHook()
	{
		Unhook();
	}

	void Hook(int idHook, HOOKPROC lpfn)
	{
		ASSERT(m_hHook == 0);
		m_hHook = ::SetWindowsHookEx(idHook, lpfn, NULL, ::GetCurrentThreadId());
	}

	void Unhook()
	{
		if (m_hHook)
		{
			::UnhookWindowsHookEx(m_hHook);
			m_hHook = 0;
		}
	}

	LRESULT HandleEvent(int nCode, WPARAM wParam, LPARAM lParam)
	{
		bool bProcess = (nCode >= 0);
		if (bProcess)  
		{
			bProcess = Filter(lParam);
		}
		LRESULT result;
		if (bProcess)
		{
			// process normally
			result = CallNextHookEx(m_hHook, nCode, wParam, lParam); 
		}
		else
		{
			// eat the message
			result = true;			
		}
		return result;
	}    
};
typedef CXTCustomizeHook::FILTER FILTER;

// A combination of keyboard/mouse hooks
class CXTCustomizeHooks
{
private:

	CXTCustomizeHook m_mouseHook;
	CXTCustomizeHook m_keyHook;
	static CXTCustomizeHooks* m_pThis;

public:
	CXTCustomizeHooks(const FILTER& FilterMouse, const FILTER& FilterKey)
	: m_mouseHook(FilterMouse)
	, m_keyHook(FilterKey)
	{ 
		ASSERT(m_pThis == 0);  // one instance at a time
		m_pThis = this;
	}

	~CXTCustomizeHooks()
	{
		Unhook();
		m_pThis = 0;
	}

	void Hook()
	{
		m_mouseHook.Hook(WH_MOUSE, HOOKPROC(MouseProc));
		m_keyHook.Hook(WH_KEYBOARD, HOOKPROC(KeyProc));
	}

	void Unhook()
	{
		m_mouseHook.Unhook();
		m_keyHook.Unhook();
	}

	static LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		return m_pThis->m_mouseHook.HandleEvent(nCode, wParam, lParam);
	}

	static LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam)
	{
		return m_pThis->m_keyHook.HandleEvent(nCode, wParam, lParam);
	}
};
CXTCustomizeHooks* CXTCustomizeHooks::m_pThis = 0;

INT_PTR CXTCustomizeSheet::DoModal()
{
	ASSERT_VALID(this);
	ASSERT(m_hWnd == NULL);

	// register common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
#if _MSC_VER >= 1200 // MFC 6.0
	AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);
#endif //_MSC_VER >= 1200

	// finish building PROPSHEETHEADER structure
	BuildPropPageArray();

	// allow OLE servers to disable themselves
	CWinApp* pApp = AfxGetApp();
	if (pApp != NULL)
		pApp->EnableModeless(FALSE);

	// find parent HWND
	HWND hWndTop;
	CWnd* pParentWnd = CWnd::GetSafeOwner(m_pWndParent, &hWndTop);
	HWND hWndParent = pParentWnd->GetSafeHwnd();
	
#if _MSC_VER != 1200 // !MFC 6.0
	PROPSHEETHEADER* psh = &m_psh;
#else
	AFX_OLDPROPSHEETHEADER* psh = GetPropSheetHeader();
#endif //_MSC_VER != 1200

	psh->hwndParent = hWndParent;
	CXTCustomizeHooks hooks(FILTER(this, &CXTCustomizeSheet::FilterMouse),
						    FILTER(this, &CXTCustomizeSheet::FilterKey));
	if (hWndParent)
	{
		hooks.Hook();
	}
	HWND hWndCapture = ::GetCapture();
	if (hWndCapture != NULL)
		::SendMessage(hWndCapture, WM_CANCELMODE, 0, 0);

	// setup for modal loop and creation
	m_nModalResult = 0;
	m_nFlags |= WF_CONTINUEMODAL;

	// hook for creation of window
	AfxHookWindowCreate(this);
	psh->dwFlags |= PSH_MODELESS;
	m_nFlags |= WF_CONTINUEMODAL;
	HWND hWnd = (HWND)::PropertySheet((PROPSHEETHEADER*)psh);
#ifdef _DEBUG
	DWORD dwError = ::GetLastError();
#endif
	psh->dwFlags &= ~PSH_MODELESS;
	AfxUnhookWindowCreate();

	// handle error
	if (hWnd == NULL || hWnd == (HWND)-1)
	{
		TRACE1("PropertySheet() failed: GetLastError returned %d\n", dwError);
		m_nFlags &= ~WF_CONTINUEMODAL;
	}

	int nResult = m_nModalResult;
	if (ContinueModal())
	{
		// enter modal loop
		DWORD dwFlags = MLF_SHOWONIDLE;
		if (GetStyle() & DS_NOIDLEMSG)
			dwFlags |= MLF_NOIDLEMSG;
		CXTCustomizeSheetWndHook hook(m_bAppActive);  // disables window menu from taskbar
		nResult = RunModalLoop(dwFlags);
	}

	// hide the window before enabling parent window, etc.
	if (m_hWnd != NULL)
	{
		SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);
	}
	hooks.Unhook();
	bool bActivateParent = false;
	if (hWndParent != NULL && ::GetActiveWindow() == m_hWnd)
	{
		bActivateParent = true;;		
	}

	// cleanup
	DestroyWindow();

	if (bActivateParent)
	{
		::SetActiveWindow(hWndParent);
	}

	// allow OLE servers to enable themselves
	if (pApp != NULL)
		pApp->EnableModeless(TRUE);

	return nResult;
}

static bool IsDescendant(HWND hwndParent, HWND hwnd)
{
	while (hwnd)
	{
		if (hwndParent == hwnd)
		{
			return true;
		}
		hwnd = ::GetParent(hwnd);
	}
	return false;
}

bool CXTCustomizeSheet::FilterMouse(LPARAM lParam)
{
	LPMOUSEHOOKSTRUCT pMsg = (LPMOUSEHOOKSTRUCT)(LPVOID)lParam;

	if (!m_bAppActive)
	{
		m_bAppActive = true;
		AfxGetMainWnd()->BringWindowToTop();
		AfxGetMainWnd()->ShowOwnedPopups();
	}

	if (IsDescendant(AfxGetMainWnd()->m_hWnd, pMsg->hwnd))
	{
		if (IsDescendant(m_hWnd, pMsg->hwnd))
		{
			return true;
		}

		HWND hwnd;
		for (hwnd = pMsg->hwnd; hwnd; hwnd = ::GetParent(hwnd))
		{
			bool bValue;
			if (m_map.Lookup(hwnd, bValue))
			{
				return true;
			}
		}
		
		// also allow frames of the floating toolbars
		if (POSITION pos = m_map.GetStartPosition())
		{
			do
			{
				HWND hwnd;
				bool bValue;
				m_map.GetNextAssoc(pos, hwnd, bValue);
				CControlBar* pBar = (CControlBar*)CWnd::FromHandlePermanent(hwnd);
				if (pBar->GetBarStyle() & CBRS_FLOATING)
				{
					if (pBar->GetParentFrame()->GetSafeHwnd() == pMsg->hwnd)
					{
						return true;
					}
				}
			}
			while(pos != 0);
		}
		// eat this message
		return false;
	}
	return true;
}

bool CXTCustomizeSheet::FilterKey(LPARAM)
{
	// allow keyboard events only if current focus window belongs to this one
	if (IsDescendant(m_hWnd, ::GetFocus()))
	{
		return true;
	}

	// pass thru if the capture is set (toolbar tracking does this)
	if (::GetCapture() != 0)
	{
		return true;
	}

	// eat this message
	return false;
}

BOOL CXTCustomizeSheet::OnNcActivate(BOOL bActive) 
{
	// always show this window as active
	if (!bActive)
	{
		// allow default processing if this window is disabled
		// (another dialog is trying to enter modal state)
		return !IsWindowEnabled();
	}
	return true;
}

void CXTCustomizeSheet::OnSysCommand(UINT nID, LPARAM lParam) 
{
	if ( CXTToolsManager::Get().IsInitialized() && nID == SC_CLOSE )
	{
		if ( m_pPage4 && ::IsWindow( m_pPage4->m_hWnd ) && !m_pPage4->ValidateTools( ) )
		{
			return;
		}
	}
	
	CXTResizePropertySheet::OnSysCommand(nID, lParam);
}

void CXTCustomizeSheet::OnCloseBtn() 
{
	if ( CXTToolsManager::Get().IsInitialized() )
	{
		if ( m_pPage4 && ::IsWindow( m_pPage4->m_hWnd ) && !m_pPage4->ValidateTools( ) )
		{
			return;
		}
	}
	
	EndDialog( IDCANCEL );
}
