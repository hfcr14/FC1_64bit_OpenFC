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
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTFunctions.h"
#include "XTToolBarCtrl.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTCoolMenu.h"
#include "XTMemDC.h"
#include "XTButton.h"
#include "XTColorSelectorCtrl.h"
#include "XTColorPopup.h"
#include "XTSkins.h"
#include "XTHelpers.h"
#include "XTPriv.h"
#include "XTExpButton.h"
#include "XTToolCmdUI.h"
#include "XTOptionsManager.h"
#include "XTAccelManager.h"
#include "XTMenuBar.h"
#include "XTFrameImpl.h"
#include "XTToolbarHelpers.h"
#include "XTImageManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int EXPANSION_BUTTON_WIDTH = 12;
static const int EXPANSION_BUTTON_INSET = 1;

struct XT_CONTROLPOS
{
	int   nID;
	int   nIndex;
	CRect rectOldPos;
};

struct XT_OLDTOOLINFO
{
	UINT cbSize;
	UINT uFlags;
	HWND hwnd;
	UINT uId;
	RECT rect;
	HINSTANCE hinst;
	LPTSTR lpszText;
};

/////////////////////////////////////////////////////////////////////////////
// CXTToolBar
/////////////////////////////////////////////////////////////////////////////
CXTToolBar::CXTToolBar()
{
	// initialize state
	m_pStringMap		   = NULL;
	m_hRsrcImageWell	   = NULL;
	m_hInstImageWell	   = NULL;
	m_hbmImageWell		   = NULL;
	m_bDelayedButtonLayout = TRUE;

	// default image sizes
	m_sizeImage.cx		   = 16;
	m_sizeImage.cy		   = 15;

	// default button sizes
	m_sizeButton.cx 	   = 23;
	m_sizeButton.cy 	   = 22;

	// top and bottom borders are 1 larger than default for ease of grabbing
	m_cyTopBorder		   = 3;
	m_cyBottomBorder	   = 3;
	m_bCustomize		   = false;
	m_bNoEntry			   = FALSE;
	m_bDisabledColor	   = FALSE;

	m_strSubKey 		   = _T("");
	m_strValueName		   = _T("");

	m_uiLastCmd 		   = (UINT)-1;
	m_pColorPopup		   = NULL;
	m_pExpButton           = NULL;

	m_bInvalidateOnMouseUp = false;
	m_bOwnsDropDowns       = true;
	m_bAdjusting		   = false;
	m_bRestoringState      = false;
	m_pvOriginalState      = 0;
	m_cbOriginalState      = 0;
	m_bFirstDrawPass	   = true;
	m_bForcedListStyle	   = false;
	m_bDropCommandText     = false;
	m_bIdealSizeChanged    = false;

	// no DrawText flags are set by default
	m_dwDrawTextMask       = 0;
	m_dwDrawTextFlags      = 0;

	m_dwSignatureFreshenAfter = 0;

	m_pCustomHandler = 0;
}

CXTToolBar::~CXTToolBar()
{
	AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
	SAFE_DELETE(m_pStringMap);
	SAFE_DELETE(m_pColorPopup);
	m_nCount = 0;
	RegisterCustomizeAPI(false);
	free(m_pvOriginalState);
}

IMPLEMENT_DYNAMIC(CXTToolBar, CXTControlBar)

BEGIN_MESSAGE_MAP(CXTToolBar, CXTControlBar)
	//{{AFX_MSG_MAP(CXTToolBar)
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_NCCREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP

	ON_MESSAGE(TB_SETBUTTONSIZE, OnSetButtonSize)
	ON_MESSAGE(TB_SETBITMAPSIZE, OnSetBitmapSize)
	ON_MESSAGE(TB_SETDRAWTEXTFLAGS, OnSetDrawTextFlags)
	ON_MESSAGE(WM_SETTINGCHANGE, OnPreserveZeroBorderHelper)
	ON_MESSAGE(WM_SETFONT,		 OnPreserveZeroBorderHelper)
	ON_MESSAGE(XTWM_FRESHENAFTER, OnFreshenAfter)

	ON_NOTIFY_REFLECT(TBN_BEGINADJUST,	 OnToolBarBeginAdjust)
	ON_NOTIFY_REFLECT(TBN_CUSTHELP, 	 OnToolBarCustomHelp)
	ON_NOTIFY_REFLECT(TBN_ENDADJUST,	 OnToolBarEndAdjust)
	ON_NOTIFY_REFLECT(TBN_GETBUTTONINFO, OnToolBarGetButtonInfo)
	ON_NOTIFY_REFLECT(TBN_QUERYDELETE,	 OnToolBarQueryDelete)
	ON_NOTIFY_REFLECT(TBN_QUERYINSERT,	 OnToolBarQueryInsert)
	ON_NOTIFY_REFLECT(TBN_RESET,		 OnToolBarReset)
	ON_NOTIFY_REFLECT(TBN_TOOLBARCHANGE, OnToolBarChange)
	ON_NOTIFY_REFLECT(TBN_HOTITEMCHANGE, OnToolBarHotItemChange)

	ON_MESSAGE(CPN_XT_CLOSEUP,	OnCloseUp)
	ON_MESSAGE(CPN_XT_SELENDOK, OnSelEndOK)

	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW,	 OnCustomDraw)
	ON_NOTIFY_REFLECT(TBN_DROPDOWN, 	 OnToolBarBtnDropDown)

END_MESSAGE_MAP()

BOOL CXTToolBar::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID)
{
	return CreateEx(pParentWnd, 0, dwStyle,
		CRect(m_cxLeftBorder, m_cyTopBorder, m_cxRightBorder, m_cyBottomBorder), nID);
}

BOOL CXTToolBar::CreateEx(CWnd* pParentWnd, DWORD dwCtrlStyle, DWORD dwStyle, CRect rcBorders, UINT nID)
{
	ASSERT_VALID(pParentWnd);	// must have a parent
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

	SetBorders(rcBorders);

	// save the style
	m_dwStyle = (dwStyle & (CBRS_ALL|CBRS_GRIPPER));
	if (nID == AFX_IDW_TOOLBAR)
		m_dwStyle |= CBRS_HIDE_INPLACE;

	dwStyle &= ~CBRS_ALL;
	dwStyle |= CCS_NOPARENTALIGN|CCS_NOMOVEY|CCS_NODIVIDER|CCS_NORESIZE;
	dwStyle |= dwCtrlStyle;

	// initialize common controls
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(icex);
	icex.dwICC = ICC_BAR_CLASSES;
	VERIFY(InitCommonControlsEx(&icex));

	ASSERT(xtAfxData.iComCtlVersion != -1);

	// create the HWND
	CRect rect; rect.SetRectEmpty();
	if (!CWnd::Create(TOOLBARCLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	// sync up the sizes and set the font.
	SetSizes(m_sizeButton, m_sizeImage);
	SetFont(&m_font);

	CWinApp* pApp = AfxGetApp();
	ASSERT( pApp );

	XT_MANAGE_STATE_BEGIN  // Switch to toolkit resources/module state

	m_strValueName.Format(XT_IDS_REG_CBARID, pApp->m_pszProfileName,
		_AfxGetDlgCtrlID(m_hWnd));

	m_strSubKey.Format(XT_IDS_REG_KEY, pApp->m_pszRegistryKey,
		pApp->m_pszProfileName);

	XT_MANAGE_STATE_END

	// Note: Parent must resize itself for control bar to be resized

	return TRUE;
}

int CXTToolBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTControlBar::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	// create a button for rendering expansion chevron
	if (IsExpansionButton())
	{
		// create expansion button invisible
		m_pExpButton = new CXTExpButton();
		if (!m_pExpButton->Create(this, XT_IDC_CUSTOMIZE))
		{
			return -1;
		}
	}

	// register API this object provides for toolbar customization
	RegisterCustomizeAPI(true);

	return 0;
}

LRESULT CXTToolBar::OnNcHitTest(CPoint point)
{
	UNUSED_ALWAYS(point);
	return HTCLIENT;
}

// Custom draw hander used to
//	* draw separators if m_bDrawSeparators is TRUE (comctl32.dll version <= 470)
//	* draw checked buttons as pressed
void CXTToolBar::OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Only do this if the style is flat.
	if (((::GetWindowLong(m_hWnd, GWL_STYLE) & TBSTYLE_FLAT) == TBSTYLE_FLAT) && !m_bNoEntry)
	{
		LPNMCUSTOMDRAW lpNMCD = (LPNMCUSTOMDRAW)pNMHDR;

		CDC* pDC = CDC::FromHandle(lpNMCD->hdc);

		switch (lpNMCD->dwDrawStage)
		{
		case CDDS_PREPAINT:
			m_bFirstDrawPass = true;
			*pResult |= (CDRF_NOTIFYITEMDRAW|CDRF_NOTIFYPOSTPAINT);
			break;

		case CDDS_ITEMPREPAINT:
			if (DrawItem(pDC, lpNMCD))
				*pResult |= CDRF_SKIPDEFAULT;
			break;

		case CDDS_POSTPAINT:
			EraseWrappedSeparators(pDC);
			DrawSeparators(pDC);

			if (IXTToolbarProxy* proxy = IXTSkin::GetInstance()->GetToolbarProxy())
			{
				int i;
				for (i = 0; i < m_arrPopupDropped.GetSize(); i++)
				{
					// delegate actual drawing to it
					proxy->DrawDroppedEffects(pDC, this, m_arrPopupDropped[i]);
				}
			}
			break;
		}
	}
}

void CXTToolBar::DrawSeparators(CDC* pDC)
{
	// Get the number of buttons in the toolbar.
	int nButtons = GetButtonCount();

	// loop thru the buttons.
	int nIndex;
	for (nIndex = 0; nIndex < nButtons; ++nIndex)
	{
		TBBUTTON button;
		GetButton(nIndex, &button);

		if ((button.fsStyle & TBSTYLE_SEP) && !(button.fsState & TBSTATE_HIDDEN))
		{
			if ( button.idCommand )
			{
				// this space is for control on the toolbar
				if (!InRebar()) // in rebar we draw transparently
				{
					if (IXTToolbarProxy* proxy = IXTSkin::GetInstance()->GetToolbarProxy())
					{
						// delegate actual drawing to it
						proxy->FillControlRect(pDC, this, nIndex);
					}
					else
					{
						CRect rcItem;
						::SendMessage(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)&rcItem);
						pDC->FillSolidRect(rcItem, GetBackgroundColor());
					}
				}

				if (CWnd* pwndChild = GetDlgItem(button.idCommand))
				{
					// the code below prints control on device context we have
					CRect rectClient;
					GetClientRect(&rectClient);
					CRect rect;
					pwndChild->GetWindowRect(&rect);
					ScreenToClient(&rect);
					rect &= rectClient;
					CPoint ptOld = pDC->SetWindowOrg(-rect.left, -rect.top);
					pwndChild->Print(pDC, PRF_CHECKVISIBLE | PRF_CHILDREN | PRF_CLIENT | PRF_OWNED);
					pDC->SetWindowOrg(ptOld);
				}
			}
			else
			{
				// draw the separator
				if (IXTToolbarProxy* proxy = IXTSkin::GetInstance()->GetToolbarProxy())
				{
					// delegate actual drawing to it
					proxy->DrawSeparator(pDC, this, nIndex);
				}
				else
				{
					CRect rcItem;
					::SendMessage(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)&rcItem);

					if (button.fsState & TBSTATE_WRAP)
					{
						int x  = rcItem.left;
						int y  = rcItem.bottom-rcItem.Width();
						int cx = rcItem.Height()+x;
						int cy = rcItem.Width() +y;

						rcItem.SetRect(x,y,cx,cy);
						rcItem.top	= ((rcItem.top + rcItem.bottom)>>1) - 1;

						pDC->FillSolidRect(rcItem, GetBackgroundColor());
						pDC->DrawEdge(&rcItem, EDGE_ETCHED, BF_TOP);
						pDC->SetPixel(rcItem.right-1, rcItem.top, xtAfxData.clr3DShadow);
					}
					else
					{
						pDC->FillSolidRect(rcItem, GetBackgroundColor());
						rcItem.DeflateRect(2,0);
						rcItem.right = rcItem.left + 1;
						pDC->DrawEdge(&rcItem, EDGE_ETCHED, BF_LEFT);
						pDC->SetPixel(rcItem.left, rcItem.bottom-1, xtAfxData.clr3DShadow);
					}
				}
			}
		}
	}
}

