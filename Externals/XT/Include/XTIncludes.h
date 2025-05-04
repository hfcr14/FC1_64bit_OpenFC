// XTIncludes.h : header file
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

#if !defined(__XTINCLUDES_H__)
#define __XTINCLUDES_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//--------------
// MFC includes:
//--------------

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>
#endif // _AFX_NO_OLE_SUPPORT

#ifdef _MFC_OVERRIDES_NEW
#define _INC_MALLOC
#endif
// MFC support for docking windows
#include <afxpriv.h>		
// MFC template classes
#include <afxtempl.h>		
// MFC ListView / TreeView support
#include <afxcview.h>		

#if _MSC_VER > 1200 //MFC 7.0
// MFC Global data
#include <..\src\mfc\afximpl.h>	
#else
// MFC Global data
#include <..\src\afximpl.h>	    
#endif

// MFC support for shell extensions
#include <shlobj.h>			

//////////////////////////////////////////////////////////////////////
// Summary: NOTE: If using the Xtreme Toolkit as a static library linked to an application
//			that is dynamically linked with MFC, you will need to do the following:
//
//			Open the XTToolkit_Lib project workspace, select one of the 
//			Win32 Dynamic build settings, and build the library. Add the following lines 
//			of code to your stdafx.h file:
//
//			<pre>#define _XT_STATICLINK
//			#include <XTToolkit.h></pre>
//
//			Add the following line of code to your *.rc2 file after the comment:
//			"Add manually edited resources here...":
//
//			<pre>#include "XTResource.rc"</pre>
#if !defined( _AFXDLL ) || defined( _XT_STATICLINK )
#define _XT_EXT_CLASS
#else
#define _XT_EXT_CLASS	__declspec( dllimport )
#endif // !defined( _AFXDLL ) || defined( _XT_STATICLINK )

//-----------------------------------------
// Xtreme global and resource definitions:
//-----------------------------------------

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTResource.h"
#include "Common.Include\\XTFunctions.h"
#include "XTVersion.h"
#include "Common.Include\\XTMemDC.h"
#include "Common.Include\\XTAnimationMemDC.h"

//-------------------------
// Xtreme control classes:
//-------------------------

#include "Common.Include\\XTButton.h"
#include "Common.Include\\XTWndHook.h"
#include "XTCoolMenu.h"
#include "Common.Include\\XTHexEdit.h"
#include "Common.Include\\XTFlatComboBox.h"
#include "Common.Include\\XTMaskEdit.h"
#include "Common.Include\\XTBrowseEdit.h"
#include "Common.Include\\XTCaption.h"
#include "Common.Include\\XTCaptionPopupWnd.h"
#include "Common.Include\\XTColorRef.h"
#include "Common.Include\\XTColorSelectorCtrl.h"
#include "Common.Include\\XTColorPopup.h"
#include "Common.Include\\XTColorPicker.h"
#include "Common.Include\\XTColorPageCustom.h"
#include "Common.Include\\XTColorPageStandard.h"
#include "Common.Include\\XTColorDialog.h"
#include "Common.Include\\XTEditListBox.h"
#include "Common.Include\\XTFontCombo.h"
#include "Common.Include\\XTComboBoxEx.h"
#include "Common.Include\\XTTabCtrl.h"
#include "Common.Include\\XTFlatTabCtrl.h"
#include "Common.Include\\XTTreeCtrl.h"
#include "Common.Include\\XTTreeView.h"
#include "Common.Include\\XTFlatHeaderCtrl.h"
#include "Common.Include\\XTListCtrl.h"
#include "Common.Include\\XTOutBarCtrl.h"
#include "Common.Include\\XTOutlookBar.h"
#include "Common.Include\\XTPagerCtrl.h"
#include "Common.Include\\XTDateTimeCtrl.h"
#include "Common.Include\\XTHyperLink.h"
#include "Common.Include\\XTTipWindow.h"
#include "Common.Include\\XTTipOfTheDay.h"
#include "Common.Include\\XTSplitterWnd.h"
#include "Common.Include\\XTStatusBar.h"
#include "Common.Include\\XTLogoPane.h"
#include "Common.Include\\XTMDIWndTab.h"
#include "Common.Include\\XTOSVersionInfo.h"

