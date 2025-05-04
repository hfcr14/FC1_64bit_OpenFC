// XTCoolMenu.cpp : implementation of the CXTCoolMenu class.
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
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTCoolMenu.h"
#include "XTFunctions.h" 
#include "XTAccelManager.h"
#include "XTMenuBar.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTFrameImpl.h"
#include "XTRegistryManager.h"
#include "XTWndShadow.h"
#include "XTPriv.h"
#include "XTOSVersionInfo.h"
#include "XTColorRef.h"
#include "XTImageManager.h"
#include "XTDrawHelpers.h"
#include "XTHelpers.h"
#include "XTAnimationMemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifndef SPI_GETMENUANIMATION
#define SPI_GETMENUANIMATION  0x1002
#endif

#ifndef SPI_SETMENUANIMATION
#define SPI_SETMENUANIMATION  0x1003
#endif

#ifndef SPI_GETDROPSHADOW
#define SPI_GETDROPSHADOW     0x1024
#endif

#ifndef SPI_SETDROPSHADOW
#define SPI_SETDROPSHADOW     0x1025
#endif

#define MFT_TOP               0x0001
#define MFT_BOTTOM            0x0002
#define MFT_HIDDEN            0x0004

#define CLASS_NAME_POPUP      _T("#32768")

const long XT_ITEMID = MAKELONG( MAKEWORD( _T( 'x' ),_T( 't' ) ),MAKEWORD( _T( 'r' ),_T( 'e' ) ) );

const int CXGAP_NORM		  = 2;	// Num pixels between button and text
const int CXTEXTMARGIN_NORM	  = 5;	// Num pixels after hilite to start text
const int CXBUTTONMARGIN_NORM = 1;	// Num pixels wider button is than bitmap
const int CYBUTTONMARGIN_NORM = 1;	// Ditto for height
const int CXGAP_XP   	      = 8;
const int CXRMARGIN_XP        = 20;
const int CXBUTTONMARGIN_XP   = 4;
const int CYBUTTONMARGIN_XP   = 3;


// Tells if given window is actually a menu popup window
static bool IsWindowMenuPopup(HWND hWnd)
{
	TCHAR class_name[128];
	GetClassName(hWnd, class_name, sizeof(class_name)/sizeof(TCHAR));
	bool bPopup = _tcscmp(class_name, CLASS_NAME_POPUP) == 0;
	return bPopup;
}

