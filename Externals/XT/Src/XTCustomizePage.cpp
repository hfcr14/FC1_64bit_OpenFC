// XTCustomizePage.cpp : implementation of the CXTCustToolBarPage class.
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
#include "XTResource.h"
#include "XTGlobal.h"
#include "XTButton.h"
#include "XTEditListBox.h"
#include "XTCheckListBox.h"
#include "XTTipWindow.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTCommandsListBox.h"
#include "XTCustomizeAPI.h"
#include "XTCallbacks.h"
#include "XTEditListBox.h"
#include "XTMemDC.h"
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrl.h"
#include "XTResizeRect.h"
#include "XTResizePoint.h"
#include "XTResize.h"
#include "XTResizeDialog.h"
#include "XTResizeFormView.h"
#include "XTResizePropertyPage.h"
#include "XTResizePropertySheet.h"
#include "XTAccelManager.h"
#include "XTAccelKeyEdit.h"
#include "XTBrowseEdit.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTCustomizePage.h"
#include "XTNewToolbarDlg.h"
#include "XTPriv.h"

#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTFrameImpl.h"
#include "XTMDIFrameWnd.h"
#include "XTCustomizeSheet.h"

#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

typedef CXTCustomizeConnection::ONNEWBAR ONNEWBAR;
typedef CXTCustomizeConnection::ONSHOWHIDE ONSHOWHIDE;
typedef CXTCustomizeConnection::ONBARRENAMED ONBARRENAMED;
typedef CXTCustomizeConnection::ONBARDESTROYED ONBARDESTROYED;
typedef CXTCustomizeConnection::GETCUSTOMGROUPS GETCUSTOMGROUPS;
typedef CXTCustomizeConnection::ONCMDCHANGE ONCMDCHANGE;

/////////////////////////////////////////////////////////////////////////////
// CXTCustomizeContext
/////////////////////////////////////////////////////////////////////////////

CXTCustomizeContext::CXTCustomizeContext(CFrameWnd* pFrameWnd)
: m_pFrameWnd(pFrameWnd)
{
	ASSERT_VALID(m_pFrameWnd);
}

CXTCustomizeContext::~CXTCustomizeContext()
{
}


/////////////////////////////////////////////////////////////////////////////
// CXTCustToolBarPage property page
/////////////////////////////////////////////////////////////////////////////

CXTCustToolBarPage::CXTCustToolBarPage(CXTCustomizeContext* context) 
: CXTResizePropertyPage(CXTCustToolBarPage::IDD)
, m_context(context)
, m_checkList(XT_IDB_CHECKLISTBOX_3D)
{
	//{{AFX_DATA_INIT(CXTCustToolBarPage)
	//}}AFX_DATA_INIT	

    RevertResourceContext();
}

CXTCustToolBarPage::~CXTCustToolBarPage()
{
}

void CXTCustToolBarPage::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTCustToolBarPage)
	DDX_Control(pDX, XT_IDC_LBOX_CHECK, m_checkList);
	DDX_Control(pDX, XT_IDC_BTN_RESET, m_resetButton);
	DDX_Control(pDX, XT_IDC_BTN_RENAME, m_renameButton);
	DDX_Control(pDX, XT_IDC_BTN_DELETE, m_deleteButton);
	DDX_Control(pDX, XT_IDC_BTN_NEW, m_newButton);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CXTCustToolBarPage, CXTResizePropertyPage)

BEGIN_MESSAGE_MAP(CXTCustToolBarPage, CXTResizePropertyPage)
	//{{AFX_MSG_MAP(CXTCustToolBarPage)
	ON_CLBN_CHKCHANGE(XT_IDC_LBOX_CHECK, OnChkChange)
	ON_BN_CLICKED(XT_IDC_BTN_NEW, OnNew)
	ON_LBN_SELCHANGE(XT_IDC_LBOX_CHECK, OnSelchangeChecklist)
	ON_BN_CLICKED(XT_IDC_BTN_RENAME, OnRename)
	ON_BN_CLICKED(XT_IDC_BTN_DELETE, OnDelete)
	ON_BN_CLICKED(XT_IDC_BTN_RESET, OnClear)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTCustToolBarPage::OnInitDialog() 
{
	CXTResizePropertyPage::OnInitDialog();
	//GetDlgItem(XT_IDC_LBOX_CHECK)->ModifyStyle(0, LBS_SHOWSELALWAYS);

	// check to see if create new is supported
	bool bNewSupported = CXTCustomizeConnection::GetInstance()->CreateBar;
	m_newButton.EnableWindow(bNewSupported);

	// Initializes the context
	// collect info about bars subject to customization, do initial update
	m_infos.RemoveAll();
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->GetCustomBarInfo(STORECUSTOMINFO(this, &CXTCustToolBarPage::StoreInfo));
	SetSelectItem();

	// listen to specific events
	connection->OnNewBar.Add(this, &CXTCustToolBarPage::StoreNewInfo);
	connection->OnShowHide.Add(this, &CXTCustToolBarPage::OnShowHideBar);
	connection->OnBarRenamed.Add(this, &CXTCustToolBarPage::OnBarRenamed);
	connection->OnBarDestroyed.Add(this, &CXTCustToolBarPage::OnBarDestroyed);

	SetResize(XT_IDC_LBOX_CHECK, SZ_TOP_LEFT,   SZ_BOTTOM_RIGHT);
	SetResize(XT_IDC_BTN_NEW,    SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_RENAME, SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_DELETE, SZ_TOP_RIGHT,  SZ_TOP_RIGHT);
	SetResize(XT_IDC_BTN_RESET,  SZ_TOP_RIGHT,  SZ_TOP_RIGHT);

	AutoLoadPlacement( _T( "CXTCustToolBarPage" ) );

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTCustToolBarPage::StoreInfo(CXTCustomControlBarInfo* pInfo)
{
	AddItem(pInfo);	
}

void CXTCustToolBarPage::StoreNewInfo(CXTCustomControlBarInfo* pInfo)
{
	int nIndex = AddItem(pInfo);	
	m_checkList.SetCheck(nIndex, true);  // called form OnCreate, eventually becomes visible
	SetSelectItem(nIndex);
}

void CXTCustToolBarPage::OnShowHideBar(CControlBar* pBar, bool bVisible)
{
	int nIndex = FindItem(pBar);
	if (nIndex >= 0)
	{
		// freshen checked status
		const bool bChecked = m_checkList.GetCheck(nIndex) != 0;
		if (bChecked != bVisible)
		{
			m_checkList.SetCheck(nIndex, bVisible);
			if (pBar->IsFloating())
			{
				pBar->GetParentFrame()->ShowWindow(SW_HIDE);
			}
			pBar->GetParentFrame()->PostMessage(WM_IDLEUPDATECMDUI);
		}			
	}
}

void CXTCustToolBarPage::OnBarRenamed(CControlBar* pBar, LPCTSTR pszText)
{
	int nIndex = FindItem(pBar);
	if (nIndex >= 0)
	{
		int pos = (int)m_checkList.GetItemData(nIndex);
		bool bChecked = m_checkList.GetCheck(nIndex) != 0;
		m_checkList.DeleteString(nIndex);
		m_checkList.InsertString(nIndex, pszText);				
		m_checkList.SetItemData(nIndex, pos);
		m_checkList.SetCheck(nIndex, bChecked);
	}
}

static CString GetBarText(CControlBar* pBar)
{
	CString strDescription;
	pBar->GetWindowText(strDescription);
	if (strDescription.IsEmpty())
	{
		XT_MANAGE_STATE;  // Switch to toolkit resources/module state
		strDescription.LoadString(XT_IDS_TOOLBAR);
	}	
	return strDescription;
}

// adds an item matching for given postion in the context
int CXTCustToolBarPage::AddItem(CXTCustomControlBarInfo* pInfo)
{
	int pos = m_infos.Add(*pInfo);
	CControlBar* pBar = pInfo->m_pBar;

	int nIndex = m_checkList.AddString(GetBarText(pBar));
	m_checkList.SetCheck(nIndex, pBar->IsVisible());
	m_checkList.SetItemData(nIndex, pos);

	return nIndex;
}

int CXTCustToolBarPage::FindItem(CControlBar* pBar) const
{
	const int nCount = m_checkList.GetCount();
	int nIndex; 
	for(nIndex = 0; nIndex < nCount; nIndex++)
	{
		int pos = (int)m_checkList.GetItemData(nIndex);
		if (m_infos[pos].m_pBar == pBar)
		{
			return nIndex;
		}
	}
	return -1;
}

void CXTCustToolBarPage::OnBarDestroyed(CControlBar* pBar)
{
	int nIndex = FindItem(pBar);
	if (nIndex >= 0)
	{
		m_checkList.DeleteString(nIndex);
		int iIndex = m_checkList.GetCurSel();
		SetSelectItem(iIndex);
	}
}

void CXTCustToolBarPage::OnChkChange()
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	int pos = (int)m_checkList.GetItemData(nIndex);
	const TBINFO& info = m_infos[pos];
	if (info.m_bCanShowHide) 
	{
		BOOL bShow = m_checkList.GetCheck(nIndex);
		m_context->GetFrame()->ShowControlBar(info.m_pBar, bShow, FALSE);
	}
	else
	{
		m_checkList.SetCheck(nIndex, 1);
	}
}

