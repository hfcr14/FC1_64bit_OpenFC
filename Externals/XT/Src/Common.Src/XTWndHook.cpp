// XTWndHook.cpp : implementation of the CXTWndHook class.
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

#ifndef _XTP_INCLUDE_CONTROLS
#include "XTToolBarCtrl.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTWndHook implementation
/////////////////////////////////////////////////////////////////////////////
CXTWndHook::CXTWndHook()
{
	m_pNextHook	  = NULL;
	m_pOldWndProc = NULL;	
	m_hWnd		  = NULL;
}

CXTWndHook::~CXTWndHook()
{
	if (m_hWnd) {
		HookWindow((HWND)NULL);
	}
}

CXTWindowMap theMap;

IMPLEMENT_DYNAMIC(CXTWndHook, CWnd);

BOOL CXTWndHook::HookWindow(HWND hWnd)
{
	ASSERT_VALID(this);

	// Hook the window
	if (hWnd)
	{
		ASSERT(m_hWnd==NULL);
		ASSERT(::IsWindow(hWnd));
		theMap.Add(hWnd, this);
	}

	// Unhook the window
	else if (m_hWnd)
	{
		theMap.Remove(this);
		m_pOldWndProc = NULL;
	}

	m_hWnd = hWnd;
	return TRUE;
}

LRESULT CXTWndHook::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	ASSERT(m_pOldWndProc);
	return m_pNextHook ? m_pNextHook->WindowProc(message, wParam, lParam) :	
		::CallWindowProc(m_pOldWndProc, m_hWnd, message, wParam, lParam);
}

LRESULT CXTWndHook::Default()
{
	// MFC stores current MSG in thread state
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	// Note: must explicitly call CXTWndHook::WindowProc to avoid infinite
	// recursion on virtual function
	return CXTWndHook::WindowProc(curMsg.message, curMsg.wParam, curMsg.lParam);
}
////////////////////////////////////////////////////////////////
// CXTWindowMap implementation
////////////////////////////////////////////////////////////////
CXTWindowMap::CXTWindowMap()
{

}

CXTWindowMap::~CXTWindowMap()
{

}

LRESULT CALLBACK CXTWindowMap::HookWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Set up MFC message state just in case anyone wants it
	// This is just like AfxCallWindowProc, but we can't use that because
	// a CXTWndHook is not a CWnd.
	MSG& curMsg = AfxGetThreadState()->m_lastSentMsg;
	MSG  oldMsg = curMsg;
	curMsg.hwnd	= hWnd;
	curMsg.message = message;
	curMsg.wParam  = wParam;
	curMsg.lParam  = lParam;

	// Get hook object for this window. Get from hook map
	CXTWndHook* pXTWndHook = theMap.Lookup(hWnd);
	ASSERT(pXTWndHook);

	LRESULT lResult;

	// Window is being destroyed: unhook all hooks (for this window)
	// and pass message to original window proc
	if (message==WM_NCDESTROY)
	{
		WNDPROC wndproc = pXTWndHook->m_pOldWndProc;
		theMap.RemoveAll(hWnd);
		lResult = ::CallWindowProc(wndproc, hWnd, message, wParam, lParam);

	}
	
	// pass to message hook
	else {
		lResult = pXTWndHook->WindowProc(message, wParam, lParam);
	}

	curMsg = oldMsg;
	return lResult;
}

void CXTWindowMap::Add(HWND hWnd, CXTWndHook* pXTWndHook)
{
	ASSERT(hWnd && ::IsWindow(hWnd));

	// Add to front of list
	pXTWndHook->m_pNextHook = Lookup(hWnd);
	SetAt(hWnd, pXTWndHook);
	
	// If this is the first hook added, subclass the window
	if (pXTWndHook->m_pNextHook==NULL)
	{
		pXTWndHook->m_pOldWndProc = (WNDPROC)GetWindowLongPtr(
			hWnd, GWL_WNDPROC);
		SetWindowLongPtr(hWnd, GWL_WNDPROC, (DWORD64)HookWndProc);
	}
	
	// just copy wndproc from next hook
	else {
		pXTWndHook->m_pOldWndProc = pXTWndHook->m_pNextHook->m_pOldWndProc;
	}

	ASSERT(pXTWndHook->m_pOldWndProc);
}

