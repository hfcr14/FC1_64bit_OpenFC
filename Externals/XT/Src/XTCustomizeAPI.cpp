// XTToolBar.cpp : implementation of the CXTToolBar class.
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
#include "XTResource.h"
#include "XTDelegate.h"
#include "XTCustomizeAPI.h"
#include "XTWndHook.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTCustomToolBar.h"
#include "XTRegistryManager.h"
#include "XTCrc32.h"
#include "XTRegistryBlob.h"

// registry entry for storing state of custom toolbars
static const LPCTSTR _customBarEntry = _T("CustomBars-Info");

///////////////////////////////////////////////////////////////////////////////////
// CXTCustomGroupItemInfo
CXTCustomGroupItemInfo::CXTCustomGroupItemInfo(Type type)
: m_nCmdID(0)
, m_type(type)
, m_pDropDown(0)
, m_bOwnsDropDown(false)
, m_nWidth(0)
{}

CXTCustomGroupItemInfo::~CXTCustomGroupItemInfo()
{
	if (m_bOwnsDropDown)
	{
		delete m_pDropDown;
	}
}

CXTCustomGroupItemInfo* CXTCustomGroupItemInfo::Clone() const
{
	CXTCustomGroupItemInfo* newInfo = new CXTCustomGroupItemInfo(m_type);
	newInfo->m_strTitle = m_strTitle;
	newInfo->m_nCmdID = m_nCmdID;
	newInfo->m_strLabel = m_strLabel;
	if (HICON hIcon = m_icon)
	{
		newInfo->m_icon = ::CopyIcon(hIcon);
	}
	if (m_pDropDown)
	{
		newInfo->m_pDropDown = new XT_DROPDOWNBUTTON(m_pDropDown);
		newInfo->m_bOwnsDropDown = true;
	}
	return newInfo;
}

///////////////////////////////////////////////////////////////////////////////////
// CXTCustomGroupInfo

CXTCustomGroupInfo::CXTCustomGroupInfo()
: m_nSortPriority(0)
{}


CXTCustomGroupInfo::~CXTCustomGroupInfo()
{
	DeleteItems();
}

void CXTCustomGroupInfo::Copy(const CXTCustomGroupInfo* other)
{
	m_nSortPriority = other->m_nSortPriority;
	DeleteItems();
	int nCount = other->m_items.GetSize();
	m_items.SetSize(nCount);
	int i;
	for (i = 0; i < nCount; i++)
	{
		m_items.SetAt(i, other->m_items[i]->Clone());
	}
}

void CXTCustomGroupInfo::DeleteItems()
{
	int i;
	for (i = 0; i < m_items.GetSize(); i++)
	{
		delete m_items[i];
	}
	m_items.RemoveAll();
}

void CXTCustomGroupInfo::DeleteItem(int nIndex)
{
	delete m_items[nIndex];
	m_items.RemoveAt(nIndex);
}

///////////////////////////////////////////////////////////////////////////////////
// CXTCustomGroups

CXTCustomGroups::CXTCustomGroups()
{ }

CXTCustomGroups::~CXTCustomGroups()
{
	RemoveAll();
}

void CXTCustomGroups::RemoveAll()
{
	int i;
	for (i = 0; i < GetSize(); i++)
	{
		delete GetAt(i);
	}
	CTypedPtrArray<CPtrArray, CXTCustomGroupInfo*>::RemoveAll();
};


///////////////////////////////////////////////////////////////////////////////////
// CXTCustomizeConnection
CXTCustomizeConnection* CXTCustomizeConnection::GetInstance()
{
	static CXTCustomizeConnection connection;
	return &connection;
}

