// XTMenuBar.cpp : implementation of the CXTMenuBar class.
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
#include "XTFunctions.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTMenuBar.h"
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTMiniDockFrameWnd.h"
#include "XTCoolMenu.h"
#include "XTMemDC.h"
#include "XTWndShadow.h"
#include "XTPriv.h"
#include "XTHelpers.h"
#include "XTOSVersionInfo.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"

#if defined(_AFXDLL) || (_MSC_VER < 1200) //vc5
#define COMPILE_MULTIMON_STUBS
#endif

#pragma warning( disable : 4706 ) 
#include <multimon.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IS_HOTITEM(pItem) ((m_iHotItem >= 0) && pItem == m_arrItems[m_iHotItem])

// callback to track the window menu popup
typedef void (CXTMenuBar::*TRACKWINDOWMENU_CB)(CXTSysMenuBarItem* pItem);
typedef void (CXTMenuBar::*RENDERWINDOWMENU_CB)(CDC* pDC, CXTSysMenuBarItem* pItem);

// callback to track the sub-menu popup
typedef void (CXTMenuBar::*TRACKSUBMENU_CB)(CXTSubMenuBarItem* pItem);
typedef void (CXTMenuBar::*RENDERSUBMENU_CB)(CDC* pDC, CXTSubMenuBarItem* pItem);
typedef void (CXTMenuBar::*RENDERCONTROL_CB)(CDC* pDC, CXTControlMenuBarItem* pItem);

CXTMenuBarItem::CXTMenuBarItem(CXTMenuBar* pMenuBar)
: m_ptOrigin(0)
, m_pMenuBar(pMenuBar)
, m_bWrapped(false)
{

}

// Descriptor of a sub-menu item
class CXTSubMenuBarItem : public CXTMenuBarItem
{
	RENDERSUBMENU_CB const m_pfnDraw;	
	TRACKSUBMENU_CB const m_pfnTrackSubMenu;

	// index of the associated submenu
	const int m_nSubMenuIndex;

	// Item text
	const CString m_strText;

	// item extent
	const CSize m_extentText;

	// accelerator
	const TCHAR m_chAccelerator;

public:

	CXTSubMenuBarItem(
		CXTMenuBar* pMenuBar,
		int nSubMenuIndex,
		const CString& strText,
		RENDERSUBMENU_CB pfnDraw,
		TRACKSUBMENU_CB pfnTrackSubMenu,
		CSize extentText,
		TCHAR chAccelerator)
	: m_pfnDraw(pfnDraw)
	, m_pfnTrackSubMenu(pfnTrackSubMenu)
	, m_nSubMenuIndex(nSubMenuIndex)
	, m_strText(strText)
	, CXTMenuBarItem(pMenuBar)
	, m_extentText(extentText)
	, m_chAccelerator(chAccelerator)
	{ }

	const CString& GetText() const
	{
		return m_strText;
	}

	int GetSubMenuIndex() const
	{
		return m_nSubMenuIndex;
	}

	virtual void TrackMenu()
	{
		(GetMenuBar()->*m_pfnTrackSubMenu)(this);
	}

	virtual CSize GetExtent()
	{
		return m_extentText;
	}

	virtual void Render(CDC* pDC)
	{
		(GetMenuBar()->*m_pfnDraw)(pDC, this);
	}

	virtual bool GetAccelerator(TCHAR* pchAccelerator)
	{	
		*pchAccelerator = m_chAccelerator;
		return m_chAccelerator != 0;
	}
};


// This class defines a window menu menubar item
class CXTSysMenuBarItem : public CXTMenuBarItem
{
	RENDERWINDOWMENU_CB const m_pfnDraw;
	TRACKWINDOWMENU_CB const m_pfnTrackWindowMenu;
	TRACKWINDOWMENU_CB const m_pfnDblClickWindowMenu;

public:	
	CXTSysMenuBarItem(
		CXTMenuBar* pMenuBar,
		RENDERWINDOWMENU_CB pfnDraw,
		TRACKWINDOWMENU_CB pfnTrackWindowMenu,
		TRACKWINDOWMENU_CB pfnDblClickWindowMenu)
	: m_pfnDraw(pfnDraw)
	, m_pfnTrackWindowMenu(pfnTrackWindowMenu)
	, CXTMenuBarItem(pMenuBar)
	, m_pfnDblClickWindowMenu(pfnDblClickWindowMenu)
	{}

	virtual bool IsHidden() const
	{
		// show this button when MDI child is maximized and that is not prohibited
		return !GetMenuBar()->IsMDIMaximized() || !GetMenuBar()->IsShowMDIButtons();
	}

	virtual void TrackMenu()
	{
		(GetMenuBar()->*m_pfnTrackWindowMenu)(this);
	}

	virtual CSize GetExtent()
	{
		int cx = xtAfxData.bXPMode ? 16 : xtAfxData.cxSmIcon;
		int cy = xtAfxData.bXPMode ? 15 : xtAfxData.cySmIcon-1;

		// in this implementation, extent does not depend on orientation of pMenuBar
		// since we do not rotate the icon
		return CSize(cx,cy);
	}

	virtual void Render(CDC* pDC)
	{
		(GetMenuBar()->*m_pfnDraw)(pDC, this);
	}

	// Draw 2 pixel gap between icon and the following menu item
	virtual int GetSpacer() const
	{
		return 2;
	}

	virtual void OnDblClick()
	{
		(GetMenuBar()->*m_pfnDblClickWindowMenu)(this);
	}
};

// Renders
class CXTControlMenuBarItem : public CXTMenuBarItem
{
	RENDERCONTROL_CB const m_pfnDraw;	

	// flags to use when rendering the control
	const UINT m_dfcs;

	// Associated command
	const UINT m_uSysCmd;

	// This item spacer
	const int m_nSpacer;

public:

	CXTControlMenuBarItem(
		CXTMenuBar* pMenuBar,
		UINT dfcs,
		UINT uSysCmd,
		RENDERCONTROL_CB pfnDraw,
		int nSpacer)
	: m_pfnDraw(pfnDraw)
	, CXTMenuBarItem(pMenuBar)
	, m_dfcs(dfcs)
	, m_uSysCmd(uSysCmd)
	, m_nSpacer(nSpacer)
	{ }

	UINT GetFrameControlFlags() const
	{
		return m_dfcs;
	}

	UINT GetSysCmd() const
	{
		return m_uSysCmd;
	}

	virtual CSize GetExtent()
	{		
		int cx = xtAfxData.bXPMode ? 16 : xtAfxData.cxSmIcon;
		int cy = xtAfxData.bXPMode ? 15 : xtAfxData.cySmIcon-1;
		return CSize(cx,cy);
	}

	virtual int GetSpacer() const
	{
		return m_nSpacer;
	}
	
	virtual void Render(CDC* pDC)
	{
		(GetMenuBar()->*m_pfnDraw)(pDC, this);
	}

	virtual bool IsWrappable() const
	{
		// you can wrap between restore and close
		return m_uSysCmd == SC_RESTORE;
	}

	virtual bool IsHidden() const
	{
		// show this button when MDI child is maximized and that is not prohibited
		return !GetMenuBar()->IsMDIMaximized() || !GetMenuBar()->IsShowMDIButtons();
	}
};

bool        CXTMenuBar::m_bShowAll      = false;
bool        CXTMenuBar::m_bAltKey       = false;
CXTMenuBar* CXTMenuBar::m_pMenuBar      = NULL;
HHOOK       CXTMenuBar::m_hMsgHook      = NULL;
int         CXTMenuBar::m_iHookRefCount = 0;

/////////////////////////////////////////////////////////////////////////////
// CXTMenuBar
/////////////////////////////////////////////////////////////////////////////
CXTMenuBar::CXTMenuBar() 
{
	m_hMenu				= NULL;
	m_pMenuPopup		= NULL;
	m_hMenuDefault		= NULL;
	m_hMenuShared		= NULL;
	m_hWndMDIClient		= NULL;
	m_bMDIMaximized		= FALSE;        // are MDI windows maximized?
	m_bTimerActive		= false;
	m_bMoreWindows		= true;
	m_bActive			= TRUE;
	m_nIDEvent			= 1000;
	m_nTimeOut			= 4500;
	m_bStretchToFit		= true;
	m_nMenuID			= 0;    
	m_bShowMDIButtons	= true;
	m_nMRUMaxWidth		=  -1;
	m_bMenuUnderlines	= TRUE;
	m_bDelayCheckMouse  = false;
	m_cyTopBorder		= 3;
	m_cyBottomBorder	= 3;
	m_pFrameHook		= NULL;
	m_pChildFrameHook   = NULL;
	m_pCustomHandler	= NULL;

    if (CXTOSVersionInfo::Get().IsWin2KOrGreater())
    {
        :: SystemParametersInfo(SPI_GETMENUUNDERLINES, 0, 
           &m_bMenuUnderlines, 0);
    }

	// create the image list used by our MDI frame buttons
	m_imageList.Create(XT_IDB_BTN_FRAME, 10, 1, RGB(255,0,255));

	ResetTrackingState();
}

void CXTMenuBar::ResetTrackingState()
{
	m_iTrackingState = TRACK_NONE;   // initial state: not tracking 
	m_iPopupTracking = -1; 
	m_iNewPopup      = -1;
	m_iButton        = -1;
	m_iDepressedItem = -1;
	m_iHotItem		 = -1;
	m_iTracking      = -1;
	m_bDown			 = 
	m_bPtMouseInit   =
	m_bProcessRightArrow =
	m_bProcessLeftArrow	 =
	m_bEscapeWasPressed  = false;
	m_ptMouse.x = m_ptMouse.y = 0;
}

CXTMenuBar::~CXTMenuBar()
{
	// cleanup item descriptors
	RemoveAll();

	if (m_hMenu && ::IsMenu(m_hMenu))
		::DestroyMenu(m_hMenu);
	if (m_hMenuDefault && ::IsMenu(m_hMenuDefault))
		::DestroyMenu(m_hMenuDefault);

	m_mapMenu2OLEItem.RemoveAll();

	SAFE_DELETE( m_pFrameHook );
	SAFE_DELETE( m_pChildFrameHook );
}

IMPLEMENT_DYNAMIC(CXTMenuBar, CXTControlBar)

BEGIN_MESSAGE_MAP(CXTMenuBar, CXTControlBar)
	//{{AFX_MSG_MAP(CXTMenuBar)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_SETTINGCHANGE()
	ON_WM_NCCALCSIZE()
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTMenuBar::RemoveAll()
{
	int i;
	for (i = 0; i < m_arrItems.GetSize(); i++)
	{
		delete m_arrItems[i];
		m_arrItems[i] = 0;
	}
	m_arrItems.RemoveAll();
}

int CXTMenuBar::FindItem(CXTMenuBarItem* pItem)
{
	int iButton = -1;
	int i;
	for (i = 0; i < m_arrItems.GetSize() && iButton < 0; i++)
	{
		if (m_arrItems[i] == pItem)
		{
			iButton = i;
		}
	}
	return iButton;
}

// Enumeration to indicate what type of items shall be queried.
enum 
{
	ITEMTYPE_POPUP   = 1,
	ITEMTYPE_CONTROL = 2
};

int* CXTMenuBar::GetItems(int* itemsBegin, int* itemsEnd, int flags) const
{	
	int lBound = 0;
	int uBound = m_arrItems.GetSize();
	if (!(flags & ITEMTYPE_POPUP))
	{
		lBound = uBound - 3;
	}
	if (!(flags & ITEMTYPE_CONTROL))
	{
		uBound -= 3;
	}

	int i;
	for (i = lBound; (i < uBound) && (itemsBegin < itemsEnd); i++)
	{
		if (!m_arrItems[i]->IsHidden())
		{
			*itemsBegin++ = i;
		}
	}
	return itemsBegin;
}

#define GET_ITEMS(flags) \
	int* itemsBegin = (int*)alloca(m_arrItems.GetSize() * sizeof(int));\
	int* itemsEnd = GetItems(itemsBegin, itemsBegin + m_arrItems.GetSize(), (flags))
				
BOOL CXTMenuBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	return CreateEx(pParentWnd, 0, dwStyle,
		CRect(m_cxLeftBorder, m_cyTopBorder, m_cxRightBorder, m_cyBottomBorder), nID);
}

