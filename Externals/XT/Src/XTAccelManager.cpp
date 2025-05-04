// XTAccelManager.cpp : implementation of the CXTAccelManager class.
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
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTFlatheaderCtrl.h"
#include "XTListCtrl.h"
#include "XTMemDC.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTResizeFormView.h"
#include "XTResizePropertyPage.h"
#include "XTResizePropertySheet.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTAccelKeyEdit.h"
#include "XTAccelManager.h"
#include "XTRegistryManager.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTMenuBar.h"
#include "XTCoolMenu.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTFrameImpl.h"
#include "XTFrameWnd.h"
#include "XTMDIChildWnd.h"
#include "XTMDIFrameWnd.h"
#include "XTOleIPFrameWnd.h"
#include "XTFunctions.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR szENTRY[]   = _T( "Accelerators" );
const TCHAR szSECTION[] = _T( "Settings" );

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////

CXTAccelManager::CXTAccelManager()
{
    m_pFrameWnd          = NULL;
    m_bDefaultInUse      = false;
    m_bChanged           = false;
    m_pAccelBuffer       = NULL;
    m_pAccelDefault      = NULL;
    m_nAccelBufferCount  = 0;
    m_nAccelDefaultCount = 0;
    m_bAutoSave          = true;
    m_bInitialized       = false;
}

CXTAccelManager::~CXTAccelManager()
{
	if ( m_bAutoSave )
	{
		Save( );
	}
	
	SAFE_DELETE_AR( m_pAccelDefault );
	SAFE_DELETE_AR( m_pAccelBuffer );

    while (!m_ExtraItems.IsEmpty())
    {
        XT_EXTRA_ACCELITEM* pItem = m_ExtraItems.RemoveHead();
        SAFE_DELETE(pItem);
    }
}

bool CXTAccelManager::AddMenuItemsToNameList(HMENU hMenu, CString strParentName, LPCTSTR lpszCategory)
{
    if ( !hMenu || !::IsMenu( hMenu )  )
    {
		return true;
    }

    int iItemCount = ::GetMenuItemCount( hMenu );

	int iItem;
	for ( iItem = 0; iItem < iItemCount; iItem++ )
	{
        TCHAR szItemName[256];
        
        XT_MENUITEMINFO info;
        info.fMask      = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE | MIIM_STATE;
        info.dwTypeData = szItemName;
        info.cch        = _countof( szItemName );
        
        ::GetMenuItemInfo( hMenu, iItem, TRUE, &info );

        CXTString strItemName = szItemName;
        MakeDisplayName( strItemName );

        if ( info.hSubMenu && ::IsMenu( info.hSubMenu ) )
		{
			if ( strParentName.IsEmpty( ) )
			{
				AddMenuItemsToNameList(
					info.hSubMenu, strItemName, lpszCategory);
			}
			else
			{
				AddMenuItemsToNameList(
					info.hSubMenu, strParentName + _T("\\") + strItemName, lpszCategory);
			}
		}
		else
        {
            UINT nTmp;
            if ( m_MapCommandExcludeList.Lookup( info.wID, nTmp ) )
            {
                continue;
            }
            
            CXTAccelSwapOutItemList* pList = 0;
            if ( m_SwapOutList.Lookup( info.wID, pList ) && pList )
            {
                POSITION rPos = pList->GetStartPosition( );
                while( rPos )
                {
                    CString strLocName;
                    pList->GetNextAssoc( rPos, info.wID, strLocName );
                    
                    if ( strParentName.GetLength( ) )
                    {
                        strLocName = strParentName + _T("\\") + strLocName;
                    }
                    
                    SetCommandStringItem( info.wID, strLocName );
                    m_mapAccelCategory.SetAt( info.wID, lpszCategory );
                    TRACE( _T( "%s = %u\n" ), strLocName, info.wID );
                }
                continue;
            }
            
            if ( strParentName.GetLength( ) )
            {
                strItemName = strParentName + _T("\\") + strItemName;
            }
            
            SetCommandStringItem( info.wID, strItemName );
            m_mapAccelCategory.SetAt( info.wID, lpszCategory );
        }
    }

	return true;
}

bool CXTAccelManager::IsCommandInCategory(UINT nCommandID, LPCTSTR szCategory)
{
	CString szCat;
	if(m_mapAccelCategory.Lookup(nCommandID, szCat) && szCat == szCategory)
		return true;
	return false;
}

