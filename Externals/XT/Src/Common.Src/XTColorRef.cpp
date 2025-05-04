// XTColorRef.cpp: implementation of the CXTColorRef class.
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
#include "XTColorRef.h"

#include <math.h>

const CXTColorRef CXTColorRef::BLACK  (0,   0,   0);
const CXTColorRef CXTColorRef::WHITE  (255, 255, 255);
const CXTColorRef CXTColorRef::GRAY_25(64,  64,  64);
const CXTColorRef CXTColorRef::GRAY_50(128, 128, 128);
const CXTColorRef CXTColorRef::GRAY_75(192, 192, 192);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CXTColorRef::toHSL(double& h, double& s, double& l) const
{   
	double r = (double)getRValue()/255;
	double g = (double)getGValue()/255;
	double b = (double)getBValue()/255;
	
	double maxcolor = __max(r, __max(g, b));
	double mincolor = __min(r, __min(g, b));

	l = (maxcolor + mincolor)/2;

	if (maxcolor == mincolor)
	{
		h = 0;
		s = 0;
	}
	else
	{
		if (l < 0.5)
			s = (maxcolor-mincolor)/(maxcolor+mincolor);
		else
			s = (maxcolor-mincolor)/(2.0-maxcolor-mincolor);

		if (r == maxcolor)
			h = (g-b)/(maxcolor-mincolor);
		else if (g == maxcolor)
			h = 2.0+(b-r)/(maxcolor-mincolor);
		else
			h = 4.0+(r-g)/(maxcolor-mincolor);

		h /= 6.0; 
		if (h < 0.0) h += 1;
/*		h *= 60; 
		if(h < 0.0)	h += 360;*/	
	}
}

void CXTColorRef::toHSB(double& h, double& s, double& b) const
{
    int red   = getRValue();
    int green = getGValue();
    int blue  = getBValue();

    int cmax = __max(red, __max(green, blue));
	int cmin = __min(red, __min(green, blue));

	b = ((double) cmax) / 255.0;
	if (cmax != 0)
	    s = ((double) (cmax - cmin)) / ((double) cmax);
	else
	    s = 0;
	if (s == 0)
	    h = 0;
	else 
    {
	    double redc =   ((double) (cmax - red))   / ((double) (cmax - cmin));
	    double greenc = ((double) (cmax - green)) / ((double) (cmax - cmin));
	    double bluec =  ((double) (cmax - blue))  / ((double) (cmax - cmin));
	    if (red == cmax)
    		h = bluec - greenc;
	    else if (green == cmax)
	        h = 2.0 + redc - bluec;
        else
	    	h = 4.0 + greenc - redc;
	    h /= 6.0;
	    if (h < 0)
    		h = h + 1.0;
	}
}

double CXTColorRef::HuetoRGB(double temp1, double temp2, double temp3)
{
	if( temp3 < 0 )
		temp3 = temp3+1.0;
	if( temp3 > 1 )
		temp3 = temp3-1.0;

	if( 6.0*temp3 < 1 )
		return (temp1+(temp2-temp1)*temp3*6.0);

	else if( 2.0*temp3 < 1 )
		return temp2;

	else if( 3.0*temp3 < 2.0 )
		return (temp1+(temp2-temp1)*((2.0/3.0)-temp3)*6.0);

	return temp1;
}

void CXTColorRef::setHSL(double h, double s, double l)
{
	double r,g,b;
	double temp1, temp2;
	
	if (s == 0) {
		r = g = b = l;
	}
	else
	{
		if(l < 0.5)
			temp2 = l*(1.0+s);
		else
			temp2 = l+s-l*s;

		temp1 = 2.0*l-temp2;
//      h /= 360;

		r = HuetoRGB(temp1, temp2, h+1.0/3.0);
		g = HuetoRGB(temp1, temp2, h);
		b = HuetoRGB(temp1, temp2, h-1.0/3.0);
	}

	*this = RGB((BYTE)(r*255),(BYTE)(g*255),(BYTE)(b*255));
}