BOOL CXTMenuBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, CRect rcBorders, UINT nID)
{
	ASSERT_VALID(pParentWnd);	// must have a parent
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

	SetBorders(rcBorders);

	// save the style
	m_dwStyle = (dwStyle & (CBRS_ALL|CBRS_GRIPPER));
	if (nID == AFX_IDW_TOOLBAR)
		m_dwStyle |= CBRS_HIDE_INPLACE;

	dwStyle &= ~CBRS_ALL;
	dwStyle |= CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE;
	dwStyle |= dwCtrlStyle;

	// initialize common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_BAR_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));

	ASSERT(xtAfxData.iComCtlVersion != -1);

	// create the HWND
	CRect rect; rect.SetRectEmpty();
	if (!CWnd::Create(TOOLBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	// Note: Parent must resize itself for control bar to be resized

	// Destroy the parent frame's menu and set it to NULL.
	CMenu* pMenu = pParentWnd->GetMenu( );
	if ( pMenu && ::IsMenu( pMenu->m_hMenu ) )
	{
		pMenu->DestroyMenu( );
		pParentWnd->SetMenu( NULL );
	}

	m_pFrameHook = new CXTMBarWndHook;
	ASSERT(m_pFrameHook != NULL);

	if (m_pFrameHook != NULL) {
		ASSERT_VALID(this);
		m_pFrameHook->Install(this, pParentWnd->GetSafeHwnd());
	}

	// If this is an MDI app, hook client window to trap WM_MDISETMENU
	if (pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		m_hWndMDIClient = ((CMDIFrameWnd*)pParentWnd)->m_hWndMDIClient;
		ASSERT(m_hWndMDIClient);

		m_pChildFrameHook = new CXTMBarMDIWndHook;
		ASSERT(m_pChildFrameHook != NULL);

		if (m_pChildFrameHook != NULL) {
			m_pChildFrameHook->Install(this, m_hWndMDIClient);
		}
	}
	
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	// Set the menubar title.
	CString string;
	string.LoadString(XT_IDS_MENUBAR);
	SetWindowText(string);

	CWinApp* pApp = AfxGetApp();
	ASSERT( pApp );

	m_strValueName.Format(XT_IDS_REG_CBARID, pApp->m_pszProfileName,
		_AfxGetDlgCtrlID(m_hWnd));

	m_strSubKey.Format(XT_IDS_REG_KEY, pApp->m_pszRegistryKey,
		pApp->m_pszProfileName);

	m_toolTip.Create(this);

	return TRUE;
}

void CXTMenuBar::UpdateDisplay(bool bAltKey)
{
	if (m_bAltKey != bAltKey)
	{
		m_bAltKey = bAltKey;
		InvalidateRect(NULL);
	}
}

BOOL CXTMenuBar::LoadMenuBar(UINT nMenuID)
{
	m_nMenuID = nMenuID;
	return LoadMenuBar(MAKEINTRESOURCE(nMenuID));
}

BOOL CXTMenuBar::LoadMenuBar(LPCTSTR lpszMenuName)
{
	ASSERT_VALID(this);
	ASSERT(m_hMenu == NULL);

	// Get the resource handle for the menu.
	HINSTANCE hInst = ::AfxFindResourceHandle(lpszMenuName, RT_MENU);

	if (!hInst) {
		TRACE1( "Unable to locate menu resource %s!\n", lpszMenuName);
		return FALSE;
	}

	// Load the menu.
	m_hMenuDefault = ::LoadMenu(hInst, lpszMenuName);

	// Load the menu, should return NULL, if not something strange
	// happened.
	if (!m_hMenuDefault || LoadMenu(m_hMenuDefault, NULL) != NULL ) 
	{
		if (HIWORD(lpszMenuName) != 0)
		{
			TRACE1( "Could not load menu for menubar %s!\n", lpszMenuName);
		}
		else
		{
			TRACE1( "Could not load menu for menubar ID=%d!\n", lpszMenuName);
		}
		return FALSE;
	}

	return TRUE;
}

BOOL CXTMenuBar::IsFloating() const
{
	return (m_dwStyle & CBRS_FLOATING) != 0;
}

BOOL CXTMenuBar::IsHorzDocked() const
{
	return (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
}

CFont& CXTMenuBar::GetTextFont() const
{
	if (IsFloating() || IsHorzDocked()) {
		return xtAfxData.font;
	}
	else {
		return xtAfxData.fontVCaptR;
	}
}

CSize CXTMenuBar::GetTextSize(CDC* pDC, CString strMenuText) const
{
    // remove the mnemonic character before getting the text size.
    _xtAfxStripMnemonics(strMenuText);

    CSize size = pDC->GetTextExtent(strMenuText);

    // add padding.
    size.cx += 14;
    size.cy += 2;

    return size;
}

TCHAR CXTMenuBar::GetHotKey(LPCTSTR lpszMenuName) const
{
	TCHAR chHotKey = 0;
    int iLen = _tcslen(lpszMenuName);

    // Get the hot key for the menu item, make sure the string
    // does not contain "&&" which would not be our hot key. 
    int i;
	for (i = 0; i < iLen; ++i)
    {
        if ((lpszMenuName[i]   == _T('&')) &&
            (lpszMenuName[i+1] != _T('&')) && (i != iLen-1))
        {
            chHotKey = lpszMenuName[i+1];
            break;
        }
    }

    return chHotKey;
}

HMENU CXTMenuBar::LoadMenu(HMENU hMenu, HMENU hMenuShared)
{
	ASSERT_VALID(this);

	// If both hMenu and hMenuShared are NULL, then use the
	// default menu.
	if (hMenu == NULL && hMenuShared == NULL) {
		hMenu = m_hMenuDefault;
	}

	ASSERT(::IsMenu(hMenu));
	
	HMENU hMenuPrev = m_hMenu;
	m_hMenu = hMenu;

    // insert the tools menu if initialized.
    if ( CXTToolsManager::Get().IsInitialized() )
    {
        CXTToolsManager::Get().InsertToolsMenu( m_hMenu,
			CXTToolsManager::Get().GetMenuIndex( m_hMenu == m_hMenuDefault ) );

		if ( !CXTToolsManager::Get().IsInitialized() )
		{
			LoadMenu( m_hMenu, m_hMenuShared );
		}
    }

	// delete existing menubar items, allocate array anew
	RemoveAll();
	ResetTrackingState();
	int nMenuCount = ::GetMenuItemCount(hMenu);
	m_arrItems.SetSize(0, nMenuCount + 4);	
	
	// add an item for MDI icon	
	CXTSysMenuBarItem* pItem = new CXTSysMenuBarItem(this, 
		&CXTMenuBar::OnDrawIcon, 
		&CXTMenuBar::OnTrackWindowMenu,
		&CXTMenuBar::OnDblClickWindowMenu);
	m_arrItems.Add(pItem);

	// add text items
	{
		CWindowDC dc(NULL);
		int iSavedDC = dc.SaveDC();

		// select in the default horizontal font.
		dc.SelectObject(&xtAfxData.font);

		int iButton;
		for ( iButton = 0; iButton < nMenuCount; iButton++ )
		{
			TCHAR szMenuName[64];
			memset(szMenuName, 0, sizeof(szMenuName));

			if (::GetMenuString(hMenu, iButton, szMenuName, _countof(szMenuName)-1, MF_BYPOSITION))
			{
                TCHAR chHotKey = GetHotKey(szMenuName);
                CSize sizeText = GetTextSize(&dc, szMenuName);
                sizeText.cy = xtAfxData.cyMenuItem;

				CXTSubMenuBarItem* pItem = new CXTSubMenuBarItem(this,
					iButton,
					szMenuName,
					&CXTMenuBar::OnDrawMenuItem,
					&CXTMenuBar::OnTrackSubMenu,
					sizeText,
					chHotKey);

				m_arrItems.Add(pItem);
			}
		}

		dc.RestoreDC(iSavedDC);
	}

	// create frame control buttons
	static const struct 
	{
		UINT dfcs;
		UINT uSysCmd;
		int nSpacer;
	} 
	mdiButtons[] = 
	{
		{ DFCS_CAPTIONMIN,     SC_MINIMIZE,			0},
		{ DFCS_CAPTIONRESTORE, SC_RESTORE,          2},
		{ DFCS_CAPTIONCLOSE,   SC_CLOSE,            0},
	};
	int i;
	for (i = 0; i < _countof(mdiButtons); i++)
	{
		CXTControlMenuBarItem* pItem = new CXTControlMenuBarItem(this, 
											mdiButtons[i].dfcs, 
											mdiButtons[i].uSysCmd, 
											&CXTMenuBar::OnRenderControl,
											mdiButtons[i].nSpacer);
		m_arrItems.Add(pItem);
	}

	((CXTMenuBar*)this)->FillCommandRoutingMap();

	m_hMenuShared = hMenuShared;

	// recalculate item layout and stuff
	CheckMinMaxState(false); // do not call DoLayout() if changed
	const bool bHorz = (m_dwStyle & (CBRS_ORIENT_HORZ | CBRS_FLOATING)) != 0;	
	CalcItemLayout(bHorz);
	DoLayout();

	return hMenuPrev;
}

void CXTMenuBar::OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL /*bDisableIfNoHndler*/)
{
	if (m_iButton == -1)
	{
		m_iPopupTracking = -1;
	}

	CheckMinMaxState(true);

	// check to see if mouse has left the client are, reset hot item if so
	if (m_bDelayCheckMouse && m_iHotItem >= 0 && GetCapture() != this)
	{
		CPoint ptCursor;
		if (::GetCursorPos(&ptCursor))
		{
			ScreenToClient(&ptCursor);
			CRect rectClient;
			GetClientRect(&rectClient);
			if (!rectClient.PtInRect(ptCursor))
			{
				SetHotItem(-1);
				m_bDelayCheckMouse = false;
			}
		}
	}

	// WINBUG: In Windows 2000, the message handler for WM_SETTINGSCHANGE 
	// is not called when "Hide keyboard indicators until I use the Alt key"
	// is turned on or off, so I have to call this here instead.
    if (CXTOSVersionInfo::Get().IsWin2KOrGreater())
    {
		BOOL bMenuUnderlines;
        ::SystemParametersInfo(SPI_GETMENUUNDERLINES, 0, &bMenuUnderlines, 0);
		if (bMenuUnderlines != m_bMenuUnderlines)
		{
			m_bMenuUnderlines = bMenuUnderlines;
			Invalidate();
		}
    }
}

void CXTMenuBar::OnDrawIcon(CDC* pDC, CXTSysMenuBarItem* pItem)
{
	ASSERT(m_hWndMDIClient);
	HWND hWnd = (HWND)::SendMessage(m_hWndMDIClient, WM_MDIGETACTIVE, 0, NULL);
	
	// Try to locate the icon for the MDI window by calling WM_GETICON
	// first, this will give us the correct icon if the user has called
	// SetIcon(...) for the child frame.
	HICON hIcon = (HICON)::SendMessage(hWnd, WM_GETICON, FALSE, 0);
	
	// If the icon returned is NULL, then try using GCL_HICONSM to get the
	// document icon for the child frame
	if (hIcon == NULL) {
		hIcon = (HICON)::GetClassLong(hWnd, GCL_HICONSM);
	}
	
	// If no icon was found, then use the default windows logo icon.
	if (hIcon == NULL) {
		hIcon = AfxGetApp()->LoadStandardIcon(IDI_WINLOGO);
	}

	// draw icon centered in the rect
	CRect rcItem(pItem->GetOrigin(), pItem->GetExtent());
	int x = (rcItem.left + rcItem.right - xtAfxData.cxSmIcon) / 2;
	int y = (rcItem.top + rcItem.bottom - xtAfxData.cySmIcon) / 2;
	::DrawIconEx(*pDC, __max(x, 0), __max(y,0), hIcon,
		xtAfxData.cxSmIcon, xtAfxData.cySmIcon, 0, NULL, DI_NORMAL);
}

DWORD CXTMenuBar::GetAlignmentStyle() const
{
	if ( IsFloating( ) ) {
		return CBRS_ALIGN_TOP;
	}
	else if ( IsBottomDocked( ) ) {
		return CBRS_ALIGN_BOTTOM;
	}
	else if ( IsTopDocked( ) ) {
		return CBRS_ALIGN_TOP;
	}
	else if ( IsLeftDocked( ) ) {
		return CBRS_ALIGN_LEFT;
	}
	else if ( IsRightDocked( ) ) {
		return CBRS_ALIGN_RIGHT;
	}

	return ( m_dwStyle & CBRS_ALIGN_ANY );
}

void CXTMenuBar::DrawHorzText(CDC* pDC, CRect& rcItem, CXTSubMenuBarItem* pItem)
{
	DWORD dwDrawFlags = DT_SINGLELINE | DT_CENTER | DT_VCENTER;

	// remove the mnemonic character if the system doesn't display
	// one, we will only show it if the Alt key is pressed.
	CString strMenuText = pItem->GetText();
	if (m_bMenuUnderlines == false && m_bAltKey == false)
	{
		_xtAfxStripMnemonics(strMenuText);
	}

	pDC->DrawText(strMenuText, &rcItem, dwDrawFlags);
}

void CXTMenuBar::DrawVertText(CDC* pDC, CRect& rcItem, CXTSubMenuBarItem* pItem, COLORREF crText)
{
	DWORD dwDrawFlags = DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX;

	// save the index of the mnemonic text character before we remove it.
	CString strMenuText = pItem->GetText();
	int iMnemonic = strMenuText.Find(_T('&'));
	
	// remove the mnemonic character for vertical text, we will have to
	// draw it ourself.
	_xtAfxStripMnemonics(strMenuText);

	::SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	// we will have to do the necessary calculations for centering the
	// text when drawn vertically, this will have the same effect as
	// using DT_CENTER | DT_VCENTER flags for horizontal text.
	CSize sizeText = pDC->GetTextExtent(strMenuText);

	// invert...
	sizeText = CSize(sizeText.cy, sizeText.cx);

	int cx = (rcItem.Width() -sizeText.cx);
	int cy = (rcItem.Height()-sizeText.cy);

	if (cx < 0) {
		cx = 2;
	}
	if (cy < 0) {
		cy = 2;
	}

	int iOffset = rcItem.Width()-(cx/2);

	CRect rcText = rcItem;
	if (rcText.Width() % 2)
		rcText.left -= 1;
	rcText.OffsetRect(iOffset, 0);
	rcText.DeflateRect(cx/2, cy/2, cx/2, cy/2);

	// draw the text
	pDC->DrawText(strMenuText, &rcText, dwDrawFlags);

	// remove the mnemonic character if the system doesn't't display
	// one, we will only show it if the Alt key is pressed.
	if (m_bMenuUnderlines || m_bAltKey && iMnemonic != -1)
	{
		CPen pen(PS_SOLID, 1, crText);
		
		CSize sizeChar = pDC->GetTextExtent(strMenuText.GetAt(iMnemonic));
		int cxUL = sizeChar.cx;
		
		rcText.OffsetRect(-sizeText.cx+1, 0);
		
		int iChar;
		for (iChar = 0; iChar < iMnemonic; ++iChar)
		{
			sizeChar = pDC->GetTextExtent(strMenuText.GetAt(iChar));
			rcText.top += sizeChar.cx;
		}
		
		rcText.bottom = rcText.top + cxUL;
		
		pDC->MoveTo(rcText.left-1, rcText.top);
		pDC->LineTo(rcText.left-1, rcText.bottom);
	}
}

void CXTMenuBar::DrawMenuItemText(CDC* pDC, CRect& rcItem, CXTSubMenuBarItem* pItem, bool bHorz)
{
	int iSavedDC = pDC->SaveDC();

	COLORREF crText = (m_bActive) ?
		xtAfxData.clrMenuText : xtAfxData.clrGrayText;
	
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(crText);
	pDC->SelectObject(&GetTextFont());

	if (bHorz)
	{
		CRect r = GetItemRect(pItem);
		DrawHorzText(pDC, r, pItem);
	}
	else {
		DrawVertText(pDC, rcItem, pItem, crText);
	}

	pDC->RestoreDC(iSavedDC);
}

void CXTMenuBar::OnDrawMenuItem(CDC* pDC, CXTSubMenuBarItem* pItem)
{
	const bool bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) || IsFloating();

	
	int iGraphicsMode = ::GetGraphicsMode(pDC->m_hDC);
	::SetGraphicsMode(pDC->m_hDC, GM_ADVANCED);

	CRect rcItem = GetItemRect(pItem);
	
	const bool bIsHot = IS_HOTITEM(pItem);

	if(xtAfxData.bXPMode)
	{
		// Make sure that we are not painting off the edge.
		CRect rcBox;
		pDC->GetClipBox(&rcBox);
		
		if (rcItem.bottom > rcBox.bottom) {
			rcItem.bottom = rcBox.bottom;
		}
		if (rcItem.right > rcBox.right) {
			rcItem.right = rcBox.right;
		}

		if (bIsHot)
		{
			if (m_bActive == FALSE) {
				// Do nothing.
			}
			else if (m_pMenuPopup == NULL) 
			{
				pDC->FillSolidRect( &rcItem, xtAfxData.clrXPHighlight );
				pDC->Draw3dRect( &rcItem, xtAfxData.clrHighlight, xtAfxData.clrHighlight);
			}
			else 
			{
				/*switch (GetAlignmentStyle())
				{
					case CBRS_ALIGN_TOP:
						pDC->FillSolidRect( 
							rcItem.left + 1, 
							rcItem.top + 1, 
							rcItem.Width() - 2, 
							rcItem.Height() - 1,
							xtAfxData.clrXPBarFace);
						break;

					case CBRS_ALIGN_BOTTOM:
						pDC->FillSolidRect( 
							rcItem.left + 1, 
							rcItem.top, 
							rcItem.Width() - 2, 
							rcItem.Height() - 1,
							xtAfxData.clrXPBarFace);
						break;

					case CBRS_ALIGN_LEFT:
						pDC->FillSolidRect( 
							rcItem.left + 1, 
							rcItem.top + 1, 
							rcItem.Width() - 1, 
							rcItem.Height() - 2,
							xtAfxData.clrXPBarFace);
						break;

					case CBRS_ALIGN_RIGHT:
						pDC->FillSolidRect( 
							rcItem.left, 
							rcItem.top + 1, 
							rcItem.Width() - 1, 
							rcItem.Height() - 2,
							xtAfxData.clrXPBarFace);
						break;
				}*/
				//rcItem.InflateRect(1, 1);
				pDC->FillSolidRect(rcItem, xtAfxData.clrXPBarFace);
				pDC->Draw3dRect( &rcItem, xtAfxData.clrXPMenuBorder, xtAfxData.clrXPMenuBorder);

			}
		}
	}
	else
	{
		// Make sure that we are not painting off the edge.
		CRect rcBox;
		pDC->GetClipBox(&rcBox);

		if (rcItem.bottom > rcBox.bottom) {
			rcItem.bottom = rcBox.bottom;
		}
		if (rcItem.right > rcBox.right) {
			rcItem.right = rcBox.right;
		}

		if (bIsHot)
		{
			if (m_bActive == FALSE) {
				// Do nothing.
			}
			else if (m_pMenuPopup == NULL) {
				pDC->Draw3dRect(&rcItem, xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
			}
			else {
				pDC->Draw3dRect(&rcItem, xtAfxData.clr3DShadow, xtAfxData.clr3DHilight);
				rcItem.OffsetRect(1,1);
			}
		}

		
	}
	DrawMenuItemText(pDC, rcItem, pItem, bHorz);

	::SetGraphicsMode( pDC->m_hDC, iGraphicsMode );
}

void CXTMenuBar::CheckMinMaxState(bool bDoLayout)
{
	BOOL bMax=FALSE;
	if (m_hWndMDIClient)
	{
		
		::SendMessage(m_hWndMDIClient,
			WM_MDIGETACTIVE, 0, (LPARAM)&bMax);

		// Update buttons when Ctrl-tabbing between views of 
		// different SysMenu styles... 
		if (bMax)
		{
			if (!(GetMDIWindowStyle() & WS_SYSMENU))
			{
				bMax = false;
			}
		}
	}

	// state changed: update menu bar
	if (bMax != m_bMDIMaximized)
	{
		m_bMDIMaximized = bMax;		
		if (bDoLayout)
		{
			// recalculate item layout and stuff
			DoLayout();
		}
	}
}

void CXTMenuBar::DoLayout()
{
	if (InRebar())
	{
		CalcItemLayout(true);
	}

	if (m_pDockSite)
	{
		m_pDockSite->RecalcLayout();
		
		// Force toolbar to recompute size
		CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, GetOwner());
		ASSERT_VALID(pFrameWnd);
		if(pFrameWnd->IsFrameWnd())
		{
			if (pFrameWnd != m_pDockSite)
			{
				pFrameWnd->RecalcLayout();
			}
			
			// floating frame
			CFrameWnd* pFrameWnd2 = GetParentFrame();
			if (pFrameWnd2 && pFrameWnd2->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd))) 
			{
				if (pFrameWnd2 != m_pDockSite && 
					pFrameWnd2 != pFrameWnd)
				{
					pFrameWnd2->RecalcLayout();
				}
			}
		}
		
		// flush pending visual changes
		RedrawWindow();
		if (IsFloating())
		{
			m_pDockSite->UpdateWindow();
		}
	}
}

