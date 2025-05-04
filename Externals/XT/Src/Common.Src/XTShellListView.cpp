// XTShellListView.cpp : implementation file
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
#include "XTFlatHeaderCtrl.h"
#include "XTListCtrl.h"
#include "XTListView.h"
#include "XTShellPidl.h"
#include "XTShellSettings.h"
#include "XTDirWatcher.h"
#include "XTShellListView.h"
#include "XTSortClass.h"
#include "XTResource.h"
#include "XTDropSource.h"
#include "XTHelpers.h"
#include "XTPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTShellListView

CXTShellListView::CXTShellListView()
{
	m_bContextMenu = TRUE;
	m_uFlags       = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;
	
	if (m_shSettings.ShowAllFiles() && !m_shSettings.ShowSysFiles()) {
		m_uFlags |= SHCONTF_INCLUDEHIDDEN;
	}

	if (!SUCCEEDED(::SHGetSpecialFolderLocation(NULL, CSIDL_INTERNET, &m_pidlINet)))
	{
		m_pidlINet = NULL;
	}

	// Start the directory monitoring thread.
	m_pDirThread = (CXTDirWatcher*)AfxBeginThread(
		RUNTIME_CLASS(CXTDirWatcher), THREAD_PRIORITY_IDLE);
}

CXTShellListView::~CXTShellListView()
{
	// End the directory monitoring thread.
	SAFE_DELETE(m_pDirThread);

	if (m_pidlINet)
	{
		LPMALLOC lpMalloc;
		if (SUCCEEDED(::SHGetMalloc(&lpMalloc)))
		{
			if ( lpMalloc && m_pidlINet ) {
				lpMalloc->Free(m_pidlINet);
			}
			if ( lpMalloc ) {
				lpMalloc->Release();
			}
		}
	}
}

IMPLEMENT_DYNCREATE(CXTShellListView, CListView)

BEGIN_MESSAGE_MAP(CXTShellListView, CListView)
	//{{AFX_MSG_MAP(CXTShellListView)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBeginDrag)
	ON_NOTIFY_REFLECT(LVN_BEGINRDRAG, OnBeginRDrag)
	//}}AFX_MSG_MAP
	ON_MESSAGE(XTWM_SHELL_NOTIFY, OnUpdateShell)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTShellListView message handlers

BOOL CXTShellListView::PopulateListView(XT_TVITEMDATA* lptvid, LPSHELLFOLDER lpsf)
{
	//clear the view for new items
    GetListCtrl().DeleteAllItems();
    UpdateWindow();
    
    // Turn off redraw so th euser does't see resorting
    SetRedraw(false);
    
    if (InitListViewItems(lptvid, lpsf))
	{
		GetListCtrl().SortItems(_xtAfxListViewCompareProc, 0);
		SetRedraw(true);
		return TRUE;
	}

	SetRedraw(false);
	return FALSE;
}

void CXTShellListView::ShellListUpdate()
{
	if (m_pDirThread != NULL)
	{
		// Turn off redraw so the user does't see resorting
		SetRedraw(false);
		
		//clear the view for new items and update the view.
		GetListCtrl().DeleteAllItems();
		
		if (InitListViewItems(
			m_pDirThread->GetItemData(),
			m_pDirThread->GetShellFolder()))
		{
			GetListCtrl().SortItems(_xtAfxListViewCompareProc, 0);
		}

		SetRedraw(true);
	}
}

void CXTShellListView::ShellListRestart()
{
	// End the directory monitoring thread.
	SAFE_DELETE(m_pDirThread);

	// Start the directory monitoring thread.
	m_pDirThread = (CXTDirWatcher*)AfxBeginThread(
		RUNTIME_CLASS(CXTDirWatcher), THREAD_PRIORITY_IDLE);

	// Set the notification window handle.
	if (m_pDirThread != NULL) {
		m_pDirThread->SetNotifyWindow(m_hWnd);
	}
}