bool CXTAccelManager::LookupCommandCategory(UINT nCommandID, CString& szCategory)
{
	CString szCat;
	if(m_mapAccelCategory.Lookup(nCommandID, szCategory))
		return true;
	return false;
}

CMenu* CXTAccelManager::GetFrameMenu()
{
	CMenu* pMenu = m_pFrameWnd->GetMenu();
	if ( pMenu == NULL )
	{
		if ( m_pFrameWnd->IsKindOf( RUNTIME_CLASS( CXTOleIPFrameWnd ) ) )
		{
			pMenu = ( ( CXTOleIPFrameWnd* )m_pFrameWnd )->GetMenu();
		}
		else if ( m_pFrameWnd->IsKindOf( RUNTIME_CLASS( CXTMDIFrameWnd ) ) )
		{
			pMenu = ( ( CXTMDIFrameWnd* )m_pFrameWnd )->GetMenu();
		}
		else if ( m_pFrameWnd->IsKindOf( RUNTIME_CLASS( CXTFrameWnd ) ) )
		{
			pMenu = ( ( CXTFrameWnd* )m_pFrameWnd )->GetMenu();
		}
	}
	return pMenu;
}

bool CXTAccelManager::MenuHasCommands(HMENU hMenu)
{
	if ( hMenu && ::IsMenu( hMenu ) )	
	{
        int iItemCount = ::GetMenuItemCount( hMenu );

		int iItem; 
		for ( iItem = 0; iItem < iItemCount; iItem++ )
		{
            TCHAR szItemName[256];
            
            XT_MENUITEMINFO info;
            info.fMask      = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE;
            info.dwTypeData = szItemName;
            info.cch        = _countof( szItemName );
            
            ::GetMenuItemInfo( hMenu, iItem, TRUE, &info );
            
            if ( ( info.fType & MFT_SEPARATOR ) == 0 && info.wID != 0 )
            {
                if ( !info.hSubMenu && !::IsMenu( info.hSubMenu ) )
                {
                    return true;
                }
            }
        }
    }
    
    return false;
}

void CXTAccelManager::BuildCategoryList(HMENU hMenu, CString strParentItem)
{
	if ( hMenu && ::IsMenu( hMenu ) )	
	{
        int iItemCount = ::GetMenuItemCount( hMenu );

		int iItem; 
		for ( iItem = 0; iItem < iItemCount; iItem++ )
		{
            TCHAR szItemName[256];
            
            XT_MENUITEMINFO info;
            info.fMask      = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE | MIIM_STATE;
            info.dwTypeData = szItemName;
            info.cch        = _countof( szItemName );
            
            ::GetMenuItemInfo( hMenu, iItem, TRUE, &info );

            if ( info.hSubMenu && ::IsMenu( info.hSubMenu ) )
            {
                CXTString strDisplayName = szItemName;
                MakeDisplayName( strDisplayName );

                CString strItemName;
                if ( !strParentItem.IsEmpty( ) )
                {
                    strItemName += strParentItem;
                    strItemName += _T( " | " );
                }

                strItemName += strDisplayName;

                if ( MenuHasCommands( info.hSubMenu ) )
                {
                    XT_CATEGORY category;
                    category.hSubMenu    = info.hSubMenu;
                    category.strCategory = strItemName;
                    
                    // Top level category
                    m_comboCategoryList.AddTail( category );
                }

                BuildCategoryList( info.hSubMenu, strItemName );
            }
        }
    }
}

