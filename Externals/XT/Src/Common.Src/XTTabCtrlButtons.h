  // XTTabCtrlbuttons.h : interface for the CXTTabCtrlbuttons class.
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

#if !defined(__XTTABCTRLBUTTONS_H_)
#define __XTTABCTRLBUTTONS_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTTabCtrlButtons;
class CXTWndHook;


//////////////////////////////////////////////////////////////////////
// Summary: CXTTabCtrlButton is a stand alone structure class.  It is internal used in CXTTabCtrlButtons class.
class CXTTabCtrlButton
{
public:
	CXTTabCtrlButton (DWORD wStyle);
	
	void Draw(CDC* pDC, COLORREF clrButton);
	DWORD Click(CXTTabCtrlButtons* pWnd, CPoint pt, BOOL bRepeat = FALSE);
	void SetRect(CRect rc);
	CRect GetRect();
	BOOL PtInRect(POINT pt) const;
	void CheckForMouseOver (CWnd* pWnd, CPoint pt);
	void SetEnable(BOOL bEnable);

	CRect m_Rect;
	DWORD m_wStyle;
	CTabCtrl* m_pTabCtrl;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTTabCtrlButton::CXTTabCtrlButton(DWORD wStyle) {
	m_wStyle = wStyle;
}
AFX_INLINE void CXTTabCtrlButton::SetRect (CRect rc) {
	m_Rect = rc;
}
AFX_INLINE CRect CXTTabCtrlButton::GetRect() {
	return m_Rect;
}
AFX_INLINE BOOL CXTTabCtrlButton::PtInRect (POINT pt) const {
	return m_Rect.PtInRect (pt) != 0;
}

//////////////////////////////////////////////////////////////////////
// Summary: CXTTabCtrlButtonsis a CWnd derived class.  It is internally used in the CXTTabCtrlBase class.
class CXTTabCtrlButtons: public CWnd
{
public:
	CXTTabCtrlButtons();
	~CXTTabCtrlButtons();
	
	BOOL Create(CTabCtrl* pParent);
	
	int GetWidth();
	
	void ShowButtons(BOOL bShow);
	BOOL ShowButtons();
	void Refresh();
	void SubclassTabButtons(CWnd* pWnd);
    void SetButtonStyle(DWORD dwFlags);

private:
	CRect AdjustRect();
private:
	CXTTabCtrlButton m_btnClose;
	CXTTabCtrlButton m_btnLeft;
	CXTTabCtrlButton m_btnRight;

	DWORD m_dwFlags;
	CTabCtrl* m_pTabCtrl;
	BOOL m_bShow;

	HWND m_hwndSubclassed;
	HWND m_hwndPrimary;

private:

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);

	DECLARE_MESSAGE_MAP()

	friend class CXTTabCtrlBase;
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__XTTABCTRLBUTTONS_H_