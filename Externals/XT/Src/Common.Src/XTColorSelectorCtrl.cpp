// XTColorSelectorCtrl.cpp : implementation file
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

#include "stdafx.h"
#include "XTDefines.h"
#include "XTResource.h"
#include "XTGlobal.h"
#include "XTColorDialog.h"
#include "XTColorSelectorCtrl.h"
#include "XTPriv.h"
#include "XTHelpers.h"
#include "XTMemDC.h"
#include "XTFunctions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CUIntArray CXTColorSelectorCtrl::m_arUserDefColors;

/////////////////////////////////////////////////////////////////////////////
// CXTColorSelectorCtrl

CXTColorSelectorCtrl::CXTColorSelectorCtrl()
{
	m_pColorDlg     = NULL;
	m_point 		= CPoint(0,0);
	m_nCols 		= 8;
	m_nCurSel		= -1;
	m_sizeButton	= CSize(18,18);
	m_dwPopup		= NULL;
	m_pParentWnd	= NULL;
	m_clrColor		= CLR_DEFAULT;
	m_clrDefault	= CLR_DEFAULT;
	m_nPressed		= -1;
	m_rcBorders		= CRect(0, 0, 0, 0);
}



CXTColorSelectorCtrl::~CXTColorSelectorCtrl()
{

	// Free memory allocated to create color cells.
	while (!m_arCells.IsEmpty())
	{
		XT_COLOR_CELL* pColorCell = m_arCells.RemoveHead();
		SAFE_DELETE (pColorCell);
	} 
}


BEGIN_MESSAGE_MAP(CXTColorSelectorCtrl, CWnd)
	//{{AFX_MSG_MAP(CXTColorSelectorCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE_VOID(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

struct XT_PICK_BUTTON
{
	COLORREF clrButton;
	UINT	 toolTipID;
};

static XT_PICK_BUTTON standardColors[] =
{
	{	RGB(0x00,0x00,0x00),	XT_IDS_CLR_BLACK			},
	{	RGB(0x80,0x80,0x80),	XT_IDS_CLR_GRAY 			},
	{	RGB(0x80,0x00,0x00),	XT_IDS_CLR_MAROON			},
	{	RGB(0x80,0x80,0x00),	XT_IDS_CLR_OLIVE			},
	{	RGB(0x00,0x80,0x00),	XT_IDS_CLR_GREEN			},
	{	RGB(0x00,0x80,0x80),	XT_IDS_CLR_TEAL 			},
	{	RGB(0x00,0x00,0x80),	XT_IDS_CLR_NAVY 			},
	{	RGB(0x80,0x00,0x80),	XT_IDS_CLR_PURPLE			},
	{	RGB(0xFF,0xFF,0xFF),	XT_IDS_CLR_WHITE			},
	{	RGB(0xC0,0xC0,0xC0),	XT_IDS_CLR_SILVER			},
	{	RGB(0xFF,0x00,0x00),	XT_IDS_CLR_RED				},
	{	RGB(0xFF,0xFF,0x00),	XT_IDS_CLR_YELLOW			},
	{	RGB(0x00,0xFF,0x00),	XT_IDS_CLR_LIME 			},
	{	RGB(0x00,0xFF,0xFF),	XT_IDS_CLR_AQUA 			},
	{	RGB(0x00,0x00,0xFF),	XT_IDS_CLR_BLUE 			},
	{	RGB(0xFF,0x00,0xFF),	XT_IDS_CLR_FUCHSIA			},
};