void CXTMenuBar::SetHotItem(int iButton)
{
	bool bChanged = m_iHotItem != iButton;
	m_iHotItem = iButton;
	if (bChanged)
	{
		Invalidate();
	}
}

void CXTMenuBar::PressItem(int iButton)
{
	bool bChanged = (m_iDepressedItem != iButton);
	m_iDepressedItem = iButton;
	if (bChanged)
	{
		RedrawWindow();
	}
}

void CXTMenuBar::SetTrackingState(XT_TRACK_STATE iState, int iButton)
{
	ASSERT_VALID(this);

	if (iState != m_iTrackingState)
	{
		if (iState == TRACK_NONE) {
			iButton = -1;
		}

		// could be none (-1)
		SetHotItem(iButton);
		
		// set related state stuff
		if (iState==TRACK_POPUP)
		{
			m_bEscapeWasPressed  = FALSE;	// assume Esc key not pressed
			m_bProcessRightArrow =			// assume left/right arrow..
			m_bProcessLeftArrow	 = TRUE;	// ..will move to prev/next popup
			m_iPopupTracking     = iButton;	// which popup I'm tracking
		}

		m_iTrackingState = iState;
	}
}

void CXTMenuBar::ToggleTrackButtonMode()
{
	ASSERT_VALID(this);

	switch (m_iTrackingState)
	{
		case TRACK_NONE:
			if (m_iHotItem == -1)
			{
				SetTrackingState(TRACK_BUTTON, 1);
			}
			else
			{
				SetTrackingState(TRACK_BUTTON, m_iHotItem);
			}
			m_bDelayCheckMouse = false;
			break;
		case TRACK_BUTTON:
			SetTrackingState(TRACK_NONE);
			break;
	}
}

int CXTMenuBar::GetNextOrPrevButton(int iButton, BOOL bPrev) const
{
	// iterate all visible popups
	GET_ITEMS(ITEMTYPE_POPUP);
	int* itemCurrent;
	for (itemCurrent = itemsBegin; itemCurrent < itemsEnd; itemCurrent++)
	{
		if (*itemCurrent >= iButton)
		{
			// found matching position
			if (bPrev) 
			{
				// circularity decrement position
				itemCurrent--;
				if (itemCurrent < itemsBegin)
				{
					itemCurrent = itemsEnd - 1;
				}
			}			
			else 
			{
				// circularity increment position
				itemCurrent++;
				if (itemCurrent >= itemsEnd)
				{
					itemCurrent = itemsBegin;
				}
			}
			return *itemCurrent;						
		}
	}

	// weird button index, 
	ASSERT(FALSE);
	return (bPrev) ? *itemsBegin : *itemsEnd;;
}

int CXTMenuBar::HitTest(CPoint p, int* itemsBegin, int* itemsEnd) const
{
    CRect rc;
	GetClientRect(&rc);
	int iHit = -1;
	if (rc.PtInRect(p)) // if point is inside client area
	{
		while (itemsBegin < itemsEnd  && iHit < 0)
		{
			int nPos = *itemsBegin++;
			CXTMenuBarItem* pItem = m_arrItems[nPos];
			CRect rcItem = GetItemRect(pItem);
			if (rcItem.PtInRect(p))
			{
				iHit = nPos;
			}
		}
	}
	return iHit;
}

void CXTMenuBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	ASSERT_VALID(this);

	GET_ITEMS(ITEMTYPE_POPUP);
	int iButton = HitTest(point, itemsBegin, itemsEnd);
	if (iButton >= 0)
	{
		if (!m_pMenuPopup) {
			m_iPopupTracking = -1;
		}
		if (iButton != m_iPopupTracking) {			
			TrackPopup(iButton);
		}
		return;
	}

	// MDI mouse message (min/max/close or icon) handled
	if (!OnMouseMessage(WM_LBUTTONDOWN, nFlags, point) && m_pDockSite)
	{
		CXTControlBar::OnLButtonDown(nFlags, point);
		m_pDockSite->DelayRecalcLayout();
	}
}

void CXTMenuBar::OnLButtonUp(UINT nFlags, CPoint pt)
{
	OnMouseMessage(WM_LBUTTONUP, nFlags, pt);
}

void CXTMenuBar::OnMouseMove(UINT nFlags, CPoint pt)
{
	if (OnMouseMessage(WM_MOUSEMOVE, nFlags, pt))
		return;
	
	switch (m_iTrackingState)
	{
		case TRACK_BUTTON:
		{
			// In button-tracking state, ignore mouse-over to non-button area.
			// Normally, the toolbar would de-select the hot item in this case.
			// 
			// Only change the hot item if the mouse has actually moved.
			// This is necessary to avoid a bug where the user moves to a different
			// button from the one the mouse is over, and presses arrow-down to get
			// the menu, then Esc to cancel it. Without this code, the button will
			// jump to wherever the mouse is--not right.
			GET_ITEMS(ITEMTYPE_POPUP | ITEMTYPE_CONTROL);
			int iHot = HitTest(pt, itemsBegin, itemsEnd);
			if (IsValidButton(iHot) && (!m_bPtMouseInit || pt != m_ptMouse))
			{
				SetHotItem(iHot);
			}
			break;
		}
		case TRACK_NONE:
		{
			GET_ITEMS(ITEMTYPE_POPUP | ITEMTYPE_CONTROL);
			int iHot = HitTest(pt, itemsBegin, itemsEnd);
			SetHotItem(iHot);
			break;
		}
		default:
		{
			m_ptMouse = pt;
			m_bPtMouseInit = TRUE;
			CXTControlBar::OnMouseMove(nFlags, pt);
		}
	}
}

// Calculate total size allocated to all borders
CSize CXTMenuBar::GetBorderSize(bool bHorz) const
{
	CRect borders(0,0,0,0);
	CalcInsideRect(borders, bHorz);
	return CSize(borders.left - borders.right, borders.top - borders.bottom);	
}

CSize CXTMenuBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;

	CSize size = CalcLayout(dwMode);

	// If the menu bar is docked stretch to fit...
	if (!m_bStretchToFit && !InRebar() && !bStretch && bHorz)
	{
		// do not shrink if wrapped, last item does not count even if it is wrapped
		GET_ITEMS(ITEMTYPE_CONTROL | ITEMTYPE_POPUP);
		bool bWrapped = false;
		while ((itemsBegin < itemsEnd - 1) && !bWrapped)
		{
			bWrapped = m_arrItems[*itemsBegin++]->IsWrapped();
		}
		if (!bWrapped)
		{
			WrapMenuBar(32767);
			size = CalcItemExtent(true);
			size += GetBorderSize(true);
		}
	}

	return size;
}

CSize CXTMenuBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	if ((nLength == -1) && !(dwMode & LM_MRUWIDTH) && !(dwMode & LM_COMMIT) &&
		((dwMode & LM_HORZDOCK) || (dwMode & LM_VERTDOCK)))
	{
		return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZDOCK);
	}
	return CalcLayout(dwMode, nLength);
}

int CXTMenuBar::GetClipBoxLength(bool bHorz)
{
	if (m_nMRUMaxWidth > 0)
	{
		return m_nMRUMaxWidth;
	}
	int nBorderDimension = GetDimension(GetBorderSize(bHorz),  bHorz);
	if (m_dwStyle & CBRS_SIZE_DYNAMIC) 
	{
		if (bHorz) 
		{
			CFrameWnd* pFrame = GetTopLevelFrame(); 
			if (pFrame)
			{
				ASSERT_VALID(pFrame);
				CRect rcFrame; 
				pFrame->GetClientRect(rcFrame);
				return rcFrame.Width() - nBorderDimension ;
			}
			else
			{
				CWnd* pParent = GetParent(); 
				ASSERT_VALID(pParent);

				CRect rcParent; 
				pParent->GetClientRect(rcParent);

				int nResult = rcParent.Height();			
				return nResult - nBorderDimension ;
			}
		}
		else 
		{
			CWnd* pParent = GetParent(); 
			ASSERT_VALID(pParent);

			CRect rcParent; 
			pParent->GetClientRect(rcParent);

			int nResult = rcParent.Height();			
			return nResult - nBorderDimension ;
		}
	}
	else 
	{
		CRect rect; 
		GetClientRect(rect);
		return GetDimension(rect.Size(), bHorz) - nBorderDimension;
	}
}

void CXTMenuBar::CalcItemLayout(bool bHorz)
{
	CRect rect(0,0,0,0);
	CalcInsideRect(rect, bHorz);
	
	int x = (bHorz) ? 0 : rect.left;
	int y = 0;

	const int nLineHeight = xtAfxData.cyMenuItem;
	int nTotalHeight = nLineHeight;
	bool bMultiline = false;

	GET_ITEMS(ITEMTYPE_POPUP | ITEMTYPE_CONTROL);
	int* itemsCurrent = itemsBegin;
	while (itemsCurrent < itemsEnd)
	{
		CXTMenuBarItem* pItem = m_arrItems[*itemsCurrent++];
		if (pItem->IsWrapped() && itemsCurrent < itemsEnd)
		{
			nTotalHeight += nLineHeight;
			bMultiline = true;
		}
		CSize extent = pItem->GetExtent();
		
		CPoint ptItem;
		if (bHorz)
		{
			ptItem.x = x;
			ptItem.y = y + (nLineHeight - extent.cy) / 2;
		}
		else
		{
			// NB: vertical orientation flips along Y axis, see below
			//  how this turns into a positive coordinate
			ptItem.x = -x - (nLineHeight + extent.cy) / 2;
			ptItem.y = y;
		}
		pItem->SetOrigin(ptItem);
		if (pItem->IsWrapped()) 
		{
			if (bHorz) 
			{
				x = rect.left;// reset x to origin
				y += nLineHeight;
			}
			else
			{
				x += nLineHeight;
				y = rect.top;  // reset y to origin
			}
		}
		else
		{
			int nSpacer = pItem->GetSpacer();
			if (bHorz) 
			{				
				x += extent.cx + nSpacer;
			}
			else
			{
				y += extent.cx + nSpacer;
			}
		}
	}
	if (!IsFloating() && m_bStretchToFit && (bHorz || bMultiline))
	{
		// offset groups of unwrapped control buttons to the end of the visible part
		GET_ITEMS(ITEMTYPE_CONTROL);		
		if (itemsBegin < itemsEnd)
		{		
			CRect borders(0,0,0,0);
			GetClientRect(&borders);

			int nLeftTopBorder = GetDimension(borders.TopLeft(), bHorz);
			int nDimension = GetClipBoxLength(bHorz) + nLeftTopBorder;
			while(itemsBegin < itemsEnd)
			{
				int* itemsGroupEnd = itemsBegin;
				// adjust past first wrappable item
				while (itemsGroupEnd < itemsEnd)
				{
					CXTMenuBarItem* pItem = m_arrItems[*itemsGroupEnd++];
					if (pItem->IsWrapped())
					{
						break;
					}
				}

				CRect rectItem = GetItemRect(m_arrItems[*(itemsGroupEnd - 1)]);
				int nExtraSpace = 
					nDimension - GetDimension(rectItem.BottomRight(), bHorz) - ::GetSystemMetrics(SM_CXEDGE);

				if (nExtraSpace > 0)
				{
					while (itemsBegin < itemsGroupEnd)
					{
						CXTMenuBarItem* pItem = m_arrItems[*itemsBegin++];
						CPoint ptOrigin = pItem->GetOrigin();
						int nNewDimension = GetDimension(ptOrigin, bHorz) + nExtraSpace;
						SetDimension(ptOrigin, bHorz, nNewDimension);
						pItem->SetOrigin(ptOrigin);				
					}
				}
				else
				{
					itemsBegin = itemsGroupEnd;
				}
			}
		}
	}
	if (!bHorz)
	{
		// complete flip all items
		int* itemsCurrent = itemsBegin;
		while (itemsCurrent < itemsEnd)
		{
			CXTMenuBarItem* pItem = m_arrItems[*itemsCurrent++];
			CPoint ptItem = pItem->GetOrigin();
			ptItem.x += nTotalHeight - rect.right;
			pItem->SetOrigin(ptItem);
		}
	}
}

CSize CXTMenuBar::CalcLayout(DWORD dwMode, int nLength)
{
#ifdef _DEBUG
	if (dwMode & LM_HORZDOCK)
	{
		ASSERT(dwMode & LM_HORZ);
	}
#endif

	CSize sizeResult(0);

	if (m_arrItems.GetSize() > 0)
	{
		bool isDefault = false;
		if (!(m_dwStyle & CBRS_SIZE_FIXED))
		{
			BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;

			if (bDynamic && (dwMode & LM_MRUWIDTH))
			{
				// since this calculation is for floating size, 
				// we do not want mdi buttons to be shifted to the end
				bool bOldStretchToFit = m_bStretchToFit;
				m_bStretchToFit = false;  // turn off the stretch
				SizeMenuBar(m_nMRUWidth, true);
				CalcItemLayout(true);
				sizeResult = CalcItemExtent(true);
				m_bStretchToFit = bOldStretchToFit; // restore old stretch mode
			}
			else 
			if (bDynamic && (dwMode & LM_HORZDOCK))
			{
				if (IsFloating() || (m_dwStyle & CBRS_ORIENT_VERT)) 
				{
					SizeMenuBar(32767, true);
				}
				else 
				{
					// Menu Button wrapped by frame width
					SizeMenuBar(GetClipBoxLength(true) + GetDimension(GetBorderSize(true), true), true);
				}

				CalcItemLayout(true);				
				sizeResult = CalcItemExtent(true);
				if (!IsFloating() && !(m_dwStyle & CBRS_ORIENT_VERT)) 
				{
					if (m_pDockContext->m_pDC) 
					{
						// while dragging (m_pDockContext->m_bDragging is helpless)
						sizeResult.cx = GetClipBoxLength(true);
					}
				}
			}
			else 
			if (bDynamic && (dwMode & LM_VERTDOCK))
			{
				CalcItemLayout(false);
				sizeResult = CalcItemExtent(false);
				if (!IsFloating() && !(m_dwStyle & CBRS_ORIENT_HORZ)) 
				{
					if (m_pDockContext->m_pDC) 
					{
						sizeResult.cy = GetClipBoxLength(FALSE);
					}
				}
			}
			else 
			if (bDynamic && (nLength != -1))
			{
				CSize sizeBorders = GetBorderSize((dwMode & LM_HORZ) != 0);
				const bool bHorz = !(dwMode & LM_LENGTHY);
				const int nLen = nLength + GetDimension(sizeBorders, bHorz);

				SizeMenuBar(nLen, bHorz);
				CalcItemLayout(bHorz);
				sizeResult = CalcItemExtent(bHorz);
			}
			else 
			if (bDynamic && (m_dwStyle & CBRS_FLOATING))
			{
				SizeMenuBar(m_nMRUWidth, true);
				CalcItemLayout(true);
				sizeResult = CalcItemExtent(true);
			}
			else
			{				
				if (!bDynamic) 
				{
					isDefault = true;
				}
				else
				{
					const bool bHorz = (dwMode & LM_HORZ) != 0;
					SizeMenuBar(GetClipBoxLength(bHorz) + GetDimension(GetBorderSize(bHorz), bHorz), bHorz);
					CalcItemLayout(bHorz);

					sizeResult = CalcItemExtent(bHorz);
					if (bHorz)
					{						
						sizeResult.cx = GetClipBoxLength(TRUE);
					}
				}
			}
		}
		else 
		{
			isDefault = true;
		}
		if (isDefault)
		{
			// CBRS_SIZE_FIXED
			bool bHorz = (dwMode & LM_HORZ) != 0;
			SizeMenuBar(32767, true);
			CalcItemLayout(bHorz);
			sizeResult = CalcItemExtent(bHorz);
		}

		if (dwMode & LM_COMMIT)
		{
			if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
				m_nMRUWidth = sizeResult.cx;
		}
	}

	//BLOCK: Adjust Margins
	{
		const bool bHorz = (dwMode & LM_HORZ) != 0;
		sizeResult += GetBorderSize(bHorz);

		CSize size = CControlBar::CalcFixedLayout((dwMode & LM_STRETCH), (dwMode & LM_HORZ));
		sizeResult.cx = __max(sizeResult.cx, size.cx);
		sizeResult.cy = __max(sizeResult.cy, size.cy);
	}
	return sizeResult;
}