// Tells if the given menu is actually a cool menu.
static bool IsCoolMenu(HMENU hMenu)
{
	int nCount = ::GetMenuItemCount( hMenu );
	
	int nPos;
	for ( nPos = 0; nPos < nCount; nPos++ )
	{
		XT_MENUITEMINFO info;
		info.fMask = MIIM_DATA;
		::GetMenuItemInfo( hMenu, nPos, TRUE, &info );
		CXTItemData* pItemData = ( CXTItemData* )info.dwItemData;
		if ( pItemData && pItemData->IsXTItemData( ) )
		{
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// Helper class to initialize cool menu internals
/////////////////////////////////////////////////////////////////////////////

static class CXTCoolMenuInit
{
public:
	CXTCoolMenuInit()
	{
		CXTCoolMenu::HookCBTHook();
	}

	~CXTCoolMenuInit()
	{
		CXTCoolMenu::UnhookCBTHook();
	}
} __initCoolMenu;

/////////////////////////////////////////////////////////////////////////////
// Helper class used to subclass actual popup windows
/////////////////////////////////////////////////////////////////////////////

BOOL      CXTPopupMenu::m_bSysMenuAnimation        = FALSE;
BOOL      CXTPopupMenu::m_bSysDropShadows          = FALSE;
BOOL      CXTPopupMenu::m_bSysDropShadowsAvailable = FALSE;
int	      CXTPopupMenu::m_iRefCount                = 0;

CXTPopupMenu::CXTPopupMenu()
{
	if (m_iRefCount == 0)
	{
		// temporarily disable animations
		::SystemParametersInfo(SPI_GETMENUANIMATION, 0, 
			&m_bSysMenuAnimation, 0);
		::SystemParametersInfo(SPI_SETMENUANIMATION, 0, FALSE, 0);
		
		m_bSysDropShadowsAvailable = CXTOSVersionInfo::Get().IsWinXPOrGreater();

		::SystemParametersInfo(SPI_GETDROPSHADOW, 0, &m_bSysDropShadows, 0);
		
		//BOOL bShadow = xtAfxData.bXPMode || !xtAfxData.bMenuShadows? FALSE: m_bSysDropShadowsAvailable; 
		::SystemParametersInfo(SPI_SETDROPSHADOW, 0, (PVOID)FALSE, 0);
	
	}
	
	++m_iRefCount;
	
	m_hMenu       = CXTCoolMenu::m_hMenuLast;
	m_bOffset     = true;
	m_pParent     = NULL;
	m_bAnimationFinished = FALSE;
	m_rectExclude.SetRectEmpty();
}

CXTPopupMenu::~CXTPopupMenu()
{
	if (--m_iRefCount == 0)
	{
		::SystemParametersInfo(SPI_SETDROPSHADOW, 0, 
			(PVOID)m_bSysDropShadows, 0);
		::SystemParametersInfo(SPI_SETMENUANIMATION, 0, 
			(PVOID)m_bSysMenuAnimation, 0);
		
		// Set the last menu to NULL since no more popups are left.
		CXTCoolMenu::m_hMenuLast = NULL;
		
	}
	
}

void CXTPopupMenu::SetParams(HMENU hMenu)
{
	m_hMenu = hMenu;
	CXTCoolMenu::RemoveBorder(m_hWnd);
}

void CXTPopupMenu::SetCoolMenuParent(CXTCoolMenu *pParent)
{
	m_pParent = pParent;
}

void CXTPopupMenu::HideBeforeExpand()
{
}

void CXTPopupMenu::ShowShadows()
{
	if (!m_bAnimationFinished)
		return;

	if (xtAfxData.bMenuShadows)
	{
		if (xtAfxData.bXPMode)
		{

			CXTEmptyRect rc;
			rc.CopyRect(m_rectExclude);

			XTShadowsManager()->SetShadow(m_hWnd, rc);
			if (!rc.IsRectEmpty())
				XTShadowsManager()->SetShadow(rc, m_hWnd);
		} 
		else if (CXTPopupMenu::m_bSysDropShadowsAvailable || xtAfxData.bUseSolidShadows == FALSE)
		{
			XTShadowsManager()->SetShadow(m_hWnd);
		}
	}
}

LRESULT CXTPopupMenu::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	static CPoint ptLast;

	// if this is not a cool menu, need not go any further, use default.
	if (!IsCoolMenu(m_hMenu))
	{
		return CXTWndHook::WindowProc(message, wParam, lParam);
	}
	
	if (message == WM_KEYDOWN)
	{
		CXTCoolMenu::m_bLastMouseEvent = FALSE;
	}

	else
	{
		CPoint pt;
		GetCursorPos(&pt);
		if (pt != ptLast) CXTCoolMenu::m_bLastMouseEvent = TRUE;		
		ptLast = pt;
	}

//	if ((message == WM_LBUTTONDOWN  ||  message == WM_LBUTTONUP  ||
//		(message >= 0x1eb  &&  message <= 0x1f0))  &&  
//		CXTCoolMenu::m_bChevronSelected)
//	{
//		CRect r;
//		::GetClientRect(m_hWnd, &r);
//		if (r.PtInRect(CPoint(lParam)))
//		{
//			// user clicked on a chevron: we won't let the menu actually
//			// process this - we'll post ourselves a message
//			if (!CXTCoolMenu::m_bIs95orNT4) // NT4 + Win95 don't like this
//			{
//				ASSERT(m_pParent != NULL);
//				if (m_pParent)
//				{
//					::SendMessage(m_pParent->m_hWnd, WM_COMMAND,
//						(WPARAM)XT_IDC_CHEVRON, (LPARAM)0);
//					message = WM_NULL;
//				}
//			}
//		}
//	}

	if (message == WM_NCPAINT)
	{
		if (!m_bAnimationFinished && !CXTCoolMenu::m_bIs95orNT4)
		{
			return 1;
		}
		
		if (xtAfxData.bXPMode && m_iRefCount != 0 &&  m_hMenu != NULL && m_hWnd)
		{
			CWnd* pWnd = CWnd::FromHandle(m_hWnd);
			CWindowDC dc(pWnd);

			OnDrawBorder(&dc);
			return 1;
		}
	}

	if (message == WM_ERASEBKGND && !CXTCoolMenu::m_bIs95orNT4)
	{
		return TRUE; 
	}

	if (message == WM_PAINT )
	{
		if (CXTCoolMenu::m_bIs95orNT4 )
		{
			if (!m_bAnimationFinished)
			{
				m_bAnimationFinished = TRUE;
				ShowShadows();
			}
		} 
		else
		{
			CWnd* pWnd = CWnd::FromHandle(m_hWnd);
			CXTClientRect rectClient(pWnd);
			CPaintDC dc(pWnd);
			COLORREF clrMenuBack = xtAfxData.bXPMode && m_hMenu != 0 ? xtAfxData.clrXPMenuTextBack : xtAfxData.clrMenu;
						
			CXTMemDC memDC(&dc, rectClient, clrMenuBack);
			//::SendMessage(m_hWnd, WM_PRINT, (WPARAM)memDC.m_hDC, PRF_CLIENT|PRF_ERASEBKGND);
			::SendMessage(m_hWnd, WM_PRINTCLIENT, (WPARAM)memDC.m_hDC, 0);
			
			if (!m_bAnimationFinished)
			{
				CXTWindowRect rectWindow(pWnd);
				rectWindow.OffsetRect(-rectWindow.TopLeft());
				CWindowDC dc(pWnd);
				CXTAnimationMemDC animationDC(&dc, rectWindow, clrMenuBack);
				
				// Draw window content
				CSize sz = rectWindow.Size() - rectClient.Size();
				animationDC.BitBlt(sz.cx/2, sz.cy/2, rectClient.Width(), rectClient.Height(), &memDC, 0, 0, SRCCOPY);
				
				// Draw nonclient area
				if (xtAfxData.bXPMode) OnDrawBorder(&animationDC);
				else ::SendMessage(m_hWnd, WM_PRINT, (WPARAM)animationDC.m_hDC, PRF_NONCLIENT);
				
				// Animation
				animationDC.Animate(CXTCoolMenu::m_nAnimationType, CXTCoolMenu::m_nAnimationSteps, CXTCoolMenu::m_nAnimationDelay);
				
				m_bAnimationFinished = TRUE;
				ShowShadows();
			}
			
			return TRUE;
		}
	}

	LRESULT lRes = CXTWndHook::WindowProc(message, wParam, lParam);
	
	if ( ( m_iRefCount == 0 ) || ( m_hMenu == NULL ) )
	{
		// Short circuit if this is not a cool menu (for instance, if 
		// this is a system menu).  When the OnInitMenuPopup() function
		// detects such menus, it does not set the m_hMenu variable.
		return lRes;
	}
	
	switch (message)
	{
	case WM_WINDOWPOSCHANGING:
		{ 
			if ( xtAfxData.bXPMode ) // offset menu position ala xp mode.
			{
				LPWINDOWPOS lpwndpos = ( LPWINDOWPOS )lParam;
                if (lpwndpos != NULL)
                {			
                    if ( m_iRefCount > 1 && lpwndpos->x > 1 && m_bOffset == true )
                    {
                        lpwndpos->x += 1;
                        lpwndpos->y += 2;

                        m_bOffset = false;
                    }
                    lpwndpos->cx -= 4;				
                }
            }
		}
		break;
		
	case WM_WINDOWPOSCHANGED:
		{
			CXTCoolMenu::RemoveBorder(m_hWnd);
			break;
		}
		
	case 0xAF: /* undocumented internal WinXP menu message */
		
		// post to ourselves
		if (xtAfxData.bXPMode  &&  xtAfxData.bWinXP)
		{
			::PostMessage(m_hWnd, WM_XT_FIX_XP_PAINT, 0 ,0);
		}
		
		break;
		
	case WM_XT_FIX_XP_PAINT:
		{
			DisableXPEffects();
			break;
		}
	}
	
	return lRes;
}

void CXTPopupMenu::OnDrawBorder(CDC* pDC)
{
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	
	CXTWindowRect rc(pWnd);
	rc.OffsetRect(-rc.TopLeft());
	pDC->Draw3dRect(rc, xtAfxData.clrXPMenuBorder, xtAfxData.clrXPMenuBorder);

	CXTWindowRect rcWindow(m_hWnd);
	::DrawAdjacentBorder(pDC, xtAfxData.clrXPBarFace, m_rectExclude, rcWindow);
}

HWND CXTPopupMenu::GetHWnd()
{
	return m_hWnd;
}

HMENU CXTPopupMenu::GetHMenu()
{
	return m_hMenu;
}

void CXTPopupMenu::DisableXPEffects()
{
/* this trick will ensure that system menu effects don't do 
	bad things to our custom drawn popups */
	ASSERT(m_hMenu != NULL);
	if (m_hMenu != NULL)
	{
		TCHAR szItemName[256];
		XT_MENUITEMINFO info;
		int i;
		for (i = 0; i < ::GetMenuItemCount(m_hMenu); ++i)
		{
			info.fMask      = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE | MIIM_STATE;
			info.dwTypeData = szItemName;
			info.cch        = _countof(szItemName);
			::GetMenuItemInfo(m_hMenu, i, TRUE, &info);
			::SetMenuItemInfo(m_hMenu, i, TRUE, &info);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CXTItemData helper class
/////////////////////////////////////////////////////////////////////////////
CXTItemData::CXTItemData()
{
	m_strText    = _T("");
	m_bHidden    = FALSE;
	m_hMenu      = NULL;
	m_hSubMenu	 = NULL;
	m_nPosition  = (UINT)-1;
	m_itemID     = XT_ITEMID;
	m_commandID  = (UINT)-1;
	m_dwType     = 0L;
	m_dwState    = 0L;
	m_dwHideType = 0L;
	m_iMBAlign   = 0;
}

CXTItemData::~CXTItemData()
{
	m_strText.Empty();
}

BOOL CXTItemData::IsXTItemData()
{
	return (m_itemID == XT_ITEMID);
}

/////////////////////////////////////////////////////////////////////////////
// CXTCoolMenu
/////////////////////////////////////////////////////////////////////////////

int              CXTCoolMenu::m_iMenuBarAlign     = 0;
int              CXTCoolMenu::m_iIgnoreCount      = 0;
bool             CXTCoolMenu::m_bShowAll          = false;
bool             CXTCoolMenu::m_bAllowIntelligent = false;
bool             CXTCoolMenu::m_bIsPopup          = true;
bool             CXTCoolMenu::m_bChevronSelected  = false;
bool             CXTCoolMenu::m_bIs95orNT4        = CXTOSVersionInfo::Get().IsWin95() || CXTOSVersionInfo::Get().IsWinNT4(); 
HHOOK            CXTCoolMenu::m_hCBTHook          = NULL;
HMENU            CXTCoolMenu::m_hMenuLast         = NULL;
CSize            CXTCoolMenu::m_szPopupBorder     = CSize( ::GetSystemMetrics( SM_CXDLGFRAME ), ::GetSystemMetrics( SM_CYDLGFRAME ) );
CSize            CXTCoolMenu::m_szIconMargin      = CSize( -1,-1 );
CXTPopupMenu*    CXTCoolMenu::m_pPopupLast        = NULL;
CXTPopupMenuList CXTCoolMenu::m_listPopups;
bool			 CXTCoolMenu::m_bLastMouseEvent	  = TRUE;
CRect			 CXTCoolMenu::m_rectExclude		 = CRect(0, 0, 0, 0);
bool			 CXTCoolMenu::m_bInstalled		  = FALSE;

int CXTCoolMenu::m_nAnimationDelay = 100;
int CXTCoolMenu::m_nAnimationSteps = 5;

int CXTCoolMenu::m_nAnimationType = animateWindowsDefault;


CXTCoolMenu::CXTCoolMenu(BOOL bNoToolbar) : m_bHasMnemonics(FALSE)
{
	UNUSED_ALWAYS(bNoToolbar);

	m_nRecentList    = 5;
	m_bIntelligent   = FALSE;
	m_bTimerActive   = FALSE;
	m_sizeBmp        = CSize(16,15);
	m_sizeBtn        = CSize(16,15);
	m_nIDEvent       = 1000;
	m_nTimeOut       = 750;

	if (CXTOSVersionInfo::Get().IsWin2KOrGreater())
	{
		::SystemParametersInfo(SPI_GETMENUUNDERLINES,
			0, &m_bHasMnemonics, 0);
	}
}

CXTCoolMenu::~CXTCoolMenu()
{
	while(!m_itemList.IsEmpty()) {
		CXTItemData* pItemData = m_itemList.RemoveHead();
		SAFE_DELETE(pItemData);
	}

	Destroy();
}

IMPLEMENT_DYNAMIC(CXTCoolMenu, CXTWndHook)

void CXTCoolMenu::RemoveBorder(HWND hWnd)
{
	if (xtAfxData.bXPMode)
	{
        const UINT SWP_STYLE = 
            SWP_FRAMECHANGED |
            SWP_NOSIZE       |
            SWP_NOMOVE       |
            SWP_NOZORDER     |
            SWP_NOACTIVATE;

		const DWORD WS_REMOVE_FLAGS =
			WS_DLGFRAME   |
			WS_THICKFRAME |
			WS_BORDER;

		const DWORD WS_EX_REMOVE_FLAGS =
			WS_EX_STATICEDGE |
			WS_EX_WINDOWEDGE |
			WS_EX_DLGMODALFRAME;

		CWnd::ModifyStyle(
			hWnd, WS_REMOVE_FLAGS, 0L, SWP_STYLE );

		CWnd::ModifyStyleEx(
			hWnd, WS_EX_REMOVE_FLAGS, WS_EX_STATICEDGE, SWP_STYLE );
	}
}


void CXTCoolMenu::HookCBTHook()
{
	if (!m_hCBTHook)
	{
		m_hCBTHook = ::SetWindowsHookEx(WH_CBT, CBTHook, 
			NULL, ::GetCurrentThreadId());
	}
}

void CXTCoolMenu::UnhookCBTHook()
{
	if (m_hCBTHook)
	{
		::UnhookWindowsHookEx(m_hCBTHook);    
		m_hCBTHook = NULL;
	}
}


LRESULT CALLBACK CXTCoolMenu::CBTHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	static int cbt_counter = 0;

	LRESULT lRes;

	++cbt_counter; 

	if (cbt_counter == 1 && m_bInstalled)
	{
		if (nCode == HCBT_CREATEWND)
		{
			HWND hWnd = (HWND)wParam;
			lRes = CallNextHookEx(m_hCBTHook, nCode, wParam, lParam);
			if (::IsWindowMenuPopup(hWnd))
			{
				CXTPopupMenu *pPopup = new CXTPopupMenu();
				
				pPopup->HookWindow(hWnd);
				m_listPopups.AddTail(pPopup);
				if (m_hMenuLast == NULL)  
				{
					m_pPopupLast = pPopup;
				}
				else
				{
					m_pPopupLast = NULL;
				}
				
				if (CXTCoolMenu::m_iMenuBarAlign)
				{
					pPopup->m_rectExclude = CXTCoolMenu::m_rectExclude;
					CXTCoolMenu::m_iMenuBarAlign = 0;
				}
				//pPopup->ShowShadows();
			}
			goto out;
		}

		if (nCode == HCBT_DESTROYWND)
		{
			HWND hWnd = (HWND)wParam;
			if (!m_listPopups.IsEmpty()  &&  ::IsWindowMenuPopup(hWnd))
			{
				// we expect the popups to be destroyed in reverse order,
				// hence this assert.  the code will try to handle other
				// situations gracefully (and will most probably succeed).
				// however, it's prudent to check this condition
				ASSERT(m_listPopups.GetTail()->GetHWnd() == hWnd);

				POSITION pos = m_listPopups.GetHeadPosition();
				while (pos)
				{
					POSITION posNext = pos;
					if (m_listPopups.GetNext(posNext)->GetHWnd() == hWnd)
					{
						break;
					}
					pos = posNext;
				}
				ASSERT(pos != NULL);  // found the popup in the list?
				if (pos)
				{
					delete m_listPopups.GetAt(pos);
					m_listPopups.RemoveAt(pos);
				}
			}
			lRes = CallNextHookEx(m_hCBTHook, nCode, wParam, lParam);
			goto out;
		}
	}

	lRes = CallNextHookEx(m_hCBTHook, nCode, wParam, lParam);

out:
	--cbt_counter;
	return lRes;
}



void CXTCoolMenu::Destroy()
{
	m_sizeBmp = CSize(0,0);
	m_sizeBtn = CSize(0,0);
}


BOOL CXTCoolMenu::HookWindow(HWND hWnd)
{	
	m_bInstalled = TRUE;
	return CXTWndHook::HookWindow(hWnd);
}

void CXTCoolMenu::Install(CFrameWnd* pFrame)
{
	HookWindow(pFrame);
	m_bInstalled = TRUE;
}

BOOL CXTCoolMenu::LoadToolbars(const UINT* nIDToolBars, int nSize, bool bEnabled/*=true*/)
{
	ASSERT( nIDToolBars );
	
	int i;
	for ( i = 0; i < nSize; ++i )
	{
		if ( !LoadToolbar( nIDToolBars[ i ], bEnabled ) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CXTCoolMenu::AddImageToMap(UINT nIDCmd, HICON hIcon, CSize sz, bool bEnabled/*=true*/)
{
	XTImageManager()->AddIcon(nIDCmd, hIcon, sz, bEnabled);
	::DestroyIcon( hIcon );
}

BOOL CXTCoolMenu::LoadToolbar(UINT nIDToolbar, bool bEnabled/*=true*/)
{
	return XTImageManager()->LoadToolBarIcons(nIDToolbar, bEnabled);
}

BOOL CXTCoolMenu::LoadToolbar(CToolBarCtrl& toolBar, bool bEnabled/*=true*/)
{
	
	CImageList *pIL = CImageList::FromHandle( ( HIMAGELIST )
		::SendMessage( toolBar.m_hWnd, TB_GETIMAGELIST, 0, 0 ) );
	
	if ( pIL == NULL )
	{
		TRACE0( "Could not access toolbar image list!\n" );
		return FALSE;
	}

	if ( pIL->GetImageCount( ) == 0 )
	{
		return TRUE;
	}

	IMAGEINFO info;
	pIL->GetImageInfo( 0, &info );
	CRect rcImage = info.rcImage;
	m_sizeBmp = rcImage.Size( );

	int i;
	for ( i = 0; i < toolBar.GetButtonCount( ); i++ )
	{
		TBBUTTON btninfo;
		VERIFY( toolBar.GetButton( i, &btninfo ) );

		if ( btninfo.fsStyle & TBSTYLE_SEP )
		{
			continue;
		}

		if ( btninfo.idCommand >= 0 && btninfo.iBitmap >= 0)
		{
			// AddImageToMap destroys the temporary icon
			HICON hIcon = pIL->ExtractIcon( btninfo.iBitmap );
			ASSERT(hIcon != NULL);
			AddImageToMap( btninfo.idCommand, hIcon, m_sizeBmp, bEnabled );
		}
	}

	return TRUE;
}

BOOL CXTCoolMenu::AppendImageList(HICON hIcon, UINT uCmdID, bool bEnabled/*=true*/)
{
	ASSERT( hIcon  != NULL );
	ASSERT( uCmdID != 0 );

	
	if ( hIcon &&  uCmdID )
	{
		// LoadToolbar() must be called at least once for the size of the
		// icon to be determined!
		ASSERT( m_sizeBmp.cx != 0 );

		// AddImageToMap destroys the temporary icon.
		AddImageToMap( uCmdID, hIcon, m_sizeBmp, bEnabled );

		return TRUE;
	}

	return FALSE;
}

BOOL CXTCoolMenu::AppendImageList(UINT nBitmapID, UINT* arCmdIDs, int nSize, bool bEnabled/*=true*/)
{
	// Load the bitmap.
	CBitmap bmp;
	bmp.LoadBitmap(nBitmapID);
	
	return AppendImageList( bmp, arCmdIDs, nSize, bEnabled );
}

BOOL CXTCoolMenu::AppendImageList(HBITMAP hBitmap, UINT* arCmdIDs, int nSize, bool bEnabled/*=true*/)
{
	ASSERT( hBitmap != NULL );
	ASSERT( arCmdIDs != NULL );
	ASSERT( nSize != 0 );

	if ( hBitmap  &&  arCmdIDs )
	{
		// LoadToolbar() must be called at least once for the size of the
		// bitmaps to be determined!
		ASSERT( m_sizeBmp.cx != 0 );

		CImageList ilTemp;
		if ( !ilTemp.Create( m_sizeBmp.cx, m_sizeBmp.cy,
			ILC_COLOR32 | ILC_MASK, 0, 10 ) )
		{
			return FALSE;
		}

		ilTemp.Add( CBitmap::FromHandle( hBitmap ), RGB(0xC0, 0xC0, 0xC0) );

		// Add ID to Map.
		int i;
		for ( i = 0; i < nSize; ++i )
		{
			if ( arCmdIDs[ i ] > 0 )
			{
				// AddImageToMap destroys the temporary icon
				AddImageToMap( arCmdIDs[ i ], ilTemp.ExtractIcon( i ), m_sizeBmp, bEnabled );
			}
		}
		return TRUE;
	}
	return FALSE;
}


LRESULT CXTCoolMenu::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_bInstalled)
	{
		return CXTWndHook::WindowProc(message, wParam, lParam);
	}

	switch(message)
	{
	case WM_TIMER:
		{
			OnMenuTimer((int)wParam);
			break;
		}
	case WM_KEYDOWN:
		{
			CXTCoolMenu::m_bLastMouseEvent = FALSE;
			break;
		}

	case WM_SYSCOLORCHANGE:
	case WM_SETTINGCHANGE:
		{
			Refresh();
			break;
		}

	case WM_MEASUREITEM:
		{
			if (OnMeasureItem((MEASUREITEMSTRUCT*)lParam)) {
				return TRUE;
			}
			break;
		}

	case WM_DRAWITEM:
		{
			DRAWITEMSTRUCT *pdis = (DRAWITEMSTRUCT*) lParam;
			if (pdis->CtlType == ODT_MENU  &&  OnDrawItem(pdis)) {
				return TRUE;
			}
			break;
		}

	case WM_ENTERMENULOOP:
		{
			m_bIsTrackPopup = (BOOL)wParam;
			break;
		}

	case WM_INITMENUPOPUP:
		{
			if (m_iIgnoreCount)
			{
				--m_iIgnoreCount;
				break;
			}
			CXTWndHook::WindowProc(message, wParam, lParam);
			CMenu* pPopupMenu = CMenu::FromHandle((HMENU)wParam);

			OnInitMenuPopup(pPopupMenu, (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam));

			return 0;
		}

	case WM_MENUSELECT:
		{
			OnMenuSelect((UINT)LOWORD(wParam), (UINT)HIWORD(wParam), (HMENU)lParam);
			break;
		}

	case WM_MENUCHAR:
		{
			CMenu* pPopupMenu = CMenu::FromHandle((HMENU)lParam);
			LRESULT lResult = OnMenuChar((TCHAR)LOWORD(wParam), (UINT)HIWORD(wParam), pPopupMenu);

			if (lResult!=0) {
				return lResult;
			}
		}
		break;

	case WM_COMMAND:
		{
			UINT uCmdID = (UINT)wParam;

			if (uCmdID == XT_IDC_CHEVRON)
			{
				m_bShowAll = true;
				m_bChevronSelected = false;

				/*
				if (m_listPopups.GetTailPosition())
				{
					CXTPopupMenu *pPopup = m_listPopups.GetTail();
					
					CMenu *pMenu = CMenu::FromHandle(pPopup->GetHMenu());
					
					if (pMenu != NULL)
					{
						RestoreMenuItems(pMenu);						
					}
				}*/
			}
			else
			{
				m_bShowAll = false;
				AddRecentItem(uCmdID);
			}
		}
		break;
	}
	
	return CXTWndHook::WindowProc(message, wParam, lParam);
}

void CXTCoolMenu::Refresh()
{
}

BOOL CXTCoolMenu::OnMeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
	ASSERT(lpMIS);
	CXTItemData* pItemData = (CXTItemData*)lpMIS->itemData;
	ASSERT(pItemData);

	UINT iItemHeight = ::GetSystemMetrics(SM_CYMENU);

	if (lpMIS->CtlType != ODT_MENU || !pItemData->IsXTItemData()) {
		return FALSE;
	}

	if (pItemData->m_dwType & MFT_SEPARATOR)
	{
		// separator: use half system height and zero width
		//lpMIS->itemHeight = xtAfxData.bXPMode? 3: iItemHeight>>1;		
		lpMIS->itemHeight = 3;
		lpMIS->itemWidth  = 0;
	}
	else
	{
		if (!pItemData->m_strText.IsEmpty())
		{
			const int cxIcon = m_sizeBmp.cx;
			const int cyIcon = m_sizeBmp.cy;

			ASSERT(cxIcon > 0  &&  cyIcon > 0);

			// Get a pointer to our hooked window and its
			// window device context.
			CWnd* pWnd = CWnd::FromHandle(m_hWnd);
			CDC* pDC   = pWnd->GetWindowDC();

			// Select the current menu font.			
			CFont* pMenuFont = &xtAfxData.font;
			if (pItemData->m_hMenu)
			{
				XT_MENUITEMINFO info;
				info.fMask = MIIM_STATE;
				if (::GetMenuItemInfo((HMENU)pItemData->m_hMenu,
						   pItemData->m_commandID, FALSE, &info))
				{
					if (info.fState & MFS_DEFAULT)
					{
						pMenuFont = &xtAfxData.fontBold;
					}
				}
			}
			CFont* pOldFont = pDC->SelectObject(pMenuFont);
			
			// get the extent of the displayed text.
			CSize size;
			CXTString cStrText = pItemData->m_strText;
			cStrText.Replace(_T("\t"), _T("XXX"));
			VERIFY(::GetTextExtentPoint32( pDC->m_hDC, cStrText,
				pItemData->m_strText.GetLength(), &size ));

			if (xtAfxData.bXPMode)
			{
				lpMIS->itemWidth = __max(100, (cxIcon + CXRMARGIN_XP + CXGAP_XP + size.cx));
			}
			else
			{
				lpMIS->itemWidth = __max(100, (cxIcon + CXGAP_NORM + size.cx + cxIcon));
			}
			lpMIS->itemHeight = cyIcon + 4;

			if (xtAfxData.bXPMode)
			{
				lpMIS->itemHeight += 3;
			}

			// cleanup.
			pDC->SelectObject(pOldFont);
			pWnd->ReleaseDC(pDC);
		}
		else
		{
			lpMIS->itemHeight = iItemHeight;
			lpMIS->itemWidth  = 100;
		}

		// if this is a chevron, adjust slightly.
		if (lpMIS->itemID == XT_IDC_CHEVRON)
		{
			if (xtAfxData.bXPMode)
			{
				lpMIS->itemHeight = m_sizeBmp.cy;  // Office  XP
			}
			else
			{
				lpMIS->itemHeight = iItemHeight+1; // Office  2000
			}
		}
		else
		{
			// Ensure that the menu item is always at least as tall as SM_CYMENU.
			if (lpMIS->itemHeight <  iItemHeight)
				lpMIS->itemHeight = iItemHeight;
		}
	}

	if (xtAfxData.bXPMode)
	{
		if (IsFirstVisibleItem(pItemData->m_hMenu, pItemData->m_nPosition))
		{
			++lpMIS->itemHeight;  // room for white line on top
		}
		if (IsLastVisibleItem(pItemData->m_hMenu, pItemData->m_nPosition,
			pItemData->m_commandID))
		{
			lpMIS->itemHeight -=3;
		}
	}

	return TRUE; // handled
}

BOOL CXTCoolMenu::OnDrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	ASSERT(lpDIS);
	ASSERT(lpDIS->hDC);
	ASSERT(lpDIS->itemAction != ODA_FOCUS);

	if (m_listPopups.GetTailPosition())
	{
		CXTPopupMenu *pPopup = m_listPopups.GetTail();
		pPopup->SetCoolMenuParent(this);
	}

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	CRect rcItem   = lpDIS->rcItem;
	int   nState   = lpDIS->itemState;
	int   nSavedDC = pDC->SaveDC();

	// does not belong to us...
	CXTItemData* pItemData = (CXTItemData*)lpDIS->itemData;
	if (lpDIS->CtlType != ODT_MENU || !pItemData || !pItemData->IsXTItemData()) {
		return FALSE;
	}

	const BOOL bDisabled   = ((nState & ODS_GRAYED)   == ODS_GRAYED);
	const BOOL bChecked    = ((nState & ODS_CHECKED)  == ODS_CHECKED);
	const BOOL bSelected   = ((nState & ODS_SELECTED) == ODS_SELECTED);
	const BOOL bDrawHidden = (pItemData->m_dwHideType & MFT_HIDDEN && m_bShowAll);

	const BOOL bIsLastVis = 
		IsLastVisibleItem(pItemData->m_hMenu, pItemData->m_nPosition,
		pItemData->m_commandID);
	const BOOL bIsFirstVis = 
		IsFirstVisibleItem(pItemData->m_hMenu, pItemData->m_nPosition);
	
	COLORREF clrMenuBack = xtAfxData.bXPMode ? xtAfxData.clrXPMenuTextBack : 
		(bDrawHidden ? xtAfxData.clr3DLight : xtAfxData.clrMenu);


	if (xtAfxData.bXPMode)
	{
		if (bIsLastVis)
		{
			rcItem.bottom += 4;
		}
	}

	CMenu* pMenu = CMenu::FromHandle(pItemData->m_hMenu);

	// re-paint the background.
	CRect rcBack = rcItem;
	if (bDrawHidden == FALSE  &&  !xtAfxData.bXPMode) {
		rcBack.top += 1;
	}
	pDC->FillSolidRect(rcBack, clrMenuBack);
	pDC->SetBkMode(TRANSPARENT);

	CString strMenuText( pItemData->m_strText );


	// fill banner on left
	CRect rcBanner;
	if (xtAfxData.bXPMode)
	{
		if (bIsFirstVis)
		{
			++rcItem.top;
		}
		if (bIsLastVis)
		{
			--rcItem.bottom;
		}

		rcBanner = rcItem;
		rcBanner.right = rcBanner.left + 16 + 2 * 4;

		COLORREF clrBanner = xtAfxData.clrXPBarFace;
		if (bDrawHidden || 
			(pItemData->m_dwType & MFT_SEPARATOR) &&
			 IsNextItemHidden(pMenu, pItemData->m_nPosition)  &&  
			 IsPrevItemHidden(pMenu, pItemData->m_nPosition))
		{
			clrBanner = xtAfxData.clrXPMenuExpanded;
		}
		pDC->FillSolidRect( &rcBanner, clrBanner );

		// draw selection rect
		if (bSelected)
		{
			CRect rcBorder = rcItem;
			rcBorder.DeflateRect(1,0);
			
			if( !bDisabled )
			{
				pDC->FillSolidRect( &rcBorder, xtAfxData.clrXPHighlight );
				pDC->Draw3dRect( &rcBorder, xtAfxData.clrXPHighlightBorder, xtAfxData.clrXPHighlightBorder);
			} else if (!m_bLastMouseEvent)
			{
				pDC->FillSolidRect( &rcBorder, xtAfxData.clrXPMenuTextBack);
				pDC->Draw3dRect( &rcBorder, xtAfxData.clrXPHighlightBorder, xtAfxData.clrXPHighlightBorder);
			}
		}
	}

	// draw the chevron
	if (lpDIS->itemID == XT_IDC_CHEVRON || strMenuText.Compare(_T(">> chevron")) == 0)
	{
//		if (m_bShowAll == false)
		{
			DrawChevron(pDC, rcItem, bSelected);
			m_bChevronSelected = (bSelected != 0);
			pDC->RestoreDC(nSavedDC);
			return TRUE;
		}
	}

	// draw separator
	else if (pItemData->m_dwType & MFT_SEPARATOR)
	{
		if (xtAfxData.bXPMode)
		{
			int x = m_sizeBtn.cx + CXGAP_XP;;
			pDC->FillSolidRect(x, 
				rcItem.top + rcItem.Height() / 2, 
				rcItem.right - x , 1, xtAfxData.clrXPSeparator);
		}
		else
		{
			if (m_bShowAll)
			{
				if (IsPrevItemHidden(pMenu, pItemData->m_nPosition) &&
					IsNextItemHidden(pMenu, pItemData->m_nPosition))
				{
					rcBack.top -= 1;
					pDC->FillSolidRect(rcBack, xtAfxData.clr3DLight);
				}
			}

			CRect rc( rcItem );
			rc.top += rc.Height()>>1;
			rc.DeflateRect(12,0);
			pDC->DrawEdge(&rc, EDGE_ETCHED, BF_TOP);
		}
	}
	else
	{

		BOOL bHasButton = FALSE;

		// Paint button, or blank if none
		CRect rcButn(rcItem.TopLeft(), m_sizeBtn);
		rcButn += CPoint(0,	(rcItem.Height() - rcButn.Height())>>1 );

		CXTImage* pImage = XTImageManager()->GetImage(pItemData->m_commandID);

		if (pImage)
		{
			bHasButton = TRUE;

			rcButn.InflateRect(/*xtAfxData.bXPMode? 1 : */0,1,1,1);

			// compute point to start drawing
			CSize size = rcButn.Size() - m_sizeBmp;
			size.cx >>= 1;
			size.cy >>= 1;
			CPoint pt(rcButn.TopLeft() + size);
			
			if (xtAfxData.bXPMode)
				pt.y += 1;

			
			
			if (xtAfxData.bXPMode)
			{

				if (bChecked)
				{
					CRect rcBox(rcButn.left + 2, rcButn.top + 2, 
						rcButn.right - 3, rcButn.bottom - 1);

					const COLORREF crBorder = 
						bDisabled?  xtAfxData.clrXPMenuBorder : 
									xtAfxData.clrXPHighlightCheckedBorder;
					pDC->Draw3dRect(rcBox, crBorder, crBorder);
					rcBox.DeflateRect(1, 1);

					if ( !bDisabled )
					{
						pDC->FillSolidRect( rcBox, bSelected ?
							xtAfxData.clrXPHighlightPushed : xtAfxData.clrXPHighlightChecked );
					}
				}
				
				if ( bDisabled ) 
				{
					if ( pImage->GetDisabledIcon() )
					{
						pDC->DrawState( pt, CSize( 0,0 ), pImage->GetDisabledIcon(), DSS_NORMAL | DST_ICON, ( HBRUSH )NULL );
					}
					else
					{
						CBrush brush( xtAfxData.clrXPDisabled );
						pDC->DrawState( pt, m_sizeBmp, pImage->GetIcon(), DST_ICON | DSS_MONO, ( HBRUSH )brush );
					}
				}
				else
				{	
					if (bChecked)pDC->DrawState(pt, m_sizeBmp,  pImage->GetIcon(), (UINT)DST_ICON, HBRUSH(0));
					else if (!bSelected) pDC->DrawState(pt, m_sizeBmp,  pImage->GetFadedIcon(), DST_ICON, HBRUSH(0));		
					else
					{
						pDC->DrawState(CPoint(pt.x + 1, pt.y + 1), m_sizeBmp,  pImage->GetShadowIcon(), DST_ICON, HBRUSH(0));
						pDC->DrawState(CPoint(pt.x - 1, pt.y - 1), m_sizeBmp,  pImage->GetIcon(), (UINT)DST_ICON, HBRUSH(0));
					}
				}
			}
			else
			{
				if (bDrawHidden == FALSE) {
					rcButn.top += 1;
				}
				
				// Paint the button area background.
				pDC->FillSolidRect(rcButn, clrMenuBack);

				// draw normal icon.
				if (!bDisabled)
				{
					if (bChecked)
					{
						pDC->DrawEdge(rcButn, BDR_SUNKENOUTER, BF_RECT);
						
						if (!bSelected)
						{
							CRect rc(rcButn);
							rc.DeflateRect(1,1);
							_xtAfxDrawShadedRect(pDC, rc);
						}
					}

					else if (bSelected)
					{
						CRect rcEdge = rcButn;
						if (pItemData->m_dwHideType & MFT_TOP)
							rcEdge.top += 1;

						pDC->DrawEdge( rcEdge, BDR_RAISEDINNER, BF_RECT );
					}

					// Draw the button.
					pDC->DrawState(pt, m_sizeBmp,  pImage->GetIcon(), (UINT)DST_ICON, HBRUSH(0));
				}

				// draw disabled icon.
				else
				{
					if ( pImage->GetDisabledIcon() )
					{
						pDC->DrawState( pt, CSize( 0,0 ), pImage->GetDisabledIcon(), DSS_NORMAL | DST_ICON, ( HBRUSH )NULL );
					}
					else
					{						
					
						pDC->DrawState( pt + CPoint(1,1), m_sizeBmp, pImage->GetIcon(), DST_ICON | DSS_MONO, ( HBRUSH )CBrush (xtAfxData.clr3DHilight));
						pDC->DrawState( pt, m_sizeBmp, pImage->GetIcon(), DST_ICON | DSS_MONO, ( HBRUSH )CBrush (xtAfxData.clr3DShadow));
					}
					
					/*if (bSelected)
					{
						CRect rcEdge = rcButn;
						if (pItemData->m_dwHideType & MFT_TOP)
							rcEdge.top += 1;

						pDC->DrawEdge( rcEdge, BDR_RAISEDINNER, BF_RECT );
					}*/
				}
			}
		}

		// If no button look for checked / unchecked bitmaps
		else
		{
			XT_MENUITEMINFO info;
			info.fMask = MIIM_CHECKMARKS;

			::GetMenuItemInfo((HMENU)lpDIS->hwndItem, lpDIS->itemID,
				FALSE, &info);
			
			if (bChecked || info.hbmpUnchecked)
			{
				rcButn.InflateRect(0,1,1,1);
				Draw3DCheckmark(pDC, rcButn, bSelected,bDisabled,
					bChecked ? info.hbmpChecked : info.hbmpUnchecked );
				bHasButton = TRUE;
			}
		}

		// Done with button, now paint text, first paint highlight if needed.
		if (bSelected)
		{
			if (xtAfxData.bXPMode)
			{
			}
			else
			{
				if (bHasButton)	{
					rcBack.left += m_sizeBtn.cx + CXGAP_NORM;
				}

				clrMenuBack = xtAfxData.clrHighlight;
				pDC->FillSolidRect(rcBack, clrMenuBack);
			}
		}

		// Compute text rectangle and colors
		CRect rcText  = rcItem;

		COLORREF colorText;
		if (xtAfxData.bXPMode)
		{
			rcText.left  += m_sizeBtn.cx + CXGAP_XP;
			if ( bDisabled )
			{
				colorText = xtAfxData.clrXPGrayText;
			}
			else
			{
				colorText = bSelected ? xtAfxData.clrXPHighlightText : xtAfxData.clrXPMenuText;
			}
		}
		else
		{
			rcText.left  += m_sizeBtn.cx + CXGAP_NORM + CXTEXTMARGIN_NORM;
			if (bDisabled)
			{
				colorText = 
					bSelected ? xtAfxData.clr3DFace : xtAfxData.clrGrayText;
			}
			else
			{
				colorText = 
					bSelected ? xtAfxData.clrHighlightText : xtAfxData.clrMenuText;
			}
		}

		XT_MENUITEMINFO info;
		info.fMask = MIIM_STATE;
		
		::GetMenuItemInfo((HMENU)lpDIS->hwndItem, lpDIS->itemID,
			FALSE, &info);

		if (CXTOSVersionInfo::Get().IsWin2KOrGreater())
		{
			// WINBUG: In Windows 2000, the message handler for WM_SETTINGSCHANGE 
			// is not called when "Hide keyboard indicators until I use the Alt key"
			// is turned on or off, so I have to call this here instead.
			::SystemParametersInfo(SPI_GETMENUUNDERLINES,
				0, &m_bHasMnemonics, 0);

			if (!m_bHasMnemonics && !CXTMenuBar::m_bAltKey) {
				_xtAfxStripMnemonics(strMenuText);
			}
		}

		// Draw the menu text.
		if (info.fState & MFS_DEFAULT)
			pDC->SelectObject(&xtAfxData.fontBold);
		else
			pDC->SelectObject(&xtAfxData.font);

		if (!xtAfxData.bXPMode && bDisabled && (!bSelected || colorText == clrMenuBack)) {
			DrawMenuText( pDC, rcText + CPoint(1,1), strMenuText,
				xtAfxData.clr3DHilight );
		}

		DrawMenuText(pDC, rcText, strMenuText, colorText);
	}

	if (bDrawHidden  &&  !xtAfxData.bXPMode)
	{
		if (pItemData->m_dwHideType & MFT_TOP)
		{
			BOOL bDrawLine = TRUE;

			if (IsItemSeparator(pMenu, pItemData->m_nPosition-1))
			{
				if (IsPrevItemHidden(pMenu, pItemData->m_nPosition-1)) {
					bDrawLine = FALSE;
				}
			}

			if (bDrawLine)
			{
				CPen penShadow(PS_SOLID, 1, xtAfxData.clr3DShadow);
				pDC->SelectObject(&penShadow);
				pDC->MoveTo(rcItem.left, rcItem.top);
				pDC->LineTo(rcItem.right, rcItem.top);
			}
		}
		
		if (pItemData->m_dwHideType & MFT_BOTTOM)
		{
			BOOL bDrawLine = TRUE;

			if (IsItemSeparator(pMenu, pItemData->m_nPosition+1))
			{
				if (IsNextItemHidden(pMenu, pItemData->m_nPosition+1)) {
					bDrawLine = FALSE;
				}
			}

			if (bDrawLine)
			{
				CPen penHilite(PS_SOLID, 1, xtAfxData.clr3DHilight);
				pDC->SelectObject(&penHilite);
				pDC->MoveTo(rcItem.left, rcItem.bottom);
				pDC->LineTo(rcItem.right, rcItem.bottom);
			}
		}
	}

	pDC->RestoreDC(nSavedDC);
	return TRUE;
}

AFX_STATIC void AFXAPI _xtAfxDrawArrow(CDC* pDC, int x, int y, BOOL bSelected)
{
	COLORREF clrMenuText;
	
	if (xtAfxData.bXPMode)
	{
		clrMenuText = bSelected ? xtAfxData.clrXPHighlightText : xtAfxData.clrXPMenuText;
	}
	else
	{
		clrMenuText = bSelected ? xtAfxData.clrMenuText : xtAfxData.clrMenuText;
	}

	pDC->SetPixel(x,   y,   clrMenuText);
	pDC->SetPixel(x,   y+1, clrMenuText);
	pDC->SetPixel(x+1, y+1, clrMenuText);
	pDC->SetPixel(x+1, y+2, clrMenuText);
	pDC->SetPixel(x+2, y+2, clrMenuText);
	pDC->SetPixel(x+2, y+3, clrMenuText);
	pDC->SetPixel(x+3, y+1, clrMenuText);
	pDC->SetPixel(x+3, y+2, clrMenuText);
	pDC->SetPixel(x+4, y,   clrMenuText);
	pDC->SetPixel(x+4, y+1, clrMenuText);
}

void CXTCoolMenu::DrawChevron(CDC* pDC, CRect rcItem, BOOL bSelected)
{
	if (!m_bIsPopup && bSelected && xtAfxData.bShowFullAfterDelay)
	{
		if (!m_bIs95orNT4)  // NT4 + Win95 don't like this
		{
			::SetTimer(m_hWnd, m_nIDEvent, m_nTimeOut, NULL);
			m_bTimerActive = true;
		}
	}
	else
	{
		::KillTimer(m_hWnd, m_nIDEvent);
		m_bTimerActive = false;
	}

	CRect rc = rcItem;

	rc.top += (rc.Height()-13);		// Office  2000

	// paint background
	if (!xtAfxData.bXPMode)
	{
		if (bSelected)
		{
			pDC->FillSolidRect(rc, xtAfxData.clr3DLight);
			pDC->Draw3dRect(rc, xtAfxData.clr3DHilight, xtAfxData.clr3DShadow);
		}
		else
		{
			pDC->FillSolidRect(rc, xtAfxData.clrMenu);
		}
	}
	
	CSize size(5,4); // arrow width and height.

	int x = (rc.Width() - size.cx)/2;
	int y = (rc.Height()-(size.cy*2))/2 + rc.top;

	_xtAfxDrawArrow(pDC, x, y, bSelected);
	_xtAfxDrawArrow(pDC, x, y+size.cy, bSelected);
}

void CXTCoolMenu::DrawMenuText(CDC* pDC, CRect rc, CString strMenuText, COLORREF color)
{
	CString strLeftText = strMenuText;
	CString strRightText;
	int iTabPos = strLeftText.Find(_T('\t'));

	if (iTabPos >= 0)
	{
		strRightText = strLeftText.Right(strLeftText.GetLength()-iTabPos-1);
		strLeftText  = strLeftText.Left(iTabPos);
	}

	pDC->SetTextColor(color);
	pDC->DrawText(strLeftText, &rc, DT_SINGLELINE|DT_LEFT|DT_VCENTER);

	if (iTabPos > 0)
	{
		if (xtAfxData.bXPMode) 
		{
			rc.right -= CXRMARGIN_XP;
		}
		else
		{
			rc.right -= m_sizeBtn.cx;
		}
		pDC->DrawText(strRightText, &rc, DT_SINGLELINE|DT_RIGHT|DT_VCENTER);
	}
}

void CXTCoolMenu::Draw3DCheckmark(CDC* pDC, const CRect& rc, BOOL bSelected, 
	BOOL bDisabled, HBITMAP hbmCheck)
{
	CRect rcDest = rc;

	COLORREF crCheckMarkFace = xtAfxData.clrMenuText;

	if (xtAfxData.bXPMode)
	{
		crCheckMarkFace = bSelected ? 
			xtAfxData.clrXPHighlightText : xtAfxData.clrXPMenuText;

		//CRect rcBox(rc.left + 2, rc.top + 2, rc.right - 3, rc.bottom - 2);
		CRect rcBox(rc.left + 2, rc.top + 2,  rc.right - 3, rc.bottom - 1);

		const COLORREF crBorder = 
			bDisabled? xtAfxData.clrXPMenuBorder : xtAfxData.clrXPHighlightCheckedBorder;
		pDC->Draw3dRect(rcBox, crBorder, crBorder); 
		rcBox.DeflateRect(1, 1);
		rcDest.OffsetRect(3, 2);

		if (!bDisabled)
		{
			pDC->FillSolidRect(rcBox, 
				bSelected?  xtAfxData.clrXPHighlightPushed : 
							xtAfxData.clrXPHighlightChecked);
		}
		else
		{
			crCheckMarkFace = xtAfxData.clrXPMenuBorder;
		}
	}
	else
	{
		if (bSelected) {
			pDC->FillSolidRect(rcDest, xtAfxData.clrMenu);
		}
		else {
			_xtAfxDrawShadedRect(pDC, rcDest);
		}

		pDC->DrawEdge(rcDest, BDR_SUNKENOUTER, BF_RECT);
	}

	int x = rcDest.left+6;
	int y = rcDest.top +8;

	// Draw the check mark
	if (hbmCheck == NULL)
	{
		pDC->SetPixel(x,   y+1, crCheckMarkFace);
		pDC->SetPixel(x,   y+2, crCheckMarkFace);
		pDC->SetPixel(x+1, y+2, crCheckMarkFace);
		pDC->SetPixel(x+1, y+3, crCheckMarkFace);
		pDC->SetPixel(x+2, y+3, crCheckMarkFace);
		pDC->SetPixel(x+2, y+4, crCheckMarkFace);
		pDC->SetPixel(x+3, y+3, crCheckMarkFace);
		pDC->SetPixel(x+3, y+2, crCheckMarkFace);
		pDC->SetPixel(x+4, y+2, crCheckMarkFace);
		pDC->SetPixel(x+4, y+1, crCheckMarkFace);
		pDC->SetPixel(x+5, y+1, crCheckMarkFace);
		pDC->SetPixel(x+5, y,   crCheckMarkFace);
		pDC->SetPixel(x+6, y,   crCheckMarkFace);
		pDC->SetPixel(x+6, y-1, crCheckMarkFace);

		if (bSelected  &&  !xtAfxData.bXPMode)
		{
			pDC->SetPixel(x-1, y+1, xtAfxData.clr3DHilight);
			pDC->SetPixel(x-1, y+2, xtAfxData.clr3DHilight);
			pDC->SetPixel(x,   y+3, xtAfxData.clr3DHilight);
			pDC->SetPixel(x+1, y+4, xtAfxData.clr3DHilight);
			pDC->SetPixel(x+2, y+5, xtAfxData.clr3DHilight);
			pDC->SetPixel(x+3, y+4, xtAfxData.clr3DHilight);
			pDC->SetPixel(x+4, y+3, xtAfxData.clr3DHilight);
			pDC->SetPixel(x+5, y+2, xtAfxData.clr3DHilight);
			pDC->SetPixel(x+6, y+1, xtAfxData.clr3DHilight);
			pDC->SetPixel(x+7, y,   xtAfxData.clr3DHilight);
			pDC->SetPixel(x+7, y-1, xtAfxData.clr3DHilight);
		}

	}
	// Draw the radio button
	else
	{
		x++;
		pDC->MoveTo(x+1, y);
		pDC->LineTo(x+4, y);
		pDC->MoveTo(x,   y+1);
		pDC->LineTo(x+5, y+1);
		pDC->MoveTo(x,   y+2);
		pDC->LineTo(x+5, y+2);
		pDC->MoveTo(x,   y+3);
		pDC->LineTo(x+5, y+3);
		pDC->MoveTo(x+1, y+4);
		pDC->LineTo(x+4, y+4);
	}
}

BOOL CXTCoolMenu::IsSysMenu(CMenu* pMenu)
{
	CWnd* pWnd = CWnd::FromHandle(m_hWnd);
	ASSERT_VALID(pWnd);

	if (((CFrameWnd*)pWnd)->IsFrameWnd())
	{
		if (pMenu == ((CFrameWnd*)pWnd)->GetSystemMenu(FALSE))
			return TRUE;

		CFrameWnd* pActiveFrame = ((CFrameWnd*)pWnd)->GetActiveFrame();
		ASSERT_VALID(pActiveFrame);

		if (pMenu == pActiveFrame->GetSystemMenu(FALSE))
			return TRUE;
	}
	
	return FALSE;
}

void CXTCoolMenu::OnInitMenuPopup(CMenu* pMenu,	UINT nIndex, BOOL bSysMenu)
{
	UNUSED_ALWAYS(nIndex);
	
	m_bChevronSelected = false;

	// initialize constants
	if (xtAfxData.bXPMode)
	{
		m_szIconMargin = CSize(CXBUTTONMARGIN_XP, CYBUTTONMARGIN_XP);
	}
	else
	{
		m_szIconMargin = CSize(CXBUTTONMARGIN_NORM, CYBUTTONMARGIN_NORM);
	}
	m_sizeBtn = m_sizeBmp;
	m_sizeBtn.cx += (m_szIconMargin.cx * 2);
	m_sizeBtn.cy += (m_szIconMargin.cy * 2);


	// bSysMenu may be FALSE even though the menu is really the system
	// menu in some cases, eg maximized MDI child window in Win 98,
	// so check whether menu is really system menu.
	if (bSysMenu == FALSE)
		bSysMenu = IsSysMenu(pMenu);

	// Find the matching CXTPopupMenu instance -
	// In some cases, the cbt hook is fired before WM_INITMENUPOPUP,
	// and in some cases it's the other way around.  The code below
	// ensures that the HMENU is set in the CXTPopupMenu instance 
	// either way.
	if (bSysMenu == FALSE)
	{
		// WINBUG: Windows doesn't send proper CBT messages when menus are
		// opened through the standard Windows menu bar.  The following 
		// cool-menu features rely on CBT hooks:
		// * XP Skinning
		// * Shadows
		// These features will not be available on the aformentioned 
		// operating systems unless you use an XT menu bar (CXTMenuBar).
		XT_ASSERT_MSG(m_bIsTrackPopup  ||  !xtAfxData.bXPMode,
			"XP style cool-menus can only be used with a "
			"CXTMenuBar (see source code for more info)");

		if (m_pPopupLast  &&  !::IsWindow(m_pPopupLast->GetHWnd()))
		{
			m_pPopupLast = NULL;
		}
		if (m_pPopupLast != NULL)
		{
			// popup already created - set the HMENU
			m_pPopupLast->SetParams(pMenu->m_hMenu);
			m_pPopupLast = NULL;
		}
		else
		{
			// popup not created yet - will pick up the HMENU from the
			// static variable m_hMenuLast when initialized
			m_hMenuLast = pMenu->m_hMenu;
		}
	}
	else
	{
		m_pPopupLast = NULL;
	}

	if (bSysMenu == FALSE)
	{
		// Convert the menu to 'cool'.
		ConvertMenu(pMenu, TRUE);
		
		// Hide the menu items if we are showing the 
		// short version of the menu.
		if (m_bIntelligent  &&  IsIntelligentModeAllowed())
        {
            HideMenuItems(pMenu);
        }
        else
        {
            // Remove the chevron if it exists.
            UpdateChevronItem(pMenu, true);
        }

		// connect to menu bar
		if (m_iMenuBarAlign)
		{
			/*int nCount = pMenu->GetMenuItemCount();

			if (m_iMenuBarAlign == CBRS_ALIGN_LEFT || m_iMenuBarAlign == CBRS_ALIGN_RIGHT)
			{			
				int nPos;
				for (nPos = 0; nPos < nCount; nPos++)
				{
					XT_MENUITEMINFO info;
					info.fMask = MIIM_DATA;
					::GetMenuItemInfo(pMenu->GetSafeHmenu(), nPos, TRUE, &info);
					CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
					if (pItemData  &&  pItemData->IsXTItemData())
					{
						pItemData->m_iMBAlign = m_iMenuBarAlign;
					}
				}
			}
			else
			{
				int nPos;
				if (m_iMenuBarAlign == CBRS_ALIGN_BOTTOM)
				{
					nPos = nCount - 1; 
				}
				else
				{
					nPos = 0;
				}

				XT_MENUITEMINFO info;
				info.fMask = MIIM_DATA;
				::GetMenuItemInfo(pMenu->GetSafeHmenu(), nPos, TRUE, &info);
				CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
				if (pItemData  &&  pItemData->IsXTItemData())
				{
					pItemData->m_iMBAlign = m_iMenuBarAlign;
				}
			}*/

			m_iMenuBarAlign = 0; // reset
		}
		
	}

}

BOOL CXTCoolMenu::HasHiddenItems(CMenu* pMenu)
{
	// if the menu is NULL return FALSE.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return FALSE;
	}

	// no need to continue if no hidden items are not defined.
	if (m_arCommandID.GetSize() == 0) {
		return FALSE;
	}

	UINT uItemCount = pMenu->GetMenuItemCount();
	UINT uItem;
	for (uItem = 0; uItem < uItemCount; uItem++)
	{
		XT_MENUITEMINFO info;
		info.fMask = MIIM_DATA | MIIM_ID;
		::GetMenuItemInfo(pMenu->GetSafeHmenu(), uItem, TRUE, &info); 
		
		// Get the menus item data pointer.
		CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
		if (!pItemData || !pItemData->IsXTItemData())
			continue;
		
		// if the item is a chevron...then leave it alone.
		if (info.wID == XT_IDC_CHEVRON)
			continue;

		if (IsRecentItem(info.wID))
			continue;

		if (pItemData->m_dwHideType & MFT_HIDDEN)
			return TRUE;
	}
	return FALSE;
}

void CXTCoolMenu::HideMenuItems(CMenu* pMenu)
{
	// if the menu is NULL return.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return;
	}

	UINT uItemCount = pMenu->GetMenuItemCount();
	
	// Next, we need to set the flags that will draw the top and bottom
	// recess lines for hidden items, if the previous item is not hidden 
	// then draw the top line, if the next item is not hidden then 
	// draw the bottom line...
	UINT uItem;
	for (uItem = 0; uItem < uItemCount; uItem++)
	{
		XT_MENUITEMINFO info;
		info.fMask = MIIM_DATA | MIIM_ID;
		::GetMenuItemInfo(pMenu->GetSafeHmenu(), uItem, TRUE, &info); 
		
		// Get the menus item data pointer.
		CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
		if (!pItemData || !pItemData->IsXTItemData())
			continue;
		
		if (info.wID == XT_IDC_CHEVRON)
			continue;
		
		if (pItemData->m_dwHideType & MFT_HIDDEN)
		{
			if (!IsPrevItemHidden(pMenu, uItem))
				pItemData->m_dwHideType |= MFT_TOP;
			
			if (!IsNextItemHidden(pMenu, uItem))
				pItemData->m_dwHideType |= MFT_BOTTOM;
		}
	}

	// add / remove the chevron.
	UpdateChevronItem(pMenu, m_bShowAll);

	// If the short menu is displayed, remove hidden items...
	if (m_bShowAll == false)
	{
		// Remove the hidden menu items.
		uItemCount = pMenu->GetMenuItemCount()-1;
		int i;
		for (i = uItemCount; i >= 0; i--)
		{
			XT_MENUITEMINFO info;
			info.fMask = MIIM_SUBMENU | MIIM_DATA | MIIM_STATE;

			// Get the menus item data pointer.
			::GetMenuItemInfo(pMenu->GetSafeHmenu(), i, TRUE, &info); 
			CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
			if (!pItemData || !pItemData->IsXTItemData())
				continue;

			if (info.wID == XT_IDC_CHEVRON)
				continue;

			if (pItemData->m_dwHideType & MFT_HIDDEN && !pItemData->m_bHidden)
			{
				pItemData->m_dwState = info.fState;
				pItemData->m_bHidden = TRUE;

				if (info.hSubMenu && ::IsMenu(info.hSubMenu)) {
					pMenu->RemoveMenu(i, MF_BYPOSITION);
				}
				else {
					pMenu->DeleteMenu(i, MF_BYPOSITION);
				}
			}
		}

		BOOL bSeparator = FALSE;

		// For the short menu, if separators are found back to back,
		// then remove duplicates...
		uItemCount = pMenu->GetMenuItemCount()-1;
		for (i = uItemCount; i >= 0; i--)
		{
			XT_MENUITEMINFO info;
			info.fMask = MIIM_DATA | MIIM_ID;

			// Get the menus item data pointer.
			::GetMenuItemInfo(pMenu->GetSafeHmenu(), i, TRUE, &info); 
			CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
			if (!pItemData || !pItemData->IsXTItemData())
				continue;

			if (info.wID == XT_IDC_CHEVRON)
				continue;

			if ((pItemData->m_dwType & MFT_SEPARATOR) && bSeparator)
			{
				pItemData->m_bHidden = TRUE;
				pMenu->DeleteMenu(i, MF_BYPOSITION);
			}

			if (pItemData->m_dwType & MFT_SEPARATOR) {
				bSeparator = TRUE;
			}
			else {
				bSeparator = FALSE;
			}
		}
	}
}

BOOL CXTCoolMenu::RestoreMenuItems(CMenu* pMenu)
{
	// if the menu is NULL return.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return FALSE;
	}

	BOOL bChanged = FALSE;

	POSITION pos = m_itemList.GetHeadPosition();
	while (pos)
	{
		// If this is the index of the menu this item belongs to, and
		// the item is currently hidden, we need to insert it back into
		// the menu...
		CXTItemData* pItemData = m_itemList.GetNext(pos);

		// owner-draw menu item isn't ours...leave it alone
		if (!pItemData || !pItemData->IsXTItemData())
			continue;

		if (pItemData->m_hMenu != pMenu->m_hMenu)
			continue;

		// Set the correct menu item data for this item.
		TCHAR szItemName[256];
		XT_MENUITEMINFO info;
		info.fMask      = MIIM_SUBMENU | MIIM_DATA | MIIM_ID | MIIM_TYPE | MIIM_STATE;
		info.dwTypeData = szItemName;
		info.cch        = _countof(szItemName);
		info.dwItemData = (UINT_PTR)pItemData;
		info.fState     = pItemData->m_dwState;
		info.fType      = pItemData->m_dwType;
		info.wID        = pItemData->m_commandID;
		info.hSubMenu   = pItemData->m_hSubMenu;

		if ( ::IsMenu( pItemData->m_hSubMenu ) )
		{
			CMenu* pSubMenu = CMenu::FromHandle( pItemData->m_hSubMenu );
			RestoreMenuItems( pSubMenu );
		}
		
		_tcscpy(szItemName, pItemData->m_strText);

		// even though we are not changing anything here for
		// items that are not hidden, we need to call SetMenuItemInfo
		// to ensure that each item is painted in the correct order.
		if (!pItemData->m_bHidden)
		{
			::SetMenuItemInfo(pMenu->GetSafeHmenu(),
				pItemData->m_nPosition, TRUE, &info);
		}
		else
		{
			if (::InsertMenuItem(pMenu->GetSafeHmenu(),
				pItemData->m_nPosition, TRUE, &info))
			{
				// Set the hidden flag to FALSE.
				pItemData->m_bHidden = FALSE;
			}
			bChanged = TRUE;
		}
	}

	// Remove the chevron from the menu.
	UpdateChevronItem( pMenu, TRUE );

	return bChanged;
}

