// XTMDIFrameWnd.cpp : implementation of the CXTFrameImpl class.
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
#include "XTMemDC.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTCoolMenu.h"
#include "XTSplitterDock.h"
#include "XTDockState.h"
#include "XTDockBar.h"
#include "XTDockWindow.h"
#include "XTMiniDockFrameWnd.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTResizeFormView.h"
#include "XTResizePropertyPage.h"
#include "XTResizePropertySheet.h"
#include "XTAccelManager.h"
#include "XTAccelKeyEdit.h"
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrl.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTFrameImpl.h"
#include "XTRegistryManager.h"

// customize includes, may change with Christian's additions.
#include "XTCheckListBox.h"
#include "XTCommandsListBox.h"
#include "XTTipWindow.h"
#include "XTCustomizeAPI.h"
#include "XTCallbacks.h"
#include "XTFlatComboBox.h"
#include "XTBrowseEdit.h"
#include "XTCustomizePage.h"
#include "XTCustomizeSheet.h"
#include "XTCustomizeAPI.h"

const DWORD CFrameWnd::dwDockBarMap[4][2] =
{
	{ AFX_IDW_DOCKBAR_TOP,    CBRS_TOP    },
	{ AFX_IDW_DOCKBAR_BOTTOM, CBRS_BOTTOM },
	{ AFX_IDW_DOCKBAR_LEFT,   CBRS_LEFT   },
	{ AFX_IDW_DOCKBAR_RIGHT,  CBRS_RIGHT  },
};

class _CAccessFrame : public CFrameWnd
{
	friend class CXTFrameImpl;
};

#define GET_FRAME(p) ((_CAccessFrame *)p)

DWORD              CXTFrameImpl::m_dwCustStyle     = CUST_XT_TOOLBARS|CUST_XT_COMMANDS;
CXTCustomizeSheet* CXTFrameImpl::m_pCustomizeSheet = 0;

/////////////////////////////////////////////////////////////////////////////
// CXTFrameImpl

CXTFrameImpl::CXTFrameImpl()
{
	CXTAccelManager::Get().EnableAutoSave( false );
	CXTToolsManager::Get().EnableAutoSave( false );
}

CXTFrameImpl::~CXTFrameImpl()
{
	if ( CXTAccelManager::Get().IsInitialized( ) )
	{
		CXTAccelManager::Get().Save( );
	}
	if ( CXTToolsManager::Get().IsInitialized( ) )
	{
		CXTToolsManager::Get().Save( );
	}

	SAFE_DELETE( m_pCustomizeSheet );

	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->RunGC();
}

void CXTFrameImpl::DockControlBarImpl(CControlBar* pBar, CDockBar* pDockBar, LPCRECT lpRect, CFrameWnd* pFrameWnd)
{
	ASSERT(pBar != NULL);
	// make sure CControlBar::EnableDocking has been called
	ASSERT(pBar->m_pDockContext != NULL);

#ifdef _DEBUG
	// make sure the bar's ID is OK
	CXTControlBar::AssertValidCtrlID(pBar, true);
#endif //_DEBUG

	if (pDockBar == NULL)
	{
		int i;
		for (i = 0; i < 4; i++)
		{
			if ((GET_FRAME(pFrameWnd)->dwDockBarMap[i][1] & CBRS_ALIGN_ANY) ==
				(pBar->m_dwStyle & CBRS_ALIGN_ANY))
			{
				pDockBar = (CDockBar*)GET_FRAME(pFrameWnd)->GetControlBar(GET_FRAME(pFrameWnd)->dwDockBarMap[i][0]);
				ASSERT(pDockBar != NULL);
				// assert fails when initial CBRS_ of bar does not
				// match available docking sites, as set by EnableDocking()
				break;
			}
		}
	}
	ASSERT(pDockBar != NULL);
	ASSERT(GET_FRAME(pFrameWnd)->m_listControlBars.Find(pBar) != NULL);
	ASSERT(pBar->m_pDockSite == GET_FRAME(pFrameWnd));
	// if this assertion occurred it is because the parent of pBar was not initially
	// this CFrameWnd when pBar's OnCreate was called
	// i.e. this control bar should have been created with a different parent initially
	ASSERT_KINDOF(CXTDockBar, pDockBar);
	((CXTDockBar*)pDockBar)->DockControlBar(pBar, lpRect);
}

