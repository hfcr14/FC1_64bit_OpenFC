// XTNewToolbarDlg.cpp : implementation of the CXTNewToolbarDlg class.
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
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTNewToolbarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTNewToolbarDlg dialog
/////////////////////////////////////////////////////////////////////////////
CXTNewToolbarDlg::CXTNewToolbarDlg(CFrameWnd* pFrameWnd/*=NULL*/)
	: CDialog(CXTNewToolbarDlg::IDD, NULL)
{
	//{{AFX_DATA_INIT(CXTNewToolbarDlg)
	m_strToolbar = _T("");
	//}}AFX_DATA_INIT
	m_strExistingName = _T("");

	m_pFrameWnd = pFrameWnd;
	ASSERT_VALID(m_pFrameWnd);
	m_nNewID = AFX_IDW_CONTROLBAR_FIRST;
	RevertResourceContext();
}

void CXTNewToolbarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTNewToolbarDlg)
	DDX_Control(pDX, XT_IDC_EDIT_TOOLBAR, m_editToolbar);
	DDX_Text(pDX, XT_IDC_EDIT_TOOLBAR, m_strToolbar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXTNewToolbarDlg, CDialog)
	//{{AFX_MSG_MAP(CXTNewToolbarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTNewToolbarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Create the suggested new toolbar name.
	if (m_strToolbar.IsEmpty())
		SetSuggestedName();

	m_strExistingName = m_strToolbar;

	// Set focus to the edit control and select
	// its contents.
	m_editToolbar.SetFocus();
	m_editToolbar.SetSel(0,-1);

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTNewToolbarDlg::OnOK() 
{
	UpdateData();

	// If no text was entered, alert user.
	if (m_strToolbar.IsEmpty()) {
		AfxMessageBox(XT_IDS_ERROR_BLANK, MB_ICONSTOP);
		return;
	}

	// Loop through all of the existing control bars to find 
	// an available ID to use.
	POSITION pos = m_pFrameWnd->m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CControlBar* pBar = (CControlBar*)m_pFrameWnd->m_listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);

		// We found a control bar with the same ID as m_nNewID, increment
		// m_nNewID and reset the position back to the head.
		if (pBar->GetDlgCtrlID() == m_nNewID)
		{
			m_nNewID++;
			pos = m_pFrameWnd->m_listControlBars.GetHeadPosition();

			// If m_nNewID is greater than the maximum number of allowed
			// custom commands, alert the user and abort.
			if (m_nNewID >= AFX_IDW_CONTROLBAR_LAST)
			{
				AfxMessageBox(XT_IDS_ERROR_LIMIT, MB_ICONSTOP);
				CDialog::OnCancel();
				return;
			}
			continue;
		}

		// Now check to see if the title for the toolbar has already
		// been used, if so, alert the user and return.
		CString strWindowText;
		pBar->GetWindowText(strWindowText);
		if (strWindowText.Compare(m_strToolbar) == 0)
		{
			if ( m_strExistingName != m_strToolbar )
			{
				CString strName;
				strName.Format(XT_IDS_ERROR_EXISTS, m_strToolbar);
				AfxMessageBox(strName, MB_ICONSTOP);
			}
			return;
		}
	}

	CDialog::OnOK(); // success!
}

void CXTNewToolbarDlg::SetSuggestedName()
{
	int iMaxCount = (AFX_IDW_CONTROLBAR_LAST-AFX_IDW_CONTROLBAR_FIRST);
	int iNewID = 1;
	CString strCustom;
	strCustom.Format(XT_IDS_CUSTOM_BAR, iNewID);

	POSITION pos = m_pFrameWnd->m_listControlBars.GetHeadPosition();
	while (pos != NULL)
	{
		CControlBar* pBar = (CControlBar*)m_pFrameWnd->m_listControlBars.GetNext(pos);
		ASSERT(pBar != NULL);
		
		CString strWindowText;
		pBar->GetWindowText(strWindowText);
		if (strWindowText.Compare(strCustom) == 0)
		{
			strCustom.Format(XT_IDS_CUSTOM_BAR, iNewID++);
			pos = m_pFrameWnd->m_listControlBars.GetHeadPosition();
		}

		if (iNewID >= iMaxCount)
		{
			AfxMessageBox(XT_IDS_ERROR_LIMIT, MB_ICONSTOP);
			CDialog::OnCancel();
			return;
		}
	}

	m_strToolbar = strCustom;
	UpdateData(FALSE);
}