void CXTCoolMenu::ConvertMenu(CMenu* pMenu, BOOL bHasButtons)
{
	// if the menu is NULL return.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return;
	}

	// loop over each item in menu
	CString strItemName;
	UINT uItemCount = pMenu->GetMenuItemCount();

	UINT uItem;
	for (uItem = 0; uItem < uItemCount; uItem++)
	{
		// get menu item info
		TCHAR szItemName[256];
		XT_MENUITEMINFO info;
		info.fMask      = MIIM_SUBMENU | MIIM_DATA | MIIM_ID | MIIM_TYPE | MIIM_STATE;
		info.dwTypeData = szItemName;
		info.cch        = _countof(szItemName);

		::GetMenuItemInfo(pMenu->GetSafeHmenu(), uItem, TRUE, &info);

		// owner-draw menu item isn't ours...leave it alone
		CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
		if (pItemData && !pItemData->IsXTItemData()) {
			continue; 
		}
	
		info.fMask = 0;

		if (bHasButtons)
		{
			if (!(info.fType & MFT_OWNERDRAW))
			{
				// Make owner drawn.
				info.fType |= MFT_OWNERDRAW;
				info.fMask |= MIIM_TYPE;

				if (!pItemData)
				{
					pItemData = new CXTItemData;
					ASSERT(pItemData);
					
					pItemData->m_strText    = info.dwTypeData;
					pItemData->m_hMenu      = pMenu->m_hMenu;
					pItemData->m_hSubMenu   = info.hSubMenu;
					pItemData->m_nPosition  = uItem;
					pItemData->m_commandID  = info.wID;
					pItemData->m_dwType     = info.fType;
					pItemData->m_dwState    = info.fState;

					if (IsItemHidden(info.wID) && m_bIntelligent && IsIntelligentModeAllowed())
						pItemData->m_dwHideType = MFT_HIDDEN;

					info.dwItemData	   = (UINT_PTR)pItemData;
					info.fMask		  |= MIIM_DATA;

					// This will ensure that memory is de-allocated.
					m_itemList.AddTail(pItemData);
				}
			}

			// now add the menu to list of "converted" menus
			if (!m_menuList.Find(pMenu)) {
				m_menuList.AddHead(pMenu);
			}
		}
		else
		{
			if (info.fType & MFT_OWNERDRAW)
			{
				info.fType &= ~MFT_OWNERDRAW;
				info.fMask |= MIIM_TYPE;
				ASSERT(pItemData);
				strItemName = pItemData->m_strText;
			}
			else {
				strItemName = info.dwTypeData;
			}

			if (pItemData)
			{
				info.dwItemData = NULL;
				info.fMask |= MIIM_DATA;
			
				POSITION pos = m_itemList.Find( pItemData );
				if (pos) {
					m_itemList.RemoveAt(pos);
				}
				SAFE_DELETE(pItemData);
			}

			// if setting name, copy name from CString to buffer and set cch
			if (info.fMask & MIIM_TYPE)
			{
				_tcsncpy(szItemName, strItemName, _countof(szItemName));
				info.dwTypeData = szItemName;
				info.cch = strItemName.GetLength();
			}
		}
		
		// if after all the above, there is anything to change, change it
		if (info.fMask) {
			::SetMenuItemInfo(pMenu->GetSafeHmenu(), uItem, TRUE, &info);
		}
	}
}

