// XTTipWindow.h interface for the CXTTipWindow class.
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

#if !defined(__XTTIPWINDOW_H__)
#define __XTTIPWINDOW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// Summary: CXTTipWindow is a CWnd derived class. It is used to create a CXTTipWindow
//			object. CXTTipWindow objects are tooltip style popup windows that can
//			display useful information to the end user. Possible options include
//			shadows, borders, and background color settings.
class _XT_EXT_CLASS CXTTipWindow : public CWnd
{
public:

    // Summary: Constructs a CXTTipWindow object
    CXTTipWindow();

    // Summary: Destroys a CXTTipWindow object, handles cleanup and de-allocation
    virtual ~CXTTipWindow();

	// Input:   pParentWnd - Points to the parent of the tooltip window.
	// Returns: TRUE if successful, otherwise returns FALSE.
	// Summary: Call this member function to create the tooltip window.
	virtual BOOL Create(CWnd *pParentWnd);

protected:

	int          m_nLineSpace;			// Amount in pixels to space the title and description text.
	UINT         m_nElapseTimerEventID;	// Elapse timer event ID.
	UINT         m_nDelayTimerEventID;	// Delay timer event ID.
	BOOL         m_bDelayTimerRunning;	// TRUE if delay timer is running.
	DWORD        m_dwTipStyle;			// Initial style for the tip window. See ShowTipWindow(..).
	COLORREF     m_crBackColor;			// Background color for the tip window.
	COLORREF     m_crTextColor;			// Text color for the tip window.
	CRect        m_rcWindow;			// Initial size of the tip window.
	CRect        m_rcScreenWindow;		// Initial size of the desktop.
	CRect        m_rcShadow;			// Initial size of the tip window shadow.
	CWnd*        m_pParentWnd;			// Parent window.
	CSize        m_sizeMargin;			// Amount in pixels of the tip and side margins of where the tip text is displayed.
	CSize        m_sizeTitle;			// title text size.
	CString      m_strTitle;			// Title text for the tip window.
	CString      m_strDescrip;			// Description text for the tip window.
	CStringArray m_arStrings;			// Stores strings if the tip is multiline.

	int		 	 m_nTabPositions;
	BOOL		 m_bMouseWasClickedSoDoNotShowTip;
	BOOL		 m_bCenterHorz;
	LPINT		 m_pnTabStopPositions;
	DWORD		 m_nTicCount;
	CRect		 m_rectTipArea;
	CRect		 m_rectLast;
	CPoint		 m_pointTipOffsetPos;
	CPoint		 m_ptMousePos;

public:

	// Input:   pointOffset - A CPoint object that represents the x,y offset.
	// Summary: Call this member function to offset the tip window by the distance 
	//          specified by pointOffset.
	void SetTipOffsetPos(CPoint pointOffset);

	// Summary: Call this member function to hide the tip window
	void HideTipWindow();

	// Input:   pnTabStopPositions - pointer to integer array containing the tab 
	//          stop positions (could be a static array)
	//          nTabPositions - number of array elements.
	// Summary: Call this member function to set the text tab stop positions for the
	//          tooltip window.
	void SetTextTabStopPositions(LPINT pnTabStopPositions, int nTabPositions);

	// Input:   lpszTitle - A NULL terminated string that is to be displayed as the 
	//          title for the tip window.
	//          lpszDescrip - A NULL terminated string that is to be displayed as 
	//          the description text for the tip window.
	//          bRedraw - If TRUE, the tip window will be redrawn.
	// Summary: This member function will set the text for the title and description
    //          values that are displayed in the tip window.
    void SetTipText(LPCTSTR lpszTitle, LPCTSTR lpszDescrip, BOOL bRedraw=FALSE);

	// Input:   crBackColor - The RGB color value for the background of the tip window.
	//          crTextColor - The RGB color value for the text of the tip window.
	//          bRedraw - If TRUE, the tip window will be redrawn.
	// Summary: This member function will set the text and background colors for
    //          the tip window display.
    void SetTipColors(COLORREF crBackColor, COLORREF crTextColor, BOOL bRedraw=FALSE);