BOOL CXTToolBar::DrawItem(CDC* pDC, LPNMCUSTOMDRAW lpNMCD)
{
	if (m_bNoEntry)
		return TRUE;

	if (m_bFirstDrawPass)
	{
		m_bFirstDrawPass = false;
		ResolveCommandIcons();
		if(IsFillToolBarClientRect())
		{
			// control seems to fill in background, this ensures correct background is rendered
			CRect rectClient;
			GetClientRect(&rectClient);
			pDC->FillSolidRect(rectClient, GetBackgroundColor());
		}
	}	

	UINT nIndex = CommandToIndex(lpNMCD->dwItemSpec);
	// check to see if a proxy can be found for drawing the item
	if (IXTToolbarProxy* proxy = IXTSkin::GetInstance()->GetToolbarProxy())
	{
		// delegate actual drawing to it
		return proxy->DrawButton(pDC, this, nIndex);
	}


	TBBUTTON button;
	GetButton(nIndex, &button);


	// don't draw text buttons: default is OK
	if (button.iBitmap < 0)
		return FALSE;

	XT_DROPDOWNBUTTON* pDropButton = FindDropDownButton(lpNMCD->dwItemSpec);

	BOOL bChecked  = (button.fsState & TBSTATE_CHECKED) != 0;
	BOOL bDisabled = (button.fsState & TBSTATE_ENABLED) != 0;
	BOOL bPressed  = (button.fsState & TBSTATE_PRESSED) != 0;

	BOOL bDropBtn  = pDropButton &&  pDropButton->bColorPicker;
	BOOL bClrPopup = bDropBtn && m_pColorPopup && ::IsWindow(m_pColorPopup->m_hWnd);

	pDC->SetBkMode(TRANSPARENT);

	if (m_uiLastCmd != lpNMCD->dwItemSpec) {
		bClrPopup = FALSE;
	}

	// Don't use MFC GetItemRect!!
	CRect rcItem;
	::SendMessage(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)&rcItem);

	// Draw the color box if this is a color picker button.
	if (pDropButton && pDropButton->bShowColor && pDropButton->bColorPicker)
	{
		CRect rcColor = rcItem;
		rcColor.left   += 4;
		rcColor.right	= rcColor.left + (m_sizeButton.cx - 8);
		rcColor.bottom -= 4;
		rcColor.top 	= rcColor.bottom - 3;

		if (bPressed || bChecked) {
			rcColor.OffsetRect(1,1);
		}

		pDC->FillSolidRect(rcColor,
			bDisabled ? xtAfxData.clr3DShadow : pDropButton->clrColor);
		pDC->ExcludeClipRect(rcColor);
	}

	// For rebar, force repaint of the background, but make sure
	// to avoid reentry. This allows bitmap backgrounds.
	// Only erase portion that lies in region I am painting (clip box).
	if (InRebar() == true)
	{
		CRect rc;
		pDC->GetClipBox(&rc);
		if (rc.IntersectRect(&rc, &rcItem))
		{
			m_bNoEntry = TRUE;
			InvalidateRect(&rc, TRUE);
			UpdateWindow();
			m_bNoEntry = FALSE;
		}
	}

	// Solid background: just draw it
	else {
		pDC->FillSolidRect(rcItem, GetBackgroundColor());
	}

	// only do custom paint for disabled color.
	if (bChecked || !(bDisabled && m_bDisabledColor) && !bClrPopup)
		return FALSE;

	BOOL bHasArrows = ((GetExtendedStyle() & TBSTYLE_EX_DRAWDDARROWS) &&
		(GetButtonStyle(nIndex) & TBSTYLE_DROPDOWN));

	// Get the button text ( if any ).
	CString strBtnText (GetButtonText(nIndex));

	// if we have text or drop arrows do not custom draw.
	if (bHasArrows || !strBtnText.IsEmpty())
		return FALSE;

	// if I got here button is disabled and I am drawing color

	// Get top left corner of image. This is top left corner of the item,
	// plus offset to center button in item (bitmap is smaller than item)
	CPoint point = rcItem.TopLeft();
	point.x += (m_sizeButton.cx - m_sizeImage.cx)/2 + 1;
	point.y += (m_sizeButton.cy - m_sizeImage.cy)/2 + 1;

	// draw button
	CImageList imageList;
	VERIFY(imageList.Attach(GetImageList()));
	UINT iBitmap = GetBitmap(lpNMCD->dwItemSpec);

	// draw embossed
	_xtAfxDrawEmbossed(pDC, imageList, iBitmap, point, TRUE);

	imageList.Detach();

	return TRUE;
}

void CXTToolBar::EraseWrappedSeparators(CDC* pDC)
{
	int nButtons = GetButtonCount();
	int i;
	for (i = 0; i < nButtons; i++)
	{
		TBBUTTON tbb;
		GetButton(i, &tbb);
		if (tbb.fsStyle & TBSTYLE_SEP && tbb.fsState & TBSTATE_WRAP && i>0)
		{
			CRect rcItem;
			SendMessage(TB_GETITEMRECT, i, (LPARAM)&rcItem);

			TBBUTTON prevButton;
			GetButton(i - 1, &prevButton);
			if (tbb.fsStyle & TBSTYLE_SEP)
			{
				continue;
			}

			CRect rcPrevItem;
			SendMessage(TB_GETITEMRECT, i-1, (LPARAM)&rcPrevItem);

			rcPrevItem += CPoint(rcPrevItem.Width(), 0);
			rcPrevItem.right = rcPrevItem.left + rcItem.Width();
			rcItem = rcPrevItem;

			CBrush br(GetBackgroundColor());
			CBrush* pOldBrush = pDC->SelectObject(&br);
			const CRect& rc=rcItem;
			pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
			pDC->SelectObject(pOldBrush);
			br.DeleteObject();
		}
	}
}

void CXTToolBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	UNUSED_ALWAYS(bCalcValidRects);

	// calculate border space (will add to top/bottom, subtract from right/bottom)
	CRect rect; rect.SetRectEmpty();
	BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	CXTControlBar::CalcInsideRect(rect, bHorz);
	ASSERT(xtAfxData.iComCtlVersion != -1);
	ASSERT(xtAfxData.iComCtlVersion >= VERSION_IE401 || rect.top >= 2);

	// adjust non-client area for border space
	lpncsp->rgrc[0].left   += rect.left;
	lpncsp->rgrc[0].top    += rect.top;
	lpncsp->rgrc[0].right  += rect.right;
	lpncsp->rgrc[0].bottom += rect.bottom;
}

void CXTToolBar::OnWindowPosChanging(WINDOWPOS FAR* lpWndPos)
{
	// not necessary to invalidate the borders
	DWORD dwStyle = m_dwStyle;
	m_dwStyle &= ~(CBRS_BORDER_ANY);
	CControlBar::OnWindowPosChanging(lpWndPos);
	m_dwStyle = dwStyle;

	// If we can resize while floating
	if (dwStyle & CBRS_SIZE_DYNAMIC)
	{
		// And we are resizing
		if (lpWndPos->flags & SWP_NOSIZE)
			return;

		// Then redraw the buttons
		Invalidate();
	}
}

void CXTToolBar::OnSysColorChange()
{
	// re-color bitmap for toolbar
	if (m_hInstImageWell != NULL && m_hbmImageWell != NULL)
	{
		HBITMAP hbmNew;
		hbmNew = _xtAfxLoadSysColorBitmap(m_hInstImageWell, m_hRsrcImageWell);
		if (hbmNew != NULL)
			AddReplaceBitmap(hbmNew);
	}
}

BOOL CXTToolBar::OnNcCreate(LPCREATESTRUCT lpCreateStruct)
{
	// If the toolbar is inside a ReBar, we need to make the parent frame
	// the owner so it will get notifications.
	CFrameWnd* pFrameWnd = (CFrameWnd*)GetParent();
	ASSERT_VALID(pFrameWnd);

	if (!CXTControlBar::OnNcCreate(lpCreateStruct)) {
		return FALSE;
	}

	// if the owner was set before the toolbar was created, set it now
	if (m_hWndOwner != NULL) {
		DefWindowProc(TB_SETPARENT, (WPARAM)m_hWndOwner, 0);
	}

	DefWindowProc(TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

	return TRUE;
}

LRESULT CXTToolBar::OnSetDrawTextFlags(WPARAM wParam, LPARAM lParam)
{
	m_dwDrawTextMask = DWORD(wParam);
	m_dwDrawTextFlags = DWORD(lParam);
	return Default();
}

DWORD CXTToolBar::GetDrawTextFlags(DWORD dwFlags) const
{
	dwFlags &= ~m_dwDrawTextMask;
	dwFlags |= m_dwDrawTextFlags;
	return dwFlags;
}

LRESULT CXTToolBar::OnSetButtonSize(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	return OnSetSizeHelper(m_sizeButton, lParam);
}

LRESULT CXTToolBar::OnSetBitmapSize(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(wParam);
	return OnSetSizeHelper(m_sizeImage, lParam);
}

LRESULT CXTToolBar::OnSetSizeHelper(CSize& size, LPARAM lParam)
{
	//WINBUG: The IE4 version of COMCTL32.DLL supports a zero border, but
	//	only if TBSTYLE_TRANSPARENT is on during the the TB_SETBITMAPSIZE
	//	and/or TB_SETBUTTONSIZE messages.  In order to enable this feature
	//	all the time (so we get consistent border behavior, dependent only
	//	on the version of COMCTL32.DLL) we turn on TBSTYLE_TRANSPARENT
	//	whenever these messages go through.  It would be nice that in a
	//	future version, the system toolbar would just allow you to set
	//	the top and left borders to anything you please.

	BOOL bModify = FALSE;
	ASSERT(xtAfxData.iComCtlVersion != -1);
	DWORD dwStyle = 0;
	if (xtAfxData.iComCtlVersion >= VERSION_IE401)
	{
		dwStyle = GetStyle();
		bModify = ModifyStyle(0, TBSTYLE_TRANSPARENT|TBSTYLE_FLAT);
	}

	LRESULT lResult = Default();
	if (lResult)
		size = lParam;

	if (bModify)
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	return lResult;
}

LRESULT CXTToolBar::OnPreserveZeroBorderHelper(WPARAM, LPARAM)
{
	BOOL bModify = FALSE;
	ASSERT(xtAfxData.iComCtlVersion != -1);
	DWORD dwStyle = 0;
	if (xtAfxData.iComCtlVersion >= VERSION_IE401)
	{
		dwStyle = GetStyle();
		bModify = ModifyStyle(0, TBSTYLE_TRANSPARENT|TBSTYLE_FLAT);
	}

	LRESULT lResult = Default();

	if (bModify)
		SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	return lResult;
}

void CXTToolBar::SetOwner(CWnd* pOwnerWnd)
{
	ASSERT_VALID(this);
	if (m_hWnd != NULL)
	{
		ASSERT(::IsWindow(m_hWnd));
		DefWindowProc(TB_SETPARENT, (WPARAM)pOwnerWnd->GetSafeHwnd(), 0);
	}
	CXTControlBar::SetOwner(pOwnerWnd);
}

void CXTToolBar::SetSizes(SIZE sizeButton, SIZE sizeImage)
{
	ASSERT_VALID(this);

	// sizes must be non-zero and positive
	ASSERT(sizeButton.cx > 0 && sizeButton.cy > 0);
	ASSERT(sizeImage.cx > 0 && sizeImage.cy > 0);

	// button must be big enough to hold image
	//	 + 7 pixels on x
	//	 + 6 pixels on y
	ASSERT(sizeButton.cx >= sizeImage.cx + 7);
	ASSERT(sizeButton.cy >= sizeImage.cy + 6);

	
	if (::IsWindow(m_hWnd))
	{
		if (xtAfxData.bXPMode)
		{
			if ((GetStyle() & TBSTYLE_LIST) == TBSTYLE_LIST)
			{
				sizeButton.cx -= 4;
				if (::IsWindow(m_hWnd)) SetPadding(CSize(3, 7));
			}
		}

		// set the sizes via TB_SETBITMAPSIZE and TB_SETBUTTONSIZE
		VERIFY(SetBitmapSize(sizeImage));
		VERIFY(SetButtonSize(sizeButton));

		Invalidate();	// just to be nice if called when toolbar is visible
	}
	else
	{
		// just set our internal values for later
		m_sizeButton = sizeButton;
		m_sizeImage = sizeImage;
	}
}

void CXTToolBar::SetHeight(int cyHeight)
{
	ASSERT_VALID(this);

	int nHeight = cyHeight;
	if (m_dwStyle & CBRS_BORDER_TOP)
		cyHeight -= afxData.cyBorder2;
	if (m_dwStyle & CBRS_BORDER_BOTTOM)
		cyHeight -= afxData.cyBorder2;
	m_cyBottomBorder = (cyHeight - m_sizeButton.cy) / 2;
	// if there is an extra pixel, m_cyTopBorder will get it
	m_cyTopBorder = cyHeight - m_sizeButton.cy - m_cyBottomBorder;
	if (m_cyTopBorder < 0)
	{
		TRACE1("Warning: CXTToolBar::SetHeight(%d) is smaller than button.\n",
			nHeight);
		m_cyBottomBorder += m_cyTopBorder;
		m_cyTopBorder = 0;	// will clip at bottom
	}

	// recalculate the non-client region
	SetWindowPos(NULL, 0, 0, 0, 0,
		SWP_DRAWFRAME|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER);
	Invalidate();	// just to be nice if called when toolbar is visible
}

BOOL CXTToolBar::LoadToolBar(LPCTSTR lpszResourceName)
{
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);

	//WINBUG: Set transparent/flat style before loading buttons to allow zero-height
	//	border. This required because of bug in comctl32.dll that always adds
	//	a border, unless flat/transparent.
	DWORD dwStyle = GetStyle();
	ModifyStyle(0, TBSTYLE_FLAT|TBSTYLE_TRANSPARENT);

	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	XT_TOOLBARDATA* pData = (XT_TOOLBARDATA*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;
	ASSERT(pData->wVersion == 1);

	UINT* pItems = (UINT*)_alloca(pData->wItemCount * sizeof(UINT));  // JAK
	int i;
	for (i = 0; i < pData->wItemCount; i++)
		pItems[i] = pData->GetItem(i);
	BOOL bResult = SetButtons(pItems, pData->wItemCount);

	if (bResult)
	{
		// set new sizes of the buttons
		CSize sizeImage(pData->wWidth, pData->wHeight);
		CSize sizeButton(pData->wWidth + 7, pData->wHeight + 7);
		SetSizes(sizeButton, sizeImage);

		// load bitmap now that sizes are known by the toolbar control
		bResult = LoadBitmap(lpszResourceName);
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	LoadButtonInfo();
	
	return bResult;
}

BOOL CXTToolBar::LoadBitmap(LPCTSTR lpszResourceName)
{
	ASSERT_VALID(this);
	ASSERT(lpszResourceName != NULL);

	XTImageManager()->LoadToolBarIcons(lpszResourceName, TRUE, TRUE);

	// determine location of the bitmap in resource fork
	HINSTANCE hInstImageWell = AfxFindResourceHandle(lpszResourceName, RT_BITMAP);
	HRSRC hRsrcImageWell = ::FindResource(hInstImageWell, lpszResourceName, RT_BITMAP);
	if (hRsrcImageWell == NULL)
		return FALSE;

	// load the bitmap
	HBITMAP hbmImageWell = _xtAfxLoadSysColorBitmap(hInstImageWell, hRsrcImageWell);

	// tell common control toolbar about the new bitmap
	if (!AddReplaceBitmap(hbmImageWell))
	{
		if (hInstImageWell)
		{
			::DeleteObject(hInstImageWell);
		}
		return FALSE;
	}

	// remember the resource handles so the bitmap can be recolored if necessary
	m_hInstImageWell = hInstImageWell;
	m_hRsrcImageWell = hRsrcImageWell;
	return TRUE;
}

BOOL CXTToolBar::SetBitmap(HBITMAP hbmImageWell)
{
	ASSERT_VALID(this);
	ASSERT(hbmImageWell != NULL);

	// the caller must manage changing system colors
	m_hInstImageWell = NULL;
	m_hRsrcImageWell = NULL;

	// tell common control toolbar about the new bitmap
	return AddReplaceBitmap(hbmImageWell);
}

BOOL CXTToolBar::AddReplaceBitmap(HBITMAP hbmImageWell)
{
	// need complete bitmap size to determine number of images
	BITMAP bitmap;
	VERIFY(::GetObject(hbmImageWell, sizeof(BITMAP), &bitmap));

	// add the bitmap to the common control toolbar
	BOOL bResult;
	if (m_hbmImageWell == NULL)
	{
		TBADDBITMAP addBitmap;
		addBitmap.hInst = NULL; // makes TBADDBITMAP::nID behave a HBITMAP
		addBitmap.nID = (UINT)hbmImageWell;
		bResult =  DefWindowProc(TB_ADDBITMAP,
			bitmap.bmWidth / m_sizeImage.cx, (LPARAM)&addBitmap) == 0;
	}
	else
	{
		TBREPLACEBITMAP replaceBitmap;
		replaceBitmap.hInstOld = NULL;
		replaceBitmap.nIDOld = (UINT)m_hbmImageWell;
		replaceBitmap.hInstNew = NULL;
		replaceBitmap.nIDNew = (UINT)hbmImageWell;
		replaceBitmap.nButtons = bitmap.bmWidth / m_sizeImage.cx;
		bResult = (BOOL)DefWindowProc(TB_REPLACEBITMAP, 0,
			(LPARAM)&replaceBitmap);
	}
	// remove old bitmap, if present
	if (bResult)
	{
		AfxDeleteObject((HGDIOBJ*)&m_hbmImageWell);
		m_hbmImageWell = hbmImageWell;
	}

	return bResult;
}

BOOL CXTToolBar::SetButtons(const UINT* lpIDArray, int nIDCount)
{
	ASSERT_VALID(this);
	ASSERT(nIDCount >= 1);	// must be at least one of them
	ASSERT(lpIDArray == NULL ||
		AfxIsValidAddress(lpIDArray, sizeof(UINT) * nIDCount, FALSE));

	// delete all existing buttons
	int nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	while (nCount--)
		VERIFY(DefWindowProc(TB_DELETEBUTTON, 0, 0));

	TBBUTTON button; memset(&button, 0, sizeof(TBBUTTON));
	button.iString = -1;
	if (lpIDArray != NULL)
	{
		// add new buttons to the common control
		int iImage = 0;
		int i;
		for (i = 0; i < nIDCount; i++)
		{
			button.fsState = TBSTATE_ENABLED;
			if ((button.idCommand = *lpIDArray++) == 0)
			{
				// separator
				button.fsStyle = TBSTYLE_SEP;
				// width of separator includes 8 pixel overlap
				ASSERT(xtAfxData.iComCtlVersion != -1);
				if ((GetStyle() & TBSTYLE_FLAT) || xtAfxData.iComCtlVersion == VERSION_IE401)
					button.iBitmap = 6;
				else
					button.iBitmap = 8;
			}
			else
			{
				// a command button with image
				button.fsStyle = TBSTYLE_BUTTON;
				button.iBitmap = iImage++;
			}
			if (!DefWindowProc(TB_ADDBUTTONS, 1, (LPARAM)&button))
				return FALSE;
		}
	}
	else
	{
		// add 'blank' buttons
		button.fsState = TBSTATE_ENABLED;
		int i;
		for (i = 0; i < nIDCount; i++)
		{
			ASSERT(button.fsStyle == TBSTYLE_BUTTON);
			if (!DefWindowProc(TB_ADDBUTTONS, 1, (LPARAM)&button))
				return FALSE;
		}
	}
	m_nCount = (int)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	m_bDelayedButtonLayout = TRUE;
	ResetHiddenButtons();

	return TRUE;
}

BOOL CXTToolBar::GetButton(int nIndex, TBBUTTON* pButton) const
{
	CXTToolBar* pBar = (CXTToolBar*)this;

	if(!pBar->SendMessage(TB_GETBUTTON, nIndex, (LPARAM)pButton))
		return FALSE;

	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;

	return TRUE;
}

void CXTToolBar::SetButton(int nIndex, TBBUTTON* pButton)
{
	if (nIndex == -1)
		return;
	
	// get original button state
	TBBUTTON button;
	VERIFY(SendMessage(TB_GETBUTTON, nIndex, (LPARAM)&button));

	// prepare for old/new button comparison
	button.bReserved[0] = 0;
	button.bReserved[1] = 0;
	// TBSTATE_ENABLED == TBBS_DISABLED so invert it
	pButton->fsState ^= TBSTATE_ENABLED;
	pButton->bReserved[0] = 0;
	pButton->bReserved[1] = 0;

	// nothing to do if they are the same
	if (memcmp(pButton, &button, sizeof(TBBUTTON)) != 0)
	{
		// don't redraw everything while setting the button
		DWORD dwStyle = GetStyle();
		ModifyStyle(WS_VISIBLE, 0);
		VERIFY(SendMessage(TB_DELETEBUTTON, nIndex, 0));
		VERIFY(SendMessage(TB_INSERTBUTTON, nIndex, (LPARAM)pButton));
		ModifyStyle(0, dwStyle & WS_VISIBLE);

		// when hiding/showing a separator, make sure its control also gets hidden/shown
		if ((pButton->fsState ^ button.fsState) & TBSTATE_HIDDEN)
		{
			// must be a separator before and after
			if (pButton->fsStyle & button.fsStyle & TBSTYLE_SEP)
			{
				if (pButton->idCommand)
				{
					if (CWnd* pWnd = GetDlgItem(pButton->idCommand))
					{
						if (pButton->fsState & TBSTATE_HIDDEN)
						{
							pWnd->ShowWindow(SW_HIDE);
						}
						else
						{
							int nIndex = CommandToIndex(pButton->idCommand);
							CRect rect;
							GetItemRect(nIndex, &rect);
							pWnd->SetWindowPos(NULL, rect.left, rect.top, 0, 0,
								SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);				
						}
						m_bDelayedButtonLayout = TRUE;
					}
				}
			}
		}

		// invalidate appropriate parts
		if (((pButton->fsStyle ^ button.fsStyle) & TBSTYLE_SEP) ||
			((pButton->fsStyle & TBSTYLE_SEP) && pButton->iBitmap != button.iBitmap))
		{
			// changing a separator
			Invalidate(FALSE);
		}
		else
		{
			// invalidate just the button
			CRect rect;
			if (::SendMessage(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)&rect))
			{
				InvalidateRect(rect, FALSE);	// don't erase background
			}
		}
	}
}

int CXTToolBar::CommandToIndex(UINT nIDFind) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	CXTToolBar* pBar = (CXTToolBar*)this;
	return (int)pBar->DefWindowProc(TB_COMMANDTOINDEX, nIDFind, 0);
}

