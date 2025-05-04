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
//  Implementation of toolbar customization features
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTPriv.h"
#include "XTFunctions.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTCustomizeAPI.h"
#include "XTCallbacks.h"
#include "XTDockContext.h"
#include "XTResource.h"
#include "XTButton.h"
#include "XTTipWindow.h"
#include "XTEditListBox.h"
#include "XTCheckListBox.h"
#include "XTCommandsListBox.h"
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
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTBrowseEdit.h"
#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTCustomizePage.h"
#include "XTCustomizeSheet.h"
#include "XTSkins.h"
#include "XTMenuBar.h"
#include "XTFrameImpl.h"
#include "XTFrameWnd.h"
#include "XTMDIChildWnd.h"
#include "XTMDIFrameWnd.h"
#include "XTOleIPFrameWnd.h"
#include "XTRegistryManager.h"
#include "XTCrc32.h"
#include "XTRegistryBlob.h"
#include "XTToolbarHelpers.h"

#include "XTImageManager.h"

// this hook ensures the cursor will stay the arrow cursor when hovering imbedded controls.
class CXTTBCtrlHook : public CXTWndHook
{
public:

	CXTTBCtrlHook( CWnd* pWndToHook=0 )
	{
		ASSERT_VALID( pWndToHook );
		HookWindow( pWndToHook );
	}

    virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
	{
		switch ( message )
		{
		case WM_SETCURSOR: // eat the message.
			return 1;
		}
		return CXTWndHook::WindowProc( message, wParam, lParam );
	}
};

typedef CXTCustomizeConnection::RESETTRACKING RESETTRACKING;
typedef CXTCustomizeConnection::GETTARGETS GETTARGETS;
typedef CXTCustomizeConnection::GETCUSTOMBARINFO GETCUSTOMBARINFO;
typedef CXTControlBar::NOTEBARVISIBLE NOTEBARVISIBLE;
typedef CXTCustomizeConnection::GETCUSTOMGROUPS GETCUSTOMGROUPS;
typedef CXTCustomizeConnection::GETICONS GETICONS;
typedef CXTCustomizeConnection::SETCUSTMODE SETCUSTMODE;
typedef CXTCustomizeConnection::ONCMDCHANGE ONCMDCHANGE;
typedef CXTCustomControlBarInfo::RESET RESET;

static const int CX_DROP = 2;
static const int CY_DROP = 2;

// a threshold that triggers adding/removing separator,
// mouse movement shall be no less than this
static const int SEPTRACK_THRESHOLD = 2;