void CXTFrameImpl::DockControlBarImpl(CControlBar* pBar, UINT nDockBarID, LPCRECT lpRect, CFrameWnd* pFrameWnd)
{
	CDockBar* pDockBar = (nDockBarID == 0) ? NULL :
		(CDockBar*)GET_FRAME(pFrameWnd)->GetControlBar(nDockBarID);
	DockControlBarImpl(pBar, pDockBar, lpRect, pFrameWnd);
}

void CXTFrameImpl::DockControlBarLeftOfImpl(CControlBar* Bar, CControlBar* LeftOf, CFrameWnd* pFrameWnd)
{
	CRect rect;
	DWORD dw;
	UINT n;
	
	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	GET_FRAME(pFrameWnd)->RecalcLayout(TRUE);
	
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP)				 ? AFX_IDW_DOCKBAR_TOP		: n;
	n = (dw&CBRS_ALIGN_BOTTOM	&& n==0) ? AFX_IDW_DOCKBAR_BOTTOM	: n;
	n = (dw&CBRS_ALIGN_LEFT		&& n==0) ? AFX_IDW_DOCKBAR_LEFT		: n;
	n = (dw&CBRS_ALIGN_RIGHT	&& n==0) ? AFX_IDW_DOCKBAR_RIGHT	: n;
	
	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a separate line. By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBarImpl(Bar, n, &rect, pFrameWnd);
}

// dock bars will be created in the order specified by dwDockBarMap
// this also controls which gets priority during layout
// this order can be changed by calling EnableDocking repetitively
// with the exact order of priority

void CXTFrameImpl::EnableDockingImpl(DWORD dwDockStyle, CFrameWnd* pFrameWnd)
{
	// must be CBRS_ALIGN_XXX or CBRS_FLOAT_MULTI only
	ASSERT((dwDockStyle & ~(CBRS_ALIGN_ANY|CBRS_FLOAT_MULTI)) == 0);

	GET_FRAME(pFrameWnd)->m_pFloatingFrameClass = RUNTIME_CLASS(CXTMiniDockFrameWnd);
	int i;
	for (i = 0; i < 4; i++)
	{
		if (GET_FRAME(pFrameWnd)->dwDockBarMap[i][1] & dwDockStyle & CBRS_ALIGN_ANY)
		{
			CXTDockBar* pDock = (CXTDockBar*)GET_FRAME(pFrameWnd)->GetControlBar(GET_FRAME(pFrameWnd)->dwDockBarMap[i][0]);
			if (pDock == NULL)
			{
				pDock = new CXTDockBar();
				if (!pDock->Create(GET_FRAME(pFrameWnd),
					WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CHILD|WS_VISIBLE |
						GET_FRAME(pFrameWnd)->dwDockBarMap[i][1], GET_FRAME(pFrameWnd)->dwDockBarMap[i][0]))
				{
					AfxThrowResourceException();
				}
			}
		}
	}
}

void CXTFrameImpl::EnableDockingExImpl(DWORD dwDockStyle, DWORD dwFlatStyle, CFrameWnd* pFrameWnd)
{
	EnableDockingImpl(dwDockStyle, pFrameWnd);

	// change the DockBar XTStyle
	int i;
	for (i = 0; i < 4; i++)
	{
		CXTDockBar* pDock = (CXTDockBar*)GET_FRAME(pFrameWnd)->GetControlBar(GET_FRAME(pFrameWnd)->dwDockBarMap[i][0]);
		if (pDock != NULL)
		{
			pDock->SetXTBarStyle(dwFlatStyle);
		}
	}
}