UINT CXTToolBar::GetItemID(int nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	TBBUTTON button;
	GetButton(nIndex, &button);
	return button.idCommand;
}

void CXTToolBar::GetItemRect(int nIndex, LPRECT lpRect) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// handle any delayed layout
	if (m_bDelayedButtonLayout)
		((CXTToolBar*)this)->Layout();

	// now it is safe to get the item rectangle
	CXTToolBar* pBar = (CXTToolBar*)this;
	if (!pBar->DefWindowProc(TB_GETITEMRECT, nIndex, (LPARAM)lpRect))
		SetRectEmpty(lpRect);
}

BOOL CXTToolBar::IsExpansionButton ()
{
	return true;
}

BOOL CXTToolBar::IsExpansionChevron()
{
	// expansion button is for toolbars
	return !m_mapHiddenButtons.IsEmpty();
}

bool CXTToolBar::IsExpansionCustomizes()
{
	return IsCustomizable();
}

void CXTToolBar::Layout()
{
	ASSERT(m_bDelayedButtonLayout);

	m_bDelayedButtonLayout = FALSE;
	NoteIdealSizeChanged();

	BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
	if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
		((CXTToolBar*)this)->CalcDynamicLayout(0, LM_HORZ | LM_MRUWIDTH | LM_COMMIT);
	else if (bHorz)
		((CXTToolBar*)this)->CalcDynamicLayout(0, LM_HORZ | LM_HORZDOCK | LM_COMMIT);
	else
		((CXTToolBar*)this)->CalcDynamicLayout(0, LM_VERTDOCK | LM_COMMIT);
}

UINT CXTToolBar::GetButtonStyle(int nIndex) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	TBBUTTON button;
	GetButton(nIndex, &button);
	return MAKELONG(button.fsStyle, button.fsState);
}

void CXTToolBar::SetButtonStyle(int nIndex, UINT nStyle)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	TBBUTTON button;
	GetButton(nIndex, &button);
	if (button.fsStyle != (BYTE)LOWORD(nStyle) || button.fsState != (BYTE)HIWORD(nStyle))
	{
		button.fsStyle = (BYTE)LOWORD(nStyle);
		button.fsState = (BYTE)HIWORD(nStyle);
		SetButton(nIndex, &button);
		m_bDelayedButtonLayout = TRUE;
	}
}

CSize CXTToolBar::CalcSize(TBBUTTON* pData, int nCount)
{	
	CPoint cur(0,0);
	CSize sizeResult(0,0);
	int cy = 0;

	DefWindowProc(TB_GETEXTENDEDSTYLE, 0, 0);

	if (nCount > 0)
	{
		int i;
		for (i = 0; i < nCount; i++)
		{
			if (pData[i].fsState & TBSTATE_HIDDEN)
				continue;

			CSize sizeButton = GetButtonSize(pData, i);

			cy = __max(cy, sizeButton.cy);
			sizeResult.cx  = __max(cur.x + sizeButton.cx, sizeResult.cx);
			sizeResult.cy  = __max(cur.y + sizeButton.cy, sizeResult.cy);

			cur.x += sizeButton.cx;

			if (pData[i].fsState & TBSTATE_WRAP)
			{
				cur.x = 0;
				cur.y += cy;
				cy = 0;
				if (pData[i].fsStyle & TBSTYLE_SEP)
					cur.y += sizeButton.cy;
			}
		}
	}
	else
	{
		sizeResult.cx = __max(m_sizeButton.cx, 22);
		sizeResult.cy = __max(m_sizeButton.cy, 22);		
	}

	// check to see if expansion button has to be taken into account
	if (IsExpansionButton() && (IsExpansionCustomizes() || IsExpansionChevron()))
	{
		if (m_bInReBar || !IsFloating())
		{
			if (IsChevronHorizontal())
			{
				sizeResult.cx += EXPANSION_BUTTON_WIDTH + EXPANSION_BUTTON_INSET;
			}
			else
			{
				sizeResult.cy += EXPANSION_BUTTON_WIDTH + EXPANSION_BUTTON_INSET;
			}
		}
	}
	return sizeResult;
}

// Tells if expansion chevron shall point horizontally
bool CXTToolBar::IsChevronHorizontal()
{
	return m_bInReBar ||
		   (m_dwStyle & CBRS_ORIENT_VERT) == 0 ||
		   (m_dwStyle & CBRS_FLOATING);		   
}

int CXTToolBar::WrapToolBar(TBBUTTON* pData, int nCount, int nWidth)
{
	ASSERT(pData != NULL && nCount > 0);

	int nResult = 0;
	int x = 0;
	int i;
	for (i = 0; i < nCount; i++)
	{
		pData[i].fsState &= ~TBSTATE_WRAP;

		if (pData[i].fsState & TBSTATE_HIDDEN)
			continue;

		CSize sizeButton = GetButtonSize(pData, i);
		int dx = sizeButton.cx;
		int dxNext = dx;

		if (x + dx > nWidth)
		{
			BOOL bFound = FALSE;
			int j;
			for (j = i; j >= 0	&&	!(pData[j].fsState & TBSTATE_WRAP); j--)
			{
				// Find last separator that isn't hidden
				// a separator that has a command ID is not
				// a separator, but a custom control.
				if ((pData[j].fsStyle & TBSTYLE_SEP) &&
					(pData[j].idCommand == 0) &&
					!(pData[j].fsState & TBSTATE_HIDDEN))
				{
					bFound = TRUE; i = j; x = 0;
					pData[j].fsState |= TBSTATE_WRAP;
					nResult++;
					break;
				}
			}
			if (!bFound)
			{
				int j;
				for (j = i - 1; j >= 0 && !(pData[j].fsState & TBSTATE_WRAP); j--)
				{
					// Never wrap anything that is hidden,
					// or any custom controls
					if ((pData[j].fsState & TBSTATE_HIDDEN) ||
						((pData[j].fsStyle & TBSTYLE_SEP) &&
						(pData[j].idCommand != 0)))
						continue;

					bFound = TRUE; i = j; x = 0;
					pData[j].fsState |= TBSTATE_WRAP;
					nResult++;
					break;
				}
				if (!bFound)
					x += dxNext;
			}
		}
		else
			x += dxNext;
	}
	return nResult + 1;
}

void CXTToolBar::SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert)
{
	if (pData == NULL || nCount == 0)
	{
		// nothing to size
		return;
	}

	if (!bVert)
	{
		if (nLength == 32767)
		{
			WrapToolBar(pData, nCount, nLength);
		}
		else
		{
			int nMin, nMax, nTarget, nCurrent, nMid;

			// Wrap ToolBar as specified
			nMax = nLength;
			nTarget = WrapToolBar(pData, nCount, nMax);

			// Wrap ToolBar vertically
			nMin = 0;
			nCurrent = WrapToolBar(pData, nCount, nMin);

			if (nCurrent != nTarget)
			{
				while (nMin < nMax)
				{
					nMid = (nMin + nMax) / 2;
					nCurrent = WrapToolBar(pData, nCount, nMid);

					if (nCurrent == nTarget)
						nMax = nMid;
					else
					{
						if (nMin == nMid)
						{
							WrapToolBar(pData, nCount, nMax);
							break;
						}
						nMin = nMid;
					}
				}
			}
		}
		CSize size = CalcSize(pData, nCount);
		WrapToolBar(pData, nCount, size.cx);
	}
	else
	{
		CSize sizeMax, sizeMin, sizeMid;

		// Wrap ToolBar vertically
		WrapToolBar(pData, nCount, 0);
		sizeMin = CalcSize(pData, nCount);

		// Wrap ToolBar horizontally
		WrapToolBar(pData, nCount, 32767);
		sizeMax = CalcSize(pData, nCount);

		while (sizeMin.cx < sizeMax.cx)
		{
			sizeMid.cx = (sizeMin.cx + sizeMax.cx) / 2;
			WrapToolBar(pData, nCount, sizeMid.cx);
			sizeMid = CalcSize(pData, nCount);

			if (nLength < sizeMid.cy)
			{
				if (sizeMin == sizeMid)
				{
					WrapToolBar(pData, nCount, sizeMax.cx);
					return;
				}
				sizeMin = sizeMid;
			}
			else if (nLength > sizeMid.cy)
				sizeMax = sizeMid;
			else
				return;
		}
	}
}