void CXTWindowMap::Remove(CXTWndHook* pUnHook)
{
	HWND hWnd = pUnHook->m_hWnd;
	ASSERT(hWnd && ::IsWindow(hWnd));

	CXTWndHook* pHook = Lookup(hWnd);
	ASSERT(pHook);
	if (pHook==pUnHook)
	{
		// hook to remove is the one in the hash table: replace w/next
		if (pHook->m_pNextHook) {
			SetAt(hWnd, pHook->m_pNextHook);
		}

		// This is the last hook for this window: restore wnd proc
		else {
			RemoveKey(hWnd);
			SetWindowLongPtr(hWnd, GWL_WNDPROC, (DWORD64)pHook->m_pOldWndProc);
		}
	}
	else
	{
		// Hook to remove is in the middle: just remove from linked list
		while (pHook->m_pNextHook!=pUnHook) {
			pHook = pHook->m_pNextHook;
		}

		ASSERT(pHook && pHook->m_pNextHook==pUnHook);
		pHook->m_pNextHook = pUnHook->m_pNextHook;
	}
}

void CXTWindowMap::RemoveAll(HWND hWnd)
{
	CXTWndHook* pXTWndHook;
	while ((pXTWndHook = Lookup(hWnd))!=NULL)
		pXTWndHook->HookWindow((HWND)NULL);
}

CXTWndHook* CXTWindowMap::Lookup(HWND hWnd)
{
	CXTWndHook* pFound = NULL;
	if (!CMapPtrToPtr::Lookup(hWnd, (void*&)pFound))
		return NULL;
	ASSERT_KINDOF(CXTWndHook, pFound);
	return pFound;
}

IMPLEMENT_DYNAMIC(CXTWindowMap, CMapPtrToPtr)


/////////////////////////////////////////////////////////////////////////////
#ifndef _XTP_INCLUDE_CONTROLS


IMPLEMENT_DYNAMIC(CXTMBarWndHook, CXTWndHook);

CXTMBarWndHook::CXTMBarWndHook()
{

}

CXTMBarWndHook::~CXTMBarWndHook()
{
	HookWindow((HWND)NULL); // (unhook)
}

// Install hook to trap window messages sent to frame or MDI client.

BOOL CXTMBarWndHook::Install(CXTMenuBar* pMenuBar, HWND hWndToHook)
{
	ASSERT_VALID(pMenuBar);
	m_pMenuBar = pMenuBar;
	return HookWindow(hWndToHook);
}

// Trap frame/MDI client messages specific to menubar. 