void CXTAccelManager::InitAccelerators(HMENU hMenu/*=NULL*/)
{
	// reset list and maps.
	m_mapAccelString.RemoveAll( );
	m_mapAccelCategory.RemoveAll( );
	m_comboCategoryList.RemoveAll( );

	// initialize accelerator data.
    if ( hMenu == NULL )
    {
        hMenu = GetFrameMenu( )->m_hMenu;
    }

    BuildCategoryList( hMenu, _T( "" ) );

	if ( hMenu && ::IsMenu( hMenu ) )	
	{
        int iItemCount = ::GetMenuItemCount( hMenu );

		int iItem; 
		for ( iItem = 0; iItem < iItemCount; iItem++ )
		{
            TCHAR szItemName[256];
            
            XT_MENUITEMINFO info;
            info.fMask      = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE | MIIM_STATE;
            info.dwTypeData = szItemName;
            info.cch        = _countof( szItemName );
            
            ::GetMenuItemInfo( hMenu, iItem, TRUE, &info );

            if ( info.hSubMenu && ::IsMenu( info.hSubMenu ) )
			{
                CXTString strItemName = szItemName;
                MakeDisplayName( strItemName );
					
				// Add the menu to out list
				AddMenuItemsToNameList( hMenu, _T( "" ), strItemName );

                POSITION pos;
                for ( pos = m_ExtraItems.GetHeadPosition( ); pos; m_ExtraItems.GetNext( pos ) )
                {
                    XT_EXTRA_ACCELITEM* pItem = m_ExtraItems.GetAt( pos );
                    
                    if ( pItem->szCategory.CompareNoCase( strItemName ) == 0 )
                    {
                        CString strItemName = pItem->szName;
                        
                        if ( strItemName.GetLength( ) )
                        {
                            strItemName = strItemName + _T( "\\" ) + strItemName;
                        }
                        
                        SetCommandStringItem( pItem->nCommandID, strItemName );
                        m_mapAccelCategory.SetAt( pItem->nCommandID, strItemName );
                    }
                }
			}
		}
	}
}

bool CXTAccelManager::Init(CFrameWnd* pFrameWnd)
{
	ASSERT_VALID( pFrameWnd ); // must be valid.
	m_pFrameWnd = pFrameWnd;

	// cannot be MDI child window.
	if ( m_pFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) )
	{
		m_pFrameWnd = m_pFrameWnd->GetParentFrame();
	}

	// save the default accelerator table.
	SaveDefaultAcceleratorTable( m_pFrameWnd );

	// Load our user accelerator if available
	if ( !Load( false ) )
	{
		if ( !LoadDefaultAccelerator( ) )
		{
			return false;
		}
	}
	else
	{
		if ( !UpdateWindowAccelerator( ) )
		{
			return false;
		}
	}

	m_bInitialized = true;
	return true;
}

bool CXTAccelManager::OnLoad()
{
	UINT   nSize   = 0;
	LPBYTE pbtData = 0;

	SAFE_DELETE_AR( m_pAccelBuffer );

	CXTRegistryManager regManager;
	if ( regManager.GetProfileBinary( szSECTION, szENTRY,
		&pbtData, &nSize ) )
	{
		m_nAccelBufferCount = ( nSize/sizeof( ACCEL ) );
		m_pAccelBuffer = ( LPACCEL )pbtData;
		return true;
	}

	SAFE_DELETE_AR( m_pAccelBuffer );

	return false;
}

bool CXTAccelManager::OnSave(LPACCEL pTable, int nCount)
{
	CXTRegistryManager regManager;
	regManager.WriteProfileBinary( szSECTION, szENTRY,
		reinterpret_cast<LPBYTE>( pTable ), sizeof( ACCEL )*nCount );

	return true;
}

bool CXTAccelManager::Load(bool bUpdateWindowAccel)
{
	if ( !OnLoad( ) )
	{
		return false;
	}

	if ( m_pAccelBuffer && bUpdateWindowAccel )
	{
		m_bChanged      = false;
		m_bDefaultInUse = false;

		UpdateWindowAccelerator( );
	}

	return true;
}

bool CXTAccelManager::Save(bool bUnInitialize/*=false*/)
{
	if ( m_bDefaultInUse )	// Do not save the default keys
	{
		// Delete the appropriate registry key
		CXTRegistryManager regManager;
		if ( !regManager.DeleteValue( szSECTION, szENTRY ) )
		{
			return false;
		}

		return true;
	}

	if(!m_bChanged)		// Do not save if we have not made any changes
		return true;

	if ( !OnSave( m_pAccelBuffer, m_nAccelBufferCount ) )
		return false;

	m_bChanged = false;

	if ( bUnInitialize )
	{
		m_bInitialized = false;
	}

	return true;
}