CSize CXTToolBar::CalcLayout(DWORD dwMode, int nLength)
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));
	if (dwMode & LM_HORZDOCK)
		ASSERT(dwMode & LM_HORZ);

	CSize sizeResult(0);
	bool bRedraw = false;

	// Load Buttons
	int nCount = SendMessage(TB_BUTTONCOUNT, 0, 0);
	TBBUTTON* pData = (nCount == 0) ? 0 : (TBBUTTON*)_alloca(nCount * sizeof(TBBUTTON));
	int i;
	for (i = 0; i < nCount; i++)
	{
		GetButton(i, &pData[i]);
	}

	if (!(m_dwStyle & CBRS_SIZE_FIXED))
	{
		BOOL bDynamic = m_dwStyle & CBRS_SIZE_DYNAMIC;

		if (bDynamic && (dwMode & LM_MRUWIDTH))
			SizeToolBar(pData, nCount, m_nMRUWidth);
		else if (bDynamic && (dwMode & LM_HORZDOCK))
			SizeToolBar(pData, nCount, 32767);
		else if (bDynamic && (dwMode & LM_VERTDOCK))
			SizeToolBar(pData, nCount, 0);
		else if (bDynamic && (nLength != -1))
		{
			CRect rect; rect.SetRectEmpty();
			CalcInsideRect(rect, (dwMode & LM_HORZ));
			BOOL bVert = (dwMode & LM_LENGTHY);
			int nLen = nLength + (bVert ? rect.Height() : rect.Width());

			SizeToolBar(pData, nCount, nLen, bVert);
		}
		else if (bDynamic && (m_dwStyle & CBRS_FLOATING))
			SizeToolBar(pData, nCount, m_nMRUWidth);
		else
			SizeToolBar(pData, nCount, (dwMode & LM_HORZ) ? 32767 : 0);
	}

	sizeResult = CalcSize(pData, nCount);

	if (dwMode & LM_COMMIT)
	{
		CArray<XT_CONTROLPOS, XT_CONTROLPOS&> controls;
		BOOL bIsDelayed = m_bDelayedButtonLayout;
		m_bDelayedButtonLayout = FALSE;

		int i;
		for (i = 0; i < nCount; i++)
		{
			if ((pData[i].fsStyle & TBSTYLE_SEP) &&
				(pData[i].idCommand != 0) &&
				!(pData[i].fsState & TBSTATE_HIDDEN))
			{
				XT_CONTROLPOS controlPos;
				controlPos.nIndex = i;
				controlPos.nID = pData[i].idCommand;

				CRect rect;
				GetItemRect(i, &rect);
				ClientToScreen(&rect);
				controlPos.rectOldPos = rect;

				controls.Add(controlPos);
			}
		}

		if ((m_dwStyle & CBRS_FLOATING) && (m_dwStyle & CBRS_SIZE_DYNAMIC))
			m_nMRUWidth = sizeResult.cx;
		for (i = 0; i < nCount; i++)
			SetButton(i, &pData[i]);

		if (controls.GetSize() > 0)
		{
			int i;
			for (i = 0; i < controls.GetSize(); i++)
			{
				CWnd* pWnd = GetDlgItem(controls[i].nID);
				if (pWnd != NULL)
				{
					CRect rect;
					pWnd->GetWindowRect(&rect);
					CPoint pt = rect.TopLeft() - controls[i].rectOldPos.TopLeft();
					GetItemRect(controls[i].nIndex, &rect);
					pt = rect.TopLeft() + pt;
					pWnd->SetWindowPos(NULL, pt.x, pt.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
				}
			}
		}

		// show/hide/reposition the expansion button
		if (m_pExpButton)
		{
			bool bFloating = (m_dwStyle & CBRS_FLOATING) != 0;
			if (!bFloating && (IsExpansionCustomizes() || IsExpansionChevron()))
			{
				if (dwMode & LM_COMMIT)
				{
					int cx, cy;
					if (IsChevronHorizontal())
					{
						cx = EXPANSION_BUTTON_WIDTH;
						cy = sizeResult.cy - 2 * EXPANSION_BUTTON_INSET;
					}
					else
					{
						cx = sizeResult.cx - 2 * EXPANSION_BUTTON_INSET;
						cy = EXPANSION_BUTTON_WIDTH;
					}
					CRect rectButton;
					m_pExpButton ->GetWindowRect(&rectButton);
					ScreenToClient(rectButton);
					if (rectButton.Width() != cx || rectButton.Height() != cy || !m_pExpButton ->IsWindowVisible())
					{
						m_pExpButton ->SetWindowPos(NULL, 0, 0, cx, cy,
							SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOMOVE );
						bRedraw = true;
					}
				}
			}
			else
			{
				if (dwMode & LM_COMMIT)
				{
					if (m_pExpButton ->IsWindowVisible())
					{
						m_pExpButton ->ShowWindow(SW_HIDE);
						bRedraw = true;
					}
				}
			}
		}

		m_bDelayedButtonLayout = bIsDelayed;
	}

	//BLOCK: Adjust Margins
	{
		CRect rect; rect.SetRectEmpty();
		CalcInsideRect(rect, (dwMode & LM_HORZ));
		sizeResult.cy -= rect.Height();
		sizeResult.cx -= rect.Width();

		CSize size = CControlBar::CalcFixedLayout((dwMode & LM_STRETCH), (dwMode & LM_HORZ));
		sizeResult.cx = __max(sizeResult.cx, size.cx);
		sizeResult.cy = __max(sizeResult.cy, size.cy);
	}

	if (bRedraw)
	{
		// force update of the UI state 
		PostMessage(WM_IDLEUPDATECMDUI, true);
		//RedrawWindow(0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}

	return sizeResult;
}

CSize CXTToolBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	DWORD dwMode = bStretch ? LM_STRETCH : 0;
	dwMode |= bHorz ? LM_HORZ : 0;

	return CalcLayout(dwMode);

}

CSize CXTToolBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	if (nLength == 0 && 
		dwMode == (LM_HORZ | LM_MRUWIDTH | LM_COMMIT))
	{
		// CControlBar::SetBarState calls the function with these
		// parameters. Defer button layout since they may cause
		// false recalc when say toolbar is docked
		// vertically and the state gets restored when the toolbar
		// is already docked
		m_bDelayedButtonLayout = true;
	}
	if ((nLength == -1) && !(dwMode & LM_MRUWIDTH) && !(dwMode & LM_COMMIT) &&
		((dwMode & LM_HORZDOCK) || (dwMode & LM_VERTDOCK)))
	{
		return CalcFixedLayout(dwMode & LM_STRETCH, dwMode & LM_HORZ);
	}

	return CalcLayout(dwMode, nLength);
}

void CXTToolBar::GetButtonInfo(int nIndex, UINT& nID, UINT& nStyle, int& iImage) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	TBBUTTON button;
	GetButton(nIndex, &button);
	nID = button.idCommand;
	nStyle = MAKELONG(button.fsStyle, button.fsState);
	iImage = button.iBitmap;
}

void CXTToolBar::SetButtonInfo(int nIndex, UINT nID, UINT nStyle, int iImage)
{
	ASSERT_VALID(this);

	TBBUTTON button;
	GetButton(nIndex, &button);
	TBBUTTON save;
	memcpy(&save, &button, sizeof(save));
	button.idCommand = nID;
	button.iBitmap = iImage;
	button.fsStyle = (BYTE)LOWORD(nStyle);
	button.fsState = (BYTE)HIWORD(nStyle);
	if (memcmp(&save, &button, sizeof(save)) != 0)
	{
		SetButton(nIndex, &button);
		m_bDelayedButtonLayout = TRUE;
	}
}

INT_PTR CXTToolBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	ASSERT_VALID(this);
	ASSERT(::IsWindow(m_hWnd));

	// check child windows first by calling CXTControlBar
	int nHit = CXTControlBar::OnToolHitTest(point, pTI);
	if (nHit != -1)
		return nHit;

	// now hit test against CXTToolBar buttons
	CXTToolBar* pBar = (CXTToolBar*)this;
	int nButtons = (int)pBar->DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	int i;
	for (i = 0; i < nButtons; i++)
	{
		CRect rect;
		TBBUTTON button;
		if (pBar->DefWindowProc(TB_GETITEMRECT, i, (LPARAM)&rect))
		{
			++rect.bottom;
			++rect.right;
			if (rect.PtInRect(point) &&
				pBar->DefWindowProc(TB_GETBUTTON, i, (LPARAM)&button) &&
				!(button.fsStyle & TBSTYLE_SEP))
			{
				int nHit = GetItemID(i);
				if (pTI != NULL && pTI->cbSize >= sizeof(XT_OLDTOOLINFO))
				{
					// if we do not display screen tips, return -1.
                    if ( !xtAfxData.bToolBarScreenTips )
					{
                        return -1;
                    }
					
					pTI->hwnd     = m_hWnd;
					pTI->rect     = rect;
					pTI->uId      = nHit;
					pTI->lpszText = LPSTR_TEXTCALLBACK;
                    
					// if we display screen tips with accelerator tips...
                    if ( xtAfxData.bToolBarAccelTips )
                    {
						TCHAR szButtonText[ 256 ];
						
						CString strTipText;
						AfxLoadString( button.idCommand, szButtonText );
						AfxExtractSubString( strTipText, szButtonText, 1, _T( '\n' ) );
						
						if ( !strTipText.IsEmpty( ) )
						{
							CString strAccel;
							CXTAccelManager::Get().GetKeyAssignmentStrings( button.idCommand, strAccel );
							
							if ( !strAccel.IsEmpty( ) )
							{
								int iFound = strAccel.Find(_T(','));
								
								if (iFound != -1) {
									strAccel = strAccel.Left( iFound );
								}
								
								strTipText += _T( " (" );
								strTipText += strAccel;
								strTipText += _T( ")" );
								
								pTI->lpszText = _tcsdup( strTipText );
								_tcscpy( pTI->lpszText, strTipText );
							}
						}
                    }
				}
                
				// found matching rect, return the ID of the button
				return nHit != 0 ? nHit : -1;
			}
		}
	}
	return -1;
}

int CXTToolBar::CacheString(LPCTSTR lpszText, UINT idCommand)
{
	// attempt to lookup string index in map
	int nString = -1;
	void* p;
	if (m_pStringMap != NULL && m_pStringMap->Lookup(lpszText, p))
	{
		nString = (int)p;
	}
	else
	{
		// add new string if not already in map

		// initialize map if necessary
		if (m_pStringMap == NULL)
			m_pStringMap = new CMapStringToPtr;

		// add new string to toolbar list
		CString strTemp(lpszText, lstrlen(lpszText)+1);
		nString = (int)DefWindowProc(TB_ADDSTRING, 0, (LPARAM)(LPCTSTR)strTemp);
		if (nString == -1)
			return FALSE;

		// cache string away in string map
		m_pStringMap->SetAt(lpszText, (void*)nString);
		ASSERT(m_pStringMap->Lookup(lpszText, p));

		// cache our string to command map.
		CString strText = lpszText;
		m_mapBtnTextCmd.SetAt(idCommand, strText);
		ASSERT(m_mapBtnTextCmd.Lookup(idCommand, strText));
	}
	return nString;
}

BOOL CXTToolBar::SetButtonText(int nIndex, LPCTSTR lpszText)
{
	// assert the index is correct
	ASSERT(nIndex >= 0);
	
	// get the button at the specified index.
	TBBUTTON button;
	GetButton(nIndex, &button);

	// cache the string
	int nString = CacheString(lpszText, button.idCommand);

	// change the toolbar button description
	if (button.iString != nString)
	{
		button.iString = nString;
		SetButton(nIndex, &button);
		DeferFreshenAfter();
	}

	return TRUE;
}

CString CXTToolBar::GetButtonText(int nIndex) const
{
	CString strResult;
	GetButtonText(nIndex, strResult);
	return strResult;
}

void CXTToolBar::GetButtonText(int nIndex, CString& rString) const
{
	if (m_pStringMap != NULL)
	{
		// get button information (need button.iString)
		TBBUTTON button;
		GetButton(nIndex, &button);

		// look in map for matching iString
		POSITION pos = m_pStringMap->GetStartPosition();
		CString str; void* p;
		while (pos)
		{
			m_pStringMap->GetNextAssoc(pos, str, p);
			if ((int)p == button.iString)
			{
				rString = str;
				return;
			}
		}
	}
	rString.Empty();
}

CSize CXTToolBar::GetButtonSize(TBBUTTON* pData, int iButton)
{
	ASSERT(xtAfxData.iComCtlVersion != -1);

	// Get the actual size of the button, not what's in m_sizeButton.
	// Make sure to do SendMessage instead of calling MFC's GetItemRect,
	// which has all sorts of bad side-effects! (Go ahead, take a look at it.)
	CRect rc;
	::SendMessage(m_hWnd, TB_GETITEMRECT, iButton, (LPARAM)&rc);
	CSize size = rc.Size();
	DWORD dwStyle = pData[iButton].fsStyle;
	if ((pData[iButton].fsState & TBSTATE_WRAP) && (dwStyle & TBSTYLE_SEP))
	{
		size.cy = size.cx;
		size.cx = 0;
	}
	return size;
}

