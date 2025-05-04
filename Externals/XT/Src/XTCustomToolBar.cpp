// XTCustomToolbar.cpp : implementation file
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
#include "XTDefines.h"
#include "XTResource.h"
#include "XTGlobal.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTToolBarCtrl.h"
#include "XTCustomToolbar.h"
#include "XTCustomizeAPI.h"
#include "XTRegistryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CXTCustomToolBar, CXTToolBar)

BEGIN_MESSAGE_MAP(CXTCustomToolBar, CXTToolBar)
	//{{AFX_MSG_MAP(CXTCustomToolBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTCustomToolBar

CXTCustomToolBar::CXTCustomToolBar()
{
	SetCustomBar(true);
}

CXTCustomToolBar::~CXTCustomToolBar()
{
}

typedef CXTCustomControlBarInfo::RENAME RENAME;
typedef CXTCustomControlBarInfo::REMOVE REMOVE;
void CXTCustomToolBar::GetCustomBarInfo(CXTCustomControlBarInfo* pInfo)
{
	CXTToolBar::GetCustomBarInfo(pInfo);
	pInfo->m_bDynamic = true;
	pInfo->Rename = RENAME(this, &CXTCustomToolBar::RenameCustomBar);	
	pInfo->Remove = REMOVE(this, &CXTCustomToolBar::DeleteCustomBar);
}

void CXTCustomToolBar::RenameCustomBar(const CString& strNewName)
{
	if (m_strTitle != strNewName)
	{
		SetWindowText(strNewName);
		if (m_dwStyle & CBRS_FLOATING)
		{
			// update the frame as well
			GetParentFrame()->SetWindowText(strNewName);
		}
		m_strTitle = strNewName;
	}
}

void CXTCustomToolBar::DeleteCustomBar()
{	
	CString strSubKey = m_strSubKey;
	CString strValueName = m_strValueName;

	// destroy this window
	DestroyWindow();

	// cleanup the registry to prevent this toolbar from reloading
	// if there is an associated registry subkey
	if (strSubKey.GetLength())
	{
		CXTRegistryManager registry;
		registry.DeleteValue(_T("Settings"), strValueName);
	}

}

void CXTCustomToolBar::PostNcDestroy()
{
	// cannot use 'delete this' because of the bug in VC7
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->ScheduleGC(this);
}

int CXTCustomToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_images.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_COLOR24|ILC_MASK, 11, 1))
	{
		TRACE0("Failed to create image list\n");
		return -1;
	}
	SetImageList(m_images);	
	
	// check to see if customization mode is currently on,
	// attach properly if so
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	if (connection->IsCustMode())
	{
		SetCustMode(true);
		CXTCustomControlBarInfo info;
		info.m_pBar = this;
		GetCustomBarInfo(&info);
		connection->OnNewBar(&info);
	}
	
	return 0;
}

bool CXTCustomToolBar::CreateDisabledImageList()
{
	if (!m_disabledImages.Create(m_sizeImage.cx, m_sizeImage.cy, ILC_COLOR24|ILC_MASK, 11, 1))
	{
		TRACE0("Failed to create image list\n");
		return false;
	}
	SetImageList( m_disabledImages.GetSafeHandle(), FALSE )	;
	return true;
}

void CXTCustomToolBar::ReportCustomGroups(CXTCustomGroups& /*groups*/)
{
	// custom toolbars report nothing
}