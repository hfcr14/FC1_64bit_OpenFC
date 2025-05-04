// XTHelpers.h interface
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

#if !defined(__XTREBARBANDINFO_H__)
#define __XTREBARBANDINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



// just a helper to properly initialize the REBARBANDINFO structure
struct XT_REBARBANDINFO : public REBARBANDINFO
{
	XT_REBARBANDINFO()
	{
		memset(this, 0, sizeof(*this));
		cbSize = sizeof(REBARBANDINFO);
	}

	XT_REBARBANDINFO(UINT fMaskBits)
	{
		memset(this, 0, sizeof(*this));
		cbSize = sizeof(REBARBANDINFO);
		fMask = fMaskBits;
	}

};

#endif __XTREBARBANDINFO_H__