// A helper to facilitate functionality during toolbar customizations
class CXTCustTbHandler : public CObject,
						 public IXTCustDropTarget
{
	struct MOUSEMSG
	{
		UINT message;
		WPARAM wParam;
		CPoint ptMouse;

		MOUSEMSG()
		{
			memset(this, 0, sizeof(*this));
		}
	};
	
	// Mode-specific mouse handler
	typedef CXTDelegate<bool, MOUSEMSG*> ONMOUSE;
	ONMOUSE OnMouse;

	// Mode-specific invalidation
	typedef CXTMultiCastDelegate<> INVALIDATE;
	INVALIDATE Invalidate;

	// Extra rendering delegate - hooks rendering on top of the default picture
	// Takes CDC to draw upon as a parameter
	typedef CXTMultiCastDelegate<CDC*> RENDEREXTRA;	
	RENDEREXTRA RenderExtra;

	CList<CXTTBCtrlHook*,CXTTBCtrlHook*> m_hookTBCtrls;

	CRect m_rectDrop;

	// a rectangle in which to to tracking for separator addd/delete
	CRect m_rectSepTracking;
	// a rectangle in which not to draw insertion mark, valid in separator tracking mode
	CRect m_rectSepExclusion;
	// a point at which tacking for separator add/delete has started
	CPoint m_ptSepStartPoint;
	// a point at which tacking for separator add/delete has ended
	CPoint m_ptSepEndPoint;

	// back-reference
	CXTToolBar* const m_pThis;

	// signature for deferred rendering of selection 
	DWORD m_signaturePostRender;

	// position of the selected button or -1 if not selected
	int m_nSelButton;

	int m_nDropAt;

	// tells if currently tracking for separator add/delete during drag operation
	bool m_bTrackingSeparator;
	// tells if start point during separator tracking has been assigned yet
	bool m_bSepStartPoint;
	// tells if drag operation is currently executed
	bool m_bDragging;

public:
	CXTCustTbHandler(CXTToolBar* pThis)
	: m_pThis(pThis)
	, m_nSelButton(-1)
	, m_nDropAt(-1)
	, m_rectDrop(0,0,0,0)
	, m_bDragging(false)
	, m_bTrackingSeparator(false)
	, m_bSepStartPoint(false)
	, m_ptSepStartPoint(0)
	, m_ptSepEndPoint(0)
	, m_rectSepTracking(0,0,0,0)
	, m_rectSepExclusion(0,0,0,0)
	, m_signaturePostRender(0)
	{ 
		ResetState();
		CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
		connection->ResetTracking.Add(this, &CXTCustTbHandler::ResetState);
		connection->GetTargets.Add(this, &CXTCustTbHandler::ReportDropTarget);
		connection->GetCustomBarInfo.Add(this, &CXTCustTbHandler::ReportCustomBar);
		connection->OnCmdRemoved.Add(this, &CXTCustTbHandler::OnCmdRemoved);
		m_pThis->NoteBarVisible.Add(this, &CXTCustTbHandler::OnShowHide);
		m_pThis->DeferFreshenAfter();

		POSITION pos;
		for ( pos = pThis->m_listTBButtons.GetHeadPosition( ); pos; pThis->m_listTBButtons.GetNext( pos ) )
		{
			XT_TBBUTTON& tbButton = pThis->m_listTBButtons.GetAt( pos );
			if ( tbButton.pWndCtrl && ::IsWindow( tbButton.pWndCtrl->m_hWnd ) )
			{
				CXTTBCtrlHook* pctrlHook = new CXTTBCtrlHook( tbButton.pWndCtrl );
				m_hookTBCtrls.AddHead( pctrlHook );
			}
		}
	}

	~CXTCustTbHandler()
	{
		m_pThis->NoteBarVisible.Remove(this);
		CXTCustomizeConnection::GetInstance()->Remove(this);		
		m_pThis->DeferFreshenAfter();
		InvalidateRect(NULL);

		while ( !m_hookTBCtrls.IsEmpty( ) )
		{
			CXTTBCtrlHook* pctrlHook = m_hookTBCtrls.RemoveHead( );
			SAFE_DELETE( pctrlHook );
		}
	}

	void InvalidateRect(LPRECT lpRect)
	{
		m_pThis->RedrawWindow(lpRect, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_FRAME);
		DeferRenderExtra();
	}

	void RedrawRect(LPRECT lpRect)
	{
		m_pThis->RedrawWindow(lpRect, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_FRAME | RDW_UPDATENOW);
		DoRenderExtra();
	}


	void OnShowHide(bool bVisible)
	{
		CXTCustomizeConnection::GetInstance()->OnShowHide(m_pThis, bVisible);
		m_pThis->DeferFreshenAfter();
	}

	void ResetState()
	{
		OnMouse = ONMOUSE(this, &CXTCustTbHandler::OnMouseDefault);
		RenderExtra.Remove(this);
		InvalidateRect(NULL);
		Invalidate.RemoveAll();
		m_nSelButton = -1;
	}

	void OnTrackStateChange(CXTDockContext* pCtx)
	{
		ResetState();
		if (pCtx->IsTracking())
		{
			OnMouse = ONMOUSE(this, &CXTCustTbHandler::OnMouseToolbarDrag);
		}
	}

	bool PreTranslateMessage(MSG* pMsg) 
	{
		switch (pMsg->message)
		{
			case WM_PAINT:
				// defer rendering the selection
				DeferRenderExtra();
				return false;
			case WM_TIMER:
				if (pMsg->hwnd != m_pThis->m_hWnd)
				{			
					::KillTimer(pMsg->hwnd, pMsg->wParam);
					DeferRenderExtra();
					return true;
				}
				return false;
		}

		// eat all mouse messages
		if (WM_MOUSEFIRST <= pMsg->message && pMsg->message <= WM_MOUSELAST)
		{
			MOUSEMSG msg;
			msg.message = pMsg->message;
			msg.wParam = pMsg->wParam;
			msg.ptMouse = CPoint(pMsg->lParam);
			if (pMsg->hwnd != m_pThis->m_hWnd)
			{
				// translate to proper client coordinates
				::ClientToScreen(pMsg->hwnd, &msg.ptMouse);
				m_pThis->ScreenToClient(&msg.ptMouse);
			}
			OnMouse(&msg);
			return true;
		}

		return false;
	}

	void DeferRenderExtra()
	{
		m_pThis->PostMessage(XTWM_POSTRENDER, 0, ++m_signaturePostRender);
	}

	bool WindowProc(LRESULT& result, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (WM_MOUSEFIRST <= message && message <= WM_MOUSELAST)
		{
			MOUSEMSG msg;
			msg.message = message;
			msg.wParam = wParam;
			msg.ptMouse = CPoint(lParam);
			if (OnMouse(&msg))
			{
				result = 0;
				return true;
			}
			return false;
		}
		switch (message)
		{
			case XTWM_POSTRENDER:
				if (m_signaturePostRender == (DWORD)lParam)
				{
					// handle rendering of the selection
					DoRenderExtra();
				}
				return true;
			case WM_MOUSEACTIVATE:
				result = MA_NOACTIVATE;
				return true;				

			case WM_SETTEXT:
			{
				LPCTSTR pszText = (LPCTSTR)(LPVOID)lParam;
				CXTCustomizeConnection::GetInstance()->OnBarRenamed(m_pThis, pszText);
				// returns false for the owner to process this message
				break;
			}

			case WM_DESTROY:
			{				
				// force the frame to recalc
				CFrameWnd* pFrame = m_pThis->GetDockingFrame();
				pFrame->DelayRecalcLayout();
				pFrame->PostMessage(WM_IDLEUPDATECMDUI);

				// notify the toolbar is destroyed
				CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
				connection->Remove(m_pThis);
				connection->OnBarDestroyed(m_pThis);

				// close up any pending customizations
				m_pThis->SetCustMode(false);
				
				// returns false for the owner to cleanup as well
				break;
			}
			case WM_CANCELMODE:
				ResetState();
				// returns false for the owner to cleanup as well
				break; 
		}
		return false;
	}

	void DoRenderExtra()
	{
		if (RenderExtra.GetSize() > 0)
		{
			CWindowDC dc(m_pThis);
			CRect rectWindow;
			m_pThis->GetWindowRect(&rectWindow);
			CRect rectClient;
			m_pThis->GetClientRect(&rectClient);
			m_pThis->ClientToScreen(&rectClient);
			CPoint windowOrg = rectWindow.TopLeft() - rectClient.TopLeft();
			dc.SetWindowOrg(windowOrg);
			RenderExtra(&dc);
		}
	}

	// handler for mouse messages when toolbar is being dragged/resized
	bool OnMouseToolbarDrag(MOUSEMSG* /*pMsg*/)
	{
		// pass all messages thru
		return false;
	}

	// handler for mouse messages in default mode
	bool OnMouseDefault(MOUSEMSG* pMsg)
	{
		int buttonPos = m_pThis->LocateHit(pMsg->ptMouse);
		if (buttonPos >= 0)
		{
			switch (pMsg->message)
			{
			case WM_LBUTTONDOWN:
				if (buttonPos == m_nSelButton)
				{
					// Already selected, start drag operation
					StartDrag();
				}
				else
				{
					// process as a click first
					SelectButton(buttonPos);
					// process as a drag or click op
					ProcessDragOrClick(pMsg->ptMouse);
				}
			}
		}

		return buttonPos >= 0;
	}

	void ProcessDragOrClick(CPoint ptMouse)
	{
		// DragDetect freezes for a bit so force window update now
		m_pThis->SetCapture();
		RedrawRect(NULL);
		// check to see if drag has started
		bool bDrag = ::DragDetect(m_pThis->m_hWnd, ptMouse) != 0;
		::ReleaseCapture();
		if (bDrag)
		{
			StartDrag();
		}
	}

	void ReportCustomBar(const STORECUSTOMINFO& Store)
	{
		CXTCustomControlBarInfo info;
		info.m_pBar = m_pThis;
		info.m_bCanShowHide = true;
		m_pThis->GetCustomBarInfo(&info);
		Store(&info);
	}

	void ReportDropTarget(IXTCustDropSink* pSink)
	{
		pSink->Add(this);
	}

	virtual bool IsDropPoint(CPoint ptCursor)
	{
		if (::WindowFromPoint(ptCursor) != m_pThis->m_hWnd)
		{
			return false;
		}

		CRect rectWindow;
		m_pThis->GetWindowRect(&rectWindow);
		if (rectWindow.PtInRect(ptCursor))
		{
			m_pThis->ScreenToClient(&ptCursor);

			// freshen  current state of separator add/delete tracking, if applicable
			if (m_bTrackingSeparator)
			{
				if (m_rectSepTracking.PtInRect(ptCursor))
				{
					// still dragging within starting rectangle
					if (!m_bSepStartPoint)
					{
						m_bSepStartPoint = true;
						m_ptSepStartPoint = ptCursor;
					}
					m_ptSepEndPoint = ptCursor;
				}
				else
				{
					// cursor has left tracking rectangle,
					// this ends tracking for separator addition
					m_bTrackingSeparator = false;
				}
			}

			CRect rc;
			m_pThis->GetClientRect(&rc);
			return rc.PtInRect(ptCursor) != 0;
		}
		return false;
	}

	bool IsBarHorizontal() const
	{
		bool bHorz = (m_pThis->m_dwStyle & CBRS_ORIENT_VERT) == 0 ||
		             (m_pThis->m_dwStyle & CBRS_FLOATING);
		return bHorz;
	}
	
	// a helper to locate the drop area
	void LocateDropArea(CPoint ptCursor, int& nDropAt, CRect& rectDrop)
	{
		const bool bHorz = IsBarHorizontal();
		const int nCount = m_pThis->GetButtonCount();

		CRect client;
		m_pThis->GetClientRect(&client);
		
		// find out the suitable button to insert before/after			
		if (nCount == 0)
		{
			// no buttons, insert at position 0
			nDropAt = 0;
			if (bHorz)
			{
				rectDrop = CRect(0,0, 2 * CX_DROP, client.bottom);
			}
			else
			{
				rectDrop = CRect(0,0, client.right, 2 * CY_DROP);
			}
		}
		else
		{
			// convert to client
			m_pThis->ScreenToClient(&ptCursor);

			nDropAt = -1;
			int minDist = 0;
			int i;
			for (i = 0; i < nCount; i++)
			{				
				TBBUTTON button;
				m_pThis->GetButton(i, &button);

				if (button.fsState & TBSTATE_HIDDEN)
				{
					continue;
				}
				CRect itemRect;
				m_pThis->GetItemRect(i, &itemRect);
				int centerX = (itemRect.left + itemRect.right) / 2;
				int centerY = (itemRect.top + itemRect.bottom) / 2;
				int dx = ptCursor.x - centerX;
				int dy = ptCursor.y - centerY;
				int dist = dx * dx + dy * dy;
				if (nDropAt == -1 || dist < minDist)
				{
					minDist = dist;
					nDropAt = i;
				}
			}
			if (nDropAt >= 0)
			{
				CRect client;
				m_pThis->GetClientRect(&client);
				CRect itemRect;
				m_pThis->GetItemRect(nDropAt, &itemRect);				
				if (bHorz)
				{
					if (abs(ptCursor.x - itemRect.left) < abs(ptCursor.x - itemRect.right))
					{
						// drop at left
						rectDrop = CRect(itemRect.left - CX_DROP,
									   itemRect.top, 
									   itemRect.left + CX_DROP,
									   itemRect.bottom);
						if (rectDrop.left < client.left)
						{
							rectDrop.OffsetRect(client.left - rectDrop.left, 0);
						}
					}
					else
					{
						// drop at right
						nDropAt++;
						rectDrop = CRect(itemRect.right - CX_DROP,
										   itemRect.top, 
										   itemRect.right + CX_DROP,
										   itemRect.bottom);
						if (rectDrop.right > client.right)
						{
							rectDrop.OffsetRect(client.right - rectDrop.right, 0);
						}
					}
				}
				else
				{
					if (abs(ptCursor.y - itemRect.top) < abs(ptCursor.y - itemRect.bottom))
					{
						// drop at top					
						rectDrop = CRect(itemRect.left,
									   itemRect.top  - CY_DROP, 
									   itemRect.right, 
									   itemRect.top + CY_DROP);
						if (rectDrop.top < client.top)
						{
							rectDrop.OffsetRect(0, client.top - rectDrop.top);
						}
					}
					else
					{
						// drop at bottom
						nDropAt++;
						rectDrop = CRect(itemRect.left,
									   itemRect.bottom  - CY_DROP, 
									   itemRect.right, 
									   itemRect.bottom + CY_DROP);
						if (rectDrop.bottom > client.bottom)
						{
							rectDrop.OffsetRect(0, client.bottom - rectDrop.bottom);
						}
					}
				}
			}
			// do not draw insertion marks if it overlaps with selected button rect
			if (!rectDrop.IsRectEmpty() && m_nSelButton >= 0)
			{
				CRect rect;
				GetActiveItemRect(&rect);
				if (!rect.IsRectEmpty())
				{
					if (!(rectDrop & rect).IsRectEmpty())
					{
						rectDrop.SetRectEmpty();
					}
				}
			}
			// do not draw insertion mark if separator tracking is on
			// and the point is within its exclusion rectangle
			if (m_bTrackingSeparator && !m_rectSepExclusion.IsRectEmpty())
			{
				if (!(rectDrop & m_rectSepExclusion).IsRectEmpty())
				{
					rectDrop.SetRectEmpty();
				}
			}
		}
	}

	void InvalidateDropArea()
	{
		CRect rect = m_rectDrop;
		rect.InflateRect(2,2);
		InvalidateRect(&rect);
	}

	void RenderDropArea(CDC* pDC)
	{
		if (!m_rectDrop.IsRectEmpty())
		{
			bool bHorz = IsBarHorizontal();
			if (bHorz)
			{
				int iLeft   = m_rectDrop.left + 1;
				int iTop    = m_rectDrop.top;
				int iBottom = m_rectDrop.bottom;

				pDC->FillSolidRect(iLeft, iTop, 2, m_rectDrop.Height(), 0);

				pDC->MoveTo(iLeft-2, iTop);
				pDC->LineTo(iLeft+4, iTop);
				pDC->MoveTo(iLeft-1, iTop+1);
				pDC->LineTo(iLeft+3, iTop+1);

				pDC->MoveTo(iLeft-2, iBottom-1);
				pDC->LineTo(iLeft+4, iBottom-1);
				pDC->MoveTo(iLeft-1, iBottom-2);
				pDC->LineTo(iLeft+3, iBottom-2);

			}
			else
			{
				int iRight = m_rectDrop.right;
				int iLeft  = m_rectDrop.left;
				int iTop   = m_rectDrop.top + 1;

				pDC->FillSolidRect(iLeft+1, iTop, m_rectDrop.Width(), 2, 0);

				pDC->MoveTo(iLeft+1, iTop-2);
				pDC->LineTo(iLeft+1, iTop+4);
				pDC->MoveTo(iLeft+2, iTop-1);
				pDC->LineTo(iLeft+2, iTop+3);

				pDC->MoveTo(iRight-1, iTop-2);
				pDC->LineTo(iRight-1, iTop+4);
				pDC->MoveTo(iRight-2, iTop-1);
				pDC->LineTo(iRight-2, iTop+3);
			}
		}
	}

	// drag has entered this drop target
	virtual bool DragEnter(CXTCustomGroupItemInfo* pInfo, CPoint ptCursor,
			bool& bCopyAllowed)
	{
		switch (pInfo->m_type)
		{
			case CXTCustomGroupItemInfo::Button: 
			{
				// a command ?
				if (pInfo->m_nCmdID)  
				{
					// if this toolbar already contains a command, then
					// only allow it if copying from the same toolbar
					int nIndex = m_pThis->CommandToIndex(pInfo->m_nCmdID);
					if (nIndex >= 0 && !m_bDragging)
					{
						return false;
					}
				}
				else
				{
					// is it a menubar dropdown?
					if (!pInfo->m_pDropDown ||
						!pInfo->m_pDropDown->bMenuBarPopup)
					{
						return false;
					}
				}
				break;
			}
			case CXTCustomGroupItemInfo::Control: 
			{
				// can accept controls only if moved within the same toolbar
				if (!m_bDragging)
				{
					return false;
				}
				break;
			}
			default:
				return false;
		}

		// can only move the button if drag is within the same toolbar
		bCopyAllowed = !m_bDragging;

		// locate where to render
		LocateDropArea(ptCursor, m_nDropAt, m_rectDrop);
	
		// install renderer
		RenderExtra.Add(this, &CXTCustTbHandler::RenderDropArea);
		Invalidate.Add(this, &CXTCustTbHandler::InvalidateDropArea);

		// invalidate initially for visual feedback
		InvalidateDropArea();
		
		return true;
	}

	// Mouse is still over the target but position has changed
	virtual void DragMove(CPoint ptCursor)
	{
		// locate where to render
		int nDropAt;
		CRect rectDrop;
		LocateDropArea(ptCursor, nDropAt, rectDrop);

		// invalidate if changed, reassign and invalidate at new position
		if (m_nDropAt != nDropAt)
		{
			InvalidateDropArea();
			m_nDropAt = nDropAt;
			m_rectDrop = rectDrop;
			InvalidateDropArea();
		}
	}

	// mouse has left the building
	virtual void DragLeave()
	{
		InvalidateDropArea();
		m_nDropAt = -1;
		m_rectDrop.SetRectEmpty();
		RenderExtra.Remove(this, &CXTCustTbHandler::RenderDropArea);
		Invalidate.Remove(this, &CXTCustTbHandler::InvalidateDropArea);
	}

	// mouse dropped, must accept the data
	virtual void Drop(CXTCustomGroupItemInfo* pInfo, CPoint ptCursor)
	{
		// locate where to drop
		LocateDropArea(ptCursor, m_nDropAt, m_rectDrop);

		// remember if this toolbar was wrapped
		bool bWrapped = false;
		int i;
		int nCount = m_pThis->GetButtonCount();
		for (i = 0; i < nCount - 1 && ! bWrapped; i++)
		{
			TBBUTTON button;
			VERIFY(m_pThis->GetButton(i, &button));
			bWrapped = (button.fsState & TBSTATE_WRAP) != 0;
		}

		m_pThis->ResetHiddenButtons();

				
		if (m_bTrackingSeparator)
		{
			m_nDropAt = m_nSelButton;
			m_bTrackingSeparator = false;
			m_bDragging = false;
			// the cursor has not left the original item rectangle, 
			// check to see if we have to add a separator
			if (m_bSepStartPoint)
			{
				CSize delta = m_ptSepEndPoint - m_ptSepStartPoint;
				bool bRemove = false;
				bool bAdd = false;
				if (IsBarHorizontal())
				{
					if (delta.cx > SEPTRACK_THRESHOLD)
					{
						bAdd = true;
					}
					else
					if (delta.cx < -SEPTRACK_THRESHOLD)
					{
						bRemove = true;
					}
				}
				else
				{
					if (delta.cy > SEPTRACK_THRESHOLD)
					{
						bAdd = true;
					}
					else
					if (delta.cy < -SEPTRACK_THRESHOLD)
					{
						bRemove = true;
					}
				}
				if (bAdd)
				{
					if (m_nSelButton > 0)
					{
						// only add separator in between two buttons
						if (m_pThis->IsSeparator(m_nSelButton - 1))
						{
							bAdd = false;
						}
					}
					else
					{
						// do not add a separator in front of the first button
						bAdd = false;
					}
					if (bAdd)
					{
						TBBUTTON button;
						memset(&button, 0, sizeof(button));
						button.fsStyle = TBSTYLE_SEP;
						VERIFY(m_pThis->SendMessage(TB_INSERTBUTTON, m_nSelButton, (LPARAM)&button));
						m_nSelButton++;
						m_nDropAt++;
						m_pThis->m_bDelayedButtonLayout = TRUE;
						m_pThis->DeferFreshenAfter();
					}					
				}
				if (bRemove && m_nSelButton > 0)
				{
					if (m_pThis->IsSeparator(m_nSelButton - 1))
					{
						VERIFY(m_pThis->SendMessage(TB_DELETEBUTTON, m_nSelButton - 1, 0));
						m_pThis->m_bDelayedButtonLayout = TRUE;
						m_pThis->DeferFreshenAfter();
						if (m_nDropAt >= m_nSelButton)
						{
							m_nDropAt--;
						}
						m_nSelButton--;
					}					
				}
			}
		}
		else
		{
			// a command ?		
			if (pInfo->m_nCmdID)
			{
				// if dragging within the same toolbar, the simply readjust button order
				int nIndex = m_pThis->CommandToIndex(pInfo->m_nCmdID);
				if (nIndex >= 0)
				{
					// attempt to drop a known command shall be rejected in dragenter()
					ASSERT(m_bDragging);
					// this tells not to handle removing the source
					m_bDragging = false;

					// do something if the order actually changes
					if (nIndex != m_nDropAt)
					{
						TBBUTTON button;
						VERIFY(m_pThis->GetButton(nIndex, &button));
						VERIFY(m_pThis->SendMessage(TB_DELETEBUTTON, nIndex, 0));
						if (nIndex < m_nDropAt)
						{
							m_nDropAt--;
						}
						VERIFY(m_pThis->SendMessage(TB_INSERTBUTTON, m_nDropAt, (LPARAM)&button));
						m_pThis->m_bDelayedButtonLayout = TRUE;
						m_pThis->DeferFreshenAfter();
					}
				}
				else
				{
					// do the data drop
					m_pThis->DropCustomItemInfo(pInfo, m_nDropAt);
				}
			}
			else
			{			
				m_pThis->DropCustomItemInfo(pInfo, m_nDropAt);			
			}
		}

		// first and last item in the toolbar cannot be a separator
		RemoveFirstLastSep();
		
		// cleanup the state
		int nSelButton = m_nDropAt;
		DragLeave();
		CXTCustomizeConnection::GetInstance()->ResetTracking();
		SelectButton(nSelButton);
		if (m_pThis->m_dwStyle & CBRS_FLOATING)
		{
			// if toolbar is not wrapped, then recalc toolbar width
			// to keep it from wrapping to fit into the old width
			if (!bWrapped)
			{
				UINT nWidth = m_pThis->CalcFixedLayout(true, true).cx;
				if (m_pThis->m_nMRUWidth != nWidth)
				{
					m_pThis->m_nMRUWidth = nWidth;
					m_pThis->DeferFreshenAfter();
				}
			}
		}

		// ideal size has changed
		m_pThis->NoteIdealSizeChanged();
	}

	// first and last item in the toolbar cannot be a separator
	void RemoveFirstLastSep()
	{
		if (int buttonCount = m_pThis->GetButtonCount())
		{
			bool bChanged = false;
			if (m_pThis->IsSeparator(0))
			{
				VERIFY(m_pThis->SendMessage(TB_DELETEBUTTON, 0, 0));
				buttonCount--;
				m_nDropAt--;
				bChanged = true;
			}			
			if (buttonCount > 0)
			{
				int nIndex = buttonCount - 1;
				if (m_pThis->IsSeparator(nIndex))
				{
					VERIFY(m_pThis->SendMessage(TB_DELETEBUTTON, nIndex, 0));
					bChanged = true;
				}
			}
			if (bChanged)
			{
				m_pThis->m_bDelayedButtonLayout = TRUE;
				m_pThis->DeferFreshenAfter();
			}
		}
	}	

	// starts dragging currently selected button
	void StartDrag()
	{		
		if (CXTCustomGroupItemInfo* pInfo = m_pThis->GetCustomItemInfo(m_nSelButton))
		{
			m_bDragging = true;
			m_bTrackingSeparator = true;
			m_bSepStartPoint = false;
			m_ptSepStartPoint = 0;
			m_ptSepEndPoint = 0;
			m_pThis->GetItemRect(m_nSelButton, &m_rectSepTracking);			
			if (IsBarHorizontal())
			{
				int extra = m_pThis->m_sizeImage.cx / 2;
				int exclusionLeft = m_rectSepTracking.right;
				m_rectSepTracking.InflateRect(extra, 0);
				m_rectSepExclusion = m_rectSepTracking;
				m_rectSepExclusion.left = exclusionLeft;
			}
			else
			{
				int extra = m_pThis->m_sizeImage.cy / 2;
				int exclusionBottom = m_rectSepTracking.bottom;
				m_rectSepTracking.InflateRect(0, extra);
				m_rectSepExclusion = m_rectSepTracking;
				m_rectSepExclusion.top = exclusionBottom;
			}

			int nOldSelButton = m_nSelButton;
			if (CXTCustomizeConnection::GetInstance()->DragNDrop(m_pThis, true, pInfo))
			{
				if (m_bDragging)
				{
					// remove the button if not handled already
					if (m_nSelButton >= 0 && m_nSelButton < nOldSelButton)
					{
						nOldSelButton++;
					}
					m_pThis->SendMessage(TB_DELETEBUTTON, nOldSelButton);
					m_pThis->m_bDelayedButtonLayout = true;
					
					// update the state
					m_pThis->DeferFreshenAfter();
					// ideal size has changed
					m_pThis->ResetHiddenButtons();
					m_pThis->NoteIdealSizeChanged();
					ResetState();
				}
				// check to see if we now have two adjacent separators,
				// collapse them into one if so
				CollapseAdjacentSeparators(nOldSelButton);

				// first and last item cannot be a separator
				RemoveFirstLastSep();					
			}
			delete pInfo;
			m_bDragging = false;
			m_bTrackingSeparator = false;
		}
	}

	// collapses two adjacent separators into one, if applicable
	void CollapseAdjacentSeparators(int nIndex)
	{
		if (int buttonCount = m_pThis->GetButtonCount())
		{
			if (nIndex > 0 && nIndex < buttonCount)
			{
				if (m_pThis->IsSeparator(nIndex))
				{					
					if (m_pThis->IsSeparator(nIndex - 1))
					{
						// one too many, remove the separator
						m_pThis->SendMessage(TB_DELETEBUTTON, nIndex);
						m_pThis->m_bDelayedButtonLayout = true;
						m_pThis->DeferFreshenAfter();
					}
				}
			}			
		}
	}

	void SelectButton(int buttonPos)
	{
		CXTCustomizeConnection::GetInstance()->ResetTracking();   // resets own state as well
		m_nSelButton = buttonPos;
		ASSERT(m_nSelButton >= 0);		
		RenderExtra.Add(this, &CXTCustTbHandler::RenderActiveItem);
		Invalidate.Add(this, &CXTCustTbHandler::InvalidateActiveItem);
		InvalidateRect(NULL);
	}

	// Gets rectangle of a currently clicked item
	void GetActiveItemRect(CRect* rect)
	{
		ASSERT(m_nSelButton >= 0);
		m_pThis->GetItemRect(m_nSelButton, rect);
	}

	// invalidates rectangle of the currently selected button
	void InvalidateActiveItem()
	{
		CRect rect;
		GetActiveItemRect(&rect);
		InvalidateRect(&rect);
	}

    CWnd* GetControl()
    {
        // get the selected button.
        TBBUTTON button;
        m_pThis->GetButton( m_nSelButton, &button );

        // loop thru the toolbar button list and return the pWndCtrl if any.
        POSITION pos;
        for (pos = m_pThis->m_listTBButtons.GetHeadPosition(); pos != NULL; m_pThis->m_listTBButtons.GetNext(pos))
        {
            XT_TBBUTTON& tbButton = m_pThis->m_listTBButtons.GetAt(pos);
            if (tbButton.idCommand == button.idCommand)
            {
                return tbButton.pWndCtrl;
            }
        }
        return NULL;
    }

	// Draws selection on top of a clicked-upon button
	void RenderActiveItem(CDC* pDC)
	{
		CRect rect;
		GetActiveItemRect(&rect);
        
        // see if this button has a control, we will us the controls 
        // rect instead of the button rect.
        CWnd* pWndCtrl = GetControl();
        if ( pWndCtrl && ::IsWindow( pWndCtrl->m_hWnd ) )
        {
            pWndCtrl->GetWindowRect( &rect );
            m_pThis->ScreenToClient( &rect );
            rect.InflateRect(1,1);
        }

		pDC->Draw3dRect(&rect, 0, 0);
		rect.DeflateRect(1,1);
		pDC->Draw3dRect(&rect, 0, 0);
	}

	void OnCmdRemoved(UINT nCmdID)
	{
		int buttonCount = m_pThis->GetButtonCount();
		int i;
		for (i = 0; i < buttonCount; i++)
		{
			TBBUTTON button;
			m_pThis->GetButton(i, &button);
			if (button.idCommand == (int)nCmdID)
			{
				VERIFY(m_pThis->SendMessage(TB_DELETEBUTTON, i, 0));
				m_pThis->m_bDelayedButtonLayout = TRUE;
				m_pThis->DeferFreshenAfter();
				if (m_nSelButton == i)
				{
					ResetState();
				}
				break;
			}
		}
	}
};