void CXTColorRef::setHSB(double hue, double saturation, double brightness) 
{
	int r = 0, g = 0, b = 0;
    if (saturation == 0) 
    {
	    r = g = b = (int) (brightness * 255.0 + 0.5);
	} 
    else 
    {
	    double h = (hue - floor(hue)) * 6.0;
	    double f = h - floor(h);
	    double p = brightness * (1.0 - saturation);
	    double q = brightness * (1.0 - saturation * f);
	    double t = brightness * (1.0 - (saturation * (1.0 - f)));
	    switch ((int) h) 
        {
	        case 0:
		        r = (int) (brightness * 255.0 + 0.5);
		        g = (int) (t * 255.0 + 0.5);
		        b = (int) (p * 255.0 + 0.5);
		        break;
	        case 1:
		        r = (int) (q * 255.0 + 0.5);
		        g = (int) (brightness * 255.0 + 0.5);
		        b = (int) (p * 255.0 + 0.5);
		        break;
	        case 2:
		        r = (int) (p * 255.0 + 0.5);
		        g = (int) (brightness * 255.0 + 0.5);
		        b = (int) (t * 255.0 + 0.5);
		        break;
	        case 3:
		        r = (int) (p * 255.0 + 0.5);
		        g = (int) (q * 255.0 + 0.5);
		        b = (int) (brightness * 255.0 + 0.5);
		        break;
	        case 4:
		        r = (int) (t * 255.0 + 0.5);
		        g = (int) (p * 255.0 + 0.5);
		        b = (int) (brightness * 255.0 + 0.5);
		        break;
	        case 5:
		        r = (int) (brightness * 255.0 + 0.5);
		        g = (int) (p * 255.0 + 0.5);
		        b = (int) (q * 255.0 + 0.5);
		        break;
	    }
	}
    *this = RGB(r, g, b);
}

CXTColorRef CXTColorRef::fromHSB(double h, double s, double b)
{
    CXTColorRef cr;
    cr.setHSB(h, s, b);
    return cr;
}

CXTColorRef CXTColorRef::fromHSL(double h, double s, double l)
{
    CXTColorRef cr;
    cr.setHSL(h, s, l);
    return cr;
}

CXTColorRef& CXTColorRef::blend(const CXTColorRef& clr, int opacity)
{
    *this = RGB(BlendColor(clr.getRValue(), getRValue(), opacity),
                BlendColor(clr.getGValue(), getGValue(), opacity),
                BlendColor(clr.getBValue(), getBValue(), opacity));
    return *this;
}

CXTColorRef& CXTColorRef::add(const CXTColorRef& clr)
{
    *this = RGB(addColorValue(clr.getRValue(), getRValue()),
                addColorValue(clr.getGValue(), getGValue()),
                addColorValue(clr.getBValue(), getBValue()));
    return *this;
}

CXTColorRef& CXTColorRef::subtract(const CXTColorRef& clr)
{
    *this = RGB(addColorValue(-clr.getRValue(), getRValue()),
                addColorValue(-clr.getGValue(), getGValue()),
                addColorValue(-clr.getBValue(), getBValue()));
    return *this;
}

COLORREF CXTColorRef::SafeRGB(int r, int g, int b)
{
	return RGB(fixColorValue(r), fixColorValue(g), fixColorValue(b));
}

CXTColorRef CXTColorRef::operator + (int val) const
{
	val = fixColorValue(val);
	return operator + (RGB(val, val, val));
}

CXTColorRef CXTColorRef::operator - (int val) const
{
	val = fixColorValue(val);
	return operator - (RGB(val, val, val));
}

CXTColorRef CXTColorRef::operator * (int val) const
{
	val = fixColorValue(val);
	return operator * (RGB(val, val, val));
}