void CXTToolBar::OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle)
{
	// a dynamically resizeable toolbar can not have the CBRS_FLOAT_MULTI
	ASSERT(!((dwNewStyle & CBRS_SIZE_DYNAMIC) &&
		(m_dwDockStyle & CBRS_FLOAT_MULTI)));

	// a toolbar can not be both dynamic and fixed in size
	ASSERT (!((dwNewStyle & CBRS_SIZE_FIXED) &&
		(dwNewStyle & CBRS_SIZE_DYNAMIC)));

	// CBRS_SIZE_DYNAMIC can not be disabled once it has been enabled
	ASSERT (((dwOldStyle & CBRS_SIZE_DYNAMIC) == 0) ||
		((dwNewStyle & CBRS_SIZE_DYNAMIC) != 0));

	if (m_hWnd != NULL &&
		((dwOldStyle & CBRS_BORDER_ANY) != (dwNewStyle & CBRS_BORDER_ANY)))
	{
		// recalc non-client area when border styles change
		SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_DRAWFRAME);
	}

	// defer button recalc only if the style really changes 
	// (changes in border style obviously do not apply)
	// NB: this is important to avoid looping when toolbar
	// is docked, a dock bar changes toolbar style to apply
	// borders as applicable to its current layout
	DWORD dwOldStyleNoBorder = dwOldStyle & ~CBRS_BORDER_ANY;
	DWORD dwNewStyleNoBorder = dwNewStyle & ~CBRS_BORDER_ANY;
	if (dwOldStyleNoBorder != dwNewStyleNoBorder)
	{
		m_bDelayedButtonLayout = TRUE;
	}

	//WINBUG: Recalculate everything when the bar goes from docked to
	//	undocked because the AdjustSize calculation happens when the bar
	//	is in the old state, and thus wrong. After the bar is docked/undocked,
	//	recalculate with the new style and commit the change.
	if (dwOldStyle != dwNewStyle)
	{
		DWORD dwMode = 0;
		if ((dwNewStyle & CBRS_SIZE_DYNAMIC) && (dwNewStyle & CBRS_FLOATING))
			dwMode = LM_HORZ | LM_MRUWIDTH;
		else if (dwNewStyle & CBRS_ORIENT_HORZ)
			dwMode = LM_HORZ | LM_HORZDOCK;
		else
			dwMode =  LM_VERTDOCK;

		// reset hidden buttons
		ResetHiddenButtons();

		CalcDynamicLayout(-1, dwMode | LM_COMMIT);
	}
}

void CXTToolBar::ResetHiddenButtons()
{
	POSITION pos = m_mapHiddenButtons.GetStartPosition();
	while (pos)
	{
		int nIndex;
		XT_HIDDENBUTTON button;
		m_mapHiddenButtons.GetNextAssoc(pos, nIndex, button);

		TBBUTTON tbButton;
		GetButton(nIndex, &tbButton);

		if ((tbButton.fsState & TBSTATE_HIDDEN))
		{
			tbButton.fsState &= ~TBSTATE_HIDDEN;
			SetButton(nIndex, &tbButton);
			m_bDelayedButtonLayout = TRUE;
		}
	}
	m_mapHiddenButtons.RemoveAll();
}


void CXTToolBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CXTToolCmdUI state;
	state.m_pOther = this;

	state.m_nIndexMax = (UINT)DefWindowProc(TB_BUTTONCOUNT, 0, 0);
	for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++)
	{
		// get buttons state
		TBBUTTON button;
		GetButton(state.m_nIndex, &button);
		state.m_nID = button.idCommand;

		// ignore separators
		if (!(button.fsStyle & TBSTYLE_SEP))
		{
			// allow reflections
			if (CWnd::OnCmdMsg(0,
				MAKELONG((int)CN_UPDATE_COMMAND_UI, WM_COMMAND+WM_REFLECT_BASE),
				&state, NULL))
				continue;

			// allow the toolbar itself to have update handlers
			if (CWnd::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
				continue;

			// allow the owner to process the update
			state.DoUpdate(pTarget, bDisableIfNoHndler);
		}
	}

	// update the dialog controls added to the toolbar
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

// Add dropdown buttons.
// The manager takes care of setting appropriate styles, etc.
//
// Args:
//		- array of LONGs: MAKELONG(commandID, menuID)
//		- number of buttons


XT_DROPDOWNBUTTON* CXTToolBar::InitDropDownButton(UINT nIDButton, BOOL bArrow)
{
	ASSERT_VALID(this);

	XT_DROPDOWNBUTTON* pDDButton = FindDropDownButton(nIDButton);

	if (!pDDButton)
	{
		pDDButton = new XT_DROPDOWNBUTTON;
		pDDButton->bArrow = bArrow;
		ASSERT(pDDButton);

		POSITION pos;
		for (pos = m_listTBButtons.GetHeadPosition(); pos != NULL; m_listTBButtons.GetNext(pos))
		{
			XT_TBBUTTON& tbButton = m_listTBButtons.GetAt(pos);
			if (tbButton.idCommand == (int)nIDButton)
			{
                // if a button was already defined, delete it.
                SAFE_DELETE( tbButton.pDDButton );

                // set the drop button pointer and style.
				tbButton.pDDButton = pDDButton;
				tbButton.fsStyle |= TBSTYLE_DROPDOWN;
				break;
			}
		}
	}

	// extended style must be set before changing the button style
	// it appears that toolbar control does not recalc TBSTYLE_EX_DRAWDDARROWS
	// is set (a winbug most likely)
	if (bArrow) {
		SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	}

	int iButton = CommandToIndex(nIDButton);

	DWORD dwStyle = GetButtonStyle(iButton);
	dwStyle |= TBSTYLE_DROPDOWN;
	SetButtonStyle(iButton, dwStyle);

	pDDButton->idButton = nIDButton;
	return pDDButton;
}

BOOL CXTToolBar::AddDropDownButton(UINT nIDButton, UINT nIDMenu, BOOL bArrow/*=TRUE*/)
{
	XT_DROPDOWNBUTTON* pDropButton = InitDropDownButton(nIDButton, bArrow);
	ASSERT(pDropButton != NULL);

	if (pDropButton)
	{
		pDropButton->idMenu = nIDMenu;
		return TRUE;
	}

	return FALSE;
}

BOOL CXTToolBar::AddDropDownButton(UINT nIDButton, COLORREF clrColor,
	COLORREF clrDefault/*=CLR_DEFAULT*/, DWORD dwPopup/*=CPS_XT_EXTENDED|CPS_XT_MORECOLORS*/,
	BOOL bShowColor/*=TRUE*/, BOOL bArrow/*=TRUE*/)
{
	XT_DROPDOWNBUTTON* pDropButton = InitDropDownButton(nIDButton, bArrow);
	ASSERT(pDropButton != NULL);


	if (pDropButton)
	{
		pDropButton->bColorPicker = TRUE;
		pDropButton->clrDefault   = clrDefault;
		pDropButton->clrColor	  = clrColor;
		pDropButton->dwPopup	  = dwPopup;
		pDropButton->bShowColor   = bShowColor;

		return TRUE;
	}

	return FALSE;
}

// Find buttons structure for given ID

XT_DROPDOWNBUTTON* CXTToolBar::FindDropDownButton(UINT nID)
{
	POSITION pos;
	for (pos = m_listTBButtons.GetHeadPosition(); pos != NULL; m_listTBButtons.GetNext(pos))
	{
		XT_TBBUTTON& tbButton = m_listTBButtons.GetAt(pos);
		if (tbButton.pDDButton && tbButton.pDDButton->idButton == nID)
		{
			return tbButton.pDDButton;
		}
	}
	return NULL;
}


// Handles TBN_HOTITEMCHANGE
//
void CXTToolBar::OnToolBarHotItemChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTBHOTITEM* lpnmhi = (LPNMTBHOTITEM) pNMHDR;

	if (m_pDockContext && m_pDockContext->m_bDragging )
	{
		lpnmhi->idNew = 0;
		*pResult = 1;
		return;
	}


	// do not change the hot item if mouse button is pressed
	*pResult = 0;
	if (lpnmhi->idOld != lpnmhi->idNew)
	{
		if (GetCapture() == this)
		{
			// not allowed
			*pResult = 1;
		}
	}
}


// Handle TBN_DROPDOWN
// Default is to display the specified menu at the right place.
// You can override to generate dynamic menus
//
// Args:
//		- NMTOOLBAR struct from TBN_DROPDOWN
//		- command id of button
//		- point to display menu at

void CXTToolBar::OnToolBarBtnDropDown(NMHDR* pNMHDR, LRESULT* pResult)
{
	if (!::IsWindow(m_hWnd))
		return;

	UNUSED_ALWAYS(pResult);
	NMTOOLBAR* pNMTB = (NMTOOLBAR*)pNMHDR;

	// get location of button
	CRect rect;
	GetRect(pNMTB->iItem, rect);
	CRect rectItem = rect;
	ClientToScreen(&rect);

	if (m_uiLastCmd == (UINT)pNMTB->iItem)
	{
		if (m_pColorPopup && ::IsWindow(m_pColorPopup->m_hWnd))
		{
			return;
		}
	}

	// ensure toolbar has proper visual appearance
	rectItem.InflateRect(4, 4);
	InvalidateRect(&rectItem);

	// call virtual function to display dropdown menu
	OnDropDownButton(pNMTB, pNMTB->iItem, rect);
}

// Virtual fn you can override to hand drop-down button
// events with more friendly args

void CXTToolBar::OnDropDownButton(NMTOOLBAR* pNMTB, UINT nID, CRect rect)
{
	UNUSED_ALWAYS(nID);
	XT_DROPDOWNBUTTON* pDropButton = FindDropDownButton(pNMTB->iItem);

	int iDroppedItem = CommandToIndex(pNMTB->iItem);
	if (pDropButton && IsDroppedItem(iDroppedItem))
		return;
	SetDroppedItem(iDroppedItem, true);

	if (pDropButton)
	{
		GetItemRect(iDroppedItem, &CXTCoolMenu::m_rectExclude);
		ClientToScreen(&CXTCoolMenu::m_rectExclude);
		CXTCoolMenu::m_iMenuBarAlign = CBRS_ALIGN_TOP;

		if (pDropButton->idMenu)
		{
			// load and display popup menu
			CXTMenu menu;
			VERIFY(menu.LoadMenu(pDropButton->idMenu));

			CXTMenu* pPopup = menu.GetSubMenu(0);
			TrackMenuPopup(pPopup, rect);

			IXTToolbarProxy* proxy = IXTSkin::GetInstance()->GetToolbarProxy();
			if (proxy != NULL)
			{
				// delegate actual drawing to it
				proxy->RemoveDroppedEffects(this, iDroppedItem);
			}
			SetDroppedItem(iDroppedItem, false);

			Invalidate();
		}
		else
		if (pDropButton->bMenuBarPopup)
		{
			TrackMenuBarPopup(pDropButton->dwMenuBarPopupID, rect);
			SetDroppedItem(iDroppedItem, false);
		}
		else
		{
			if (pDropButton->bColorPicker)
			{
				// If the popup exists, destroy the window.
				if (m_pColorPopup && ::IsWindow(m_pColorPopup->m_hWnd)) {
					m_pColorPopup->DestroyWindow();
				}

				// Free any previously allocated memory.
				SAFE_DELETE(m_pColorPopup);


				m_uiLastCmd = pNMTB->iItem;

				// Create the color picker popup window.
				m_pColorPopup = CreateNewColorPopup(pDropButton, rect);
				ASSERT(::IsWindow(m_pColorPopup->m_hWnd));
			}
			else
			{
				CWnd* pOwner = GetOwner();
				ASSERT_VALID(pOwner);

				// this assumes callback works in modal state if return value is 0.
				if (pOwner->SendMessage(CBRN_XT_DROPDOWN, (WPARAM)pNMTB, (LPARAM)&rect) == 0)
				{
					if (IXTToolbarProxy* proxy = IXTSkin::GetInstance()->GetToolbarProxy())
					{
						// delegate actual drawing to it
						proxy->RemoveDroppedEffects(this, iDroppedItem);
					}
					SetDroppedItem(iDroppedItem, false);
				}
				if (::GetKeyState(VK_LBUTTON) < 0)
				{
					m_bInvalidateOnMouseUp = true;
					SetCapture();
				}
			}
		}
		CXTCoolMenu::m_rectExclude.SetRectEmpty();
		CXTCoolMenu::m_iMenuBarAlign = 0;
	}

	RedrawWindow(0,0,RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

// track instantiated menu popup
void CXTToolBar::TrackMenuPopup(CXTMenu* pPopup, const CRect& rect)
{
	ASSERT(pPopup);

	CXTCoolMenu::m_iMenuBarAlign = CBRS_ALIGN_TOP;

	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);

	CRect rcExclude = rect;
	if (xtAfxData.bXPMode)
		rcExclude.DeflateRect(0, 1, 0, 1);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,
		rcExclude.left, rcExclude.bottom, pOwner, &rcExclude);
}

// Creates and initializes color picker popup 
CXTColorPopup* CXTToolBar::CreateNewColorPopup(XT_DROPDOWNBUTTON* pDropButton, CRect rect)
{
	CXTColorPopup* pColorPopup = new CXTColorPopup(FALSE, m_uiLastCmd);
	ASSERT(pColorPopup);
	pColorPopup->DisplayShadow(TRUE);

	pColorPopup->Create(rect, this, pDropButton->dwPopup,
			pDropButton->clrColor, pDropButton->clrDefault);
	return pColorPopup;
}

LRESULT CXTToolBar::OnSelEndOK(WPARAM wParam, LPARAM lParam)
{
	UINT uiLastCmd = (UINT)lParam;

	// get the color for the toolbar button.
	XT_DROPDOWNBUTTON* pDropButton = FindDropDownButton(uiLastCmd);
	ASSERT(pDropButton);
	pDropButton->clrColor = (COLORREF)wParam;

	// redraw to reflect the change
	Invalidate();
	
	// pass message on to parent.
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);
	pOwner->SendMessage(CPN_XT_SELENDOK, wParam, lParam);

	return 0;
}

