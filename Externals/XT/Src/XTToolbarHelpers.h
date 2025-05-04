// XTToolBarHelpers.h - helpers for the CXTToolBar implementation
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

#if !defined(__XTTOOLBARHELPERS_H__)
#define __XTTOOLBARHELPERS_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Summary: Toolbar private messages
enum TOOLBAR_MESSAGES
{
	// defers layout recalc and UI update when buttons have been collapsed/expanded
	// this message incurs another frame recalc because showing/hiding buttons
	// may affect both dimensions if there are wrapped toolbar buttons
	XTWM_FRESHENAFTER = WM_APP + 462,
	XTWM_POSTRENDER, // defers rendering customize selection/insertion marker
};

#endif //__XTTOOLBARHELPERS_H__