void CXTToolBar::EnableCustomization(bool bEnable) 
{
	if (m_bCustomize != bEnable)
	{
		m_bCustomize = bEnable;
		if (m_bCustomize)
		{
			CXTCustomizeConnection::GetInstance()->
				SetCustMode.Add(this, &CXTToolBar::SetCustMode);
		}
		else
		{
			CXTCustomizeConnection::GetInstance()->Remove(this);
		}
	}
}

void CXTToolBar::SetCustMode(bool bMode)
{
	// only do something if the mode actually changes
	if (GetCustomBar())
	{
		bool bCustomMode = m_pCustomHandler != 0;
		if (bCustomMode != bMode)
		{
			if (bMode)
			{
				IXTSkin::GetInstance()->NotifyToolbarTracking(true);
				m_pCustomHandler = new CXTCustTbHandler(this);
				if (m_pDockContext)
				{
					CXTDockContext* pCtx = (CXTDockContext*)m_pDockContext;
					pCtx->AddTrackStateListener(
						m_pCustomHandler, 
						CXTDockContext::LISTENERCB(&CXTCustTbHandler::OnTrackStateChange));
				}
			}
			else
			{
				IXTSkin::GetInstance()->NotifyToolbarTracking(false);
				if (m_pDockContext)
				{
					CXTDockContext* pCtx = (CXTDockContext*)m_pDockContext;
					pCtx->RemoveListener(m_pCustomHandler);
				}
				delete m_pCustomHandler;
				m_pCustomHandler = 0;
			}
		}
	}
}