static XT_PICK_BUTTON extendedColors[] =
{
	{	RGB(0x00,0x00,0x00),	XT_IDS_CLR_BLACK			},
	{	RGB(0x99,0x33,0x00),	XT_IDS_CLR_BROWN			},
	{	RGB(0x33,0x33,0x00),	XT_IDS_CLR_OLIVE_GREEN		},
	{	RGB(0x00,0x33,0x00),	XT_IDS_CLR_DARK_GREEN		},
	{	RGB(0x00,0x33,0x66),	XT_IDS_CLR_DARK_TEAL		},
	{	RGB(0x00,0x00,0x80),	XT_IDS_CLR_DARK_BLUE		},
	{	RGB(0x33,0x33,0x99),	XT_IDS_CLR_INDIGO			},
	{	RGB(0x33,0x33,0x33),	XT_IDS_CLR_GRAY80			},
	{	RGB(0x80,0x00,0x00),	XT_IDS_CLR_DARK_RED 		},
	{	RGB(0xFF,0x66,0x00),	XT_IDS_CLR_ORANGE			},
	{	RGB(0x80,0x80,0x00),	XT_IDS_CLR_DARK_YELLOW		},
	{	RGB(0x00,0x80,0x00),	XT_IDS_CLR_GREEN			},
	{	RGB(0x00,0x80,0x80),	XT_IDS_CLR_TEAL 			},
	{	RGB(0x00,0x00,0xFF),	XT_IDS_CLR_BLUE 			},
	{	RGB(0x66,0x66,0x99),	XT_IDS_CLR_BLUEGRAY 		},
	{	RGB(0x80,0x80,0x80),	XT_IDS_CLR_GRAY50			},
	{	RGB(0xFF,0x00,0x00),	XT_IDS_CLR_RED				},
	{	RGB(0xFF,0x99,0x00),	XT_IDS_CLR_LIGHTORANGE		},
	{	RGB(0x99,0xCC,0x00),	XT_IDS_CLR_LIME 			},
	{	RGB(0x33,0x99,0x66),	XT_IDS_CLR_SEA_GREEN		},
	{	RGB(0x33,0xCC,0xCC),	XT_IDS_CLR_AQUA 			},
	{	RGB(0x33,0x66,0xFF),	XT_IDS_CLR_LIGHT_BLUE		},
	{	RGB(0x80,0x00,0x80),	XT_IDS_CLR_VIOLET			},
	{	RGB(0x99,0x99,0x99),	XT_IDS_CLR_GRAY40			},
	{	RGB(0xFF,0x00,0xFF),	XT_IDS_CLR_PINK 			},
	{	RGB(0xFF,0xCC,0x00),	XT_IDS_CLR_GOLD 			},
	{	RGB(0xFF,0xFF,0x00),	XT_IDS_CLR_YELLOW			},
	{	RGB(0x00,0xFF,0x00),	XT_IDS_CLR_BRIGHT_GREEN 	},
	{	RGB(0x00,0xFF,0xFF),	XT_IDS_CLR_TURQUOISE		},
	{	RGB(0x00,0xCC,0xFF),	XT_IDS_CLR_SKY_BLUE 		},
	{	RGB(0x99,0x33,0x66),	XT_IDS_CLR_PLUM 			},
	{	RGB(0xC0,0xC0,0xC0),	XT_IDS_CLR_GRAY25			},
	{	RGB(0xFF,0x99,0xCC),	XT_IDS_CLR_ROSE 			},
	{	RGB(0xFF,0xCC,0x99),	XT_IDS_CLR_TAN				},
	{	RGB(0xFF,0xFF,0x99),	XT_IDS_CLR_LIGHT_YELLOW 	},
	{	RGB(0xCC,0xFF,0xCC),	XT_IDS_CLR_LIGHT_GREEN		},
	{	RGB(0xCC,0xFF,0xFF),	XT_IDS_CLR_LIGHT_TURQUOISE	},
	{	RGB(0x99,0xCC,0xFF),	XT_IDS_CLR_PALE_BLUE		},
	{	RGB(0xCC,0x99,0xFF),	XT_IDS_CLR_LAVENDER 		},
	{	RGB(0xFF,0xFF,0xFF),	XT_IDS_CLR_WHITE			},
};

#define COLOR_POPUP_CX		  158
#define COLOR_POPUP_CY		  130

#define COLOR_SELECTOR_CX		  (18 * 8)
#define COLOR_SELECTOR_CY		  110


/////////////////////////////////////////////////////////////////////////////
// CXTColorSelectorCtrl message handlers

