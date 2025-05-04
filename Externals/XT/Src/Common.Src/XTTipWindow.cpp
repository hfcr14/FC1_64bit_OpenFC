// XTTipWindow.cpp : implementation of the CXTTipWindow class.
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

/////////////////////////////////////////////////////////////////////////////
// Many thanks to Michael Berlenz for helping us by reworking the CXTTipWindow
// class and sample project to add enhanced functionality...kudos!
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTTipWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTTipWindow
/////////////////////////////////////////////////////////////////////////////

/* CXTTipWindow
 * ----------------------------------------------------------------------------
 */
CXTTipWindow::CXTTipWindow()
{
    m_sizeMargin                     = CSize(0,0);
    m_nLineSpace                     = 0;
    m_dwTipStyle                     = 0;
    m_nElapseTimerEventID            = 1;
    m_pParentWnd                     = NULL;
    m_strTitle                       = _T("");
    m_strDescrip                     = _T("");
    m_crBackColor                    = ::GetSysColor(COLOR_INFOBK);
    m_crTextColor                    = ::GetSysColor(COLOR_INFOTEXT);
    m_sizeTitle                      = CSize(0,0);
    m_bMouseWasClickedSoDoNotShowTip = FALSE;
    m_ptMousePos                     = CPoint(0,0);
    m_bCenterHorz                    =  FALSE;
    m_nDelayTimerEventID             = 2;
    m_bDelayTimerRunning             = FALSE;
    m_pointTipOffsetPos              = CPoint(0,20); //20 = mouse cursor height
    m_pnTabStopPositions             = NULL;
    m_nTabPositions                  = 0;

	m_rectTipArea.SetRectEmpty();
}

/* ~CXTTipWindow
 * ----------------------------------------------------------------------------
 */
CXTTipWindow::~CXTTipWindow()
{
}

/* Create
 * ----------------------------------------------------------------------------
 */
BOOL CXTTipWindow::Create(CWnd * pParentWnd)
{
	// register the wnd class.
	CString wndClass = ::AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW,0, CBrush(m_crBackColor));

	// call the base class for creation.
    if( !CWnd::CreateEx( /*WS_EX_TOOLWINDOW | WS_EX_TOPMOST*/ 0 , wndClass, _T(""), WS_POPUP, 0, 0, 0, 0, pParentWnd->m_hWnd, 0, NULL ))
	{
		TRACE0( "Failed to create popup window.\n" );
		return FALSE;
	}
	m_pParentWnd = pParentWnd;
	return TRUE;
}

/* MESSAGE_MAP
 * ----------------------------------------------------------------------------
 */
BEGIN_MESSAGE_MAP(CXTTipWindow, CWnd)
	//{{AFX_MSG_MAP(CXTTipWindow)
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/* ShowTipWindow
 * ----------------------------------------------------------------------------
 */
BOOL CXTTipWindow::ShowTipWindow(const CPoint& point, DWORD dwTipStyle/*=TWS_XT_DROPSHADOW*/, UINT nDelay/*=0*/, UINT nElapse/*=5000*/, BOOL bCenterHorz /*= FALSE*/) 
{
	//Check for valid tip text
	if(m_strTitle.IsEmpty() && m_strDescrip.IsEmpty())
		return FALSE;

	// Save values for InitializeSize() function
	m_ptMousePos = point;
	m_bCenterHorz = bCenterHorz;

	// Return if process is running
	if (m_bDelayTimerRunning || IsWindowVisible())
		return FALSE;

	// Analyze tip text
	m_arStrings.RemoveAll();
	if (m_strDescrip.Find(_T("\n"))!=-1)
	{
		CString strDesc(m_strDescrip);

		while(strDesc.Find(_T("\n"))!=-1)
		{
			int nDelim = strDesc.Find(_T("\n"));
			CString strAdd = strDesc.Left(nDelim);
			m_arStrings.Add(strAdd);
			strDesc = strDesc.Mid(nDelim+1);
		}
		m_arStrings.Add(strDesc);
	}
	
	// Save the tip style
	m_dwTipStyle = dwTipStyle;

	if (!nDelay)
	{
		// Show the tip window
		InitializeSize(m_ptMousePos, m_bCenterHorz);
		SetWindowPos(&wndTop, m_rcScreenWindow.left, m_rcScreenWindow.top, m_rcShadow.right - m_rcWindow.left,
			m_rcShadow.bottom - m_rcWindow.top, SWP_SHOWWINDOW|SWP_NOACTIVATE); //see also InitializeSize()
		SetCapture();
	}

	// If a time-out value was specified, start the timer.
	if (nElapse || nDelay)
	{
		if (nDelay)
		{
			SetTimer(m_nDelayTimerEventID, nDelay, NULL); //if timer runns out, window will be shown
			m_bDelayTimerRunning = TRUE;
		}

		if (nElapse)
			SetTimer(m_nElapseTimerEventID, nDelay + nElapse, NULL); //if timer runns out, window will be hide
	}

	return TRUE; // success!
}

