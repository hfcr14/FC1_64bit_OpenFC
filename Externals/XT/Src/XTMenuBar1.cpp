// XTMenuBar1.cpp : implementation of the CXTMenuBar class.
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
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTMenuBar.h"
#include "XTOSVersionInfo.h"
#include "XTCustomizeAPI.h"
#include "XTCrc32.h"

typedef CXTCustomizeConnection::GETCUSTOMGROUPS GETCUSTOMGROUPS;
typedef CXTCustomizeConnection::SETCUSTMODE SETCUSTMODE;
typedef CXTCustomizeConnection::GETMENUPOPUP GETMENUPOPUP;

class CXTMenuCustomHandler : public CObject
{
	 CXTMenuBar* m_pThis;
public:
	CXTMenuCustomHandler(CXTMenuBar* pThis)
	: m_pThis(pThis)
	{
		CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
		connection->GetCustomGroups.Add(this, &CXTMenuCustomHandler::ReportCustomGroups);
	}

	~CXTMenuCustomHandler()
	{
		CXTCustomizeConnection::GetInstance()->Remove(this);
	}

	void ReportCustomGroups(CXTCustomGroups& groups)
	{
		CXTCrc32 crc;
		CMenu* pMenu = m_pThis->GetMenu();

		// if a NULL pointer was returned or the menu is not valid, return.
		if (!pMenu || !::IsMenu( pMenu->m_hMenu ) ) {
			return;
		}

		UINT nMenuItems = pMenu->GetMenuItemCount();
		UINT nIndex;
		for(nIndex = 0; nIndex < nMenuItems; nIndex++ )
		{		
			crc.SetCrc32(0);
			CXTString strMenuText;
			if( pMenu->GetMenuString(nIndex, strMenuText, MF_BYPOSITION ))
			{
				crc.Compute((LPCTSTR)strMenuText, strMenuText.GetLength() * sizeof(TCHAR));
				DWORD zero = 0;
				crc.Compute(&zero, sizeof(zero));				
				DWORD dwCrcPopup = crc.GetCrc32();
				int pos = strMenuText.Find(_T('&'));
				if (pos >= 0)
				{
					strMenuText.Delete(pos);
				}
				CXTCustomGroupInfo* group = new CXTCustomGroupInfo();
				group->m_nSortPriority = 1000 + nIndex;
				groups.Add(group);
				group->m_strTitle = strMenuText;
					
				CMenu* pPopupMenu =	pMenu->GetSubMenu(nIndex);
				UINT nMenuItems = pPopupMenu->GetMenuItemCount();
					
				XT_MENUITEMINFO menuInfo;
				menuInfo.fMask = MIIM_ID|MIIM_SUBMENU;

				UINT i;
				for(i = 0; i < nMenuItems; i++ )
				{
					if (::GetMenuItemInfo(pPopupMenu->m_hMenu, i, TRUE, &menuInfo))
					{
						CXTString strMenuText;
						if(pPopupMenu->GetMenuString(i, strMenuText, MF_BYPOSITION ))
						{
							// compute CRC for the menu dropdown/this popup
							crc.SetCrc32(dwCrcPopup);
							crc.Compute((LPCTSTR)strMenuText, strMenuText.GetLength() * sizeof(TCHAR));
							crc.Compute(&zero, sizeof(zero));

							int nHotPos = strMenuText.Find(_T('&'));
							if (nHotPos >= 0)
							{
								strMenuText.Delete(nHotPos);
							}
							int iTabPos = strMenuText.Find(_T('\t'));

							if (iTabPos >= 0)
							{
								strMenuText  = strMenuText.Left(iTabPos);
							}

							UINT nCmdID;
							CString strLabel;
							if (menuInfo.hSubMenu)
							{								
								strLabel = strMenuText;
								nCmdID = 0;
							}
							else
							{
								nCmdID = menuInfo.wID;
							}

							CXTCustomGroupItemInfo info(CXTCustomGroupItemInfo::Button);
							info.m_strTitle = strMenuText;
							info.m_strLabel = strLabel;
							info.m_nCmdID = nCmdID;
							XT_DROPDOWNBUTTON dropDown;
							if (nCmdID == 0)
							{
								dropDown.bArrow = true;
								dropDown.bMenuBarPopup = true;
								dropDown.dwMenuBarPopupID = crc.GetCrc32();
								info.m_pDropDown = &dropDown;
							}
							group->m_items.Add(info.Clone());
						}
					}
				}
			}
		}
	}
	
};

HMENU CXTMenuBar::GetMenuPopupByCrc(DWORD dwMenuPopupID)
{
	CXTCrc32 crc;
	CMenu* pMenu = GetMenu();
	
	// if a NULL pointer was returned or the menu is not valid, return.
	if (!pMenu || !::IsMenu( pMenu->m_hMenu ) ) {
		return NULL;
	}
	
	UINT nMenuItems = pMenu->GetMenuItemCount();
	UINT nIndex;
	for(nIndex = 0; nIndex < nMenuItems; nIndex++ )
	{		
		crc.SetCrc32(0);
		CXTString strMenuText;
		if( pMenu->GetMenuString(nIndex, strMenuText, MF_BYPOSITION ))
		{
			crc.Compute((LPCTSTR)strMenuText, strMenuText.GetLength() * sizeof(TCHAR));
			DWORD zero = 0;
			crc.Compute(&zero, sizeof(zero));				
			DWORD dwCrcPopup = crc.GetCrc32();
			int pos = strMenuText.Find(_T('&'));
			if (pos >= 0)
			{
				strMenuText.Delete(pos);
			}
			CXTCustomGroupInfo group;
			group.m_strTitle = strMenuText;
				
			CMenu* pPopupMenu =	pMenu->GetSubMenu(nIndex);
			UINT nMenuItems = pPopupMenu->GetMenuItemCount();
				
			XT_MENUITEMINFO menuInfo;
			menuInfo.fMask = MIIM_ID|MIIM_SUBMENU;

			UINT i;
			for(i = 0; i < nMenuItems; i++ )
			{
				if (::GetMenuItemInfo(pPopupMenu->m_hMenu, i, TRUE, &menuInfo) &&
					menuInfo.hSubMenu &&
					pPopupMenu->GetMenuString(i, strMenuText, MF_BYPOSITION ))
				{
					// compute CRC for the menu dropdown/this popup
					crc.SetCrc32(dwCrcPopup);
					crc.Compute((LPCTSTR)strMenuText, strMenuText.GetLength() * sizeof(TCHAR));
					crc.Compute(&zero, sizeof(zero));
					if (crc.GetCrc32() == dwMenuPopupID)
					{
						return menuInfo.hSubMenu;
					}
				}
			}
		}
	}
	return NULL;
}

int CXTMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->SetCustMode.Add(this, &CXTMenuBar::SetCustMode);
	connection->GetMenuPopup = GETMENUPOPUP(this, &CXTMenuBar::GetMenuPopupByCrc);
	
	return 0;
}

void CXTMenuBar::OnDestroy() 
{
	SetCustMode(false);
	CXTCustomizeConnection::GetInstance()->Remove(this);

	CXTControlBar::OnDestroy();	
}

void CXTMenuBar::SetCustMode(bool bMode) 
{
	bool bCustomMode = m_pCustomHandler != 0;
	if (bCustomMode != bMode)
	{
		if (bMode)
		{
			m_pCustomHandler = new CXTMenuCustomHandler(this);
		}
		else
		{
			delete m_pCustomHandler;
			m_pCustomHandler = 0;
		}
	}
}