BOOL CXTColorSelectorCtrl::Create(CRect rect, CWnd* pParentWnd, DWORD dwPopup, COLORREF clrColor, 
						   COLORREF clrDefault/*=CLR_DEFAULT*/) 
{
	m_pParentWnd = pParentWnd;
	ASSERT(m_pParentWnd != NULL); // must be valid.
	AddListener(m_pParentWnd->m_hWnd);
	
	m_dwPopup	 = dwPopup;
	m_clrColor	 = clrColor;
	m_clrDefault = clrDefault;
	
	m_rcWnd.CopyRect(rect);
	
	m_rcWnd.right	  = m_rcWnd.left + COLOR_SELECTOR_CX + m_rcBorders.left + m_rcBorders.right;
	m_rcWnd.top 	  = m_rcWnd.bottom;
	m_rcWnd.bottom	  = m_rcWnd.top  + COLOR_SELECTOR_CY + m_rcBorders.top + m_rcBorders.bottom;
	
	if (m_dwPopup & CPS_XT_EXTENDED) {
		m_nLastIndex = _countof(extendedColors);
		m_rcWnd.bottom += 34;
	}
	else {
		m_nLastIndex = _countof(standardColors);
	}
	
	m_nRows = m_nLastIndex/m_nCols;
	m_nLastIndex++;
	
	// register the wnd class.
	CString wndClass = ::AfxRegisterWndClass(0, ::LoadCursor(0, IDC_ARROW));
	
	if ((m_dwPopup & CPS_XT_MORECOLORS)==0) {
		m_rcWnd.bottom -= 28;
	}
	// TSF NEU 18.01.02 - user defined colors
	else
	{
		if( m_arUserDefColors.GetSize() > 0 )
		{
			m_rcWnd.bottom += 8 + m_sizeButton.cy;
			m_nLastIndex += m_arUserDefColors.GetSize();
		}
	}
	// TSF NEU 18.01.02 - user defined colors
	
	// call the base class for creation.
	if( !CWnd::CreateEx( NULL, wndClass, _T(""), dwPopup & WS_POPUP? WS_POPUP: WS_CHILD,
		m_rcWnd, m_pParentWnd, 0, NULL ))
	{
		TRACE0( "Failed to create color popup window.\n" );
		return FALSE;
	}
	ShowWindow(SW_SHOWNA);
	
	// Set the font and control ID.
	SetFont(&xtAfxData.font);
	
	return TRUE;	
}


int CXTColorSelectorCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state
	
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	int nIndex	  = -1;
	bool bChecked = false;
	
	// Create the ToolTip control.
	m_tooltip.Create(this );
	m_tooltip.Activate(TRUE);	
	
	CString strText;
	if (m_dwPopup & CPS_XT_NOFILL) {
		strText.LoadString(XT_IDS_CLR_NOFILL);
	}
	else {
		strText.LoadString(XT_IDS_AUTOMATIC);
	}
	
	int l = m_rcBorders.left;
	int t = m_rcBorders.top;
	int r = lpCreateStruct->cx - m_rcBorders.right;
	int b = t + 23;
	
	int nResID = 0;
	
	if (m_dwPopup & CPS_XT_NOFILL) {
		nResID = XT_IDS_CLR_NOFILL_TIP;
	}
	else {
		nResID = XT_IDS_AUTOMATIC_TIP;
	}
	
	nIndex++;
	
	// Create the automatic color button.
	XT_COLOR_CELL* pColorCell = new XT_COLOR_CELL;
	_tcscpy(pColorCell->szText, strText);
	
	pColorCell->nID 		= nResID;
	pColorCell->rect.left	= l;
	pColorCell->rect.top	= t;
	pColorCell->rect.right	= r;
	pColorCell->rect.bottom = b;
	pColorCell->nIndex		= nIndex;
	pColorCell->bChecked	= false;
	pColorCell->dwStyle 	= m_dwPopup;
	pColorCell->clr 		= m_clrDefault;
	
	m_arCells.AddTail(pColorCell);
	m_tooltip.AddTool(this, nResID, pColorCell->rect, 1);
	
	if (!(m_dwPopup & CPS_XT_NOFILL))
	{
		if (m_clrColor == m_clrDefault)
		{
			bChecked = true;
			pColorCell->bChecked = true;
		}
	}
	
	// Create the color pick buttons.
	XT_PICK_BUTTON* pickBtn = (m_dwPopup & CPS_XT_EXTENDED) ? 
        extendedColors : standardColors;
	
	t += ((m_dwPopup & CPS_XT_EXTENDED) ? 27 : 46);
	CRect rect(l, t, l+m_sizeButton.cx, t+m_sizeButton.cy);
	int x = -1;

	int nRow;
	for ( nRow = 0; nRow < m_nRows; ++nRow )
	{
		int nCol;
		for ( nCol = 0; nCol < m_nCols; ++nCol )
		{
			nIndex++;
			++x;
			
			nResID = pickBtn[x].toolTipID;
			COLORREF clrColor = pickBtn[x].clrButton;
			
			pColorCell = new XT_COLOR_CELL;
			pColorCell->szText[0]	= 0;
			pColorCell->nID 		= nResID;
			pColorCell->rect.left	= rect.left;
			pColorCell->rect.top	= rect.top;
			pColorCell->rect.right	= rect.right;
			pColorCell->rect.bottom = rect.bottom;
			pColorCell->nIndex		= nIndex;
			pColorCell->bChecked	= false;
			pColorCell->dwStyle 	= CPS_XT_PICKBOX;
			pColorCell->clr 		= clrColor;
			
			m_arCells.AddTail(pColorCell);
			m_tooltip.AddTool(this, nResID, pColorCell->rect, 1);
			
			if ((m_clrColor == clrColor) && !bChecked)
			{
				bChecked = true;
				pColorCell->bChecked = true;
				m_nCurSel = pColorCell->nIndex;
			}
			// offset to next column.
			rect.OffsetRect(m_sizeButton.cx,0);
		}
		
		// offset to next row.
		rect.OffsetRect(0,m_sizeButton.cy);
		// offset to next column.
		rect.OffsetRect(-(m_sizeButton.cx*m_nCols),0);
	}
	
	if (m_dwPopup & CPS_XT_MORECOLORS)
	{
		// TSF NEU 18.01.02 - user defined colors
		if( m_arUserDefColors.GetSize() > 0 )
		{
			rect.top	+= 4;
			rect.bottom += 4;

			int nCol;
			for ( nCol = 0; nCol < m_arUserDefColors.GetSize(); ++nCol )
			{
				nIndex++;
				++x;
				
				COLORREF clrColor = m_arUserDefColors.GetAt( nCol );
				
				pColorCell = new XT_COLOR_CELL;
				pColorCell->szText[0]	= 0;
				pColorCell->nID 		= 0;
				pColorCell->rect.left	= rect.left;
				pColorCell->rect.top	= rect.top;
				pColorCell->rect.right	= rect.right;
				pColorCell->rect.bottom = rect.bottom;
				pColorCell->nIndex		= nIndex;
				pColorCell->bChecked	= false;
				pColorCell->dwStyle 	= CPS_XT_PICKBOX;
				pColorCell->clr 		= clrColor;
				
				m_arCells.AddTail(pColorCell);
				m_tooltip.AddTool(this, XT_IDS_MORE_COLORS_TIP, pColorCell->rect, 1);
				
				if (m_clrColor == clrColor)
				{
					bChecked = TRUE;
					pColorCell->bChecked = true;
					if( !bChecked )
						m_nCurSel = pColorCell->nIndex;
				}
				// offset to next column.
				rect.OffsetRect(m_sizeButton.cx,0);
			}
			
			// offset to next row.
			rect.OffsetRect(0,m_sizeButton.cy);
			// offset to next column.
			rect.OffsetRect(-(m_sizeButton.cx*m_nCols),0);	 
			
			
			t += rect.Height() + 4;
		}
		// TSF NEU 18.01.02 - user defined colors
		
		t += ((m_dwPopup & CPS_XT_EXTENDED) ? 94 : 44);
		b  = t + ((m_dwPopup & CPS_XT_EXTENDED) ? 23 : 22);
		
		strText.LoadString(XT_IDS_MORE_COLORS);
		
		nIndex++;
		pColorCell = new XT_COLOR_CELL;
		_tcscpy(pColorCell->szText, strText);
		
		pColorCell->nID 		= XT_IDS_MORE_COLORS_TIP;
		pColorCell->rect.left	= l;
		pColorCell->rect.top	= t;
		pColorCell->rect.right	= r;
		pColorCell->rect.bottom = b;
		pColorCell->nIndex		= nIndex;
		pColorCell->bChecked	= false;
		pColorCell->dwStyle 	= CPS_XT_DEFAULT;
		pColorCell->clr 		= m_clrColor;
		
		m_arCells.AddTail(pColorCell);
		m_tooltip.AddTool(this, XT_IDS_MORE_COLORS_TIP, pColorCell->rect, 1);
		
		if (m_clrColor != -1)
		{
			if (bChecked == false)
			{
				bChecked = true;
				pColorCell->bChecked = true;
			}
		}
	}	

	m_nCurSel = -1;
	
	return 0;
	
}


