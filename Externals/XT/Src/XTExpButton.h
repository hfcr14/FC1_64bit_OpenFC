// XTExpButton.h : header file
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

#if !defined(__EXPBUTTON_H__)
#define __EXPBUTTON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTToolBar;
class CXTExpMenuWnd;

/////////////////////////////////////////////////////////////////////////////
// Summary:  CXTExpButton is a helper class that renders a toolbar expansion button; 
//			 that is, a button that displays horizontal/vertical chevron (if a toolbar
//			 is partially hidden) and down arrow that indicates the toolbar
//			 is customizable.	
//
//			 The CXTExpButton button handles its 'button pressed' event by displaying 
//			 a popup window filled with buttons currently hidden by the toolbar and a button
//			 that invokes customize toolbar command. The popup gets closed once the toolbar
//			 owner receives a command, either from toolbar or from toolbar's owned popup.
//
//			 NB: This is a private XTreme toolkit class subject to change shall the need arise.
// 
// See Also: CXTExpMenuWnd
class CXTExpButton : public CXTButton
{
	DECLARE_DYNAMIC(CXTExpButton)

    CXTToolBar* m_pParent; // This window parent.
    CXTExpMenuWnd* m_popup;// Popup window that displays hidden buttons of m_pParent.

// Construction

public:

	// Summary: Constructs a CXTExpButton object.
	CXTExpButton();

// Attributes

public:

// Operations

public:

	// Input:	pParent - 
	//			nID - 
    // Summary:	Creates this button attached to a toolbar.
    BOOL Create(CXTToolBar* pParent, UINT nID);

	//{{AFX_VIRTUAL(CXTExpButton)

	protected:
	virtual void PostNcDestroy();
    virtual void DrawButtonBitmap(CDC* pDC, UINT nState, CRect& rcItem);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation

	protected:

	// Summary: Destroys a CXTExpButton object, handles clean up and de-allocation.
	//			NB: Do not delete object of this class directly, use DestroyWindow() instead.
	virtual ~CXTExpButton();

protected:

	//{{AFX_MSG(CXTExpButton)
	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	afx_msg void OnPaint();
	afx_msg void OnCustomize();
	//}}AFX_MSG
    
	afx_msg void OnPopupClosed();
	afx_msg void OnUpdateCustomize(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
private:

    void DrawBits(CDC* pDC, const CRect& rcItem, UINT nState, int cx, int cy, bool isTop, const WORD* bits);
	virtual DWORD GetXButtonStyle();
    virtual COLORREF GetColorFace();

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__EXPBUTTON_H__)