void CXTShellListView::BuildDefaultColumns()
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	CString strLabel;
	strLabel.LoadString(XT_IDS_NAME);
	GetListCtrl().InsertColumn(0, strLabel, LVCFMT_LEFT, 150, 0);

	strLabel.LoadString(XT_IDS_SIZE);
	GetListCtrl().InsertColumn(1, strLabel, LVCFMT_RIGHT, 100, 1);

	strLabel.LoadString(XT_IDS_TYPE);
	GetListCtrl().InsertColumn(2, strLabel, LVCFMT_LEFT, 120, 2);

	strLabel.LoadString(XT_IDS_MODIFIED);
	GetListCtrl().InsertColumn(3, strLabel, LVCFMT_LEFT, 120, 3);
}

BOOL CXTShellListView::InitSystemImageLists()
{
    SHFILEINFO sfi;
    HIMAGELIST himlSmall = (HIMAGELIST)::SHGetFileInfo( _T("C:\\"), 0, &sfi, 
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON );

    HIMAGELIST himlLarge = (HIMAGELIST)::SHGetFileInfo( _T("C:\\"), 0, &sfi, 
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_LARGEICON );

    if (himlSmall && himlLarge)
	{
        GetListCtrl().SetImageList(CImageList::FromHandle(himlSmall), LVSIL_SMALL);
		GetListCtrl().SetImageList(CImageList::FromHandle(himlLarge), LVSIL_NORMAL);
		return TRUE;
    }

    return FALSE;
}