BOOL CXTToolBar::PreTranslateMessage(MSG* pMsg) 
{
	if (m_pCustomHandler &&	m_pCustomHandler->PreTranslateMessage(pMsg))
	{
		return true;
	}

	// check to see if the drag has to be started	
	if (m_hWnd != pMsg->hwnd && pMsg->message == WM_LBUTTONDOWN)
	{
		// translate to proper client coordinates
		CPoint ptMouse(pMsg->lParam);		
		::ClientToScreen(pMsg->hwnd, &ptMouse);
		ScreenToClient(&ptMouse);
		if (StartCustomDrag(ptMouse))
		{
			return true;
		}
	}

	return CXTControlBar::PreTranslateMessage(pMsg);
}

LRESULT CXTToolBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (m_pCustomHandler)
	{
		LRESULT lResult;
		if (m_pCustomHandler->WindowProc(lResult, message, wParam, lParam))
		{
			return lResult;
		}
	}
	return CXTControlBar::WindowProc(message, wParam, lParam);
}

void CXTToolBar::Customize() 
{
	ASSERT(::IsWindow(m_hWnd)); 

	CFrameWnd* pFrameWnd = GetDockingFrame( );
	if ( pFrameWnd != NULL && pFrameWnd->IsKindOf( RUNTIME_CLASS( CMiniDockFrameWnd ) ) )
	{
		pFrameWnd = pFrameWnd->GetParentFrame( );
	}

	if ( pFrameWnd != NULL )
	{
		if ( pFrameWnd->IsKindOf( RUNTIME_CLASS( CXTOleIPFrameWnd ) ) )
		{
			( ( CXTOleIPFrameWnd* )pFrameWnd )->Customize();
		}
		else if ( pFrameWnd->IsKindOf( RUNTIME_CLASS( CXTMDIChildWnd ) ) )
		{
			( ( CXTMDIChildWnd* )pFrameWnd )->Customize();
		}
		else if ( pFrameWnd->IsKindOf( RUNTIME_CLASS( CXTMDIFrameWnd ) ) )
		{
			( ( CXTMDIFrameWnd* )pFrameWnd )->Customize();
		}
		else if ( pFrameWnd->IsKindOf( RUNTIME_CLASS( CXTFrameWnd ) ) )
		{
			( ( CXTFrameWnd* )pFrameWnd )->Customize();
		}
	}
}

