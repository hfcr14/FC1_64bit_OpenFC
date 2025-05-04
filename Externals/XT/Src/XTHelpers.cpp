// XTHelpers.cpp : implementation file
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
#include "XTHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// Draws a line between the two adjacent rects
void DrawAdjacentBorder(CDC* pDC, COLORREF clrBorder, const CRect& rcExclude, const CRect& rcWindow)
{
    /*CRect rcOverlap;
    rcOverlap.left = __max(rc1.left, rc2.left);
    rcOverlap.top = __max(rc1.top, rc2.top);
    rcOverlap.right = __min(rc1.right, rc2.right);
    rcOverlap.bottom = __min(rc1.bottom, rc2.bottom);
    rcOverlap -= rc2.TopLeft();
    // only draw if they overlap on the edge
    if (rcOverlap.top == rcOverlap.bottom && rcOverlap.left + 2 < rcOverlap.right)
    {
        CPen pen(PS_SOLID, 1, clrBorder);
        CXTContextObjectHandler<CPen> penHandler(pDC, &pen);
        pDC->MoveTo(rcOverlap.left + 1, rcOverlap.top);
        pDC->LineTo(rcOverlap.right, rcOverlap.top);
    }
    if (rcOverlap.left == rcOverlap.right && rcOverlap.top + 2 < rcOverlap.bottom)
    {
        CPen pen(PS_SOLID, 1, clrBorder);
        CXTContextObjectHandler<CPen> penHandler(pDC, &pen);
        pDC->MoveTo(rcOverlap.left, rcOverlap.top + 1);
        pDC->LineTo(rcOverlap.left, rcOverlap.bottom);
    }*/
	CRect rcIntersect;
	if (rcIntersect.IntersectRect(rcExclude, rcWindow))
	{
		rcIntersect.OffsetRect(-rcWindow.TopLeft());
		
		if (rcIntersect.Height() == 1)
		{	
			rcIntersect.DeflateRect(1, 0);
			pDC->FillSolidRect(rcIntersect, clrBorder);
		} else
		if (rcIntersect.Width() == 1)
		{	
			rcIntersect.DeflateRect(0, 1);
			pDC->FillSolidRect(rcIntersect, clrBorder);
		}
	}
}

void RenderBits(CDC* pDC, const CRect& rcDest, LPCVOID bits)
{
	CBitmap bitmap;
	bitmap.CreateBitmap(8, // assumes 8 pixel max width
		rcDest.Height(), 1, 1, bits); 

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// first, make black hole where we would copy the bits
	COLORREF clrText = pDC->SetTextColor(RGB(0,0,0));
	COLORREF clrBk = pDC->SetBkColor(RGB(255,255,255));
	pDC->BitBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), &memDC, 0, 0, SRCAND);

	// second, fill the zeroed bits with required color, do not change the rest
	pDC->SetTextColor(clrText);		
	pDC->SetBkColor(RGB(0,0,0));		
	pDC->BitBlt(rcDest.left, rcDest.top, rcDest.Width(), rcDest.Height(), &memDC, 0, 0, SRCPAINT);

	pDC->SetBkColor(clrBk);
	memDC.SelectObject(pOldBitmap);
}

// helper for the CalcPopupRect() function
static CPoint CalcPopupRectOrigin(
				const CRect& rcExclusion,
				const CRect& rcScreen,
				UINT tpmFlags,
				CSize extent,
				bool& bShifted)
{
	bShifted = false;
	CPoint origin;
	if (tpmFlags & TPM_VERTICAL)
	{
		// natural origin
		origin.x = (tpmFlags & TPM_RIGHTALIGN)
					? rcExclusion.right - extent.cx
					: rcExclusion.left;
		origin.y = (tpmFlags & TPM_BOTTOMALIGN)
					? rcExclusion.bottom
					: rcExclusion.top - extent.cy;
		// if the rect goes out of screen, shift horizontally
		if (origin.x + extent.cx >= rcScreen.right)
		{
			bShifted = true;
			origin.x = rcScreen.right - extent.cx;
			if (origin.x < rcExclusion.left - extent.cx)
			{
				origin.x = rcExclusion.left - extent.cx;
			}
		}
		if (origin.x < 0)
		{
			bShifted = true;
			origin.x = 0;
			if (rcExclusion.right + extent.cx < 0)
			{
				origin.x = rcExclusion.right + extent.cx;
			}
		}
	}
	else
	{
		origin.x = (tpmFlags & TPM_RIGHTALIGN)
					? rcExclusion.right
					: rcExclusion.left - extent.cx;
		origin.y = (tpmFlags & TPM_BOTTOMALIGN)
					? rcExclusion.bottom - extent.cx
					: rcExclusion.top;
		// if the rect goes out of screen, shift vertically
		if (origin.y + extent.cy >= rcScreen.bottom)
		{
			bShifted = true;
			origin.y = rcScreen.bottom - extent.cy;
			if (origin.y < rcExclusion.top - extent.cy)
			{
				origin.y = rcExclusion.top - extent.cy;
			}
		}
		if (origin.y < 0)
		{
			bShifted = true;
			origin.y = 0;
			if (rcExclusion.bottom + extent.cy < 0)
			{
				origin.y = rcExclusion.bottom + extent.cy;
			}
		}
	}
	return origin;
}

