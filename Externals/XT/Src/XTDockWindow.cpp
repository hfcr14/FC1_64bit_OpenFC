// XTDockWindow.cpp : implementation of the CXTDockWindow class.
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
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTDockWindow.h"
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTCallbacks.h"
#include "XTDockContext.h"
#include "XTButton.h"
#include "XTMemDC.h"
#include "XTRegistryManager.h"
#include "XTTabCtrlButtons.h"
#include "XTTabCtrl.h"
#include "XTTabCtrlBar.h"
#include "XTHelpers.h"
#include "XTPriv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IS_CONTROL_BAR(pWnd) \
	((pWnd)->GetRuntimeClass()->IsDerivedFrom(RUNTIME_CLASS(CControlBar)))

enum
{
	HIDE_CONTRLBAR = 0,
	MAXI_H_DISABLE,
	MAXI_H_ENABLED,
	MAXI_V_DISABLE,
	MAXI_V_ENABLED,
	NORM_H_DISABLE,
	NORM_H_ENABLED,
	NORM_V_DISABLE,
	NORM_V_ENABLED
};

const int FLOAT_CX = 300;
const int FLOAT_CY = 150;

/////////////////////////////////////////////////////////////////////////////
// CXTBarButton

class CXTBarButton : public CXTButton
{
	DECLARE_DYNAMIC(CXTBarButton)
public:
	CXTBarButton(CXTDockWindow *pWinDock) : m_pWinDock(pWinDock) { }

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS) 
	{
		SetColorFace(xtAfxData.bXPMode && m_pWinDock->m_bActive ? 
			xtAfxData.clrActiveCaption : xtAfxData.clr3DFace);

		CXTButton::DrawItem(lpDIS);
	}

	virtual void DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem)
	{
		UNREFERENCED_PARAMETER(rcItem);

		bool bDisabled = ((nState & ODS_DISABLED) == ODS_DISABLED);
		bool bSelected = ((nState & ODS_SELECTED) == ODS_SELECTED);

		// Get the window text, return if empty.
		CString strText = GetButtonText(TRUE);
		
		// get the handle to the icon (if any) associated
		// with this button.
		HICON hIcon = m_hIcon;
		if (hIcon)
		{
			CPoint point(1,1);
			
			// if the button is selected.
			if (bSelected)
			{
				if (m_hIconPushed != NULL)
					hIcon = m_hIconPushed;
				else
					point.Offset(1,1);
			}
			
			// Set the draw state flags.
			DWORD dwFlags = DST_ICON|DSS_NORMAL;
			
			if (xtAfxData.bXPMode && m_pWinDock->m_bActive && !bDisabled)
				pDC->DrawState(point, m_sizeImage, hIcon,
					dwFlags|DSS_MONO, (HBRUSH)CBrush(RGB(255,255,255)));
			else
				pDC->DrawState(point, m_sizeImage, hIcon,
					dwFlags, (HBRUSH)NULL);
		}
	}
	
	virtual void CleanUpGDI()
	{
		// since we are using toolkit resources for button icons, override
		// the CleanUpGDI method so we are not deleting these when the button
		// destructor is called.
		AfxDeleteObject((HGDIOBJ*)&m_hBitmap);
		AfxDeleteObject((HGDIOBJ*)&m_hBitmapMono);
	}
	
	CXTDockWindow *m_pWinDock;
};

IMPLEMENT_DYNAMIC(CXTBarButton, CXTButton);

/////////////////////////////////////////////////////////////////////////////
// CXTDockWindow

CXTDockWindow::CXTDockWindow()
{
	m_nBtnIndex        = HIDE_CONTRLBAR;
	m_pChildWnd        = NULL;
	m_pBtnClose        = NULL;
	m_pBtnMinMax       = NULL;
	m_bActive          = FALSE;
	m_pParentFrame     = NULL;
	m_dwInitSignature  = 1;

	// Load the image list used by control bar frame buttons.
	HIMAGELIST hImageList = ImageList_LoadBitmap( xtAfxData.hInstance, 
		MAKEINTRESOURCE( XT_IDB_BTN_BAR ), 10, 1, RGB( 0xFF,0x00,0xFF ) );

	if ( hImageList != NULL )
	{
		// initialize the icons used by control bars.
		m_hIconHide = ImageList_ExtractIcon(
			hInstance, hImageList, HIDE_CONTRLBAR);
		ASSERT(m_hIconHide != NULL);

		m_hIconMaxHDisable = ImageList_ExtractIcon(
			hInstance, hImageList, MAXI_H_DISABLE);
		ASSERT(m_hIconMaxHDisable != NULL);
		
		m_hIconMaxHEnable = ImageList_ExtractIcon(
			hInstance, hImageList, MAXI_H_ENABLED);
		ASSERT(m_hIconMaxHEnable != NULL);
		
		m_hIconMaxVDisable = ImageList_ExtractIcon(
			hInstance, hImageList, MAXI_V_DISABLE);
		ASSERT(m_hIconMaxVDisable != NULL);
		
		m_hIconMaxVEnable = ImageList_ExtractIcon(
			hInstance, hImageList, MAXI_V_ENABLED);
		ASSERT(m_hIconMaxVEnable != NULL);
		
		m_hIconHDisable = ImageList_ExtractIcon(
			hInstance, hImageList, NORM_H_DISABLE);
		ASSERT(m_hIconHDisable != NULL);
		
		m_hIconHEnabled = ImageList_ExtractIcon(
			hInstance, hImageList, NORM_H_ENABLED);
		ASSERT(m_hIconHEnabled != NULL);
		
		m_hIconVDisable = ImageList_ExtractIcon(
			hInstance, hImageList, NORM_V_DISABLE);
		ASSERT(m_hIconVDisable != NULL);
		
		m_hIconVEnabled = ImageList_ExtractIcon(
			hInstance, hImageList, NORM_V_ENABLED);
		ASSERT(m_hIconVEnabled != NULL);

		// Free image list.
		ImageList_Destroy(hImageList);
		hImageList = NULL;
	}
}

CXTDockWindow::~CXTDockWindow()
{
	SAFE_DELETE( m_pBtnClose );
	SAFE_DELETE( m_pBtnMinMax );
}

IMPLEMENT_DYNAMIC(CXTDockWindow, CXTControlBar)