CXTColorRef CXTColorRef::operator / (int val) const
{
	val = fixColorValue(val);
	return operator / (RGB(val, val, val));
}

CXTColorRef CXTColorRef::operator + (COLORREF cr) const
{
	CXTColorRef temp(*this);
	return (temp += cr);
}

CXTColorRef CXTColorRef::operator - (COLORREF cr) const
{
	CXTColorRef temp(*this);
	return (temp -= cr);
}

CXTColorRef CXTColorRef::operator * (COLORREF cr) const
{
	CXTColorRef temp(*this);
	return (temp *= cr);
}

CXTColorRef CXTColorRef::operator / (COLORREF cr) const
{
	CXTColorRef temp(*this);
	return (temp -= cr);
}

CXTColorRef& CXTColorRef::operator += (int val)
{
	val = fixColorValue(val);
	return operator += (RGB(val, val, val));
}

CXTColorRef& CXTColorRef::operator -= (int val)
{
	val = fixColorValue(val);
	return operator -= (RGB(val, val, val));
}

CXTColorRef& CXTColorRef::operator *= (int val)
{
	val = fixColorValue(val);
	return operator *= (RGB(val, val, val));
}

CXTColorRef& CXTColorRef::operator /= (int val)
{
	val = fixColorValue(val);
	return operator /= (RGB(val, val, val));
}

CXTColorRef& CXTColorRef::operator += (COLORREF cr)
{
	m_ColorRef = SafeRGB(
		getRValue() + GetRValue(cr),
		getGValue() + GetGValue(cr), 
		getBValue() + GetBValue(cr));
	return *this;
}

CXTColorRef& CXTColorRef::operator -= (COLORREF cr)
{
	m_ColorRef = SafeRGB(
		getRValue() - GetRValue(cr),
		getGValue() - GetGValue(cr), 
		getBValue() - GetBValue(cr));
	return *this;
}

CXTColorRef& CXTColorRef::operator *= (COLORREF cr)
{
	m_ColorRef = SafeRGB(
		getRValue() * GetRValue(cr),
		getGValue() * GetGValue(cr), 
		getBValue() * GetBValue(cr));
	return *this;
}

CXTColorRef& CXTColorRef::operator /= (COLORREF cr)
{
	m_ColorRef = SafeRGB(
		getRValue() / GetRValue(cr),
		getGValue() / GetGValue(cr), 
		getBValue() / GetBValue(cr));
	return *this;
}

////  XP Colors Routings

#ifndef _XTP_INCLUDE_CONTROLS

#pragma warning( disable  : 4244 ) 

COLORREF CXTColorRef::m_arrColor[53];

#define clrToolBar GetColor(XPCOLOR_BARFACE)
#define clrWindow GetColor(COLOR_WINDOW)
#define clrHighlight GetColor(COLOR_HIGHLIGHT)
#define pow2(x) pow(x, 2) 
#define NORMVALUE(a, max) a = (a < 0) ? 0 : (a > max) ? max : a


float CXTColorRef::ColorWidth(int nLength, int nWidth)
{
	if (nLength > 220) nLength = 220;
	return (2.0 - (float)nLength / 220) * nWidth;
}


float CXTColorRef::ColorDelta(COLORREF clrA, COLORREF clrB)
{	
	return pow2(GetRDelta(clrA) - GetRDelta(clrB)) + 
		pow2(GetGDelta(clrA) - GetGDelta(clrB)) +
		pow2(GetBDelta(clrA) - GetBDelta(clrB));
}

float CXTColorRef::Length(COLORREF clrA, COLORREF clrB)
{
	return sqrt(ColorDelta(clrA, clrB));
}

BOOL CXTColorRef::LongColor(COLORREF clrMain, COLORREF clrSub, BOOL bCalcLength, float fDistance)
{
	if (bCalcLength)
		fDistance =  ColorWidth(Length(clrMain, 0), fDistance);

	return pow2(fDistance) < ColorDelta(clrMain, clrSub);
}