//------------------------------------------------------------------------------------
// Xtreme resizing dialog controls
//------------------------------------------------------------------------------------

#include "Common.Include\\XTResizeRect.h"
#include "Common.Include\\XTResizePoint.h"
#include "Common.Include\\XTResize.h"
#include "Common.Include\\XTResizeDialog.h"
#include "Common.Include\\XTResizeFormView.h"
#include "Common.Include\\XTResizePropertyPage.h"
#include "Common.Include\\XTResizePropertySheet.h"
#include "Common.Include\\XTSearchOptionsCtrl.h"
#include "XTCBarDialog.h"

//------------------------------------------------------------------------------------
// Xtreme manager controls
//------------------------------------------------------------------------------------

#include "XTIconMap.h"
#include "XTToolsManager.h"
#include "XTOptionsManager.h"
#include "XTAccelManager.h"
#include "XTAccelKeyEdit.h"

//-----------------------------------------------
// Xtreme control bars -
// Replaces: CControlBar, CDockBar, CDockContext:
//-----------------------------------------------

#include "XTDelegate.h"
#include "XTCustomizeAPI.h"
#include "XTCallbacks.h"
#include "XTControlBar.h"
#include "XTDialogBar.h"
#include "XTSplitterDock.h"
#include "XTDockBar.h"
#include "XTDockContext.h"
#include "XTDockWindow.h"
#include "Common.Include\\XTTabCtrlBar.h"
#include "XTDockColorSelector.h"

//------------------------------------------------------
// Xtreme toolbars - 
// Replaces: CToolBar, CToolBarCtrl, CReBar, CReBarCtrl:
//------------------------------------------------------

#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTMenuBar.h"
#include "XTReBar.h"
#include "XTReBarCtrl.h"
#include "XTToolBarPopupWnd.h"
#include "XTPopupTearOffWnd.h"

//------------------------------------------------------------------------------------
// Xtreme frame windows -
// Replaces: CFrameWnd, CMDIFrameWnd, CMDIChildWnd, COleIPFrameWnd, CMiniDockFrameWnd:
//------------------------------------------------------------------------------------

#include "XTFrameImpl.h"
#include "XTFrameWnd.h"
#include "XTMDIFrameWnd.h"
#include "XTMDIChildWnd.h"

#ifndef _AFX_NO_OLE_SUPPORT
#include "XTOleIPFrameWnd.h"
#endif // _AFX_NO_OLE_SUPPORT

#include "XTMiniDockFrameWnd.h"

//-----------------------
// Xtreme customization:
//-----------------------

#include "Common.Include\\XTCheckListBox.h"
#include "XTCommandsListBox.h"
#include "XTNewToolbarDlg.h"
#include "XTCustomizePage.h"
#include "XTCustomizeSheet.h"

//-----------------------
// Xtreme CView classes:
//-----------------------

#include "Common.Include\\XTHtmlView.h"
#include "Common.Include\\XTListView.h"
#include "XTPreviewView.h"
#include "Common.Include\\XTTabView.h"

//--------------------------
// Xtreme shell extensions:
//--------------------------

#include "Common.Include\\XTDropSource.h"
#include "Common.Include\\XTShellPidl.h"
#include "Common.Include\\XTShellSettings.h"
#include "Common.Include\\XTShellTreeView.h"
#include "Common.Include\\XTShellTreeCtrl.h"
#include "Common.Include\\XTDirWatcher.h"
#include "Common.Include\\XTShellListView.h"
#include "Common.Include\\XTShellListCtrl.h"
#include "Common.Include\\XTTrayIcon.h"
#include "Common.Include\\XTBrowseDialog.h"

//----------------------------
// Xtreme utility classes:
//----------------------------

#include "Common.Include\\XTWindowPos.h"
#include "Common.Include\\XTMemFile.h"
#include "Common.Include\\XTSortClass.h"
#include "Common.Include\\XTRegistryManager.h"

/////////////////////////////////////////////////////////////////////////////

#endif // #if !defined(__XTINCLUDES_H__)