BOOL CXTCoolMenu::UpdateChevronItem(CMenu* pMenu, BOOL bRemove)
{
	// if the menu is NULL return.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return FALSE;
	}

	BOOL bChanged = FALSE;

	int iIndex = MenuItemExists(pMenu, XT_IDC_CHEVRON);

	// The chevron exists, however the menu has changed since it
	// was inserted and now is not at the correct position, so
	// we will need to remove it then append it to the end
	// of the menu...
	if ((iIndex != -1) && (iIndex != (int)pMenu->GetMenuItemCount()-1))
		bRemove = TRUE;

	// Remove the chevron.
	if (m_bShowAll || bRemove)
	{
		if (iIndex != -1)
		{
			XT_MENUITEMINFO info;
			info.fMask = MIIM_DATA;
			::GetMenuItemInfo(pMenu->GetSafeHmenu(), XT_IDC_CHEVRON, FALSE, &info); 
			
			// get the item data associated with the menu item,
			// remove it from the list and free the allocated memory.
			POSITION pos = m_itemList.Find((CXTItemData*)info.dwItemData);
			if (pos)
			{
				CXTItemData* pItemData = m_itemList.GetAt(pos);
				m_itemList.RemoveAt(pos);
				SAFE_DELETE(pItemData);
			}
			
			// remove the existing item.
			pMenu->DeleteMenu(XT_IDC_CHEVRON, MF_BYCOMMAND);

			bChanged = TRUE;

			if (!m_bShowAll)
				iIndex = -1;
		}
	}

	// Insert the chevron.
	if (!m_bShowAll && (iIndex == -1) && HasHiddenItems(pMenu) && IsIntelligentModeAllowed())
	{
		pMenu->AppendMenu(MF_STRING, XT_IDC_CHEVRON, _T(">> chevron"));
		
		TCHAR szItemName[256];
		XT_MENUITEMINFO info;
		info.fMask      = MIIM_SUBMENU | MIIM_DATA | MIIM_ID | MIIM_TYPE;
		info.dwTypeData = szItemName;
		info.cch        = _countof(szItemName);
		
		// get menu item info
		::GetMenuItemInfo(pMenu->GetSafeHmenu(), XT_IDC_CHEVRON, FALSE, &info); 
		
		CXTItemData* pItemData = new CXTItemData;
		ASSERT(pItemData);
			
		pItemData->m_dwType     = info.fType;
		pItemData->m_dwHideType = 0;
		pItemData->m_commandID  = info.wID;
		pItemData->m_strText    = szItemName;
		
		m_itemList.AddTail(pItemData);

		info.dwItemData	= (UINT_PTR)pItemData;
		info.fType     |= MFT_OWNERDRAW;
		info.fMask		= MIIM_DATA | MIIM_TYPE;

		::SetMenuItemInfo(pMenu->GetSafeHmenu(), XT_IDC_CHEVRON, FALSE, &info);

		bChanged = TRUE;
	}

	return bChanged;
}