// Removes a target 
void CXTCustomizeConnection::Remove(CObject* pTarget)
{
	ResetTracking.Remove(pTarget);
	SetCustMode.Remove(pTarget);
	GetTargets.Remove(pTarget);
	GetCustomBarInfo.Remove(pTarget);
	GetCustomGroups.Remove(pTarget);
	OnNewBar.Remove(pTarget);
	OnShowHide.Remove(pTarget);
	OnBarRenamed.Remove(pTarget);
	OnBarDestroyed.Remove(pTarget);
	GetCustomGroups.Remove(pTarget);
	GetIcons.Remove(pTarget);
	GetMenuPopup.Remove(pTarget);
	OnCmdRemoved.Remove(pTarget);
	if (SetCustMode.GetSize() == 1)
	{
		SetCustMode.Remove(this);		
	}	
}

CXTCustomizeConnection::CXTCustomizeConnection()
: m_startPoint(200, 200)
, m_bCustMode(false)
, m_bEnableCmdUI(false)
{
	IsCustCmd.Set(this, &CXTCustomizeConnection::IsCustCmdImp);
	CreateBar.Set(this, &CXTCustomizeConnection::CreateBarImp);
	PlaceBar.Set(this, &CXTCustomizeConnection::PlaceBarImp);
	LoadDynamicBars.Set(this, &CXTCustomizeConnection::LoadDynamicBarsImp);
	SaveDynamicBars.Set(this, &CXTCustomizeConnection::SaveDynamicBarsImp);
	SetCustMode.Add(this, &CXTCustomizeConnection::OnSetCustMode);
} 

// collects objects scheduled for deletion
void CXTCustomizeConnection::ScheduleGC(CObject* obj)
{
	m_garbage.Add(obj);
}

// actually flushes pending garbage
void CXTCustomizeConnection::RunGC()
{
	CObArray garbage;
	while (m_garbage.GetSize() > 0)
	{
		garbage.RemoveAll();
		garbage.Append(m_garbage);
		m_garbage.RemoveAll();
		int i; 
		for(i = 0; i < garbage.GetSize(); i++)
		{
			delete garbage[i];
		}
	}
}

void CXTCustomizeConnection::OnSetCustMode(bool bMode)
{
	m_bCustMode = bMode;
	m_bEnableCmdUI = bMode;
	if (!bMode)
	{
		RunGC();
	}
}

bool CXTCustomizeConnection::IsCustCmdImp(UINT nCmdID)
{
	return (nCmdID < ID_FILE_MRU_FIRST || nCmdID > ID_FILE_MRU_LAST) &&
		   (nCmdID < AFX_IDM_FIRST_MDICHILD || nCmdID > AFX_IDM_FIRST_MDICHILD+8) &&  // 9th is window list
		   nCmdID != XT_IDC_CHEVRON;
}

void CXTCustomizeConnection::CreateBarImp(CXTNewCustomBarRequest* pRequest)
{
	CXTCustomToolBar* pToolBar = new CXTCustomToolBar(); 
	if (!pToolBar->CreateEx(pRequest->m_pFrame,
							TBSTYLE_FLAT | TBSTYLE_LIST,
							WS_CHILD | 
							CBRS_TOP | 
							CBRS_GRIPPER | 
							CBRS_TOOLTIPS | 
							CBRS_FLYBY | 
							CBRS_SIZE_DYNAMIC, 
							CRect(0,0,0,0), 
							pRequest->m_nBarID))
	{
		TRACE0("Failed to create toolbar\n");
		AfxThrowResourceException();
	}
	pToolBar->EnableDocking(CBRS_ALIGN_ANY);
	pToolBar->SetCustomBar();
	pToolBar->SetWindowText(pRequest->m_strTitle);
	ASSERT(pRequest->m_pFrame->m_listControlBars.Find(pToolBar) != 0);
	pRequest->m_pNewBar = pToolBar;
}

class CXTCustomizeConnectionDnd : public IXTCustDropSink
{
	HCURSOR m_hOldCursor;
	CWnd* const m_pWnd;
	CTypedPtrArray<CPtrArray, IXTCustDropTarget*> m_targets;
	IXTCustDropTarget* m_currentTarget;
	CXTCustomGroupItemInfo* const m_pInfo;
	CPoint m_targetPoint;
	bool m_bMove;
	bool m_bCopyAllowed;

public:
	CXTCustomizeConnectionDnd(CWnd* pWnd, CXTCustomGroupItemInfo* pInfo)
	: m_hOldCursor(::GetCursor())
	, m_pWnd(pWnd)
	, m_currentTarget(0)
	, m_bMove(false)
	, m_bCopyAllowed(true)
	, m_targetPoint(0)
	, m_pInfo(pInfo)
	{
		pWnd->SetCapture();
		ASSERT(pWnd->m_hWnd == ::GetCapture());
	}