CSize CXTMenuBar::CalcItemExtent(bool bHorz)
{
	int xMin = 0, xMax = 0;
	bool bFirst = true;
	GET_ITEMS(ITEMTYPE_POPUP | ITEMTYPE_CONTROL);
	int nHeight = xtAfxData.cyMenuItem;
	while (itemsBegin < itemsEnd)
	{
		CXTMenuBarItem* pItem = m_arrItems[*itemsBegin++];
		if (pItem->IsWrapped() && itemsBegin < itemsEnd)
		{
			nHeight += xtAfxData.cyMenuItem;
		}
		CPoint origin = pItem->GetOrigin();
		int x = GetDimension(origin, bHorz);
		if (bFirst || x < xMin)
		{
			xMin = x;
		}
		CSize extent = pItem->GetExtent();
		x += extent.cx;
		if (bFirst || x > xMax)
		{
			xMax = x;			
		}
		bFirst = false;
	}

	CSize sizeResult;
	SetDimension(sizeResult, bHorz, xMax - xMin);
	SetDimension(sizeResult, !bHorz, nHeight);
	return sizeResult;
}

void CXTMenuBar::SizeMenuBar(int nLength, bool bHorz)
{
	if (bHorz) 
	{				
		// nLength is horizontal length

		if (IsFloating()) 
		{		
			int cxMax, cxMin;

			// Wrap MenuBar vertically
			WrapMenuBar(0);
			CalcItemLayout(true);
			cxMin = CalcItemExtent(true).cx;

			// Wrap MenuBar horizontally
			WrapMenuBar(32767);
			CalcItemLayout(true);
			cxMax = CalcItemExtent(true).cx;

			// dichotomy to find out min dimension that yields the same number of wraps as 
			// at the given length
			while (cxMin < cxMax) 
			{
				int cxMid = (cxMin + cxMax) / 2;
				WrapMenuBar(cxMid);
				CalcItemLayout(true);
				cxMid = CalcItemExtent(true).cx;
				if (cxMid == cxMax) 
				{	
					return;
				}

				if (nLength >= cxMax) 
				{
					if (cxMin == cxMid) 
					{
						WrapMenuBar(cxMax);
						return;
					}
					cxMin = cxMid;
				}
				else 
				if (nLength < cxMax) 
				{
					cxMax = cxMid;
				}
				else 
				{
					return;
				}
			}
		}
		else 
		{								
			// each one wrapping
			WrapMenuBar(nLength - GetBorderSize(true).cx);
		}
	}
	else 
	{	
		// each one wrapping
		WrapMenuBar(nLength - GetBorderSize(false).cy);
	}
}

int CXTMenuBar::WrapMenuBar(int nWidth)
{
	int nResult = 0;
	int x = 0;

	GET_ITEMS(ITEMTYPE_POPUP | ITEMTYPE_CONTROL);
	CXTMenuBarItem* pPrevWrappableItem = 0;
	while (itemsBegin < itemsEnd)
	{
		CXTMenuBarItem* pItem = m_arrItems[*itemsBegin++];
		CSize extent = pItem->GetExtent();
		int nSpacer = pItem->GetSpacer();
		if (itemsBegin == itemsEnd)
		{
			return ++nResult;
		}
				
		if (x + extent.cx > nWidth) 
		{
			// itself is over
			if (pItem->IsWrappable()) 
			{
				pItem->SetWrapped(true);
				++nResult;
				x = 0;
			}
		}
		else 
		if (x + extent.cx + nSpacer + m_arrItems[*itemsBegin]->GetExtent().cx > nWidth) 
		{			
			if (pItem->IsWrappable()) 
			{
				pItem->SetWrapped(true);
			}
			else
			{
				// wrap previous wrappable
				pPrevWrappableItem->SetWrapped(true);
			}
			++nResult;
			x = 0;
		}
		else 
		{
			if (pItem->IsWrappable())
			{
				pItem->SetWrapped(false);			
			}			
			x += extent.cx + nSpacer;
		}
		if (pItem->IsWrappable())
		{
			pPrevWrappableItem = pItem;
		}
	}

	return nResult + 1;
}

void CXTMenuBar::OnInitMenuPopup()
{
	OnInitWindowMenu();
}

void CXTMenuBar::OnMenuSelect(HMENU hMenu, UINT iItem)
{
	if (m_iTrackingState > 0)
	{
		// process right-arrow if item is NOT a submenu
		m_bProcessRightArrow = 
			(hMenu == NULL  ||  (::GetSubMenu(hMenu, iItem) == NULL));
		// process left-arrow if current menu is one I'm tracking
		m_bProcessLeftArrow = TRUE;
	}
}

LRESULT CALLBACK CXTMenuBar::MenuInputFilter(int code, WPARAM wParam, LPARAM lParam)
{
	return (code==MSGF_MENU && m_pMenuBar &&
		m_pMenuBar->OnMenuInput(*((MSG*)lParam))) ? TRUE
		: CallNextHookEx(m_hMsgHook, code, wParam, lParam);
}

BOOL CXTMenuBar::OnMenuInput(MSG& m, CWnd *pWndMenu)
{
	ASSERT_VALID(this);
	ASSERT(m_iTrackingState == TRACK_POPUP); // sanity check
	int message = m.message;
	
	if (message == WM_KEYDOWN)
	{
		// handle left/right-arrow.
		WPARAM vkey = m.wParam;
		if ((vkey == VK_LEFT  && m_bProcessLeftArrow) ||
			(vkey == VK_RIGHT && m_bProcessRightArrow))
		{
			CancelMenuAndTrackNewOne(
				GetNextOrPrevButton(m_iPopupTracking, vkey==VK_LEFT));
			return TRUE; // eat it
			
		}
		else if (vkey == VK_ESCAPE)
		{
			m_bEscapeWasPressed = TRUE;	 // (menu will abort itself)
		}
		
	}
	else if (message == WM_MOUSEMOVE || message == WM_LBUTTONDOWN)
	{
		bool bSetTimer = false;

		// handle mouse move or click
		CPoint pt = m.lParam;
		if (pWndMenu)
		{
			::ClientToScreen(m.hwnd, &pt);
		}
		if (CWnd* pwnd = CWnd::WindowFromPoint(pt))
		{
			if (pwnd != this)
			{
				//this belongs to some other window, report as not handled
				return false;
			}
		}

		ScreenToClient(&pt);
		
		if (message == WM_MOUSEMOVE)
		{
			// Only use the timer if we have hidden commands.
			bool bUseTimer = m_arrHiddenCommands.GetSize() ? true : false;

			if (bSetTimer == false) {
				bSetTimer = true;
			}

			if (!m_bPtMouseInit || pt != m_ptMouse)
			{
				GET_ITEMS(ITEMTYPE_POPUP);
				int iButton = HitTest(pt, itemsBegin, itemsEnd);
				if (IsValidButton(iButton))
				{
					// user moved mouse over a different button: track its popup
					if (iButton != m_iPopupTracking) {
						CancelMenuAndTrackNewOne(iButton);
						bSetTimer = false;
					}
					else {
						bSetTimer = true;
					}
				}
				else {
					bSetTimer = false;
				}
				
				m_ptMouse = pt;
				m_bPtMouseInit = TRUE;
			}
			
			if (!CXTOSVersionInfo::Get().IsWin95()  &&
				!CXTOSVersionInfo::Get().IsWinNT4()) // NT4 + Win95 don't like this
			{
				if (bSetTimer == true)
				{
					if (bUseTimer && m_bTimerActive == false) 
					{
						GetOwner()->SetTimer(m_nIDEvent, m_nTimeOut, NULL);
						m_bTimerActive = true;
					}
				}
				else
				{
					if (bUseTimer && m_bTimerActive == true)
					{
						GetOwner()->KillTimer(m_nIDEvent);
						m_bTimerActive = false;
					}
				}
			}
		}
		else if (message == WM_LBUTTONDOWN)
		{
			GET_ITEMS(ITEMTYPE_CONTROL);
			int iButton = HitTest(pt, itemsBegin, itemsEnd);
			if (IsValidButton(iButton))
			{
				if (iButton == m_iPopupTracking)
				{
					// user clicked on same button I am tracking: cancel menu
					CancelMenuAndTrackNewOne(-1);
					return TRUE; // eat it
				}
			}
		}
	}

	return FALSE; // not handled
}

void CXTMenuBar::CancelMenuAndTrackNewOne(int iNewPopup)
{
	ASSERT_VALID(this);
	if (iNewPopup != m_iPopupTracking)
	{
		GetOwner()->PostMessage(WM_CANCELMODE);	// quit menu loop
		m_iNewPopup = iNewPopup;				// go to this popup (-1 = quit)
	}
}

CRect CXTMenuBar::GetItemRect(CXTMenuBarItem* pItem) const
{
	CPoint origin = pItem->GetOrigin();
	CSize extent = pItem->GetExtent();
	if ((m_dwStyle & CBRS_ORIENT_VERT) && !IsFloating())
	{
		extent = CSize(extent.cy, extent.cx);
	}
	CRect rectItem(origin, extent);
	return rectItem;
}

void CXTMenuBar::OnTrackSubMenu(CXTSubMenuBarItem* pItem)
{
	CRect rectItem = GetItemRect(pItem);
	ClientToScreen(&rectItem);

	// get submenu and display it beneath button
	TPMPARAMS tpm;
    ::ZeroMemory(&tpm, sizeof(TPMPARAMS));

	UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL;
	CPoint pt = ComputeMenuTrackPoint(rectItem, tpm, nFlags);

	CXTCoolMenu::m_bShowAll = m_bShowAll;

	XT_MENUITEMINFO info;
	info.fMask = MIIM_ID | MIIM_SUBMENU;
	::GetMenuItemInfo(m_hMenu, pItem->GetSubMenuIndex(), TRUE, &info);

	CWnd* pWndOwner = GetOwner();
	ASSERT_VALID(pWndOwner);

	if (::IsMenu(info.hSubMenu))
	{
		m_pMenuPopup = CXTMenu::FromHandle(info.hSubMenu);
		ASSERT(m_pMenuPopup && ::IsMenu(m_pMenuPopup->GetSafeHmenu()));

		CXTCoolMenu::m_rectExclude = GetItemRect(pItem);
		RedrawWindow(CXTCoolMenu::m_rectExclude, NULL);
		ClientToScreen(CXTCoolMenu::m_rectExclude);
		
		CXTCoolMenu::m_iMenuBarAlign = GetAlignmentStyle();
				
		m_pMenuPopup->TrackPopupMenuEx(nFlags,
			pt.x, pt.y, pWndOwner, &tpm);
		CXTCoolMenu::m_iMenuBarAlign = 0;
		CXTCoolMenu::m_rectExclude.SetRectEmpty();
	}
	else 
	{
		pWndOwner->PostMessage(WM_COMMAND, info.wID);
	}

	m_pMenuPopup = NULL;
}

void CXTMenuBar::OnTrackWindowMenu(CXTSysMenuBarItem* pItem)
{
	CFrameWnd* pFrame = GetTopLevelFrame()->GetActiveFrame();
	ASSERT_VALID(pFrame);
	
	// track menu
	if (HMENU hMenu = ::GetSystemMenu(*pFrame, FALSE))
	{
		CPoint origin = pItem->GetOrigin();
		ClientToScreen(&origin);
		CRect rectItem(origin, pItem->GetExtent());

	    TPMPARAMS tpm;
        ::ZeroMemory(&tpm, sizeof(TPMPARAMS));

		UINT nFlags = TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL;
		CPoint point = ComputeMenuTrackPoint(rectItem, tpm, nFlags);
		CXTCoolMenu::IgnoreNextPopup();
		::TrackPopupMenuEx(hMenu, nFlags, point.x, point.y, GetOwner()->m_hWnd, &tpm);
	}
}