void CXTAccelManager::SaveDefaultAcceleratorTable(CFrameWnd* pFrameWnd)
{
	ASSERT_VALID( pFrameWnd ); // Must be valid.

	SAFE_DELETE_AR( m_pAccelDefault );

	// if this is a MDI child window, we need to make sure we get the 
	// main frame window's accelerator table.
	if ( pFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) )
	{
		pFrameWnd = pFrameWnd->GetParentFrame( );
	}

	// get the number of accelerators
	m_nAccelDefaultCount = ::CopyAcceleratorTable(
		pFrameWnd->m_hAccelTable, NULL, 0 );

	// if accelerators were found, initialize the default accelerator buffer.
	if ( m_nAccelDefaultCount != 0 )
	{
		m_pAccelDefault = new ACCEL[ m_nAccelDefaultCount ];
		::CopyAcceleratorTable( pFrameWnd->m_hAccelTable,
			m_pAccelDefault, m_nAccelDefaultCount );
	}
}

bool CXTAccelManager::LoadDefaultAccelerator()
{
	SAFE_DELETE_AR( m_pAccelBuffer );
	
	// save the old accelerator table.
	HACCEL hAccelOld = m_pFrameWnd->m_hAccelTable;
	
	// create a new table based on the defaults.
	HACCEL hAccelTable = ::CreateAcceleratorTable(
		m_pAccelDefault, m_nAccelDefaultCount );

	if ( !hAccelTable )
	{
		return false;
	}

	// destroy the old table and set the handle for
	// the applications table.
	if ( !::DestroyAcceleratorTable( hAccelOld ) )
	{
		return false;
	}

	m_pFrameWnd->m_hAccelTable = hAccelTable;
	
	// Set the number of accelerators
	m_nAccelBufferCount = m_nAccelDefaultCount;
	
	// initialize our accelerator buffer.
	m_pAccelBuffer = new ACCEL[m_nAccelBufferCount];
	::CopyAcceleratorTable( hAccelTable,
		m_pAccelBuffer, m_nAccelBufferCount );
	
	m_bChanged      = false;
	m_bDefaultInUse = true;

	return true;
}

bool CXTAccelManager::UpdateWindowAccelerator()
{
	// save the old accelerator table.
	HACCEL hAccelOld = m_pFrameWnd->m_hAccelTable;
	
	// We have an array to work with. Create an accelerator table
    HACCEL hAccelTable = ::CreateAcceleratorTable(
		m_pAccelBuffer, m_nAccelBufferCount );
	
	if ( !hAccelTable )
	{
		return false;
	}

	// destroy the old table and set the handle for
	// the applications table.
	if ( !::DestroyAcceleratorTable( hAccelOld ) )
	{
		return false;
	}

	m_pFrameWnd->m_hAccelTable = hAccelTable;

	return true;
}

void CXTAccelManager::GetKeyAssignmentStrings(UINT nCommand, CString& szKeys)
{
	// Given a Command ID, we want a list of key assignments (if any) assigned to this
	// command. 

	int x;
	for (x = 0; x < m_nAccelBufferCount; x++)
	{
		if(m_pAccelBuffer[x].cmd == nCommand)
		{
			CString szName;
			MakeKeyString(m_pAccelBuffer[x], szName);
			if(szName.GetLength())
			{
				if(szKeys.GetLength())
					szKeys += _T(", ");
				szKeys += szName;
			}
		}
	}
}

bool CXTAccelManager::GetKeyAssignmentInfo(int& nPos, UINT nCommand, CString& szName, BYTE& cVirt, WORD& wKey)
{
	if(nPos >= m_nAccelBufferCount)
		return false;

	for ( ; nPos < m_nAccelBufferCount; nPos++)
	{
		if(m_pAccelBuffer[nPos].cmd == nCommand)
		{
			MakeKeyString(m_pAccelBuffer[nPos], szName);

			cVirt = m_pAccelBuffer[nPos].fVirt;
			wKey = m_pAccelBuffer[nPos].key;

			nPos++;

			return true;
		}
	}
	return false;
}

XT_VKEYMAP CXTAccelManager::m_VirtSysKeys[] = 
{
	{ FCONTROL,		_T("Ctrl") },
	{ FALT,			_T("Alt") },
	{ FSHIFT,		_T("Shift") },
};
#define VIRTUALSYSKEYCOUNT _countof(m_VirtSysKeys)

