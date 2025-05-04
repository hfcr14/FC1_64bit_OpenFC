// XTAccelManager.cpp : implementation of the CXTAccelManager class.
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
#include "XTIconMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//Constructor
CXTIconMap::CXTIconMap()
{}

CXTIconMap::~CXTIconMap()
{
	RemoveAll();
}
	
void CXTIconMap::RemoveAll()
{
	if (POSITION pos = m_map.GetStartPosition())
	{
		do
		{
			XT_MAPENTRY entry;
			UINT nCmdID;
			m_map.GetNextAssoc(pos, nCmdID, entry);
			DestroyEntry(entry);
		}
		while(pos);
		m_map.RemoveAll();
	}
}

void CXTIconMap::DestroyEntry(XT_MAPENTRY& entry)
{
	if (entry.hIcon)
	{
		::DestroyIcon(entry.hIcon);
		entry.hIcon = 0;
	}
	if (entry.hDisabledIcon)
	{
		::DestroyIcon(entry.hDisabledIcon);
		entry.hDisabledIcon = 0;
	}
	if (entry.hHotIcon)
	{
		::DestroyIcon(entry.hHotIcon);
		entry.hHotIcon = 0;
	}
}

void CXTIconMap::SetAt(UINT nCmdID, HICON hIcon, HICON hDisabledIcon, HICON hHotIcon)
{
	XT_MAPENTRY entry;
	if (m_map.Lookup(nCmdID, entry))
	{
		DestroyEntry(entry);
	}
	
	entry.hIcon         = hIcon;
	entry.hDisabledIcon = hDisabledIcon;
	entry.hHotIcon      = hHotIcon;

	m_map.SetAt(nCmdID, entry);
}
	
BOOL CXTIconMap::Lookup(UINT nCmdID, HICON& hIcon, HICON& hDisabledIcon, HICON& hHotIcon)
{
	XT_MAPENTRY entry;
	if (!m_map.Lookup(nCmdID, entry))
	{
		return false;
	}
	
	hDisabledIcon = entry.hDisabledIcon;
	hIcon         = entry.hIcon;
	hHotIcon      = entry.hHotIcon;

	return true;
};
