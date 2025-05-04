// XTToolsManager.cpp: implementation of the CXTToolsManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTRegistryManager.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTMenuBar.h"
#include "XTFunctions.h"

#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR szENTRY[]   = _T( "Tools" );
const TCHAR szSECTION[] = _T( "Settings" );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTToolsManager::CXTToolsManager()
: m_iArgPopup(0)
, m_iDirPopup(0)
, m_iNormalIndex(0)
, m_iWindowIndex(0)
, m_iHTMLIcon(-1)
, m_hToolsMenu(0)
, m_pMenuBar(0)
, m_pCoolMenu(0)
, m_hImageList(0)
, m_pFrameWnd(0)
, m_nToolsCount(0)
, m_pToolsArray(0)
, m_bChanged(false)
, m_bAutoSave(true)
, m_bInitialized(false)
, m_bRemoveAll(false)
{
	// Load our user tools if available
	if ( Load( ) )
	{
		// construct the tools menu.
		BuildToolsMenu( TRUE );
	}
}

CXTToolsManager::~CXTToolsManager()
{
	if ( m_bAutoSave )
	{
		Save( );
	}
	
	SAFE_DELETE_AR( m_pToolsArray );

	if ( m_hToolsMenu )
	{
		::DestroyMenu( m_hToolsMenu );
	}

	m_mapIconCmd.RemoveAll( );
}

void CXTToolsManager::RemoveAllTools()
{
    m_nToolsCount = 0;
	SAFE_DELETE_AR( m_pToolsArray );
    m_bRemoveAll = true;
    m_bChanged   = true;
}

bool CXTToolsManager::RemoveTool(XT_TOOL* pTool)
{
	int iIndex;
	for ( iIndex = 0; iIndex < m_nToolsCount; iIndex++ )
	{
		if ( &m_pToolsArray[ iIndex ] == pTool )
		{
			m_bChanged = true;
			break;
		}
	}
	
	if ( iIndex == m_nToolsCount )
	{
		return false;
	}
	
	XT_TOOL* pToolsArray = new XT_TOOL[ m_nToolsCount-1 ];
	memcpy( pToolsArray, m_pToolsArray, iIndex*sizeof( XT_TOOL ) );
	memcpy( &pToolsArray[ iIndex ], &m_pToolsArray[ iIndex+1 ], ( m_nToolsCount-iIndex-1 )*sizeof( XT_TOOL ) );

	// Delete our old buffer
	SAFE_DELETE_AR( m_pToolsArray );

	// Keep our new buffer
	m_pToolsArray = pToolsArray;

	// We have deleted one keyboard shortcut
	m_nToolsCount--;

	return true;
}

UINT CXTToolsManager::GetNextCmdID()
{
	if ( !m_hToolsMenu || !::IsMenu( m_hToolsMenu ) )
	{
		return 0;
	}

	UINT uCmdID;
	for ( uCmdID = XT_IDC_TOOLSMANAGER_MIN; uCmdID <= XT_IDC_TOOLSMANAGER_MAX; ++ uCmdID )
	{
		int iCount = ::GetMenuItemCount( m_hToolsMenu );
		int iIndex;
		for ( iIndex = 0; iIndex < iCount; ++iIndex )
		{
			XT_MENUITEMINFO info;
			info.fMask = MIIM_ID;
			
			::GetMenuItemInfo( m_hToolsMenu, iIndex, TRUE, &info );

			if ( info.wID == uCmdID )
			{
				break;
			}
		}

		if ( iIndex == iCount )
		{
			return uCmdID;
		}
	}

	return 0;
}