void CXTCustToolBarPage::OnNew() 
{
	CXTNewToolbarDlg dlg(m_context->GetFrame());
	if (dlg.DoModal() == IDOK)
	{
		CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();

		// create the new toolbar
		CXTNewCustomBarRequest request;
		request.m_nBarID = dlg.m_nNewID;
		request.m_pFrame = m_context->GetFrame();
		request.m_strTitle = dlg.m_strToolbar;
		connection->CreateBar(&request);
		CControlBar* pBar = request.m_pNewBar;
		
		// properly place it
		connection->PlaceBar(m_context->GetFrame(), pBar);
	}
}

void CXTCustToolBarPage::OnSelchangeChecklist() 
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	SetSelectItem(nIndex);
}

void CXTCustToolBarPage::OnRename() 
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	int pos = (int)m_checkList.GetItemData(nIndex);
	const TBINFO& info = m_infos[pos];
	if (info.Rename)
	{
		CXTNewToolbarDlg dlg(m_context->GetFrame());
		info.m_pBar->GetWindowText(dlg.m_strToolbar);
		if (dlg.DoModal() == IDOK)
		{
			info.Rename(dlg.m_strToolbar);
		}
		// Set focus to the check list
		m_checkList.SetFocus();
		SetSelectItem(nIndex);
	}
}

void CXTCustToolBarPage::OnDelete() 
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	int pos = (int)m_checkList.GetItemData(nIndex);
	const TBINFO& info = m_infos[pos];
	if (info.Remove)
	{
		CString strWindow;
		info.m_pBar->GetWindowText(strWindow);

		CString strMessage;
		strMessage.Format(XT_IDS_CUSTOM_CONFIRM, strWindow);

		if (AfxMessageBox(strMessage, MB_ICONWARNING|MB_OKCANCEL) == IDOK)
		{
			info.Remove();
		}
	}
}

void CXTCustToolBarPage::SetSelectItem(int iIndex/*=0*/)
{
	bool bDisable = true;
	if (iIndex >= 0)
	{
		m_checkList.SetCurSel(iIndex);
		// Get the toolbar represented by the item data.
		int nSelPos = (int)m_checkList.GetItemData(iIndex);	
		if (LB_ERR != nSelPos)
		{
			const TBINFO& info = m_infos[nSelPos];
			m_resetButton.EnableWindow(info.Reset);
			m_renameButton.EnableWindow(info.Rename);
			m_deleteButton.EnableWindow(info.Remove);
			bDisable = false;
		}
	}
	if (bDisable)
	{
		m_resetButton.EnableWindow(false);
		m_renameButton.EnableWindow(false);
		m_deleteButton.EnableWindow(false);
	}
}


void CXTCustToolBarPage::OnClear()
{
	int nIndex = m_checkList.GetCurSel();
	if (nIndex == LB_ERR)
		return;

	int pos = (int)m_checkList.GetItemData(nIndex);
	const TBINFO& info = m_infos[pos];
	if (info.Reset)
	{
		info.Reset();
	}
}

void CXTCustToolBarPage::OnDestroy() 
{
	CXTResizePropertyPage::OnDestroy();
	
	// unregister listeners
	CXTCustomizeConnection::GetInstance()->Remove(this);
}

////////////////////////////////////////////////////////////////////////////
// CXTCustCommandsPage property page
/////////////////////////////////////////////////////////////////////////////

#pragma warning(disable: 4355) // OK here
CXTCustCommandsPage::CXTCustCommandsPage(CXTCustomizeContext* context)
: CXTResizePropertyPage(CXTCustCommandsPage::IDD)
, m_context(context)
, m_listCommands(this)
{
	//{{AFX_DATA_INIT(CXTCustCommandsPage)
	//}}AFX_DATA_INIT

	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->OnCmdRemoved.Add(this, &CXTCustCommandsPage::OnCmdRemoved);

    RevertResourceContext();
}

CXTCustCommandsPage::~CXTCustCommandsPage()
{
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->Remove(this);
}

void CXTCustCommandsPage::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTCustCommandsPage)
	DDX_Control(pDX, XT_IDC_LBOX_COMMANDS, m_listCommands);
	DDX_Control(pDX, XT_IDC_LBOX_CATEGORIES, m_listGroups);
	DDX_Control(pDX, XT_IDC_BTN_DESCRIPTION, m_btnDescription);
	DDX_Control(pDX, XT_IDC_TXT_SELECTED, m_txtSelected);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CXTCustCommandsPage, CXTResizePropertyPage)

BEGIN_MESSAGE_MAP(CXTCustCommandsPage, CXTResizePropertyPage)
	//{{AFX_MSG_MAP(CXTCustCommandsPage)
	ON_WM_CTLCOLOR()
	ON_LBN_SELCHANGE(XT_IDC_LBOX_CATEGORIES, OnSelchangeListCategories)
	ON_BN_CLICKED(XT_IDC_BTN_DESCRIPTION, OnBtnDescription)
	ON_LBN_SELCHANGE(XT_IDC_LBOX_COMMANDS, OnSelchangeListCommands)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int __cdecl SortGroups(const void * pv1, const void * pv2)
{
	CXTCustomGroupInfo* group1 = *(CXTCustomGroupInfo**)pv1;
	CXTCustomGroupInfo* group2 = *(CXTCustomGroupInfo**)pv2;
	int delta = group1->m_nSortPriority - group2->m_nSortPriority;
	if (delta == 0)
	{
		// sort by title if priority is the same
		delta = group1->m_strTitle.CompareNoCase(group2->m_strTitle);
	}
	return delta;
}

