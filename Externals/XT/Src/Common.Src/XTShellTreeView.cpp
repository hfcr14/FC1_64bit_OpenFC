// XTShellTreeView.cpp : implementation file
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
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTFunctions.h"
#include "XTShellPidl.h"
#include "XTDropSource.h"
#include "XTShellSettings.h"
#include "XTTreeView.h"
#include "XTShellTreeView.h"
#include "XTComboBoxEx.h"
#include "XTMemDC.h"
#include "XTHelpers.h"
#include "XTOSVersionInfo.h"

#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTShellTreeView

CXTShellTreeView::CXTShellTreeView()
: m_bTunneling(false)
{
	m_bContextMenu = TRUE;
	m_pComboBox    = NULL;
	m_uFlags       = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;

	if (m_shSettings.ShowAllFiles() && !m_shSettings.ShowSysFiles()) {
		m_uFlags |= SHCONTF_INCLUDEHIDDEN;
	}
}

CXTShellTreeView::~CXTShellTreeView()
{

}

IMPLEMENT_DYNCREATE(CXTShellTreeView, CXTTreeView)

BEGIN_MESSAGE_MAP(CXTShellTreeView, CXTTreeView)
	//{{AFX_MSG_MAP(CXTShellTreeView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBeginDrag)
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginRDrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTShellTreeView message handlers

HTREEITEM CXTShellTreeView::InsertDesktopItem()
{
	HTREEITEM hItem = TVI_ROOT;

	LPMALLOC lpMalloc = NULL;
	if (SUCCEEDED(::SHGetMalloc(&lpMalloc)))
	{
		// Get ShellFolder Pidl
		LPITEMIDLIST pidlDesktop = NULL;
		::SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &pidlDesktop );
		
		// insert the desktop.
		if ( pidlDesktop )
		{
			SHFILEINFO fileInfo;
			::ZeroMemory( &fileInfo, sizeof( fileInfo ) );
			
			::SHGetFileInfo( ( LPCTSTR )pidlDesktop, NULL, &fileInfo, sizeof( fileInfo ),
				SHGFI_PIDL | SHGFI_ATTRIBUTES | SHGFI_DISPLAYNAME );
			
			TV_ITEM  tvi;
			tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			
			// Allocate memory for ITEMDATA struct
			XT_TVITEMDATA* lptvid = (XT_TVITEMDATA*)lpMalloc->Alloc(sizeof(XT_TVITEMDATA));
			if (lptvid != NULL )
			{
				GetNormalAndSelectedIcons( pidlDesktop, &tvi );
				
				// Now, make a copy of the ITEMIDLIST and store the parent folders SF.
				lptvid->lpi        = DuplicateItem(lpMalloc, pidlDesktop);
				lptvid->lpsfParent = NULL;
				lptvid->lpifq      = ConcatPidls(NULL, pidlDesktop);
				
				TCHAR szBuff[256];
				_tcscpy( szBuff, fileInfo.szDisplayName );
				
				tvi.lParam     = (LPARAM)lptvid;
				tvi.pszText    = szBuff;
				tvi.cchTextMax = MAX_PATH;
				
				// Populate the TreeVeiw Insert Struct
				// The item is the one filled above.
				// Insert it after the last item inserted at this level.
				// And indicate this is a root entry.
				TV_INSERTSTRUCT tvins;
				tvins.item         = tvi;
				tvins.hInsertAfter = hItem;
				tvins.hParent      = hItem;
				
				// Add the item to the tree
				hItem = GetTreeCtrl().InsertItem(&tvins);
			}
			
			if ( lpMalloc && pidlDesktop ) {
				lpMalloc->Free( pidlDesktop );
			}
		}
		if ( lpMalloc ) {
			lpMalloc->Release();
		}
	}

	return hItem;
}