LRESULT CXTToolBar::OnCloseUp(WPARAM wParam, LPARAM lParam)
{
	UNUSED_ALWAYS(lParam);

	// set the color for the toolbar button.
	CRect rcItem;
	int iDroppedItem = CommandToIndex(m_uiLastCmd);
	SendMessage(TB_GETITEMRECT, iDroppedItem, (LPARAM)&rcItem);
	if (IXTToolbarProxy* proxy = IXTSkin::GetInstance()->GetToolbarProxy())
	{
		// delegate actual drawing to it
//TBD		proxy->RemoveDroppedEffects(this, m_iDroppedItem);
	}
	SetDroppedItem(iDroppedItem, false);
	Invalidate();
	

//TBD	m_iDroppedItem = -1;

	// pass message on to parent.
	CWnd* pOwner = GetOwner();
	ASSERT_VALID(pOwner);
	pOwner->SendMessage(CPN_XT_CLOSEUP, wParam, (LPARAM)m_uiLastCmd);

	m_uiLastCmd = (UINT)-1;

	return 0;
}

void CXTToolBar::PositionControl(CWnd* pWnd)
{
	ASSERT_VALID( pWnd ); // must be valid.

	if (pWnd != NULL)
	{
		// get the control id for the CWnd.
		int iCmdID = pWnd->GetDlgCtrlID();
		ASSERT( iCmdID >= 0 );

		// if this button was hidden hide the associated control...
		if (GetToolBarCtrl().IsButtonHidden(iCmdID))
		{
			pWnd->ShowWindow(SW_HIDE);
		}

		// otherwise move the control accordingly.
		else
		{
			pWnd->ShowWindow(SW_SHOWNA);
			
			// get the button index for this command.
			int iIndex = CommandToIndex(iCmdID);
			ASSERT(iIndex != -1);
			
			// get the size of the control.
			CRect rcCtrl;
			pWnd->GetWindowRect(&rcCtrl);
			ScreenToClient(&rcCtrl);
			
			// set the button style for a separator.
			SetButtonInfo(iIndex, iCmdID, TBBS_SEPARATOR, rcCtrl.Width());
			
			// get the size of the toolbar button.
			CRect rcItem;
			GetItemRect( iIndex, &rcItem );

			if (rcItem != rcCtrl)
			{
				// Move the control to its correct position in the toolbar, DO NOT RESIZE!!!.
				pWnd->SetWindowPos( NULL, rcItem.left, rcItem.top, 0, 0,
					SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOSIZE );

				// Make sure the control is correctly centered in the toolbar vertically.
				CRect rcCtrlNew;
				pWnd->GetWindowRect(&rcCtrlNew);
				ScreenToClient(&rcCtrlNew);

				if (rcCtrlNew.Height() != rcItem.Height())
				{
					int y = (rcItem.top + rcItem.bottom - rcCtrlNew.Height()) / 2;
					if (y < 0)
					{
						y = 0;
					}

					pWnd->SetWindowPos( NULL, rcItem.left, y, 0, 0,
						SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS );

				}			
			}
		}
	}
}

void CXTToolBar::PositionControls()
{
	// loop thru all of our buttons that have controls
	// and move the controls to the correct location.
	POSITION pos;
	for (pos = m_listTBButtons.GetHeadPosition(); pos != NULL; m_listTBButtons.GetNext(pos))
	{
		XT_TBBUTTON& tbButton = m_listTBButtons.GetAt(pos);
		if (tbButton.pWndCtrl)
		{
			PositionControl(tbButton.pWndCtrl);
		}
	}

	// loop again...
	// this will cause any controls that are placed side by side and swapped
	// indexes during customization to be correctly positioned.

	for (pos = m_listTBButtons.GetHeadPosition(); pos != NULL; m_listTBButtons.GetNext(pos))
	{
		XT_TBBUTTON& tbButton = m_listTBButtons.GetAt(pos);
		if (tbButton.pWndCtrl)
		{
			PositionControl(tbButton.pWndCtrl);
		}
	}

	// force the frame window to recalculate the size
	CFrameWnd* pFrame = GetParentFrame();
	if (pFrame && pFrame->IsFrameWnd())
	{
		pFrame->RecalcLayout();
	}

	// resize the toolbar.
	AutoSizeToolbar();
}

BOOL CXTToolBar::InsertControl(CWnd* pWnd, bool bBoldFont/*=false*/)
{
	ASSERT_VALID(pWnd);

	// get the control id for the CWnd.
	int iCmdID = pWnd->GetDlgCtrlID();
	ASSERT( iCmdID >= 0 );
	
	POSITION pos;
	for (pos = m_listTBButtons.GetHeadPosition(); pos != NULL; m_listTBButtons.GetNext(pos))
	{
		XT_TBBUTTON& tbButton = m_listTBButtons.GetAt(pos);
		if (tbButton.idCommand == iCmdID)
		{
			tbButton.pWndCtrl  = pWnd;
            tbButton.bBoldFont = bBoldFont;

            pWnd->SetFont(
                bBoldFont ? &m_fontBold : &m_font);

			PositionControl(tbButton.pWndCtrl);
			break;
		}
	}

	return TRUE;
}

void CXTToolBar::OnPaint()
{
	if (m_bDelayedButtonLayout)
		Layout();

	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// eliminate screen flicker.
	CXTMemDC memDC(&dc, rectClient);

	if (IsFillToolBarClientRect())
	{
		memDC.FillSolidRect(rectClient, GetBackgroundColor());
	}
	// Now let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
}

// tells if client rectangle shall be filled with background color
bool CXTToolBar::IsFillToolBarClientRect()
{
	return xtAfxData.bXPMode && !InRebar();
}

//////////////////////////////////////////////////////////////////////
// TOOLBAR CUSTOMIZATION
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// This function is called when the user initially calls up the toolbar
// customization dialog box.
//////////////////////////////////////////////////////////////////////

