// XTToolCmdUI.cpp : implementation of the CXTToolCmdUI class.
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
#include "XTGlobal.h"
#include "XTDelegate.h"
#include "XTWndHook.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTToolCmdUI.h"
#include "XTCustomizeAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static void EnableButton(CXTToolBar* pToolBar, int nIndex, bool bOn, bool bIgnore)
{
	UINT nNewStyle = pToolBar->GetButtonStyle(nIndex) & ~TBBS_DISABLED;
	if (!bOn)
	{
		// whole dropdown buttons are not supposed to be disabled
		// since they send notifications that are handled
		// by the toolbar
		if (!bIgnore && nNewStyle & BTNS_WHOLEDROPDOWN)
		{
			bOn = true;
		}
		if (!bOn)
		{
			nNewStyle |= TBBS_DISABLED;
			// WINBUG: If a button is currently pressed and then is disabled
			// COMCTL32.DLL does not unpress the button, even after the mouse
			// button goes up!	We work around this bug by forcing TBBS_PRESSED
			// off when a button is disabled.
			nNewStyle &= ~TBBS_PRESSED;
		}
	}
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(nIndex, nNewStyle);
}

/////////////////////////////////////////////////////////////////////////////
// CXTToolCmdUI implementation
/////////////////////////////////////////////////////////////////////////////
void CXTToolCmdUI::Enable(BOOL bOn)
{
	m_bEnableChanged = TRUE;
	CXTToolBar* pToolBar = (CXTToolBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CXTToolBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	// if customize mode is on and UI shall be enabled, then all buttons shall be displayed as enabled
	// for the customizable toolbars
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();	
	if (connection->IsCustMode() && connection->IsEnableCmdUI())
	{
		if (pToolBar->GetCustomBar())
		{
			EnableButton(pToolBar, m_nIndex, true, false);
			return;			
		}
	}

	// menubar dropdowns shall be disabled if it i not possible
	// to connect to their menu handle
	TBBUTTON button;
	if (pToolBar->GetButton(m_nIndex, &button) && !(button.fsStyle & TBSTYLE_SEP))
	{
		if (XT_DROPDOWNBUTTON* pDropDown = pToolBar->FindDropDownButton(button.idCommand))
		{
			if (pDropDown->bMenuBarPopup)
			{
				CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
				HMENU hPopup = connection->GetMenuPopup(pDropDown->dwMenuBarPopupID);
				EnableButton(pToolBar, m_nIndex, hPopup != 0, true);
				return;
			}
		}
	}

	EnableButton(pToolBar, m_nIndex, bOn != 0, false);
}

void CXTToolCmdUI::SetCheck(int nCheck)
{
	ASSERT(nCheck >= 0 && nCheck <= 2); // 0=>off, 1=>on, 2=>indeterminate
	CXTToolBar* pToolBar = (CXTToolBar*)m_pOther;
	ASSERT(pToolBar != NULL);
	ASSERT_KINDOF(CXTToolBar, pToolBar);
	ASSERT(m_nIndex < m_nIndexMax);

	UINT nNewStyle = pToolBar->GetButtonStyle(m_nIndex) &
				~(TBBS_CHECKED | TBBS_INDETERMINATE);
	if (nCheck == 1)
		nNewStyle |= TBBS_CHECKED;
	else if (nCheck == 2)
		nNewStyle |= TBBS_INDETERMINATE;
	ASSERT(!(nNewStyle & TBBS_SEPARATOR));
	pToolBar->SetButtonStyle(m_nIndex, nNewStyle | TBBS_CHECKBOX);
}

void CXTToolCmdUI::SetText(LPCTSTR)
{
	// ignore it
}