void CXTShellTreeView::PopulateTreeView() 
{
    // Get a pointer to the desktop folder.
    LPSHELLFOLDER lpsf = NULL;
    if ( SUCCEEDED( ::SHGetDesktopFolder( &lpsf ) ) )
    {
		// Initialize the tree view to be empty.
		GetTreeCtrl().DeleteAllItems( );

		HTREEITEM hItemDesktop = InsertDesktopItem( );

		// Fill in the tree view from the root.
		InitTreeViewItems( lpsf, NULL, hItemDesktop );
		
		// Release the folder pointer.
		lpsf->Release( );
	
		TV_SORTCB tvscb;
		tvscb.hParent     = hItemDesktop;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = _xtAfxTreeViewCompareProc;
		
		// Sort the items in the tree view
		GetTreeCtrl().SortChildrenCB( &tvscb );
    
		HTREEITEM hItemRoot = GetTreeCtrl().GetRootItem( );
		GetTreeCtrl().Expand( hItemRoot, TVE_EXPAND );

		if ( hItemDesktop != TVI_ROOT )
		{
			GetTreeCtrl().SetRedraw( FALSE );

			HTREEITEM hItemChild = GetTreeCtrl().GetChildItem( hItemDesktop );
			GetTreeCtrl().Expand( hItemChild, TVE_EXPAND );

			if ( ( ::GetWindowLong( GetTreeCtrl().m_hWnd, GWL_STYLE ) & TVS_SINGLEEXPAND ) == 0 )
			{
				GetTreeCtrl().Expand( hItemChild, TVE_EXPAND );
			}
			
			GetTreeCtrl().SetRedraw( TRUE );
		}
		else
		{
			GetTreeCtrl().Select( hItemRoot, TVGN_CARET );
		}

		GetTreeCtrl().SetFocus( );
	}
}

void CXTShellTreeView::SetAttributes(HTREEITEM hItem, DWORD dwAttributes)
{
    MapShellFlagsToItemAttributes(&GetTreeCtrl(), hItem, dwAttributes, TVIS_CUT);
}

BOOL CXTShellTreeView::InitTreeViewItems(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq, HTREEITEM     hParent)
{
	CWaitCursor wait; // show wait cursor.

    // Allocate a shell memory object. 
    LPMALLOC lpMalloc = NULL;
    if (SUCCEEDED(::SHGetMalloc(&lpMalloc)))
    {
       // Get the IEnumIDList object for the given folder.
 	   LPENUMIDLIST lpe = NULL;
       if (SUCCEEDED(lpsf->EnumObjects(::GetParent(m_hWnd), m_uFlags, &lpe)))
        {
			ULONG        ulFetched = 0;
			HTREEITEM    hPrev     = NULL;
		    LPITEMIDLIST lpi       = NULL;

            // Enumerate through the list of folder and non-folder objects.
            while (lpe->Next(1, &lpi, &ulFetched) == S_OK)
            {
                // Create a fully qualified path to the current item
                // the SH* shell api's take a fully qualified path pidl,
                // (see GetIcon above where I call SHGetFileInfo) whereas the
                // interface methods take a relative path pidl.
                ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_DISPLAYATTRMASK | SFGAO_REMOVABLE;
				
                // Determine what type of object we have.
                lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);
				
                if (ulAttrs & (SFGAO_HASSUBFOLDER | SFGAO_FOLDER))
                {
					// We need this next if statement so that we don't add things like
					// the MSN to our tree.  MSN is not a folder, but according to the
					// shell it has subfolders.
					if (ulAttrs & SFGAO_FOLDER)
					{
						TV_ITEM  tvi;
						tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
						
						if (ulAttrs & SFGAO_HASSUBFOLDER)
						{
							//This item has sub-folders, so let's put the + in the TreeView.
							//The first time the user clicks on the item, we'll populate the
							//sub-folders.
							tvi.cChildren = 1;
							tvi.mask |= TVIF_CHILDREN;
						}
						
						// Allocate memory for ITEMDATA struct
						TCHAR szBuff[256];
						XT_TVITEMDATA* lptvid = (XT_TVITEMDATA*)lpMalloc->Alloc(sizeof(XT_TVITEMDATA));
						if (lptvid == NULL || GetName(lpsf, lpi, SHGDN_NORMAL, szBuff) == FALSE)
						{
							if (lpe) {
								lpe->Release();
							}
							if (lpi && lpMalloc) {
								lpMalloc->Free(lpi);
							}
							if (lpMalloc) {
								lpMalloc->Release();
							}
							return FALSE;
						}
						
						LPITEMIDLIST lpifqThisItem = ConcatPidls(lpifq, lpi);
						GetNormalAndSelectedIcons(lpifqThisItem, &tvi);
						
						// Now, make a copy of the ITEMIDLIST and store the parent folders SF.
						lptvid->lpi        = DuplicateItem(lpMalloc, lpi);
						lptvid->lpsfParent = lpsf;
						lptvid->lpifq      = ConcatPidls(lpifq, lpi);
						lpsf->AddRef();
						
						tvi.lParam     = (LPARAM)lptvid;
						tvi.pszText    = szBuff;
						tvi.cchTextMax = MAX_PATH;
						
						// Populate the TreeVeiw Insert Struct
						// The item is the one filled above.
						// Insert it after the last item inserted at this level.
						// And indicate this is a root entry.
						TV_INSERTSTRUCT tvins;
						tvins.item         = tvi;
						tvins.hInsertAfter = hPrev;
						tvins.hParent      = hParent;
						
						// Add the item to the tree
						hPrev = GetTreeCtrl().InsertItem(&tvins);
						SetAttributes(hPrev, ulAttrs);

						// Free this items task allocator.
						if (lpifqThisItem && lpMalloc) {
							lpMalloc->Free(lpifqThisItem);
						}
					}
				}
				
                // Free the pidl that the shell gave us.
				if (lpi && lpMalloc) {
					lpMalloc->Free(lpi);
					lpi = 0;
				}
            }
			if (lpi && lpMalloc) {
				lpMalloc->Free(lpi);
			}
			if (lpe) {
				lpe->Release();
			}
        }
		if (lpMalloc) {
			lpMalloc->Release();
		}

		return TRUE;
    }

	return FALSE;
}

