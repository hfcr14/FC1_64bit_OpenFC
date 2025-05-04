// XTDirWatcher.h : header file
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

#if !defined(__XTDIRWATCHER_H__)
#define __XTDIRWATCHER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Summary: CXTDirWatcher is a CWinThread derived class.  This class monitors
// a specified directory for file change notifications.  When a file change
// has been made a notification message is sent to a predefined notification 
// window set by the user.  Example of notifications are file or directory 
// creation, file or directory deletion, name change, size change, attribute 
// change and so on.
//
// To use CXTDirWatcher, instanciate a new thread using AfxBeginThread, 
// add a CXTDirWatcher* m_pDirThread to your class header file:
// <pre>
//	// Start the directory monitoring thread.
//	m_pDirThread = (CXTDirWatcher*)AfxBeginThread(
//		RUNTIME_CLASS(CXTDirWatcher), THREAD_PRIORITY_IDLE);
//
//	// Set the notification window handle.
//	if (m_pDirThread != NULL) {
//		m_pDirThread->SetNotifyWindow(GetSafeHandle());
//	}
// </pre>
// You can then specify what directory you wish to monitory by calling SetMonitorPath:
// <pre>
//	if (m_pDirThread != NULL) {
//		m_pDirThread->SetFolderData(lptvid, lpsf);
//	}
// </pre>
// You can add a message handler to the window receiving messages and handle your 
// change event notifications:
// </pre>
//BEGIN_MESSAGE_MAP(CNotifyWnd, CWnd)
//	//{{AFX_MSG_MAP(CNotifyWnd)
//	//}}AFX_MSG_MAP
//	ON_MESSAGE(XTWM_SHELL_NOTIFY, OnUpdateShell)
//END_MESSAGE_MAP()
//
//LRESULT CNotifyWnd::OnUpdateShell(WPARAM wParam, LPARAM lParam)
//{
//	switch (wParam)
//	{
//	case SHN_XT_LISTUPDATE:
//		{
//			// TODO: Handle update notification.
//			break;
//		}
//	default:
//		break;
//	}
//	
//	return 0;
//}
// </pre>
// When you are finished using the CXTDirWatcher thread, you will need to free the
// memory associated with the object.  To do so you can use the convient SAFE_DELETE
// macro like so:
// <pre>
// SAFE_DELETE(m_pDirThread);
// </pre>
// The CXTDirWatcher class is also used internally by the CXTShellListCtrl and 
// CXTShellListView classes to handle refreshing the list when a file or directory
// change occurs.
class _XT_EXT_CLASS CXTDirWatcher : public CWinThread
{
	DECLARE_DYNCREATE(CXTDirWatcher)

public:

    // Summary: Constructs a CXTDirWatcher object 
	CXTDirWatcher();

    // Summary: Destroys a CXTDirWatcher object, handles cleanup and de-allocation 
	virtual ~CXTDirWatcher();

protected:

	HWND			m_hWndNotify;	// Handle of window to notify.
	HANDLE			m_hNotify;		// Event object handle
	CString			m_strPath;		// Path to monitor
	XT_TVITEMDATA*	m_lptvid;		// Pointer to tree view item data.
	LPSHELLFOLDER	m_lpsf;			// Pointer to the parent shell folder.

public:

	// Input:	hWnd - Handle of the window to receive update notifications.
    // Summary: Call this member function to set the notification handle for the thread.
	void SetNotifyWindow(HWND hWnd);
	
	// Input:	strPath - Full path of directory to monitor.
    // Summary: Call this member function to monitor the directory specified by strPath.
	void SetMonitorPath(CString strPath);

	// Summary: Call this member function to retrieve the full path to the directory that 
	//          is currently monitored.
	// Returns: A CString object.
	CString GetMonitorPath() const;

	// Input:	lptvid - Pointer to tree view item data.
	//			lpsf - Pointer to the parent shell folder.
    // Summary: This member function is called by the CXTShellListCtrl to perform directory 
	//          monitoring using tree view item data defined by the associated CXTShellTreeCtrl.
	void SetFolderData(XT_TVITEMDATA* lptvid, LPSHELLFOLDER lpsf);

	// Summary: This member function is called by the CXTShellListCtrl to used to return the 
	//          XT_TVITEMDATA data that was set for the directory that is currently monitored.
	// Returns: A pointer to a XT_TVITEMDATA structure.
	XT_TVITEMDATA* GetItemData() const;

	// Summary: This member function is called by the CXTShellListCtrl to used to return the 
	//          LPSHELLFOLDER pointer that was set for the directory that is currently monitored.
	// Returns: A points to a LPSHELLFOLDER object.
	LPSHELLFOLDER GetShellFolder() const;

	//{{AFX_VIRTUAL(CXTDirWatcher)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTDirWatcher)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

AFX_INLINE XT_TVITEMDATA* CXTDirWatcher::GetItemData() const {
	return m_lptvid;
}
AFX_INLINE LPSHELLFOLDER CXTDirWatcher::GetShellFolder() const {
	return m_lpsf;
}
AFX_INLINE CString CXTDirWatcher::GetMonitorPath() const {
	return m_strPath;
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTDIRWATCHER_H__)
