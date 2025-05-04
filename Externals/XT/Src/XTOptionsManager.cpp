// XTOptionsManager.cpp: implementation of the CXTOptionsManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResource.h"
#include "XTRegistryManager.h"
#include "XTOptionsManager.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTAccelManager.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const TCHAR szENTRY[]   = _T( "Options" );
const TCHAR szSECTION[] = _T( "Settings" );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTOptionsManager::CXTOptionsManager()
: m_bInitialized(false)
, m_bAutoSave(true)
, m_bChanged(false)
, m_bRemoveAll(false)
, m_pFrameWnd(0)
{

}

CXTOptionsManager::~CXTOptionsManager()
{
	if ( m_bAutoSave )
	{
		Save( );
	}
}

bool CXTOptionsManager::OnLoad()
{
	UINT   nSize   = 0;
	LPBYTE pbtData = 0;

	CXTRegistryManager regManager;
	if ( regManager.GetProfileBinary( szSECTION, szENTRY,
		&pbtData, &nSize ) )
	{
		if (nSize == sizeof( XT_OPTIONS ))
		{
			memcpy( &m_options, pbtData, nSize);
			SAFE_DELETE( pbtData );

			xtAfxData.bMenuShadows        = m_options.bMenuShadows;
			xtAfxData.bMenuRecentCommands = m_options.bMenuRecentCommands;
			xtAfxData.bToolBarScreenTips  = m_options.bToolBarScreenTips;
			xtAfxData.bShowFullAfterDelay = m_options.bShowFullAfterDelay;
			xtAfxData.bToolBarAccelTips   = m_options.bToolBarAccelTips;
			xtAfxData.bToolBarVisualize   = m_options.bToolBarVisualize;

			CXTCoolMenu::m_nAnimationType = m_options.nAnimationType;
			
			return true;
		}
		SAFE_DELETE( pbtData );
	}
	// use default settings

	m_options.bMenuShadows        = ( xtAfxData.bMenuShadows        == TRUE );
	m_options.bMenuRecentCommands = ( xtAfxData.bMenuRecentCommands == TRUE );
	m_options.bToolBarScreenTips  = ( xtAfxData.bToolBarScreenTips  == TRUE );
	m_options.bShowFullAfterDelay = ( xtAfxData.bShowFullAfterDelay == TRUE );
	m_options.bToolBarAccelTips   = ( xtAfxData.bToolBarAccelTips   == TRUE );
	m_options.bToolBarVisualize   = ( xtAfxData.bToolBarVisualize   == TRUE );
	m_options.nAnimationType = CXTCoolMenu::m_nAnimationType;

	return false;
}

bool CXTOptionsManager::OnSave()
{
	CXTRegistryManager regManager;
	regManager.WriteProfileBinary( szSECTION, szENTRY,
		reinterpret_cast<LPBYTE>( &m_options ), sizeof( XT_OPTIONS ) );

	xtAfxData.bMenuShadows        = m_options.bMenuShadows;
	xtAfxData.bMenuRecentCommands = m_options.bMenuRecentCommands;
	xtAfxData.bToolBarScreenTips  = m_options.bToolBarScreenTips;
	xtAfxData.bShowFullAfterDelay = m_options.bShowFullAfterDelay;
	xtAfxData.bToolBarAccelTips   = m_options.bToolBarAccelTips;
	xtAfxData.bToolBarVisualize   = m_options.bToolBarVisualize;
	
	CXTCoolMenu::m_nAnimationType = m_options.nAnimationType;

	return true;
}

bool CXTOptionsManager::Save()
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
        if ( !OnSave( ) )
        {
            return false;
        }
    }

    m_bChanged = false;
    
	return true;
}

bool CXTOptionsManager::Load()
{
	if ( !OnLoad( ) )
	{
		return false;
	}

	return true;
}

bool CXTOptionsManager::Init(CFrameWnd* pFrameWnd, CXTCoolMenu* pCoolMenu)
{
	ASSERT_VALID(pFrameWnd); // must be valid.
	m_pFrameWnd = pFrameWnd;

    ASSERT(pCoolMenu != NULL);
    m_pCoolMenu = pCoolMenu;

	// cannot be MDI child window.
	if ( m_pFrameWnd->IsKindOf( RUNTIME_CLASS( CMDIChildWnd ) ) )
	{
		m_pFrameWnd = m_pFrameWnd->GetParentFrame();
	}

	// Load if available
	if ( !Load( ) )
	{
		
	}

	m_bInitialized = true;

	return true;
}

void CXTOptionsManager::SetOptions(XT_OPTIONS& options)
{
	m_options = options;
	m_bChanged = true;
}

XT_OPTIONS& CXTOptionsManager::GetOptions()
{
	return m_options;
}

void CXTOptionsManager::Reset()
{
    if ( AfxMessageBox( XT_IDS_AREYOUSURE_2, MB_ICONWARNING|MB_YESNO ) == IDYES )
    {
        m_pCoolMenu->ResetUsageData( );
    }
}

CXTOptionsManager& CXTOptionsManager::Get()
{
	static CXTOptionsManager theManager;
	return theManager;
}