/* ShowTipWindow
 * ----------------------------------------------------------------------------
 * rectCtrl = if the mouse is inside this rectangle the tip will be shown
 *          => client coordinates of the window that owns this tip window
 * pointMousePos => client coordinates of the window that owns this tip window
 */
BOOL CXTTipWindow::ShowTipWindow(const CRect& rectCtrl, const CPoint& pointMousePos, DWORD dwTipStyle/*=TWS_XT_DROPSHADOW*/, UINT nDelay/*=0*/, UINT nElapse/*=5000*/, BOOL bCenterHorz /*=FALSE*/, BOOL bShowTipAtCursorPos/*=FALSE*/)
{
	//Check for valid tip text
	//	if(m_strTitle.IsEmpty() && m_strDescrip.IsEmpty()) //Do not process this check here!
	//		return FALSE;

	//Check for zero rectangle
	if (rectCtrl.IsRectNull())
		return ShowTipWindow(pointMousePos, dwTipStyle, nDelay, nElapse, bCenterHorz);

	//Check mouse was clicked -> do not show tip window
	if(m_bMouseWasClickedSoDoNotShowTip && rectCtrl == m_rectLast)
		return FALSE;

	//In case the tip window is visible hide it
	HideTipWindow();

	//Calculate offset for bShowTipAtCursorPos = TRUE
	CPoint pointMove = pointMousePos - rectCtrl.TopLeft();
	pointMove.Offset(m_pointTipOffsetPos);

	//Copy rectCtrl and move rectangle to zero origin
	m_rectTipArea = rectCtrl;
	m_rectTipArea.OffsetRect(-rectCtrl.TopLeft());
	
	//Offset rectangle
	CPoint ptTipWindowPos = rectCtrl.TopLeft();
	if(bShowTipAtCursorPos) //else show window at top left corner of rectCtrl
	{
		m_rectTipArea.OffsetRect(-pointMove);
		ptTipWindowPos += pointMove;
	}

	m_bMouseWasClickedSoDoNotShowTip = FALSE;
	m_rectLast = rectCtrl;

	m_pParentWnd->ClientToScreen(&ptTipWindowPos);
	return ShowTipWindow(ptTipWindowPos, dwTipStyle, nDelay, nElapse, bCenterHorz);
}