LRESULT CXTMBarWndHook::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// verify that the menubar is still a window, it is possible that CMainFrame
    // hooks will still get called after the menubar has been destroyed, specifically
    // WM_NCACTIVATE...

	if (!::IsWindow(m_pMenuBar->m_hWnd)) {
		return CXTWndHook::WindowProc(message, wParam, lParam);
	}

	BOOL bSend = FALSE;
	if (HWND hWndServer = m_pMenuBar->OleMenuDescriptor(bSend, message, wParam, lParam)) 
	{
		// OLE wnd will handle message
		if (bSend)
			return ::SendMessage(hWndServer, message, wParam, lParam);
		else
			return ::PostMessage(hWndServer, message, wParam, lParam);
	}

	switch (message)
	{
		// The following messages are trapped for the frame window
	case WM_SYSCOLORCHANGE:
		m_pMenuBar->SetFont(&xtAfxData.fontHCapt);
		break;
		
	case WM_MENUSELECT:
		m_pMenuBar->OnMenuSelect(
			(HMENU)lParam, (UINT)LOWORD(wParam));
		break;
		
	case WM_INITMENUPOPUP:
		// set the correct submenu index for the popup.
		lParam = MAKELPARAM((WORD)m_pMenuBar->m_iButton, HIWORD(lParam));

		if (!HIWORD(lParam) && (HMENU)wParam == 
			m_pMenuBar->m_hMenuShared)
			m_pMenuBar->OnInitMenuPopup();
		break;
		
	// The following messages are trapped for the MDI client window
	// only sent to MDI client window
	// Setting new frame/window menu: bypass MDI. wparam is new menu.
	case WM_MDISETMENU: 
		m_pMenuBar->LoadMenu((HMENU)wParam, (HMENU)lParam);
		m_pMenuBar->GetOwner()->SetMenu(NULL);
		return 0;
		
	// only sent to MDI client window
	// Normally, would call DrawMenuBar, but I have the menu, so eat it.
	case WM_MDIREFRESHMENU:
		m_pMenuBar->Invalidate( ); // cause system icon to update if maximized.
		return 0;
		
	// After changing the MDI maximized state, the client window gets a
	// paint message. This is the most convenient place to find out; there
	// is no WM_MDIMAXIMIZED message.
	case WM_PAINT:
		if (m_hWnd == m_pMenuBar->m_hWndMDIClient)
			m_pMenuBar->CheckMinMaxState(true);
		break;

	// We need to track when the frames non-client area needs to be changed to 
	// indicate an active or inactive state, this is primarily used for 
	// Windows 2000 display.
	case WM_NCACTIVATE:
		{
			if (m_pMenuBar->GetOwner()->m_nFlags & WF_STAYACTIVE) {
				m_pMenuBar->m_bActive = TRUE;
			}
			else if (m_pMenuBar->GetOwner()->IsWindowEnabled() == FALSE) {
				m_pMenuBar->m_bActive = FALSE;
			}
			else if (m_pMenuBar->m_bActive != (BOOL)wParam) {
				m_pMenuBar->m_bActive = (BOOL)wParam;
			}
			m_pMenuBar->InvalidateRect(NULL);
		}
		break;
		
	case WM_COMMAND:
		{
			if (XT_IDC_CHEVRON == (UINT)wParam) {
				CXTMenuBar::m_bShowAll = true;
			}
			else {
				CXTMenuBar::m_bShowAll = false;
			}
		}
		break;
	}
	
	return CXTWndHook::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CXTMBarMDIWndHook, CXTMBarWndHook);

// Trap frame/MDI client messages specific to menubar. 
LRESULT CXTMBarMDIWndHook::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// verify that the menubar is still a window, it is possible that CMainFrame
    // hooks will still get called after the menbar has been destroyed, specifically
    // WM_NCACTIVATE...

	if (!::IsWindow(m_pMenuBar->m_hWnd)) {
		return CXTWndHook::WindowProc(message, wParam, lParam);
	}

	switch (message)
	{
	// The following messages are trapped for the MDI client window
	// only sent to MDI client window
	// Setting new frame/window menu: bypass MDI. wparam is new menu.
	case WM_MDISETMENU: 
		m_pMenuBar->LoadMenu((HMENU)wParam, (HMENU)lParam);
		m_pMenuBar->GetOwner()->SetMenu(NULL);
		return 0;
		
	// only sent to MDI client window
	// Normally, would call DrawMenuBar, but I have the menu, so eat it.
	case WM_MDIREFRESHMENU:
		m_pMenuBar->Invalidate( ); // cause system icon to update if maximized.
		return 0;

	// After changing the MDI maximized state, the client window gets a
	// paint message. This is the most convenient place to find out; there
	// is no WM_MDIMAXIMIZED message.
	case WM_PAINT:
		if (m_hWnd == m_pMenuBar->m_hWndMDIClient)
			m_pMenuBar->CheckMinMaxState(true);
		break;

	// We need to track when the frames non-client area needs to be changed to 
	// indicate an active or inactive state, this is primarily used for 
	// Windows 2000 display.
	case WM_NCACTIVATE:
		{
			if (m_pMenuBar->GetOwner()->m_nFlags & WF_STAYACTIVE) {
				m_pMenuBar->m_bActive = TRUE;
			}
			else if (m_pMenuBar->GetOwner()->IsWindowEnabled() == FALSE) {
				m_pMenuBar->m_bActive = FALSE;
			}
			else if (m_pMenuBar->m_bActive != (BOOL)wParam) {
				m_pMenuBar->m_bActive = (BOOL)wParam;
			}
			m_pMenuBar->InvalidateRect(NULL);
		}
		break;
		
	case WM_COMMAND:
		{
			if (XT_IDC_CHEVRON == (UINT)wParam) {
				CXTMenuBar::m_bShowAll = true;
			}
			else {
				CXTMenuBar::m_bShowAll = false;
			}
		}
		break;
	}
		
	return CXTWndHook::WindowProc(message, wParam, lParam);
}

#endif