void CXTFrameImpl::LoadBarStateImpl(LPCTSTR lpszProfileName, CFrameWnd* pFrameWnd)
{
#ifdef _DEBUG
	// Check to see if the control bar's ID is valid, this will assert if
	// an incorrect ID was used during creation.
	_xtAfxAssertValidBarIDs(pFrameWnd);
#endif

	// restore dynamic custom bars
	CXTCustomizeConnection::GetInstance()->LoadDynamicBars(pFrameWnd, lpszProfileName);	


	// Check to see if the saved bar state has changed. This could happen
	// if a toolbar has been removed from the application but left in the
	// registry. We will return here if this is the case, SaveBarState will
	// clean up the registry for us when we exit.
	CXTDockState state;
	state.LoadState(lpszProfileName);
	int i;
	int iBarCount = state.m_arrBarInfo.GetSize();
	for (i = iBarCount-1; i >= 0; --i)
	{
		CXTControlBarInfo* pBarInfo = (CXTControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pBarInfo != NULL);
		int iDockedCount = pBarInfo->m_arrBarID.GetSize();
		if (iDockedCount > 0)
		{
			// dockbar
			int j;
			for (j = iDockedCount-1; j >= 0; --j)
			{
				UINT nID = (UINT) pBarInfo->m_arrBarID[j];
				
				// separator
				if (nID == 0)
				{
					continue;
				}
				
				// placeholder - get the ID
				if (nID > 0xFFFF)
				{
					nID &= 0xFFFF;
				}

				// invalid dock bar found, remove it from the bar ID array.
				if ( GET_FRAME( pFrameWnd )->GetControlBar( nID ) == NULL )
				{
					pBarInfo->m_arrBarID.RemoveAt( j );
				}
			}
		}
		
		// floating dockbars can be created later
		if ( !pBarInfo->m_bFloating ) 
		{
			// invalid control bar found, remove it from the bar ID array.
			if ( GET_FRAME( pFrameWnd )->GetControlBar( pBarInfo->m_nBarID ) == NULL )
			{
				state.m_arrBarInfo.RemoveAt( i );
				SAFE_DELETE( pBarInfo );
			}
		}
	}

	// load CXTDockWindows extra state: 
	CString strSection;

	POSITION pos = GET_FRAME(pFrameWnd)->m_listControlBars.GetHeadPosition();
	int nIndex;

	CXTRegistryManager regManager;
	
	while (pos != NULL)
	{
		CXTControlBar *pBar = (CXTControlBar*)GET_FRAME(pFrameWnd)->m_listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);
		if (CXTDockWindow::IsDockWindow(pBar))
		{
			nIndex = 0;
			DWORD dwID, dwBarID = _AfxGetDlgCtrlID(pBar->m_hWnd);
			do 
			{
				strSection.Format(_T("%s-Bar%d"), lpszProfileName, nIndex);
				dwID = regManager.GetProfileInt(strSection, _T("BarID"), -1);
				if (dwID == -1)
					// error
					return;
				nIndex++;
			} while (dwID != dwBarID);
			((CXTDockWindow*)pBar)->LoadBarSize(strSection);
		}
	}

	// load normal bar state
	SetDockStateImpl(state, pFrameWnd);
}

void CXTFrameImpl::SaveBarStateImpl(LPCTSTR lpszProfileName, CFrameWnd* pFrameWnd) const
{
#ifdef _DEBUG
	// Check to see if the control bar's ID is valid, this will assert if
	// an incorrect ID was used during creation.
	_xtAfxAssertValidBarIDs(pFrameWnd);
#endif

	// save dynamic custom bars
	CXTCustomizeConnection::GetInstance()->SaveDynamicBars(pFrameWnd, lpszProfileName);

	CXTDockState state;
	GetDockStateImpl(state, pFrameWnd);
	state.SaveState(lpszProfileName);

	// save CXTDockWindows extra state: 
	CXTRegistryManager regManager;
	CString strSection;

	POSITION pos = GET_FRAME(pFrameWnd)->m_listControlBars.GetHeadPosition();
	int nIndex;

	while (pos != NULL)
	{
		CControlBar *pBar = (CXTDockWindow *)GET_FRAME(pFrameWnd)->m_listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);
		if (CXTDockWindow::IsDockWindow(pBar))
		{
			nIndex = 0;
			DWORD dwID, dwBarID = _AfxGetDlgCtrlID(pBar->m_hWnd);
			do 
			{
				strSection.Format(_T("%s-Bar%d"), lpszProfileName, nIndex);
				dwID = regManager.GetProfileInt(strSection, _T("BarID"), -1);
				if (dwID == -1)
					// error
					return;
				nIndex++;
			} while (dwID != dwBarID);
			((CXTDockWindow*)pBar)->SaveBarSize(strSection);
		}
	}
}