BEGIN_MESSAGE_MAP(CXTDockWindow, CXTControlBar)
	//{{AFX_MSG_MAP(CXTDockWindow)
	ON_WM_NCHITTEST()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_COMMAND(XT_IDC_BTN_CLOSE, OnButtonClose)
	ON_UPDATE_COMMAND_UI(XT_IDC_BTN_CLOSE, OnUpdateButtonClose)
	ON_COMMAND(XT_IDC_BTN_EXPAND, OnButtonMinimize)
	ON_UPDATE_COMMAND_UI(XT_IDC_BTN_EXPAND, OnUpdateButtonMinimize)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_LBUTTONDOWN()
	ON_WM_INITMENUPOPUP()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_INITIALUPDATE, OnInitialUpdate)
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnXTInitialUpdate)
END_MESSAGE_MAP()

BOOL CXTDockWindow::ModifyXTBarStyle(DWORD dwRemove, DWORD dwAdd, BOOL bRedraw)
{
	if (!CXTControlBar::ModifyXTBarStyle(dwRemove, dwAdd, bRedraw))
		return FALSE;

	HideShowButtons( IsFloating( ) ? SW_HIDE : SW_SHOW );

	return TRUE;
}

void CXTDockWindow::SetDockWindowStyle(DWORD dwStyle)
{
	ASSERT((dwStyle & CBRS_ALL) == dwStyle);

	EnableToolTips(dwStyle & CBRS_TOOLTIPS);

	if (m_dwStyle != dwStyle)
	{
		DWORD dwOldStyle = m_dwStyle;
		m_dwStyle = dwStyle;
		OnBarStyleChange(dwOldStyle, dwStyle);
	}
}

BOOL CXTDockWindow::Create(CWnd *pParentWnd, UINT nID, LPCTSTR lpszWindowName/*=NULL*/,
	CSize sizeDefault/*=CSize(200,100)*/, DWORD dwStyle/*=CBRS_LEFT*/, DWORD dwBarStyle/*=CBRS_XT_DEFAULT*/)
{
	ASSERT_VALID(pParentWnd);   // must have a parent
	m_pParentFrame = (CFrameWnd*)pParentWnd;

	// Set initial control bar style.  We use SetDockWindowStyle instead of SetBarStyle for
	// VC7 compatibility.
	SetDockWindowStyle(dwStyle & CBRS_ALL|CBRS_HIDE_INPLACE|CBRS_SIZE_DYNAMIC|CBRS_FLOAT_MULTI);

	m_bp.sizeDefault = m_bp.sizeHorz = m_bp.sizeVert = sizeDefault;

	// the floating height and width should initially be
	// the same and set to the smaller of the two values.
	m_bp.sizeFloat.cx = FLOAT_CX;
	m_bp.sizeFloat.cy = FLOAT_CY;

	// if we are docked vertically, then the horizontal
	// sizes need to be reversed.
	if (dwStyle & (CBRS_LEFT|CBRS_RIGHT))
	{
		m_bp.sizeHorz.cx = sizeDefault.cy;
		m_bp.sizeHorz.cy = sizeDefault.cx;
	}

	// if we are docked horizontally, then the vertical
	// sizes need to be reversed.
	else
	{
		m_bp.sizeVert.cx = sizeDefault.cy;
		m_bp.sizeVert.cy = sizeDefault.cx;
	}

	// Initialize XTControl bar style.
	m_bp.dwStyle = dwBarStyle;

	// Register the window class.
	CString wndClass = ::AfxRegisterWndClass(CS_DBLCLKS|CS_CLASSDC,
		0, ::GetSysColorBrush(COLOR_BTNFACE));

	dwStyle &= ~CBRS_ALL;
	dwStyle |= WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	CRect rect( 0, 0, sizeDefault.cx, sizeDefault.cy );
	if (!CWnd::Create( wndClass, lpszWindowName, dwStyle, rect, pParentWnd, nID ))
		return FALSE;

	// Post XTWM_INITIAL_UPDATE message back to ourself so we can send a
	// WM_INITIALUPDATE message to children views when floating since
	// CMiniDockFrameWnd does not do this for us.
	PostMessage(XTWM_INITIAL_UPDATE, 0, ++m_dwInitSignature);

	return TRUE;
}

BOOL CXTDockWindow::PreTranslateMessage(MSG* pMsg) 
{
	if (HasButtons() && ::IsWindow(m_ToolTip.m_hWnd)) {
		m_ToolTip.RelayEvent(pMsg);
	}
	return CXTControlBar::PreTranslateMessage(pMsg);
}

void CXTDockWindow::SetActiveState(BOOL bActive)
{
	if (bActive != m_bActive)
	{
		// Our active state has changed.
		m_bActive = !m_bActive;

		// Update the caption area.
		if (HasCaption() && !IsFloating())
		{
			CClientDC dc(this);
			OnDrawGripper(&dc);
			
			if (m_pBtnClose && ::IsWindow(m_pBtnClose->m_hWnd)) {
				m_pBtnClose->Invalidate();
			}
			if (m_pBtnClose && ::IsWindow(m_pBtnClose->m_hWnd)) {
				m_pBtnMinMax->Invalidate();
			}
		}
	}
}

void CXTDockWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ASSERT_VALID(this);

	// This message is sent to the dock window when the user clicks on the
	// caption bar or on the border of the dock window.  The expected 
	// behavior in this case is to "activate" the view inside the dock bar.
	// However - there is no "real" way to determine what that view is.
	// For example, the dock bar could have a tab control embedded in it. 
	// The "real" view would then be a child of the tab control, and not the
	// tab control itself.  You can see this on DevStudio's own Workspace
	// view (see diagram below): When you click on the gripper, the tree 
	// view activates - not it's parent tab control.
	// 
	//                                 Dock Window
	//                       +---------------------------+
	//                       v                           v
	//                        +=========================+
	//                      + | Workspace 'Samples'    ---+
	//                     +  | [-] ## Button files     |  +
	//                     +  |     [+] Source Files    |  +
	//                     +  |     [+] Header Files    |  +
	//                     +  |     [-] Resource Files  |  +
	//                     +  |         +- Button.ico   |   +
	//                    +   |         +- Button.rc2   |    + Tree Control
	//      Tab Control  +    |         +- imagelist.bm |   +
	//                    +   | [-] ## ColorPicker file |  +
	//                     +  |     [+] Source Files    |  +
	//                     +  |     [+] Header Files    |  +
	//                     +  |     [+] Resource Files  |  +
	//                     +  |                        ---+
	//                     +  |+------+-------+--------+|
	//                      + ||Class.|Resou..|FileView||
	//                        +\------\-------\========/+
	// 
	// I have chosen to take the 80%/20% rule here and do some "cheating".
	// I decided to operate under the assumption that the "real" view is 
	// probably the one that is covering the most visible space.  In our 
	// example above, the tree control that displays DevStudio's FileView 
	// has more viewable space than its parent tab control.  This is not to
	// be confused with the actual rectangle the controls are encompassing.  
	// It is obvious that the tab control has a larger rectangle since it 
	// encompasses the tree control.  However, the tree control has a larger
	// visible area which masks parts of the tab control.
	// The code below searches for the view with the largest viewable area.
	// Again, I'm assuming that this will be the view that the user intends
	// to get activated.  This should suit in most cases, and should not be
	// a big problem in the "miss" cases.
	// 
	// - osolo

	CWnd *pChild = GetWindow(GW_CHILD);
	CWnd *pCandidate = NULL, *pWndToFocus = NULL;
	int nCandidateSize = 0, nWndToActivateSize = 0;
	do 
	{
		nCandidateSize = 0;
		pCandidate = NULL;
		while (pChild)
		{
			if (!IS_CONTROL_BAR(pChild)  &&  
				!pChild->IsKindOf(RUNTIME_CLASS(CXTBarButton))  &&
				pChild->IsWindowVisible()  && 
				pChild->IsWindowEnabled())
			{
				CRect r;
				pChild->GetClientRect(&r);
				const int nSize = r.Width() * r.Height();
				if (nSize > nCandidateSize)
				{
					nCandidateSize = nSize;
					pCandidate = pChild;
				}
			}
			pChild = pChild->GetWindow(GW_HWNDNEXT);
		}

		if (pCandidate)
		{
			pChild = pCandidate->GetWindow(GW_CHILD);
			if (!pWndToFocus  ||
				(pWndToFocus  &&  nWndToActivateSize < (2 * nCandidateSize)))
			{
				pWndToFocus = pCandidate;
				nCandidateSize = 0;
			}
		}
	} while (pChild);
	if (pWndToFocus)
	{
		pWndToFocus->SetFocus();
		SetActiveState(TRUE);
	}

	// Only allow WM_LBUTTONDOWN message to pass to the base 
	// class if CBRS_XT_NOFLOAT flag is not set.
	if ((GetXTBarStyle() & CBRS_XT_NOFLOAT) == 0)
	{
		CXTControlBar::OnLButtonDown(nFlags, point);
	}
	else
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
}

void CXTDockWindow::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// Only allow WM_LBUTTONDBLCLK message to pass to the base 
	// class if CBRS_XT_NOFLOAT flag is not set.
	if ((GetXTBarStyle() & CBRS_XT_NOFLOAT) == 0)
	{
		CXTControlBar::OnLButtonDblClk(nFlags, point);
	}
	else
	{
		CWnd::OnLButtonDblClk(nFlags, point);
	}
}

void CXTDockWindow::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	UNUSED_ALWAYS(pTarget);
	UNUSED_ALWAYS(bDisableIfNoHndler);

	// Get the window that has input focus.
	CWnd* pWndChild = CWnd::FromHandle(::GetFocus());
	
	// Indicates change in active state. Check if the focus window is 
	// either a child window or descendant window.
	if (m_bActive != (pWndChild && ::IsWindow(pWndChild->m_hWnd) && IsChild(pWndChild)))
	{
		SetActiveState(!m_bActive);
	}

	// update the min/max button's icon.
	if ( m_pBtnMinMax && ::IsWindow( m_pBtnMinMax->m_hWnd ) && !IsFloating( ) )
	{
		BOOL  bEnable = FALSE;
		UINT  uIDText = NULL;
		HICON hIcon   = NULL;
		int nBtnIndex = m_nBtnIndex;
		
		if (IsHorzDocked())
		{
			if (IsUnique())
			{
				uIDText   = XT_IDC_BTN_EXPAND;
				nBtnIndex = MAXI_H_DISABLE;
				bEnable   = FALSE;
				hIcon	  = m_hIconMaxHDisable;
			}
			else if (!IsMaximized())
			{
				uIDText   = XT_IDC_BTN_EXPAND;
				nBtnIndex = MAXI_H_ENABLED;
				bEnable   = TRUE;
				hIcon	  = m_hIconMaxHEnable;
			}
			else
			{
				uIDText   = XT_IDS_CONTRACT;
				nBtnIndex = NORM_H_ENABLED;
				bEnable   = TRUE;
				hIcon	  = m_hIconHEnabled;
			}
		}
		else
		{
			if (IsUnique())
			{
				uIDText   = XT_IDC_BTN_EXPAND;
				nBtnIndex = MAXI_V_DISABLE;
				bEnable   = FALSE;
				hIcon	  = m_hIconMaxVDisable;
			}
			else if (!IsMaximized())
			{
				uIDText   = XT_IDC_BTN_EXPAND;
				nBtnIndex = MAXI_V_ENABLED;
				bEnable   = TRUE;
				hIcon	  = m_hIconMaxVEnable;
			}
			else
			{
				uIDText   = XT_IDS_CONTRACT;
				nBtnIndex = NORM_V_ENABLED;
				bEnable   = TRUE;
				hIcon	  = m_hIconVEnabled;
			}
		}
		
		// we are only concerned if the image index has changed.
		if ( m_nBtnIndex != nBtnIndex )
		{
			m_nBtnIndex = nBtnIndex;
			
			// set the new icon.
			m_pBtnMinMax->SetIcon(CSize(10,10), hIcon);
			m_pBtnMinMax->EnableWindow(bEnable);
			
			// if the tooltip control is valid, update the tip text.
			if ( m_ToolTip.GetSafeHwnd( ) )
			{
				m_ToolTip.UpdateTipText(uIDText, m_pBtnMinMax);
			}
		}
	}
}

CSize CXTDockWindow::CalcDynamicLayout(int nLength, DWORD nMode)
{
	CSize sizeResult;

	if (nMode & LM_HORZDOCK)
	{
		ASSERT(nMode & LM_HORZ);
		ASSERT(!(nMode & LM_VERTDOCK));
		ASSERT(!(nMode & LM_MRUWIDTH));
		sizeResult = m_bp.sizeHorz;
	}
	else if (nMode & LM_VERTDOCK)
	{
		ASSERT(!(nMode & LM_MRUWIDTH));
		sizeResult = m_bp.sizeVert;
	}
	else if (nMode & LM_MRUWIDTH)
	{
		sizeResult = m_bp.sizeFloat;
	}
	// check LM_STRECH  and LM_HORZ
	else
	{
		if (nMode == LM_HORZ) {
			if (nLength < 50) {
				nLength = 50;
			}
			m_bp.sizeDefault.cx = nLength;
		}
		
		if (nMode == (LM_LENGTHY | LM_HORZ)) {
			if (nLength < 50) {
				nLength = 50;
			}
			m_bp.sizeDefault.cy = nLength;
		}

		sizeResult = m_bp.sizeDefault;
	}

	if (nMode & LM_COMMIT) {
		m_bp.sizeFloat = m_bp.sizeDefault;
	}

	return sizeResult;
}

