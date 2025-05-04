// XTDialogBar.cpp : implementation of the CXTDialogBar class.
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
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"
#include "XTDialogBar.h"

#ifndef _AFX_NO_OCC_SUPPORT
class COccManager : public CNoTrackObject
{
public:
	virtual BOOL OnEvent(CCmdTarget* pCmdTarget, UINT idCtrl, AFX_EVENT* pEvent, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual COleControlContainer* CreateContainer(CWnd* pWnd);
	virtual COleControlSite* CreateSite(COleControlContainer* pCtrlCont);
	virtual const DLGTEMPLATE* PreCreateDialog(_AFX_OCC_DIALOG_INFO* pOccDialogInfo, const DLGTEMPLATE* pOrigTemplate);
	virtual void PostCreateDialog(_AFX_OCC_DIALOG_INFO* pOccDialogInfo);
	virtual DLGTEMPLATE* SplitDialogTemplate(const DLGTEMPLATE* pTemplate, DLGITEMTEMPLATE** ppOleDlgItems);
	virtual BOOL CreateDlgControls(CWnd* pWndParent, LPCTSTR lpszResourceName, _AFX_OCC_DIALOG_INFO* pOccDialogInfo);
	virtual BOOL CreateDlgControls(CWnd* pWndParent, void* lpResource, _AFX_OCC_DIALOG_INFO* pOccDialogInfo);
	virtual BOOL IsDialogMessage(CWnd* pWndDlg, LPMSG lpMsg);
	static BOOL AFX_CDECL IsLabelControl(CWnd* pWnd);
	static BOOL AFX_CDECL IsMatchingMnemonic(CWnd* pWnd, LPMSG lpMsg);
	static void AFX_CDECL SetDefaultButton(CWnd* pWnd, BOOL bDefault);
	static DWORD AFX_CDECL GetDefBtnCode(CWnd* pWnd);
protected:
	HWND CreateDlgControl(CWnd* pWndParent, HWND hwAfter, BOOL bDialogEx, LPDLGITEMTEMPLATE pDlgItem, WORD nMsg, BYTE* lpData, DWORD cb);
	void BindControls(CWnd* pWndParent);
	static void AFX_CDECL UIActivateControl(CWnd* pWndNewFocus);
	static void AFX_CDECL UIDeactivateIfNecessary(CWnd* pWndOldFocus, CWnd* pWndNewFocus);
};
#endif //!_AFX_NO_OCC_SUPPORT

#ifdef AFX_CORE3_SEG
#pragma code_seg(AFX_CORE3_SEG)
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW


/////////////////////////////////////////////////////////////////////////////
// CXTDialogBar
/////////////////////////////////////////////////////////////////////////////
CXTDialogBar::CXTDialogBar()
{
#ifndef _AFX_NO_OCC_SUPPORT
	m_lpszTemplateName = NULL;
	m_pOccDialogInfo = NULL;
#endif

	m_pParentWnd = NULL;
}

CXTDialogBar::~CXTDialogBar()
{
	DestroyWindow();    // avoid PostNcDestroy problems
}

const TCHAR _xtAfxWndControlBar[] = AFX_WNDCONTROLBAR;

// diagnostic routine to check for and decode dialog templates
// return FALSE if a program error occurs (i.e. bad resource ID or
//   bad dialog styles).

BOOL AFXAPI _xtAfxCheckDialogTemplate(LPCTSTR lpszResource, BOOL bInvisibleChild)
{
	ASSERT(lpszResource != NULL);
	HINSTANCE hInst = AfxFindResourceHandle(lpszResource, RT_DIALOG);
	HRSRC hResource = ::FindResource(hInst, lpszResource, RT_DIALOG);
	if (hResource == NULL)
	{
		if (HIWORD(lpszResource) != 0)
			TRACE1("ERROR: Cannot find dialog template named '%s'.\n",
			lpszResource);
		else
			TRACE1("ERROR: Cannot find dialog template with IDD 0x%04X.\n",
			LOWORD((DWORD)lpszResource));
		return FALSE;
	}
	
	if (!bInvisibleChild)
		return TRUE;        // that's all we need to check
	
	// we must check that the dialog template is for an invisible child
	//  window that can be used for a form-view or dialog-bar
	HGLOBAL hTemplate = LoadResource(hInst, hResource);
	if (hTemplate == NULL)
	{
		TRACE0("Warning: LoadResource failed for dialog template.\n");
		// this is only a warning, the real call to CreateDialog will fail
		return TRUE;        // not a program error - just out of memory
	}
	DLGTEMPLATEEX* pTemplate = (DLGTEMPLATEEX*)LockResource(hTemplate);
	DWORD dwStyle;
	if (pTemplate->signature == 0xFFFF)
		dwStyle = pTemplate->style;
	else
		dwStyle = ((DLGTEMPLATE*)pTemplate)->style;
	UnlockResource(hTemplate);
	FreeResource(hTemplate);
	
	if (dwStyle & WS_VISIBLE)
	{
		if (HIWORD(lpszResource) != 0)
			TRACE1("ERROR: Dialog named '%s' must be invisible.\n",
			lpszResource);
		else
			TRACE1("ERROR: Dialog with IDD 0x%04X must be invisible.\n",
			LOWORD((DWORD)lpszResource));
		return FALSE;
	}
	if (!(dwStyle & WS_CHILD))
	{
		if (HIWORD(lpszResource) != 0)
			TRACE1("ERROR: Dialog named '%s' must have the child style.\n",
			lpszResource);
		else
			TRACE1("ERROR: Dialog with IDD 0x%04X must have the child style.\n",
			LOWORD((DWORD)lpszResource));
		return FALSE;
	}
	
	return TRUE;
}

BOOL CXTDialogBar::Create(CWnd* pParentWnd, LPCTSTR lpszTemplateName,
						  UINT nStyle, UINT nID)
{
	ASSERT(pParentWnd != NULL);
	ASSERT(lpszTemplateName != NULL);
	
	m_pParentWnd = pParentWnd;

#ifdef _DEBUG
	// dialog template must exist and be invisible with WS_CHILD set
	if (!_xtAfxCheckDialogTemplate(lpszTemplateName, TRUE))
	{
		ASSERT(FALSE);          // invalid dialog template name
		PostNcDestroy();        // cleanup if Create fails too soon
		return FALSE;
	}
#endif //_DEBUG
	
	// allow chance to modify styles
	m_dwStyle = nStyle;
	CREATESTRUCT cs;
	memset(&cs, 0, sizeof(cs));
	cs.lpszClass = _xtAfxWndControlBar;
	cs.style = (DWORD)nStyle | WS_CHILD;
	cs.hMenu = (HMENU)nID;
	cs.hInstance = AfxGetInstanceHandle();
	cs.hwndParent = pParentWnd->GetSafeHwnd();
	if (!PreCreateWindow(cs))
		return FALSE;
	
	// create a modeless dialog
	
#ifndef _AFX_NO_OCC_SUPPORT
	m_lpszTemplateName = lpszTemplateName;
#endif
	
	// initialize common controls
	VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
	
	BOOL bSuccess = CreateDlg(lpszTemplateName, pParentWnd);
	
#ifndef _AFX_NO_OCC_SUPPORT
	m_lpszTemplateName = NULL;
#endif
	
	if (!bSuccess)
		return FALSE;
	
	// dialog template MUST specify that the dialog
	//  is an invisible child window
	SetDlgCtrlID(nID);
	CRect rect;
	GetWindowRect(&rect);
	m_sizeDefault = rect.Size();    // set fixed size
	
	// force WS_CLIPSIBLINGS
	ModifyStyle(0, WS_CLIPSIBLINGS);
	
	if (!ExecuteDlgInit(lpszTemplateName))
		return FALSE;
	
	// force the size to zero - resizing bar will occur later
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
	
	return TRUE;
}

CSize CXTDialogBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	if (bStretch) // if not docked stretch to fit
		return CSize(bHorz ? 32767 : m_sizeDefault.cx,
		bHorz ? m_sizeDefault.cy : 32767);
	else
		return m_sizeDefault;
}

void CXTDialogBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

BEGIN_MESSAGE_MAP(CXTDialogBar, CXTControlBar)
	//{{AFX_MSG_MAP(CXTDialogBar)
#ifndef _AFX_NO_OCC_SUPPORT
	ON_MESSAGE(WM_INITDIALOG, HandleInitDialog)
#endif //!_AFX_NO_OCC_SUPPORT
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CXTDialogBar::OnEraseBkgnd(CDC* pDC) 
{
	CPoint pt(0,0);
	MapWindowPoints(m_pParentWnd, &pt, 1);
	pt = pDC->OffsetWindowOrg(pt.x, pt.y);
	LRESULT lResult = m_pParentWnd->SendMessage(WM_ERASEBKGND,
		(WPARAM)pDC->m_hDC, 0L);
	pDC->SetWindowOrg(pt.x, pt.y);
	return lResult;
}

LRESULT CXTDialogBar::HandleInitDialog(WPARAM, LPARAM)
{
#ifndef _AFX_NO_OCC_SUPPORT
	Default();  // allow default to initialize first (common dialogs/etc)
	
	// create OLE controls
	COccManager* pOccManager = afxOccManager;
	if ((pOccManager != NULL) && (m_pOccDialogInfo != NULL))
	{
		if (!pOccManager->CreateDlgControls(this, m_lpszTemplateName,
			m_pOccDialogInfo))
		{
			TRACE0("Warning: CreateDlgControls failed during dialog bar init.\n");
			return FALSE;
		}
	}
#endif //!_AFX_NO_OCC_SUPPORT
	return TRUE;
}

BOOL CXTDialogBar::SetOccDialogInfo(_AFX_OCC_DIALOG_INFO* pOccDialogInfo)
{
#ifndef _AFX_NO_OCC_SUPPORT
	m_pOccDialogInfo = pOccDialogInfo;
#endif //!_AFX_NO_OCC_SUPPORT
	return TRUE;
}


#ifdef AFX_INIT_SEG
#pragma code_seg(AFX_INIT_SEG)
#endif

IMPLEMENT_DYNAMIC(CXTDialogBar, CXTControlBar)