void CXTShellTreeView::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
	// Note that we don't check the return value here because if GetIcon()
	// fails, then we're in big trouble...
	lptvitem->iImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	
	lptvitem->iSelectedImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
}

void CXTShellTreeView::OnFolderExpanding(NM_TREEVIEW* pNMTreeView) 
{
    if (!(pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE))
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA* lptvid = (XT_TVITEMDATA*)pNMTreeView->itemNew.lParam;
		if (lptvid != NULL && lptvid->lpsfParent != NULL )
		{
			LPSHELLFOLDER lpsf = NULL;
			if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0, IID_IShellFolder,(LPVOID *)&lpsf))) {
				InitTreeViewItems(lpsf, lptvid->lpifq, pNMTreeView->itemNew.hItem);
			}
			
			TV_SORTCB tvscb;
			tvscb.hParent     = pNMTreeView->itemNew.hItem;
			tvscb.lParam      = 0;
			tvscb.lpfnCompare = _xtAfxTreeViewCompareProc;
			GetTreeCtrl().SortChildrenCB(&tvscb);
		}
	}
}

HTREEITEM CXTShellTreeView::GetContextMenu() 
{
	CPoint point;
	::GetCursorPos(&point);
	GetTreeCtrl().ScreenToClient(&point);

	TV_HITTESTINFO tvhti;
	tvhti.pt = point;
	GetTreeCtrl().HitTest(&tvhti);
	
	if (tvhti.flags & (TVHT_ONITEMLABEL|TVHT_ONITEMICON))
	{
		TV_ITEM tvi;
		tvi.mask  = TVIF_PARAM;
		tvi.hItem = tvhti.hItem;

		// Long pointer to TreeView item data
		XT_TVITEMDATA*	lptvid = (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(tvhti.hItem);
		
		GetTreeCtrl().ClientToScreen(&point);

		if ( lptvid->lpsfParent == NULL )
		{
			LPMALLOC lpMalloc = NULL;
			if ( FAILED( ::SHGetMalloc( &lpMalloc ) ) )
			{
				return NULL;
			}

			LPSHELLFOLDER lpShellFolder;
			if ( FAILED( ::SHGetDesktopFolder( &lpShellFolder ) ) )
			{
				return NULL;
			}

			LPITEMIDLIST pidlDesktop = NULL;
			if ( FAILED( ::SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &pidlDesktop ) ) )
			{
				lpShellFolder->Release();
				return NULL;
			}
			
			IShellFolder *pFolder = NULL;
			if ( lpShellFolder->CompareIDs( 0, lptvid->lpifq,pidlDesktop ) )
			{
				pFolder = lpShellFolder;
			}
			else
			{
				lpShellFolder->BindToObject( lptvid->lpifq, 0, IID_IShellFolder, ( LPVOID* )&pFolder );
			}
			
			ShowContextMenu(m_hWnd,
				lpShellFolder, lptvid->lpi, &point);
			
			if ( lpMalloc && pidlDesktop ) {
				lpMalloc->Free( pidlDesktop );
			}
			if ( lpMalloc ) {
				lpMalloc->Release();
			}
			if ( lpShellFolder ) {
				lpShellFolder->Release();
			}
		}
		else
		{
			ShowContextMenu(m_hWnd,
				lptvid->lpsfParent, lptvid->lpi, &point);
		}

		return tvhti.hItem;
	}

	return NULL;
}