void CXTToolBar::GetCustomBarInfo(CXTCustomControlBarInfo* pInfo)
{
	pInfo->m_bCanShowHide = true;
	pInfo->m_bDynamic = false;
	pInfo->Reset.Set(this, &CXTToolBar::ResetCustomizations);
}

void CXTToolBar::ResetCustomizations()
{
	// get the window that has input focus ( should be reset button )
	CWnd* pWnd = GetFocus();

	CString strMessage;
	XT_MANAGE_STATE_BEGIN  // Switch to toolkit resources/module state
	strMessage.LoadString(XT_IDS_RESTORE);
	XT_MANAGE_STATE_END

	if (pWnd->MessageBox(strMessage, NULL, MB_YESNO|MB_ICONWARNING) == IDYES)
	{
		DoRestoreState();
	}
}

void CXTToolBar::DoRestoreState()
{
	// must be defined by now
	ASSERT(m_pvOriginalState);
	
	ResetHiddenButtons();
	SetCurrentBarState(m_pvOriginalState, m_cbOriginalState);

	// reset state of the custom mode handler if available
	if (m_pCustomHandler)
	{
		m_pCustomHandler->ResetState();
	}

	// resize the toolbar.
	AutoSizeToolbar();
	
	// move the controls to thier correct locations.
	PositionControls();
	
	// force the frame window to recalculate the size
	DeferFreshenAfter();

	// ideal size has changed
	NoteIdealSizeChanged();
}

void CXTToolBar::NoteIdealSizeChanged()
{
	if (!m_bIdealSizeChanged)
	{
		m_bIdealSizeChanged = true;
		DeferFreshenAfter();
	}
}

CXTCustomGroupItemInfo* CXTToolBar::GetCustomItemInfo(const TBBUTTON& button)
{
	CXTCustomGroupItemInfo* pInfo = 0;
	if (!IsSeparator(button) && button.idCommand)
	{
		if (IsControl(button))
		{
			pInfo = GetCustomControlInfo(button);
		}
		else
		{
			pInfo = GetCustomButtonInfo(button);
		}
	}
	return pInfo;
}

CXTCustomGroupItemInfo* CXTToolBar::GetCustomControlInfo(const TBBUTTON& button)
{
	CXTCustomGroupItemInfo::Type type = CXTCustomGroupItemInfo::Control;
	CXTCustomGroupItemInfo* pInfo = new CXTCustomGroupItemInfo(type);
	pInfo->m_nCmdID = button.idCommand;
	pInfo->m_nWidth = button.iBitmap;
	return pInfo;
}

CXTCustomGroupItemInfo* CXTToolBar::GetCustomButtonInfo(const TBBUTTON& button)
{
	CString strMenuText;
	strMenuText.LoadString(button.idCommand);

	if (strMenuText.IsEmpty()) {
		strMenuText.Format(XT_IDS_NOHELPTOPIC, button.idCommand);
	}

	CString strLeftText = strMenuText;
	CString strRightText;
	int iTabPos = strLeftText.Find(_T('\n'));

	if (iTabPos >= 0)
	{
		strRightText = strLeftText.Right(strLeftText.GetLength()-iTabPos-1);
		strLeftText  = strLeftText.Left(iTabPos);
	}

	if (strRightText.IsEmpty())
		strRightText = strLeftText;

	// create info fro transferring the data
	CXTCustomGroupItemInfo::Type type = CXTCustomGroupItemInfo::Button;
	CXTCustomGroupItemInfo* pInfo = new CXTCustomGroupItemInfo(type);
	pInfo->m_nCmdID = button.idCommand;
	pInfo->m_strTitle = strRightText;
	if (button.iBitmap >= 0)
	{
		pInfo->m_icon = GetToolBarCtrl().GetImageList()->ExtractIcon(button.iBitmap);
	}

	return pInfo;
}

CXTCustomGroupItemInfo* CXTToolBar::GetCustomItemInfo(int nIndex)
{
	TBBUTTON button;
	GetButton(nIndex, &button);

	if (CXTCustomGroupItemInfo* pInfo = GetCustomItemInfo(button))
	{
		// attach dropdown info if present
		if (XT_DROPDOWNBUTTON* pDropButton = FindDropDownButton(button.idCommand))
		{
			pInfo->m_pDropDown = pDropButton;
		}

		// attach text
		if (button.iString >= 0)
		{
			pInfo->m_strLabel = GetButtonText(nIndex);
		}
		return pInfo;
	}
	
	return 0;
}

// sets TBSTYLE_LIST if the toolbar does not have any texts in buttons
void CXTToolBar::SetListStyleIfNoText()
{
	int nCount = GetButtonCount();
	int i;
	for (i = 0; i < nCount; i++)
	{
		TBBUTTON button;
		GetButton(i, &button);
		if (button.iString >= 0)
		{
			// a string has been assigned
			return;
		}
	}
	// assign TBSTYLE_LIST, force layout to recalc
	::SetWindowLong(m_hWnd, GWL_STYLE, GetStyle() | TBSTYLE_LIST);
	m_bForcedListStyle = true;
	AutoSizeToolbar();
	DeferFreshenAfter();
}