////////////////////////////////////////////////////////////////////////////////
// CXTColorSelectorCtrl Drawing

void CXTColorSelectorCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	CXTMemDC memDC(&dc, rc);
	
	COLORREF clrBackground = (xtAfxData.bXPMode) ?
            xtAfxData.clrXPMenuTextBack : xtAfxData.clr3DFace;
	
	if (GetStyle() & WS_CHILD && (GetParent()->SendMessage(WM_CTLCOLORSTATIC, (WPARAM)memDC.m_hDC, (LPARAM)m_hWnd) != 0))
	{	
		clrBackground = memDC.GetBkColor();		
	} 
	memDC.FillSolidRect(&rc, clrBackground);

		
	DrawColorSelector(&memDC);
}


void CXTColorSelectorCtrl::DrawColorSelector(CDC *pDC)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state
	
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&xtAfxData.font);
	
	if ((m_dwPopup & CPS_XT_EXTENDED) == 0)
	{
		CString strText;
		strText.LoadString(XT_IDS_STD_COLORS);
		
		pDC->TextOut(6, 31, strText);
	}


	POSITION pos;
	int nIndex = 0;
	for ( pos = m_arCells.GetHeadPosition(); pos; m_arCells.GetNext(pos))
	{
		XT_COLOR_CELL* pColorCell = m_arCells.GetAt(pos);
		ASSERT(pColorCell!= NULL);
		
		BOOL bHighlighted = (((nIndex == m_nCurSel) && (m_nPressed == -1)) || (nIndex == m_nPressed));
		BOOL bPressed = ((nIndex == m_nPressed) && (nIndex == m_nCurSel));

		DrawColorCell(pColorCell, pDC, bHighlighted, bPressed);
		nIndex++;
	}
	
	pDC->SelectObject(pOldFont);	
}

void CXTColorSelectorCtrl::DrawColorCell(XT_COLOR_CELL* pColorCell, CDC* pDC, BOOL bHilite, BOOL bPressed)
{
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&xtAfxData.font);
	
	CRect rcItem = pColorCell->rect;
	
	bool bChecked = pColorCell->bChecked;
	
	if (!xtAfxData.bXPMode)
	{
		if (bChecked || bHilite || bPressed)
		{
			if (bChecked && !bHilite && !bPressed)
			{		
				_xtAfxDrawShadedRect(pDC, rcItem);
			}

			COLORREF clrTopLeft = bPressed || bChecked? xtAfxData.clr3DShadow: xtAfxData.clr3DHilight;
			COLORREF clrBottomRight = bPressed || bChecked? xtAfxData.clr3DHilight: xtAfxData.clr3DShadow;
			
			pDC->Draw3dRect(rcItem, clrTopLeft, clrBottomRight);
		}
	} else
	{
		if (bChecked || bHilite || bPressed)
		{
			if (bPressed) pDC->FillSolidRect(rcItem, xtAfxData.clrXPHighlightPushed);
			else if (bHilite) pDC->FillSolidRect(rcItem, xtAfxData.clrXPHighlight);
			else if (bChecked) pDC->FillSolidRect(rcItem, xtAfxData.clrXPHighlightChecked);
			

			pDC->Draw3dRect(rcItem, xtAfxData.clrHighlight, xtAfxData.clrHighlight);

		}
	}

	
	if (_tcslen(pColorCell->szText))
		pDC->DrawText(pColorCell->szText, rcItem, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	
	if ((pColorCell->dwStyle & CPS_XT_DEFAULT) == 0)
	{
		rcItem.DeflateRect(3,3);
		
		if (!xtAfxData.bXPMode || 0 != pColorCell->nIndex)
			pDC->Draw3dRect(rcItem, xtAfxData.clr3DShadow, xtAfxData.clr3DShadow);
		
		if ((pColorCell->dwStyle & CPS_XT_PICKBOX) == 0)
		{
			if ((pColorCell->dwStyle & CPS_XT_NOFILL) == 0)
			{
				rcItem.DeflateRect(3,3);
				rcItem.right  = rcItem.left + 12;
				rcItem.bottom = rcItem.top	+ 11;
				rcItem.OffsetRect(1,0);
				pDC->Draw3dRect(rcItem, xtAfxData.clr3DShadow, xtAfxData.clr3DShadow);
				rcItem.DeflateRect(1,1);
				pDC->FillSolidRect(rcItem, pColorCell->clr);
			}
		}
		else
		{
			rcItem.DeflateRect(1,1);
			pDC->FillSolidRect(rcItem, pColorCell->clr);
		}
	}
	else
	{
		if (pColorCell->bChecked)
		{
			rcItem = pColorCell->rect;
			rcItem.DeflateRect(5,6);
			rcItem.left = rcItem.right - 12;
			pDC->Draw3dRect(rcItem, xtAfxData.clr3DShadow, xtAfxData.clr3DShadow);
			rcItem.DeflateRect(1,1);
			pDC->FillSolidRect(rcItem, pColorCell->clr);
		}
	}
	
	pDC->SelectObject(pOldFont);	
}

