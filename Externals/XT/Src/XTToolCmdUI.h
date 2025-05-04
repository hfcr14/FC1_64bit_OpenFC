// XTToolCmdUI.h a helper for handling toolbar button UI updates
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


#if !defined(__XTTOOLCMDUI_H__)
#define __XTTOOLCMDUI_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000



/////////////////////////////////////////////////////////////////////////////
// CXTToolCmdUI
/////////////////////////////////////////////////////////////////////////////

class CXTToolCmdUI : public CCmdUI
{
public:
	virtual void Enable(BOOL bOn);
	virtual void SetCheck(int nCheck);
	virtual void SetText(LPCTSTR lpszText);
};

#endif 
// __XTTOOLCMDUI_H__