void CXTToolBar::DropCustomItemInfo(CXTCustomGroupItemInfo* pInfo, int nIndex)
{
	switch (pInfo->m_type)
	{
		case CXTCustomGroupItemInfo::Button:
		{
			bool bMenuDropDown = pInfo->m_pDropDown && pInfo->m_pDropDown->bMenuBarPopup;
			CString strLabel = pInfo->m_strLabel;
			XT_TBBUTTON button;
			button.fsStyle = TBSTYLE_BUTTON;
			button.fsState = 0;
			button.iString = -1;
			button.iBitmap = I_IMAGENONE;
			if (bMenuDropDown)
			{
				// generate new unique ID for this popup
				UINT nPopupID = 100;
				if (POSITION pos = m_listTBButtons.GetHeadPosition())
				{
					do
					{
						XT_TBBUTTON& button = m_listTBButtons.GetNext(pos);
						if (button.pDDButton && button.pDDButton->bMenuBarPopup)
						{
							UINT nID = button.idCommand + 1;
							if (nID > nPopupID)
							{
								nPopupID = nID;
							}
						}
					}
					while (pos);
				}
				button.idCommand = nPopupID;
			}
			else
			{
				button.idCommand = pInfo->m_nCmdID;
			}
			if (pInfo->m_icon != NULL)
			{
				if (pInfo->m_disabledIcon != NULL)
				{
					if (CImageList* pDisabledIL = GetToolBarCtrl().GetDisabledImageList())
					{
						pDisabledIL->Add(pInfo->m_disabledIcon);
					}
				}
				CImageList* pIL = GetToolBarCtrl().GetImageList();
				int iImage = pIL->Add(pInfo->m_icon);
				button.iBitmap = iImage;
				if (m_bDropCommandText)
				{
					if (strLabel.IsEmpty())
					{
						strLabel = pInfo->m_strTitle;
					}
				}
			}
			else
			{
				if (strLabel.IsEmpty())
				{
					strLabel = pInfo->m_strTitle;
				}
			}
			if (pInfo->m_pDropDown)
			{
				button.fsStyle |= TBSTYLE_DROPDOWN;
				XT_DROPDOWNBUTTON* pDDButton = new XT_DROPDOWNBUTTON(pInfo->m_pDropDown);
				button.pDDButton = pDDButton;
				pDDButton->idButton = button.idCommand;
				if (bMenuDropDown)
				{
					button.fsStyle |= BTNS_WHOLEDROPDOWN;
				}
				m_listTBButtons.AddTail(button);
				ASSERT(FindDropDownButton(button.idCommand) != 0);
				button.bOwnsButton = false;
			}			
			
			if (!strLabel.IsEmpty())
			{
				AddCustomButtonText(&button, strLabel);				
			}	
			VERIFY(InsertButton(nIndex, &button));
			if (pInfo->m_pDropDown && pInfo->m_pDropDown->bArrow) 
			{
				SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
				DeferFreshenAfter();
			}
			break;
		}
		default:
			TRACE1("Cannot drop items of this type %d\n", (int)pInfo->m_type);
			ASSERT(false);
	};
	if (GetDockingFrame())
	{
		m_bDelayedButtonLayout = TRUE;
		AutoSizeToolbar();		
		DeferFreshenAfter();
	}
}

// default implementation of how to add text to a button created during
// toolbar customization
void CXTToolBar::AddCustomButtonText(TBBUTTON* button, const CString& strLabel)
{
	// button shall be autosizable
	button->fsStyle |= TBSTYLE_AUTOSIZE;

	// toolbar shall have TBSTYLE_LIST
	SetListStyleIfNoText();

	// remeber text and assign the string
	button->iString = CacheString(strLabel, button->idCommand);
}

// tracks menu popup that actually belongs to the app menubar
void CXTToolBar::TrackMenuBarPopup(DWORD dwMenuBarPopupID, const CRect& rect)
{
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	if (connection->GetMenuPopup)
	{
		if (HMENU hPopup = connection->GetMenuPopup(dwMenuBarPopupID))
		{
			CXTMenu menu;
			menu.Attach(hPopup);
			TrackMenuPopup(&menu, rect);
			menu.Detach();
		}
	}
}

void CXTToolBar::SaveToolBarState(LPCTSTR pszSubKey, LPCTSTR pszValueName)
{
	// get current toolbar state
	LPVOID pvData;
	DWORD cbData;
	bool bOwnsData = true;
	GetCurrentBarState(&pvData, &cbData);

	// compute CRC on original state
	if (!m_pvOriginalState)
	{
		m_pvOriginalState = pvData;
		m_cbOriginalState = cbData;
		bOwnsData = false;
	}

	CXTRegistryBlob blob;

	DWORD dwOriginalCRC = blob.ComputeCrc32(m_pvOriginalState, m_cbOriginalState);

	// archive the data
	BYTE buffer[512];
	CArchive ar(blob.GetFile(), CArchive::store, 512, buffer);
	ar << dwOriginalCRC;	  // crc
	ar.WriteCount(cbData);    // data length
	ar.Write(pvData, cbData); // data per se	
	ar.Flush();               // done

	// save to the registry
	blob.Save(pszSubKey, pszValueName);

	// cleanup
	if (bOwnsData)
	{
		free(pvData);
	}

}

bool CXTToolBar::LoadToolBarState(LPCTSTR pszSubKey, LPCTSTR pszValueName)
{	
	if (!m_pvOriginalState)
	{
		// fix the initial state if run for the first time
		GetCurrentBarState(&m_pvOriginalState, &m_cbOriginalState);
	}

	CXTRegistryBlob blob;

	// compute CRC on the initial state
	DWORD dwOriginalCRC = blob.ComputeCrc32(m_pvOriginalState, m_cbOriginalState);

	// get the blob as stored in the registry	
	if (!blob.Load(pszSubKey, pszValueName))
	{
		// none available, return
		return false;
	}

	TRY
	{
		// de-archive
		BYTE buffer[512];
		CArchive ar(blob.GetFile(), CArchive::load, 512, buffer);
		
		// get old crc, refuse to load if changed
		DWORD dwOldCrc;
		ar >> dwOldCrc;
		if (dwOldCrc != dwOriginalCRC)
		{
			// initial state has changed, refuse to load
			return false;
		}

		// load data
		DWORD cbData = ar.ReadCount();
		if (cbData == 0 || cbData >= blob.GetFile()->GetLength())
		{
			// nothing there or misplaced counter
			return false;
		}
		LPVOID pvData = _alloca(cbData);
		ar.Read(pvData, cbData);
		
		// restore the state
		return SetCurrentBarState(pvData, cbData);
	}
	CATCH(CArchiveException, e)
	{
		// bad format
	}
	END_CATCH

	// abnormal completion
	return false;
}

// flags to define persistent state of the toolbar button
enum
{
	// button flags
	XTBFLAG_HASDROPDOWN = 0x0001,  // has a popup
	XTBFLAG_TEXTONLY    = 0x0002,  // text-only layout
	XTBFLAG_IMAGE		= 0x0004,  // has image

	// toolbar flags
	XTBFLAG_FORCEDLISTSTYLE = 0x0001,  // has a forced list style

	XTBFLAG_CURVERSION  = 1,       // current version
};

// serializes current bar state into a blob
void CXTToolBar::GetCurrentBarState(LPVOID* ppvState, DWORD* pcbState)
{
	BYTE buffer[512];
	CMemFile file;
	CArchive ar(&file, CArchive::store, 512, buffer);

	// store version stamp
	WORD version = XTBFLAG_CURVERSION;
	ar << version;			  

	WORD flags = 0;
	if (m_bForcedListStyle)
	{
		flags |= XTBFLAG_FORCEDLISTSTYLE;
	}
	ar << flags;

	int nButtonPopupSize = sizeof(XT_DROPDOWNBUTTON);
	ar.WriteCount(nButtonPopupSize);

	int nCount = GetButtonCount();
	ar.WriteCount(nCount);

	int i;
	for (i = 0; i < nCount; i++)
	{
		TBBUTTON button;
		GetButton(i, &button);
		
		// save button ID, style, icon index
		ar << button.idCommand;
		ar << button.fsStyle;
		ar << button.iBitmap;

		// build flag bits
		flags = 0;
		XT_DROPDOWNBUTTON* pDropButton = FindDropDownButton(button.idCommand);
		if (pDropButton)
		{
			flags |= XTBFLAG_HASDROPDOWN;
		}

		if (button.iBitmap == I_IMAGENONE)
		{
			flags |= XTBFLAG_TEXTONLY;
		}

		if (button.iBitmap >= 0)
		{
			flags |= XTBFLAG_IMAGE;
		}

		// save flags
		ar << flags;

		// save dropdown info if applicable
		if (pDropButton)
		{
			ar.Write(pDropButton, sizeof(XT_DROPDOWNBUTTON));
		}
	}
	// save texts
	ar.WriteCount(m_mapBtnTextCmd.GetCount());
	if (POSITION pos = m_mapBtnTextCmd.GetStartPosition())
	{
		do
		{
			int nCmdID;
			CString text;
			m_mapBtnTextCmd.GetNextAssoc(pos, nCmdID, text);
			ar << (UINT)nCmdID;
			ar << text;
		}
		while(pos);
	}
	
	// done saving
	ar.Flush();

	// report the data through out parameters
	*pcbState = ( DWORD )file.GetLength();
	*ppvState = file.Detach();	
}

// helper class for toolbar persistent state
class CXTToolBarState
{
public:

	// array of buttons
	CArray<TBBUTTON, TBBUTTON&> m_buttons;