void CXTToolsManager::AddTool(XT_TOOL* pTool)
{
	XT_TOOL* pToolsArray = new XT_TOOL[ m_nToolsCount+1 ];

	// Get a copy of the current tools
	memcpy( pToolsArray, m_pToolsArray, sizeof( XT_TOOL )*m_nToolsCount );

	pToolsArray[ m_nToolsCount ].szTitle[ 0 ] = '\0';
	pToolsArray[ m_nToolsCount ].szCmd[ 0 ]   = '\0';
	pToolsArray[ m_nToolsCount ].szArg[ 0 ]   = '\0';
	pToolsArray[ m_nToolsCount ].szDir[ 0 ]   = '\0';

	// Set the value of the new tool
	if ( _tcslen( pTool->szTitle ) )
	{
		_tcscpy( pToolsArray[ m_nToolsCount ].szTitle, pTool->szTitle );
	}
	if ( _tcslen( pTool->szCmd ) )
	{
		_tcscpy( pToolsArray[ m_nToolsCount ].szCmd, pTool->szCmd );
	}
	if ( _tcslen( pTool->szArg ) )
	{
		_tcscpy( pToolsArray[ m_nToolsCount ].szArg, pTool->szArg );
	}
	if ( _tcslen( pTool->szDir ) )
	{
		_tcscpy( pToolsArray[ m_nToolsCount ].szDir, pTool->szDir );
	}
	
	if ( pTool->uCmdID != 0 )
	{
		pToolsArray[ m_nToolsCount ].uCmdID = pTool->uCmdID;
	}
	else
	{
		pToolsArray[ m_nToolsCount ].uCmdID = GetNextCmdID();
	}

	// Delete our old buffer
	SAFE_DELETE_AR( m_pToolsArray );

	// Keep our new buffer
	m_pToolsArray = pToolsArray;

	// We have added one extra tool
	m_nToolsCount++;

	// Our data has been changed
	m_bChanged = true;
    m_bRemoveAll = false;

	// Refresh the tools menu.
	BuildToolsMenu( FALSE );
}

bool CXTToolsManager::OnLoad()
{
	UINT   nSize   = 0;
	LPBYTE pbtData = 0;

	SAFE_DELETE_AR( m_pToolsArray );

	CXTRegistryManager regManager;
	if ( regManager.GetProfileBinary( szSECTION, szENTRY,
		&pbtData, &nSize ) )
	{
		m_nToolsCount = ( nSize/sizeof( XT_TOOL ) );
		m_pToolsArray = ( XT_TOOL* )pbtData;
		return true;
	}

	SAFE_DELETE_AR( m_pToolsArray );

	return false;
}

bool CXTToolsManager::OnSave(XT_TOOL* pToolsArray, int nCount)
{
	CXTRegistryManager regManager;
	regManager.WriteProfileBinary( szSECTION, szENTRY,
		reinterpret_cast<LPBYTE>( pToolsArray ), sizeof( XT_TOOL )*nCount );

	return true;
}

bool CXTToolsManager::Save(bool bUnInitialize)
{
	if ( m_bRemoveAll )	// Do not save the tools
	{
		// Delete the appropriate registry value
		CXTRegistryManager regManager;
        if ( !regManager.DeleteValue( szSECTION, szENTRY ) )
        {
            return false;
        }
    }
    else
    {
        if ( !m_bChanged )
        {
            return true;
        }
        
        if ( !OnSave( m_pToolsArray, m_nToolsCount ) )
        {
            return false;
        }
        
        if ( bUnInitialize )
        {
            m_bInitialized = false;
        }
    }

    m_bChanged = false;
	BuildToolsMenu( FALSE );
    
	return true;
}

bool CXTToolsManager::Load()
{
	if ( !OnLoad( ) )
	{
		return false;
	}

	return true;
}

HMENU CXTToolsManager::ToolsMenuExists( HMENU hMenu, int& iMenuItem )
{
    int iCount = ::GetMenuItemCount( hMenu );
    
    CString strTools = CXTToolsManager::Get( ).GetMenuTitle( );
    _xtAfxStripMnemonics( strTools );

    for ( iMenuItem = 0; iMenuItem < iCount; ++iMenuItem )
    {
        TCHAR szMenuItem[ 256 ];
        ::GetMenuString( hMenu, iMenuItem,
            szMenuItem, 256, MF_BYPOSITION );
        
        CString strMenuItem = szMenuItem;
        _xtAfxStripMnemonics( strMenuItem );
        
        if ( strTools.CompareNoCase( strMenuItem ) == 0 )
        {
            return ::GetSubMenu( hMenu, iMenuItem );
        }
    }

	iMenuItem = -1;

    return NULL;
}