void CXTDockWindow::DoPaint(CDC* pDC)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	// paint inside client area
	CRect rect;
	GetClientRect(rect);
	DrawBorders(pDC, rect);

	// draw a border around the client area.
	GetInsideRect(rect);

	// Get the child border style (if any) and draw the border.
	if (HasClientBorderOutline()) {
		pDC->DrawEdge(rect, EDGE_ETCHED, BF_RECT);
	}
	else if (HasClientBorderStatic()) {
		pDC->DrawEdge(rect, BDR_SUNKENOUTER, BF_RECT);
	}
	else if (HasClientBorderModal()) {
		pDC->DrawEdge(rect, EDGE_RAISED, BF_RECT);
	}
}

LRESULT CXTDockWindow::OnNcHitTest(CPoint point)
{
	UNUSED_ALWAYS(point);
	if (afxData.hcurArrow != ::GetCursor())  {
		::SetCursor(afxData.hcurArrow);
	}

	return HTCLIENT;
}

int CXTDockWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ( !m_pBtnClose )
	{
		m_pBtnClose = new CXTBarButton(this);
		ASSERT(m_pBtnClose != NULL);
	}

	if (!::IsWindow( m_pBtnClose->m_hWnd ))
	{
		if(!m_pBtnClose->Create(NULL, WS_CHILD | WS_TABSTOP |
			BS_NOTIFY | BS_OWNERDRAW | BS_ICON | BS_CENTER,
			CRect(0,0,0,0), this, XT_IDC_BTN_CLOSE ))
		{
			TRACE0( "Unable to create close button\n" );
			return false;
		}
		
		m_pBtnClose->SetIcon(CSize(10,10), m_hIconHide);
	}

	if ( !m_pBtnMinMax )
	{
		m_pBtnMinMax = new CXTBarButton(this);
		ASSERT(m_pBtnMinMax != NULL);
	}

	if (!::IsWindow( m_pBtnMinMax->m_hWnd ))
	{
		if(!m_pBtnMinMax->Create(NULL, WS_CHILD | WS_TABSTOP |
			BS_NOTIFY | BS_OWNERDRAW | BS_ICON | BS_CENTER,
			CRect(0,0,0,0), this, XT_IDC_BTN_EXPAND ))
		{
			TRACE0( "Unable to create minimize button\n" );
			return false;
		}
	}

	HideShowButtons(SW_SHOW);

	// Create the ToolTip control.
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool(m_pBtnClose, XT_IDC_BTN_CLOSE);
	m_ToolTip.AddTool(m_pBtnMinMax, XT_IDC_BTN_EXPAND);
	
	return 0;
}

void CXTDockWindow::UpdateButtonXStyles()
{
	DWORD dwxStyle = 0L;
	
	if ( HasSemiFlatBorder( ) ) {
		dwxStyle |= BS_XT_SEMIFLAT;
	}
	if ( HasFlatButtons( ) ) {
		dwxStyle |= BS_XT_FLAT;
	}
	if ( m_pBtnClose && ::IsWindow( m_pBtnClose->m_hWnd ) ) {
		m_pBtnClose->SetXButtonStyle(dwxStyle);
	}
	if ( m_pBtnClose && ::IsWindow( m_pBtnClose->m_hWnd ) ) {
		m_pBtnMinMax->SetXButtonStyle(dwxStyle);
	}
}

bool CXTDockWindow::HideShowButtons(int nCmdShow)
{
	if( m_pBtnClose && ::IsWindow( m_pBtnClose->m_hWnd ) ) {
		m_pBtnClose->ShowWindow( HasCloseButton( ) ? nCmdShow : SW_HIDE );
	}
	if( m_pBtnMinMax && ::IsWindow( m_pBtnMinMax->m_hWnd ) ) {
		m_pBtnMinMax->ShowWindow( HasMinMaxButton( ) ? nCmdShow : SW_HIDE );
	}

	UpdateButtonXStyles();

	return true;
}

void CXTDockWindow::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	CXTControlBar::OnWindowPosChanged(lpwndpos);

    // prepare for deferred (when this function returns) window repositioning
    CXTDeferWindowPosHandler winMover(8);
    
	// Hide/show caption buttons.
	HideShowButtons( IsFloating() ? SW_HIDE : SW_SHOW );
	
	if ( !IsFloating( ) )
	{
		if ( HasCloseButton( ) )
		{
			CRect rcClose(GetButtonRect());

			if ( IsVertDocked( ) ) {
				rcClose.OffsetRect( 14,0 );
			}
			if ( m_pBtnClose && ::IsWindow( m_pBtnClose->m_hWnd ) ) {
				winMover.MoveWindow( m_pBtnClose, rcClose );
			}
		}
		if ( HasMinMaxButton( ) )
		{
			CRect rcMinim( GetButtonRect( ) );

			if ( IsHorzDocked( ) )
			{
				rcMinim.OffsetRect( 0,14 );

				if ( !HasCloseButton( ) ) {
					rcMinim.OffsetRect( 0,-14 );
				}
			}
			else
			{
				if ( !HasCloseButton( ) ) {
					rcMinim.OffsetRect( 14,0 );
				}
			}

			if ( m_pBtnMinMax && ::IsWindow( m_pBtnMinMax->m_hWnd ) ) {
				winMover.MoveWindow( m_pBtnMinMax, rcMinim );
			}
		}
	}
	else
	{
		if ( m_pDockBar && !HasCloseButton( ) )
		{
			CWnd* pParent = m_pDockBar->GetParent();
			if( pParent->IsKindOf(RUNTIME_CLASS(CMiniFrameWnd)))
			{
				pParent->ModifyStyle( WS_SYSMENU, 0, 0 );
			}
		}
	}

	if( m_pChildWnd->GetSafeHwnd( ))
	{
		CRect rect;
		GetInsideRect(rect);
		rect.DeflateRect(1,1);
		winMover.MoveWindow(m_pChildWnd, rect);
	}
}

void CXTDockWindow::OnButtonClose()
{
	GetDockingFrame()->ShowControlBar(this,FALSE,FALSE);
}