BOOL CXTShellListView::InitListViewItems(XT_TVITEMDATA* lptvid, LPSHELLFOLDER lpsf)
{
	if (m_pDirThread != NULL) {
		m_pDirThread->SetFolderData(lptvid, lpsf);
	}

    LPMALLOC lpMalloc;                                             
    if (SUCCEEDED(::SHGetMalloc(&lpMalloc)))
    {
        LPENUMIDLIST lpe = NULL;
        
        if (SUCCEEDED(lpsf->EnumObjects(::GetParent(m_hWnd), m_uFlags, &lpe)))
        {
            int          iCtr      = 0;
            ULONG        ulFetched = 0;
            LPITEMIDLIST lpi       = NULL;
            
            while (lpe->Next(1, &lpi, &ulFetched) == S_OK)
            {
                // allocate memory for ITEMDATA struct
                XT_LVITEMDATA* lplvid = (XT_LVITEMDATA*)lpMalloc->Alloc(sizeof(XT_LVITEMDATA));
                if (lplvid == NULL)
                {
                    if (lpe) {
                        lpe->Release();
                    }
                    if (lpMalloc) {
                        lpMalloc->Release();
                    }
                    return FALSE;
                }
                
                // Now get the friendly name that we'll put in the treeview...
                TCHAR szBuff[_MAX_PATH];
                GetName(lpsf, lpi, SHGDN_NORMAL, szBuff);
                
                // Note that since we are interested in the display attributes as well as
                // the other attributes, we need to set ulAttrs to SFGAO_DISPLAYATTRMASK
                // before calling GetAttributesOf();
                ULONG ulAttrs = SFGAO_DISPLAYATTRMASK | SFGAO_REMOVABLE;
                UINT  uFlags  = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON;
                
                lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);
                lplvid->ulAttribs = ulAttrs;
                LPITEMIDLIST lpifqThisItem = ConcatPidls(lptvid->lpifq, lpi);
                
                LV_ITEM lvi;
                lvi.mask       = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
                lvi.iItem      = iCtr++;
                lvi.iSubItem   = 0;
                lvi.pszText    = szBuff;
                lvi.cchTextMax = MAX_PATH;
                lvi.iImage     = GetItemIcon(ConcatPidls(lptvid->lpifq, lpi), uFlags);
                
                lplvid->lpsfParent = lpsf;
                lpsf->AddRef();
                
                // Make a copy of the ITEMIDLIST
                lplvid->lpi = DuplicateItem(lpMalloc, lpi);
                lvi.lParam = (LPARAM)lplvid;
                
                // Add the item to the listview   
                int iIndex = GetListCtrl().InsertItem(&lvi);
                SetAttributes(iIndex, ulAttrs);
                if (iIndex >= 0)
                {
					TCHAR szItemPath[_MAX_PATH];
					::SHGetPathFromIDList( lpifqThisItem, szItemPath );
					
                    if (((ulAttrs & SFGAO_FILESYSTEM) == SFGAO_FILESYSTEM) &&
                        ((ulAttrs & SFGAO_FOLDER) == 0))
                    {
						WIN32_FIND_DATA fdata;
						HANDLE handle = ::FindFirstFile( szItemPath, &fdata );
						
						if ( handle != INVALID_HANDLE_VALUE )
						{
							LONGLONG fsize = fdata.nFileSizeHigh*( ( LONGLONG )ULONG_MAX+1 ) + fdata.nFileSizeLow;
							
							TCHAR szBuffer[16];
							CString strSize;
							strSize.Format( _T( "%s KB" ), InsertCommas( ( fsize+1024 )/1024, szBuffer,15 ) );
							
							GetListCtrl().SetItemText( iIndex, 1, strSize );

							FILETIME ltime;
							::FileTimeToLocalFileTime( &fdata.ftLastWriteTime, &ltime );

							SYSTEMTIME time;
							::FileTimeToSystemTime( &ltime, &time );

							CTime cTime;
							cTime = CTime(
								time.wYear,
								time.wMonth,
								time.wDay,
								time.wHour,
								time.wMinute,
								time.wSecond);
							
							GetListCtrl().SetItemText( iIndex, 3, cTime.Format( _T( "%m/%d/%y %I:%M %p" ) ) );

							::FindClose( handle );
						}

						/*CFileFind ff;
						if (ff.FindFile(szItemPath))
						{
							ff.FindNextFile();
							CString strSize;
							strSize.Format(_T("%ldKB"), (ff.GetLength()+1024)/1024);
							GetListCtrl().SetItemText(iIndex, 1, strSize);
							
							CTime time;
							ff.GetCreationTime(time);                               
							GetListCtrl().SetItemText(iIndex, 3, time.Format(_T("%m/%d/%y %I:%M %p")));
						}*/
                    }
                    else {
                        GetListCtrl().SetItemText(iIndex, 2, _T("0 bytes"));
                    }
                    
                    SHFILEINFO sfi;
                    ::SHGetFileInfo((TCHAR*)lpifqThisItem, 0, &sfi, 
                        sizeof(SHFILEINFO), SHGFI_PIDL|SHGFI_TYPENAME);
                    
                    GetListCtrl().SetItemText(iIndex,2,sfi.szTypeName);
                }
                else {
                    return FALSE;
                }
                
				if (lpifqThisItem) {
					lpMalloc->Free(lpifqThisItem);
				}
                
				// Free the pidl that the shell gave us.
				if (lpi) {
					lpMalloc->Free(lpi);
                    lpi = NULL;
				}
            }
        }
        
        if (lpe) {
            lpe->Release();
        }
        if (lpMalloc) {
            lpMalloc->Release();
        }
        
        return TRUE;
    }
    
    return FALSE;
}

TCHAR* CXTShellListView::InsertCommas(LONGLONG value, TCHAR* szBufferOut, UINT nSize)
{
	TCHAR szBufferIn[ 30 ]; // 30 digits is a really big number
	TCHAR szDecimalSep[ 5 ];
	TCHAR szThousandSep[ 5 ];
	
	NUMBERFMT fmt;
	fmt.NumDigits = 0; // No decimal places
	::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_ILZERO, szBufferIn, 2 );
	fmt.LeadingZero = _ttoi( szBufferIn );
	fmt.Grouping = 3; 
	::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, szDecimalSep, 4 );
	fmt.lpDecimalSep = szDecimalSep; 
	::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, szThousandSep,4 );
	fmt.lpThousandSep = szThousandSep; 
	::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, szBufferIn, 2 );
	fmt.NegativeOrder = _ttoi( szBufferIn );
	_stprintf( szBufferIn, _T( "%I64d" ), value );
	::GetNumberFormat( LOCALE_USER_DEFAULT, 0, szBufferIn, &fmt, szBufferOut, nSize );
	
	return szBufferOut;
}