BOOL CXTCustCommandsPage::OnInitDialog() 
{
	CXTResizePropertyPage::OnInitDialog();

	CXTCustomizeConnection* connection=CXTCustomizeConnection::GetInstance();

	// query for customization groups
	m_groups.RemoveAll();
	connection->GetCustomGroups(m_groups);
	if (m_groups.GetSize())
	{
		qsort(m_groups.GetData(), m_groups.GetSize(), sizeof(CXTCustomGroupInfo*), 
			  &SortGroups);
	}
	int pos = 0;
	for(pos = 0; pos < m_groups.GetSize(); pos++)
	{
		CXTCustomGroupInfo* pInfo = m_groups[pos];
		int nIndex = m_listGroups.AddString(pInfo->m_strTitle);
		m_listGroups.SetItemData(nIndex, pos);
	}

	// query for command to icon mappings
	m_iconMap.RemoveAll();
	connection->GetIcons(&m_iconMap);

	m_listGroups.SetCurSel(0);
	OnSelchangeListCategories();

	SetResize(XT_IDC_LBOX_CATEGORIES, SZ_TOP_LEFT,     SZ_BOTTOM_CENTER);
	SetResize(XT_IDC_TXT_COMMANDS,    SZ_TOP_CENTER,   SZ_TOP_CENTER);
	SetResize(XT_IDC_LBOX_COMMANDS,   SZ_TOP_CENTER,   SZ_BOTTOM_RIGHT);
	SetResize(XT_IDC_TXT_SELECTED,    SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);
	SetResize(XT_IDC_BTN_DESCRIPTION, SZ_BOTTOM_LEFT,  SZ_BOTTOM_LEFT);

	AutoLoadPlacement( _T( "CXTCustCommandsPage" ) );

	m_tipWindow.Create(this);
	m_tipWindow.SetMargins(CSize(5,5));
	m_tipWindow.SetLineSpace(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTCustCommandsPage::OnCmdRemoved(UINT nCmdID)
{
	// synch up the arrays, update  command list 
	int i,k;
	bool found = false;
	for (i = 0; i < m_groups.GetSize() && !found; i++)
	{
		CXTCustomGroupInfo* pGroup = m_groups[i];
		for (k = 0; k < pGroup->m_items.GetSize() && !found; k++)
		{
			CXTCustomGroupItemInfo* pItem = m_groups[i]->m_items[k];
			found = pItem->m_nCmdID == nCmdID;
			if (found)
			{
				m_groups[i]->DeleteItem(k);
				UpdateListContents();
			}
		}
	}
}

bool CXTCustCommandsPage::GetCommandIcon(UINT nCmdID, HICON& hIcon, HICON& hDisabledIcon, HICON& hHotIcon)
{
	if (m_iconMap.Lookup(nCmdID, hIcon, hDisabledIcon, hHotIcon))
	{
		hIcon = ::CopyIcon(hIcon);
		if (hDisabledIcon)
		{
			hDisabledIcon = ::CopyIcon(hDisabledIcon);
		}
		if (hHotIcon)
		{
			hHotIcon = ::CopyIcon(hHotIcon);
		}
	}
	else
	{
		hIcon = hDisabledIcon = 0;
	}
	return hIcon != 0;
}

UINT CXTCustCommandsPage::GetItemCommand(int pos)
{
	int nGroupIndex = m_listGroups.GetCurSel();
	int nGroupPos = m_listGroups.GetItemData(nGroupIndex);
	const CXTCustomGroupItemInfo* pItem = m_groups[nGroupPos]->m_items[pos];
	return pItem->m_nCmdID;
}

void CXTCustCommandsPage::DragNDrop(int pos)
{
	int nGroupIndex = m_listGroups.GetCurSel();
	int nGroupPos = m_listGroups.GetItemData(nGroupIndex);	
	CXTCustomGroupItemInfo* pItem = m_groups[nGroupPos]->m_items[pos];
	HICON hIcon, hDisabledIcon, hHotIcon;
	if (GetCommandIcon(pItem->m_nCmdID, hIcon, hDisabledIcon, hHotIcon))
	{
		pItem->m_icon = hIcon;
		pItem->m_disabledIcon = hDisabledIcon;
		pItem->m_hotIcon = hHotIcon;
	}
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->ResetTracking();
	connection->DragNDrop(this, false, pItem);
}

HBRUSH CXTCustCommandsPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CXTResizePropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd == &m_txtSelected)
	{
		if( m_btnDescription.IsWindowEnabled()) {
			pDC->SetTextColor(xtAfxData.clrBtnText);
		} else {
			pDC->SetTextColor(xtAfxData.clr3DShadow);
		}
	}
	
	return hbr;
}

void CXTCustCommandsPage::UpdateListContents()
{
    m_listCommands.ResetContent();
    int nIndex = m_listGroups.GetCurSel();
    if (nIndex != LB_ERR)
    {
        CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
        int pos = m_listGroups.GetItemData(nIndex);
        const CXTCustomGroupInfo* group = m_groups[pos];
		int posItem;
        for (posItem = 0; posItem < group->m_items.GetSize(); posItem++)
        {
            const CXTCustomGroupItemInfo* pItem = group->m_items[posItem];
            if (pItem->m_type != CXTCustomGroupItemInfo::Control)
            {
                // controls do not get added since we do not
                // support drag-n-drop of controls between toolbars
                UINT nCmdID = pItem->m_nCmdID;
                CXTString strTitle = group->m_items[posItem]->m_strTitle;

                // make sure we strip off any extra text if a newline character is found.
                int iFound = strTitle.Find( _T("\n"), 0 );
                if ( iFound != -1 ) {
                    strTitle = strTitle.Left( iFound );
                }

                if (connection->IsCustCmd(nCmdID))
                {               
                    int nIndex = m_listCommands.AddString(strTitle);
                    m_listCommands.SetItemData(nIndex, posItem);
                }
            }
        }
    }

    m_listCommands.SetCurSel(0);
}

void CXTCustCommandsPage::SetEnableControls(BOOL bEnable)
{
	m_btnDescription.EnableWindow(bEnable);
	m_txtSelected.InvalidateRect(NULL);
}

void CXTCustCommandsPage::OnSelchangeListCategories() 
{
	UpdateListContents();
	m_listCommands.SetCurSel(-1);
	SetEnableControls(FALSE);
}

void CXTCustCommandsPage::OnBtnDescription() 
{
	CRect rect;
	m_btnDescription.GetWindowRect(&rect);
	m_tipWindow.ShowTipWindow(CPoint(rect.left-50, rect.bottom+5),
		TWS_XT_THICKBORDER|TWS_XT_DROPSHADOW);
}

