// XTDockState.cpp : implementation file
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
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTRegistryManager.h"
#include "XTDockState.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const TCHAR _afxVisible[]          = _T("Visible");
static const TCHAR _afxBarSection[]       = _T("%s-Bar%d");
static const TCHAR _afxSummarySection[]   = _T("%s-Summary");
static const TCHAR _afxXPos[]             = _T("XPos");
static const TCHAR _afxYPos[]             = _T("YPos");
static const TCHAR _afxMRUWidth[]         = _T("MRUWidth");
static const TCHAR _afxDocking[]          = _T("Docking");
static const TCHAR _afxMRUDockID[]        = _T("MRUDockID");
static const TCHAR _afxMRUDockLeftPos[]   = _T("MRUDockLeftPos");
static const TCHAR _afxMRUDockRightPos[]  = _T("MRUDockRightPos");
static const TCHAR _afxMRUDockTopPos[]    = _T("MRUDockTopPos");
static const TCHAR _afxMRUDockBottomPos[] = _T("MRUDockBottomPos");
static const TCHAR _afxMRUFloatStyle[]    = _T("MRUFloatStyle");
static const TCHAR _afxMRUFloatXPos[]     = _T("MRUFloatXPos");
static const TCHAR _afxMRUFloatYPos[]     = _T("MRUFloatYPos");
static const TCHAR _afxBarID[]            = _T("BarID");
static const TCHAR _afxHorz[]             = _T("Horz");
static const TCHAR _afxFloating[]         = _T("Floating");
static const TCHAR _afxBars[]             = _T("Bars");
static const TCHAR _afxScreenCX[]         = _T("ScreenCX");
static const TCHAR _afxScreenCY[]         = _T("ScreenCY");
static const TCHAR _afxBar[]              = _T("Bar#%d");

BOOL CXTControlBarInfo::LoadState(LPCTSTR lpszProfileName, int nIndex, CDockState* pDockState)
{
	ASSERT(pDockState != NULL);

	TCHAR szSection[256];
	wsprintf(szSection, _afxBarSection, lpszProfileName, nIndex);

    CXTRegistryManager regManager;
	m_nBarID = regManager.GetProfileInt(szSection, _afxBarID, 0);
	m_bVisible = (BOOL) regManager.GetProfileInt(szSection, _afxVisible, TRUE);
	m_bHorz = (BOOL) regManager.GetProfileInt(szSection, _afxHorz, TRUE);
	m_bFloating = (BOOL) regManager.GetProfileInt(szSection, _afxFloating, FALSE);
	m_pointPos = CPoint(
		regManager.GetProfileInt(szSection, _afxXPos, -1),
		regManager.GetProfileInt(szSection, _afxYPos, -1));
	pDockState->ScalePoint(m_pointPos);
	m_nMRUWidth = regManager.GetProfileInt(szSection, _afxMRUWidth, 32767);
	m_bDocking = regManager.GetProfileInt(szSection, _afxDocking, 0);
	if (m_bDocking)
	{
		m_uMRUDockID = regManager.GetProfileInt(szSection, _afxMRUDockID, 0);

		m_rectMRUDockPos = CRect(
			regManager.GetProfileInt(szSection, _afxMRUDockLeftPos, 0),
			regManager.GetProfileInt(szSection, _afxMRUDockTopPos, 0),
			regManager.GetProfileInt(szSection, _afxMRUDockRightPos, 0),
			regManager.GetProfileInt(szSection, _afxMRUDockBottomPos, 0));
		pDockState->ScaleRectPos(m_rectMRUDockPos);

		m_dwMRUFloatStyle = regManager.GetProfileInt(szSection, _afxMRUFloatStyle, 0);

		m_ptMRUFloatPos = CPoint(
			regManager.GetProfileInt(szSection, _afxMRUFloatXPos, 0),
			regManager.GetProfileInt(szSection, _afxMRUFloatYPos, 0));
		pDockState->ScalePoint(m_ptMRUFloatPos);
	}

	int nBars = regManager.GetProfileInt(szSection, _afxBars, 0);
	int i;
	for (i=0; i < nBars; i++)
	{
		TCHAR buf[16];
		wsprintf(buf, _afxBar, i);
		m_arrBarID.Add(regManager.GetProfileInt(szSection, buf, 0));
	}

	return m_nBarID != 0;
}