BOOL CXTShellTreeView::OnFolderSelected(NM_TREEVIEW* pNMTreeView, CString &strFolderPath) 
{
	BOOL bRet = FALSE;

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	if (hItem != NULL)
	{
		LPSHELLFOLDER lpsf = NULL;

		// Long pointer to TreeView item data
		XT_TVITEMDATA*	lptvid = (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(hItem);
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0, IID_IShellFolder,(LPVOID*)&lpsf)))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;
				
				// Determine what type of object we have.
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);
				
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					TCHAR szBuff[MAX_PATH];
					if(::SHGetPathFromIDList(lptvid->lpifq, szBuff))
					{
						strFolderPath = szBuff;
						bRet = TRUE;
					}
				}

				if (GetTreeCtrl().ItemHasChildren(pNMTreeView->itemNew.hItem) && !(pNMTreeView->itemNew.state & TVIS_EXPANDEDONCE))
				{
					InitTreeViewItems(lpsf, lptvid->lpifq, pNMTreeView->itemNew.hItem);
					
					TV_SORTCB tvscb;
					tvscb.hParent     = pNMTreeView->itemNew.hItem;
					tvscb.lParam      = 0;
					tvscb.lpfnCompare = _xtAfxTreeViewCompareProc;
					GetTreeCtrl().SortChildrenCB(&tvscb);
					
					pNMTreeView->itemNew.state     |= TVIS_EXPANDEDONCE;
					pNMTreeView->itemNew.stateMask |= TVIS_EXPANDEDONCE;
					pNMTreeView->itemNew.mask      |= TVIF_STATE;
					GetTreeCtrl().SetItem(&pNMTreeView->itemNew);
				}

				if(lpsf) {
					lpsf->Release();
				}
			}
		}
	}	

	return bRet;
}

BOOL CXTShellTreeView::InitSystemImageLists()
{
    SHFILEINFO sfi;
    HIMAGELIST himlSmall = (HIMAGELIST)::SHGetFileInfo( _T("C:\\"), 0, &sfi, 
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
	
    if (himlSmall) {
        GetTreeCtrl().SetImageList(CImageList::FromHandle(himlSmall), TVSIL_NORMAL);
		return TRUE;
    }
	
    return FALSE;
}

BOOL CXTShellTreeView::GetSelectedFolderPath(CString &strFolderPath)
{
	BOOL bRet = FALSE;

	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	if (hItem != NULL)
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA*	lptvid = (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(hItem);
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			LPSHELLFOLDER lpsf = NULL;
			if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0,IID_IShellFolder,(LPVOID *)&lpsf)))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;
				
				// Determine what type of object we have.
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);
				
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					TCHAR szBuff[MAX_PATH];
					if(::SHGetPathFromIDList(lptvid->lpifq,szBuff))
					{
						strFolderPath = szBuff;
						bRet = TRUE;
					}
				}
				if(lpsf) {
					lpsf->Release();
				}
			}
		}
	}

	return bRet;
}

BOOL CXTShellTreeView::FindTreeItem(HTREEITEM hItem, XT_LVITEMDATA* lplvid)
{
	while (hItem)
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA*	lptvid = (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(hItem);
		if(lptvid && lplvid)
		{
			if (SCODE_CODE(GetScode(lplvid->lpsfParent->CompareIDs(
				0, lplvid->lpi, lptvid->lpi))) == 0)
			{
				GetTreeCtrl().EnsureVisible(hItem);
				GetTreeCtrl().SelectItem(hItem);
				return TRUE;
			}
		}
		
		HTREEITEM hNextItem = GetTreeCtrl().GetChildItem(hItem);
		if(hNextItem)
		{
			if (FindTreeItem(hNextItem, lplvid)) {
				return TRUE;
			}
		}

		hItem = GetTreeCtrl().GetNextSiblingItem(hItem);
	}

	return FALSE;
}

void CXTShellTreeView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	OnFolderExpanding(pNMTreeView);	
	*pResult = 0;
}

void CXTShellTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	CString strFolderPath;
	OnFolderSelected(pNMTreeView, strFolderPath);

	if (strFolderPath.IsEmpty()) {
		strFolderPath = GetTreeCtrl().GetItemText(pNMTreeView->itemNew.hItem);
	}

	// currently selected TreeItem
	HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
	if (hItem != NULL)
	{
		CDocument* pDoc = GetDocument( );
		if ( pDoc != NULL && !m_bTunneling )
		{
			pDoc->UpdateAllViews( this, SHN_XT_TREESELCHANGE,
				( CObject* )GetTreeCtrl( ).GetItemData( hItem ) );
		}	
		
		if( m_pComboBox->GetSafeHwnd())
		{
			if (m_pComboBox->IsKindOf(RUNTIME_CLASS(CXTComboBoxEx)))
			{
				CXTComboBoxEx* pComboBoxEx = DYNAMIC_DOWNCAST(CXTComboBoxEx, m_pComboBox);
				ASSERT_VALID(pComboBoxEx);
				
				int nFound = CB_ERR;
				int nIndex;
				for (nIndex = 0; nIndex < pComboBoxEx->GetCount(); ++nIndex)
				{
					CString strText;
					pComboBoxEx->GetLBText(nIndex, strText);
					
					if (strFolderPath.Compare(strText) == 0)
					{
						nFound = nIndex;
						pComboBoxEx->SetCurSel(nIndex);
						break;
					}
				}
				
				if (nFound == CB_ERR)
				{
					HTREEITEM hti = GetTreeCtrl().GetSelectedItem();
					ASSERT(hti);
					
					if (strFolderPath.IsEmpty()) {
						strFolderPath = GetTreeCtrl().GetItemText(hti);
					}
					
					int nImage, nSelectedImage;
					GetTreeCtrl().GetItemImage(hti, nImage, nSelectedImage);
					
					pComboBoxEx->InsertItem(0, strFolderPath, 0, nImage, nImage);
					pComboBoxEx->SetCurSel(0);
					pComboBoxEx->SetItemData(0, (DWORD_PTR)(HTREEITEM)hti);
				}
			}
			else if (m_pComboBox->IsKindOf(RUNTIME_CLASS(CComboBox)))
			{
				CComboBox* pComboBox = DYNAMIC_DOWNCAST(CComboBox, m_pComboBox);
				ASSERT_VALID(pComboBox);
				
				int nFound = pComboBox->FindString(-1, strFolderPath);
				if (nFound == CB_ERR)
				{
					HTREEITEM hti = GetTreeCtrl().GetSelectedItem();
					ASSERT(hti);
					
					pComboBox->InsertString(0, strFolderPath);
					pComboBox->SetCurSel(0);
					pComboBox->SetItemData(0, (DWORD_PTR)(HTREEITEM)hti);
				}
				else {
					pComboBox->SetCurSel(nFound);
				}
			}
		}
	}
	
	*pResult = 0;
}

void CXTShellTreeView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED_ALWAYS(pNMHDR);

	// Display the shell context menu.
	if (m_bContextMenu == TRUE)
	{
		HTREEITEM hItem = GetContextMenu();
		if (hItem != NULL)
		{
			// TODO: Additional error handling.
		}
	}

	*pResult = 0;
}


BOOL CXTShellTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CXTTreeView::PreCreateWindow(cs))
		return FALSE;

	cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT;

	return TRUE;
}

void CXTShellTreeView::OnInitialUpdate() 
{
	CXTTreeView::OnInitialUpdate();
	
	// Initialize the image list for the list view and populate it.
	if (!GetTreeCtrl().GetImageList(TVSIL_NORMAL))
	{
		// Initialize the image list for the list view and populate it.
		InitSystemImageLists();

		DWORD dwStyle = 0;
		
		// winxp explorer style is different than older os's.
		if ( CXTOSVersionInfo::Get( ).IsWinXPOrGreater( ) ) {
			dwStyle = TVS_HASBUTTONS | TVS_SINGLEEXPAND | TVS_TRACKSELECT | TVS_SHOWSELALWAYS;
		}
		else {
			dwStyle = TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
		}
		
		ModifyStyle(NULL, dwStyle);
		
		PopulateTreeView();
		EnableMultiSelect(FALSE);
	}
}

void CXTShellTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UNUSED_ALWAYS(pSender);
	
	switch (lHint)
	{
	// Update selection of tree view to the
	case SHN_XT_SELECTCHILD:
		{
			XT_LVITEMDATA*	lplvid = (XT_LVITEMDATA*)pHint;
			ASSERT(lplvid != NULL);

			LockWindowUpdate();
			if(!FindTreeItem(GetTreeCtrl().GetSelectedItem(), lplvid))
			{
				// The folder was not found so we send back a message
				// to the listview to execute the itemid
				CDocument* pDoc = GetDocument();
				pDoc->UpdateAllViews(this, SHN_XT_NOFOLDER, (CObject*)lplvid);
			}
			UnlockWindowUpdate();
		}
		break;
		
	default:
		break;
	}	
}

