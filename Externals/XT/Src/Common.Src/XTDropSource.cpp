// XTDropSource.cpp : implementation file
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
/////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "XTDropSource.h"

/////////////////////////////////////////////////////////////////////////////
// CXTDropSource

CXTDropSource::CXTDropSource(void)
{
	m_uiRefCount = 1;
}

CXTDropSource::~CXTDropSource(void)
{
}

/////////////////////////////////////////////////////////////////////////////
// IUnknown Methods

STDMETHODIMP CXTDropSource::QueryInterface(REFIID riid, LPVOID *ppReturn) 
{
	*ppReturn = NULL;

	//IUnknown
	if(IsEqualIID(riid, IID_IUnknown))
	{
		*ppReturn = this;
	}
	//IDropTarget
	else if(IsEqualIID(riid, IID_IDropSource))
	{
		*ppReturn = (IDropSource*)this;
	}
	if(*ppReturn)
	{
		(*(LPUNKNOWN*)ppReturn)->AddRef();
		return S_OK;
	}
	return E_NOINTERFACE;
}


STDMETHODIMP_(ULONG) CXTDropSource::AddRef(VOID)
{
	return ++m_uiRefCount;
}


STDMETHODIMP_(ULONG) CXTDropSource::Release(VOID)
{
	if(--m_uiRefCount == 0)
	{
		delete this;
	}
	return m_uiRefCount;
}  

/////////////////////////////////////////////////////////////////////////////
// IDropSource Methods

STDMETHODIMP CXTDropSource::QueryContinueDrag(BOOL bEsc, DWORD dwKeyState)
{  
	if(bEsc)
	{
		return DRAGDROP_S_CANCEL;
	}

	if ((dwKeyState & MK_LBUTTON) == 0x0000)
	{
		return DRAGDROP_S_DROP;
	}
	return S_OK;
}

STDMETHODIMP CXTDropSource::GiveFeedback(DWORD)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