void CXTShellListView::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
   // Note that we don't check the return value here because if GetIcon()
   // fails, then we're in big trouble...
	lptvitem->iImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
   
	lptvitem->iSelectedImage = GetItemIcon(lpifq,
		SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON);
}

int CXTShellListView::GetDoubleClickedItem()
{
	CPoint point;
	::GetCursorPos(&point);
	GetListCtrl().ScreenToClient(&point);

	LV_HITTESTINFO	lvhti;
	lvhti.pt = point;
	GetListCtrl().HitTest(&lvhti);

	if ((lvhti.flags & LVHT_ONITEM) == 0) {
		return -1;
	}

	return lvhti.iItem;
}

int CXTShellListView::GetContextMenu()
{
	CPoint point;
	::GetCursorPos(&point);
	GetListCtrl().ScreenToClient(&point);

	LV_HITTESTINFO lvhti;
	lvhti.pt = point;
	GetListCtrl().HitTest(&lvhti);

	if (lvhti.flags & LVHT_ONITEM)
	{
        LV_ITEM lvi;
        lvi.mask     = LVIF_PARAM;
        lvi.iItem    = lvhti.iItem;
        lvi.iSubItem = 0;
        
        // Long pointer to ListView item data
        XT_LVITEMDATA* lplvid = (XT_LVITEMDATA*)GetListCtrl().GetItemData(lvhti.iItem);
		ASSERT(lplvid != NULL);
        
        GetListCtrl().ClientToScreen(&point);
        ShowContextMenu(m_hWnd, 
            lplvid->lpsfParent, lplvid->lpi, &point);

		return lvhti.iItem;
	}

	return -1;
}

bool CXTShellListView::ShellOpenItem(int iItem)
{
	// Long pointer to ListView item data
	XT_LVITEMDATA*	lplvid = (XT_LVITEMDATA*)GetListCtrl().GetItemData(iItem);
	return ShellOpenItem(lplvid);
}

bool CXTShellListView::ShellOpenItem(XT_LVITEMDATA* lplvid)
{
	// Long pointer to ListView item data
	if (!(lplvid->ulAttribs & (SFGAO_FOLDER | SFGAO_FILESYSANCESTOR | SFGAO_REMOVABLE)))
	{
		SHELLEXECUTEINFO sei;
		::ZeroMemory(&sei, sizeof(SHELLEXECUTEINFO));
        
		sei.cbSize   = sizeof(SHELLEXECUTEINFO);
		sei.fMask    = SEE_MASK_INVOKEIDLIST;
        sei.hwnd     = ::GetParent(m_hWnd);
        sei.nShow    = SW_SHOWNORMAL;
        sei.hInstApp = AfxGetInstanceHandle();
        sei.lpIDList = GetFullyQualPidl(lplvid->lpsfParent, lplvid->lpi);

		if ( ::ShellExecuteEx(&sei) )
		{
			return true;
		}
	}

	return false;
}

BOOL CXTShellListView::GetItemPath(int iItem, CString &strItemPath)
{
	if(iItem >= 0)
	{
		// Long pointer to TreeView item data
		XT_LVITEMDATA*	lplvid = (XT_LVITEMDATA*)GetListCtrl().GetItemData(iItem);
		if (lplvid != 0)
		{
			LPITEMIDLIST lpid = GetFullyQualPidl(lplvid->lpsfParent, lplvid->lpi);

            TCHAR szItemPath[_MAX_PATH];
			if (::SHGetPathFromIDList(lpid, szItemPath))
			{
                strItemPath = szItemPath;
				return TRUE;
			}
		}
	}
	
	return FALSE;
}

void CXTShellListView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED_ALWAYS(pNMHDR);

	// Display the shell context menu.
	if (m_bContextMenu == TRUE)
	{
		int iIndex = GetContextMenu();
		if (iIndex != -1)
		{
			// TODO: Additional error handling.
		}
	}

	*pResult = 0;
}

void CXTShellListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UNUSED_ALWAYS(pNMHDR);

	int	iIndex = GetDoubleClickedItem();
	if (iIndex >= 0)
	{
		CDocument* pDoc = GetDocument();
		pDoc->UpdateAllViews(this, SHN_XT_SELECTCHILD,
			(CObject*)GetListCtrl().GetItemData(iIndex));
	}
	*pResult = 0;
}