void CXTShellTreeView::InitTreeNode(HTREEITEM hItem, XT_TVITEMDATA* lptvid)
{
	SetRedraw(FALSE);
	if (lptvid)
	{
		LPSHELLFOLDER lpsf = NULL;
		if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
			0, IID_IShellFolder,(LPVOID *)&lpsf))) {
			InitTreeViewItems(lpsf, lptvid->lpifq, hItem);
		}
		
		TV_SORTCB tvscb;
		tvscb.hParent     = hItem;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = _xtAfxTreeViewCompareProc;
		
		GetTreeCtrl().SortChildrenCB(&tvscb);
	}

	SetRedraw();
}

BOOL CXTShellTreeView::SearchTree(HTREEITEM hItem, CString strSearchName, FindAttribs attr, BOOL bFindRoot)
{
	BOOL bRet = FALSE;
	
	while (hItem && bRet == FALSE)
	{
		// Long pointer to TreeView item data
		XT_TVITEMDATA*	lptvid = (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(hItem);
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			LPSHELLFOLDER lpsf = NULL;
			if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
				0,IID_IShellFolder,(LPVOID *)&lpsf)))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;
				lptvid->lpsfParent->GetAttributesOf(1,
					(const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					TCHAR szBuff[MAX_PATH];
					if (::SHGetPathFromIDList(lptvid->lpifq, szBuff))
					{
						switch(attr)
						{
						case type_drive:
							{
								TCHAR szDrive[_MAX_DRIVE];
								TCHAR szDir[_MAX_DIR];
								TCHAR szFileName[_MAX_FNAME];
								TCHAR szExt[_MAX_EXT];

								_tsplitpath(szBuff, szDrive, szDir, szFileName, szExt);
								_tcscpy(szBuff, szDrive);

								// Making this null will cause the compare to fail
								if ( bFindRoot && _tcslen( szDir ) != 1 )
								{
									szBuff[0] = 0; 
								}
							}
							break;
						
						case type_folder:
							_tcscpy(szBuff, GetTreeCtrl().GetItemText(hItem));
							break;
						}
						
						if (strSearchName.CompareNoCase(szBuff) == 0)
						{
							GetTreeCtrl().EnsureVisible(hItem);
							GetTreeCtrl().SelectItem(hItem);
							bRet = TRUE;
						}
					}
				}
				lpsf->Release();
			}
		}
		hItem = GetTreeCtrl().GetNextSiblingItem(hItem);
	}
	return bRet;
}