/* HideTipWindow
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::HideTipWindow()
{
	// Kill the timers
	KillTimer(m_nElapseTimerEventID);
	KillTimer(m_nDelayTimerEventID);
	m_bDelayTimerRunning = FALSE;

	// Release capture
	if(GetCapture() == this)
		ReleaseCapture();

	// Hide the tip window
	if(IsWindowVisible())
		ShowWindow(SW_HIDE);

	//Initialize variables
	m_rectTipArea.SetRectEmpty();
}

/* InitializeSize
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::InitializeSize(const CPoint& point, BOOL bCenterHorz /*FALSE*/)
{
	// Create a temporary window dc, we are not actually
	// going to paint anything, we just want to get the sizes
	// of the title and description text.
	CWindowDC dc(NULL);

	// Get the size of the title string.
	CFont* pOldFont = dc.SelectObject(&xtAfxData.fontBold);
	m_sizeTitle = dc.GetTabbedTextExtent(m_strTitle, m_nTabPositions, m_pnTabStopPositions);

	// Get the size of the description string.
	dc.SelectObject(&xtAfxData.font);

	CSize sizeDescrip(0,0);	// description text size.
	if (m_arStrings.GetSize() == 0)
	{
		sizeDescrip = dc.GetTabbedTextExtent(m_strDescrip, m_nTabPositions, m_pnTabStopPositions);
	}
	else
	{
		TEXTMETRIC tm;			// get the text metrics for the device context that is using the 
		dc.GetTextMetrics(&tm); // control's font, this will give us the text height in pixels.

		CSize sizeTemp;
		int nStrings;
		for (nStrings = 0; nStrings < m_arStrings.GetSize(); ++nStrings)
		{
			// Strings could be empty because only a line break could be in the line.
			// A empty string will return zero text extend. But we still have to count this line!
			if(m_arStrings[nStrings].IsEmpty())
				sizeDescrip.cy += tm.tmHeight;
			else
			{
				sizeTemp = dc.GetTabbedTextExtent(m_arStrings[nStrings], m_nTabPositions, m_pnTabStopPositions);
				sizeDescrip.cy += sizeTemp.cy;
				sizeDescrip.cx = __max(sizeDescrip.cx, sizeTemp.cx);
			}
		}
	}
	
	// Restore GDI object.
	dc.SelectObject(pOldFont);

	// Define the width of the tip window.
	int nWidth = __max(m_sizeTitle.cx, sizeDescrip.cx);
	nWidth = (m_sizeMargin.cx*2 + nWidth);

	// Define the height of the tip window.
	int nHeight = (m_sizeMargin.cy*2 + ((m_strTitle.GetLength()>0) ? m_sizeTitle.cy + m_nLineSpace : 0)  + sizeDescrip.cy);

	// Set the sizes for the tip window and its shadow.
	m_rcScreenWindow.left   = point.x;
	m_rcScreenWindow.top	= point.y;
	m_rcScreenWindow.right  = point.x + nWidth + 2; //+2 for better view
	m_rcScreenWindow.bottom = point.y + nHeight;

	if (bCenterHorz)
		m_rcScreenWindow.OffsetRect(-m_rcScreenWindow.Width()/2, 0);

	// Initialize the size of the shadow rect.
	m_rcWindow = m_rcScreenWindow;
	m_rcWindow.top = 0;
	m_rcWindow.left = 0;
	m_rcWindow.right = m_rcScreenWindow.Width();
	m_rcWindow.bottom = m_rcScreenWindow.Height();

	m_rcShadow = m_rcWindow;
	if (m_dwTipStyle & TWS_XT_DROPSHADOW)
		m_rcShadow.OffsetRect(5,5);
}

/* OnKillFocus
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);
	
	HideTipWindow();

	// Repaint the area where the shadow was displayed.
	if (m_dwTipStyle & TWS_XT_DROPSHADOW)
	{
		m_pParentWnd->RedrawWindow( 0, 0,
			RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN );
	}
}

/* CheckValue
 * ----------------------------------------------------------------------------
 */
int CheckValue(int iValue)
{
	return ((iValue > 255) ? 255 : ((iValue < 0) ? 0 : iValue));
}

/* AlphaPixel
 * ----------------------------------------------------------------------------
 */
COLORREF AlphaPixel(COLORREF crPixel, int i)
{
	return RGB(
		CheckValue(GetRValue(crPixel)-i),
		CheckValue(GetGValue(crPixel)-i),
		CheckValue(GetBValue(crPixel)-i));
}