void CXTMenuBar::TrackPopup(int iButton)
{
	ASSERT_VALID(this);
	ASSERT(m_hMenu);

	CXTCoolMenu::m_bIsPopup = false;
	m_bShowAll = false;

	CXTMenu menu;
	menu.Attach(m_hMenu);
	
	// while user selects another menu
	while (iButton >= 0)
	{
		CXTMenuBarItem* pItem = m_arrItems[iButton];
		m_iButton = iButton;

		m_iNewPopup = -1;	// assume quit after this
		PressItem(iButton);	// press the button
		
		// enter tracking state
		SetTrackingState(TRACK_POPUP, iButton);
		
		// Need to install a hook to trap menu input in order to make
		// left/right-arrow keys and "hot" mouse tracking work.
		//
		ASSERT((m_pMenuBar == NULL  &&  m_iHookRefCount == 0)  ||
			   (m_pMenuBar == this  &&  m_iHookRefCount > 0));
		m_pMenuBar = this;
	
		if (m_iHookRefCount == 0)
		{
			ASSERT(m_hMsgHook == NULL);
			m_hMsgHook = ::SetWindowsHookEx(WH_MSGFILTER,
				MenuInputFilter, NULL, ::GetCurrentThreadId());
		}
		++m_iHookRefCount;
		
		// delegate tracking to its concrete implementation		
		pItem->TrackMenu();

		// uninstall hook.
		if (--m_iHookRefCount == 0)
		{
			::UnhookWindowsHookEx(m_hMsgHook);
			m_hMsgHook = NULL;
			m_pMenuBar = NULL;
		}

		PressItem(-1);	 // un-press button

		if (m_iHookRefCount == 0)
		{
			// If the user exited the menu loop by pressing Escape, return 
			// to track-button state; otherwise normal non-tracking state.
			SetTrackingState(m_bEscapeWasPressed ?
				TRACK_BUTTON : TRACK_NONE, iButton);
		}
		
		// If the user moved mouse to a new top-level popup (eg from File to
		// Edit button), I will have posted a WM_CANCELMODE to quit
		// the first popup, and set m_iNewPopup to the new menu to show.
		// Otherwise, m_iNewPopup will be -1 as set above.
		// So just set iButton to the next popup menu and keep looping...
		m_iButton = iButton = m_iNewPopup;

		// clean up shadow
		RedrawWindow();
	}

	m_bShowAll = false;
	m_iButton = -1;
	menu.Detach();

	CXTCoolMenu::m_bIsPopup = true;
}

BOOL CXTMenuBar::TranslateFrameMessage(MSG* pMsg)
{
	ASSERT_VALID(this);
	ASSERT(pMsg);

	if (!IsWindowVisible())
	{
		// probably, in a print preview
		return FALSE;
	}

    bool bShift  = (::GetKeyState(VK_SHIFT) < 0);
    bool bCtrl   = (::GetKeyState(VK_CONTROL) < 0);
    bool bAltKey = (::GetKeyState(VK_MENU) < 0);

	// if the shift or control keys are pressed let the framework 
	// translate the message.
	if ( bShift || bCtrl )
	{
		// probably an accelerator key combination.
		return FALSE;
	}
	
	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
		{
			// The definitions for VK_F1 through VK_F11 are the same definitions as
			// the characters p - z (why, I don't know). This confuses the menu bar
			// when Alt+F? or similar combinations are performed. For this reason, we 
			// will bypass the menubar and pass this back to the frame class.

			switch (pMsg->wParam) // virt key
			{
			case VK_F1:
			case VK_F2:
			case VK_F3:
			case VK_F4:
			case VK_F5:
			case VK_F6:
			case VK_F7:
			case VK_F8:
			case VK_F9:
			case VK_F10:
			case VK_F11:
				return FALSE;
			}
		}
	}

	UINT message = pMsg->message;
	
	if (message == WM_SYSKEYDOWN && (HIWORD(pMsg->lParam) & KF_ALTDOWN))
	{
		UpdateDisplay(true);
	}

	if (WM_LBUTTONDOWN <= message && message <= WM_MOUSELAST)
	{
		// user clicked outside menu bar: exit tracking mode
		if (pMsg->hwnd != m_hWnd && m_iTrackingState > 0) {
			SetTrackingState(TRACK_NONE);
		}

		UpdateDisplay(false);
	}
	else if (message == WM_SYSKEYDOWN || message == WM_SYSKEYUP || message == WM_KEYDOWN)
	{
		WPARAM vkey = pMsg->wParam;							  // get virt key

		// key is VK_MENU or F10 with no alt/ctrl/shift: toggle menu mode
		if ( vkey == VK_MENU || ( vkey == VK_F10 && !bAltKey ) )
		{
			if ( message == WM_SYSKEYUP )
			{
				ToggleTrackButtonMode( );
			}

			return TRUE;
		}
		else if ((message == WM_SYSKEYDOWN || message == WM_KEYDOWN))
		{
			// I am tracking: handle left/right/up/down/space/Esc
			if (m_iTrackingState == TRACK_BUTTON)
			{
				switch (vkey)
				{
				case VK_LEFT:
				case VK_RIGHT:
					// left or right-arrow: change hot button if tracking buttons
					SetHotItem(GetNextOrPrevButton(m_iHotItem, vkey==VK_LEFT));
					return TRUE;

				case VK_SPACE:
				case VK_RETURN:
				case VK_UP:
				case VK_DOWN:
					// I find it easier to track if pressing down/up sent
					// you to the top/bottom of the menu respectively
					// up or down-arrow: move into current menu, if any
					TrackPopup(m_iHotItem);
					return TRUE;
				
				case VK_ESCAPE:
					// escape key: exit tracking mode
					SetTrackingState(TRACK_NONE);
					UpdateDisplay(false);
					return TRUE;
				}
			}
			
			// Handle alphanumeric key: invoke menu. Note that Alt-X
			// chars come through as WM_SYSKEYDOWN, plain X as WM_KEYDOWN.
			if ((bAltKey || m_iTrackingState == TRACK_BUTTON) && isalnum(vkey))
			{
				// Alt-X, or else X while in tracking mode
				int nAccelItem = -1;
				GET_ITEMS(ITEMTYPE_POPUP);
				while (itemsBegin < itemsEnd && nAccelItem < 0)
				{
					int nPos = *itemsBegin++;
					CXTMenuBarItem* pItem = m_arrItems[nPos];
					TCHAR cKey;
					if (pItem->GetAccelerator(&cKey))
					{
						if ((WPARAM)_totupper(cKey) == vkey) {
							nAccelItem = nPos;
						}
					}
				}
				if (nAccelItem >= 0)
				{
					TrackPopup(nAccelItem); // found menu mnemonic: track it
					return TRUE;		 // handled
				}
				else 
				if (m_iTrackingState==TRACK_BUTTON && !bAltKey)
				{
					MessageBeep(0);
					return TRUE;
				}
			}
		}
	}

	return FALSE; // not handled, pass along
}

void CXTMenuBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// check to see if a popup item is hit
 	GET_ITEMS(ITEMTYPE_POPUP);
	int iButton = HitTest(point, itemsBegin, itemsEnd);
	if (iButton >= 0)
	{
		if (iButton != m_iPopupTracking)
		{
			// track popup
			TrackPopup(iButton);
		}
		else
		{
			// process double click on this item
			CXTMenuBarItem* pItem = m_arrItems[iButton];
			pItem->OnDblClick();			
		}
		return;
	}

	if (OnMouseMessage(WM_LBUTTONDBLCLK, nFlags, point))
		return;

	CXTControlBar::OnLButtonDblClk(nFlags, point);
}

void CXTMenuBar::OnDblClickWindowMenu(CXTSysMenuBarItem* /*pItem*/)
{
	// we just close the parent frame window
	// probably, the default menu command shall be executed
	CFrameWnd* pFrame = GetTopLevelFrame()->GetActiveFrame();
	ASSERT_VALID(pFrame);
	pFrame->PostMessage(WM_SYSCOMMAND, SC_CLOSE);
	m_iTracking = -1; // stop tracking			
}

CPoint CXTMenuBar::ComputeMenuTrackPoint(const CRect& rcButn, TPMPARAMS& tpm, UINT &nFlags)
{
	tpm.cbSize = sizeof(tpm);
	CPoint pt;
	CRect& rcExclude = (CRect&)tpm.rcExclude;
	rcExclude = rcButn;
	if (xtAfxData.bXPMode) rcExclude.DeflateRect(1, 1);
	
	
	switch ( GetAlignmentStyle( ) )
	{
	case CBRS_ALIGN_BOTTOM:
	case CBRS_ALIGN_TOP:
		pt = CPoint(rcButn.left, rcButn.bottom - 1);
		break;		
	case CBRS_ALIGN_LEFT:
	case CBRS_ALIGN_RIGHT:
		pt = CPoint(rcButn.right - 1, rcButn.top);
		if (xtAfxData.bXPMode) rcExclude.left += 4;
		nFlags &= ~TPM_VERTICAL;
		break;
	}

	if ( ::IsWindow( m_hWnd ) )
	{
		POINT point;
		::GetCursorPos( &point );

		HMONITOR hMonitor = ::MonitorFromPoint( point, MONITOR_DEFAULTTONULL );
		if ( hMonitor )
		{
			MONITORINFOEX info;
			info.cbSize = sizeof( MONITORINFOEX );
			if ( ::GetMonitorInfo( hMonitor, ( MONITORINFO* )&info ) )
			{
				// if the point falls outside of screen coordinates set to zero.
				if( pt.x < info.rcMonitor.left ) {
					pt.x = info.rcMonitor.left;
				}
				if( pt.y < info.rcMonitor.top ) {
					pt.y = info.rcMonitor.top;
				}
			}
		}
	}

	return pt;
}

void CXTMenuBar::OnInitWindowMenu()
{
	CMenu menu;
	menu.Attach((HMENU)m_hMenuShared);
	
	// scan for first window command
	int n = menu.GetMenuItemCount();
	BOOL bAddSeperator = TRUE;
	
	int iPos;
	for (iPos=0; iPos<n; iPos++)
	{
		if (menu.GetMenuItemID(iPos) >= AFX_IDM_FIRST_MDICHILD) {
			bAddSeperator = FALSE;
			break;
		}
	}
	
	// iPos is either first window item, or end if none found delete everything after.
	while (iPos < (int)menu.GetMenuItemCount())
		menu.RemoveMenu(iPos, MF_BYPOSITION);
	
	// get active window so I can check its menu item
	ASSERT(m_hWndMDIClient);
	HWND hWndActive = (HWND)::SendMessage(m_hWndMDIClient,
		WM_MDIGETACTIVE, 0, NULL);
	
	int nIDs[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};
	HWND hWnds[9];
	bool bWins = false;
	int nActive = -1;
	
	HWND hWnd;
	for (hWnd =::GetWindow(m_hWndMDIClient, GW_CHILD); hWnd; hWnd=::GetWindow(hWnd, GW_HWNDNEXT))
	{
		int nID = CWnd::FromHandle(hWnd)->GetDlgCtrlID();
		int nNum = nID - AFX_IDM_FIRST_MDICHILD;
		if(hWnd == hWndActive && nNum > 8)
			nNum = 8;
		if(nNum < 9)
		{
			nIDs[nNum] = nID;
			hWnds[nNum] = hWnd;
			if(hWnd == hWndActive)
				nActive = nNum;
		}
		bWins = true;
	}
	
	if (bWins)
	{
		if(bAddSeperator) 
			menu.InsertMenu(iPos++, MF_BYPOSITION|MF_SEPARATOR);
		
		int nNum;
		for (nNum = 0; nNum < 9; ++nNum)
		{
			CString sWinName, sMenuItem;
			// build item name and add it to the menu
			if(nIDs[nNum] != -1)
			{
				CWnd::FromHandle(hWnds[nNum])->GetWindowText(sWinName);
				sMenuItem.Format(_T("&%d  %s"), nNum + 1, (LPCTSTR)sWinName);
				menu.InsertMenu(nNum+iPos, MF_BYPOSITION, nIDs[nNum], sMenuItem);
			}
		}
		if(nActive != -1)
			menu.CheckMenuItem(nActive+iPos, MF_BYPOSITION|MF_CHECKED);

		if (m_bMoreWindows)
		{
			XT_MANAGE_STATE;

			CString strWindows;
			strWindows.LoadString(XT_IDS_WINDOWS);
			
			menu.AppendMenu(MF_BYPOSITION|MF_SEPARATOR);
			menu.AppendMenu(MF_STRING, AFX_IDM_FIRST_MDICHILD+9, strWindows);
		}
	}
	menu.Detach();
}

