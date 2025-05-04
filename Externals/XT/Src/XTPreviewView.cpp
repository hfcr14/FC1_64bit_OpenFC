// XTPreviewView.cpp : implementation of the CXTPreviewView class.
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
#include "XTResource.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTButton.h"
#include "XTDelegate.h"
#include "XTWndHook.h"
#include "XTControlBar.h"
#include "XTToolBar.h"
#include "XTPreviewView.h"
#include "XTPriv.h"
#include "XTImageManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXTPreviewView

CXTPreviewView::CXTPreviewView()
{
}

CXTPreviewView::~CXTPreviewView()
{
}

IMPLEMENT_DYNCREATE(CXTPreviewView, CPreviewView)

BEGIN_MESSAGE_MAP(CXTPreviewView, CPreviewView)
	//{{AFX_MSG_MAP(CXTPreviewView)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(AFX_ID_PREVIEW_NUMPAGE, OnUpdateNumPageChange)
	ON_WM_WINDOWPOSCHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXTPreviewView message handlers

int CXTPreviewView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPreviewView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	ASSERT_VALID (m_pToolBar); // dialog bar must be valid.

	XT_MANAGE_STATE;

	// Create the toolbar that is used in the dialog bar.
	if (!m_wndToolBar.CreateEx( m_pToolBar, TBSTYLE_FLAT | TBSTYLE_LIST, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_HIDE_INPLACE ) ||
		!m_wndToolBar.LoadToolBar( XT_IDR_PREVIEW_TOOLBAR ) )
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Save the index for setting the num page toolbar icons.
	m_i1PageIndex = m_wndToolBar.CommandToIndex( AFX_ID_PREVIEW_NUMPAGE );
	m_i2PageIndex = m_wndToolBar.CommandToIndex( XT_IDC_BLANK );

	// Delete the place holder button.
	m_wndToolBar.DeleteButton( m_i2PageIndex );

	// Set the button text for the print and close toolbar commands.
	CString strText;
	strText.LoadString( AFX_ID_PREVIEW_PRINT );

	CString strSubString;
	AfxExtractSubString( strSubString, strText, 1 );

	m_wndToolBar.SetButtonText( m_wndToolBar.CommandToIndex(
		AFX_ID_PREVIEW_PRINT ), strSubString );

	strText.LoadString( AFX_ID_PREVIEW_CLOSE );
	AfxExtractSubString( strSubString, strText, 1 );

	m_wndToolBar.SetButtonText( m_wndToolBar.CommandToIndex(
		AFX_ID_PREVIEW_CLOSE ), strSubString );

	m_wndToolBar.AutoSizeToolbar();

	return 0;
}

void CXTPreviewView::UpdateNumPageIcon()
{
	// Get the button info for the num pages button.
	TBBUTTON button;
	m_wndToolBar.GetButton( m_i1PageIndex, &button );

	// Get the current display state.
	UINT nPages = m_nZoomState == ZOOM_OUT ? m_nPages : m_nZoomOutPages;
	int iBitmap = nPages == 1? m_i2PageIndex: m_i1PageIndex;

	if (button.iBitmap != iBitmap)
	{
		button.iBitmap = iBitmap;

		HIMAGELIST hList = m_wndToolBar.GetImageList();
		CXTIconHandle hIcon = ImageList_ExtractIcon(0, hList, iBitmap);
		ASSERT(hIcon);

		XTImageManager()->AddIcon(AFX_ID_PREVIEW_NUMPAGE, hIcon, CSize(16, 16), TRUE, TRUE);

		// Update the num pages button image.
		m_wndToolBar.SetButton( m_i1PageIndex, &button );
	}
}

void CXTPreviewView::OnUpdateNumPageChange(CCmdUI* pCmdUI) 
{
	CPreviewView::OnUpdateNumPageChange(pCmdUI);

	// Reload the toolbar
	UpdateNumPageIcon();
}

void CXTPreviewView::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CPreviewView::OnWindowPosChanged(lpwndpos);
	
	// If the toolbar is a window...
	if ( ::IsWindow( m_wndToolBar.m_hWnd ) )
	{
		// Get a pointer to the parent frame.
		CFrameWnd* pFrameWnd = m_pToolBar->GetDockingFrame( );
		ASSERT_VALID( pFrameWnd );

		// Get the size of the client area for the docking frame.
		CRect r;
		pFrameWnd->GetClientRect( &r );

		// Calculate the actual size of the toolbar.
		CSize size = m_wndToolBar.CalcFixedLayout( TRUE, TRUE );
		size.cx = r.Width( );

		// Set the height of the dialog bar equal to the 
		// toolbar's height.
		m_pToolBar->m_sizeDefault.cy = size.cy;

		// Reposition the toolbar.
		m_wndToolBar.MoveWindow( 0,0,size.cx,size.cy );
	}
}

_XT_EXT_CLASS bool _xtAfxShowPrintPreview(CView *pView)
{
	XT_MANAGE_STATE;

	// In derived classes, implement special window handling here
	// Be sure to Unhook Frame Window close if hooked.

	// must not create this on the frame.  Must outlive this function
	CPrintPreviewState* pState = new CPrintPreviewState;

	// DoPrintPreview's return value does not necessarily indicate that
	// Print preview succeeded or failed, but rather what actions are necessary
	// at this point.  If DoPrintPreview returns TRUE, it means that
	// OnEndPrintPreview will be (or has already been) called and the
	// pState structure will be/has been deleted.
	// If DoPrintPreview returns FALSE, it means that OnEndPrintPreview
	// WILL NOT be called and that cleanup, including deleting pState
	// must be done here.
	
	if ( !pView->DoPrintPreview( XT_IDD_PREVIEW_TOOLBAR, pView,
		RUNTIME_CLASS( CXTPreviewView ), pState ))
	{
		// In derived classes, reverse special window handling here for
		// Preview failure case
		TRACE0( "Error: DoPrintPreview failed.\n" );
		AfxMessageBox( AFX_IDP_COMMAND_FAILURE );
		delete pState;      // preview failed to initialize, delete State now

		return false;
	}

	return true;
}