BOOL CXTShellListView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CListView::PreCreateWindow(cs))
		return FALSE;

	cs.style |= LVS_REPORT | LVS_SHAREIMAGELISTS;

	return TRUE;
}

void CXTShellListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	// Set the notification window handle.
	if (m_pDirThread != NULL) {
		m_pDirThread->SetNotifyWindow(m_hWnd);
	}
	
	// Initialize the columns and image list for the list control.
	BuildDefaultColumns();
	InitSystemImageLists();
	
	SubclassHeader(FALSE);
	GetFlatHeaderCtrl()->ShowSortArrow(TRUE);
}

LRESULT CXTShellListView::OnUpdateShell(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (wParam)
	{
	case SHN_XT_LISTRESTART:
		{
			ShellListRestart();
			break;
		}
	case SHN_XT_LISTUPDATE:
		{
			ShellListUpdate();
			break;
		}
		
	default:
		break;
	}
	
	return 0;
}

void CXTShellListView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	UNUSED_ALWAYS(pSender);

	switch (lHint)
	{
		case SHN_XT_TREESELCHANGE:
			{
				CWnd* pOwner = GetOwner();
				ASSERT_VALID(pOwner);

				// The tree view selection has changed, so update the contents
				// of the list view
				XT_TVITEMDATA*	lptvid = (XT_TVITEMDATA*)pHint;
				ASSERT(lptvid != NULL);

				if ( lptvid->lpsfParent == NULL )
				{
					LPMALLOC lpMalloc = NULL;
					if ( FAILED( ::SHGetMalloc( &lpMalloc ) ) )
					{
						return;
					}
					
					LPSHELLFOLDER lpShellFolder;
					if ( FAILED( ::SHGetDesktopFolder( &lpShellFolder ) ) )
					{
						return;
					}
					
					LPITEMIDLIST pidlDesktop = NULL;
					if ( FAILED( ::SHGetSpecialFolderLocation( NULL, CSIDL_DESKTOP, &pidlDesktop ) ) )
					{
						lpShellFolder->Release();
						return;
					}

					IShellFolder *pFolder = NULL;
					if ( lpShellFolder->CompareIDs( 0, lptvid->lpifq,pidlDesktop ) )
					{
						pFolder = lpShellFolder;
					}
					else
					{
						lpShellFolder->BindToObject( lptvid->lpifq, 0, IID_IShellFolder, ( LPVOID* )&pFolder );
						lpShellFolder->Release();
					}

					PopulateListView( lptvid, lpShellFolder );
						
					if ( m_pidlINet && ( lpShellFolder->CompareIDs( 0, lptvid->lpifq, m_pidlINet ) == 0 ) )
					{
						pOwner->SendMessage( XTWM_SHELL_NOTIFY, SHN_XT_INETFOLDER );
					}
					else
					{
						pOwner->SendMessage( XTWM_SHELL_NOTIFY );
					}
					
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
					LPSHELLFOLDER lpsf = NULL;
					if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi,
						0, IID_IShellFolder, (LPVOID*)&lpsf)))
					{
						PopulateListView(lptvid, lpsf);
						lpsf->Release();

						if (SUCCEEDED(::SHGetDesktopFolder(&lpsf)))
						{
							if (m_pidlINet && (lpsf->CompareIDs(0, lptvid->lpifq, m_pidlINet) == 0)) {
								pOwner->SendMessage(XTWM_SHELL_NOTIFY, SHN_XT_INETFOLDER);
							}
							else {
								pOwner->SendMessage(XTWM_SHELL_NOTIFY);
							}

							lpsf->Release();
						}
					}
					else
					{
						if (SUCCEEDED(::SHGetDesktopFolder(&lpsf)))
						{
							if (m_pidlINet && (lpsf->CompareIDs(0, lptvid->lpifq, m_pidlINet) == 0)) {
								pOwner->SendMessage(XTWM_SHELL_NOTIFY, SHN_XT_INETFOLDER);
							}

							lpsf->Release();
						}
					}
				}
			}
			break;

		case SHN_XT_NOFOLDER:
			{
				// The item double clicked was not found in the treeview
				// so it sent us back a confirmation to execute it
				XT_LVITEMDATA*	lplvid = (XT_LVITEMDATA*)pHint;
				ASSERT(lplvid);

				ShellOpenItem(lplvid);
			}
			break;

		default:
			break;
	}
}