	// Input:   point - The initial x and y coordinates of where the tip window is 
	//          to be displayed.
	//          dwTipStyle - Initial style for the tip window. The desired styles
    //			for the tip window can be one or more of the following:
    //			[ul]
    //			[li]<b>TWS_XT_THICKBORDER</b> - This style will cause the tip
	//			       window to display a thick border around its edges.[/li]
    //			[li]<b>TWS_XT_DROPSHADOW</b> - This style will cause the tip
	//			       window to cast a shadow on the parent window.[/li]
	//			[li]<b>TWS_XT_ALPHASHADOW</b> - Used with TWS_XT_DROPSHADOW to display
	//			       an alpha style shadow. By default, mono shadowing is enabled.[/li]
    //			[/ul]
	//          nDelay - Specifies the delay value in milliseconds of when
    //			the tip window should show.
	//          nElapse - Specifies the time-out value in milliseconds of when
    //			the tip window should close.
	//          bCenterHorz - Set this parameter to 'true' to center the window horizontally on
	//			the point passed in.
	// Summary: This member function will display a tip window to display a title
	//          and description text. Returns TRUE if the tip window was successfully
	//          created, otherwise returns FALSE.
    virtual BOOL ShowTipWindow(const CPoint& point, DWORD dwTipStyle=TWS_XT_DROPSHADOW, UINT nDelay=0, UINT nElapse=5000, BOOL bCenterHorz = FALSE);

	// Input:   rectCtrl - Reference to a CRect object that represents the size of the 
	//          control window where the tip is displayed.
	//          pointMousePos - Reference to a CPoint object that represents the x,y 
	//          position of the mouse cursor.
	//          dwTipStyle - Initial style for the tip window. The desired styles
    //			for the tip window can be one or more of the following:
    //			[ul]
    //			[li]<b>TWS_XT_THICKBORDER</b> - This style will cause the tip
	//			       window to display a thick border around its edges.[/li]
    //			[li]<b>TWS_XT_DROPSHADOW</b> - This style will cause the tip
	//			       window to cast a shadow on the parent window.[/li]
	//			[li]<b>TWS_XT_ALPHASHADOW</b> - Used with TWS_XT_DROPSHADOW to display
	//			       an alpha style shadow. By default, mono shadowing is enabled.[/li]
    //			[/ul]
	//          nDelay - Specifies the delay value in milliseconds of when
    //			the tip window should show.
	//          nElapse - Specifies the time-out value in milliseconds of when
    //			the tip window should close.
	//          bCenterHorz - Set this parameter to 'true' to center the window horizontally on
	//			the point passed in.
	//          bShowTipAtCursorPos - TRUE to show the tip at the cursor position.
	// Summary: This member function will display a tip window to display a title
	//          and description text. Returns TRUE if the tip window was successfully
	//          created, otherwise returns FALSE.
	virtual BOOL ShowTipWindow(const CRect& rectCtrl, const CPoint& pointMousePos, DWORD dwTipStyle=TWS_XT_DROPSHADOW, UINT nDelay=0, UINT nElapse=5000, BOOL bCenterHorz=FALSE, BOOL bShowTipAtCursorPos=FALSE);

	// Input:   size - Initial size for the horizontal and vertical text margins.
	// Summary: This member function will set the text margins for the tip window.
    void SetMargins(CSize size);

	// Input:   nLineSpace - Initial size for text line spacing.
    // Summary: This member function will set the line spacing for the tip window.
    void SetLineSpace(int nLineSpace);

protected:

    // Input:   pDC - Device context of the window to cast the shadow on.
	//          rect - Reference to a CRect object that represents the initial size of the shadow.
	// Summary: This member function will draw a shadow rect to the device context
    //          specified by 'pDC'.
    void DrawShadowRect(CDC *pDC, const CRect& rect);

	// Input:   point - The x and y coordinates of where the tip window is to be displayed.
	//          bCenterHorz - Horizontal center the window position.
	// Summary: This member function will set the sizes for the tip window.
    void InitializeSize(const CPoint& point, BOOL bCenterHorz = FALSE);

	// Used internal to process the button messages

	void ForwardMessage(UINT uiMsg, UINT nFlags, CPoint point);

    //:Ignore
    //{{AFX_VIRTUAL(CXTTipWindow)
    //}}AFX_VIRTUAL

    //:Ignore
    //{{AFX_MSG(CXTTipWindow)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTTipWindow::SetTipColors(COLORREF crBackColor, COLORREF crTextColor, BOOL bRedraw) {
    m_crBackColor = crBackColor; m_crTextColor = crTextColor; if (bRedraw) { InvalidateRect(NULL); }
}
AFX_INLINE void CXTTipWindow::SetMargins(CSize size) {
    m_sizeMargin.cx = __max(0, size.cx); m_sizeMargin.cy = __max(0, size.cy); //EM_BM
}
AFX_INLINE void CXTTipWindow::SetLineSpace(int nLineSpace) {
    m_nLineSpace = __max(0, nLineSpace);
}
AFX_INLINE void CXTTipWindow::SetTipOffsetPos(CPoint pointOffset) {
    m_pointTipOffsetPos = pointOffset;
}

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTTIPWINDOW_H__)