void CXTMenuBar::DrawXPFrameControl(CDC* pDC, CRect& r, UINT uStyle, bool bHilite/*=false*/, bool bPushed/*=false*/)
{
    int iSavedDC = pDC->SaveDC();

    COLORREF crBack = xtAfxData.clr3DFace;
	COLORREF crText = (m_bActive) ?
		xtAfxData.clrMenuText : xtAfxData.clrGrayText;
	
    if (bHilite)
    {
        crBack = xtAfxData.clrXPHighlight;
    }
    if (bPushed)
    {
        crBack = xtAfxData.clrXPHighlightPushed;
        crText = xtAfxData.clrXPBarTextPushed;
    }

    pDC->FillSolidRect(&r, crBack);

    if (bHilite || bPushed)
    {
        pDC->Draw3dRect(&r,
            xtAfxData.clrHighlight, xtAfxData.clrHighlight);
    }

	CPoint pt = r.TopLeft();
	
	pt.x += (r.Size().cx-10)/2;
	pt.y += (r.Size().cy-10)/2;
	
	if (r.Size().cx <= 10) {
		pt.x = 0;
	}
	if (r.Size().cy <= 10) {
		pt.y = 0;
	}

	CBrush brush(crText);
				
    switch (uStyle)
    {
    case DFCS_CAPTIONMIN:
        {
			if(xtAfxData.bXPMode)
			{
				CXTIconHandle hIcon( m_imageList.ExtractIcon( 0 ) );
				pDC->DrawState( pt, r.Size(), hIcon, DST_ICON|DSS_MONO, (HBRUSH)brush );
			}
			else
			{
				if (bPushed)
				{
					uStyle |= DFCS_PUSHED;
				}
				pDC->DrawFrameControl(&r, DFC_CAPTION, uStyle);
			}
			if (bHilite)
			{
				ActivateToolTips(m_ptMouse, DFCS_CAPTIONMIN);
			}
        }
        break;

    case DFCS_CAPTIONRESTORE:
        {
			if(xtAfxData.bXPMode)
			{
				CXTIconHandle hIcon( m_imageList.ExtractIcon( 1 ) );
				pDC->DrawState( pt, r.Size(), hIcon, DST_ICON|DSS_MONO, (HBRUSH)brush );
			}
			else
			{
				if (bPushed)
				{
					uStyle |= DFCS_PUSHED;
				}
				pDC->DrawFrameControl(&r, DFC_CAPTION, uStyle);
			}
			if (bHilite)
			{
				ActivateToolTips(m_ptMouse, DFCS_CAPTIONRESTORE);
			}
        }
        break;

    case DFCS_CAPTIONCLOSE:
        {
			if(xtAfxData.bXPMode)
			{
				CXTIconHandle hIcon( m_imageList.ExtractIcon( 2 ) );
				pDC->DrawState( pt, r.Size(), hIcon, DST_ICON|DSS_MONO, (HBRUSH)brush );
			}
			else
			{
				if (bPushed)
				{
					uStyle |= DFCS_PUSHED;
				}
				pDC->DrawFrameControl(&r, DFC_CAPTION, uStyle);
			}
			if (bHilite)
			{
				ActivateToolTips(m_ptMouse, DFCS_CAPTIONCLOSE);
			}
        }
        break;
    }

    pDC->RestoreDC(iSavedDC);
}

void CXTMenuBar::ActivateToolTips(CPoint point, UINT uState)
{
	// don't display if tooltips are not allowed...
	if ((GetBarStyle() & CBRS_TOOLTIPS) == 0) {
		return;
	}

	if (::IsWindow(m_toolTip.m_hWnd))
	{
		::SendMessage(m_toolTip.m_hWnd, TTM_POP, 0, 0);
		
		MSG msg;
		msg.hwnd    = m_hWnd;
		msg.message = WM_MOUSEMOVE;
		msg.wParam  = 0;
		msg.lParam  = MAKELPARAM(point.x,point.y);
		msg.time    = 0;
		msg.pt      = point;
		
		if (m_toolTip.GetToolCount())
		{
			m_toolTip.DelTool(this);
			m_toolTip.Activate(false);
		}

		switch (uState)
		{
		case DFCS_CAPTIONMIN:
			m_toolTip.AddTool(this, XT_IDS_WIND_MINIMIZE);
			m_toolTip.Activate(true);
			break;
		case DFCS_CAPTIONRESTORE:
			m_toolTip.AddTool(this, XT_IDS_WIND_RESTORE);
			m_toolTip.Activate(true);
			break;
		case DFCS_CAPTIONCLOSE:
			m_toolTip.AddTool(this, XT_IDS_WIND_CLOSE);
			m_toolTip.Activate(true);
			break;
		}
		
		// Pass on to tooltip.
		m_toolTip.RelayEvent(&msg);
	}
}


void CXTMenuBar::OnRenderControl(CDC* pDC, CXTControlMenuBarItem* pItem)
{
	const bool bPressed = m_iTracking >= 0 && 
		                  pItem == m_arrItems[m_iTracking] &&
						  m_bDown;
	const bool bIsHot   = (m_iTracking < 0) && IS_HOTITEM(pItem);

	// use DrawFrameControl to draw min/restore/close button
	long window_style = GetMDIWindowStyle();

	UINT uState = pItem->GetFrameControlFlags();

	// we do not rotate frame control so it rect is invariant of menubar orientation
	CRect rc(pItem->GetOrigin(), pItem->GetExtent());

	if (::IsWindow(m_toolTip.m_hWnd))
	{
		::SendMessage(m_toolTip.m_hWnd, TTM_POP, 0, 0);
	}
	
    if ((uState == DFCS_CAPTIONMIN) && !(window_style & WS_MINIMIZEBOX))
    {
        if (window_style & WS_MAXIMIZEBOX)
        {
            pDC->DrawFrameControl(&rc, DFC_CAPTION, uState|DFCS_INACTIVE);
        }
    }
    
    else
    if ((uState == DFCS_CAPTIONRESTORE) && !(window_style & WS_MAXIMIZEBOX))
    {
        if (window_style & WS_MINIMIZEBOX)
        {
            pDC->DrawFrameControl(&rc, DFC_CAPTION, uState|DFCS_INACTIVE);
        }
    }
    
    else
    {
        DrawXPFrameControl(pDC, rc, uState, bIsHot, bPressed);
    }
}

void CXTMenuBar::SetTimerInfo(UINT nElapse, UINT nIDEvent/*=1000*/)
{
	m_nIDEvent = nIDEvent;
	m_nTimeOut = nElapse;
}

#ifdef _DEBUG

void CXTMenuBar::AssertValid() const
{
	CXTControlBar::AssertValid();
	ASSERT(m_hMenu==NULL || ::IsMenu(m_hMenu));
	ASSERT(TRACK_NONE<=m_iTrackingState && m_iTrackingState<=TRACK_POPUP);

	if (m_pFrameHook != NULL) {
		m_pFrameHook->AssertValid();
	}
	if (m_pChildFrameHook != NULL) {
		m_pChildFrameHook->AssertValid();
	}
}

void CXTMenuBar::Dump(CDumpContext& dc) const
{
	CXTControlBar::Dump(dc);
}

#endif

long CXTMenuBar::GetMDIWindowStyle()
{
	if (m_hWndMDIClient)
	{
		HWND hWndActive = (HWND)::SendMessage(m_hWndMDIClient,
			WM_MDIGETACTIVE, 0, NULL);
		
		if (hWndActive)
			return ::GetWindowLong(hWndActive, GWL_STYLE);
	}

	return 0L;
}

BOOL CXTMenuBar::HideCommand(UINT nCommandID)
{
	m_arrHiddenCommands.Add(nCommandID);
	return TRUE;
}


BOOL CXTMenuBar::HideCommands(const UINT* nCommandIDs, int nSize)
{
	ASSERT(nCommandIDs);
	int i;
	for (i = 0; i < nSize; ++i) {
		if (!HideCommand(nCommandIDs[i])) {
			return FALSE;
		}
	}
	return TRUE;
}

void CXTMenuBar::OnPaint() 
{

	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient, GetBackgroundColor());
		
	// Redraw all the items
	GET_ITEMS(ITEMTYPE_POPUP | ITEMTYPE_CONTROL);
	while (itemsBegin < itemsEnd)
	{
		CXTMenuBarItem* pItem = m_arrItems[*itemsBegin++];
		pItem->Render(&memDC);
	}
}

COLORREF CXTMenuBar::GetBackgroundColor() const
{
	// menu bar background is always clr3DFace
	return xtAfxData.clr3DFace;
}

BOOL CXTMenuBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_bMDIMaximized)
	{
		// check if over 
		POINT pt;
		CRect rcClient;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		GetClientRect(rcClient);

		// go directly to CControlBar::OnSetCursor() so CXTControlBar
		// doesn't set a "move" cursor
		GET_ITEMS(ITEMTYPE_POPUP | ITEMTYPE_CONTROL);
		if (HitTest(pt, itemsBegin, itemsEnd) >= 0)
		{
			return CControlBar::OnSetCursor(pWnd, nHitTest, message);
		}
	}
	
	return CXTControlBar::OnSetCursor(pWnd, nHitTest, message);
}

/////////////////////////////////////////////////////////////////////////////
// MFC doesn't do command routing for other process server. ::TrackPopupMenuEx 
// won't accept HWND of other process and we have to determine a message 
// target(ole server window or not) as ::OleCreateMenuDescriptor do.
// First menu(ordinarily File menu) and WindowMenu regarded as container's own menu.
// Some components can't update toolbar button and statusbar pane.
//
HWND CXTMenuBar::OleMenuDescriptor(BOOL& bSend, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if( m_mapMenu2OLEItem.GetCount() == 0 )
		return NULL;

	CWnd* pOleWnd = GetCmdSentOleWnd();
	if (pOleWnd == NULL)
		return NULL;

	HMENU hMenu = NULL;
	if (nMsg == WM_INITMENUPOPUP || nMsg == WM_INITMENU)
		hMenu = (HMENU)wParam;
	else if (nMsg == WM_MENUSELECT)
		hMenu = (HMENU)lParam;

	BOOL	bMap;
	switch (nMsg) 
	{
		case WM_INITMENUPOPUP:
		case WM_INITMENU:
		case WM_MENUSELECT:
			bSend = TRUE;
			if (m_iTrackingState == TRACK_POPUP) 
			{
				if( m_mapMenu2OLEItem.Lookup( m_iButton, bMap) )
					return pOleWnd->GetSafeHwnd();
			}
			break;

		case WM_COMMAND:
			bSend = FALSE;

			if( m_mapMenu2OLEItem.Lookup( m_iPopupTracking, bMap) )
				return pOleWnd->GetSafeHwnd();
	}

	return NULL;// send to frame
}

CFrameWnd* CXTMenuBar::GetOwnerFrame()
{
	// get a pointer to the owner window, if it is NULL or
	// not a frame, return NULL.
	
	CWnd* pWnd = GetOwner();
	if (pWnd == NULL || !pWnd->IsFrameWnd()) {
		return NULL;
	}

	// check to see if we need to get the MDI active frame, then
	// cast the pointer as a frame.
	
	CFrameWnd* pOwnerFrame = NULL;
	if (pWnd->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd)))
	{
		CMDIFrameWnd* pMDIFrame = DYNAMIC_DOWNCAST(CMDIFrameWnd, pWnd);
		pOwnerFrame = pMDIFrame->GetActiveFrame();
	}
	else {
		pOwnerFrame = DYNAMIC_DOWNCAST(CFrameWnd, pWnd);
	}

	if (pOwnerFrame && pOwnerFrame->IsFrameWnd()) {
		return pOwnerFrame;
	}

	return NULL;
}