LONG CXTCoolMenu::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu)
{
	UNUSED_ALWAYS(nFlags);

	ASSERT(::IsMenu(pMenu->m_hMenu));

	UINT iCurrentItem = (UINT)-1;
	CUIntArray arItemsMatched;

	UINT uItemCount = pMenu->GetMenuItemCount();
	UINT uItem;
	for (uItem=0; uItem< uItemCount; uItem++)
	{
		// get menu info
		XT_MENUITEMINFO info;
		info.fMask = MIIM_DATA | MIIM_TYPE | MIIM_STATE;
		::GetMenuItemInfo(pMenu->GetSafeHmenu(), uItem, TRUE, &info);

		CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
		if ((info.fType & MFT_OWNERDRAW) && pItemData && pItemData->IsXTItemData())
		{
			CString& text = pItemData->m_strText;
			int iAmpersand = text.Find(_T('&'));
			if (iAmpersand >=0 && toupper(nChar)==toupper(text[iAmpersand+1]))
				arItemsMatched.Add(uItem);
		}
		if (info.fState & MFS_HILITE) {
			iCurrentItem = uItem;
		}
	}

	UINT nFound = arItemsMatched.GetSize();
	if (nFound == 0) {
		return 0;
	}
	else if (nFound==1) {
		return MAKELONG(arItemsMatched[0], MNC_EXECUTE);
	}

	// more than one found--return 1st one past current selected item;
	UINT iSelect = 0;
	for (uItem=0; uItem < nFound; uItem++) {
		if (arItemsMatched[uItem] > iCurrentItem) {
			iSelect = uItem;
			break;
		}
	}

	return MAKELONG(arItemsMatched[iSelect], MNC_SELECT);
}