////////////////////////////////////////////////////////////////////////////////

XT_COLOR_CELL* CXTColorSelectorCtrl::GetCellFromIndex(int iIndex)
{
	POSITION pos = m_arCells.FindIndex(iIndex);
	if (pos != NULL)
		return m_arCells.GetAt(pos);
	
	return NULL;
}


void CXTColorSelectorCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!IsColorDlgVisible())
	{
		if (m_point != point)
		{
			m_point = point;
			
			POSITION pos;
			for ( pos = m_arCells.GetHeadPosition(); pos; m_arCells.GetNext(pos))
			{
				XT_COLOR_CELL* pColorCell = m_arCells.GetAt(pos);
				ASSERT(pColorCell!= NULL);
				
				CRect rcItem = pColorCell->rect;
				if (rcItem.PtInRect(point))
				{
					SelectColorCell(pColorCell);
					TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd};	
					_TrackMouseEvent(&tme);
					
					return;
				}
			}
			
			SelectColorCell(NULL);
		}
		
		CWnd::OnMouseMove(nFlags, point);
	}
}

void CXTColorSelectorCtrl::OnMouseLeave()
{
	if (!IsColorDlgVisible())
	{
		m_nPressed = m_nCurSel = -1;
		Invalidate(FALSE);
	}
}

void CXTColorSelectorCtrl::SelectColorCell(XT_COLOR_CELL* pColorCell)
{

	m_nCurSel = pColorCell == NULL? -1: pColorCell->nIndex;
	Invalidate(FALSE);
}

// Adds a window to send color picker notifications to.
void CXTColorSelectorCtrl::AddListener(HWND hwndListener)
{
	m_listeners.Add(hwndListener);
}

void CXTColorSelectorCtrl::RemoveListener(HWND hwndListener)
{
	int i;
	for (i = m_listeners.GetSize() - 1; i >= 0 ; i--)
	{	
		if (m_listeners[i] == hwndListener)
			m_listeners.RemoveAt(i);
	}
}


void CXTColorSelectorCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (!IsColorDlgVisible())
	{
		CWnd::OnLButtonDown(nFlags, point);

		CRect rc;
		GetClientRect(&rc);
		
		if (rc.PtInRect(point))
		{
			m_nPressed = m_nCurSel;
			Invalidate(FALSE);
		}
	}
}

void CXTColorSelectorCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (!IsColorDlgVisible())
	{
		CWnd::OnLButtonUp(nFlags, point);
		
		if (m_nPressed == m_nCurSel &&  m_nCurSel != -1) 
		{
			EndSelection(m_nCurSel);
		}

		m_nCurSel = m_nPressed = -1;
		Invalidate(FALSE);
	}
}

void CXTColorSelectorCtrl::SetCurSel(int nIndex)
{
	ASSERT((nIndex >= 0) && (nIndex <= m_nLastIndex));
	
	POSITION pos = m_arCells.FindIndex(nIndex);
	if (pos == NULL)
		return;
	
	XT_COLOR_CELL* pColorCell = m_arCells.GetAt(pos);
	SelectColorCell(pColorCell);
}

void CXTColorSelectorCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TSF ÄNDERUNG 19.04.02
	if (!IsColorDlgVisible())
	{
		int rows = m_nRows + (m_arUserDefColors.GetSize() + m_nCols - 1)/m_nCols;

		//		    0
		//
		//	1  1  2  3  4  5  6  7  8
		//	2  9 10 11 12 13 14 15 16 
		//	3 17 18 19 20 21 22 23 24 
		//  4 25 26 27 28 29 30 31 32 
		//
		//         33
		//
		//		    0
		//
		//	1  1  2  3  4  5  6  7  8
		//	2  9 10 11 12 13 14 15 16 
		//	3 17 18 19 20 21 22 23 24 
		//  4 25 26 27 28 29 30 31 32 
		//  5 33 
		//
		//         34

		switch (nChar)
		{
		case VK_DOWN:
			{
                // Check for no current cursor yet
                if (m_nCurSel == -1) {
                    m_nCurSel = 0 ;
                }    
                // top button to first color cell
                else if (m_nCurSel == 0) {
                         m_nCurSel = 1 ;
				}
                // bottom button to top bottom
                else if (m_nCurSel == m_nLastIndex) {
                    m_nCurSel = 0 ;
                }    
                // a color square to the next color square below it
				else if (( m_nCurSel + m_nCols) < m_nLastIndex) {
					m_nCurSel += m_nCols;
				}
                // a color square to the last button because there is no color square below it   
				else {
					m_nCurSel = m_nLastIndex;
				}
				SetCurSel(m_nCurSel);
				break;
			}
			
		case VK_UP:
			{
                // Check for no current cursor yet
                if (m_nCurSel == -1) {
                    m_nCurSel = m_nLastIndex ;
                }    
                // top button to last button
                else if (m_nCurSel == 0) {
                         m_nCurSel = m_nLastIndex ;
                }    
                // bottom button to first color square in the last row
                else if (m_nCurSel == m_nLastIndex) { 
                    m_nCurSel = m_nCols * (rows - 1) + 1;
                }    
                // a color square to another color square just above it
				else if (( m_nCurSel - m_nCols) > 0) {
					m_nCurSel -= m_nCols;
				}
                // a color square to the top button because there is no color square above this one
				else {
					m_nCurSel = 0;
				}
				SetCurSel(m_nCurSel);
				break;
			}
			
		case VK_RIGHT:
			{
                // Check for no current cursor yet
                if (m_nCurSel == -1) {
                    m_nCurSel = 0 ;
                }    
				else {
                    ++m_nCurSel;
				    if (m_nCurSel > m_nLastIndex) {
					    m_nCurSel = 0;
				    }
                }    
				SetCurSel(m_nCurSel);
				break;
			}
			
		case VK_LEFT:
			{
                // Check for no current cursor yet
                if (m_nCurSel == -1) {
                    m_nCurSel = m_nLastIndex ;
                }
                else {    
				    --m_nCurSel;
				    if (m_nCurSel < 0) {
					    m_nCurSel = m_nLastIndex;
				    }
                }    
				SetCurSel(m_nCurSel);
				break;
			}
			
		case VK_ESCAPE:
			{
				EndSelection(-1);
				return;
			}
		case VK_RETURN:
		case VK_SPACE:
			{
				EndSelection(m_nCurSel);
				return;
			}
		}

		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

BOOL CXTColorSelectorCtrl::PreTranslateMessage(MSG* pMsg) 
{
	m_tooltip.RelayEvent(pMsg);
	
	return CWnd::PreTranslateMessage(pMsg);
}

int CXTColorSelectorCtrl::GetCurSel(XT_COLOR_CELL* pColorCell)
{
	pColorCell = GetCellFromIndex(m_nCurSel);
	ASSERT(pColorCell != NULL);
	return m_nCurSel;
}

void CXTColorSelectorCtrl::EndSelection(int nCurSel)
{
	_EndSelection(nCurSel, 0);
}

void CXTColorSelectorCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	if (IsColorDlgVisible())
	{
		m_pColorDlg->SetFocus();
		pOldWnd = this;
	}
	CWnd::OnSetFocus(pOldWnd);
}