COLORREF CXTColorRef::MixColor(COLORREF clrMain, COLORREF clrSub, float fDistance)
{
	float fMainLength =  Length(clrMain, 0);

	float fGate = ColorWidth(fMainLength, fDistance);

	BOOL bReverse = TRUE;

	if (Length(clrSub, 0) > fMainLength)
	{
		if (fMainLength > 442 - fGate) 
			bReverse = FALSE;

	}
	else
	{
		if (fMainLength > fGate)  
			bReverse = FALSE;		
	}

	float fSubRed = GetRDelta(clrSub);
	float fSubGreen = GetGDelta(clrSub);
	float fSubBlue= GetBDelta(clrSub);

	float fMainRed = GetRDelta(clrMain);
	float fMainGreen = GetGDelta(clrMain);
	float fMainBlue= GetBDelta(clrMain);

	if (bReverse)
	{
		fSubRed = 195.0 - fSubRed;
		fSubGreen = 390.0 - fSubGreen;
		fSubBlue = 65.0 - fSubBlue;

		fMainRed = 195.0 - fMainRed;
		fMainGreen = 390.0 - fMainGreen;
		fMainBlue = 65.0 - fMainBlue;
	}		 

	float A = __max(0.001, sqrt(pow2(fSubRed) + pow2(fSubGreen) + pow2(fSubBlue)));
	float B = fMainRed * fSubRed + fMainGreen * fSubGreen + fMainBlue * fSubBlue;
	float X = B / pow2(A);

	float fRed = X * fSubRed - fMainRed;
	float fGreen = X * fSubGreen - fMainGreen;
	float fBlue = X * fSubBlue - fMainBlue;

	float Y = sqrt(pow2(fGate) - (pow2(fRed) + pow2(fGreen) + pow2(fBlue)));

	float Z =(B / A - Y);

	fRed = Z * fSubRed / A;
	fGreen= Z * fSubGreen / A;
	fBlue = Z * fSubBlue / A;

	if (bReverse)
	{
		fRed = 195.0 - fRed;
		fGreen = 390.0 - fGreen;
		fBlue = 65.0 - fBlue;
	}

	NORMVALUE(fRed, 195.0);
	NORMVALUE(fGreen, 390.0);
	NORMVALUE(fBlue, 65.0);

	int nRed = .5 + fRed * (255.0/195.0);
	int nGreen = .5 + fGreen * (255.0/390.0);
	int nBlue = .5 + fBlue * (255.0/65.0);

	return RGB(NORMVALUE(nRed, 255), NORMVALUE(nGreen, 255), NORMVALUE(nBlue, 255));
}

COLORREF CXTColorRef::AdjustColor(COLORREF clrMain, COLORREF clrSub, float fDistance)
{
	float Z = sqrt(
		pow2(GetRDelta(clrMain) - GetRDelta(clrSub)) + 
		pow2(GetGDelta(clrMain) - GetGDelta(clrSub)) +
		pow2(GetBDelta(clrMain) - GetBDelta(clrSub)));

	float Q = (Z - ColorWidth(Length(0, clrMain), fDistance)) / Z;

	float fRed = Q * (GetRDelta(clrMain) - GetRDelta(clrSub)) + GetRDelta(clrSub);
	float fGreen = Q * (GetGDelta(clrMain) - GetGDelta(clrSub)) + GetGDelta(clrSub);
	float fBlue = Q * (GetBDelta(clrMain) - GetBDelta(clrSub)) + GetBDelta(clrSub);

	NORMVALUE(fRed, 195.0);
	NORMVALUE(fGreen, 390.0);
	NORMVALUE(fBlue, 65.0);

	int nRed = .5 + fRed * (255.0/195.0);
	int nGreen = .5 + fGreen * (255.0/390.0);
	int nBlue = .5 + fBlue * (255.0/65.0);

	return RGB(NORMVALUE(nRed, 255), NORMVALUE(nGreen, 255), NORMVALUE(nBlue, 255));
}