/* DrawShadowRect
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::DrawShadowRect(CDC *pDC, const CRect& rcShadow)
{
	if ((m_dwTipStyle & TWS_XT_ALPHASHADOW) == 0)
	{
		// Bit pattern for a monochrome brush with every
		// other pixel turned off
		WORD bits[] = {
			0x0055, 0x00AA, 0x0055, 0x00AA,
			0x0055, 0x00AA, 0x0055, 0x00AA
		};

		// Need a monochrome pattern bitmap
		CBitmap bitmap;
		bitmap.CreateBitmap( 8, 8, 1, 1, &bits );

		// Create the pattern brush
		CBrush brush;
		brush.CreatePatternBrush( &bitmap );
		CBrush *pOldBrush = pDC->SelectObject( &brush );

		// Turn every other pixel to black
		COLORREF clrBk   = pDC->SetBkColor( RGB(0x00,0x00,0x00) );
		COLORREF clrText = pDC->SetTextColor( RGB(0xFF,0xFF,0xFF) );
		
		// 0x00A000C9 is the ROP code to AND the brush with the destination
		pDC->PatBlt(rcShadow.left, rcShadow.top, rcShadow.Width(), rcShadow.Height(), 
			(DWORD)0x00A000C9);			//DPa - raster code

		// Restore the device context
		pDC->SelectObject( pOldBrush );
		pDC->SetTextColor( clrText );
		pDC->SetBkColor( clrBk );

		brush.DeleteObject();
		bitmap.DeleteObject();
	}
	else
	{
		CRect rc1 = rcShadow;
		CRect rc2 = rcShadow;
		CRect rc3 = rcShadow;
		CRect rc4 = rcShadow;
		CRect rc5 = rcShadow;
		CRect rc6 = rcShadow;

		rc2.DeflateRect(1,1);
		rc3.DeflateRect(2,2);
		rc4.DeflateRect(3,3);
		rc5.DeflateRect(4,4);
		rc6.DeflateRect(5,5);
		
		int cx = rcShadow.Width()  + 5;
		int cy = rcShadow.Height() + 5;
		
		int x;
		for (x = 5; x < cx; ++x)
		{
			int y;
			for (y = 5; y < cy; ++y)
			{
				CPoint pt(x,y);
				int iAlpha = 0;
				
				// area covered by menu...continue.
				if (rc6.PtInRect(pt)) {
					continue;
				}
				else if (rc5.PtInRect(pt)) {
					iAlpha = 75;
				}
				else if (rc4.PtInRect(pt)) {
					iAlpha = 60;
				}
				else if (rc3.PtInRect(pt)) {
					iAlpha = 45;
				}
				else if (rc2.PtInRect(pt)) {
					iAlpha = 30;
				}
				else if (rc1.PtInRect(pt)) {
					iAlpha = 15;
				}
				if  (iAlpha != 0)
				{
					COLORREF crAlpha = AlphaPixel(pDC->GetPixel(x,y), iAlpha);
					pDC->SetPixel(x,y,crAlpha);
				}
			}
		}
	}
}

/* OnPaint
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// Set the background mode to transparent.
	dc.SetBkMode(TRANSPARENT);

	// Draw a shadow on the the parent window, we will need the
	// parent's device context to do this.
	if (m_dwTipStyle & TWS_XT_DROPSHADOW)
		DrawShadowRect(&dc, m_rcShadow);

	// use the system tooltip colors for text and background.
	dc.SetTextColor(m_crTextColor);
	dc.FillSolidRect(m_rcWindow, m_crBackColor);

	// Draw a border around the window.
	dc.Draw3dRect(m_rcWindow, xtAfxData.clr3DDkShadow,
		xtAfxData.clr3DDkShadow);

	// Draw an inner light grey border.
	if (m_dwTipStyle & TWS_XT_THICKBORDER)
	{
		CRect rect(m_rcWindow); rect.DeflateRect(1,1);
		dc.Draw3dRect(rect, xtAfxData.clr3DShadow,
			xtAfxData.clr3DShadow);
	}

	// Draw the title text.
	CFont* pOldFont = dc.SelectObject(&xtAfxData.fontBold);
	dc.TabbedTextOut(m_sizeMargin.cx + 1, m_sizeMargin.cy, m_strTitle, m_nTabPositions,
		m_pnTabStopPositions, m_sizeMargin.cx + 1);

	// Draw the description text.
	dc.SelectObject(&xtAfxData.font);

	if (m_arStrings.GetSize() == 0)
		dc.TabbedTextOut(m_sizeMargin.cx + 1, m_sizeMargin.cy + ((m_strTitle.GetLength()>0) ? m_sizeTitle.cy + m_nLineSpace : 0),
			m_strDescrip, m_nTabPositions, m_pnTabStopPositions, m_sizeMargin.cx + 1);
	else
	{
		// Get the string height for stepping through
		CSize sizeDesc = dc.GetTabbedTextExtent(m_arStrings[0], m_nTabPositions, m_pnTabStopPositions);

		// The second parameter is long because we add the height of the strings already done, and also
		// we don't add anything to get below the title if the length of the title is 0.
		int nStrings;
		for (nStrings = 0; nStrings < m_arStrings.GetSize(); ++nStrings)
			dc.TabbedTextOut(m_sizeMargin.cx + 1,
			m_sizeMargin.cy + (nStrings * sizeDesc.cy) + ((m_strTitle.GetLength()>0) ? m_sizeTitle.cy + m_nLineSpace : 0),
			m_arStrings[nStrings], m_nTabPositions, m_pnTabStopPositions, m_sizeMargin.cx + 1);
	
	}

	// Restore the device context.
	dc.SelectObject(pOldFont);
}

/* OnTimer
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::OnTimer(UINT_PTR nIDEvent) 
{
	if (m_nElapseTimerEventID == nIDEvent)
	{
		// Hide the tip window.
		HideTipWindow();
	}

	if (m_nDelayTimerEventID == nIDEvent)
	{
		KillTimer(m_nDelayTimerEventID);
		if (!m_rectTipArea.IsRectNull())
		{
			//If someone moves the mouse over a tip area in the client window
			//it may happen that the tip delay timer will be started. During this
			//delay time the user may move the mouse out of the application window.
			//After the timer runs out the tip window will be shown but because
			//of the fast mouse move the cursor will not be located inside the tip area.
			//Because the cursor my located outside the application we will not get
			//the mouse move messages at CXTTipWindow::OnMouseMove().
			//So better check if the mouse is still located inside the tip area.
			//If not do not show the tiop window.
			CRect rect = m_rectTipArea;
			rect.left += m_ptMousePos.x;
			rect.top += m_ptMousePos.y;
			rect.right += m_ptMousePos.x;
			rect.bottom += m_ptMousePos.y;
	
			CPoint pointMouse;
			GetCursorPos(&pointMouse);
			if(!rect.PtInRect(pointMouse))
				return;
		}

		// Show the tip window
		InitializeSize(m_ptMousePos, m_bCenterHorz); //Calculates m_rcScreenWindow values
		SetWindowPos(&wndTop, m_rcScreenWindow.left, m_rcScreenWindow.top, m_rcScreenWindow.Width(), m_rcScreenWindow.Height(), SWP_SHOWWINDOW|SWP_NOACTIVATE); //+5 because of the shadow
		SetCapture();
		m_bDelayTimerRunning = FALSE;
	}

	CWnd::OnTimer(nIDEvent);
}

/* OnEraseBkgnd
 * ----------------------------------------------------------------------------
 */