void CXTDockWindow::OnUpdateButtonClose(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CXTDockWindow::OnButtonMinimize()
{
	if (!IsMaximized())
	{
		if (m_pDockBar != NULL) {
			((CXTDockBar*)m_pDockBar)->Maximize(this);
		}
		m_bp.bMaximized = TRUE;
	}
	else
	{
		if (m_pDockBar != NULL) {
			((CXTDockBar*)m_pDockBar)->Normalize(this);
		}
		m_bp.bMaximized = FALSE;
	}
}

void CXTDockWindow::OnUpdateButtonMinimize(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();
}

void CXTDockWindow::EnableDocking(DWORD dwDockStyle)
{
	ASSERT(m_pDockContext == NULL);

	m_dwDockStyle = dwDockStyle;
	if (m_pDockContext == NULL) {
		m_pDockContext = new CXTDockContext(this);
	}
	if (m_hWndOwner == NULL) {
		m_hWndOwner = ::GetParent(m_hWnd);
	}
}

void CXTDockWindow::Maximize(int size)
{
	if (IsHorzDocked()) {
		m_bp.sizeDefault.cx = size;
	}
	else if (IsVertDocked()) {
		m_bp.sizeDefault.cy = size;
	}
	
	m_bp.bMaximized = TRUE;
}

int CXTDockWindow::Minimize()
{
	m_bp.bMaximized = FALSE;

	if (IsHorzDocked()) {
		m_bp.sizeDefault.cx = GetMinExt();
		return m_bp.sizeHorz.cx - m_bp.sizeDefault.cx;
	}
	else if (IsVertDocked()) {
		m_bp.sizeDefault.cy = GetMinExt();
		return m_bp.sizeVert.cy - m_bp.sizeDefault.cy;
	}
	
	return 0;
}

void CXTDockWindow::Normalize()
{
	if (IsHorzDocked()) {
		m_bp.sizeDefault.cx = m_bp.sizeHorz.cx;
	}
	else if (IsVertDocked()) {
		m_bp.sizeDefault.cy = m_bp.sizeVert.cy;
	}
	
	m_bp.bMaximized = FALSE;
}

int CXTDockWindow::GetMinExt()
{
	if (HasGripper())
    {
        return IsHorzDocked() ? 18 : 24;
	}
    
	return 5;
}

void CXTDockWindow::GetInsideRect(CRect& rect)
{
	// Get the client area of the control bar, and
	// deflate the rect by 3 pixels.
	GetClientRect(&rect);

	// Define offset vars for horz and vert docked.
	int x  = 3;
	int y  = 3;
	int cx = 0;
	int cy = 0;

	// If we are floating just return.
	if (IsFloating()) {
		rect.DeflateRect(x,y);
		return;
	}
	else {
		rect.DeflateRect(x,y*2);
	}
	
	if (IsVertDocked()) {
		cy = (GetMinExt()-y);
	}
	else {
		cx = (GetMinExt()-x);
	}

	// Set the rect size.
	rect.DeflateRect(cx, cy, 0, 0);

	if( rect.left > rect.right || rect.top > rect.bottom ) {
		rect = CRect(0,0,0,0);
	}

	if (rect.IsRectEmpty())
	{
		if (IsVertDocked()) {
			rect.OffsetRect(0, GetMinExt()+2);
		}
		else if (IsHorzDocked()) {
			rect.OffsetRect(GetMinExt()+2, 0);
		}
	}
	else if (IsVertDocked()) {
		rect.top -= 5;
	}

	if (xtAfxData.bXPMode)
	{
		if (IsVertDocked())
		{
			rect.InflateRect(4,0);
		}
		
        else if (IsHorzDocked())
		{
			rect.InflateRect(0,3);
		}

        if (IsKindOf(RUNTIME_CLASS(CXTTabCtrlBar)))
        {
            rect.bottom += 5;
        }
	}
}

void CXTDockWindow::EraseNonClient()
{
	CWindowDC dc(this);

	CRect rectClient;
	GetClientRect(rectClient);
	CRect rectWindow;
	GetWindowRect(rectWindow);
	ScreenToClient(rectWindow);
	rectClient.OffsetRect(-rectWindow.left, -rectWindow.top);
	
	dc.ExcludeClipRect(rectClient);
	rectWindow.OffsetRect(-rectWindow.left, -rectWindow.top);
}

void CXTDockWindow::DrawBorders(CDC *pDC, CRect &rect)
{
	ASSERT_VALID(this);
	ASSERT_VALID(pDC);

	ASSERT(rect.top == 0 && rect.left == 0);
	if(m_dwStyle & CBRS_BORDER_RIGHT) {
		pDC->FillSolidRect(rect.right-1, 0, rect.right, rect.bottom, xtAfxData.clr3DShadow);
	}
	if(m_dwStyle & CBRS_BORDER_BOTTOM) {
		pDC->FillSolidRect(0, rect.bottom-1, rect.right, rect.bottom, xtAfxData.clr3DShadow);
	}
	if(m_dwStyle & CBRS_BORDER_TOP) {
		pDC->FillSolidRect(0, 0, rect.right, 1, xtAfxData.clr3DHilight);
	}
	if(m_dwStyle & CBRS_BORDER_LEFT) {
		pDC->FillSolidRect(0, 0, 1, rect.bottom, xtAfxData.clr3DHilight);
	}

	OnDrawGripper(pDC);
}

void CXTDockWindow::DrawCaption(CDC* pDC, const CRect& rect)
{
	ASSERT( pDC );
	ASSERT( !rect.IsRectNull() );

	if (xtAfxData.bXPMode)
	{
		CRect rc = rect;

		int iButtonSize = 1;

		if ( HasCloseButton( ) ) {
			iButtonSize += 15;
		}
		if ( HasMinMaxButton( ) ) {
			iButtonSize += 15;
		}

		if (IsVertDocked()) {
			rc.InflateRect(3,0);
		}

		if (IsVertDocked())
		{
			rc.top -= 2;
			rc.bottom += 2;
			rc.right += iButtonSize-5;
		}
		else
		{
			rc.left -= 2;
			rc.right += 2;
			rc.top -= iButtonSize-3;
		}

		pDC->FillSolidRect(&rc, xtAfxData.clr3DFace);

		if (m_bActive)
		{
			pDC->SetTextColor(xtAfxData.clrActiveCaptText);
			pDC->FillSolidRect(&rc, xtAfxData.clrActiveCaption);
		}
		else
		{
			pDC->SetTextColor(xtAfxData.clrMenuText);
			pDC->Draw3dRect(&rc, xtAfxData.clr3DShadow,
			    xtAfxData.clr3DShadow);

			pDC->SetPixel(rc.left,    rc.top,      xtAfxData.clr3DFace);
			pDC->SetPixel(rc.left,    rc.bottom-1, xtAfxData.clr3DFace);
			pDC->SetPixel(rc.right-1, rc.top,      xtAfxData.clr3DFace);
			pDC->SetPixel(rc.right-1, rc.bottom-1, xtAfxData.clr3DFace);
		}
	}

	else if (!HasCaption())
	{
		pDC->SetTextColor(xtAfxData.clr3DShadow);
	}

	else if (HasGradientCaption())
	{
		// draw the caption background.
		COLORREF clrLeftCapt = m_bActive ? xtAfxData.clrActiveCaption :
			xtAfxData.clrInActiveCaption;
		
		// query gradient info (usually TRUE for Win98/Win2k)
		BOOL bGradient = FALSE;
		::SystemParametersInfo(SPI_GETGRADIENTCAPTIONS,
			0, &bGradient, 0);
		
		// solid color
		if (bGradient == FALSE) {
			pDC->FillSolidRect(&rect, clrLeftCapt);
		}
		else
		{
			// gradient from left to right or from bottom to top
			// get second gradient color (the right end)
			COLORREF clrRightCapt = m_bActive ?
				xtAfxData.clrGradActiveCapt : xtAfxData.clrGradInActiveCapt;
			
			// this will make 2^6 = 64 fountain steps
			int nShift = 6;
			int nSteps = 1 << nShift;
			
			int i;
			for (i = 0; i < nSteps; i++)
			{
				// do a little alpha blending
				int nRed   = (GetRValue(clrLeftCapt) * (nSteps - i) + GetRValue(clrRightCapt) * i) >> nShift;
				int nGreen = (GetGValue(clrLeftCapt) * (nSteps - i) + GetGValue(clrRightCapt) * i) >> nShift;
				int nBlue  = (GetBValue(clrLeftCapt) * (nSteps - i) + GetBValue(clrRightCapt) * i) >> nShift;
				
				// then paint with the resulting color
				CRect rcCapt = rect;
				if (IsHorzDocked())
				{
					rcCapt.bottom = rect.bottom - ((i       * rect.Height()) >> nShift);
					rcCapt.top    = rect.bottom - (((i + 1) * rect.Height()) >> nShift);
					if (rcCapt.Height() > 0) {
						pDC->FillSolidRect(rcCapt, RGB(nRed, nGreen, nBlue));
					}
				}
				else
				{
					rcCapt.left  = rect.left + ((i       * rect.Width()) >> nShift);
					rcCapt.right = rect.left + (((i + 1) * rect.Width()) >> nShift);
					if (rcCapt.Width() > 0) {
						pDC->FillSolidRect(rcCapt, RGB(nRed, nGreen, nBlue));
					}
				}
			}
		}

		pDC->SetTextColor(m_bActive ? xtAfxData.clrActiveCaptText : 
			xtAfxData.clrInactiveCaptText);
	}
	
	else
	{
		pDC->FillSolidRect(rect, m_bActive ? xtAfxData.clrActiveCaption :
			xtAfxData.clrInActiveCaption);

		pDC->SetTextColor(m_bActive ? xtAfxData.clrActiveCaptText : 
			xtAfxData.clrInactiveCaptText);
	}
	
	if ( HasText( ) )
	{
		// Get the window text to be used with docked 
		// caption display...
		CString strCaption = _T( "" );
		GetWindowText( strCaption );
	
		// Draw the caption text.
		if ( !strCaption.IsEmpty( ) )
		{
			pDC->SetBkMode( TRANSPARENT );

			CRect rcTitle = rect;
			if ( IsVertDocked( ) )
			{
				CFont* pOldFont = pDC->SelectObject( &m_font );
				CSize size = pDC->GetTextExtent( strCaption );

				CRect rcGrip(GetGripperRect());
				int i = (size.cy - rcGrip.Height())/2;

				if (i > 0)
				{
					rcTitle.top = i;
					rcTitle.bottom = rcTitle.top + rcTitle.Height();
				}

				rcTitle.OffsetRect(2,0);
				rcTitle.right  = rcTitle.left + size.cx;
				rcTitle.bottom = rcTitle.top  + size.cy;

				if ( rect.right < rcTitle.right )
				{
					rcTitle.right = rect.right;
				}

				pDC->DrawText( strCaption, &rcTitle, DT_SINGLELINE | DT_END_ELLIPSIS | DT_VCENTER );
				pDC->SelectObject( pOldFont );
			}
			else
			{
				CFont* pOldFont = pDC->SelectObject( &m_fontVert );
				CSize size = pDC->GetTextExtent( strCaption );

				// invert
				size = CSize( size.cy, size.cx );

				rcTitle.OffsetRect(0,-4);
				rcTitle.top = rcTitle.bottom - size.cy;

				if ( rect.top > rcTitle.top )
				{
					rcTitle.top = rect.top;
				}

				int x = rcTitle.left + ( ( rcTitle.Width( )-size.cx )/2 );
				int y = rcTitle.bottom;

				CRect rc = rect;
				rc.right = rect.left + size.cx;

				pDC->ExtTextOut(x, y, ETO_CLIPPED, rc, strCaption, NULL);
				pDC->SelectObject( pOldFont );
			}
		}
	}
}

void CXTDockWindow::OnDrawGripper(CDC *pDC)
{
	if (!IsFloating() && HasGripper())
	{
		// Set the initial gripper sizes.
		CRect rcGrip1(GetGripperRect());
		CRect rcGrip2(rcGrip1);

		if (IsHorzDocked()) {
			rcGrip2.OffsetRect(4,0);
		} else {
			rcGrip2.OffsetRect(0,4);
		}

		CRect rcGrip( rcGrip1.left, rcGrip1.top, rcGrip2.right, rcGrip2.bottom);            
		if (IsHorzDocked()) {
			rcGrip.DeflateRect(-2,0);
		}
		else {
			rcGrip.DeflateRect(0,-2);
		}

		// Draw the caption area...
		DrawCaption(pDC, rcGrip);

		// or draw the default gripper.
		if (!HasCaption())
		{
			if (HasText())
			{
				CString strWindText;
				GetWindowText(strWindText);
                
                int iLength = strWindText.GetLength();
                if (iLength > 0)
                {
			        CFont* pOldFont = pDC->SelectObject(
                        IsVertDocked() ? &m_font : &m_fontVert);
                    CSize size = pDC->GetTextExtent(strWindText, iLength);
                    pDC->SelectObject(pOldFont);
                    
                    if( IsHorzDocked() )
                    {
                        rcGrip1.bottom -= size.cx+5;
                        rcGrip2.bottom -= size.cx+5;
                    }
                    else
                    {
                        rcGrip1.left += size.cx+5;
                        rcGrip2.left += size.cx+5;
                    }
                }
			}
			
			if( IsHorzDocked() ) {
				if (rcGrip1.top >= rcGrip1.bottom) {
					return;
				}
			} else {
				if (rcGrip1.right <= rcGrip1.left) {
					return;
				}
			}
			
			// Draw the gripper.
			if (HasTwoGrips())
			{
				pDC->Draw3dRect( rcGrip1, xtAfxData.clr3DHilight,
					xtAfxData.clr3DShadow );
				pDC->Draw3dRect( rcGrip2, xtAfxData.clr3DHilight,
					xtAfxData.clr3DShadow );
			}
			else
			{
				if( IsHorzDocked() ) {
					rcGrip1.OffsetRect(2,0);
				}
				else {
					rcGrip1.OffsetRect(0,2);
				}
				pDC->Draw3dRect( rcGrip1, xtAfxData.clr3DHilight,
					xtAfxData.clr3DShadow );
			}
		}
	}
}

CRect CXTDockWindow::GetButtonRect()
{
	CRect rect;
	GetClientRect(&rect);
	rect.OffsetRect(-rect.left,-rect.top);

	if (IsHorzDocked())
	{
		rect.top	 += 3;
		rect.bottom = rect.top+12;
		rect.left  += 3;
		rect.right  = rect.left+12;
	
		if (xtAfxData.bXPMode) {
			rect.OffsetRect(0,2);
		}
	}
	else
	{
		rect.right -= 17;
		rect.left   = rect.right-12;
		rect.top   += 6;
		rect.bottom = rect.top+12;

		if (xtAfxData.bXPMode) {
			rect.OffsetRect(-2,0);
		}
	}
	
	return rect;
}

CRect CXTDockWindow::GetGripperRect()
{
	CRect rect;
	GetClientRect(&rect);
	rect.OffsetRect(-rect.left,-rect.top);

	if (IsHorzDocked())
	{
		rect.DeflateRect(3,3);
		rect.left	+= 2;
		rect.right	= rect.left+3;
		rect.bottom	-= 1;
		rect.top	 = 1;

		if ( HasButtons( ) ) {
			rect.top += 16;
		}
		if ( HasCloseButton( ) && HasMinMaxButton( ) ) {
			rect.top += 14;
		}
	}
	else
	{
		rect.DeflateRect(4,4);
		rect.top	 += 4;
		rect.bottom	 = rect.top+3;
		rect.right	 -= 2;

		if ( HasButtons( ) ) {
			rect.right -= 12;
		}
		if ( HasCloseButton( ) && HasMinMaxButton( ) ) {
			rect.right -= 14;
		}
	}

	return rect;
}

void CXTDockWindow::DelayShow(BOOL bShow)
{
	if (bShow) {
		ModifyXTBarStyle(0, CBRS_XT_NEWDOCKED, FALSE);
	}

	if (!bShow && IsMaximized())
	{
		if (m_pDockBar != NULL) {
			((CXTDockBar*)m_pDockBar)->Normalize(this);
		}
		Normalize();
	}

	CXTControlBar::DelayShow(bShow);
}

void CXTDockWindow::SaveBarSize(CString strSection)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	if(::IsWindow(m_hWnd))
	{
		CString string;
		string.LoadString(XT_IDS_REG_BARPOS);

		CXTRegistryManager regManager;
		regManager.WriteProfileBinary(strSection,
			string, reinterpret_cast<LPBYTE>(&m_bp),
			sizeof(XT_BARPLACEMENT));
	}
}