XT_VKEYMAP CXTAccelManager::m_VirtKeys[] = 
{
	{ _T('0'),		_T("0") },
	{ _T('1'),		_T("1") },
	{ _T('2'),		_T("2") },
	{ _T('3'),		_T("3") },
	{ _T('4'),		_T("4") },
	{ _T('5'),		_T("5") },
	{ _T('6'),		_T("6") },
	{ _T('7'),		_T("7") },
	{ _T('8'),		_T("8") },
	{ _T('9'),		_T("9") },
	{ _T('A'),		_T("A") },
	{ _T('B'),		_T("B") },
	{ _T('C'),		_T("C") },
	{ _T('D'),		_T("D") },
	{ _T('E'),		_T("E") },
	{ _T('F'),		_T("F") },
	{ _T('G'),		_T("G") },
	{ _T('H'),		_T("H") },
	{ _T('I'),		_T("I") },
	{ _T('J'),		_T("J") },
	{ _T('K'),		_T("K") },
	{ _T('L'),		_T("L") },
	{ _T('M'),		_T("M") },
	{ _T('N'),		_T("N") },
	{ _T('O'),		_T("O") },
	{ _T('P'),		_T("P") },
	{ _T('Q'),		_T("Q") },
	{ _T('R'),		_T("R") },
	{ _T('S'),		_T("S") },
	{ _T('T'),		_T("T") },
	{ _T('U'),		_T("U") },
	{ _T('V'),		_T("V") },
	{ _T('W'),		_T("W") },
	{ _T('X'),		_T("X") },
	{ _T('Y'),		_T("Y") },
	{ _T('Z'),		_T("Z") },
	{ VK_LBUTTON,	_T("Left Button") },
	{ VK_RBUTTON,	_T("Right Button") },
	{ VK_CANCEL,	_T("Cancel") },
	{ VK_MBUTTON,	_T("Middle Button") },
	{ VK_BACK,		_T("Back") },
	{ VK_TAB,		_T("Tab") },
	{ VK_CLEAR,		_T("Clear") },
	{ VK_RETURN,	_T("Enter") },
	{ VK_SHIFT,		_T("Shift") },
	{ VK_CONTROL,	_T("Ctrl") },
	{ VK_MENU,		_T("Alt") },
	{ VK_PAUSE,		_T("Pause") },
	{ VK_CAPITAL,	_T("Caps Lock") },
	{ VK_ESCAPE,	_T("Esc") },
	{ VK_SPACE,		_T("Space") },
	{ VK_PRIOR,		_T("Page Up") },
	{ VK_NEXT,		_T("Page Down") },
	{ VK_END,		_T("End") },
	{ VK_HOME,		_T("Home") },
	{ VK_LEFT,		_T("Left Arrow") },
	{ VK_UP,		_T("Up Arrow") },
	{ VK_RIGHT,		_T("Right Arrow") },
	{ VK_DOWN,		_T("Down Arrow") },
	{ VK_SELECT,	_T("Select") },
	{ VK_PRINT,		_T("Print") },
	{ VK_EXECUTE,	_T("Execute") },
	{ VK_SNAPSHOT,	_T("Snapshot") },
	{ VK_INSERT,	_T("Ins") },
	{ VK_DELETE,	_T("Del") },
	{ VK_HELP,		_T("Help") },
	{ VK_LWIN ,		_T("Left Windows") },
	{ VK_RWIN,		_T("Right Windows") },
	{ VK_APPS,		_T("Application") },
	{ VK_MULTIPLY,	_T("Num *") },
	{ VK_ADD,		_T("Num +") },
	{ VK_SEPARATOR,	_T("Separator") },
	{ VK_SUBTRACT,	_T("Num -") },
	{ VK_DECIMAL,	_T("Num .") },
	{ VK_DIVIDE,	_T("Num /") },
	{ VK_F1,		_T("F1") },
	{ VK_F2,		_T("F2") },
	{ VK_F3,		_T("F3") },
	{ VK_F4,		_T("F4") },
	{ VK_F5,		_T("F5") },
	{ VK_F6,		_T("F6") },
	{ VK_F7,		_T("F7") },
	{ VK_F8,		_T("F8") },
	{ VK_F9,		_T("F9") },
	{ VK_F10,		_T("F10") },
	{ VK_F11,		_T("F11") },
	{ VK_F12,		_T("F12") },
	{ VK_NUMPAD0,	_T("Num 0") },
	{ VK_NUMPAD1,	_T("Num 1") },
	{ VK_NUMPAD2,	_T("Num 2") },
	{ VK_NUMPAD3,	_T("Num 3") },
	{ VK_NUMPAD4,	_T("Num 4") },
	{ VK_NUMPAD5,	_T("Num 5") },
	{ VK_NUMPAD6,	_T("Num 6") },
	{ VK_NUMPAD7,	_T("Num 7") },
	{ VK_NUMPAD8,	_T("Num 8") },
	{ VK_NUMPAD9,	_T("Num 9") },
	{ VK_NUMLOCK,	_T("Num Lock") },
	{ VK_SCROLL,	_T("Scrl Lock") },
	{ VK_ATTN,		_T("Attn") },
	{ VK_CRSEL,		_T("Crsel") },
	{ VK_EXSEL,		_T("Exsel") },
	{ VK_EREOF,		_T("Ereof") },
	{ VK_PLAY,		_T("Play") },
	{ VK_ZOOM,		_T("Zoom") },
	{ VK_NONAME,	_T("No Name") },
	{ VK_PA1,		_T("Pa1") },
	{ VK_OEM_CLEAR,	_T("Oem Clear") },
};