void CXTFrameImpl::SetDockStateImpl(const CXTDockState& state, CFrameWnd* pFrameWnd)
{
	// first pass through barinfo's sets the m_pBar member correctly
	// creating floating frames if necessary
	int i;
	for (i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CXTControlBarInfo* pInfo = (CXTControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		if (pInfo->m_bFloating)
		{
			// need to create floating frame to match
			CMiniDockFrameWnd* pDockFrame = GET_FRAME(pFrameWnd)->CreateFloatingFrame(
				pInfo->m_bHorz ? CBRS_ALIGN_TOP : CBRS_ALIGN_LEFT);
			ASSERT(pDockFrame != NULL);
			CRect rect(pInfo->m_pointPos, CSize(10, 10));
			pDockFrame->CalcWindowRect(&rect);
			pDockFrame->SetWindowPos(NULL, rect.left, rect.top, 0, 0,
				SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
			CDockBar* pDockBar =
				(CDockBar*)pDockFrame->GetDlgItem(AFX_IDW_DOCKBAR_FLOAT);
			ASSERT(pDockBar != NULL);
			ASSERT_KINDOF(CDockBar, pDockBar);
			pInfo->m_pBar = pDockBar;
		}
		else // regular dock bar or toolbar
		{
			pInfo->m_pBar = GET_FRAME(pFrameWnd)->GetControlBar(pInfo->m_nBarID);
			ASSERT(pInfo->m_pBar != NULL); //toolbar id's probably changed
		}
		pInfo->m_pBar->m_nMRUWidth = pInfo->m_nMRUWidth;
	}

	// the second pass will actually dock all of the control bars and
	//  set everything correctly
	for (i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CXTControlBarInfo* pInfo = (CXTControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		if (pInfo->m_pBar != NULL)
			pInfo->m_pBar->SetBarInfo(pInfo, GET_FRAME(pFrameWnd));
	}

	// last pass shows all the floating windows that were previously shown
	for (i = 0; i < state.m_arrBarInfo.GetSize(); i++)
	{
		CXTControlBarInfo* pInfo = (CXTControlBarInfo*)state.m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		ASSERT(pInfo->m_pBar != NULL);
		if (pInfo->m_bFloating)
		{
			CFrameWnd* pFrameWnd = pInfo->m_pBar->GetParentFrame();
			CDockBar* pDockBar = (CDockBar*)pInfo->m_pBar;
			ASSERT_KINDOF(CDockBar, pDockBar);
			if (pDockBar->GetDockedVisibleCount() > 0)
			{
				pFrameWnd->RecalcLayout();
				pFrameWnd->ShowWindow(SW_SHOWNA);
			}
		}
	}
	GET_FRAME(pFrameWnd)->DelayRecalcLayout();
}

void CXTFrameImpl::GetDockStateImpl(CXTDockState& state, CFrameWnd* pFrameWnd) const
{
	state.Clear(); //make sure dockstate is empty

	// get state info for each bar
	POSITION pos = GET_FRAME(pFrameWnd)->m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CControlBar* pBar = (CControlBar*)GET_FRAME(pFrameWnd)->m_listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);
		CXTControlBarInfo* pInfo = new CXTControlBarInfo;
		pBar->GetBarInfo(pInfo);
		state.m_arrBarInfo.Add(pInfo);
	}
}

void CXTFrameImpl::DrawMenuBarImpl(CFrameWnd* pFrameWnd)
{
	ASSERT(::IsWindow(pFrameWnd->m_hWnd));

	// if we have a menubar, we need to update the
	// menu layout, this can be done by detaching
	// the existing menu then re-loading it...

	CXTMenuBar* pMenuBar = GetMenuBarImpl( pFrameWnd );
	if ( pMenuBar && pMenuBar->GetSafeHwnd())
	{
		HMENU hMenu = GetMenuImpl(pFrameWnd)->Detach();
		ASSERT(hMenu && ::IsMenu(hMenu));
		pMenuBar->LoadMenu(hMenu, NULL); 
	}
}

BOOL CXTFrameImpl::PreCreateWindowImpl(CREATESTRUCT& cs, UINT uIcon, CFrameWnd* pFrameWnd)
{
	UNREFERENCED_PARAMETER(pFrameWnd);
	
	// Register wnd class with no redraw, cursor (use default), background
	// brush and load your apps icon resource.
	cs.lpszClass = AfxRegisterWndClass( 0, NULL, NULL,
		AfxGetApp()->LoadIcon(uIcon));
	ASSERT(cs.lpszClass);

	return TRUE;
}

BOOL CXTFrameImpl::PreTranslateMessageImpl(MSG* pMsg, CFrameWnd* pFrameWnd)
{
	// check to see if this is an accelerator, if so let the framework handle it.
	if ( CXTAccelManager::Get().IsInitialized( ) && 
		 CXTAccelManager::Get().TranslateAccelerator( pMsg ) )
	{
		return FALSE;
	}

	// give the menubar a chance to translate the message.
	CXTMenuBar* pMenuBar = GetMenuBarImpl( pFrameWnd );
	if ( pMenuBar && pMenuBar->GetSafeHwnd( ) )
	{
		if ( pMenuBar->TranslateFrameMessage( pMsg ) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CXTFrameImpl::OnSysColorChangeImpl(CFrameWnd* pFrameWnd)
{
	UNREFERENCED_PARAMETER(pFrameWnd);

	// Update system color and settings.
	xtAfxData.UpdateSysColors();
//	xtAfxData.UpdateSysMetrics();
//	xtAfxData.LoadSysFonts();
}

void CXTFrameImpl::OnSettingChangeImpl(UINT uFlags, LPCTSTR lpszSection, CFrameWnd* pFrameWnd)
{
	UNREFERENCED_PARAMETER(uFlags);
	UNREFERENCED_PARAMETER(lpszSection);
	UNREFERENCED_PARAMETER(pFrameWnd);

	// Update system color and settings.
	xtAfxData.UpdateSysColors();
//	xtAfxData.UpdateSysMetrics();	
//	xtAfxData.LoadSysFonts();
}

void CXTFrameImpl::OnCustomizeBarImpl(CFrameWnd* pFrameWnd)
{
#ifdef _DEBUG
	// Check to see if the control bar's ID is valid, this will assert if
	// an incorrect ID was used during creation.
	_xtAfxAssertValidBarIDs(pFrameWnd);
#endif

	// open the customization dialog.
	m_pCustomizeSheet = new CXTCustomizeSheet( pFrameWnd, m_dwCustStyle );

	m_pCustomizeSheet->DoModal( );

	SAFE_DELETE( m_pCustomizeSheet );
}

bool CXTFrameImpl::UpdateToolsItem(LPCTSTR lpszText, int iWhich, bool bAppend)
{
	if ( m_pCustomizeSheet && m_pCustomizeSheet->m_pPage4 )
	{
        if ( CXTToolsManager::Get().IsInitialized( ) )
        {
			if ( m_pCustomizeSheet && m_pCustomizeSheet->m_pPage4 )
			{
				m_pCustomizeSheet->m_pPage4->UpdateToolsItem( lpszText, iWhich, bAppend );
			}
		    return true;
        }
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////
// Menu Related Code

void CXTFrameImpl::InstallCoolMenusImpl(const UINT* nIDToolBars, int nSize, CFrameWnd* pFrameWnd)
{
	ASSERT_VALID(GET_FRAME(pFrameWnd));
	xtAfxData.bCoolMenuCompatMode = TRUE;
	m_coolMenu.Install(GET_FRAME(pFrameWnd));
	m_coolMenu.LoadToolbars(nIDToolBars, nSize);
}

void CXTFrameImpl::InstallCoolMenusImpl(const UINT nIDToolBar, CFrameWnd* pFrameWnd)
{
	InstallCoolMenusImpl(&nIDToolBar, 1, pFrameWnd);
}

CXTCoolMenu* CXTFrameImpl::GetCoolMenuImpl(CFrameWnd* pFrameWnd) 
{
	pFrameWnd;
	ASSERT(::IsWindow(GET_FRAME(pFrameWnd)->m_hWnd)); 
	ASSERT(xtAfxData.bCoolMenuCompatMode);
	return &m_coolMenu;
}


CXTMenuBar* CXTFrameImpl::GetMenuBarImpl(CFrameWnd* pFrameWnd) const
{
	ASSERT( ::IsWindow( GET_FRAME( pFrameWnd )->m_hWnd ) ); 

	CPtrList& lstCBars = GET_FRAME( pFrameWnd )->m_listControlBars;

	POSITION pos;
	for (pos = lstCBars.GetHeadPosition(); pos; lstCBars.GetNext( pos ) )
	{
		CControlBar* pBar = ( CControlBar* )lstCBars.GetAt( pos );
		if ( pBar && pBar->GetDlgCtrlID( ) == AFX_IDW_MENUBAR )
		{
			if ( pBar->IsKindOf( RUNTIME_CLASS( CXTMenuBar ) ) )
			{
				return ( CXTMenuBar* )pBar;
			}
		}
	}

	return NULL;
}

CMenu* CXTFrameImpl::GetMenuImpl(CFrameWnd* pFrameWnd) const
{
	ASSERT( ::IsWindow( GET_FRAME( pFrameWnd )->m_hWnd ) ); 

	CXTMenuBar* pMenuBar = GetMenuBarImpl( pFrameWnd );
	if ( !pMenuBar && pFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) )
	{
		CFrameWnd* pParentWnd = pFrameWnd->GetParentFrame( );
		pMenuBar = GetMenuBarImpl( pParentWnd );
		
		if ( pMenuBar == NULL )
		{
			return GET_FRAME( pFrameWnd )->GetMenu( );
		}
		
		pFrameWnd = pParentWnd;
	}

	CMenu* pMenu = GET_FRAME(pFrameWnd)->CFrameWnd::GetMenu();
	if ( pMenu == NULL )
	{ 
		if ( pMenuBar && pMenuBar->GetSafeHwnd( ) )
		{
			pMenu = pMenuBar->GetMenu( );
		}
	}
	
	return pMenu;
}

BOOL CXTFrameImpl::HideMenuItemImpl(UINT nCommandID, CFrameWnd* pFrameWnd)
{
	return HideMenuItemsImpl(&nCommandID, 1, pFrameWnd);
}
BOOL CXTFrameImpl::HideMenuItemImpl(LPCTSTR lpszItem, CFrameWnd* pFrameWnd)
{
	ASSERT(FALSE);
	return GetCoolMenuImpl(pFrameWnd)->HideCommand(lpszItem);
}
BOOL CXTFrameImpl::HideMenuItemsImpl(const UINT* nCommandIDs, int nSize, CFrameWnd* pFrameWnd)
{
	BOOL bRetVal;
	bRetVal = GetMenuBarImpl(pFrameWnd)->HideCommands(nCommandIDs, nSize);
	if (xtAfxData.bCoolMenuCompatMode)
	{
		GetCoolMenuImpl(pFrameWnd)->HideCommands(nCommandIDs, nSize);
	}
	return bRetVal;
}

BOOL CXTFrameImpl::HideMenuItemsImpl(const LPCTSTR lpszItems, int nSize, CFrameWnd* pFrameWnd)
{
	ASSERT(FALSE);
	return GetCoolMenuImpl(pFrameWnd)->HideCommands(lpszItems, nSize);
}

void CXTFrameImpl::OnInitMenuPopupImpl(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
    UNREFERENCED_PARAMETER(nIndex);
    UNREFERENCED_PARAMETER(bSysMenu);

	// if the accelerator manager has not been initialized we do
	// not want to continue further.
	if (!CXTAccelManager::Get().IsInitialized())
		return;

	UINT nCount = pPopupMenu->GetMenuItemCount();
	UINT uItem;
	for (uItem = 0; uItem < nCount; uItem++)
	{
		XT_MENUITEMINFO info;
		info.fMask = MIIM_TYPE | MIIM_ID;
        
        if (!::GetMenuItemInfo(pPopupMenu->m_hMenu, uItem, TRUE, &info)) {
			continue;
        }

		if (info.fType == MFT_STRING)
		{
			CString strMenuText;
			if (pPopupMenu->GetMenuString(info.wID, strMenuText, MF_BYCOMMAND))
			{
				if (strMenuText.GetLength())
				{
					bool bTextChanged = false;
					int iFound = strMenuText.Find(_T('\t'));
					if (iFound != -1)
					{
						bTextChanged = true;
						strMenuText = strMenuText.Left(iFound);
					}
					
					CString strKeyAssignments;
					CXTAccelManager::Get().GetKeyAssignmentStrings(info.wID, strKeyAssignments);
					if (strKeyAssignments.GetLength())
					{
						iFound = strKeyAssignments.Find(_T(','));
						
                        if (iFound != -1) {
							strKeyAssignments = strKeyAssignments.Left(iFound);
                        }
						
                        strMenuText += _T("\t") + strKeyAssignments;
						bTextChanged = true;
					}
					
                    if (bTextChanged)
                    {
						pPopupMenu->ModifyMenu( uItem, MF_BYPOSITION|MF_STRING,
                            info.wID, strMenuText );
                    }
				}
			}
		}
	}	
}

bool CXTFrameImpl::InitAccelManagerImpl(CFrameWnd* pFrameWnd)
{
	// Include the keyboard style.
	m_dwCustStyle |= CUST_XT_KEYBOARD;

	// Initialize saved accelerators.
	return CXTAccelManager::Get().Init( pFrameWnd );
}

bool CXTFrameImpl::InitToolsManagerImpl(CFrameWnd* pFrameWnd, int iNormalIndex, int iWindowIndex, int iArgPopupMenu, int iDirPopupMenu)
{
    // This ASSERT will fire if you do not have a menubar defined for your application.  The
    // tools manager is only available for menubar enabled applications.
    
    CXTMenuBar* pMenuBar = GetMenuBarImpl( pFrameWnd );

	// Include the tools style.
	m_dwCustStyle |= CUST_XT_TOOLS;

	CXTToolsManager::Get().SetArgPopupMenu( iArgPopupMenu );
	CXTToolsManager::Get().SetDirPopupMenu( iDirPopupMenu );
    CXTToolsManager::Get().SetMenuIndex(iNormalIndex, iWindowIndex);

    return CXTToolsManager::Get().Init( pFrameWnd, pMenuBar, &m_coolMenu );
}

bool CXTFrameImpl::InitOptionsManagerImpl(CFrameWnd* pFrameWnd)
{
	// Include the options style.
	m_dwCustStyle |= CUST_XT_OPTIONS;

	// Initialize saved accelerators.
	return CXTOptionsManager::Get().Init( pFrameWnd, &m_coolMenu );
}

BOOL CXTFrameImpl::OnToolsManagerImpl(UINT nID)
{
	// get a reference to the tools command map.
	CXTToolsMap& mapTools = CXTToolsManager::Get().GetToolsMap();

	// find the command in the map.
	XT_TOOL* pTool = 0;
	mapTools.Lookup( nID, pTool );

	// make a copy so we do not change anything.
	XT_TOOL tool;
	memcpy( &tool, pTool, sizeof( XT_TOOL ) );

	// launch the process.
	VERIFY( ::ShellExecute( NULL, _T( "open" ),
		tool.szCmd, tool.szArg, tool.szDir, SW_SHOW ) > ( HINSTANCE )SE_ERR_DLLNOTFOUND );

	return TRUE;
}

void CXTFrameImpl::OnUpdateToolsManagerImpl(CCmdUI* pCmdUI)
{
	pCmdUI->ContinueRouting();
}

void CXTFrameImpl::OnUpdateChevronImpl(CCmdUI* pCmdUI)
{
	pCmdUI->Enable( true );
}