	// dropdown map
	CMap<UINT, UINT, XT_DROPDOWNBUTTON, XT_DROPDOWNBUTTON&> m_dropDownMap;

	// maps command ID into its text
	CMap<UINT, UINT, CString, CString&> m_textMap;

	bool m_bForcedListStyle;

	// constructor
	CXTToolBarState()
	: m_bForcedListStyle(false)
	{	}

	// destructor
	~CXTToolBarState()
	{	}

	// loads state from serialized data
	bool Load(LPVOID pvData, DWORD cbData)
	{
		BYTE buffer[512];
		CMemFile file;
		file.Attach((LPBYTE)pvData, cbData);
		CArchive ar(&file, CArchive::load, 512, buffer);

		// get version stamp, refuse to load if changed
		WORD version;
		ar >> version;
		if (version != XTBFLAG_CURVERSION)
		{
			return false;
		}

		WORD flags;
		ar >> flags;
		m_bForcedListStyle = (flags & XTBFLAG_FORCEDLISTSTYLE) != 0;

		int nButtonPopupSize = (int)ar.ReadCount();
		if (nButtonPopupSize != sizeof(XT_DROPDOWNBUTTON))
		{
			// button size has changed
			return false;
		}

		// read buttons one by one
		int nCount = (int)ar.ReadCount();
		while(nCount--)
		{
			TBBUTTON button;
			memset(&button, 0, sizeof(button));

			// restore button command ID and style
			ar >> button.idCommand;
			ar >> button.fsStyle;
			ar >> button.iBitmap;
			button.iString = -1;

			// read and decode button flags
			flags;
			ar >> flags;

			// restore dropdown if applicable
			if (flags & XTBFLAG_HASDROPDOWN)
			{
				XT_DROPDOWNBUTTON dropDown;
				ar.Read(&dropDown, sizeof(dropDown));
				ASSERT((UINT)button.idCommand == dropDown.idButton);
				m_dropDownMap.SetAt(button.idCommand, dropDown);
			}

			m_buttons.Add(button);
		}

		// load texts	
		int nTextCount = (int)ar.ReadCount();
		while(nTextCount--)
		{
			UINT nCmdID;
			CString text;
			ar >> nCmdID;
			ar >> text;
			m_textMap.SetAt(nCmdID, text);
		}

		// done
		return true;
	}

};

bool CXTToolBar::SetCurrentBarState(LPVOID pvData, DWORD cbData)
{
	CXTToolBarState state;
	if (!state.Load(pvData, cbData))
	{
		// cannot load
		return false;
	}

	m_bForcedListStyle = state.m_bForcedListStyle;
	if (m_bForcedListStyle &&
		(GetStyle() & TBSTYLE_LIST) == 0)
	{
		::SetWindowLong(m_hWnd, GWL_STYLE, GetStyle() | TBSTYLE_LIST);
	}

	// get icons available in this toolbar
	CXTIconMap iconMap;
	GetIcons(&iconMap);

	// get customize connection
	CXTIconMap iconMapGlobal;
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->GetIcons(&iconMapGlobal);

	// remove all but those linked to controls 
	// since they are not customizable as of time of writing
	if (POSITION pos = m_listTBButtons.GetHeadPosition())
	{
		do
		{
			POSITION pos1 = pos;
			XT_TBBUTTON& tbButton = m_listTBButtons.GetNext(pos);
			if (!tbButton.pWndCtrl)
			{
				m_listTBButtons.RemoveAt(pos1);
			}
		}
		while(pos);
	}

	bool bArrowStyleSet = false;
	const int nCount = state.m_buttons.GetSize();
	if (nCount > 0)
	{
		TBBUTTON* buttonsBegin = (TBBUTTON*)_alloca(nCount * sizeof(TBBUTTON));
		TBBUTTON* buttonsEnd = buttonsBegin;
		int i;
		for (i = 0; i < nCount; i++, buttonsEnd++)
		{
			// copy button descriptor
			*buttonsEnd = state.m_buttons[i];

			// create and store button info
			XT_TBBUTTON xtButton;
			(TBBUTTON&)xtButton = *buttonsEnd;
			m_listTBButtons.AddTail(xtButton);

			if (xtButton.fsStyle & TBSTYLE_SEP)
			{
				// done for separators
				continue;
			}

			XT_DROPDOWNBUTTON dropDown;
			bool bDropDown = false;
			if (state.m_dropDownMap.Lookup(buttonsEnd->idCommand, dropDown))
			{
				bDropDown = true;
				XT_DROPDOWNBUTTON* pDDButton = InitDropDownButton(buttonsEnd->idCommand, 
													dropDown.bArrow);
				*pDDButton = dropDown;
				if (pDDButton->bArrow && !bArrowStyleSet) 
				{
					bArrowStyleSet = true;
					SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
				}
				ASSERT(FindDropDownButton(buttonsEnd->idCommand));
			}

			// check to see if this item was created from a menu popup
			// this is a special case since its command ID is generated internally
			// and not visible from outside
			if (bDropDown && dropDown.bMenuBarPopup)
			{
				// done menu bar popup				
				continue;
			}

			// add an icon for the command
			HICON hIcon;
			HICON hDisabledIcon;
			HICON hHotIcon;
			if (iconMap.Lookup(buttonsEnd->idCommand, hIcon, hDisabledIcon, hHotIcon))
			{
				// this command belong to toolbar as originally setup,
				// reuse original image index which is in button.iBitmap now
			}
			else
			{
				// lookup the icon in global icon map, add it if available
				if (iconMapGlobal.Lookup(buttonsEnd->idCommand, hIcon, hDisabledIcon, hHotIcon))
				{
					CImageList* pDisabledIL = GetToolBarCtrl().GetDisabledImageList();
					if (pDisabledIL && hDisabledIcon)
					{
						pDisabledIL->Add(hDisabledIcon);
					}
					CImageList* pHotIL = GetToolBarCtrl().GetHotImageList();
					if (pHotIL && hHotIcon)
					{
						pHotIL->Add(hHotIcon);
					}
					CImageList* pIL = GetToolBarCtrl().GetImageList();
					int iImage = pIL->Add(hIcon);
					buttonsEnd->iBitmap = iImage;
				}
				else
				{
					// check the tools manager icon map
					if ( CXTToolsManager::Get( ).m_mapIconCmd.Lookup(
						buttonsEnd->idCommand, hIcon, hDisabledIcon, hHotIcon ) )
					{
						CImageList* pIL = GetToolBarCtrl().GetImageList();
						int iImage = pIL->Add(hIcon);
						buttonsEnd->iBitmap = iImage;
					}
					else
					{
						// icon is not available
						buttonsEnd->iBitmap = I_IMAGENONE;
					}
				}
			}
		}

		int nRemoved;
		int nButtonCount = GetButtonCount();
		for (nRemoved = 0; nRemoved < nButtonCount; nRemoved++)
		{
			SendMessage(TB_DELETEBUTTON, 0, 0);
		}
		ASSERT(GetButtonCount() == 0);

		delete m_pStringMap;
		m_pStringMap = NULL;

		// set texts
		TBBUTTON* button;
		for (button = buttonsBegin; button < buttonsEnd; button++)
		{
			CString label;
			if (state.m_textMap.Lookup(button->idCommand, label))
			{
				button->iString = CacheString(label, button->idCommand);
			}
		}

		if (!SendMessage(TB_ADDBUTTONS, buttonsEnd - buttonsBegin, (LPARAM)buttonsBegin))
		{
			return false;
		}
		ASSERT(GetButtonCount() == buttonsEnd - buttonsBegin);

	}	

	return true;
}

// For reasons unknown, toolbar resets its image list every now and then
// The function below resolves references to orphaned command icons 
void CXTToolBar::ResolveCommandIcons()
{
	CImageList* pImageList = GetToolBarCtrl().GetImageList();
	CImageList* pDisabledImageList = GetToolBarCtrl().GetDisabledImageList();
	CImageList* pHotImageList = GetToolBarCtrl().GetHotImageList();
	if (pImageList && pImageList->m_hImageList)
	{
		int nImageCount = pImageList->GetImageCount();
		// get customize connection
		CXTIconMap iconMapGlobal;
		bool bMapEmpty = true;

		// first pass - get commands that have orphaned command IDs
		int nButtonCount = GetButtonCount();
		int i ;
		for (i = 0; i < nButtonCount; i++)
		{
			TBBUTTON button;
			GetButton(i, &button);
			if (!(button.fsStyle & TBSTYLE_SEP) && 
				button.iBitmap >= nImageCount)
			{
				if (bMapEmpty)
				{
					bMapEmpty = false;
					CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
					connection->GetIcons(&iconMapGlobal);					
				}
				HICON hIcon, hDisabledIcon, hHotIcon;
				if (iconMapGlobal.Lookup(button.idCommand, hIcon, hDisabledIcon, hHotIcon))
				{
					if (pDisabledImageList)
					{
						pDisabledImageList->Add(::CopyIcon(hDisabledIcon));
					}
					if (pHotImageList)
					{
						pHotImageList->Add(::CopyIcon(hHotIcon));
					}
					int iImage = pImageList->Add(::CopyIcon(hIcon));
					if (button.iBitmap != iImage)
					{
						button.iBitmap = iImage;
						SetButton(i, &button);
					}
				}				
			}
		}
	}
}