	~CXTCustomizeConnectionDnd()
	{
		if (m_currentTarget)
		{
			// notify drag has exited
			m_currentTarget->DragLeave();
			m_currentTarget = 0;
		}

		if (m_hOldCursor != ::GetCursor())
		{
			::SetCursor(m_hOldCursor);
		}
		if (HWND hwndCapture = ::GetCapture())
		{
			if (hwndCapture == m_pWnd->m_hWnd)
			{
				::ReleaseCapture();
			}
		}		
	}

	void Add(IXTCustDropTarget* target)
	{
		m_targets.Add(target);
	}

	// picks the current target
	void PickTarget(CPoint ptCursor)
	{
		// check to see if cursor is still over the old target
		if (m_currentTarget)
		{
			if (m_currentTarget->IsDropPoint(ptCursor))
			{
				// notify new drag position
				m_targetPoint = ptCursor;
				m_currentTarget->DragMove(ptCursor);
			}
			else
			{
				// notify drag has exited
				m_currentTarget->DragLeave();
				m_currentTarget = 0;
			}
		}
		if (!m_currentTarget)
		{
			// no current target, find one out
			int i;
			for (i = 0; i < m_targets.GetSize(); i++)
			{
				IXTCustDropTarget* target = m_targets[i];
				if (target->IsDropPoint(ptCursor))
				{
					// the drag has entered
					m_bCopyAllowed = true;
					if (target->DragEnter(m_pInfo, ptCursor, m_bCopyAllowed))
					{
						m_targetPoint = ptCursor;
						m_currentTarget = target;						
					}
					break;
				}
			}
		}

		// set the cursor as appropriate
		FreshenCursor();
	}

	void FreshenCursor()
	{
		HCURSOR cursor;
		if (!m_currentTarget)
		{
			cursor = xtAfxData.hcurDragNone;
		}
		else
		{
			cursor = (m_bMove || !m_bCopyAllowed) ? xtAfxData.hcurDragMove : xtAfxData.hcurDragCopy;
		}
		::SetCursor(cursor);
	}