BOOL CXTControlBarInfo::SaveState(LPCTSTR lpszProfileName, int nIndex)
{
	TCHAR szSection[256];
	wsprintf(szSection, _afxBarSection, lpszProfileName, nIndex);

	// delete the section
    CXTRegistryManager regManager;
	regManager.WriteProfileString(szSection, NULL, NULL);

	if (m_bDockBar && m_bVisible && !m_bFloating && m_pointPos.x == -1 &&
		m_pointPos.y == -1 && m_arrBarID.GetSize() <= 1)
	{
		return FALSE;
	}

	regManager.WriteProfileInt(szSection, _afxBarID, m_nBarID);
	if (!m_bVisible)
		regManager.WriteProfileInt(szSection, _afxVisible, m_bVisible);
	if (m_bFloating)
	{
		regManager.WriteProfileInt(szSection, _afxHorz, m_bHorz);
		regManager.WriteProfileInt(szSection, _afxFloating, m_bFloating);
	}
	if (m_pointPos.x != -1)
		regManager.WriteProfileInt(szSection, _afxXPos, m_pointPos.x);
	if (m_pointPos.y != -1)
		regManager.WriteProfileInt(szSection, _afxYPos, m_pointPos.y);
	if (m_nMRUWidth != 32767)
		regManager.WriteProfileInt(szSection, _afxMRUWidth, m_nMRUWidth);
	if (m_bDocking)
	{
		regManager.WriteProfileInt(szSection, _afxDocking, m_bDocking);
		regManager.WriteProfileInt(szSection, _afxMRUDockID, m_uMRUDockID);
		regManager.WriteProfileInt(szSection, _afxMRUDockLeftPos, m_rectMRUDockPos.left);
		regManager.WriteProfileInt(szSection, _afxMRUDockTopPos, m_rectMRUDockPos.top);
		regManager.WriteProfileInt(szSection, _afxMRUDockRightPos, m_rectMRUDockPos.right);
		regManager.WriteProfileInt(szSection, _afxMRUDockBottomPos, m_rectMRUDockPos.bottom);
		regManager.WriteProfileInt(szSection, _afxMRUFloatStyle, m_dwMRUFloatStyle);
		regManager.WriteProfileInt(szSection, _afxMRUFloatXPos, m_ptMRUFloatPos.x);
		regManager.WriteProfileInt(szSection, _afxMRUFloatYPos, m_ptMRUFloatPos.y);
	}

	if (m_arrBarID.GetSize() > 1) //if ==1 then still empty
	{
		regManager.WriteProfileInt(szSection, _afxBars, m_arrBarID.GetSize());
		int i;
		for (i = 0; i < m_arrBarID.GetSize(); i++)
		{
			TCHAR buf[16];
			wsprintf(buf, _afxBar, i);
			regManager.WriteProfileInt(szSection, buf, (int)m_arrBarID[i]);
		}
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CXTDockState

IMPLEMENT_SERIAL(CXTDockState, CDockState, 0)

/////////////////////////////////////////////////////////////////////////////
// CXTDockState message handlers

void CXTDockState::LoadState(LPCTSTR lpszProfileName)
{
	TCHAR szSection[256];
	wsprintf(szSection, _afxSummarySection, lpszProfileName);
    CXTRegistryManager regManager;
	int nBars = regManager.GetProfileInt(szSection, _afxBars, 0);

	CSize size;
	size.cx = regManager.GetProfileInt(szSection, _afxScreenCX, 0);
	size.cy = regManager.GetProfileInt(szSection, _afxScreenCY, 0);
	SetScreenSize(size);

	int i;
	for (i = 0; i < nBars; i++)
	{
		CXTControlBarInfo* pInfo = new CXTControlBarInfo;
		m_arrBarInfo.Add(pInfo);
		pInfo->LoadState(lpszProfileName, i, this);
	}
}

void CXTDockState::SaveState(LPCTSTR lpszProfileName)
{
	int nIndex = 0;
	int i;
	for (i = 0;i < m_arrBarInfo.GetSize(); i++)
	{
		CXTControlBarInfo* pInfo = (CXTControlBarInfo*)m_arrBarInfo[i];
		ASSERT(pInfo != NULL);
		if (pInfo->SaveState(lpszProfileName, nIndex))
			nIndex++;
	}

	TCHAR szSection[256];
	wsprintf(szSection, _afxSummarySection, lpszProfileName);
    CXTRegistryManager regManager;
	regManager.WriteProfileInt(szSection, _afxBars, nIndex);

	CSize size = GetScreenSize();
	regManager.WriteProfileInt(szSection, _afxScreenCX, size.cx);
	regManager.WriteProfileInt(szSection, _afxScreenCY, size.cy);
}