void CXTCustCommandsPage::OnSelchangeListCommands() 
{
	int nGroupIndex = m_listGroups.GetCurSel();
	int nGroupPos = (int)m_listGroups.GetItemData(nGroupIndex);
	int nIndex = m_listCommands.GetCurSel();
	int pos = int(m_listCommands.GetItemData(nIndex));
	const CXTCustomGroupItemInfo* info = m_groups[nGroupPos]->m_items[pos];

	CString strDescrip;
	
	if (info->m_nCmdID)
	{
		strDescrip.LoadString(info->m_nCmdID);

		if (strDescrip.IsEmpty()) {
			strDescrip.Format(XT_IDS_NOHELPTOPIC, info->m_nCmdID);
		}

		CString strTitle;
		m_listCommands.GetText(nIndex, strTitle);

		int iTrimPos = strDescrip.Find(_T('\n'));
		if (iTrimPos >= 0) {
			strDescrip  = strDescrip.Left(iTrimPos);
		}

		if (!strDescrip.IsEmpty()) {
			m_tipWindow.SetTipText(strTitle, strDescrip);
		}
	}

	SetEnableControls(!strDescrip.IsEmpty());
}

/////////////////////////////////////////////////////////////////////////////
// CXTCustAccelerators property page
/////////////////////////////////////////////////////////////////////////////

CXTCustAccelerators::CXTCustAccelerators(CXTCustomizeContext* context) 
: CXTResizePropertyPage(CXTCustAccelerators::IDD)
, m_context(context)
{
	//{{AFX_DATA_INIT(CXTCustAccelerators)
	m_strTitle  = _T("");
	m_strDesc   = _T("");
	m_iCategory = 0;
	//}}AFX_DATA_INIT

    RevertResourceContext();
}

CXTCustAccelerators::~CXTCustAccelerators()
{
	if ( CXTAccelManager::Get().IsInitialized() )
	{
		CXTAccelManager::Get().UpdateWindowAccelerator();
	}
}

void CXTCustAccelerators::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTCustAccelerators)
	DDX_Control(pDX, XT_IDC_EDIT_SHORTCUTKEY, m_editShortcutKey);
	DDX_Control(pDX, XT_IDC_LBOX_KEYS, m_lboxKeys);
	DDX_Control(pDX, XT_IDC_LBOX_ACCEL_COMMANDS, m_lboxCommands);
	DDX_Control(pDX, XT_IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Text(pDX, XT_IDC_TXT_FRAMETITLE, m_strTitle);
	DDX_Text(pDX, XT_IDC_TXT_DESC_OVERVIEW, m_strDesc);
	DDX_CBIndex(pDX, XT_IDC_COMBO_CATEGORY, m_iCategory);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CXTCustAccelerators, CXTResizePropertyPage)