void CXTCoolMenu::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	UNREFERENCED_PARAMETER(nItemID);

	if ((hSysMenu == NULL) && (nFlags == 0xFFFF))
	{
		// Windows has closed the menu.
		while (!m_menuList.IsEmpty())
		{
			// and restore menu to original state.
			CMenu* pMenu = m_menuList.RemoveHead();

			RestoreMenuItems(pMenu);
			ConvertMenu(pMenu, FALSE);
		}

		m_bShowAll = false;
	}
}


void CXTCoolMenu::SetTimerInfo(UINT nElapse, UINT nIDEvent/*=1000*/)
{
	m_nIDEvent = nIDEvent;
	m_nTimeOut = nElapse;
}

BOOL CXTCoolMenu::HideCommand(UINT nCommandID)
{
	SetIntelligentMode();
	m_arCommandID.Add(nCommandID);
	return TRUE;
}

BOOL CXTCoolMenu::HideCommand(LPCTSTR lpszItem)
{
	UNUSED_ALWAYS(lpszItem);
	TRACE0("CXTCoolMenu::HideCommand(LPCTSTR lpszItem) not yet implemented.\n");
	return FALSE;
}

BOOL CXTCoolMenu::HideCommands(const UINT* nCommandIDs, int nSize)
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

BOOL CXTCoolMenu::HideCommands(const LPCTSTR lpszItems, int nSize)
{
	UNUSED_ALWAYS(nSize);
	UNUSED_ALWAYS(lpszItems);
	TRACE0("HideCommands(const LPCTSTR lpszItems, int nSize) not yet implemented.\n");
	return FALSE;
}