void CXTDockWindow::LoadBarSize(CString strSection)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state

	// restore the previous bar style.
	if(::IsWindow(m_hWnd))
	{
		UINT nSize = 0;
		LPBYTE pbtData = 0;
		CString string;
		string.LoadString(XT_IDS_REG_BARPOS);

		CXTRegistryManager regManager;
		if (regManager.GetProfileBinary( strSection,
			string, &pbtData, &nSize))
		{
			XT_BARPLACEMENT* pBP = 
				reinterpret_cast<XT_BARPLACEMENT*>(pbtData);

			SetBarPlacement(pBP);
			delete [] pbtData;
		}
	}
}

void CXTDockWindow::SetBarPlacement(XT_BARPLACEMENT* pBP)
{
	ASSERT(pBP);

	m_bp.sizeVert	 = pBP->sizeVert;
	m_bp.sizeHorz	 = pBP->sizeHorz;
	m_bp.sizeFloat	 = pBP->sizeFloat;
	m_bp.sizeDefault = pBP->sizeDefault;
	m_bp.bUnique	 = pBP->bUnique;
	m_bp.bMaximized	 = pBP->bMaximized;

	UpdateButtonXStyles();
}

void CXTDockWindow::SetNormalSize(const CSize &size)
{
	ASSERT(::IsWindow(m_hWnd)); 

	if (IsHorzDocked()) {
		m_bp.sizeHorz = m_bp.sizeDefault = size;
	}
	else if (IsVertDocked()) {
		m_bp.sizeVert = m_bp.sizeDefault = size; 
	}
	else {
		m_bp.sizeFloat = m_bp.sizeDefault = size; 
	}
}