#define VIRTUALKEYCOUNT _countof(m_VirtKeys)

LPCTSTR CXTAccelManager::GetVirtualKeyString(WORD wKey)
{
	int index;
	for (index = 0; index < VIRTUALKEYCOUNT; index++)
	{
		if(m_VirtKeys[index].wKey == wKey)
			return m_VirtKeys[index].szKeyName;
	}
	return _T("");
}

void CXTAccelManager::MakeKeyString(ACCEL Accel, CString& szName)
{
	szName = _T("");

	if(Accel.key == 0)
		return;

	int i;
	for (i = 0; i < _countof(m_VirtSysKeys); i++)
	{
		if(Accel.fVirt & m_VirtSysKeys[i].wKey)
		{
			szName += m_VirtSysKeys[i].szKeyName;
			szName += _T("+");
		}
	}

	for (i = 0; i < _countof(m_VirtKeys); i++)
	{
		if(Accel.key == m_VirtKeys[i].wKey)
		{
			szName += m_VirtKeys[i].szKeyName;
			return;
		}
	}
}

void CXTAccelManager::MakeDisplayName( CXTString& strText )
{
	// We do not want any mnemonic symbols
	_xtAfxStripMnemonics( strText );

	// Trim any keyboard shortcut info
	int nLoc = strText.Find( _T( '\t' ) );
	if ( nLoc != -1 )
    {
		strText = strText.Left( nLoc );
    }
}

void CXTAccelManager::GetCategoryList(CComboBox& comboCategory)
{
    POSITION pos;
    for ( pos = m_comboCategoryList.GetHeadPosition( ); pos; m_comboCategoryList.GetNext( pos ) )
    {
        XT_CATEGORY category = m_comboCategoryList.GetAt( pos );
        int iIndex = comboCategory.AddString( category.strCategory );
        comboCategory.SetItemData( iIndex, (DWORD_PTR)( HMENU )category.hSubMenu );
    }
}

bool CXTAccelManager::AddKeyAssignment(UINT nCommand, BYTE cVirt, WORD wKey)
{
	// Allocate a new buffer
	LPACCEL pNewAccelBuffer = new ACCEL[m_nAccelBufferCount+1];

	// Get a copy of the current keys
	memcpy(pNewAccelBuffer, m_pAccelBuffer, sizeof(ACCEL)*m_nAccelBufferCount);

	// Set the value of the new one
	pNewAccelBuffer[m_nAccelBufferCount].cmd = (WORD)nCommand;
	pNewAccelBuffer[m_nAccelBufferCount].fVirt = cVirt;
	pNewAccelBuffer[m_nAccelBufferCount].key = wKey;

	// Delete our old buffer
	SAFE_DELETE_AR( m_pAccelBuffer );

	// Keep our new buffer
	m_pAccelBuffer = pNewAccelBuffer;

	// We have added one extra keyboard shortcut
	m_nAccelBufferCount++;

	// Default is no longer in use
	m_bDefaultInUse = false;

	// Our data has been changed
	m_bChanged = true;

	return true;
}

bool CXTAccelManager::GetKeyAssignment(BYTE cVirt, WORD wKey, UINT& nCommand)
{
	// Search the accelerator looking for the assignment
	LPACCEL Accel = m_pAccelBuffer;
	int nCount = m_nAccelBufferCount;

	int x;
	for (x = 0; x < nCount; x++)
	{
		if( (Accel[x].fVirt&(FCONTROL|FALT|FSHIFT)) == (cVirt&(FCONTROL|FALT|FSHIFT)) &&
			Accel[x].key == wKey)
		{
			nCommand = Accel[x].cmd;
			return true;
		}
	}
	return false;
}