void CXTToolBar::GetIcons(CXTIconMap* pMap)
{	
	ASSERT_VALID(this);

	if (!m_hWnd)
	{
		// not created yet
		return;
	}
	
	HIMAGELIST hImageList = GetImageList();
	if (!hImageList)
	{
		// no images
		return;
	}

	CImageList* pImageList = CImageList::FromHandle(hImageList);
	CImageList* pDisabledImageList = GetToolBarCtrl().GetDisabledImageList();
	CImageList* pHotImageList = GetToolBarCtrl().GetHotImageList();

	if (m_pvOriginalState)
	{
		// the state has been loaded at least once, lookup the original state
		CXTToolBarState state;
		if (state.Load(m_pvOriginalState, m_cbOriginalState))
		{
			int i;
			for (i = 0; i < state.m_buttons.GetSize(); i++)
			{
				const TBBUTTON& button = state.m_buttons[i];

				if (IsSeparator(button) || IsControl(button))
					continue;

				if (button.iBitmap >= 0)
				{
					HICON hIcon = pImageList->ExtractIcon(button.iBitmap);
					ASSERT(hIcon != NULL);
					HICON hDisabledIcon = 0;
					if (pDisabledImageList)
					{
						hDisabledIcon = pDisabledImageList->ExtractIcon(button.iBitmap);
					}
					HICON hHotIcon = 0;
					if (pHotImageList)
					{
						hHotIcon = pHotImageList->ExtractIcon(button.iBitmap);
					}
					pMap->SetAt(button.idCommand, hIcon, hDisabledIcon, hHotIcon);	
				}
			}
		}
	}
	else
	{
		const int nButtonCount = GetButtonCount();
		int nIndex;
		for (nIndex = 0; nIndex < nButtonCount; ++nIndex)
		{
			TBBUTTON button;
			GetButton(nIndex, &button);

			if (IsSeparator(button) || IsControl(button))
				continue;

			if (button.iBitmap >= 0)
			{
				HICON hIcon = pImageList->ExtractIcon(button.iBitmap);
				if (hIcon)
				{
					HICON hDisabledIcon = 0;
					if (pDisabledImageList)
					{
						hDisabledIcon = pDisabledImageList->ExtractIcon(button.iBitmap);
					}
					HICON hHotIcon = 0;
					if (pHotImageList)
					{
						hHotIcon = pHotImageList->ExtractIcon(button.iBitmap);
					}
					pMap->SetAt(button.idCommand, hIcon, hDisabledIcon, hHotIcon);	
				}
			}
		}
	}
	
	// make sure "cool menu" icons are included.
	POSITION pos = XTImageManager()->m_mapImages[1].GetStartPosition(); 
	while(pos)
	{
		UINT nID; CXTImage* pImage;
		XTImageManager()->m_mapImages[1].GetNextAssoc( pos, nID, pImage );
		
		HICON hIcon, hIconDisabled, hIconHot;
		if ( !pMap->Lookup( nID, hIcon, hIconDisabled, hIconHot ) )
		{
			HICON hIcon = CopyIcon(pImage->GetIcon());
			pMap->SetAt( nID, hIcon, hIcon, 0 );
		}
	}
}

void CXTToolBar::RegisterCustomizeAPI(bool bRegister)
{
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	if (bRegister)
	{
		connection->GetIcons.Add(this, &CXTToolBar::GetIcons);
		connection->GetCustomGroups.Add(this, &CXTToolBar::ReportCustomGroups);
	}
	else
	{
		connection->Remove(this);
	}
}

void CXTToolBar::ReportCustomGroups(CXTCustomGroups& groups)
{
	CString strWindowText;
	GetWindowText(strWindowText);
	if (strWindowText.IsEmpty()) 
	{
		XT_MANAGE_STATE_BEGIN  // Switch to toolkit resources/module state
		strWindowText.LoadString(XT_IDS_TOOLBAR);
		XT_MANAGE_STATE_END
	}
	// toolbar reports only one group
	CXTCustomGroupInfo* group = new CXTCustomGroupInfo();
	group->m_nSortPriority = 2000 + GetDlgCtrlID();
	groups.Add(group);
	group->m_strTitle = strWindowText;
	if (!ReportOriginalCustomGroups(group->m_items))
	{
		// report group as is
		const int nButtonCount = GetButtonCount();
		int nIndex;
		for (nIndex = 0; nIndex < nButtonCount; ++nIndex)
		{
			if (CXTCustomGroupItemInfo* pInfo = GetCustomItemInfo(nIndex))
			{
				// save item info
				group->m_items.Add(pInfo);
			}
		}
	}
}

bool CXTToolBar::StartCustomDrag(CPoint point)
{
	// intercept attempt to initiate drag operation
	if (m_dwStyle & TBSTYLE_ALTDRAG &&
		m_dwStyle & CCS_ADJUSTABLE)
	{
		if (::GetKeyState(VK_MENU) < 0)
		{
			// alt-drag
			DoCustomDrag(point);
			return true;
		}
	}
	return false;	
}

int CXTToolBar::LocateHit(const CPoint& point)
{
	int buttonCount = (int)SendMessage(TB_BUTTONCOUNT);
	int i;
	int buttonPos = -1;
	for (i = 0; i < buttonCount && buttonPos < 0; i++)
	{
		TBBUTTON button;
		if (GetButton(i, &button))
		{
			if (!IsSeparator(button) &&
				!(button.fsState & TBSTATE_HIDDEN))
			{
				CRect rect;
				if (SendMessage(TB_GETITEMRECT, i, (LPARAM)&rect))
				{
					if (rect.PtInRect(point))
					{
						buttonPos = i;
					}
				}
			}
		}
	}
	return buttonPos;
}

void CXTToolBar::DoCustomDrag(CPoint point)
{
	if (!GetCustomBar())
	{
		// not customizable
		return;
	}

	int nSelButton = LocateHit(point);
	if (nSelButton < 0)
	{
		// clicked on the blank surface
		return;
	}

	if (IsSeparator(nSelButton))
	{
		// separators not selectable
		return;
	}

	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->SetCustMode(true);
	connection->SetEnableCmdUI(false);
	ASSERT(m_pCustomHandler != 0);
	m_pCustomHandler->SelectButton(nSelButton);
	m_pCustomHandler->ProcessDragOrClick(point);
	connection->SetCustMode(false);
}

// tells if a button at given position is a separator
bool CXTToolBar::IsSeparator(int nIndex) const
{
	TBBUTTON button;
	GetButton(nIndex, &button);
	return IsSeparator(button);
}

// tells if a button is a separator
bool CXTToolBar::IsSeparator(const TBBUTTON& button)
{
	return (button.fsStyle & TBSTYLE_SEP) != 0 && button.idCommand == 0;
}

// tells if a button is a control item
bool CXTToolBar::IsControl(const TBBUTTON& button)
{
	return (button.fsStyle & TBSTYLE_SEP) != 0 && button.idCommand != 0;
}

bool CXTToolBar::ReportOriginalCustomGroups(XT_CUSTOMITEMS& items)
{
	bool success = false;
	if (m_pvOriginalState)
	{
		// report group per original state
		CXTToolBarState state;
		if (state.Load(m_pvOriginalState, m_cbOriginalState))
		{
			const int nCount = state.m_buttons.GetSize();
			if (nCount > 0)
			{
				success = true;
				int i;
				for (i = 0; i < nCount; i++)
				{
					// copy button descriptor
					TBBUTTON button = state.m_buttons[i];
					if (IsSeparator(button))
					{
						continue;
					}
					if (CXTCustomGroupItemInfo* pInfo = GetCustomItemInfo(button))
					{
						CString label;
						if (state.m_textMap.Lookup(button.idCommand, label))
						{
							pInfo->m_strLabel = label;
						}
						items.Add(pInfo);
						XT_DROPDOWNBUTTON dropDown;
						if (state.m_dropDownMap.Lookup(button.idCommand, dropDown))
						{
							pInfo->m_pDropDown = new XT_DROPDOWNBUTTON(&dropDown);
							pInfo->m_bOwnsDropDown = true;
						}
					}
				}
			}
		}
	}
	return success;
}

