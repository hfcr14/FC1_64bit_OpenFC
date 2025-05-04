// XTExpMenuWnd.h : header file
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

#if !defined(__XTEXPMENUWND_H__)
#define __XTEXPMENUWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CXTExpMenuWnd;

/////////////////////////////////////////////////////////////////////////////
// Summary: CXTHiddenButtonsToolbar is a toolbar custom-tailored for usage
//          in a toolbar expansion popup window.
//          NB: This is a private toolkit class subject to change.
class CXTHiddenButtonsToolbar : public CXTToolBar
{
    DECLARE_DYNAMIC(CXTHiddenButtonsToolbar)
	
	CXTExpMenuWnd* m_pPopup; // Back reference to the toolbar expansion popup.

public:

	// Summary:	Constructs a CXTHiddenButtonsToolbar object.
	CXTHiddenButtonsToolbar() : m_pPopup(0) { }

    // Returns: Returns a RGB value that represents the background color.
	// Summary:	Defines background color as that used in popup windows
    virtual COLORREF GetBackgroundColor() const;

    // Returns: Returns true if successful, otherwise returns false.
	// Summary:	Tells to always fill the background of this toolbar with its
	//          background color 
    virtual bool IsFillToolBarClientRect();

    // Returns: Returns a pointer to a CXTColorPopup object.
	// Input:	pDropButton - Pointer to a XTDROPDOWNBUTTON structure.
	//			rect - Size of the area to display.
	// Summary:	Creates and initializes color picker popup window.
	//			The color picker window will send close notification to expansion popup.
	virtual CXTColorPopup* CreateNewColorPopup(XT_DROPDOWNBUTTON* pDropButton,CRect rect);

	friend CXTExpMenuWnd;
};

/////////////////////////////////////////////////////////////////////////////
// Summary: CXTHiddenButtonsWndHook is used to hook a toolbar owner frame ???
//          to figure out when to close the popup. Its WindowProc() filters out
//          the commands the owner frame receives and, if applicable, sends request
//          for the toolbar expansion popup to close.
class CXTHiddenButtonsWndHook : public CXTWndHook
{
    DECLARE_DYNAMIC(CXTHiddenButtonsWndHook)
	
	CXTExpMenuWnd* m_pPopup; // Back reference to the toolbar expansion popup.
public:

	// Summary: Constructs a CXTHiddenButtonsWndHook object.
	CXTHiddenButtonsWndHook() : m_pPopup(0) { }

	// Input:	wParam - Specifies the Windows message to be processed.
	//			wParam - Provides additional information used in processing the message.  The parameter
    //			value depends on the message.
	//			lParam - Provides additional information used in processing the message.  The parameter
    //			value depends on the message.
	// Summary:	Filters messages as required to synch up the hidden toolbar popup 
    virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);

	friend CXTExpMenuWnd;
};


/////////////////////////////////////////////////////////////////////////////
// Summary: CXTExpMenuWnd popup window displays a toolbar that contains buttons
//          currently hidden in the parent toolbar. Optionally, this window can display
//          another toolbar that contains a set of predefined commands (currently,
//          customize toolbar is implemented).
//
// See Also: CXTExpButton
class CXTExpMenuWnd : public CWnd
{
    DECLARE_DYNAMIC(CXTExpMenuWnd)

	bool					m_bDestroying;		// Tells if the window gets destroyed right now.
	CRect					m_exclusionRect;	// Exclusion rectangle for drawing adjacent borders.
	CXTToolBar*				m_pParent;			// A toolbar whose hidden buttons are displayed here.
	CWnd* const				m_pWndNotify;		// A window to send notifications to.
	const UINT				m_nDestroyedMsg;	// A message to post to the parent that this window is shutting down.
    CXTHiddenButtonsToolbar m_toolbar;			// A toolbar that displays hidden buttons of m_pParent.
    CXTHiddenButtonsToolbar m_toolbarCommands;	// A toolbar to draw predefined commands.
	CXTHiddenButtonsWndHook m_hook;				// Message hook that properly closes this window.

public:
	
	// Input:	pWndNotify - A window to notify when this popup closes.
	//			nDestroyedMsg - Notification message to send to the notification listener.
	// Summary:	Constructs a CXTExpMenuWnd object 
	CXTExpMenuWnd(CWnd* pWndNotify,UINT nDestroyedMsg);

// Operations
public:
    
	// Input:	rect - The popup will be displayed adjacent to this rectangle.  The exact
	//			shared border depends on the style of the toolbar.
	//			pParent - A toolbar whose hidden buttons and optional 'customize' command
	//			is to be displayed.
	// Summary:	Creates this window, attaches it to a toolbar, and displays its buttons 
    BOOL Create(CRect rect,CXTToolBar* pParent);

	//{{AFX_VIRTUAL(CXTExpMenuWnd)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	
	// Summary:	Destroys a CXTExpMenuWnd object, handles clean up and de-allocation.
	//          NB: Do not delete objects of this class directly, use DestroyWindow() instead.
	virtual ~CXTExpMenuWnd();

	//{{AFX_MSG(CXTExpMenuWnd)

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnCustomize();
	//}}AFX_MSG

#if _MSC_VER > 1200 //MFC 7.0
    afx_msg void OnActivateApp(BOOL bActive, DWORD dwThreadID);
#else
    afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);
#endif //MFC 7.0

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTEXPMENUWND_H__)