BEGIN_MESSAGE_MAP(CXTCustAccelerators, CXTResizePropertyPage)
	//{{AFX_MSG_MAP(CXTCustAccelerators)
	ON_CBN_SELCHANGE(XT_IDC_COMBO_CATEGORY, OnSelchangeCategory)
	ON_LBN_SELCHANGE(XT_IDC_LBOX_ACCEL_COMMANDS, OnSelchangeCommands)
	ON_LBN_SELCHANGE(XT_IDC_LBOX_KEYS, OnSelchangeCurKeys)
	ON_BN_CLICKED(XT_IDC_BTN_ASSIGN, OnAssign)
	ON_BN_CLICKED(XT_IDC_BTN_ACCEL_RESET, OnResetAll)
	ON_BN_CLICKED(XT_IDC_BTN_REMOVE, OnRemove)
	ON_EN_CHANGE(XT_IDC_EDIT_SHORTCUTKEY, OnChangeShortcutKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTCustAccelerators::OnInitDialog() 
{
	CXTResizePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetResize( XT_IDC_COMBO_CATEGORY,      SZ_TOP_LEFT,      SZ_TOP_CENTER    );
    SetResize( XT_IDC_LBOX_ACCEL_COMMANDS, SZ_TOP_LEFT,      SZ_BOTTOM_CENTER );
    SetResize( XT_IDC_TXT_DESC_TITLE,      SZ_BOTTOM_LEFT,   SZ_BOTTOM_LEFT   );
    SetResize( XT_IDC_TXT_DESC_OVERVIEW,   SZ_BOTTOM_LEFT,   SZ_BOTTOM_RIGHT  );
    SetResize( XT_IDC_TXT_FRAMETITLE,      SZ_TOP_CENTER,    SZ_TOP_RIGHT     );
    SetResize( XT_IDC_TXT_ASSIGNMENTS,     SZ_TOP_CENTER,    SZ_TOP_RIGHT     );
    SetResize( XT_IDC_LBOX_KEYS,           SZ_TOP_CENTER,    SZ_BOTTOM_RIGHT  );
    SetResize( XT_IDC_TXT_NEWKEY,          SZ_BOTTOM_CENTER, SZ_BOTTOM_RIGHT  );
    SetResize( XT_IDC_EDIT_SHORTCUTKEY,    SZ_BOTTOM_CENTER, SZ_BOTTOM_RIGHT  );
    SetResize( XT_IDC_BTN_ASSIGN,          SZ_TOP_RIGHT,     SZ_TOP_RIGHT     );
    SetResize( XT_IDC_BTN_REMOVE,          SZ_TOP_RIGHT,     SZ_TOP_RIGHT     );
    SetResize( XT_IDC_BTN_ACCEL_RESET,     SZ_TOP_RIGHT,     SZ_TOP_RIGHT     );

	AutoLoadPlacement( _T( "CXTCustToolBarPage" ) );
	
	// ASSERT if the manager is not initialized..
	ASSERT( CXTAccelManager::Get().IsInitialized( ) );
	
	// load accelerators.
	CXTAccelManager::Get().InitAccelerators();
	CXTAccelManager::Get().GetCategoryList( m_comboCategory );

    m_comboCategory.SetFont( &xtAfxData.font );
	m_comboCategory.SetCurSel( 0 );
	OnSelchangeCategory( );

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTCustAccelerators::ReloadList()
{
	int iIndex = m_comboCategory.GetCurSel( );
	if( iIndex == LB_ERR )
    {
        return;
    }

    HMENU hMenu = ( HMENU )m_comboCategory.GetItemData( iIndex );
    if ( !hMenu || !::IsMenu( hMenu ) )
    {
        return;
    }

	m_lboxCommands.ResetContent();

    int iItem;
    for ( iItem = 0; iItem < ::GetMenuItemCount( hMenu ); ++iItem )
    {
        TCHAR szItemName[256];
        
        XT_MENUITEMINFO info;
        info.fMask      = MIIM_SUBMENU | MIIM_ID | MIIM_TYPE | MIIM_STATE;
        info.dwTypeData = szItemName;
        info.cch        = _countof( szItemName );
        
        ::GetMenuItemInfo( hMenu, iItem, TRUE, &info );
        
        if ( ( info.fType & MFT_SEPARATOR ) == 0 && info.wID != 0 )
        {
            if ( !info.hSubMenu && !::IsMenu( info.hSubMenu ) )
            {
                if ( ( info.wID < ID_FILE_MRU_FIRST ) || ( info.wID > ID_FILE_MRU_LAST ) )
                {
                    CXTString strItemName = szItemName;
                    CXTAccelManager::Get().MakeDisplayName( strItemName );
                    
                    int iIndex = m_lboxCommands.AddString( strItemName );
                    m_lboxCommands.SetItemData( iIndex, info.wID );
                }
            }
        }
    }

    CString strCategory;
    m_comboCategory.GetLBText(iIndex, strCategory);

    POSITION pos;
    for (pos = CXTAccelManager::Get().m_ExtraItems.GetHeadPosition(); pos; CXTAccelManager::Get().m_ExtraItems.GetNext(pos))
    {
        XT_EXTRA_ACCELITEM* pItem = CXTAccelManager::Get().m_ExtraItems.GetAt(pos);
        if (pItem != NULL)
        {
            if (strCategory.CompareNoCase(pItem->szCategory) == 0)
            {
                CXTString strItemName = pItem->szName;
                CXTAccelManager::Get().MakeDisplayName( strItemName );
                
                int iIndex = m_lboxCommands.AddString( strItemName );
                m_lboxCommands.SetItemData( iIndex, pItem->nCommandID );
            }
        }
    }

	if ( m_lboxCommands.GetCount( ) )
    {
		m_lboxCommands.SetCurSel( 0 );
    }
}

void CXTCustAccelerators::OnChangeShortcutKey()
{
	EnableControls( );
}

void CXTCustAccelerators::OnSelchangeCurKeys()
{
	EnableControls( );
}

void CXTCustAccelerators::OnSelchangeCategory()
{
	UpdateData( );
	ReloadList( );
	OnSelchangeCommands( );
	EnableControls( );
}

void CXTCustAccelerators::OnSelchangeCommands() 
{
	m_lboxKeys.ResetContent( );

	int iIndex = m_lboxCommands.GetCurSel( );
	if ( iIndex == -1 )
	{
		return;
	}

	UINT nCommandID = m_lboxCommands.GetItemData( iIndex );

	int nPos = 0;
	CString szName;
	BYTE cVirt;
	WORD wKey;

	while ( CXTAccelManager::Get().GetKeyAssignmentInfo( nPos, nCommandID, szName, cVirt, wKey ) )
	{
		int nItem = m_lboxKeys.AddString( szName );
		WORD wVirt = cVirt;
		m_lboxKeys.SetItemData( nItem, MAKELONG( wKey, wVirt ) );
	}

	m_strDesc.LoadString( nCommandID );

	int nLoc = m_strDesc.FindOneOf( _T( "\t\n" ) );
	
	if ( nLoc != -1 )
	{
		m_strDesc = m_strDesc.Left( nLoc );
	}

	UpdateData( FALSE );
	EnableControls( );
}

void CXTCustAccelerators::OnAssign() 
{
	int iIndex = m_lboxCommands.GetCurSel( );
	if ( iIndex == -1 )
	{
		return;
	}

	UINT nCommandID = m_lboxCommands.GetItemData( iIndex );

	WORD nVirtualKey;
	BOOL bCtrl, bAlt, bShift;
	if ( m_editShortcutKey.GetKeyInfo( nVirtualKey, bCtrl, bAlt, bShift ) )
	{
		BYTE fVirt = FVIRTKEY|FNOINVERT;

		if ( bCtrl ) {
			fVirt |= FCONTROL;
		}
		if ( bAlt ) {
			fVirt |= FALT;
		}
		if ( bShift ) {
			fVirt |= FSHIFT;
		}

		UINT nAssignedCommandID;
		if ( CXTAccelManager::Get().GetKeyAssignment( fVirt, nVirtualKey, nAssignedCommandID ) )
		{
			CString strCategory, strCommand;
			CXTAccelManager::Get().LookupCommandStringItem( nAssignedCommandID, strCommand );

			CString strBuffer;
			strBuffer.Format( XT_IDS_REASSIGN, strCommand );

			if ( AfxMessageBox( strBuffer, MB_ICONWARNING|MB_YESNO ) != IDYES )
			{
				return;
			}

			CXTAccelManager::Get().DeleteKeyAssignment( fVirt, nVirtualKey );
		}

		if ( CXTAccelManager::Get().AddKeyAssignment( nCommandID, fVirt, nVirtualKey ) )
		{
			OnSelchangeCommands( );
			m_editShortcutKey.SetWindowText( _T( "" ) );
			EnableControls( );
		}
	}
}

void CXTCustAccelerators::EnableControls()
{
	CString strText;
	m_editShortcutKey.GetWindowText( strText );
	
	CWnd* pWnd = GetDlgItem( XT_IDC_BTN_ASSIGN );
	if ( pWnd )
	{
		pWnd->EnableWindow( !strText.IsEmpty( ) && m_editShortcutKey.IsKeyDefined( ) );
	}

	int iSel = m_lboxKeys.GetCurSel( );
	
	pWnd = GetDlgItem( XT_IDC_BTN_REMOVE );
	if ( pWnd )
	{
		pWnd->EnableWindow( iSel != LB_ERR );
	}
}

void CXTCustAccelerators::OnResetAll() 
{
	if ( AfxMessageBox( XT_IDS_AREYOUSURE, MB_ICONWARNING|MB_YESNO ) == IDYES )
	{
		CXTAccelManager::Get().LoadDefaultAccelerator( );
		OnSelchangeCategory( );
		EnableControls( );
	}
}

void CXTCustAccelerators::OnRemove() 
{
	int iIndex = m_lboxKeys.GetCurSel( );
	if ( iIndex != -1 )
	{
		DWORD nData = m_lboxKeys.GetItemData( iIndex );

		WORD wKey  = LOWORD( nData );
		BYTE cVirt = LOBYTE( HIWORD( nData ) );

		if ( CXTAccelManager::Get().DeleteKeyAssignment( cVirt, wKey ) )
		{
			m_lboxKeys.DeleteString( iIndex );
			EnableControls( );
		}
	}
}

BOOL CXTCustAccelerators::OnSetActive() 
{
	UpdateData( FALSE );
	ReloadList( );
	EnableControls( );

	return CXTResizePropertyPage::OnSetActive();
}

/////////////////////////////////////////////////////////////////////////////
// CXTCustTools property page
/////////////////////////////////////////////////////////////////////////////

const TCHAR szNULL[] = _T( "" );

CXTCustTools::CXTCustTools(CXTCustomizeContext* context)
: CXTResizePropertyPage(CXTCustTools::IDD)
, m_context(context)
, m_pOldData(0)
{
	//{{AFX_DATA_INIT(CXTCustTools)
	m_strToolsDir = _T( "" );
	m_strToolsArg = _T( "" );
	m_strToolsCmd = _T( "" );
	//}}AFX_DATA_INIT	

    RevertResourceContext();
}

CXTCustTools::~CXTCustTools()
{
    FreeToolsList( m_arTools );

	if ( CXTToolsManager::Get().IsInitialized() )
	{
		CXTToolsManager::Get().Save( );
	}
}

void CXTCustTools::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTCustTools)
	DDX_Control(pDX, XT_IDC_TXT_TOOLSDIR, m_txtToolsDir);
	DDX_Control(pDX, XT_IDC_TXT_TOOLSARG, m_txtToolsArg);
	DDX_Control(pDX, XT_IDC_TXT_TOOLSCMD, m_txtToolsCmd);
	DDX_Control(pDX, XT_IDC_EDIT_TOOLSDIR, m_editToolsDir);
	DDX_Control(pDX, XT_IDC_EDIT_TOOLSARG, m_editToolsArg);
	DDX_Control(pDX, XT_IDC_EDIT_TOOLSCMD, m_editToolsCmd);
	DDX_Control(pDX, XT_IDC_LBOX_TOOLSLIST, m_lboxToolsList);
	DDX_Text(pDX, XT_IDC_EDIT_TOOLSDIR, m_strToolsDir);
	DDX_Text(pDX, XT_IDC_EDIT_TOOLSARG, m_strToolsArg);
	DDX_Text(pDX, XT_IDC_EDIT_TOOLSCMD, m_strToolsCmd);
    //}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CXTCustTools, CXTResizePropertyPage)

BEGIN_MESSAGE_MAP(CXTCustTools, CXTResizePropertyPage)
	//{{AFX_MSG_MAP(CXTCustTools)
	ON_LBN_SELCHANGE(XT_IDC_LBOX_TOOLSLIST, OnSelchangeEditList)
	ON_EN_CHANGE(XT_IDC_EDIT_TOOLSCMD, OnChangeToolsCmd)
	ON_EN_CHANGE(XT_IDC_EDIT_TOOLSARG, OnChangeToolsArg)
	ON_EN_CHANGE(XT_IDC_EDIT_TOOLSDIR, OnChangeToolsDir)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_LBN_XT_LABELEDITEND(XT_IDC_LBOX_TOOLSLIST, OnLabelEditEnd)
	ON_LBN_XT_LABELEDITCANCEL(XT_IDC_LBOX_TOOLSLIST, OnLabelEditCancel)
	ON_LBN_XT_NEWITEM(XT_IDC_LBOX_TOOLSLIST, OnNewItem)
	ON_LBN_XT_PREDELETEITEM(XT_IDC_LBOX_TOOLSLIST, OnPreDeleteItem)
	ON_LBN_XT_DELETEITEM(XT_IDC_LBOX_TOOLSLIST, OnDeleteItem)
	ON_LBN_XT_MOVEITEMUP(XT_IDC_LBOX_TOOLSLIST, OnMoveItemUp)
	ON_LBN_XT_MOVEITEMDOWN(XT_IDC_LBOX_TOOLSLIST, OnMoveItemDown)
END_MESSAGE_MAP()

BOOL CXTCustTools::OnInitDialog() 
{
	CXTResizePropertyPage::OnInitDialog();

	// Set control resizing.
    SetResize( XT_IDC_LBOX_TOOLSLIST, SZ_TOP_LEFT,    SZ_BOTTOM_RIGHT );
    SetResize( XT_IDC_TXT_TOOLSCMD,   SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT  );
    SetResize( XT_IDC_EDIT_TOOLSCMD,  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT );
    SetResize( XT_IDC_TXT_TOOLSARG,   SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT  );
    SetResize( XT_IDC_EDIT_TOOLSARG,  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT );
    SetResize( XT_IDC_TXT_TOOLSDIR,   SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT  );
    SetResize( XT_IDC_EDIT_TOOLSDIR,  SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT );

	m_editToolsCmd.SetBrowseStyle(BES_XT_CHOOSEFILE);
	
    CString strBuffer;
    strBuffer.LoadString( XT_IDS_BROWSE );
    m_editToolsCmd.SetDlgTitle( strBuffer );

    strBuffer.LoadString( XT_IDS_BROWSE_FILTER );
	m_editToolsCmd.SetDlgFilter( strBuffer );

    strBuffer.LoadString( XT_IDS_MENU_CONTENTS );
	m_lboxToolsList.SetListEditStyle( strBuffer, LBS_XT_DEFAULT);

	// ASSERT if the manager is not initialized..
	ASSERT( CXTToolsManager::Get().IsInitialized( ) );
	
    int iArgPopup = CXTToolsManager::Get().GetArgPopupMenu( );
    int iDirPopup = CXTToolsManager::Get().GetDirPopupMenu( );
	
	if ( iArgPopup != 0 ) {
		m_editToolsArg.SetBrowseStyle( BES_XT_POPUPMENU, iArgPopup, 0 );
	}
	else {
		m_editToolsArg.SetBrowseStyle( BES_XT_CHOOSEFILE );
	}

	if ( iDirPopup != 0 ) {
		m_editToolsDir.SetBrowseStyle( BES_XT_POPUPMENU, iDirPopup, 0 );
	}
	else {
		m_editToolsDir.SetBrowseStyle( BES_XT_CHOOSEDIR );
	}

	CXTToolsManager::Get().GetToolsList( m_lboxToolsList );
	m_lboxToolsList.SetCurSel( 0 );
	OnSelchangeEditList( );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXTCustTools::EnableControls(bool bEnable)
{
	m_txtToolsDir.EnableWindow( bEnable );
	m_txtToolsArg.EnableWindow( bEnable );
	m_txtToolsCmd.EnableWindow( bEnable );
	m_editToolsDir.EnableWindow( bEnable );
	m_editToolsArg.EnableWindow( bEnable );
	m_editToolsCmd.EnableWindow( bEnable );
}

void CXTCustTools::OnSelchangeEditList() 
{
	int iIndex = m_lboxToolsList.GetCurSel( );
	if ( iIndex == LB_ERR ) 
    {
	    m_strToolsDir = _T( "" );
	    m_strToolsArg = _T( "" );
	    m_strToolsCmd = _T( "" );
        
        EnableControls( false );
	}
    else if ( iIndex >= m_lboxToolsList.GetCount( ) )
    {
        return;
    }
    else
    {
        XT_TOOL* pTool = ( XT_TOOL* )m_lboxToolsList.GetItemData( iIndex );
        if ( pTool == NULL )
        {
            return;
        }
        
        m_strToolsDir = pTool->szDir;
        m_strToolsArg = pTool->szArg;
        m_strToolsCmd = pTool->szCmd;
     
        EnableControls( true );
   }

    UpdateData( FALSE );
}

void CXTCustTools::UpdateToolData()
{
    UpdateData();
    
    // get the index of the currently selected item.
	int iItem = m_lboxToolsList.GetCurSel( );
    if ( iItem == LB_ERR )
	{
        return;
	}

    // get a pointer to the item data for the selection.
	XT_TOOL* pTool = ( XT_TOOL* )m_lboxToolsList.GetItemData( iItem );
    if ( pTool != NULL )
    {
        _tcscpy( pTool->szCmd, m_strToolsCmd );
        _tcscpy( pTool->szArg, m_strToolsArg );
        _tcscpy( pTool->szDir, m_strToolsDir );

        CXTToolsManager::Get().HasChanged( true );
    }
}

void CXTCustTools::OnChangeToolsCmd() 
{
	UpdateToolData( );
}

void CXTCustTools::OnChangeToolsArg() 
{
	UpdateToolData( );
}

void CXTCustTools::OnChangeToolsDir() 
{
	UpdateToolData( );
}

void CXTCustTools::UpdateToolsItem(LPCTSTR lpszText, int iWhich, bool bAppend)
{
	int iIndex = m_lboxToolsList.GetCurSel( );
	XT_TOOL* pTool = ( XT_TOOL* )m_lboxToolsList.GetItemData( iIndex );
	if ( pTool != NULL )
	{
        UpdateData();
        
		switch ( iWhich )
		{
		case XT_TOOLCMD:
			{
				if ( bAppend ) {
					m_strToolsCmd += lpszText;
				}
				else {
					m_strToolsCmd = lpszText;
				}
                
                UpdateData( FALSE );

				int iSel = m_strToolsCmd.GetLength( );
				m_editToolsCmd.SetSel( iSel,iSel );

				_tcscpy( pTool->szCmd, m_strToolsCmd );
			}
			break;
		case XT_TOOLARG:
			{
				if ( bAppend ) {
					m_strToolsArg += lpszText;
				}
				else {
					m_strToolsArg = lpszText;
				}

                UpdateData( FALSE );

				int iSel = m_strToolsArg.GetLength( );
				m_editToolsArg.SetSel( iSel,iSel );

				_tcscpy( pTool->szArg, m_strToolsArg );
			}
			break;
		case XT_TOOLDIR:
			{
				if ( bAppend ) {
					m_strToolsDir += lpszText;
				}
				else {
					m_strToolsDir = lpszText;
				}
                
                UpdateData( FALSE );

				int iSel = m_strToolsDir.GetLength( );
				m_editToolsDir.SetSel( iSel,iSel );

				_tcscpy( pTool->szDir, m_strToolsDir );
			}
			break;
		}
	}
}

void CXTCustTools::OnLabelEditEnd()
{
	int iIndex = m_lboxToolsList.GetCurrentIndex( );
	XT_TOOL* pTool = ( XT_TOOL* )m_lboxToolsList.GetItemData( iIndex );
	if ( pTool != NULL )
	{
		CString strText;
		m_lboxToolsList.GetText( iIndex, strText );

		_tcscpy( pTool->szTitle, strText );
		CXTToolsManager::Get().HasChanged( true );

		m_lboxToolsList.SetCurSel( iIndex );
	}
}

void CXTCustTools::OnLabelEditCancel()
{
	m_lboxToolsList.SetCurSel(
		m_lboxToolsList.GetCurrentIndex( ) );
}

bool CXTCustTools::ValidateTools()
{
	if ( m_editToolsDir.IsBrowsing() ||
		 m_editToolsArg.IsBrowsing() ||
		 m_editToolsCmd.IsBrowsing() )
	{
		return true;
	}

    int i;
    for ( i = 0; i < m_lboxToolsList.GetCount( ); ++i )
    {
        XT_TOOL* pTool = ( XT_TOOL* )m_lboxToolsList.GetItemData( i );
        if ( pTool != NULL )
        {
			if ( _taccess( pTool->szCmd, 0 ) == -1 )
			{
				// if 'http://' or 'ftp://' is found in the string, assume URL.
				CXTString strBuffer = pTool->szCmd;
				if ( ( strBuffer.Find( _T( "http://"), 0 ) != -1 ) ||
					 ( strBuffer.Find( _T( "ftp://"),  0 ) != -1 ) )
				{
                    continue;
                }

                AfxMessageBox( XT_IDS_PATH_INVALID );
                
                m_lboxToolsList.SetFocus();
                m_lboxToolsList.SetCurSel( i );
                
                OnSelchangeEditList( );
                
                return false;
			}
		}
	}

	return true;
}

void CXTCustTools::OnNewItem()
{
	int iItem = m_lboxToolsList.GetCurrentIndex( );
	if ( iItem != -1 )
	{
		CString strTitle;
		m_lboxToolsList.GetText( iItem, strTitle );
		
		XT_TOOL* pTool = new XT_TOOL;
        _tcscpy( pTool->szTitle, strTitle );
        _tcscpy( pTool->szDir, szNULL );
        _tcscpy( pTool->szArg, szNULL );
        _tcscpy( pTool->szCmd, szNULL );

		pTool->uCmdID = 0;

		m_lboxToolsList.SetItemData( iItem, ( DWORD_PTR )pTool );
		m_lboxToolsList.SetCurSel( iItem );

	    m_strToolsDir = pTool->szDir;
	    m_strToolsArg = pTool->szArg;
	    m_strToolsCmd = pTool->szCmd;

        m_arTools.AddHead( pTool );
        
        EnableControls( true );
        UpdateData( FALSE );
	}
}

void CXTCustTools::OnPreDeleteItem()
{
	m_pOldData = 0;

	int iIndex = m_lboxToolsList.GetCurSel( );
    if ( iIndex == LB_ERR )
    {
        return;
    }

	// save a pointer to the item data before the item is removed.
	m_pOldData = ( XT_TOOL* )m_lboxToolsList.GetItemData( iIndex );
}

void CXTCustTools::OnDeleteItem()
{
	int iIndex = m_lboxToolsList.GetCurSel( );
    if ( iIndex == LB_ERR )
    {
        iIndex = 0;
    }

	// if the deleted item had a data pointer...
	if ( m_pOldData )
	{
		UINT uCmdID = m_pOldData->uCmdID;
		bool bRemoved = false;

		// if the data was new, remove it from our list and free memory.
		POSITION pos = m_arTools.Find( m_pOldData );
		if ( pos )
		{
			m_arTools.RemoveAt( pos );
			SAFE_DELETE( m_pOldData );
			bRemoved = true;
		}
		
		// if the data was existing, remove it from the tools manager and
		// rebuild tools list.
		else
		{
			
			if (CXTToolsManager::Get().RemoveTool( m_pOldData ))
			{
				bRemoved = true;
			}
			CXTToolsManager::Get().GetToolsList( m_lboxToolsList );
		}

		m_pOldData = 0;
		if (bRemoved && uCmdID)
		{
			CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
			connection->OnCmdRemoved(uCmdID);
		}			
	}

    if ( m_lboxToolsList.GetCount( ) != 0 )
    {
	    m_lboxToolsList.SetCurSel( iIndex );
	    OnSelchangeEditList();
    }
}

void CXTCustTools::FreeToolsList(CXTToolsList& arTools, bool bAddTool/*=false*/)
{
    while ( !arTools.IsEmpty( ) )
    {
        XT_TOOL* pTool = arTools.RemoveHead( );

        if ( bAddTool ) {
            CXTToolsManager::Get().AddTool( pTool );
        }
        
        SAFE_DELETE( pTool );
    }
}

void CXTCustTools::UpdateToolsList()
{
    int i;
    for ( i = 0; i < m_lboxToolsList.GetCount( ); ++i )
    {
        XT_TOOL* pOldTool = ( XT_TOOL* )m_lboxToolsList.GetItemData( i );
        if ( pOldTool == NULL ) {
            continue;
        }
        
        XT_TOOL* pNewTool = new XT_TOOL;
        
        _tcscpy( pNewTool->szArg,   pOldTool->szArg   );
        _tcscpy( pNewTool->szCmd,   pOldTool->szCmd   );
        _tcscpy( pNewTool->szDir,   pOldTool->szDir   );
        _tcscpy( pNewTool->szTitle, pOldTool->szTitle );

		pNewTool->uCmdID = pOldTool->uCmdID;
        
        m_arTools.AddTail( pNewTool );
        m_lboxToolsList.SetItemData( i, (DWORD_PTR)pNewTool );
        
        POSITION pos = m_arTools.Find( pOldTool );
        if ( pos )
        {
            m_arTools.RemoveAt( pos );
            SAFE_DELETE( pOldTool );
        }
    }
    
    CXTToolsManager::Get().RemoveAllTools( );
    
    POSITION pos;
    for ( pos = m_arTools.GetTailPosition( ); pos; m_arTools.GetPrev( pos ) )
    {
        XT_TOOL* pNewTool = m_arTools.GetAt( pos );
        CXTToolsManager::Get().AddTool( pNewTool );
    }
}

void CXTCustTools::OnMoveItemUp()
{

}

void CXTCustTools::OnMoveItemDown()
{

}

BOOL CXTCustTools::OnKillActive() 
{
	if ( ValidateTools( ) == false )
	{
		return false;
	}
	
	return CXTResizePropertyPage::OnKillActive();
}

void CXTCustTools::OnDestroy() 
{
    UpdateToolsList( );
	CXTResizePropertyPage::OnDestroy();
}

/////////////////////////////////////////////////////////////////////////////
// CXTCustOptions property page
/////////////////////////////////////////////////////////////////////////////

CXTCustOptions::CXTCustOptions(CXTCustomizeContext* context)
: CXTResizePropertyPage(CXTCustOptions::IDD)
, m_context(context)
{
	// ASSERT if the manager is not initialized..
	ASSERT( CXTOptionsManager::Get().IsInitialized( ) );
	
	XT_OPTIONS& options = CXTOptionsManager::Get().GetOptions( );

	//{{AFX_DATA_INIT(CXTCustOptions)
    m_bMenuShadows        = options.bMenuShadows;
    m_bMenuRecentCommands = options.bMenuRecentCommands;
    m_bToolBarScreenTips  = options.bToolBarScreenTips;
    m_bShowFullAfterDelay = options.bShowFullAfterDelay;
    m_bToolBarAccelTips   = options.bToolBarAccelTips;
    m_bToolBarVisualize   = options.bToolBarVisualize;
	m_nAnimationType      = options.nAnimationType;
	//}}AFX_DATA_INIT

    RevertResourceContext();
}

CXTCustOptions::~CXTCustOptions()
{
	if ( CXTOptionsManager::Get().IsInitialized( ) )
	{
        XT_OPTIONS options;
        
        options.bMenuShadows        = ( m_bMenuShadows        == TRUE );
        options.bMenuRecentCommands = ( m_bMenuRecentCommands == TRUE );
        options.bToolBarScreenTips  = ( m_bToolBarScreenTips  == TRUE );
        options.bShowFullAfterDelay = ( m_bShowFullAfterDelay == TRUE );
        options.bToolBarAccelTips   = ( m_bToolBarAccelTips   == TRUE );
        options.bToolBarVisualize   = ( m_bToolBarVisualize   == TRUE );
		options.nAnimationType    = m_nAnimationType;
        
		CXTOptionsManager::Get().SetOptions( options );
		CXTOptionsManager::Get().Save( );		
	}
}

void CXTCustOptions::DoDataExchange(CDataExchange* pDX)
{
	CXTResizePropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXTCustOptions)
	DDX_Control(pDX, XT_IDC_COMBO_ANIMATION, m_comboAnimationType);
    DDX_Control(pDX, XT_IDC_CHK_SHOWFULL, m_chkShowFull);
    DDX_Control(pDX, XT_IDC_CHK_SHOWSHORTCUTKEYS, m_chkToolBarAccelTips);
	DDX_Check(pDX, XT_IDC_CHK_MENUSHADOWS, m_bMenuShadows);
	DDX_Check(pDX, XT_IDC_CHK_RECENTMENU, m_bMenuRecentCommands);
	DDX_Check(pDX, XT_IDC_CHK_SCREENTIPS, m_bToolBarScreenTips);
	DDX_Check(pDX, XT_IDC_CHK_SHOWFULL, m_bShowFullAfterDelay);
	DDX_Check(pDX, XT_IDC_CHK_SHOWSHORTCUTKEYS, m_bToolBarAccelTips);
	DDX_Check(pDX, XT_IDC_CHK_VISUALIZE, m_bToolBarVisualize);
	DDX_CBIndex(pDX, XT_IDC_COMBO_ANIMATION, m_nAnimationType);
	//}}AFX_DATA_MAP
}

IMPLEMENT_DYNAMIC(CXTCustOptions, CXTResizePropertyPage)

BEGIN_MESSAGE_MAP(CXTCustOptions, CXTResizePropertyPage)
	//{{AFX_MSG_MAP(CXTCustOptions)
	ON_BN_CLICKED(XT_IDC_CHK_MENUSHADOWS, OnMenuShadows)
	ON_BN_CLICKED(XT_IDC_CHK_RECENTMENU, OnRecentMenu)
	ON_BN_CLICKED(XT_IDC_CHK_SHOWFULL, OnShowFull)
	ON_BN_CLICKED(XT_IDC_BTN_RESET, OnBtnReset)
	ON_BN_CLICKED(XT_IDC_CHK_SCREENTIPS, OnScreenTips)
	ON_BN_CLICKED(XT_IDC_CHK_SHOWSHORTCUTKEYS, OnShowShortcutKeys)
	ON_BN_CLICKED(XT_IDC_CHK_VISUALIZE, OnVisualize)
	ON_CBN_SELCHANGE(XT_IDC_COMBO_ANIMATION, OnAnimation)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CXTCustOptions::OnMenuShadows() 
{
	UpdateData();
}
void CXTCustOptions::OnAnimation()
{
	UpdateData();
}

void CXTCustOptions::OnRecentMenu() 
{
	UpdateData();
    
    m_chkShowFull.EnableWindow(
        m_bMenuRecentCommands );
}

void CXTCustOptions::OnShowFull() 
{
	UpdateData();
}

void CXTCustOptions::OnBtnReset() 
{
	CXTOptionsManager::Get().Reset();
}

void CXTCustOptions::OnScreenTips() 
{
	UpdateData();
    
    m_chkToolBarAccelTips.EnableWindow(
        CXTToolsManager::Get().IsInitialized( ) && m_bToolBarScreenTips );
}

void CXTCustOptions::OnShowShortcutKeys() 
{
	UpdateData();
}

void CXTCustOptions::OnVisualize() 
{
	UpdateData();
}

void CXTCustOptions::AddString( UINT nIDResource )
{
	CString strBuffer;
	strBuffer.LoadString( nIDResource );
	m_comboAnimationType.AddString( strBuffer );
}

BOOL CXTCustOptions::OnInitDialog() 
{
	CXTResizePropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
    SetResize( XT_IDC_SEP_PERSONALIZED, SZ_TOP_LEFT, SZ_TOP_RIGHT );
    SetResize( XT_IDC_SEP_OTHER,        SZ_TOP_LEFT, SZ_TOP_RIGHT );
    SetResize( XT_IDC_BTN_RESET,        SZ_TOP_LEFT, SZ_TOP_RIGHT );
    SetResize( XT_IDC_COMBO_ANIMATION,  SZ_TOP_LEFT, SZ_TOP_RIGHT );

    AutoLoadPlacement( _T( "CXTCustOptions" ) );

    m_chkToolBarAccelTips.EnableWindow(
        CXTToolsManager::Get().IsInitialized( ) && m_bToolBarScreenTips );

    m_chkShowFull.EnableWindow(
        m_bMenuRecentCommands );

	// Animation is not supported for Windows 95 & NT.
	m_comboAnimationType.EnableWindow(
		!xtAfxData.bWin95 && !xtAfxData.bWinNT );

	// Add strings to animation combo box.
	AddString( XT_IDS_ANIM_SYSDEF );
	AddString( XT_IDS_ANIM_RANDOM );
	AddString( XT_IDS_ANIM_UNFOLD );
	AddString( XT_IDS_ANIM_SLIDE );
	AddString( XT_IDS_ANIM_FADE );
	AddString( XT_IDS_ANIM_NONE );

	// Ensure combo selection is made.
	UpdateData( FALSE );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