COleDocument* CXTMenuBar::GetCmdSentOleDoc()
{
	// get a pointer to the owner frame.
	CFrameWnd* pOwnerFrame = GetOwnerFrame();

	// if the frame pointer is valid, drill down until we get
	// the COleClientItem object from the OLE document.
	if (pOwnerFrame != NULL)
 	{
		CDocument* pDoc = pOwnerFrame->GetActiveDocument();
		if ((pDoc != NULL) && pDoc->IsKindOf(RUNTIME_CLASS(COleDocument)))
		{
			COleDocument* pOLEDoc = DYNAMIC_DOWNCAST(COleDocument, pDoc);
			if (pOLEDoc != NULL)
			{
				// Get a pointer to the active view.
				CView* pActiveView = pOwnerFrame->GetActiveView( );
				if ( pActiveView != NULL )
				{
					COleClientItem* pClientItem = pOLEDoc->GetInPlaceActiveItem( pActiveView );
					if ( pClientItem != NULL )
					{
						// return a pointer to the OLE document
						CWnd* pInPlaceWnd = pClientItem->GetInPlaceWindow();
						if (pInPlaceWnd != NULL)
						{
							return pOLEDoc;
						}
					}
				}
			}
		}
	}

	return NULL;
}

CWnd* CXTMenuBar::GetCmdSentOleWnd()
{
	// get a pointer to the owner frame.
	CFrameWnd* pOwnerFrame = GetOwnerFrame();

	// if the frame pointer is valid, drill down until we get
	// the COleClientItem object from the OLE document.
	if (pOwnerFrame != NULL)
 	{
		CDocument* pDoc = pOwnerFrame->GetActiveDocument();
		if ((pDoc != NULL) && pDoc->IsKindOf(RUNTIME_CLASS(COleDocument)))
		{
			COleDocument* pOLEDoc = DYNAMIC_DOWNCAST(COleDocument, pDoc);
			if (pOLEDoc != NULL)
			{
				// Get a pointer to the active view.
				CView* pActiveView = pOwnerFrame->GetActiveView( );
				if ( pActiveView != NULL )
				{
					COleClientItem* pClientItem = pOLEDoc->GetInPlaceActiveItem(pOwnerFrame);
					if ((pClientItem != NULL))
					{
						// return a pointer to the inplace window.
						CWnd* pInPlaceWnd = pClientItem->GetInPlaceWindow();
						if (pInPlaceWnd != NULL)
						{
							return pInPlaceWnd;
						}
					}
				}
			}
		}
	}

	return NULL;
}

LRESULT CXTMenuBar::OnToolHitTest(CPoint, TOOLINFO*) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	return -1;
}

BOOL CXTMenuBar::FillCommandRoutingMap()
{
	m_mapMenu2OLEItem.RemoveAll();

	COleDocument* pDoc = GetCmdSentOleDoc();
	if( pDoc == NULL )
		return FALSE;

	TCHAR szContainerMenuName[64];
	TCHAR szServerMenuName[64];
	TCHAR szContainerSubMenuName[64];
	TCHAR szServerSubMenuName[64];

	CDocTemplate *pTemplate = pDoc->GetDocTemplate();
	int iCombined;
	for ( iCombined = 0; iCombined < ::GetMenuItemCount(m_hMenu); iCombined++ )
	{
		BOOL  bFound = FALSE;
		memset(szContainerMenuName, 0, sizeof(szContainerMenuName));
		memset(szServerMenuName, 0, sizeof(szServerMenuName));

		if (::GetMenuString(m_hMenu, iCombined, szContainerMenuName, _countof(szContainerMenuName)-1, MF_BYPOSITION))
		{
			int iInPlace;
			for ( iInPlace = 0; iInPlace < ::GetMenuItemCount(pTemplate->m_hMenuInPlace); iInPlace++ )
			{
				if ( !::GetMenuString(pTemplate->m_hMenuInPlace, iInPlace, szServerMenuName, _countof(szServerMenuName)-1, MF_BYPOSITION))
					continue;
				
				// compare the menu name
				if( !_tcscmp( szContainerMenuName, szServerMenuName) )
				{
					HMENU hContainerSubMenu = GetSubMenu( m_hMenu, iCombined ); 
					HMENU hServerSubMenu = GetSubMenu( pTemplate->m_hMenuInPlace, iInPlace ); 

					// compare the item count in the submenu
					int iItemCount = ::GetMenuItemCount( hContainerSubMenu );
					if( iItemCount == ::GetMenuItemCount( hServerSubMenu ) )
					{
						bFound = TRUE;

						// for each item in the submenu
						for ( iItemCount--; iItemCount >= 0; iItemCount-- )
						{								
							memset(szContainerSubMenuName, 0, sizeof(szContainerSubMenuName));
							memset(szServerSubMenuName, 0, sizeof(szServerSubMenuName));

							// compare the name
							if (::GetMenuString(hContainerSubMenu, iItemCount, szContainerSubMenuName, _countof(szContainerSubMenuName)-1, MF_BYPOSITION) &&
								::GetMenuString(hServerSubMenu, iItemCount, szServerSubMenuName, _countof(szServerSubMenuName)-1, MF_BYPOSITION))
							{
								if( _tcscmp( szContainerSubMenuName, szServerSubMenuName) )
								{
									bFound = FALSE;
									break;
								}
								else
								{
									// and command id
									if( GetMenuItemID( hContainerSubMenu, iItemCount ) != GetMenuItemID( hServerSubMenu, iItemCount ) )
									{
										bFound = FALSE;
										break;
									}
								}
							}
						}								
					}

					// all items are equal
					if( bFound )
						break;
				}
			}
		}

		if( bFound == FALSE )
			m_mapMenu2OLEItem.SetAt( iCombined + 1, TRUE );
	}

	return TRUE;
}

BOOL CXTMenuBar::OnMouseMessage(UINT message, UINT nFlags, CPoint pt)
{
	UNREFERENCED_PARAMETER(nFlags);
					 
	// remember to check if the hot item must be reset once mouse moves outside the area
	m_bDelayCheckMouse = true;

	// not maximized, don't even bother 
	if (!m_bMDIMaximized)
		return FALSE;

	ASSERT_VALID(this);

	long window_style = GetMDIWindowStyle();
	
	if (message == WM_LBUTTONDOWN || message == WM_LBUTTONDBLCLK) 
	{
		GET_ITEMS(ITEMTYPE_CONTROL);
		m_iTracking = HitTest(pt, itemsBegin, itemsEnd); // which button?
		m_bDown = m_iTracking >= 0;
	}
	
	// No system menu, return FALSE.
	if (!(window_style & WS_SYSMENU))
	{
		m_iTracking = -1;
		return FALSE;
	}
	
	CXTControlMenuBarItem* pItem = NULL;
	if (m_iTracking >= 0)
	{
		pItem = (CXTControlMenuBarItem*)m_arrItems[m_iTracking];
	}
	PressItem((m_bDown) ? m_iTracking : -1);

	// If pItem is NULL we need go no further.
	if (pItem == NULL) {
		return FALSE;
	}
	
	// No minimize box...
	if ((window_style & WS_MINIMIZEBOX) == 0)
	{
		if (pItem->GetSysCmd() == SC_MINIMIZE)
		{
			return FALSE;
		}
	}
	
	// No maximize box...
	if ((window_style & WS_MAXIMIZEBOX) == 0)
	{
		if (pItem->GetSysCmd() == SC_RESTORE)
		{
			return FALSE;
		}
	}

	if (message == WM_LBUTTONDBLCLK)
	{
		if (pItem->GetSysCmd() == SC_CLOSE)
		{
			CFrameWnd* pFrame = GetTopLevelFrame()->GetActiveFrame();
			ASSERT_VALID(pFrame);
			pFrame->PostMessage(WM_SYSCOMMAND, SC_CLOSE);
			m_iTracking = -1; // stop tracking
			m_bDown = false;
			return TRUE;	  // handled (eat)
		}
	}

	else if (message == WM_LBUTTONDOWN)
	{
		if (m_iTracking>=0)
		{
			// use clicked min, restore or close button: go into tracking mode
			UpdateWindow();
			m_bDown = true;						  // remember state
			SetCapture();				  // grab mouse input
			return TRUE;						// handled
		}
		
	}

	else if ((message == WM_MOUSEMOVE) && m_iTracking>=0)
	{
		// mouse moved, and I am tracking: possibly draw button up/down
		CRect rectItem = GetItemRect(pItem);
		m_bDown = rectItem.PtInRect(pt) != 0;
		PressItem((m_bDown) ? m_iTracking : -1);
		return TRUE; // handled
		
	}

	else if (message == WM_LBUTTONUP && m_iTracking>=0)
	{
		// user released the mouse and I am tracking: do button command
		ReleaseCapture(); // let go the mouse

		// if button was down when released: draw button up, and do system cmd
		if (m_bDown)
		{			
			CFrameWnd* pFrame = GetTopLevelFrame()->GetActiveFrame();
			ASSERT_VALID(pFrame);
			
			pFrame->PostMessage(WM_SYSCOMMAND, pItem->GetSysCmd());
		}
		m_bDown = false;
		PressItem(-1);
		m_iTracking = -1; // stop tracking
		return TRUE;	  // handled (eat)
	}
	
	return FALSE;
}

void CXTMenuBar::OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
{
	// Reset tracking state if toolbar floats/docks
	if ((dwOldStyle ^ dwNewStyle) & CBRS_FLOATING)
	{
		ResetTrackingState();
	}

	// Do default
	CXTControlBar::OnBarStyleChange(dwOldStyle, dwNewStyle);
}

void CXTMenuBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CXTControlBar::OnSettingChange(uFlags, lpszSection);
	
	// Reload the menu bar if the system settings have changed.
	LoadMenu(m_hMenu, m_hMenuDefault);

	// WINBUG: In some cases mainframe does not recalc itself correctly
	// the result is that maximized MDI children do not adjust themselves
	// to the new client area of the frame if the size has changed.  To 
	// fix this we will resize the main frame window to force the 
	// necessary adjustments to be made.
	
	CFrameWnd* pTopFrameWnd = GetTopLevelFrame();
	if (pTopFrameWnd && pTopFrameWnd->IsFrameWnd())
	{
		CRect r;
		pTopFrameWnd->GetWindowRect(&r);

		pTopFrameWnd->SetWindowPos(NULL, 0,0,r.Width()+1,r.Height()+1,
			SWP_NOMOVE|SWP_FRAMECHANGED);

		pTopFrameWnd->SetWindowPos(NULL, 0,0,r.Width(),r.Height(),
			SWP_NOMOVE|SWP_FRAMECHANGED);
	}
}

bool CXTMenuBar::OnAddedToRebar(REBARBANDINFO* pRBBI, bool bHorz)
{
	// Compute default sizing
	CXTControlBar::OnAddedToRebar(pRBBI, bHorz);

	return true;
}

void CXTMenuBar::OnRebarBandResized(XT_REBARSIZECHILDINFO* pInfo, bool bHorz)
{
	m_nMRUMaxWidth = pInfo->pSizeInfo->rcChild.right - pInfo->pSizeInfo->rcChild.left,
	CalcItemLayout(bHorz);
	Invalidate();
}

bool CXTMenuBar::IsFullWindowDrag()
{
	// menu bar uses full window when dragging
	return true;
}

bool CXTMenuBar::GetHotSpot(LPSIZE pSize)
{
	pSize->cx /= 2;
	CRect borders(0,0,0,0);
	CMiniFrameWnd::CalcBorders(&borders);
	pSize->cy = -borders.top / 2;
	return true;
}

void CXTMenuBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	UNREFERENCED_PARAMETER(bCalcValidRects);

	// calculate border space (will add to top/bottom, subtract from right/bottom)
	CRect rect; rect.SetRectEmpty();
	BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	CXTControlBar::CalcInsideRect(rect, bHorz);
	ASSERT(xtAfxData.iComCtlVersion != -1);
	ASSERT(xtAfxData.iComCtlVersion >= VERSION_IE401 || rect.top >= 2);

	// adjust non-client area for border space
	lpncsp->rgrc[0].left   += rect.left;
	lpncsp->rgrc[0].top    += rect.top;
	lpncsp->rgrc[0].right  += rect.right;
	lpncsp->rgrc[0].bottom += rect.bottom;
}

LRESULT CXTMenuBar::OnNcHitTest(CPoint point)
{
	UNREFERENCED_PARAMETER(point);
	return HTCLIENT;
}

bool CXTMenuBar::HasCmdHandlers(CWnd* pWnd)
{
    UNREFERENCED_PARAMETER(pWnd);
	return true;
}