bool CXTToolsManager::InsertToolsMenu(HMENU hNewMenu, int iIndex)
{
	if ( !hNewMenu || !::IsMenu( hNewMenu ) )
	{
		return false;
	}

	int iCount = ::GetMenuItemCount( hNewMenu );
	
	if ( iCount != -1 )
	{
		if ( iIndex >= iCount )
		{
			iIndex = 0;
		}
		
		// rebuild tools menu.
		CXTToolsManager::Get( ).BuildToolsMenu( TRUE );
		
		// check to see if a 'Tools' menu already exists.
		int iMenuItem;
		HMENU hExistMenu = ToolsMenuExists( hNewMenu, iMenuItem );
		HMENU hToolsMenu = CXTToolsManager::Get( ).GetToolsMenu( );
		bool  bMenuExist = ( hExistMenu == hToolsMenu );
		
		// map the original 'Tools' menu found with the current menu.
		if ( ::IsMenu( hExistMenu ) && !bMenuExist )
		{
			m_mapToolsMenu[ hNewMenu ] = hExistMenu;
			
			iIndex = iMenuItem;
			::RemoveMenu( hNewMenu, iMenuItem, MF_BYPOSITION );
		}
		
		// if one already exists, remove it, but save the menu handle we will
		// need this to update the tools menu...
		if ( ::IsMenu( hExistMenu ) && ::IsMenu( m_mapToolsMenu[ hNewMenu ] ) )
		{
			// insert a separator.
			::InsertMenu( hToolsMenu, 0, MF_BYPOSITION | MF_SEPARATOR, 0, ( LPCTSTR )0 );
			
			// now update the 'Tools' menu with the existing menu items.
			int iCount = ::GetMenuItemCount( m_mapToolsMenu[ hNewMenu ] );
			int iMenuItem;
			for ( iMenuItem = iCount-1; iMenuItem >= 0; --iMenuItem )
			{
				TCHAR szItemName[256];
				XT_MENUITEMINFO info;
				info.fType      = MFT_STRING;
				info.fMask      = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE | MIIM_STATE | MIIM_DATA;
				info.cch        = _countof( szItemName );
				info.dwTypeData = szItemName;
				
				if ( ::GetMenuItemInfo( m_mapToolsMenu[ hNewMenu ], iMenuItem, TRUE, &info ) )
				{
					::InsertMenuItem( hToolsMenu, 0, TRUE, &info );
				}
			}
		}
		
		// insert the tools menu.
		if ( ::IsMenu( hToolsMenu ) && !bMenuExist )
		{
			TCHAR szItemName[256];
			_tcscpy( szItemName, CXTToolsManager::Get( ).GetMenuTitle( ) );
			
			XT_MENUITEMINFO info;
			info.fType      = MFT_STRING;
			info.fMask      = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE | MIIM_STATE | MIIM_DATA;
			info.cch        = _countof( szItemName );
			info.dwTypeData = szItemName;
			info.fState     = MFS_ENABLED;
			info.wID        = 0;
			info.hSubMenu   = hToolsMenu;
			
			::InsertMenuItem( hNewMenu, iIndex, TRUE, &info );
		}

		return true;
	}

	return false;
}

