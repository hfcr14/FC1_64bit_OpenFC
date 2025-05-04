// XTLoadSysColorBitmap.cpp : implementation of the XT_AUX_DATA struct.
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
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTHelpers.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//	DIBs use RGBQUAD format:
//		0xbb 0xgg 0xrr 0x00
//
//	Reasonably efficient code to convert a COLORREF into an
//	RGBQUAD is byte-order-dependent, so we need different
//	code depending on the byte order we're targeting.

#define RGB_TO_RGBQUAD(r,g,b)   (RGB(b,g,r))
#define RGBQUAD_INIT(r,g,b)   (RGB_TO_RGBQUAD(r,g,b)), (RGB(r, g, b))

#define CLR_TO_RGBQUAD(clr)     (RGB(GetBValue(clr), GetGValue(clr), GetRValue(clr)))
/////////////////////////////////////////////////////////////////////////////
// XT_COLORMAP
/////////////////////////////////////////////////////////////////////////////
struct XT_COLORMAP
{
	DWORD rgbqFrom;
    COLORREF clrFrom;
	int iSysColorTo;
};

static const XT_COLORMAP _xtAfxSysColorMap[] =
{
	// mapping from color in DIB to system color
	{ RGBQUAD_INIT(0x00, 0x00, 0x00),  COLOR_BTNTEXT },       // black
	{ RGBQUAD_INIT(0x80, 0x80, 0x80),  COLOR_BTNSHADOW },     // dark gray
	{ RGBQUAD_INIT(0xC0, 0xC0, 0xC0),  COLOR_BTNFACE },       // bright gray
	{ RGBQUAD_INIT(0xFF, 0xFF, 0xFF),  COLOR_BTNHIGHLIGHT }   // white
};

// Helper object to replace color in a given DC
class CXTReplaceColorHandler
{
    CDC m_dcMask; // DC
    CBitmap m_bitmapMask;  // monochrome bitmap to work with
    CBitmap* m_pOldBitmap; // previously selected bitmap 
    bool m_bOldBitmap;  // true if m_pOldBitmap was initialized
    CDC* m_pTargetDC;   // a DC to replace colors in place
    COLORREF m_clrOldText;
    COLORREF m_clrOldBk;
    int m_nWidth;
    int m_nHeight;
public:
    // Constructor takes parameters common for the operation
    CXTReplaceColorHandler(
        // DC in which to replace colors
        CDC* pTargetDC, 

        // width of the rectangle to replace (starting from (0, 0))
        int nWidth, 
        
        // height of the rectangle to replace the color(starting from (0, 0))
        int nHeight)

    : m_pOldBitmap(NULL)
    , m_bOldBitmap(false)
    , m_pTargetDC(pTargetDC)
    , m_clrOldBk(pTargetDC->GetBkColor())
    , m_clrOldText(pTargetDC->GetTextColor())
    , m_nWidth(nWidth)
    , m_nHeight(nHeight)
    {}

    ~CXTReplaceColorHandler()
    {
        if (m_bOldBitmap)
        {
            m_dcMask.SelectObject(m_pOldBitmap);
        }
        m_pTargetDC->SetBkColor(m_clrOldBk);
        m_pTargetDC->SetTextColor(m_clrOldText);    
    }

    // Error-prone initialization, returns false if fails to initialize
    bool Init()
    {
        if (!m_dcMask.CreateCompatibleDC(m_pTargetDC))
        {
            return false;
        }
        if (!m_bitmapMask.CreateBitmap(m_nWidth, m_nHeight, 1, 1, NULL))
        {
            return false;
        }
        m_pOldBitmap = m_dcMask.SelectObject(&m_bitmapMask);
        m_bOldBitmap = true;
        
        return true;
    }