BOOL CXTCoolMenu::IsItemHidden(UINT nCommandID)
{
	int i;
	for (i = 0; i < m_arCommandID.GetSize(); ++i)
	{
		if (m_arCommandID.GetAt(i) == nCommandID) {
			return (IsRecentItem(nCommandID) == FALSE);
		}
	}
	return FALSE;
}

BOOL CXTCoolMenu::IsRecentItem(UINT nItem)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	CXTRegistryManager regManager;

	CString strSection;
	strSection.LoadString(XT_IDS_REG_RECENTLIST);
	int i;
	for (i = 1; i <= m_nRecentList; ++i)
	{
		CString strEntry;
		strEntry.Format(XT_IDS_REG_ITEMID, i);
		if (regManager.GetProfileInt(strSection, strEntry, 0) == nItem) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXTCoolMenu::AddRecentItem(UINT nItem)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	if (!IsItemHidden(nItem) && !IsRecentItem(nItem))
		return FALSE;

	CXTRegistryManager regManager;

	CList<UINT,UINT> arItems;

	CString strSection;
	strSection.LoadString(XT_IDS_REG_RECENTLIST);

	// build the rest of the item array...
	int i;
	for (i = 0; i < m_nRecentList; ++i)
	{
		CString strEntry;
		strEntry.Format(XT_IDS_REG_ITEMID, i+1);

		UINT uFound = regManager.GetProfileInt(
			strSection, strEntry, 0);
		arItems.AddTail(uFound);
	}

	POSITION pos = arItems.Find(nItem);
	if (pos) {
		arItems.RemoveAt(pos);
	}

	arItems.AddHead(nItem);

	int iItem = 0;
	pos = arItems.GetHeadPosition();
	while (pos)
	{
		UINT iCmd = arItems.GetNext(pos);
		++iItem;

		CString strEntry;
		strEntry.Format(XT_IDS_REG_ITEMID, iItem);

		regManager.WriteProfileInt(
			strSection, strEntry, iCmd);

		if (iItem == m_nRecentList) {
			break;
		}
	}

	return TRUE;
}