void CXTDockWindow::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	// not necessary to invalidate the borders
	DWORD dwStyle = m_dwStyle;
	m_dwStyle &= ~(CBRS_BORDER_ANY);
	CXTControlBar::OnWindowPosChanging(lpwndpos);
	m_dwStyle = dwStyle;

	// If we can resize while floating
	if (dwStyle & CBRS_SIZE_DYNAMIC)
	{
		// And we are resizing
		if (lpwndpos->flags & SWP_NOSIZE)
			return;

		// Then redraw the buttons
		Invalidate();
	}
}

LRESULT CXTDockWindow::OnXTInitialUpdate(WPARAM, LPARAM lParam)
{
	if ((DWORD)lParam == m_dwInitSignature)
	{
		if (IsFloating())   // send initial update
			SendMessageToDescendants(WM_INITIALUPDATE, 0, 0, TRUE, TRUE);
	}
	return 0;
}

void CXTDockWindow::OnInitialUpdate()
{
	CXTControlBar::OnInitialUpdate();

	// invalidate pending requests to send initial update
	m_dwInitSignature++;
}

CWnd* CXTDockWindow::CreateView(CRuntimeClass *pViewClass, CDocument *pDocument, CCreateContext *pContext)
{
#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	CFrameWnd* pParentFrame = GetParentFrame();
	ASSERT_VALID(pParentFrame);

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		// client if possible.
		contextT.m_pLastView	   = pParentFrame->GetActiveView();
		contextT.m_pCurrentDoc	   = pDocument;
		contextT.m_pNewViewClass   = pViewClass;
		contextT.m_pNewDocTemplate = pDocument ? pDocument->GetDocTemplate() : NULL;
		contextT.m_pCurrentFrame   = NULL;

		pContext = &contextT;
	}

	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pContext->m_pNewViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" );
		// Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL
		
	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL); // not yet created.

	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;

	// Create with the right size (wrong position)
	CRect rect(0,0,0,0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, this, AFX_IDW_PANE_FIRST, pContext))
	{
		TRACE0( "Failed to create docking window view.\n" );
		return NULL;
	}

	pWnd->SetOwner(pParentFrame);
	return pWnd;
}