COLORREF CXTColorRef::LightColor(COLORREF clrLight, COLORREF clrDark, int nDelta)
{
	int nParam = (nDelta < 100 ? 100 : 1000);

	int nRed = (GetRValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetRValue(clrLight) * nDelta) / nParam;
	int nGreen = (GetGValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetGValue(clrLight) * nDelta) / nParam;
	int nBlue = (GetBValue(clrDark) * (nParam - nDelta) + nParam / 2 +  GetBValue(clrLight) * nDelta) / nParam;


	return RGB(nRed, nGreen, nBlue);
}

COLORREF CXTColorRef::GetColor(int nIndex)
{
	return m_arrColor[nIndex];
}

void CXTColorRef::RefreshSysColors()
{
	m_arrColor[0] = 0;
	int i;
	for (i = 1; i < 28; i++) m_arrColor[i] = GetSysColor(i);
}

void CXTColorRef::RefreshXPColors()
{
    m_arrColor[XPCOLOR_BARFACE]                  = GetColor(COLOR_3DFACE);             
    m_arrColor[XPCOLOR_HIGHLIGHT]                = GetColor(COLOR_WINDOW);             
    m_arrColor[XPCOLOR_HIGHLIGHT_PUSHED]         = GetColor(COLOR_HIGHLIGHT);             
    m_arrColor[XPCOLOR_MENUTEXT_BACK]            = GetColor(COLOR_WINDOW);             
    m_arrColor[XPCOLOR_GRAYTEXT]                 = GetColor(COLOR_GRAYTEXT);             
    m_arrColor[XPCOLOR_GRIPPER]                  = GetColor(COLOR_3DSHADOW);             
    m_arrColor[XPCOLOR_SEPARATOR]                = GetColor(COLOR_3DSHADOW);             
    m_arrColor[XPCOLOR_MENU_BORDER]			     = GetColor(COLOR_BTNSHADOW);             
    m_arrColor[XPCOLOR_DISABLED]                 = GetColor(COLOR_3DSHADOW);             
    m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED]        = GetColor(COLOR_WINDOW);             
    m_arrColor[XPCOLOR_HIGHLIGHT_BORDER]         = GetColor(COLOR_HIGHLIGHT);
    m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED_BORDER] = GetColor(COLOR_HIGHLIGHT);
    m_arrColor[XPCOLOR_MENUTEXT]                 = GetColor(COLOR_WINDOWTEXT);
    m_arrColor[XPCOLOR_HIGHLIGHT_TEXT]           = GetColor(COLOR_MENUTEXT); 
    m_arrColor[XPCOLOR_BARTEXT]                  = GetColor(COLOR_BTNTEXT);        
    m_arrColor[XPCOLOR_BARTEXT_PUSHED]           = GetColor(COLOR_HIGHLIGHTTEXT);  
    m_arrColor[XPCOLOR_TAB_INACTIVE_BACK]        = GetColor(COLOR_3DFACE);
    m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT]        = GetColor(COLOR_BTNTEXT);
	m_arrColor[XPCOLOR_MENU_EXPANDED]            = GetColor(COLOR_3DSHADOW);
	m_arrColor[XPCOLOR_3DFACE]                   = GetColor(COLOR_3DFACE);             
	m_arrColor[XPCOLOR_3DSHADOW]                 = GetColor(COLOR_3DSHADOW);             
}