BOOL CXTCoolMenu::IsFirstVisibleItem(HMENU hMenu, UINT nPos)
{
	if (!::IsMenu(hMenu))
	{
		return FALSE;
	}
	
	XT_MENUITEMINFO info;
	info.fMask = MIIM_DATA;
	
	// Get the data pointer for this menu item.
	::GetMenuItemInfo(hMenu, 0, TRUE, &info); 
	CXTItemData* pItemDataFirst = (CXTItemData*)info.dwItemData;

	return (pItemDataFirst->m_nPosition == nPos);
}

BOOL CXTCoolMenu::IsLastVisibleItem(HMENU hMenu, UINT nPos, UINT cmd)
{
	if (cmd == XT_IDC_CHEVRON)
	{
		return TRUE;
	}

	if (!::IsMenu(hMenu))
	{
		return FALSE;
	}

	int nItemCount = ::GetMenuItemCount(hMenu);
	if (nItemCount)
	{
		int iLastCmd = ::GetMenuItemID(hMenu, nItemCount - 1);
		if (iLastCmd == XT_IDC_CHEVRON)
		{
			// well, this ain't the chevron
			return FALSE;
		}
	}

	return (nPos == (UINT)(nItemCount - 1));
}

BOOL CXTCoolMenu::IsPrevItemHidden(CMenu* pMenu, int nPos)
{
	// if the menu is NULL return FALSE.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return FALSE;
	}

	// If we are the first item in the menu, return TRUE.
	if (nPos <= 0)
		return TRUE;

	XT_MENUITEMINFO info;
	info.fMask = MIIM_DATA;
	
	// Get the data pointer for this menu item.
	::GetMenuItemInfo(pMenu->GetSafeHmenu(), nPos-1, TRUE, &info); 
	CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
	
	if (pItemData == NULL)
		return FALSE;

	return (pItemData->m_dwHideType & MFT_HIDDEN);
}

BOOL CXTCoolMenu::IsNextItemHidden(CMenu* pMenu, int nPos)
{
	// if the menu is NULL return FALSE.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return FALSE;
	}

	int nItemCount = pMenu->GetMenuItemCount()-1;

	// If we are the last item in the menu, return TRUE.
	if (nPos >= nItemCount)
		return TRUE;

	XT_MENUITEMINFO info;
	info.fMask = MIIM_DATA;

	// Get the item data pointer for the next menu item.
	::GetMenuItemInfo(pMenu->GetSafeHmenu(), nPos+1, TRUE, &info); 
	CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
	
	if (pItemData == NULL)
		return FALSE;

	return (pItemData->m_dwHideType & MFT_HIDDEN);
}

BOOL CXTCoolMenu::IsItemSeparator(CMenu* pMenu, int nPos)
{
	// if the menu is NULL return FALSE.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return FALSE;
	}

	int nItemCount = pMenu->GetMenuItemCount()-1;

	// If we are the last item in the menu, return TRUE.
	if (nPos >= nItemCount)
		return TRUE;

	XT_MENUITEMINFO info;
	info.fMask = MIIM_DATA;

	// Get the item data pointer for the next menu item.
	::GetMenuItemInfo(pMenu->GetSafeHmenu(), nPos, TRUE, &info); 
	CXTItemData* pItemData = (CXTItemData*)info.dwItemData;
	
	if (pItemData == NULL)
		return FALSE;

	return ((pItemData->m_dwType & MFT_SEPARATOR) == MFT_SEPARATOR);
}

int CXTCoolMenu::MenuItemExists(CMenu* pMenu, UINT nCommandID)
{
	// if the menu is NULL return -1.
	if (pMenu == NULL || !::IsMenu(pMenu->m_hMenu)) {
		return -1;
	}

	int nCount = pMenu->GetMenuItemCount();
	int i;
	for (i = 0; i < nCount; ++i)
	{
		if (pMenu->GetMenuItemID(i) == nCommandID) {
			return i;
		}
	}
	return -1;
}

void CXTCoolMenu::OnMenuTimer(int nIDEvent)
{
	::KillTimer(m_hWnd, m_nIDEvent);

	if (m_bChevronSelected && nIDEvent == m_nIDEvent && !m_bIsPopup && xtAfxData.bShowFullAfterDelay)
	{
		/*::SendMessage(m_hWnd, WM_COMMAND,
			(WPARAM)XT_IDC_CHEVRON, (LPARAM)0);*/
		keybd_event(VK_RETURN, 0, 0, 0);
	}
}

void CXTCoolMenu::ResetUsageData()
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	int iItem = 1;

	CString strSection;
	strSection.LoadString(XT_IDS_REG_RECENTLIST);

	CString strEntry;
	strEntry.Format(XT_IDS_REG_ITEMID, iItem);

	// remove all registry data.
	CXTRegistryManager regManager;
	while (regManager.GetProfileInt(strSection, strEntry, -1) != -1)
	{
		regManager.DeleteValue(strSection, strEntry);
		strEntry.Format(XT_IDS_REG_ITEMID, ++iItem);
	}
}

void CXTCoolMenu::IgnoreNextPopup()
{
	++m_iIgnoreCount;
}

bool CXTCoolMenu::IsIntelligentModeAllowed()
{
	return m_bAllowIntelligent && xtAfxData.bMenuRecentCommands;
}

void CXTCoolMenu::SetIconSize(CSize size)
{
	m_sizeBmp = size;
}

CSize CXTCoolMenu::GetIconSize()
{
	return m_sizeBmp;
}


/////////////////////////////////////////////////////////////////////////////
// CXTMenu

CXTMenu::CXTMenu()
{
	CXTCoolMenu::m_bAllowIntelligent = true;
}

CXTMenu::~CXTMenu()
{
	CXTCoolMenu::m_bAllowIntelligent = false;
}

BOOL CXTMenu::TrackPopupMenu(UINT nFlags, int x, int y, CWnd* pWnd, LPCRECT lpRect, BOOL bNotify)
{
	//UNUSED_ALWAYS(lpRect);
	TPMPARAMS tpm = { sizeof(TPMPARAMS), {0, 0, 0, 0}};
	if (lpRect)	tpm.rcExclude = *lpRect;

	return TrackPopupMenuEx(nFlags, x, y, pWnd, &tpm, bNotify);
}

BOOL CXTMenu::TrackPopupMenuEx(UINT nFlags, int x, int y, CWnd* pWnd, LPTPMPARAMS lptpm, BOOL bNotify)
{
	_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();
	HWND hWndOld;
	HMENU hMenuOld;

	int iMenuBarAllign = CXTCoolMenu::m_iMenuBarAlign;

	BOOL iCmd;
	nFlags |= TPM_RETURNCMD;
	
	// Save thread menu tracking state
	hWndOld = pThreadState->m_hTrackingWindow;
	pThreadState->m_hTrackingWindow = pWnd->GetSafeHwnd();
	hMenuOld = pThreadState->m_hTrackingMenu;
	pThreadState->m_hTrackingMenu = m_hMenu;

	POSITION pos = CXTCoolMenu::m_listPopups.GetHeadPosition();
	while (pos)
	{
		::PostMessage( CXTCoolMenu::m_listPopups.GetNext( pos )->GetHWnd( ),
			WM_CLOSE, 0, 0 );
	}
	
	// process all messages left in queue to ensure other popups that just
	// exited get a chance to remove themselves
	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		AfxGetApp()->PumpMessage();
	}

	// all other menus should be closed by this point
	ASSERT( CXTCoolMenu::m_listPopups.GetCount( ) == 0 ); 

	iCmd = ::TrackPopupMenuEx(GetSafeHmenu(), nFlags,
		x, y, pWnd->GetSafeHwnd(), lptpm);
	
	// Restore thread menu tracking state
	pThreadState->m_hTrackingWindow = hWndOld;
	pThreadState->m_hTrackingMenu = hMenuOld;
	
	if (iCmd == XT_IDC_CHEVRON)
	{
		CXTCoolMenu::m_iMenuBarAlign = iMenuBarAllign;

		::SendMessage(pWnd->m_hWnd, WM_COMMAND,
			(WPARAM)XT_IDC_CHEVRON, (LPARAM)0);

		return CXTMenu::TrackPopupMenuEx(nFlags, x, y, pWnd, lptpm, bNotify);
	}
	
	//	ENCOM TECHNOLOGY 20 March 2001 S.Roberts
	if (bNotify && iCmd != 0) {
		pWnd->PostMessage(WM_COMMAND, iCmd);
	}
	return iCmd;
}

CXTMenu* PASCAL CXTMenu::FromHandle(HMENU hMenu)
{
	return (CXTMenu*)CMenu::FromHandle(hMenu);
}

CXTMenu* PASCAL CXTMenu::FromHandlePermanent(HMENU hMenu)
{
	return (CXTMenu*)CMenu::FromHandlePermanent(hMenu);
}

CXTMenu* CXTMenu::GetSubMenu(int nPos) const
{
	return (CXTMenu*)CMenu::GetSubMenu(nPos);
}