void CXTColorSelectorCtrl::_EndSelection(int nCurSel, LPARAM callerParam)
{
	XT_MANAGE_STATE;  // Switch to toolkit resources/module state
	
	int nMessage = 0;
	
	// copy off the listeners
	CArray<HWND, HWND> listeners;
	listeners.Append(m_listeners);
	
	// copy other relevant data
	COLORREF clrColor = m_clrColor;
	DWORD dwPopup = m_dwPopup;
	
	if (nCurSel == m_nLastIndex)
	{
		// display dialog and send proper notifications
		m_pColorDlg = new CXTColorDialog(clrColor, clrColor,
			(dwPopup & (CPS_XT_SHOWHEXVALUE|CPS_XT_SHOW3DSELECTION)), GetParent());
		
		if (m_pColorDlg->DoModal() == IDOK)
		{
			COLORREF clrDlgColor = m_pColorDlg->GetColor();
			
			if (clrColor == clrDlgColor) {
				nMessage = CPN_XT_SELCHANGE;
			}
			else {
				nMessage = CPN_XT_SELENDOK;
				clrColor = clrDlgColor;
			}
			
			// TSF NEU 18.01.02 - user defined colors
			if( m_arUserDefColors.GetSize() >= m_nCols )
				m_arUserDefColors.RemoveAt( 0 );

			m_arUserDefColors.Add( clrColor );
			// TSF NEU 18.01.02 - user defined colors
		}
		else {
			nMessage = CPN_XT_SELENDCANCEL;
		}
		
		SAFE_DELETE(m_pColorDlg);
	}
	
	else if (nCurSel == 0)
	{
		if (m_dwPopup & CPS_XT_NOFILL)
		{
			nMessage = CPN_XT_SELNOFILL;
			clrColor = (ULONG)-1;
		}
		else if (m_clrColor == m_clrDefault) {
			nMessage = CPN_XT_SELCHANGE;
		}
		else {
			nMessage = CPN_XT_SELENDOK;
			clrColor = m_clrDefault;
		}
	}
	
	else if (nCurSel == -1)
	{
		nMessage = CPN_XT_SELENDCANCEL;
	}
	
	else
	{
		XT_COLOR_CELL* pColorCell = GetCellFromIndex(nCurSel);
		ASSERT(pColorCell != NULL);
		
		COLORREF clrCellColor = pColorCell->clr;
		
		if (clrColor == clrCellColor) {
			nMessage = CPN_XT_SELCHANGE;
		}
		else {
			nMessage = CPN_XT_SELENDOK;
			clrColor = clrCellColor;
		}
	}
	

	// Pass this message to the registered listeners
	int i;
	for (i = 0; i < listeners.GetSize(); i++)
	{
		if (::IsWindow(listeners[i]))
		{
			::SendMessage(listeners[i], nMessage, 
				(WPARAM)clrColor, callerParam);
		}
	} 
}

BOOL CXTColorSelectorCtrl::OnEraseBkgnd(CDC* ) 
{
	return TRUE;
}

void CXTColorSelectorCtrl::SelectColor(COLORREF clr)
{
	POSITION pos;
	for ( pos = m_arCells.GetHeadPosition(); pos; m_arCells.GetNext(pos))
	{
		XT_COLOR_CELL* pColorCell = m_arCells.GetAt(pos);
		ASSERT(pColorCell!= NULL);
		
		if (pColorCell->dwStyle & CPS_XT_PICKBOX)		
			pColorCell->bChecked = clr == pColorCell->clr;
	}
	
	m_clrColor = clr;
	
	Invalidate(FALSE);
}


INT_PTR CXTColorSelectorCtrl::OnNcHitTest(CPoint point)
{
	// If we have a valid tooltip pointer, bring the tooltip control
	// to the top of the Z-order.
	
	if (m_tooltip.GetSafeHwnd() && !IsColorDlgVisible())
	{
		m_tooltip.SetWindowPos(&wndTop,0, 0, 0, 0,
			SWP_NOACTIVATE | SWP_NOSIZE |SWP_NOMOVE);
	}
	
	return CWnd::OnNcHitTest(point);
}

bool CXTColorSelectorCtrl::IsColorDlgVisible()
{
	return (m_pColorDlg && ::IsWindow(m_pColorDlg->m_hWnd));
}