void CXTToolBar::OnToolBarBeginAdjust(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPTBNOTIFY lpTBNotify = (LPTBNOTIFY)pNMHDR;
	ASSERT(lpTBNotify != NULL);

	*pResult = 0;
	if (!m_bRestoringState)
	{
		if (IsExpansionButton())
		{
			if (IsExpansionChevron())
			{
				// display all dynamically hidden buttons 
				ResetHiddenButtons();
			}
			m_bAdjusting = true;
			DeferFreshenAfter();
		}
		else
		{
			m_bAdjusting = true;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// This function is called when the user dismisses the toolbar customization
// dialog box.
//////////////////////////////////////////////////////////////////////

void CXTToolBar::OnToolBarEndAdjust(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	UNREFERENCED_PARAMETER(pResult);

	m_bAdjusting = false;

	if (!m_bRestoringState)
	{
		// save the state of the toolbar for reinitialization
		SaveState();
	}
}

//////////////////////////////////////////////////////////////////////
// This function is called when the user clicks on the help button on the
// toolbar customization dialog box.
//////////////////////////////////////////////////////////////////////

void CXTToolBar::OnToolBarCustomHelp(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	UNREFERENCED_PARAMETER(pResult);

	// Sample displays a message box but a valid help topic
	// can be displayed for the customize dialog for this toolbar
	AfxMessageBox(_T("Help not implemented!"), MB_ICONINFORMATION);
}

//////////////////////////////////////////////////////////////////////
// This function is called to populate the toolbar customization dialog box
// with information regarding all of the possible toolbar buttons.
//////////////////////////////////////////////////////////////////////

void CXTToolBar::OnToolBarGetButtonInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	// data needed by customize dialog box
	LPTBNOTIFY lpTBNotify = (LPTBNOTIFY)pNMHDR;
	ASSERT(lpTBNotify != NULL);

	*pResult = TRUE;

	int iItem = lpTBNotify->iItem;

	// if the index is valid
	if (0 <= iItem && iItem < m_listTBButtons.GetCount())
	{
		// copy the stored button structure
		POSITION pos = m_listTBButtons.FindIndex(iItem);
		if (pos == NULL)
		{
			*pResult = FALSE;
			return;
		}

		// get the button text from our command to text map

		XT_TBBUTTON& tbButton = m_listTBButtons.GetAt( pos );
		if (tbButton.idCommand > 0 && (tbButton.fsStyle & TBSTYLE_SEP) == 0)
		{
			// check to see if this button has text associated with it, if not found
			// construct the button text from the string table if possible...
			CString strBtnText;
			if (!m_mapBtnTextCmd.Lookup(tbButton.idCommand, strBtnText))
			{
				CString strBuffer;
				strBuffer.LoadString(tbButton.idCommand);
				if (!strBuffer.IsEmpty())
				{
					int iNewLine = strBuffer.Find(_T('\n'));
					
					if (iNewLine >= 0)
					{
						int iCount = strBuffer.GetLength() - iNewLine - 1;
						strBtnText = strBuffer.Right(iCount);
						
						if (strBtnText.IsEmpty())
							strBtnText = strBuffer.Left(iNewLine);
					}
				}
			}
			else
			{
				// see if we can find the string in our string to index map, if
				// so, set the string index for the button.
				if (m_pStringMap != NULL)
				{
					void* p;
					if (m_pStringMap->Lookup(strBtnText, p)) {
						tbButton.iString = (int)p;
					}
				}
			}

			// copy the text for the button label in the dialog
			if (!strBtnText.IsEmpty())
			{
				_tcscpy( lpTBNotify->pszText, strBtnText );
				lpTBNotify->cchText = strBtnText.GetLength();
			}
		}

		// initialize the lpTBNotify.
		lpTBNotify->tbButton = tbButton;

		// see if there are any controls in the toolbar
		if (tbButton.pWndCtrl != NULL)
			PositionControl(tbButton.pWndCtrl);

		// indicate valid data was sent
		*pResult = TRUE;
	}

	// else there is no button for this index
	else
	{
		*pResult = FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
// This function is called when the user begins dragging a toolbar
// button or when the customization dialog is being populated with
// toolbar information.  Basically, *pResult should be populated with
// your answer to the question, "is the user allowed to delete this
// button?".
//////////////////////////////////////////////////////////////////////

void CXTToolBar::OnToolBarQueryDelete(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPTBNOTIFY lpTBNotify = (LPTBNOTIFY)pNMHDR;
	ASSERT(lpTBNotify != NULL);

	BOOL bIsHidden = GetToolBarCtrl().IsButtonHidden(
		lpTBNotify->tbButton.idCommand);

	// do not allow hidden button to be deleted as they just do not go
	// to the Add listbox.
	if ((lpTBNotify->tbButton.idCommand) && bIsHidden)
		*pResult = FALSE;
	else					 
		*pResult = TRUE;  
}

//////////////////////////////////////////////////////////////////////
// This function is called when the user begins dragging a toolbar
// button or when the customization dialog is being populated with
// toolbar information.  Basically, *pResult should be populated with
// your answer to the question, "is the user allowed to insert a
// button to the left of this one?".
//////////////////////////////////////////////////////////////////////

void CXTToolBar::OnToolBarQueryInsert(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	*pResult = TRUE;
}

//////////////////////////////////////////////////////////////////////
// This function is called when the user clicks on the reset button on the
// toolbar customization dialog box.
//////////////////////////////////////////////////////////////////////

void CXTToolBar::OnToolBarReset(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPTBNOTIFY lpTBNotify = (LPTBNOTIFY)pNMHDR;
	ASSERT(lpTBNotify != NULL);

	*pResult = TRUE;

	ResetCustomizations();
}

//////////////////////////////////////////////////////////////////////
// This function is called whenever the user makes a change to the
// layout of the toolbar.  Calling the mainframe's RecalcLayout forces
// the toolbar to repaint itself.
//////////////////////////////////////////////////////////////////////

void CXTToolBar::OnToolBarChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	UNREFERENCED_PARAMETER(pNMHDR);
	UNREFERENCED_PARAMETER(pResult);
	
	// properly reposition toolbar buttons (new wrapping may be required)
	DWORD dwMode = 0;
	if ((m_dwStyle & CBRS_SIZE_DYNAMIC) && (m_dwStyle & CBRS_FLOATING))
		dwMode = LM_HORZ | LM_MRUWIDTH;
	else if (m_dwStyle & CBRS_ORIENT_HORZ)
		dwMode = LM_HORZ | LM_HORZDOCK;
	else
		dwMode =  LM_VERTDOCK;

	CalcDynamicLayout(-1, dwMode | LM_COMMIT);	

	PositionControls();

	OnIdleUpdateCmdUI(TRUE, 0L);
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////

void CXTToolBar::LoadButtonInfo()
{
	// Get the number of buttons in the toolbar.
	int nButtons = GetButtonCount();

	m_listTBButtons.RemoveAll();
	int iButton;
	for (iButton = 0; iButton < nButtons; ++iButton)
	{
		XT_TBBUTTON button;
		GetButton(iButton, &button);

		// is the button hidden?
		if (GetToolBarCtrl().IsButtonHidden(button.idCommand)) {
			button.bHidden = true;
		}

		m_listTBButtons.AddTail(button);
	}
}

//////////////////////////////////////////////////////////////////////
// This function saves the state (visible buttons, toolbar position, etc.)
// of the toolbar, using the registry key provided to the Create(...) function.
//////////////////////////////////////////////////////////////////////

void CXTToolBar::SaveState()
{
	// if there is an associated registry subkey
	if (m_strSubKey.GetLength())
	{
		// save the toolbar state to the registry
		SaveToolBarState(_T("Settings"), m_strValueName);
	}
}

//////////////////////////////////////////////////////////////////////
// This function restores the state (visible buttons, toolbar position, etc.)
// of the toolbar, using the registry key provided to the Create(...) function.
//////////////////////////////////////////////////////////////////////

void CXTToolBar::RestoreState()
{
	// parent must be a frame window.
	CFrameWnd* pFrame = (CFrameWnd*)GetParent();
	if (!pFrame->IsFrameWnd())
	{
		ModifyStyle(CCS_ADJUSTABLE | TBSTYLE_ALTDRAG, 0L);
		return;
	}

	// if there is an associated registry subkey
	if (m_strSubKey.GetLength())
	{
		LoadToolBarState(_T("Settings"), m_strValueName);
		DeferFreshenAfter();
	}

	// move the controls to thier correct locations.
	PositionControls();	
}

//////////////////////////////////////////////////////////////////////
// END TOOLBAR CUSTOMIZATION
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG

void CXTToolBar::AssertValid() const
{
	// Note: CXTControlBar::AssertValid is not called because it checks for
	//	m_nCount and m_pData to be in sync, which they are not in CXTToolBar.

	ASSERT(m_hbmImageWell == NULL ||
		( xtAfxData.bWin95 || ::GetObjectType(m_hbmImageWell) == OBJ_BITMAP));

	if (m_hInstImageWell != NULL && m_hbmImageWell != NULL)
		ASSERT(m_hRsrcImageWell != NULL);
}

void CXTToolBar::Dump(CDumpContext& dc) const
{
	CXTControlBar::Dump(dc);

	dc << _T("m_hbmImageWell = ") << (UINT)m_hbmImageWell;
	dc << _T("\nm_hInstImageWell = ") << (UINT)m_hInstImageWell;
	dc << _T("\nm_hRsrcImageWell = ") << (UINT)m_hRsrcImageWell;
	dc << _T("\nm_sizeButton = ") << m_sizeButton;
	dc << _T("\nm_sizeImage = ") << m_sizeImage;

	if (dc.GetDepth() > 0)
	{
		CXTToolBar* pBar = (CXTToolBar*)this;
		int nCount = pBar->DefWindowProc(TB_BUTTONCOUNT, 0, 0);
		int i;
		for (i = 0; i < nCount; i++)
		{
			TBBUTTON button;
			GetButton(i, &button);
			dc << _T("\ntoolbar button[" << i << "] = {");
			dc << _T("\n\tnID = ") << button.idCommand;
			dc << _T("\n\tnStyle = ") << MAKELONG(button.fsStyle, button.fsState);
			if (button.fsStyle & TBSTYLE_SEP)
				dc << _T("\n\tiImage (separator width) = ") << button.iBitmap;
			else
				dc <<_T("\n\tiImage (bitmap image index) = ") << button.iBitmap;
			dc << _T("\n}");
		}
	}

	dc << _T("\n");
}

#endif

BOOL CXTToolBar::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CXTControlBar::PreCreateWindow(cs))
		return FALSE;

	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return TRUE;
}

void CXTToolBar::OnDestroy()
{
	RegisterCustomizeAPI(false);
	
	// If the toolbar is customizable, save the current state.	
	if (IsCustomizable() == true)
	{
		SaveState();
		EnableCustomization( false );
	}	

	CXTControlBar::OnDestroy();
}

BOOL CXTToolBar::AutoSizeToolbar()
{
	// if the toolbar's style does not include TBSTYLE_LIST, the call the
	// default AutoSize(), and return success.
	if ((::GetWindowLong(m_hWnd, GWL_STYLE) & TBSTYLE_LIST) == 0)
	{
		AutoSize();
		return TRUE;
	}
	
	// Get the number of buttons in the toolbar.
	int nButtons = GetButtonCount();

	// loop thru the buttons.
	int nIndex;
	for (nIndex = 0; nIndex < nButtons; ++nIndex)
	{
/*		TBBUTTON button;
		GetButton(nIndex, &button);
		

		if (button.idCommand != 0)
		{
			bool bControl = false;
			
			POSITION pos;
			for (pos = m_listTBButtons.GetHeadPosition(); pos != NULL; m_listTBButtons.GetNext(pos))
			{
				XT_TBBUTTON& tbButton = m_listTBButtons.GetAt(pos);
				if (tbButton.idCommand == button.idCommand)
				{
					if (tbButton.pWndCtrl != NULL)
					{
						if (::IsWindow(tbButton.pWndCtrl->m_hWnd))
							PositionControl(tbButton.pWndCtrl);
						bControl = true;
						break;
					}
				}
			}

			if (!bControl)
			{
				button.fsStyle = TBSTYLE_AUTOSIZE;
				SetButton(nIndex, &button);
			}
		}
		*/

	
		
		TBBUTTON button;
		GetButton(nIndex, &button);

		if ( (button.idCommand != 0))
		{
			bool bIsControlButton = false;

			POSITION pos;
			for (pos = m_listTBButtons.GetHeadPosition(); pos != NULL; m_listTBButtons.GetNext(pos))
			{
				XT_TBBUTTON& tbButton = m_listTBButtons.GetAt(pos);
				if (tbButton.idCommand == button.idCommand)
				{
					if (tbButton.pWndCtrl != NULL)
					{
						bIsControlButton = true;
						break;
					}
				}
			}

			if (bIsControlButton)
			{
				CRect rect;
				GetItemRect(nIndex, rect);
				CWnd *pWndControl = GetDlgItem(button.idCommand);
				if (pWndControl)
				{
					CRect rectButton;
					pWndControl->GetWindowRect(&rectButton);
					ScreenToClient(rectButton);
					if (rectButton.left != rect.left || 
						rectButton.top != rect.top || 
						pWndControl->IsWindowVisible())
					{
						pWndControl->SetWindowPos(NULL, rect.left, rect.top, 0, 0,
									 SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
					}
				}
			}
			else
			{
				button.fsStyle |= TBSTYLE_AUTOSIZE;
				SetButton(nIndex, &button);
			}
		}
	}

	return TRUE;
}

void CXTToolBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// intercept attempt to initiate drag operation
	if (StartCustomDrag(point))
	{
		return;
	}
	
	CXTControlBar::OnLButtonDown(nFlags, point);
}

void CXTToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bInvalidateOnMouseUp)
	{
		ReleaseCapture();
		m_bInvalidateOnMouseUp = false;
		RedrawWindow(0,0,RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
	}
	CXTControlBar::OnLButtonUp(nFlags, point);
}


int CXTToolBar::CompactButtons(int dimensionCurrent, int dimensionMin, bool bHorz)
{
	if (IsAdjustingToolbar())
	{
		return dimensionCurrent;
	}

	// get snapshot of the button state
	bool isVisible = false;
	int nCount = SendMessage(TB_BUTTONCOUNT, 0, 0);
	TBBUTTON* buttons = (nCount) ? (TBBUTTON*)_alloca(nCount * sizeof(TBBUTTON))  : 0;
	int i;
	for (i = 0; i < nCount; i++)
	{
		GetButton(i, buttons + i);
		if (!(buttons[i].fsState & TBSTATE_HIDDEN))
		{
			isVisible = true;
		}
	}

	MoveExpButton(dimensionMin, bHorz);

	// if nothing is visible, then there is nothing to recalc
	if (!isVisible)
	{
		return dimensionCurrent;
	}

	// check to see if we need any compaction
	if (m_mapHiddenButtons.GetCount() == 0 && dimensionCurrent <= dimensionMin)
	{
		// nothing is hidden and toolbar fits allocated width
		return dimensionCurrent;
	}
	// deduct dimension of the expansion button out of alloted space
	dimensionMin -= EXPANSION_BUTTON_WIDTH + EXPANSION_BUTTON_INSET;
	
	// count all visible buttons first - we'll use that to leave out at least one
	// visible button
	int nVisibleCount = 0;
	for (i = 0;  i < nCount; i++)
	{
		if (!(buttons[i].fsState & TBSTATE_HIDDEN))
		{
			nVisibleCount++;
		}
	}

	// iterate buttons hide until we can fit into the maxed out dimension
	int dimension = dimensionCurrent;
	bool bRedraw = false;
	for (i = nCount - 1; i >= 0 && nVisibleCount > 1 ; i--)
	{
		if (!(buttons[i].fsState & TBSTATE_HIDDEN))
		{
			CRect rcItem;
			SendMessage(TB_GETITEMRECT, i, (LPARAM)&rcItem);

			buttons[i].fsState |= TBSTATE_HIDDEN;
			SetButton(i, buttons + i);
			m_bDelayedButtonLayout = TRUE;
			nVisibleCount--;

			XT_HIDDENBUTTON hidden;
			hidden.nWidth = rcItem.Width();
			m_mapHiddenButtons.SetAt(i, hidden);
			bRedraw = true;
	
			dimension = GetDimension(CalcFixedLayout(false, bHorz), bHorz);
			if (dimension <= dimensionMin )
			{
				// fitted to the min length
				break;
			}
		}
	}

	if (bRedraw)
	{
		DeferFreshenAfter();
	}

	// all buttons are hidden
	return dimensionCurrent;
}

void CXTToolBar::DeferFreshenAfter()
{
	PostMessage(XTWM_FRESHENAFTER, 0, ++m_dwSignatureFreshenAfter);
}

void CXTToolBar::ResetFreshenAfter()
{
	++m_dwSignatureFreshenAfter;
}

LRESULT CXTToolBar::OnFreshenAfter(WPARAM, LPARAM lp)
{
	if (DWORD(lp) == m_dwSignatureFreshenAfter)
	{
		if (m_bIdealSizeChanged)
		{
			m_bIdealSizeChanged = false;
			if (CWnd* pParent = GetParent())
			{
				// notify parent that ideal size of this control has changed
				// in the context of toolbar, ideal size is the extent of toolbar positioned horizontally
				UINT cxIdeal = CalcFixedLayout(false, true).cx; 
				pParent->PostMessage(XTWM_IDEALSIZECHANGED, (WPARAM)m_hWnd, (LPARAM)cxIdeal);
				// repost to complete after XTWM_IDEALSIZECHANGED message gets processed
				DeferFreshenAfter();
				return 0;
			}
		}

		if (m_bDelayedButtonLayout)
		{
			Layout();			
		}
		CFrameWnd* pFrame = GetParentFrame();
		if (pFrame && pFrame->IsFrameWnd())
		{
			pFrame = GetDockingFrame();
			pFrame->RecalcLayout();
		}
		if (GetParentFrame() != pFrame)
		{
			pFrame = GetParentFrame();
			if (pFrame)
			{
				pFrame->RecalcLayout();
			}
		}
		
		PositionControls();
		m_dwSignatureFreshenAfter++;  // invalidate pending requests to avoid recursion
		OnIdleUpdateCmdUI(TRUE, 0L);
		RedrawWindow(0,0,RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);		
	}
	return 0;
}

void CXTToolBar::MoveExpButton(int nDimension, bool bHorz)
{
	if (m_pExpButton)
	{
		// check to see if the button shall be hidden 
		bool bVisible = IsExpansionChevron() || IsExpansionCustomizes();
		bool bCurrentlyVisible = m_pExpButton ->IsWindowVisible() != 0;
		CPoint pos(0);

		CRect borders(0,0,0,0);
		CalcInsideRect(borders, bHorz);
		int border = -GetDimension(borders.Size(), bHorz);

		if (bHorz)
		{
			if (m_bInReBar)
			{
				pos.x = nDimension - EXPANSION_BUTTON_WIDTH;
			}
			else
			{
				pos.x = nDimension  -  EXPANSION_BUTTON_WIDTH - border;
			}
			pos.y = EXPANSION_BUTTON_INSET;
		}
		else
		{
			pos.x = EXPANSION_BUTTON_INSET;
			pos.y = nDimension - EXPANSION_BUTTON_WIDTH - border;
		}

		// offset the button if it would overlap 

		// Get the number of buttons in the toolbar.
		int nButtons = GetButtonCount();

		// loop thru the buttons. position shall not overlap with at least first
		// (second is enforced in expand/collapse buttons)
		int nIndex;
		for (nIndex = 0; nIndex < nButtons; ++nIndex)
		{
			TBBUTTON button;
			GetButton(nIndex, &button);
			if (!(button.fsState & TBSTATE_HIDDEN))
			{
				CRect rcItem;
				::SendMessage(m_hWnd, TB_GETITEMRECT, nIndex, (LPARAM)&rcItem);
				int nDimensionMin = GetDimension(rcItem.Size(), bHorz);
				if (nDimensionMin > GetDimension(pos, bHorz))
				{
					SetDimension(pos, bHorz, nDimensionMin);
				}
				break;
			}
		}

		CRect rectButton;
		m_pExpButton ->GetWindowRect(&rectButton);
		ScreenToClient(rectButton);
		if (rectButton.TopLeft() != pos || bVisible != bCurrentlyVisible)
		{
			DWORD flags = SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE;
			flags |= (bVisible) ? SWP_SHOWWINDOW : SWP_HIDEWINDOW;

			m_pExpButton ->SetWindowPos(NULL, pos.x, pos.y, 0, 0, flags);
			m_pExpButton ->RedrawWindow();
		}
	}
}

int CXTToolBar::ExpandButtons(int currentDimension, int dimensionMax, bool bHorz)
{
	// if nothing is hidden, then there is nothing to expand
	if (m_mapHiddenButtons.IsEmpty())
	{
		// move expansion button to the proper position
		MoveExpButton(currentDimension, bHorz);
		return currentDimension;
	}

	// there are hidden buttons, show
	MoveExpButton(dimensionMax, bHorz);

	// get snapshot of the button state
	int nCount = SendMessage(TB_BUTTONCOUNT, 0, 0);
	TBBUTTON* buttons = (nCount) ? (TBBUTTON*)_alloca(nCount * sizeof(TBBUTTON))  : 0;
	int i;
	for (i = 0; i < nCount; i++)
	{
		GetButton(i, buttons + i);
	}

	// calculate max dimension that is left out after displaying
	// expansion button (will not if there would be none)
	int extent = dimensionMax;

	// iterate buttons and recover their visible state until we can fit into the maxed out dimension
	int dimension = currentDimension;
	bool bChanged = false;
	for (i = 0; i < nCount; i++)
	{
		if (buttons[i].fsState & TBSTATE_HIDDEN)
		{
			XT_HIDDENBUTTON hidden;
			if (m_mapHiddenButtons.Lookup(i, hidden))
			{
				buttons[i].fsState &= ~TBSTATE_HIDDEN;
				SetButton(i, buttons + i);
				dimension = GetDimension(CalcFixedLayout(false, bHorz), bHorz);
				if (dimension > extent)
				{
					// cannot fit, break at this point
					buttons[i].fsState |= TBSTATE_HIDDEN;
					SetButton(i, buttons + i);
					break;
				}

				bChanged = true;				
				m_mapHiddenButtons.RemoveKey(i);
				m_bDelayedButtonLayout = TRUE;
			}
		}
	}

	if (IsExpansionChevron())
	{
		dimension = dimensionMax;
	}
	MoveExpButton(dimension, bHorz);

	if (bChanged)
	{
		// update UI state to properly display current state
		DeferFreshenAfter();
	}

	// calculated additional dimension, report it
	return dimension;
}

struct XT_BUTTONINFO
{
	CString text;	// button text
	CWnd* pControl;  // control
	XT_DROPDOWNBUTTON* pDrop; // dropdown descriptor
	int nNewIndex;	 // index in the new toolbar
};

// creates buttons in the other toolbar that are hidden in this one
void CXTToolBar::CopyHiddenButtons(CXTToolBar* other, int preferredWidth)
{
	int nCount = SendMessage(TB_BUTTONCOUNT, 0, 0);
	if (!nCount)
	{
		// nothing to do
		return;
	}

	CArray<TBBUTTON, TBBUTTON&> buttons;
	CArray<XT_BUTTONINFO, XT_BUTTONINFO&> infos;

	// adjust the preferred width to the max of the longest hidden button
	POSITION pos;
	for (pos = m_mapHiddenButtons.GetStartPosition( ); pos;)
	{
		int nIndex;
		XT_HIDDENBUTTON hiddenButton;
		m_mapHiddenButtons.GetNextAssoc(pos, nIndex, hiddenButton);
		if (hiddenButton.nWidth > preferredWidth)
		{
			preferredWidth = hiddenButton.nWidth;
		}
	}

	// fill in the order the buttons appear on the toolbar
	int length = 0; // length of the current row
	int nIndex;
	for (nIndex = 0; nIndex < nCount; nIndex++)
	{
		// skip buttons that were not dynamically hidden
		XT_HIDDENBUTTON hiddenButton;

		if (!m_mapHiddenButtons.Lookup(nIndex, hiddenButton))
		{
			continue;
		}

		TBBUTTON button;
		GetButton(nIndex, &button);

		// copy buttons and separators that have matching controls
		CWnd* pControl = 0;
		bool bCopy = !(button.fsStyle & TBSTYLE_SEP);
		if (!bCopy)
		{
			if (button.idCommand)
			{
				pControl = GetDlgItem(button.idCommand);
				if (pControl)
				{
					bCopy = true;
				}
			}
		}

		if (bCopy)
		{
			button.fsState &= ~TBSTATE_HIDDEN;

			// figure out if we have to start another row
			if (length + hiddenButton.nWidth > preferredWidth)
			{
				// must start a new row unless it has already been started
				if (length > 0)
				{
					// insert a separator
					TBBUTTON separator;
					memset(&separator, 0, sizeof(separator));
					separator.fsStyle = TBSTYLE_SEP;
					separator.fsState = TBSTATE_ENABLED | TBSTATE_WRAP;
					buttons.Add(separator);
					length = 0;
				}
			}
			length += hiddenButton.nWidth;

			XT_BUTTONINFO info;
			if (button.iString >= 0)
			{
				info.text = GetButtonText(nIndex);
				button.iString = -1;
			}

			info.pDrop = 0;
			if (!(button.fsStyle & TBSTYLE_SEP) && (button.idCommand > 0))
			{
				info.pDrop = FindDropDownButton(button.idCommand);
			}

			info.nNewIndex = buttons.Add(button);
			info.pControl = pControl;
			infos.Add(info);

		}
	}
	if (buttons.GetSize() > 0)
	{
		if (m_hInstImageWell != NULL && m_hRsrcImageWell != NULL)
		{
			// reload bitmap
			other->m_hInstImageWell = m_hInstImageWell;
			other->m_hRsrcImageWell = m_hRsrcImageWell;
			HBITMAP hbmNew;
			hbmNew = _xtAfxLoadSysColorBitmap(m_hInstImageWell, m_hRsrcImageWell);
			if (hbmNew != NULL)
			{
				other->AddReplaceBitmap(hbmNew);
			}
		}
		else
		{
			if (CImageList* pIL = GetToolBarCtrl().GetImageList())
			{
				other->GetToolBarCtrl().SetImageList(pIL);
			}
		}

		other->AddButtons(buttons.GetSize(), buttons.GetData());
		other->LoadButtonInfo();
		other->m_bOwnsDropDowns = false;
		other->ResolveCommandIcons();

		// share disabled image list if available
		if ( CImageList* pIL = GetToolBarCtrl( ).GetDisabledImageList( ) )
		{
			other->SetImageList( pIL->GetSafeHandle( ), FALSE );
		}
		
		bool bFirst = true;
		int i;
		for (i = 0; i < infos.GetSize(); i++)
		{
			XT_BUTTONINFO info = infos[i];
			// setup texts
			if (!info.text.IsEmpty())
			{
				// Kludge: toolbar does not seem to work correctly when only one string
				// is assigned in it. Assign something irrelevant to make at least
				// two strings
				if (bFirst)
				{
					bFirst = false;
					VERIFY(other->SetButtonText(info.nNewIndex, _T("---")));
				}
				VERIFY(other->SetButtonText(info.nNewIndex, info.text));
			}

			// copy the controls
			if (info.pControl)
			{
				info.pControl->SetParent(other);
				other->PositionControl(info.pControl);
			}

			// share dropdowns so that color selections would automatically
			// propagate back
			if (info.pDrop)
			{
				POSITION pos;
				for (pos = other->m_listTBButtons.GetHeadPosition(); pos != NULL; other->m_listTBButtons.GetNext(pos))
				{
					XT_TBBUTTON& tbButton = other->m_listTBButtons.GetAt(pos);
					if (tbButton.idCommand == (int)info.pDrop->idButton)
					{
						tbButton.pDDButton = info.pDrop;
						tbButton.bOwnsButton = false;
					}
				}
			}
		}
	}

	// copy text rendering flags
	if (m_dwDrawTextMask)
	{
		other->GetToolBarCtrl().SetDrawTextFlags(m_dwDrawTextMask, m_dwDrawTextFlags);
	}
}

CSize CXTToolBar::GetMinExtent()
{
	CSize extent(0);
	if (int nCount = SendMessage(TB_BUTTONCOUNT, 0, 0))
	{
		// find first visible
		int pos = -1;
		int i;
		for (i = 0; i < nCount && pos < 0; i++)
		{
			TBBUTTON button;
			GetButton(i, &button);
			if (!(button.fsState & TBSTATE_HIDDEN))
			{
				pos = i;
			}
		}
		if (pos >= 0)
		{
			BOOL bHorz = (m_dwStyle & CBRS_ORIENT_HORZ) != 0;
			
			CRect rcItem;
			SendMessage(TB_GETITEMRECT, pos, (LPARAM)&rcItem);
			extent.cx = rcItem.right;
			extent.cy = rcItem.bottom;

			CRect borders; 
			borders.SetRectEmpty();
			CalcInsideRect(borders, bHorz);
			extent.cy -= borders.Height();
			extent.cx -= borders.Width();

			if (bHorz)
			{
				extent.cx += EXPANSION_BUTTON_WIDTH + EXPANSION_BUTTON_INSET;
			}
			else
			{
				extent.cy += EXPANSION_BUTTON_WIDTH + EXPANSION_BUTTON_INSET;
			}
		}
	}
	return extent;
}

void CXTToolBar::EnableDocking(DWORD dwDockStyle)
{
	// Restore the previous toolbar state.
	if ( IsCustomizable( ) == true )
	{
		RestoreState();
	}

	CXTControlBar::EnableDocking(dwDockStyle);
}

void CXTToolBar::EnableDockingEx(DWORD dwDockStyle, DWORD dwFlatStyle)
{
	// Restore the previous toolbar state.
	if ( IsCustomizable( ) == true )
	{
		RestoreState( );
	}

	CXTControlBar::EnableDockingEx(dwDockStyle, dwFlatStyle);
}

bool CXTToolBar::OnAddedToRebar(REBARBANDINFO* pRBBI, bool bHorz)
{
    // if the toolbar is customizable, we need to restore the
    // previous toolbar state here before it is added to the rebar.
    // Restore the previous toolbar state.
    if (IsCustomizable())
    {
        RestoreState();
    }

	// Compute default sizing
	CXTControlBar::OnAddedToRebar(pRBBI, bHorz);
	if (IsExpansionButton())
	{
		// if this toolbar has expansion button enabled, then use the above default width (already computed)
		// as ideal width, min width would be the width of toolbar containing
		// the first visible button plus width of the expansion button
		ASSERT(pRBBI->fMask & RBBIM_CHILDSIZE);
		pRBBI->fMask |= RBBIM_IDEALSIZE | RBBIM_SIZE;
		pRBBI->cxIdeal = pRBBI->cxMinChild;
		pRBBI->cx = pRBBI->cxMinChild + 14; // gripper width
		pRBBI->cxMinChild = GetMinExtent().cx;
	}
	return true;
}

void CXTToolBar::OnRebarBandResized(XT_REBARSIZECHILDINFO* pInfo, bool bHorz)
{
	if (IsExpansionButton())
	{
		CSize sizeMax(pInfo->pBandInfo->cxIdeal, pInfo->pBandInfo->cyMinChild);				
		CSize sizeCurrent = CalcFixedLayout(FALSE, bHorz);

		CSize sizeVisible(pInfo->pSizeInfo->rcChild.right - pInfo->pSizeInfo->rcChild.left,
					      pInfo->pSizeInfo->rcChild.bottom - pInfo->pSizeInfo->rcChild.top);

		DWORD dwMode = LM_COMMIT;
		int nExtent;
		if (bHorz)
		{
			dwMode |= LM_HORZ;
			if (sizeVisible.cx < sizeCurrent.cx)
			{
				// compact toolbar horizontally
				nExtent = CompactButtons(sizeCurrent.cx, sizeVisible.cx, true);
			}
			else
			{
				// expand toolbar horizontally
				nExtent = ExpandButtons(sizeCurrent.cx, sizeVisible.cx, true);
			}
		}
		else
		{
			if (sizeVisible.cy < sizeCurrent.cy)
			{
				nExtent = CompactButtons(sizeCurrent.cy, sizeVisible.cy, false);
			}
			else
			{
				nExtent = ExpandButtons(sizeCurrent.cy, sizeVisible.cy, false);
			}
		}
		CalcLayout(dwMode, nExtent);
	}
}

bool CXTToolBar::IsFullWindowDrag()
{
	// tool bar uses full window when dragging
	return true;
}

COLORREF CXTToolBar::GetBackgroundColor() const
{
    if (InRebar())
    {
        // V.1.9. kludge: Let's mimic transparency when in rebar control by
        // drawing with non-XP background. This may be a problem with bitmapped 
        // rebar backgrounds. For v.1.9. the solution is actually
        // postponed until it becomes a problem
        return xtAfxData.clr3DFace;            
    }
	return xtAfxData.bXPMode? xtAfxData.clrXPBarFace : xtAfxData.clr3DFace;
}

HIMAGELIST CXTToolBar::SetImageList(HIMAGELIST hImgList, BOOL bEnabled) 
{
	XTImageManager()->LoadToolBarImageList(this, hImgList, bEnabled);

	if (!::IsWindow(m_hWnd))
		return 0;

	if (bEnabled)
	{
		return (HIMAGELIST) ::SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)hImgList);
	}
	else 
	{
		return (HIMAGELIST) ::SendMessage(m_hWnd, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)hImgList);
	}
}

