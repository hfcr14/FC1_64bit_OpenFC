// XTDrawHelpers.h some useful classes for drawing
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

#if !defined(__XTDRAWHELPERS_H__)
#define __XTDRAWHELPERS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// CXTEmptyRect class

class CXTEmptyRect : public CRect
{
public:
	CXTEmptyRect()
	{
		SetRectEmpty();
	}
};

//////////////////////////////////////////////////////////////////////
// CXTWindowRect class

class CXTWindowRect : public CRect
{
public:
	CXTWindowRect (HWND hWnd)
	{
		::GetWindowRect (hWnd, this);
	};
	CXTWindowRect (const CWnd* pWnd)
	{
		::GetWindowRect (pWnd->GetSafeHwnd(), this);
	};
};

//////////////////////////////////////////////////////////////////////
// CXTClientRect class

class CXTClientRect : public CRect
{
public:
	CXTClientRect (const CWnd* pWnd)
	{
		::GetClientRect (pWnd->GetSafeHwnd(), this);
	};
};

//////////////////////////////////////////////////////////////////////
// CXTBufferDC class

class CXTBufferDC : public CDC
{
	HDC     m_hDestDC;
	CBitmap m_bitmap;     // Bitmap in Memory DC
	CRect   m_rect;
	HGDIOBJ m_hOldBitmap; // Previous Bitmap
	
public:
	CXTBufferDC (HDC hDestDC, const CRect rcPaint) : m_hDestDC (hDestDC)
	{
		m_rect = rcPaint;
		VERIFY(Attach (::CreateCompatibleDC (m_hDestDC)));
		m_bitmap.Attach (::CreateCompatibleBitmap (m_hDestDC, m_rect.right, m_rect.bottom));
		m_hOldBitmap = ::SelectObject (m_hDC, m_bitmap);
	}
	~CXTBufferDC ()
	{
		::BitBlt (m_hDestDC, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), m_hDC, m_rect.left, m_rect.top, SRCCOPY);
	    ::SelectObject (m_hDC, m_hOldBitmap);
	}
};

//////////////////////////////////////////////////////////////////////
// CXTFontDC class

class CXTFontDC
{
	CDC*     m_pDC;
	CFont* m_pOldFont;
public:
	CXTFontDC (CDC* pDC, CFont* pFont) : m_pDC (pDC)
	{
		m_pOldFont = (CFont*) m_pDC->SelectObject (pFont);
	}
	void SetFont(CFont* pFont)
	{
		m_pDC->SelectObject (m_pOldFont);
		m_pOldFont = (CFont*) m_pDC->SelectObject (pFont);
	}
	~CXTFontDC()
	{
		m_pDC->SelectObject (m_pOldFont);
	}
};

//////////////////////////////////////////////////////////////////////
// CXTPenDC class

class CXTPenDC
{
protected:
	CPen m_pen;
	HDC m_hDC;
	HPEN m_hOldPen;
	
public:
	CXTPenDC (HDC hDC, COLORREF crColor): m_hDC (hDC)
	{
		VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
		m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
	}
	
	~CXTPenDC ()
	{
		::SelectObject (m_hDC, m_hOldPen);
	}
	void Color (COLORREF crColor)
	{
		::SelectObject (m_hDC, m_hOldPen);
		VERIFY(m_pen.DeleteObject());
		VERIFY(m_pen.CreatePen (PS_SOLID, 1, crColor));
		m_hOldPen = (HPEN)::SelectObject (m_hDC, m_pen);
	}
};

//////////////////////////////////////////////////////////////////////
// CXTBrushDC class

class CXTBrushDC
{
protected:
	CBrush m_brush;
	HDC m_hDC;
	HBRUSH m_hOldBrush;
	
public:
	CXTBrushDC (HDC hDC, COLORREF crColor): m_hDC (hDC)
	{
		VERIFY(m_brush.CreateSolidBrush (crColor));
		m_hOldBrush = (HBRUSH)::SelectObject (m_hDC, m_brush);
	}
	~CXTBrushDC ()
	{
		::SelectObject (m_hDC, m_hOldBrush);
	}
};

class CXTCompatibleDC : public CDC
{
public:
	CXTCompatibleDC(CDC* pDC, CBitmap* pBitmap)
	{
		CreateCompatibleDC(pDC);	
		m_pBmp = SelectObject(pBitmap);
	}
	~CXTCompatibleDC()
	{
		SelectObject(m_pBmp);
		DeleteDC();
	}
	CBitmap* m_pBmp;
};

inline void Line(CDC* pDC, CPoint p0, CPoint p1)
{
	pDC->MoveTo(p0); 
	pDC->LineTo(p1);
} 
//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTDRAWHELPERS_H__)
