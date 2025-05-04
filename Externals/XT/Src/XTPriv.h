// XTPriv.h interface for the CXTMenuBar class.
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

#ifndef __XTPRIV_H
#define __XTPRIV_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Summary: Macro for using the CXTManageState class - see class for more details
#if defined(_XT_STATICLINK) || defined(_XT_DYN_STATICLIB) || defined(_XT_NO_MANAGED_STATE)
#define XT_MANAGE_STATE
#define XT_MANAGE_STATE_BEGIN
#define XT_MANAGE_STATE_END
#define XT_UNDO_STATE 
#define XT_REDO_STATE 
#else
#define XT_MANAGE_STATE CXTManageState xtManageState;
#define XT_MANAGE_STATE_BEGIN {CXTManageState xtManageState;
#define XT_MANAGE_STATE_END	  }
#define XT_UNDO_STATE xtManageState.Undo();
#define XT_REDO_STATE xtManageState.Redo();
#endif // _XT_STATICLINK

//////////////////////////////////////////////////////////////////////
// Summary: CXTManageState is used internally by the toolkit to switch module
//          states.  Specifically, the class enables the toolkit to read its
//          own .dll resources, and not that of the host application (or .dll).
//          Future versions of the class may handle other such details
class CXTManageState 
{
	friend class CXTDialogState;

public:
	
	// Summary: Constructs a CXTManageState object 
	CXTManageState();

	// Summary: Destroys a CXTManageState object, handles clean up and de-allocation 
	~CXTManageState();

	// Summary: Call this member function to UNDO managed state 
	void Undo();

	// Summary: Call this member function to REDO managed state 
	void Redo();

	// Input:	hModInst - Handle to modules resource.
	//			hModRes - Handle to modules instance.
	// Summary:	Call this member function to set the resource and instance handles
    //          for the module.
	static void SetModuleInfo(HMODULE hModRes,HMODULE hModInst);

protected:
	
	bool			m_bSet;				// true to set
	HMODULE			m_hModOldResource;	// Handle to previous resource.
	HMODULE			m_hModOldInstance;	// Handle to previous instance.
	static HMODULE	m_hModXTResource;	// Handle to toolkit resource.
	static HMODULE	m_hModXTInstance;	// Handle to toolkit instance.
};


#endif // __XTPRIV_H