	bool Run(bool canMove)
	{
		// set initial drop target
		if (canMove)
		{
			m_bMove = ::GetKeyState(VK_CONTROL) >= 0;
		}
		CPoint ptCursor;
		::GetCursorPos(&ptCursor);
		PickTarget(ptCursor);

		// run the loop
		CPoint ptLastMouse;
		bool bEatMosueMove = false;		
		bool bDone = false;
		while (!bDone && ::GetCapture() == m_pWnd->m_hWnd)
		{
			MSG msg;
        
			// assess current state
			// pump WM_PAINT first for better feedback
			while (::PeekMessage(&msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE))
			{
				if (!GetMessage(&msg, NULL, WM_PAINT, WM_PAINT))
					break;
				DispatchMessage(&msg);
			}

			// get a message
			if (!::GetMessage(&msg, NULL, 0, 0))
			{
				AfxPostQuitMessage(msg.wParam);
				return false;
			}

			if (::GetCapture() != m_pWnd->m_hWnd)
			{
				// capture was stolen while repainting
				return false;
			}

			// eat mouse move messages that are at the same mouse position
			if (msg.message != WM_MOUSEMOVE)
			{
				bEatMosueMove = false;
			}
			else
			{
				// terminate loop if we happen to get mouse move with button up
				if (!(msg.wParam & MK_LBUTTON))
				{
					// mouse move with button up - eventually, the capture was silently stolen
					// terminate the loop
					if (canMove)
					{
						m_bMove = !(msg.wParam & MK_CONTROL);
					}
					PickTarget(msg.pt);
					bDone = true;
					break;
				}

				if (bEatMosueMove && msg.pt.x == ptLastMouse.x && msg.pt.y == ptLastMouse.y )
				{
					continue;
				}
				ptLastMouse = msg.pt;
				bEatMosueMove = true;
			}
        			
			switch (msg.message)
			{
			case WM_CANCELMODE:
				// Dispatch message anyway so that window could cancel its modes
				::DispatchMessage(&msg);
				if (msg.hwnd == m_pWnd->m_hWnd)
				{
					// cancel drag-n-drop
					return false;
				}
				break;
				
			case WM_LBUTTONUP:
				// freshen target and terminate the loop
				if (canMove)
				{
					m_bMove = !(msg.wParam & MK_CONTROL);
				}
				PickTarget(msg.pt);

				// done
				bDone = true;
				break;
				
			case WM_MOUSEMOVE:
				if (canMove)
				{
					m_bMove = !(msg.wParam & MK_CONTROL);
				}
				PickTarget(msg.pt);				
				break;


			case WM_KEYDOWN:
				if (msg.wParam == VK_ESCAPE)
				{
					// cancelled
					return false;
				}
				if (canMove && msg.wParam == VK_CONTROL)
				{
					if (m_bMove)
					{
						m_bMove = false;
						FreshenCursor();
					}
				}
				break;
			case WM_KEYUP:
				if (canMove && msg.wParam == VK_CONTROL)
				{
					if (!m_bMove)
					{
						m_bMove = true;
						FreshenCursor();
					}
				}
				break;

			case WM_RBUTTONDOWN:
				// cancelled
				return false;

			case WM_TIMER:
				// prevent combos from trashing the picture
				break;
							
			default:
				// just dispatch rest of the messages
				::DispatchMessage(&msg);
				break;
			}
		}

		if (m_currentTarget)
		{
			// commit
			m_currentTarget->Drop(m_pInfo, m_targetPoint);
			m_currentTarget = 0;
		}
		return m_bMove;
	}
};

bool CXTCustomizeConnection::DragNDrop(CWnd* pWnd, 
		bool canMove, CXTCustomGroupItemInfo* pInfo)
{
	bool bMove = false;
	if (::GetCapture() == NULL)
	{
		CXTCustomizeConnectionDnd dnd(pWnd, pInfo);
		// collect all current drop targets
		GetTargets(&dnd);
		// run the loop, remember the result
		bMove = dnd.Run(canMove);
	}
	return bMove;
}
	
bool CXTCustomizeConnection::LoadDynamicBarsImp(CFrameWnd* pFrame, LPCTSTR lpszProfileName)
{
	// get data from the registry
	CXTRegistryBlob blob;
	if (!blob.Load(lpszProfileName, _customBarEntry))
	{
		return false;
	}

	// de-archive
	BYTE buffer[512];
	CArchive ar(blob.GetFile(), CArchive::load, 512, buffer);
	
	DWORD dwVersion, dwCount;
	TRY
	{
		// load version stamp
		ar >> dwVersion;
		if (dwVersion != 1)
		{
			// version mismatch
			return false;
		}

		// load toolbar count
		ar >> dwCount;
	}
	CATCH(CArchiveException, e)
	{
		// bad format
		return false;
	}
	END_CATCH

	if (dwCount)
	{
		// get used control bar IDs
		CMap<UINT, UINT, bool, bool> mapUsed;
		POSITION pos = pFrame->m_listControlBars.GetHeadPosition();
		while (pos != NULL)
		{
			CControlBar* pBar = (CControlBar*)pFrame->m_listControlBars.GetNext(pos);
			ASSERT(pBar != NULL);
			mapUsed.SetAt(pBar->GetDlgCtrlID(), true);  // actual value does not matter
		}
	
		// for each tool bar, load its ID and title
		CString strTitle;
		int i;
		for (i = 0; i < (int)dwCount; i++)
		{
			// extract single toolbar info
			UINT nID;
			TRY
			{
				ar >> nID;
				ar >> strTitle;
			}
			CATCH(CArchiveException, e)
			{
				// bad format
				return false;
			}
			END_CATCH

			// only create a toolbar if this ID is not taken already (newly added toolbar?)
			bool value;
			if (!mapUsed.Lookup(nID, value))
			{
				 // ID is not used, create the custom tool bar
				CXTNewCustomBarRequest request;
				request.m_nBarID = nID;
				request.m_pFrame = pFrame;
				request.m_strTitle = strTitle;
				CreateBar(&request);
			}
		}
	}
	return true;
}

