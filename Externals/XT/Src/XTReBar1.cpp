// XTReBar1.cpp : implementation of the CXTReBar class - customization support
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
#include "XTFunctions.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTReBar.h"
#include "XTReBarCtrl.h"
#include "XTRegistryManager.h"
#include "XTCustomizeAPI.h"
#include "XTRebarBandInfo.h"
#include "XTToolBar.h"
#include "XTCustomToolBar.h"

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

typedef CXTCustomizeConnection::PLACEBAR PLACEBAR;
typedef CXTCustomizeConnection::ONBARDESTROYED ONBARDESTROYED;
typedef CXTCustomizeConnection::CREATEBAR CREATEBAR;

int CXTReBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CXTControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->PlaceBar.Set(this, &CXTReBar::PlaceNewBar);
	connection->OnBarDestroyed.Add(this, &CXTReBar::OnBarDestroyed);
	connection->CreateBar.Set(this, &CXTReBar::CreateBar);
	
	return 0;
}

void CXTReBar::PlaceNewBar(CFrameWnd* /*pFrame*/, CControlBar* pBar)
{
	XT_REBARBANDINFO info(RBBIM_ID | RBBIM_STYLE);
	info.wID = pBar->GetDlgCtrlID();
	info.fStyle = RBBS_BREAK;
	AddBar(pBar, &info);
}

void CXTReBar::OnBarDestroyed(CControlBar* pBar)
{
	CXTReBarCtrl& rebarCtrl = GetReBarCtrl();

	// find the band, remove it
	UINT uBand;
	for (uBand = 0; uBand < rebarCtrl.GetBandCount(); ++uBand)
	{
		XT_REBARBANDINFO rbbi(RBBIM_ID | RBBIM_CHILD);
		rebarCtrl.GetBandInfo(uBand, &rbbi);
		if (rbbi.hwndChild == pBar->m_hWnd)
		{
			rebarCtrl.DeleteBand(uBand);
			break;
		}
	}
}

void CXTReBar::SaveCustomBars(LPCTSTR pszSection)
{
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->SaveDynamicBars(GetParentFrame(), pszSection);
}

// helper class to delegate creation of the newly added toolbar
class CXTCreateBarFactory : public CObject
{
	CXTReBar* const m_pThis;
	CXTCustomizeConnection* const m_connection;
	CREATEBAR OldCreateBar;
public:
	CXTCreateBarFactory(CXTReBar* pThis)
	: m_connection(CXTCustomizeConnection::GetInstance())
	, m_pThis(pThis)
	{
		OldCreateBar = m_connection->CreateBar;
		m_connection->CreateBar.Set(this, &CXTCreateBarFactory::CreateBar);
	}

	~CXTCreateBarFactory()
	{
		m_connection->CreateBar = OldCreateBar;
	}

	void CreateBar(CXTNewCustomBarRequest* pRequest)
	{
		OldCreateBar(pRequest);
		if (pRequest->m_pNewBar)
		{
			// add this bar to rebar, 
			m_pThis->PlaceNewBar(m_pThis->GetParentFrame(), pRequest->m_pNewBar);		
		}
	}
};

void CXTReBar::LoadCustomBars(LPCTSTR pszSection)
{	
	CXTCreateBarFactory factory(this);
	CXTCustomizeConnection* connection = CXTCustomizeConnection::GetInstance();
	connection->LoadDynamicBars(GetParentFrame(), pszSection);
}

void CXTReBar::CreateBar(CXTNewCustomBarRequest* pRequest)
{
	CXTCustomToolBar* pToolBar = new CXTCustomToolBar(); 
	if (!pToolBar->CreateEx(pRequest->m_pFrame,
							TBSTYLE_FLAT | TBSTYLE_LIST,
							WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
							CRect(0,0,0,0), 
							pRequest->m_nBarID))
	{
		TRACE0("Failed to create toolbar\n");
		AfxThrowResourceException();
	}
	pToolBar->SetCustomBar();
	pToolBar->SetWindowText(pRequest->m_strTitle);
	ASSERT(pRequest->m_pFrame->m_listControlBars.Find(pToolBar) != 0);
	pRequest->m_pNewBar = pToolBar;
}