    // Replaces one color with another
    bool ReplaceColor(COLORREF clrFrom, COLORREF clrTo)
    {
        // filter out trivial case
        if (clrFrom == clrTo)
        {
            return true;
        }

        // Initialize the mask:
        // 1. Set the background color of the source DC to the color to translate.
        // so 1s would be the bits where color match has been found
        // 2. Create the object mask for the bitmap by performing a BitBlt
        // from the source bitmap to a monochrome bitmap.
        m_pTargetDC->SetBkColor(clrFrom);
        if (!m_dcMask.BitBlt(0, 0, m_nWidth, m_nHeight, m_pTargetDC, 0, 0, SRCCOPY))
		{
			return false;
		}

        // zero out destination pixels we would want to replace with another color
        m_pTargetDC->SetTextColor(RGB(255, 255, 255));
        m_pTargetDC->SetBkColor(RGB(0,0,0));
        if (!m_pTargetDC->BitBlt(0, 0, m_nWidth, m_nHeight, &m_dcMask, 0, 0, SRCAND))
        {
            return false;
        }

        // now, replace zeroed pixels with destination color
        if (clrTo != RGB(0, 0, 0))
        {
            m_pTargetDC->SetBkColor(clrTo);
            m_pTargetDC->SetTextColor(RGB(0, 0, 0));
            if (!m_pTargetDC->BitBlt(0, 0, m_nWidth, m_nHeight, &m_dcMask, 0, 0, SRCPAINT))
            {
                return false;
            }
        }

        // done
        return true;
    }
};

// A helper to unlock resource 
class CXTUnlockResourceHandler
{
    HGLOBAL m_hglb;
public:
    CXTUnlockResourceHandler(HINSTANCE hInst, HRSRC hRsrc)
    : m_hglb(LoadResource(hInst, hRsrc))
    {}    
    
    ~CXTUnlockResourceHandler()
    {
        if (m_hglb)
        {
            ::FreeResource(m_hglb);
        }
    }