bool CXTToolsManager::Init(CFrameWnd* pFrameWnd, CXTMenuBar* pMenuBar, CXTCoolMenu* pCoolMenu)
{
	ASSERT_VALID( pFrameWnd ); // must be valid.
	m_pFrameWnd = pFrameWnd;

	m_pMenuBar = pMenuBar;

	ASSERT( pCoolMenu != NULL );
	m_pCoolMenu = pCoolMenu;

	// cannot be MDI child window.
	if ( m_pFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) )
	{
		m_pFrameWnd = m_pFrameWnd->GetParentFrame();
	}

	CMenu* pMenu = 0;
	if ( m_pMenuBar )
	{
		pMenu = m_pMenuBar->GetMenu( );
	}
	else
	{
		pMenu = m_pFrameWnd->GetMenu( );
	}

	if ( pMenu && ::IsMenu( pMenu->m_hMenu ) )
	{
		// insert the tools menu.
		if ( InsertToolsMenu( pMenu->m_hMenu, m_iNormalIndex ) )
		{
			m_pMenuBar->LoadMenu( pMenu->m_hMenu, NULL );
		}

		// set the initialized flag to true.
		m_bInitialized = true;

		return true;
	}

	return false;
}

void CXTToolsManager::GetToolsList(CXTEditListBox& editListBox)
{
    editListBox.ResetContent( );
    
	int i;
	for ( i = m_nToolsCount-1; i >= 0; --i )
	{
		int iItem = editListBox.AddString( m_pToolsArray[ i ].szTitle );
		editListBox.SetItemData( iItem, (DWORD_PTR)&m_pToolsArray[ i ] );
	}
}