void CXTShellTreeView::TunnelTree(CString strFindPath)
{
	if ( strFindPath.GetLength() == 1 )
	{
		strFindPath += _T(":\\");
	}
	if ( strFindPath.GetLength() == 2 && strFindPath.GetAt( 1 ) == ':' )
	{
		strFindPath += _T("\\");
	}

	if (_taccess(strFindPath, 0) == -1)
	{
		if (strFindPath.GetLength() != 3) {
			MessageBox(strFindPath, _T("Folder not found"), MB_ICONERROR);
			return;
		}
	}

	m_bTunneling = true;
	
	if(strFindPath.ReverseFind(_T('\\')) != strFindPath.GetLength()-1) {
		strFindPath += _T("\\");
	}
	
	BOOL bFound = FALSE;
	BOOL bLock = LockWindowUpdate();
	BOOL bFindRoot = FALSE;

	HTREEITEM hItemRoot = GetTreeCtrl().GetRootItem();
	hItemRoot = GetTreeCtrl().GetChildItem( hItemRoot );

	while (hItemRoot && !bFound)
	{
		TCHAR szDrive[_MAX_DRIVE];
		TCHAR szDir[_MAX_DIR];
		TCHAR szFileName[_MAX_FNAME];
		TCHAR szExt[_MAX_EXT];

		_tsplitpath(strFindPath, szDrive, szDir, szFileName, szExt);
		bFindRoot = _tcslen(szDir) == 1; // There will only be a single backslash if it's the root

		// Ensure child items are populated
		UINT uState = GetTreeCtrl().GetItemState(hItemRoot, TVIS_EXPANDEDONCE);
		if (!(uState & TVIS_EXPANDEDONCE))
		{
			InitTreeNode(hItemRoot, (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(hItemRoot));
			GetTreeCtrl().SetItemState(hItemRoot, TVIS_EXPANDEDONCE, TVIS_EXPANDEDONCE);
		}

		CString strItemText = GetTreeCtrl().GetItemText(hItemRoot);
		HTREEITEM hItemNext = GetTreeCtrl().GetChildItem(hItemRoot);
		strItemText = GetTreeCtrl().GetItemText(hItemNext);

		// search the szDrive first
		if ( SearchTree( hItemNext, szDrive, CXTShellTreeView::type_drive, bFindRoot ) )
		{
			if ( bFindRoot )
			{
				bFound = TRUE;
			}
			else 
			{
				// break down subfolders and search
				TCHAR* pszNext = _tcstok(szDir, _T("\\"));
				while(pszNext)
				{
					HTREEITEM hItem = GetTreeCtrl().GetSelectedItem();
					strItemText = GetTreeCtrl().GetItemText(hItem);
					
					// Ensure child items are populated
					uState = GetTreeCtrl().GetItemState(hItem, TVIS_EXPANDEDONCE);
					if (!(uState & TVIS_EXPANDEDONCE))
					{
						InitTreeNode(hItem, (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(hItem));
						GetTreeCtrl().SetItemState(hItem, TVIS_EXPANDEDONCE, TVIS_EXPANDEDONCE);
					}
					
					HTREEITEM hItemChild = GetTreeCtrl().GetChildItem(hItem);
					if (hItemChild) {
						strItemText = GetTreeCtrl().GetItemText(hItemChild);
					}
					
					// Find?
					if(SearchTree(hItemChild, pszNext, CXTShellTreeView::type_folder))
					{
						// We know this was successful - expand at this 
						GetTreeCtrl().Expand(hItem, TVE_EXPAND);
						pszNext = _tcstok(NULL, _T("\\"));
						
						// No more?
						if (pszNext == NULL)
						{
							// Tunnelled successfully down
							bFound = TRUE;
							break;
						}
					}
					else 
					{
						// Abort as this part not found
						pszNext = NULL;
					}
				}
			}
		}

		// Try next item in the namespace
		hItemRoot = GetTreeCtrl().GetNextSiblingItem(hItemRoot);
	}
	
	if (bLock) {
		UnlockWindowUpdate();
	}

	m_bTunneling = false;

	HTREEITEM hItem = GetTreeCtrl( ).GetSelectedItem( );
	if ( hItem != NULL )
	{
		CDocument* pDoc = GetDocument( );
		if ( pDoc != NULL )
		{
			pDoc->UpdateAllViews( this, SHN_XT_TREESELCHANGE,
				( CObject* )GetTreeCtrl( ).GetItemData( hItem ) );
		}
	}
}

BOOL CXTShellTreeView::GetFolderItemPath(HTREEITEM hItem, CString &strFolderPath)
{
	BOOL bRet = FALSE;

	// Long pointer to TreeView item data
	XT_TVITEMDATA*	lptvid = (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(hItem);
	if (lptvid && lptvid->lpsfParent && lptvid->lpi)
	{
		LPSHELLFOLDER lpsf = NULL;
		if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
			0,IID_IShellFolder,(LPVOID *)&lpsf)))
		{
			ULONG ulAttrs = SFGAO_FILESYSTEM;
			
			// Determine what type of object we have.
			lptvid->lpsfParent->GetAttributesOf(1,
				(const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);
			
			if (ulAttrs & (SFGAO_FILESYSTEM))
			{
				TCHAR szBuff[MAX_PATH];
				if (::SHGetPathFromIDList(lptvid->lpifq, szBuff))
				{
					strFolderPath = szBuff;
					bRet = TRUE;
				}
			}
		}
		if(lpsf) {
			lpsf->Release();
		}
	}
	
	return bRet;
}

void CXTShellTreeView::PopulateTree(LPCTSTR lpszPath)
{
	CString	strFolder = lpszPath;
	CString	strNextFolder;
	CString strPath;
	
	LPMALLOC pMalloc;
	if (::SHGetMalloc(&pMalloc) == NOERROR)
	{
		// Get a pointer to the desktop folder.
		LPSHELLFOLDER lpsf = NULL;
		if (SUCCEEDED(::SHGetDesktopFolder(&lpsf)))
		{
			LPITEMIDLIST lpi = NULL;
			USES_CONVERSION;

			// Initialize the tree view to be empty.
			GetTreeCtrl().DeleteAllItems();
			
			do
			{
				// Get the Next Component
				strNextFolder = _xtAfxPathFindNextComponent(strFolder);
				if (!strNextFolder.IsEmpty())
				{
					strPath = strFolder.Left(strFolder.GetLength() -
						strNextFolder.GetLength());
				}
				else
				{
					strPath = strFolder;
					strNextFolder.Empty();
				}
				
				// Get ShellFolder Pidl
				ULONG eaten;
				if (FAILED(lpsf->ParseDisplayName( NULL, NULL, T2OLE((TCHAR*)(LPCTSTR)strPath),
					&eaten, &lpi, NULL))) {
					break;
				}

				LPSHELLFOLDER lpsf2 = NULL;
				if (FAILED(lpsf->BindToObject(lpi, 0, IID_IShellFolder,(LPVOID *)&lpsf2))) {
					break;
				}
				
				pMalloc->Free(lpi);
				
				// Release the Parent Folder pointer.
				lpsf->Release();
				
				// Change Folder Info
				lpsf = lpsf2;
				strFolder = strNextFolder;
			}
			while (!strNextFolder.IsEmpty());
			
			InitTreeViewItems(lpsf, NULL, TVI_ROOT);

			if ( lpsf ) {
				lpsf->Release();
			}
		}
		if ( pMalloc ) {
			pMalloc->Release();
		}
	}

	TV_SORTCB tvscb;
    tvscb.hParent     = TVI_ROOT;
    tvscb.lParam      = 0;
    tvscb.lpfnCompare = _xtAfxTreeViewCompareProc;
	
    // Sort the items in the tree view
	GetTreeCtrl().SortChildrenCB(&tvscb);
    
	HTREEITEM hItem;
	hItem = GetTreeCtrl().GetRootItem();
	GetTreeCtrl().Expand(hItem,TVE_EXPAND);
	GetTreeCtrl().Select(GetTreeCtrl().GetRootItem(),TVGN_CARET);
}

void CXTShellTreeView::AssociateCombo(CWnd* pWnd)
{
	ASSERT(pWnd != NULL);
	m_pComboBox = pWnd;

	if (m_pComboBox->IsKindOf(RUNTIME_CLASS(CXTComboBoxEx)))
	{
		SHFILEINFO sfi;
		HIMAGELIST hImageList = (HIMAGELIST)::SHGetFileInfo( _T("C:\\"), 0, &sfi, 
			sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );
		
		if (hImageList != NULL)
		{
			CXTComboBoxEx* pComboBoxEx = DYNAMIC_DOWNCAST(CXTComboBoxEx, m_pComboBox);
			ASSERT_VALID(pComboBoxEx);
			pComboBoxEx->SetImageList(CImageList::FromHandle(hImageList));
		}
	}
}

BOOL CXTShellTreeView::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

void CXTShellTreeView::OnPaint() 
{
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to reduce screen flicker.
	CXTMemDC memDC(&dc, rectClient, xtAfxData.clrWindow);

	// Let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

void CXTShellTreeView::OnDragDrop(NM_TREEVIEW* pNMTreeView)
{
	// Long pointer to ListView item data
	XT_TVITEMDATA*	lplvid = (XT_TVITEMDATA*)GetTreeCtrl().GetItemData(pNMTreeView->itemNew.hItem);
	ASSERT(lplvid);

	if ( lplvid && lplvid->lpsfParent )
	{
		LPDATAOBJECT lpdo;
		
		HRESULT hResult = lplvid->lpsfParent->GetUIObjectOf( AfxGetMainWnd( )->m_hWnd, 1,
			( const struct _ITEMIDLIST** )&lplvid->lpi, IID_IDataObject, 0, ( LPVOID* )&lpdo );

		if ( SUCCEEDED( hResult ) )
		{
			LPDROPSOURCE lpds = new CXTDropSource();
			ASSERT(lpds != NULL);

			DWORD dwEffect;
			::DoDragDrop(lpdo, lpds,
				DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK, &dwEffect);

			lpdo->Release();
			lpds->Release();
		}
	}
}

void CXTShellTreeView::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	OnDragDrop(pNMTreeView);
	*pResult = 0;
}

void CXTShellTreeView::OnBeginRDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	OnDragDrop(pNMTreeView);	
	*pResult = 0;
}