void CXTDockWindow::SetCaption(LPCTSTR lpszCaption)
{
	// let the base class handle most of this.
	SetWindowText(lpszCaption);

	// if we are floating, then we need to set the
	// floating frames title as well...
	if (IsFloating())
	{
        // cast a pointer to the mini dock frame window.
		CMiniDockFrameWnd* pParentFrame = DYNAMIC_DOWNCAST(CMiniDockFrameWnd, GetParentFrame());
		ASSERT_KINDOF(CMiniDockFrameWnd, pParentFrame);

        // set the text for the frame and the dock bar.
        pParentFrame->SetWindowText(lpszCaption);
        pParentFrame->m_wndDockBar.SetWindowText(lpszCaption);
	}
    
	// otherwise we just need to invalidate the docked bar.
	else
	{
		InvalidateRect(NULL);
	}
}

void CXTDockWindow::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu) 
{
	UNREFERENCED_PARAMETER(nIndex);

	if (!bSysMenu)
	{
		ASSERT(pPopupMenu != NULL);
		
		// check the enabled state of various menu items
		CCmdUI state;        
		state.m_pMenu = pPopupMenu;
		ASSERT(state.m_pOther == NULL);
		
		state.m_nIndexMax = pPopupMenu->GetMenuItemCount();
		for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax;
			 state.m_nIndex++)
		{
			state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);
			if (state.m_nID == 0)
				continue; // menu separator or invalid cmd - ignore it
				
			ASSERT(state.m_pOther == NULL);
			ASSERT(state.m_pMenu != NULL);
			if (state.m_nID == (UINT)-1)
			{
				// possibly a popup menu, route to first item of that popup
				state.m_pSubMenu = pPopupMenu->GetSubMenu(state.m_nIndex);
				if (state.m_pSubMenu == NULL ||
				    (state.m_nID = state.m_pSubMenu->GetMenuItemID(0)) == 0 ||
				    state.m_nID == (UINT)-1)
				{				    			 
			        continue; // first item of popup can't be routed to
			    }
			    state.DoUpdate(this, FALSE);  // popups are never auto disabled
			}
			else
			{
				// normal menu item
				// Auto enable/disable if command is _not_ a system command
				state.m_pSubMenu = NULL;
				state.DoUpdate(this, state.m_nID < 0xF000);
			}
		}
	}
}

#define XT_MSG_TEXT "Directly using a CView object as a child of a CXTDockWindow may have\nundesired results such as messages not correctly routing within the\nframework, see KB article \"005.007 - HOW TO: Use views with docking\nwindows \" for more details.\n\nIf you would like to use a CView derived class with a CXTDockWindow\nobject, please use the CreateFrameDocView(...) method instead, for example:\n\n// create the frame to display our view.\nCFrameWnd* pFrameWnd = CreateFrameDocView(\n    RUNTIME_CLASS(CFrameWnd), RUNTIME_CLASS(CDynamicHelpView), NULL);\n\n// get a pointer to the active view.\nm_pHelpView = DYNAMIC_DOWNCAST(\n    CDynamicHelpView, pFrameWnd->GetActiveView());\n\n// display the frame in the docking window.\nSetChild(pFrameWnd);"

void CXTDockWindow::SetChild(CWnd *pWnd)
{
    ASSERT(::IsWindow(m_hWnd));
    
    if (pWnd->IsKindOf(RUNTIME_CLASS(CView)))
    {
	    XT_ASSERT_MSG( FALSE, XT_MSG_TEXT );
    }

    m_pChildWnd = pWnd;
}

CFrameWnd* CXTDockWindow::CreateFrameDocView(
    CRuntimeClass* pFrameClass,
    CRuntimeClass* pViewClass,
    CDocument* pDocument/*=NULL*/,
    bool bNoBorder/*=true*/)
{
#ifdef _DEBUG
	ASSERT_VALID(this);

	ASSERT(pFrameClass != NULL);
	ASSERT(pFrameClass->IsDerivedFrom(RUNTIME_CLASS(CFrameWnd)));
	ASSERT(AfxIsValidAddress(pFrameClass, sizeof(CRuntimeClass), FALSE));

	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

    // create the frame object.
    CObject* pObject = pFrameClass->CreateObject();
    ASSERT_KINDOF(CFrameWnd, pObject);

    // cast a pointer to the frame.
    CFrameWnd* pFrameWnd = DYNAMIC_DOWNCAST(CFrameWnd, pObject);
    ASSERT_VALID(pFrameWnd);

    // initialize create context for frame.
    CCreateContext context;
    ::ZeroMemory(&context, sizeof(context));

    context.m_pCurrentDoc   = pDocument;
    context.m_pCurrentFrame = GetParentFrame();
    context.m_pNewViewClass = pViewClass;
    
    // create the frame.
    if (!pFrameWnd->Create(NULL, NULL, WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
        CRect(0,0,0,0), this, NULL, 0, &context))
    {
        TRACE0("Unable to create docking window frame.\n");
        return NULL;
    }

    // set the active view for the frame.
	CView* pView = NULL;
	if (pFrameWnd->GetActiveView() == NULL)
	{
		CWnd* pWnd = pFrameWnd->GetDescendantWindow(AFX_IDW_PANE_FIRST, TRUE);
		if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView)))
		{
			pView = DYNAMIC_DOWNCAST(CView, pWnd);
			pFrameWnd->SetActiveView(pView, FALSE);
		}
	}

    // turn off the frame border since we use the views
    // border for display by default.
    if ( bNoBorder && pFrameWnd )
    {
        pFrameWnd->ModifyStyleEx(
            WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
    }

    return pFrameWnd;
}