    LPBITMAPINFOHEADER GetBitmapHeader() const
    {
        if (m_hglb)
        {
            return (LPBITMAPINFOHEADER)::LockResource(m_hglb);
        }
        return NULL;
    }

};

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc, BOOL bMono)
/////////////////////////////////////////////////////////////////////////////
_XT_EXT_CLASS HBITMAP AFXAPI _xtAfxLoadSysColorBitmap(HINSTANCE hInst, HRSRC hRsrc, BOOL bMono)
{
    CXTUnlockResourceHandler hglb(hInst, hRsrc);
	LPBITMAPINFOHEADER lpBitmap = hglb.GetBitmapHeader();
	if (lpBitmap == NULL)
		return NULL;

    // figure out what is the size of the palette the bitmap has
	int nColorTableSize;
    if (lpBitmap->biBitCount <= 8)
    {
        nColorTableSize = (1 << lpBitmap->biBitCount);
	}
	else
	{
        // No palette needed for 24 BPP DIB 
        nColorTableSize = 0;        
	}

	// make copy of BITMAPINFOHEADER so we can modify the color table    
	UINT nSize = lpBitmap->biSize + nColorTableSize * sizeof(RGBQUAD);
	LPBITMAPINFOHEADER lpBitmapInfo = (LPBITMAPINFOHEADER)::_alloca(nSize);
	memcpy(lpBitmapInfo, lpBitmap, nSize);

	bool bConverted = false;
	if (nColorTableSize)
	{
		// color table is in RGBQUAD DIB format
		DWORD* pColorTable = (DWORD*)(((LPBYTE)lpBitmapInfo) + (UINT)lpBitmapInfo->biSize);

		int iColor;
		for (iColor = 0; iColor < nColorTableSize; iColor++)
		{
			// look for matching RGBQUAD color in original
			int i;
			for (i = 0; i < _countof(_xtAfxSysColorMap); i++)
			{
				if (pColorTable[iColor] == _xtAfxSysColorMap[i].rgbqFrom)
				{
					if (bMono)
					{
						// all colors except text become white
						if (_xtAfxSysColorMap[i].iSysColorTo != COLOR_BTNTEXT)
							pColorTable[iColor] = RGB_TO_RGBQUAD(255, 255, 255);
					}
					else
						pColorTable[iColor] =
							CLR_TO_RGBQUAD(::GetSysColor(_xtAfxSysColorMap[i].iSysColorTo));
					break;
				}
			}
		}
		bConverted = true;
	}

	int nWidth = (int)lpBitmapInfo->biWidth;
	int nHeight = (int)lpBitmapInfo->biHeight;
	CWindowDC dcScreen(NULL);
	CBitmap bmp;
	if (!bmp.CreateCompatibleBitmap(&dcScreen, nWidth, nHeight))
	{
		return NULL;
	}

	CDC dcGlyphs;
	if (!dcGlyphs.CreateCompatibleDC(&dcScreen))
	{
		return NULL;
	}
	CXTContextObjectHandler<CBitmap> handlerBmp(&dcGlyphs, &bmp);

	// reserve space for color translation table used when doing direct translation
	struct 
	{
		COLORREF clrFrom;
		COLORREF clrTo;
	} 
	colorTranslationTable [_countof(_xtAfxSysColorMap)];
	memset(colorTranslationTable, 0, sizeof(colorTranslationTable));

    if (!bConverted)
    {
		// get translation colors
		const int nTestWidth = (_countof(_xtAfxSysColorMap) % 8 + 1) * 8;
		const UINT nTestDibSize = sizeof(BITMAPINFOHEADER) + nTestWidth * 3;
		LPBITMAPINFOHEADER pTestDIB = (LPBITMAPINFOHEADER)::_alloca(nTestDibSize);
		memset(pTestDIB, 0, nTestDibSize);
		pTestDIB->biSize = sizeof(BITMAPINFOHEADER);
        pTestDIB->biWidth = nTestWidth;
        pTestDIB->biHeight = 1;
        pTestDIB->biPlanes = 1;
        pTestDIB->biBitCount = 24;
        pTestDIB->biCompression = BI_RGB; 
        pTestDIB->biSizeImage = nTestDibSize - sizeof(BITMAPINFOHEADER);
        pTestDIB->biXPelsPerMeter = lpBitmap->biXPelsPerMeter;
        pTestDIB->biYPelsPerMeter = lpBitmap->biYPelsPerMeter;
        pTestDIB->biClrUsed = 0;
        pTestDIB->biClrImportant = 0;
	
		LPBYTE lpTestBits = (LPBYTE)(pTestDIB + 1);
		int i;
		for (i = 0; i < _countof(_xtAfxSysColorMap); i++, lpTestBits+=3)
		{
			colorTranslationTable[i].clrFrom = _xtAfxSysColorMap[i].clrFrom;
            COLORREF clrTo;
            if (bMono)
            {
                // all colors except text become white
                if (_xtAfxSysColorMap[i].iSysColorTo != COLOR_BTNTEXT)
                {
                    clrTo = RGB(255, 255, 255);
                }
                else
                {
                    clrTo = RGB(0, 0, 0);
                }
            }
            else
			{
                clrTo = ::GetSysColor(_xtAfxSysColorMap[i].iSysColorTo);
			}
			colorTranslationTable[i].clrTo = clrTo;
			// RGB QUAD
			lpTestBits[2] = GetRValue(colorTranslationTable[i].clrFrom); 
			lpTestBits[1] = GetGValue(colorTranslationTable[i].clrFrom); 
			lpTestBits[0] = GetBValue(colorTranslationTable[i].clrFrom); 
		}

		// fill in the target bitmap
		if (::StretchDIBits(dcGlyphs, 
							0, 0, nTestWidth, 1, 
							0, 0, nTestWidth, 1,
							(LPBYTE)(pTestDIB + 1), 
							(LPBITMAPINFO)pTestDIB, DIB_RGB_COLORS, SRCCOPY) == GDI_ERROR)
		{
			return NULL;
		}

		for (i = 0; i < _countof(_xtAfxSysColorMap); i++)
		{
			colorTranslationTable[i].clrFrom = dcGlyphs.GetPixel(i, 0); 
		}
	}

	// get the pointer to actual bitmap bits
	LPBYTE lpBits = LPBYTE(lpBitmap + 1) + nColorTableSize * sizeof(RGBQUAD);

	// fill in the target bitmap
	if (::StretchDIBits(dcGlyphs, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight,
		lpBits, (LPBITMAPINFO)lpBitmapInfo, DIB_RGB_COLORS, SRCCOPY) == GDI_ERROR)
	{
		return NULL;
	}

    // the bit map is created now, translate colors hard way if that  could not
    // be done via the palette massaging
    if (!bConverted)
    {		
        CXTReplaceColorHandler replaceHandler(&dcGlyphs, nWidth, nHeight);
        if (!replaceHandler.Init())
        {
            return NULL;
        }

		int i;
		for (i = 0; i < _countof(colorTranslationTable); i++)
	    {
            if (!replaceHandler.ReplaceColor(colorTranslationTable[i].clrFrom, 
						colorTranslationTable[i].clrTo))
            {
                return NULL;
            }

        }
    }
    return (HBITMAP)bmp.Detach();;
}