int CXTToolsManager::GetHTMLIcon()
{
    if ( m_iHTMLIcon == -1 )
    {
        TCHAR* szTempDir = _tgetenv( _T( "TEMP" ) );
        
        CString strTempFile = szTempDir;

        if ( strTempFile.GetLength( ) == 0 ) {
            strTempFile = _T( "c:\\~tempfile.htm" );
        }
        else {
            strTempFile += _T( "\\~tempfile.htm" );
        }
        
        CStdioFile fileWrite( strTempFile, CFile::modeCreate | CFile::modeWrite | CFile::typeText );
        fileWrite.WriteString( _T( "<html>\n<head>\n<title>Temporary File</title>\n</head>\n\n<body>\n<font face=\"verndana\" size=\"2\">\n\nThis file is temporary and can be deleted.\n\n</font>\n\n</body>\n</html>\n" ) );
        fileWrite.Close();
        
		SHFILEINFO sfi;
        ::SHGetFileInfo( strTempFile, 0, &sfi,
            sizeof( SHFILEINFO ), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
        
        TRY
        {
            CFile::Remove( strTempFile );
        }
        CATCH( CFileException, e )
        {
            #ifdef _DEBUG
            afxDump << "File " << strTempFile << " cannot be removed\n";
            #endif
        }
        END_CATCH

        m_iHTMLIcon = sfi.iIcon;
    }

    return m_iHTMLIcon;
}

void CXTToolsManager::BuildToolsMenu(BOOL bDeleteAll)
{
	bool bNonManagerCmds = false;

	// if the tools menu already exists...
	if ( m_hToolsMenu && ::IsMenu( m_hToolsMenu ) )
	{
		// reset image map.
		m_mapIconCmd.RemoveAll( );

		// remove tools manager menu commands.
		int iCount = ::GetMenuItemCount( m_hToolsMenu );
		int iItem;
		for ( iItem = iCount-1; iItem >= 0; --iItem )
		{
			if ( bDeleteAll )
			{
				::DeleteMenu( m_hToolsMenu, iItem, MF_BYPOSITION );
			}
			else
			{
				// get the command id for this menu item.
				int iCmdID = ::GetMenuItemID( m_hToolsMenu, iItem );

				// remove the customize menu command.
				if ( iCmdID == XT_IDC_CUSTOMIZE )
				{
					::DeleteMenu( m_hToolsMenu, iItem, MF_BYPOSITION );
					
					// remove the separator if any.
					if ( ::GetMenuItemID( m_hToolsMenu, iItem-1 ) == 0 )
					{
						::DeleteMenu( m_hToolsMenu, iItem-1, MF_BYPOSITION );
					}
				}
				
				// remove only the toolsmanager command ids.
				else if ( iCmdID >= XT_IDC_TOOLSMANAGER_MIN && iCmdID <= XT_IDC_TOOLSMANAGER_MAX )
				{
					::DeleteMenu( m_hToolsMenu, iItem, MF_BYPOSITION );
				}

				// non tools manager commands were found.
				else
				{
					bNonManagerCmds = true;
				}
			}
		}
	}
	else
	{
		// create the popup menu to be displayed.
		m_hToolsMenu = ::CreatePopupMenu( );
	}

	// if non tools manager commands were found, make sure a separator
	// has been appended to the menu.
	if ( bNonManagerCmds )
	{
		int iCount = ::GetMenuItemCount( m_hToolsMenu )-1;
		if ( iCount > 0 && ( ::GetMenuItemID( m_hToolsMenu, iCount ) != 0 ) )
		{
			::AppendMenu( m_hToolsMenu, MF_SEPARATOR, 0, 0 );
		}
	}

	// get a handle to the system image list.
    SHFILEINFO sfi;
	if ( m_hImageList == 0 )
	{
		m_hImageList = ( HIMAGELIST )::SHGetFileInfo( _T( "C:\\" ), 0, &sfi, 
			sizeof( SHFILEINFO ), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
	}

	// iterate thru each user command and add it to the tools menu.
	int i;
	for ( i = m_nToolsCount-1; i >= 0; --i )
	{
		// get a pointer to the tool data for this command.
        XT_TOOL* pTool = &m_pToolsArray[ i ];
		if ( !pTool )
		{
			continue;
		}

		UINT iCmdID = pTool->uCmdID;

		TRACE2( "%d : %s\n", pTool->uCmdID, pTool->szTitle );

		// must be in valid range
		ASSERT( iCmdID >= XT_IDC_TOOLSMANAGER_MIN && iCmdID <= XT_IDC_TOOLSMANAGER_MAX );

		// if the command does not exist...
		if ( !_tcslen( pTool->szCmd ) || _taccess( pTool->szCmd, 0 ) == -1 )
		{
			// if 'http://' or 'ftp://' is found in the string, assume URL.
			CXTString strBuffer = pTool->szCmd;
			if ( ( strBuffer.Find( _T( "http://"), 0 ) != -1 ) ||
			     ( strBuffer.Find( _T( "ftp://"),  0 ) != -1 ) )
			{
                sfi.iIcon = GetHTMLIcon( );
			}
            else
            {
				continue;
            }
		}
        else
        {
            // get the icon index from the shell.
		    ::SHGetFileInfo( pTool->szCmd, 0, &sfi,
			    sizeof( SHFILEINFO ), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
        }

		// construct the icon to be used in the menu and update the cool menus.
		HICON hIcon = ImageList_ExtractIcon( 0, m_hImageList, sfi.iIcon );
		m_mapIconCmd.SetAt( iCmdID, hIcon, hIcon, 0 );

		// update cool menu image map.
		if ( m_pCoolMenu != NULL ) {
			m_pCoolMenu->AddImageToMap( iCmdID, hIcon, m_pCoolMenu->GetIconSize( ) );
		}

		// add the item to our tools map
		m_mapTools.SetAt( iCmdID, pTool );

		// add the item to the tools menu.
        ::AppendMenu( m_hToolsMenu, MF_STRING|MF_ENABLED, iCmdID, pTool->szTitle );
	}

	// add a separator if there are user defined tools.
    if ( m_nToolsCount )
    {
        ::AppendMenu( m_hToolsMenu, MF_SEPARATOR, 0, NULL );
    }

	// append the customize menu command.
    CString strMenuText;
    strMenuText.LoadString( XT_IDS_COLOR_CUST );
    ::AppendMenu( m_hToolsMenu, MF_STRING|MF_ENABLED, XT_IDC_CUSTOMIZE, strMenuText );
}

CXTToolsManager& CXTToolsManager::Get()
{
	static CXTToolsManager theManager;
	return theManager;
}

CString CXTToolsManager::GetMenuTitle() const
{
	CString strToolsTitle = m_strToolsTitle;

	if ( strToolsTitle.IsEmpty( ) )
	{
		strToolsTitle.LoadString( XT_IDS_TOOLS );
	}

	return strToolsTitle;
}