bool CXTAccelManager::DeleteKeyAssignment(BYTE cVirt, WORD wKey)
{
	// Delete a virtual key assignment.
	int iIndex;
	for ( iIndex = 0; iIndex < m_nAccelBufferCount; ++iIndex )
	{
		if ( ( m_pAccelBuffer[ iIndex ].fVirt & ( FCONTROL|FALT|FSHIFT ) ) == 
			( cVirt & ( FCONTROL|FALT|FSHIFT ) ) && m_pAccelBuffer[ iIndex ].key == wKey )
		{
			m_bChanged      = true;
			m_bDefaultInUse = false;
			break;
		}
	}
	
	if ( iIndex == m_nAccelBufferCount )
	{
		return false;
	}
	
	LPACCEL pNewAccelBuffer = new ACCEL[ m_nAccelBufferCount-1 ];
	memcpy( pNewAccelBuffer, m_pAccelBuffer, iIndex*sizeof( ACCEL ) );
	memcpy( &pNewAccelBuffer[ iIndex ], &m_pAccelBuffer[ iIndex+1 ], ( m_nAccelBufferCount-iIndex-1 )*sizeof( ACCEL ) );

	// Delete our old buffer
	SAFE_DELETE_AR( m_pAccelBuffer );

	// Keep our new buffer
	m_pAccelBuffer = pNewAccelBuffer;

	// We have deleted one keyboard shortcut
	m_nAccelBufferCount--;

	return true;
}

void CXTAccelManager::AddSwapoutCommand(UINT nSwapOutID, UINT nCommandID, LPCTSTR szName)
{
	CXTAccelSwapOutItemList* pList = NULL;
	if(!m_SwapOutList.Lookup(nSwapOutID, pList))
	{
		pList = new CXTAccelSwapOutItemList;
		m_SwapOutList.SetAt(nSwapOutID, pList);
	}
	pList->SetAt(nCommandID, CString(szName));
}

void CXTAccelManager::AddExtraCommand(LPCTSTR szCategory, UINT nCommandID, LPCTSTR szName)
{
	XT_EXTRA_ACCELITEM* pItem = new XT_EXTRA_ACCELITEM;

	pItem->nCommandID = nCommandID;
	pItem->szCategory = szCategory;
	pItem->szName = szName;

	m_ExtraItems.AddTail(pItem);
}

void CXTAccelManager::GetNextCommandStringItem(POSITION& rPos, UINT& nCommandID, CString& szString)
{
	CXTMapString& item = m_mapAccelString.GetNext(rPos);
	nCommandID = item.m_nID;
	szString   = item.m_szName;
}

bool CXTAccelManager::LookupCommandStringItem(UINT nCommandID, CString& szString)
{
	POSITION rPos = GetCommandStringStartPosition();
	while(rPos)
	{
		UINT nCmdID;
		GetNextCommandStringItem(rPos, nCmdID, szString);
		if(nCmdID == nCommandID)
			return true;
	}
	return false;
}

void CXTAccelManager::SetCommandStringItem(UINT nCommandID, CString szName)
{
	CXTMapString item;
	item.m_nID    = nCommandID;
	item.m_szName = szName;
	m_mapAccelString.AddTail(item);
}

bool CXTAccelManager::TranslateAccelerator(MSG* pMsg)
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch ( pMsg->wParam )
			{
			case VK_SHIFT:   return false;
			case VK_CONTROL: return false;
			case VK_MENU:    return false;
			}

			BYTE fVirt = FVIRTKEY|FNOINVERT;

			if ( ::GetKeyState( VK_MENU ) < 0 ) {
				fVirt |= FALT;
			}
			if ( ::GetKeyState( VK_CONTROL ) < 0 ) {
				fVirt |= FCONTROL;
			}
			if ( ::GetKeyState( VK_SHIFT ) < 0 ) {
				fVirt |= FSHIFT;
			}

			UINT nCommand;
			if ( GetKeyAssignment( fVirt, ( WORD )pMsg->wParam, nCommand ) )
			{
				return true;
			}
		}
	}

	return false;
}

CXTAccelManager& CXTAccelManager::Get()
{
	static CXTAccelManager theManager;
	return theManager;
}
