// XTResizeRect.cpp: implementation of the CXTResizeRect class.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//--------------------------------------------------------------------
// Permission granted, based on the resizable classes created by Torben B. Haagh.
// http://www.codeguru.com/dialog/torbenResizeDialog.shtml
//--------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResizeRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXTResizeRect::CXTResizeRect()
{
}

//////////////////////////////////////////////////////////////////////

CXTResizeRect::CXTResizeRect(XT_RESIZE l, XT_RESIZE t, XT_RESIZE r, XT_RESIZE b)
{
	left   = l;
	top    = t;
	right  = r;
	bottom = b;
}

CXTResizeRect& CXTResizeRect::operator = (const RECT& rc) 
{ 
	left   = (XT_RESIZE) rc.left; 
	top    = (XT_RESIZE) rc.top; 
	right  = (XT_RESIZE) rc.right; 
	bottom = (XT_RESIZE) rc.bottom; 

	return *this;
}

CXTResizeRect& CXTResizeRect::operator = (const XT_RESIZERECT& rrc) 
{ 
	left   = rrc.left; 
	top    = rrc.top; 
	right  = rrc.right; 
	bottom = rrc.bottom; 

	return *this;
}

CXTResizeRect& CXTResizeRect::operator += (const XT_RESIZERECT& rrc) 
{ 
	left   += rrc.left; 
	top    += rrc.top; 
	right  += rrc.right; 
	bottom += rrc.bottom; 

	return *this;
}

CXTResizeRect CXTResizeRect::operator & (const XT_RESIZERECT& rrc)
{ 
	ASSERT(IsNormalized() && ((CXTResizeRect&) rrc).IsNormalized());

	CXTResizeRect rrcI(
        __max(left,   rrc.left),
        __max(top,    rrc.top), 
		__min(right,  rrc.right),
        __min(bottom, rrc.bottom)); 
	
	// only intersection if resulting rect is normalized
	return (rrcI.IsNormalized()) ? rrcI : CXTResizeRect(0,0,0,0);
}

