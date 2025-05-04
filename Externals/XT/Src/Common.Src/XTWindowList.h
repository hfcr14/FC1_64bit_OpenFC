// XTWindowList.h : header file
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

#if !defined(_XTWINDOWLIST_H__)
#define _XTWINDOWLIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _XTP_INCLUDE_CONTROLS
#define CXTMDIFrameWnd CMDIFrameWnd
#endif

class CXTMDIFrameWnd;

/////////////////////////////////////////////////////////////////////////////
// CXTWindowList dialog
class CXTWindowList : public CXTResizeDialog
{
// Construction
public:
	
	// Input:	pMDIFrameWnd - Pointer to applications main window.
	// Summary:	Constructs a CXTWindowList object.
	CXTWindowList(CXTMDIFrameWnd* pMDIFrameWnd);   

	//{{AFX_DATA(CXTWindowList)
	enum { IDD = XT_IDD_WINDOWLIST };
	CButton  m_btnMinimize;
	CButton  m_btnCascade;
	CButton  m_btnOk;
	CButton  m_btnTileVert;
	CButton  m_btnTileHorz;
	CButton  m_btnSave;
	CButton  m_btnClose;
	CButton  m_btnActivate;
	//}}AFX_DATA

	HWND						m_hWndMDIClient; // Handle to MDI client area.
	HICON						m_hIcon;		 // Resize dialogs handle
	CXTMDIFrameWnd*				m_pMDIFrameWnd;	 // Pointer to applications MDI frame.
	CXTNoFlickerWnd <CListBox>	m_lboxWindows;   // Flicker free list box to display MDI children.

	//{{AFX_VIRTUAL(CXTWindowList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	
	// Input:	hIcon - Icon handle to set.
	// Summary:	Call this member function to set the dialogs icon.
	void SetWindowIcon(HICON hIcon) {
        m_hIcon = hIcon;
    }

protected:
	
	// Input:	pMDIChildWnd - Pointer to a MDI child window.
	// Summary:	Call this member funciton to add a MDI child to the window list.
	void InsertMDIChild(CMDIChildWnd* pMDIChildWnd);
	
	// Input:	hWnd - Handle to a MDI child window.
	// Returns:	A CString object representing the windows text.
	// Summary:	Call this member function to return the MDI child windows.
	CString GetChildWndText(HWND hWnd) const;
	
	// Summary:	Call this member function to toggle restore / minimize state for selected MDI children.
	void RestoreAndMinimize();
	
	// Summary:	Call this member function to enable / disable button controls for the window list dialog
    //          depending on selection.
	void SetEnableButtons();
	
	// Summary:	Call this member function to populate the window list with currently open MDI children.
	void FillMDIWindowList();
	
	// Summary:	Call this member function to select the active MDI child window.
	void SelectMDIActive();

	//{{AFX_MSG(CXTWindowList)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLBox();
	afx_msg void OnBtnActivate();
	afx_msg void OnBtnSave();
	afx_msg void OnBtnClosewindows();
	afx_msg void OnBtnTilehorz();
	afx_msg void OnBtnTilevert();
	afx_msg void OnBtnCascade();
	afx_msg void OnBtnMinimize();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_XTWINDOWLIST_H__)