// Calculates a rectangle that would be completely visible inside screen rectangle
void CalcPopupRect(
	// on exit, gets coordinates of computed rectangle
	CRect* prcTarget,

	// screen coordinates
	const CRect& rcScreen,

	// exclusion rect, generated rectangle shall not overlap it and be adjacent to it
	const CRect& rcExclusion,

	// flags controlling placement of the generated rectangle
	// Recognized values are
	// TPM_LEFTALIGN   align left coordinate of the generated rectangle with the left
	//                 coordinate of exclusion rect
	// TPM_RIGHTALIGN  align right coordinate of the generated rectangle with the right
	//                 coordinate of exclusion rect
	// TPM_TOPALIGN    align bottom coordinate of the generated rectangle with the top
	//                 coordinate of exclusion rect
	// TPM_BOTTOMALIGN align top coordinate of the generated rectangle with the bottom
	//                 coordinate of exclusion rect
	// TPM_HORIZONTAL
	// TPM_VERTICAL
	UINT tpmFlags,

	// extent of the rectangle to generate
	CSize extent)
{
	// check to see if visible as requested
	bool bShifted;
	CPoint originMin = CalcPopupRectOrigin(rcExclusion, rcScreen, tpmFlags, extent, bShifted);
	*prcTarget = CRect(originMin, extent);
	if ((*prcTarget & rcScreen) == *prcTarget)
	{
		return;
	}

	UINT distanceMin = 32767;

	// generate permutations and and their distances to the original
	int i;
	for (i = 0; i < 8; i++)
	{
		static const UINT flags[8] =
		{
			TPM_LEFTALIGN | TPM_TOPALIGN | TPM_VERTICAL,
			TPM_RIGHTALIGN | TPM_TOPALIGN | TPM_VERTICAL,
			TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_VERTICAL,
			TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_VERTICAL,
			TPM_LEFTALIGN | TPM_TOPALIGN | TPM_HORIZONTAL,
			TPM_RIGHTALIGN | TPM_TOPALIGN | TPM_HORIZONTAL,
			TPM_LEFTALIGN | TPM_BOTTOMALIGN | TPM_HORIZONTAL,
			TPM_RIGHTALIGN | TPM_BOTTOMALIGN | TPM_HORIZONTAL,
		};

		CRect rc(CalcPopupRectOrigin(rcExclusion, rcScreen, flags[i], extent, bShifted), extent);
		if ((rc & rcScreen) == rc)
		{
			// evaluate distance
			UINT distance = 0;

			// 4 is the weight for mismatch in orientation
			if ((flags[i] & TPM_VERTICAL) != (tpmFlags & TPM_VERTICAL))
			{
				distance += 8;
			}
			if (bShifted)
			{
				distance += 4;
			}
			UINT nHorzAlignWeight = (tpmFlags & TPM_VERTICAL) ? 1 : 2;
			UINT nVertAlignWeight = (tpmFlags & TPM_VERTICAL) ? 2 : 1;
			if ((flags[i] & TPM_RIGHTALIGN) != (tpmFlags & TPM_RIGHTALIGN))
			{
				distance += nHorzAlignWeight;
			}
			if ((flags[i] & TPM_BOTTOMALIGN) != (tpmFlags & TPM_BOTTOMALIGN))
			{
				distance += nVertAlignWeight;
			}
			if (distance < distanceMin)
			{
				distanceMin = distance;
				*prcTarget = rc;
			}
		}
	}
}

void AdjoinRect(const CRect& rc1, CRect& rc2)
{
	int dx = 0;
	int dy = 0;
	if (rc1.right == rc2.left)
	{
		dx = -1;
	}
	else
	if (rc1.left == rc2.right)
	{
		dx = 1;
	}
	else
	if (rc1.bottom == rc2.top)
	{
		dy = -1;
	}
	else
	if (rc1.top == rc2.bottom)
	{
		dy = 1;
	}
	rc2.OffsetRect(dx, dy);
}
