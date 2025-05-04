// XTDirWatcher.cpp : implementation file
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
#include "XTGlobal.h"
#include "XTDirWatcher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTDirWatcher

IMPLEMENT_DYNCREATE(CXTDirWatcher, CWinThread)

CXTDirWatcher::CXTDirWatcher()
{
	m_bAutoDelete = FALSE;
    m_hWndNotify  = NULL;
    m_strPath     = _T("");
	m_lptvid      = NULL;
	m_lpsf        = NULL;
    m_hNotify     = INVALID_HANDLE_VALUE;
}

CXTDirWatcher::~CXTDirWatcher()
{
}

BOOL CXTDirWatcher::InitInstance()
{
	if (m_strPath.IsEmpty() || (m_hNotify == INVALID_HANDLE_VALUE))
	{
		// send restart notification.
		if (::IsWindow(m_hWndNotify))
		{
			::SendMessage( m_hWndNotify, XTWM_SHELL_NOTIFY,
				(WPARAM)SHN_XT_LISTRESTART, (LPARAM)0 );
		}
		TRACE0("couldn't start monitoring thread.\n");
	}

	// time-out check
	bool bContinue = true;

	// while we're still going...
	while (m_hNotify != INVALID_HANDLE_VALUE)
	{
		//while we're still timed out...
		while (bContinue)
		{
			// our call to WaitForSingleObject has a 2 second timeout to avoid thread-lock
			DWORD dw = ::WaitForSingleObject(m_hNotify, 2000);

			// if it got signalled
			if (dw == WAIT_OBJECT_0)
			{
				bContinue = true;
				
				// send update notification.
				if (::IsWindow(m_hWndNotify))
				{
					::SendMessage( m_hWndNotify, XTWM_SHELL_NOTIFY,
						(WPARAM)SHN_XT_LISTUPDATE, (LPARAM)0 );
				}
				
				// renew the handle
				if (!::FindNextChangeNotification(m_hNotify))
				{
					bContinue = false;
					break;
				}
			}

			// just keep going
			else if (dw == WAIT_TIMEOUT) {
				bContinue = true;
			}
			
			else
			{
				bContinue = false;
				break;
			}
		}
	}

	return TRUE;
}

int CXTDirWatcher::ExitInstance()
{
	// cleanup
	::FindCloseChangeNotification(m_hNotify);

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CXTDirWatcher, CWinThread)
	//{{AFX_MSG_MAP(CXTDirWatcher)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTDirWatcher message handlers

void CXTDirWatcher::SetNotifyWindow(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd)); // must be valid
	m_hWndNotify = hWnd;
}

void CXTDirWatcher::SetMonitorPath(CString strPath)
{
	if (strPath != m_strPath)
	{
		m_strPath = strPath;

		// close our old handle
		if (m_hNotify != INVALID_HANDLE_VALUE) {
			::FindCloseChangeNotification(m_hNotify);
		}
		
		m_hNotify = ::FindFirstChangeNotification(m_strPath, false, 
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_ATTRIBUTES |
			FILE_NOTIFY_CHANGE_SIZE |
			FILE_NOTIFY_CHANGE_LAST_WRITE);

		//get the handle to the event object
		if (m_hNotify == INVALID_HANDLE_VALUE) {
			TRACE0("could not create notification handle.\n");
		}
	}
}

void CXTDirWatcher::SetFolderData(XT_TVITEMDATA* lptvid, LPSHELLFOLDER lpsf)
{
	if (lptvid != 0)
	{
		m_lptvid = lptvid;
		m_lpsf   = lpsf;

		TCHAR szFolderPath[_MAX_PATH];
		if (::SHGetPathFromIDList(m_lptvid->lpi, szFolderPath))
		{
			SetMonitorPath(szFolderPath);
		}
	}
}