bool CXTShellListView::SortList( int nCol, bool bAscending )
{
	XT_DATA_TYPE arrColType[] = {
		DT_STRING, DT_INT, DT_STRING, DT_DATETIME
	};

	CXTSortClass sortClass(&GetListCtrl(), nCol);
	sortClass.Sort(bAscending, arrColType[nCol]);

	return true;
}

void CXTShellListView::SetAttributes(int iItem, DWORD dwAttributes)
{
    MapShellFlagsToItemAttributes(&GetListCtrl(), iItem, dwAttributes, LVIS_CUT);
}

void CXTShellListView::OnDragDrop(NM_LISTVIEW* pNMListView)
{
	UNREFERENCED_PARAMETER(pNMListView);

    COleDataSource oleDataSource;
	HGLOBAL        hgDrop;
	DROPFILES*     pDrop;
	CStringList    lsDraggedFiles;
	POSITION       pos;
	int            nSelItem;
	CString        sFile;
	UINT           uBuffSize = 0;
	TCHAR*         pszBuff;

	FORMATETC      etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	
    // For every selected item in the list, put the filename into lsDraggedFiles.
#if _MSC_VER < 1200 // MFC 5.0
    pos = GetFirstSelectedItemPosition();
#else
    pos = GetListCtrl().GetFirstSelectedItemPosition();
#endif // MFC 5.0
	
    while ( NULL != pos )
	{
#if _MSC_VER < 1200 // MFC 5.0
        nSelItem = GetNextSelectedItem ( pos );
#else
        nSelItem = GetListCtrl().GetNextSelectedItem ( pos );
#endif // MFC 5.0
		GetItemPath(nSelItem, sFile);
        lsDraggedFiles.AddTail ( sFile );
		
        // Calculate the # of chars required to hold this string.
        uBuffSize += lstrlen ( sFile ) + 1;
	}
	
    // Add 1 extra for the final null char, and the size of the DROPFILES struct.
    uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);
	
    // Allocate memory from the heap for the DROPFILES struct.
    hgDrop = ::GlobalAlloc ( GHND | GMEM_SHARE, uBuffSize );
	
    if ( NULL == hgDrop )
        return;
	
    pDrop = (DROPFILES*) ::GlobalLock ( hgDrop );
	
    if ( NULL == pDrop )
	{
        ::GlobalFree ( hgDrop );
        return;
	}
	
    // Fill in the DROPFILES struct.
    pDrop->pFiles = sizeof(DROPFILES);
	
#ifdef _UNICODE
    // If we're compiling for Unicode, set the Unicode flag in the struct to
    // indicate it contains Unicode strings.
    pDrop->fWide = TRUE;