// helper class to save info about dynamic custom bars that relate to a given frame
class CXTCustomBarSaver : public CObject
{
	// true to reset custom mode on cleanup
	const bool bResetMode;  
	CXTCustomizeConnection* const connection;
	CFrameWnd* const pFrame;
	CTypedPtrArray<CObArray, CControlBar*> bars;
	LPBYTE pData;
public:
	CXTCustomBarSaver(CXTCustomizeConnection* connection,
					  CFrameWnd* pFrame)
	: connection(connection)
	, bResetMode(!connection->IsCustMode())
	, pFrame(pFrame)
	, pData(0)
	{
		if (bResetMode)
		{
			connection->SetCustMode(true);
		}
	}

	~CXTCustomBarSaver()
	{
		if (bResetMode)
		{
			connection->SetCustMode(false);
		}
		free(pData);
		bars.RemoveAll();
	}

	void StoreCustomInfo(CXTCustomControlBarInfo* pInfo)
	{
		if (pInfo->m_bDynamic)
		{
			if (POSITION pos = pFrame->m_listControlBars.GetHeadPosition())
			{
				do
				{
					CControlBar* pBar = (CControlBar*)pFrame->m_listControlBars.GetNext(pos);
					if (pBar == pInfo->m_pBar)
					{
						bars.Add(pInfo->m_pBar);
						pos = 0;
					}
				}
				while(pos);
			}
		}
	}

	void Process(LPCTSTR lpszProfileName)
	{
		// retrieve dynamic bars
		connection->GetCustomBarInfo(STORECUSTOMINFO(this, &CXTCustomBarSaver::StoreCustomInfo));

		if (bars.GetSize() == 0)
		{
			CXTRegistryManager regManager;
			regManager.DeleteValue(lpszProfileName, _customBarEntry);
			return;
		}

		CXTRegistryBlob blob;
		CMemFile file;
		BYTE buffer[512];
		CArchive ar(blob.GetFile(), CArchive::store, 512, buffer);

		// save version stamp
		DWORD dwVersion = 1;
		ar << dwVersion;

		// save toolbar count
		DWORD dwCount = bars.GetSize();
		ar << dwCount;

		// for each tool bar, save its ID and title
		CString strTitle;
		int i;
		for (i = 0; i < bars.GetSize(); i++)
		{
			UINT nID = bars[i]->GetDlgCtrlID();
			ar << nID;
			bars[i]->GetWindowText(strTitle);
			ar << strTitle;
		}

		// done saving, flush archive
		ar.Flush();

		//commit to the registry
		blob.Save(lpszProfileName, _customBarEntry);
	}

};

bool CXTCustomizeConnection::SaveDynamicBarsImp(CFrameWnd* pFrame, LPCTSTR lpszProfileName)
{
	CXTCustomBarSaver saver(this, pFrame);
	saver.Process(lpszProfileName);
	return true;
}


void CXTCustomizeConnection::PlaceBarImp(CFrameWnd* pFrame, CControlBar* pBar)
{
	// a control bar shall be instantiated
	ASSERT(pBar != NULL);

	// the tool bar must be created at this time
	ASSERT(pBar->m_hWnd != NULL);

	// the tool bar must be invisible
	ASSERT(!pBar->IsVisible());
	
	pFrame->FloatControlBar(pBar, m_startPoint);
	pFrame->ShowControlBar(pBar, true, false);
	m_startPoint.Offset(15,15);		

	// flush pending state
	pFrame->RecalcLayout();
}