BOOL CXTTipWindow::OnEraseBkgnd(CDC* pDC) 
{
	UNREFERENCED_PARAMETER(pDC);
	return FALSE;
}

/* ForwardMessage
 * ----------------------------------------------------------------------------
 * Hide the tip window and forward the message to the window at mouse point.
 */
void CXTTipWindow::ForwardMessage(UINT uiMsg, UINT nFlags, CPoint point) 
{
	m_bMouseWasClickedSoDoNotShowTip = TRUE;

	HideTipWindow();

	CPoint pt(point);
	ClientToScreen(&pt);
	CWnd* pClick = WindowFromPoint(pt);
	
	if (!pClick->IsChild(this) && pClick != this)
	{
		int iHitTest = pClick->SendMessage(WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));
		pClick->SendMessage(WM_MOUSEACTIVATE, (LPARAM)AfxGetMainWnd()->m_hWnd, MAKELPARAM(iHitTest , uiMsg));
		pClick->ScreenToClient(&pt);
		pClick->SendMessage(uiMsg, nFlags, MAKELPARAM(pt.x, pt.y));
	}
	
	CWnd::OnLButtonDown(nFlags, point);
}

/* OnLButtonDown
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ForwardMessage(WM_LBUTTONDOWN, nFlags, point);
}

/* OnRButtonDown
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ForwardMessage(WM_RBUTTONDOWN, nFlags, point);
}

/* OnMButtonDown
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::OnMButtonDown(UINT nFlags, CPoint point) 
{
	ForwardMessage(WM_MBUTTONDOWN, nFlags, point);
}

/* OnMouseMove
 * ----------------------------------------------------------------------------
 * After the tip window is visible OnMouseMove will check if the mouse position
 * is located outside the specified tip area. If so it will hide the tip window.
 */
void CXTTipWindow::OnMouseMove(UINT nFlags, CPoint point)
{
	UNREFERENCED_PARAMETER(nFlags);
	if (!m_rectTipArea.IsRectNull() && !m_rectTipArea.PtInRect(point))
		HideTipWindow();
}

/* SetTipText
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::SetTipText(LPCTSTR lpszTitle, LPCTSTR lpszDescrip, BOOL bRedraw)
{
    m_strTitle   = lpszTitle;      //Single line

#if _MSC_VER < 1200 // MFC 5.0
	// Avoid uggly outputed rectangle character in the tip window
	CXTString strDescrip = lpszDescrip;
	strDescrip.Remove(_T('\r'));
	m_strDescrip = strDescrip;
#else
	m_strDescrip = lpszDescrip;    //Could be multi-line
	m_strDescrip.Remove(_T('\r'));

#endif
	
	if(bRedraw) {
		InvalidateRect(NULL);
	}
}

/* SetTextTabStopPositions
 * ----------------------------------------------------------------------------
 */
void CXTTipWindow::SetTextTabStopPositions(LPINT pnTabStopPositions, int nTabPositions)
{
	//pointer to integer array containing the tab stop positions (could be a static array)
    m_pnTabStopPositions = pnTabStopPositions;

	//number of array elements
    m_nTabPositions = nTabPositions;
}