#endif
	
    // Copy all the filenames into memory after the end of the DROPFILES struct.
    pos = lsDraggedFiles.GetHeadPosition();
    pszBuff = (TCHAR*) (LPBYTE(pDrop) + sizeof(DROPFILES));
	
    while ( NULL != pos )
	{
        lstrcpy ( pszBuff, (LPCTSTR) lsDraggedFiles.GetNext ( pos ) );
        pszBuff = 1 + _tcschr ( pszBuff, '\0' );
	}
	
    ::GlobalUnlock ( hgDrop );
	
    // Put the data in the data source.
    oleDataSource.CacheGlobalData ( CF_HDROP, hgDrop, &etc );
	
    // Add in our own custom data, so we know that the drag originated from our 
    // window.  OnDragEnter() checks for this custom format, and
    // doesn't allow the drop if it's present.  This is how we prevent the user
    // from dragging and then dropping in our own window.
    // The data will just be a dummy bool.
	HGLOBAL hgBool;
	
    hgBool = ::GlobalAlloc ( GHND | GMEM_SHARE, sizeof(bool) );
	
    if ( NULL == hgBool )
	{
        ::GlobalFree ( hgDrop );
        return;
	}
	
	static CLIPFORMAT clpFormat = (CLIPFORMAT)
		::RegisterClipboardFormat(_T("{B0D76F7A-B5D9-436c-8F10-BA16AEE69D42}"));

    // Put the data in the data source.
    etc.cfFormat = clpFormat;
    oleDataSource.CacheGlobalData(clpFormat, hgBool, &etc);
	
    // Start the drag 'n' drop!
	DROPEFFECT dwEffect = oleDataSource.DoDragDrop ( DROPEFFECT_COPY | DROPEFFECT_MOVE );
	
    // If the DnD completed OK, we remove all of the dragged items from our
    // 
    switch ( dwEffect )
	{
	case DROPEFFECT_MOVE:
		{
            // The files were copied or moved.
            // Note: Don't call ::GlobalFree() because the data will be freed by the drop target.
            for ( nSelItem = GetListCtrl().GetNextItem ( -1, LVNI_SELECTED );
			nSelItem != -1;
			nSelItem = GetListCtrl().GetNextItem ( nSelItem, LVNI_SELECTED ) )
			{
                GetListCtrl().DeleteItem ( nSelItem );
                nSelItem--;
			}
		}
	case DROPEFFECT_COPY: // fall thru.
		{
            // Resize the list columns.
//          GetListCtrl().SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
//          GetListCtrl().SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
//          GetListCtrl().SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
		}
        break;
		
	case DROPEFFECT_NONE:
		{
            // This needs special handling, because on NT, DROPEFFECT_NONE
            // is returned for move operations, instead of DROPEFFECT_MOVE.
            // See Q182219 for the details.
            // So if we're on NT, we check each selected item, and if the
            // file no longer exists, it was moved successfully and we can
            // remove it from the 
            if ((GetVersion() & 0x80000000) == 0)
			{
                bool bDeletedAnything = false;
				
                for ( nSelItem = GetListCtrl().GetNextItem ( -1, LVNI_SELECTED );
				nSelItem != -1;
				nSelItem = GetListCtrl().GetNextItem ( nSelItem, LVNI_SELECTED ) )
				{
					GetItemPath(nSelItem, sFile);
					
                    if ( 0xFFFFFFFF == GetFileAttributes ( sFile ) &&
						GetLastError() == ERROR_FILE_NOT_FOUND )
					{
                        // We couldn't read the file's attributes, and GetLastError()
                        // says the file doesn't exist, so remove the corresponding 
                        // item from the 
                        GetListCtrl().DeleteItem ( nSelItem );
						
                        nSelItem--;
                        bDeletedAnything = true;
					}
				}
				
                // Resize the list columns if we deleted any items.
                if ( bDeletedAnything )
				{
                    GetListCtrl().SetColumnWidth ( 0, LVSCW_AUTOSIZE_USEHEADER );
                    GetListCtrl().SetColumnWidth ( 1, LVSCW_AUTOSIZE_USEHEADER );
                    GetListCtrl().SetColumnWidth ( 2, LVSCW_AUTOSIZE_USEHEADER );
					
                    // Note: Don't call ::GlobalFree() because the data belongs to 
                    // the caller.
				}
                else
				{
                    // The DnD operation wasn't accepted, or was canceled, so we 
                    // should call ::GlobalFree() to clean up.
                    ::GlobalFree ( hgDrop );
                    ::GlobalFree ( hgBool );
				}
			}   // end if (NT)
            else
			{
                // We're on 9x, and a return of DROPEFFECT_NONE always means
                // that the DnD operation was aborted.  We need to free the
                // allocated memory.
                ::GlobalFree ( hgDrop );
                ::GlobalFree ( hgBool );
			}
		}
        break;  // end case DROPEFFECT_NONE
	}   // end switch
}

void CXTShellListView::OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	OnDragDrop(pNMListView);	
	*pResult = 0;
}

void CXTShellListView::OnBeginRDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	OnDragDrop(pNMListView);	
	*pResult = 0;
}