CPoint CXTToolBar::ComputePopupTrackPoint(CRect rcExclude, CSize szPopup)
{
	CPoint pt;
	if (IsBottomDocked()) {
		pt = CPoint(rcExclude.left, rcExclude.top - szPopup.cy + (xtAfxData.bXPMode? 1: -1) );
	}
	
	else if (IsRightDocked()) {
		pt = CPoint(rcExclude.left - szPopup.cx + 1, rcExclude.top);
	}
	
	else if (IsLeftDocked()) {
		pt = CPoint(rcExclude.right + (xtAfxData.bXPMode? -1: 0), rcExclude.top);
	}
	
	else {
		pt = CPoint(rcExclude.left, rcExclude.bottom + (xtAfxData.bXPMode? -1: 0));
	}
	return pt;
}

// helper to compare two ints, bsearch/qsort compatible
static int  __cdecl CompareInts(const void *elem1, const void *elem2) 
{
	int int1 = *(const int*)elem1;
	int int2 = *(const int*)elem2;
	return int1 - int2;
}

bool CXTToolBar::IsDroppedItem(int nIndex) const
{
	return bsearch(&nIndex, m_arrPopupDropped.GetData(), 
		m_arrPopupDropped.GetSize(), sizeof(int), CompareInts) != 0;
		           
}

bool CXTToolBar::IsDroppedItem() const
{
	return m_arrPopupDropped.GetSize() > 0;
}

void CXTToolBar::SetDroppedItem(int iDroppedItem, bool bDropped) 
{
	// locate the item in the array
	int* pItem = (int*)bsearch(&iDroppedItem, 
							   m_arrPopupDropped.GetData(), 
							   m_arrPopupDropped.GetSize(), 
							   sizeof(int), 
							   CompareInts);

	if (bDropped)
	{
		// add item to the array if not already there
		if (pItem == 0)
		{
			// add this item, re-sort array
			m_arrPopupDropped.Add(iDroppedItem);
			qsort(m_arrPopupDropped.GetData(), 
				  m_arrPopupDropped.GetSize(), 
				  sizeof(int), 
				  CompareInts);
		}
	}
	else
	{
		// remove the item if present
		if (pItem != 0)
		{
			int pos = pItem - m_arrPopupDropped.GetData();
			m_arrPopupDropped.RemoveAt(pos);
		}
	}
}

bool CXTToolBar::HasCmdHandlers(CWnd* pWnd)
{
	CXTToolCmdUI state;
	state.m_pOther = this;
	
	state.m_nIndexMax = ( UINT )DefWindowProc( TB_BUTTONCOUNT, 0, 0 );
	for ( state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++ )
	{
		// get buttons state
		TBBUTTON button;
		GetButton( state.m_nIndex, &button );
		state.m_nID = button.idCommand;
		if (state.m_nID == 32816)
		{
			TRACE0("");
		}
		// ignore separators
		if ( !( button.fsStyle & TBSTYLE_SEP ) )
		{
			AFX_CMDHANDLERINFO info;
			info.pTarget = NULL;
			
			if ( pWnd->OnCmdMsg( state.m_nID, CN_COMMAND, &state, &info ) )
			{
				return true;
			}
		}
	}

	return false;
}