void CXTColorRef::RefreshColors()
{
	struct MUTECOLOR
	{
		int nIndex;
		int clrMain;
		int clrSub;
		int nDistance;
	} ;

	BOOL bSimpleColors = FALSE;

	CDC *pDC = CWnd::GetDesktopWindow()->GetDC();
	if (pDC)
	{
		int nColors = pDC->GetDeviceCaps(BITSPIXEL);
		CWnd::GetDesktopWindow()->ReleaseDC(pDC);
		
		bSimpleColors = (nColors > 0 && nColors <= 8);
	}

	RefreshSysColors();
	RefreshXPColors();

	if (bSimpleColors)
		return;

	const MUTECOLOR IndexTable[] =
	{ 
        { XPCOLOR_BARFACE,          COLOR_WINDOW,    COLOR_3DFACE,    165  }, 
        { XPCOLOR_HIGHLIGHT,        COLOR_HIGHLIGHT, COLOR_WINDOW,    0x1E },
        { XPCOLOR_HIGHLIGHT_PUSHED, COLOR_HIGHLIGHT, COLOR_WINDOW,    0x32 }, 
        { XPCOLOR_MENUTEXT_BACK,    COLOR_3DFACE,    COLOR_WINDOW,    0x8f },
        { XPCOLOR_GRAYTEXT,         COLOR_GRAYTEXT,  COLOR_WINDOW,    0x46 },
        { XPCOLOR_GRIPPER,          COLOR_3DSHADOW,  COLOR_WINDOW,    0x4b },
        { XPCOLOR_SEPARATOR,        COLOR_3DSHADOW,  COLOR_WINDOW,    0x46 },
        { XPCOLOR_MENU_BORDER,      XPCOLOR_BARTEXT, COLOR_BTNSHADOW, 0x14 },
        { XPCOLOR_DISABLED,         COLOR_3DSHADOW,  COLOR_WINDOW,    0x5A },
		{ XPCOLOR_MENU_EXPANDED,    COLOR_3DFACE,    COLOR_3DSHADOW,  90   } 
	};

	int i;
	for (i = 0; i < sizeof(IndexTable) / sizeof(IndexTable[0]); i++)
	{
		m_arrColor[IndexTable[i].nIndex] = LightColor(GetColor(IndexTable[i].clrMain), 
			GetColor(IndexTable[i].clrSub), IndexTable[i].nDistance);

	}

	const MUTECOLOR LongTable[] =
	{
        { XPCOLOR_HIGHLIGHT,                XPCOLOR_BARFACE,          XPCOLOR_HIGHLIGHT,                50  },
        { XPCOLOR_HIGHLIGHT_BORDER,         XPCOLOR_BARFACE,          COLOR_HIGHLIGHT,                  100 },
        { XPCOLOR_HIGHLIGHT_PUSHED,         XPCOLOR_BARFACE,          XPCOLOR_HIGHLIGHT_PUSHED,         30  },
        { XPCOLOR_GRAYTEXT,                 XPCOLOR_MENUTEXT_BACK,    XPCOLOR_GRAYTEXT,                 80  },
        { XPCOLOR_HIGHLIGHT_CHECKED_BORDER, XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_HIGHLIGHT_CHECKED_BORDER, 100 },
        { XPCOLOR_GRIPPER,                  XPCOLOR_BARFACE,          XPCOLOR_GRIPPER,                  85  },
        { XPCOLOR_SEPARATOR,                XPCOLOR_BARFACE,          XPCOLOR_SEPARATOR,                50  },
        { XPCOLOR_MENU_BORDER,              XPCOLOR_BARTEXT_PUSHED,   XPCOLOR_MENU_BORDER,              100 },
        { XPCOLOR_DISABLED,                 XPCOLOR_BARFACE,          XPCOLOR_DISABLED,                 80  },
        { XPCOLOR_MENUTEXT,                 XPCOLOR_MENUTEXT_BACK,    XPCOLOR_MENUTEXT,                 180 },
        { XPCOLOR_HIGHLIGHT_TEXT,           XPCOLOR_HIGHLIGHT,        XPCOLOR_HIGHLIGHT_TEXT,           180 },
        { XPCOLOR_BARTEXT,                  XPCOLOR_BARFACE,          XPCOLOR_BARTEXT,                  180 },
        { XPCOLOR_BARTEXT_PUSHED,           XPCOLOR_HIGHLIGHT_PUSHED, XPCOLOR_BARTEXT_PUSHED,           180 }
	};
	
	if (LongColor(GetColor(COLOR_3DFACE), GetColor(XPCOLOR_BARFACE), 1, 35))
	{
		m_arrColor[XPCOLOR_BARFACE] = AdjustColor(GetColor(COLOR_3DFACE), GetColor(XPCOLOR_BARFACE), 35);
	}

	for (i = 0; i < sizeof(LongTable) / sizeof(LongTable[0]); i++)
	{
		if (! LongColor(GetColor(LongTable[i].clrMain),GetColor(LongTable[i].clrSub) , 1, LongTable[i].nDistance))
		{
			m_arrColor[LongTable[i].nIndex] = MixColor(GetColor(LongTable[i].clrMain),GetColor(LongTable[i].clrSub), LongTable[i].nDistance);
		}		
	}
	
	if (LongColor(GetColor(XPCOLOR_MENUTEXT_BACK), GetColor(XPCOLOR_GRAYTEXT), 1, 145))
	{

		m_arrColor[XPCOLOR_GRAYTEXT] = AdjustColor(GetColor(XPCOLOR_MENUTEXT_BACK), GetColor(XPCOLOR_GRAYTEXT), 145);
	}

	m_arrColor[XPCOLOR_HIGHLIGHT_CHECKED] = 
		RGB( GetRValue(clrWindow) * 40 / 100 + GetRValue(clrHighlight) * 10 / 100 + GetRValue(clrToolBar) * 50 / 100, 
		GetGValue(clrWindow) * 40 / 100 + GetGValue(clrHighlight) * 10 / 100 + GetGValue(clrToolBar) * 50 / 100,
		GetBValue(clrWindow) * 40 / 100 + GetBValue(clrHighlight) * 10 / 100 + GetBValue(clrToolBar) * 50 / 100);


	COLORREF clrBtn = GetColor(COLOR_3DFACE);
	int r =  GetRValue(clrBtn), g = GetGValue(clrBtn), b = GetBValue(clrBtn);
	int nMax = __max(__max(r, g),  b);	
	if (nMax == 0)
		m_arrColor[XPCOLOR_TAB_INACTIVE_BACK] = RGB(35, 35, 35); else
	{
		int nDelta = __min(35, 255 - nMax) + nMax;
		m_arrColor[XPCOLOR_TAB_INACTIVE_BACK] = RGB(r * nDelta / nMax, g * nDelta / nMax, b * nDelta / nMax);
	}

	COLORREF clrShadow = GetColor(COLOR_3DSHADOW);
	r =  GetRValue(clrShadow), g = GetGValue(clrShadow), b = GetBValue(clrShadow );
	nMax = __max(__max(r, g),  b);	
	
	if (clrBtn == 0) m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(0x7f, 0x7f, 0x7f);
	else if (nMax == 0)	m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(0, 0, 0); 
	else
	{
		int nDelta = __max(0, nMax - 43);
		m_arrColor[XPCOLOR_TAB_INACTIVE_TEXT] = RGB(r * nDelta / nMax, g * nDelta / nMax, b * nDelta / nMax);		
	}		

	COLORREF clrBackground = m_arrColor[XPCOLOR_HIGHLIGHT];
	m_arrColor[XPCOLOR_ICONSHADDOW] = RGB( GetRValue(clrBackground) * .75, GetGValue(clrBackground ) * .75, GetBValue(clrBackground) * .75);
}

#else
COLORREF CXTColorRef::GetColor(int nIndex)
{
	return XTPColorManager()->GetColor(nIndex);
}
#endif //_XTP_INCLUDE_CONTROLS
