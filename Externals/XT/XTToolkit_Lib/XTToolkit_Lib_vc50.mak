# Microsoft Developer Studio Generated NMAKE File, Based on XTToolkit_Lib_vc50.dsp
!IF "$(CFG)" == ""
CFG=XTToolkit_Lib - Win32 Dynamic Unicode Debug
!MESSAGE No configuration specified. Defaulting to XTToolkit_Lib - Win32\
 Dynamic Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "XTToolkit_Lib - Win32 Release" && "$(CFG)" !=\
 "XTToolkit_Lib - Win32 Debug" && "$(CFG)" !=\
 "XTToolkit_Lib - Win32 Unicode Release" && "$(CFG)" !=\
 "XTToolkit_Lib - Win32 Unicode Debug" && "$(CFG)" !=\
 "XTToolkit_Lib - Win32 Dynamic Release" && "$(CFG)" !=\
 "XTToolkit_Lib - Win32 Dynamic Debug" && "$(CFG)" !=\
 "XTToolkit_Lib - Win32 Dynamic Unicode Release" && "$(CFG)" !=\
 "XTToolkit_Lib - Win32 Dynamic Unicode Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XTToolkit_Lib_vc50.mak"\
 CFG="XTToolkit_Lib - Win32 Dynamic Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XTToolkit_Lib - Win32 Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Unicode Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Unicode Debug" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Dynamic Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Dynamic Debug" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Dynamic Unicode Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Dynamic Unicode Debug" (based on\
 "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "XTToolkit_Lib - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\Lib_vc50\XT3100LibStatic.lib"

!ELSE 

ALL : "..\Lib_vc50\XT3100LibStatic.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Lib_vc50\XT3100LibStatic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

RSC=rc.exe
CPP=cl.exe
CPP_PROJ=/nologo /MT /W4 /WX /GX /O2 /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK" /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch"\
 /Yu"StdAfx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Lib_vc50.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../Lib_vc50/XT3100LibStatic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XTAccelKeyEdit.obj" \
	"$(INTDIR)\XTAccelManager.obj" \
	"$(INTDIR)\XTAnimationMemDC.obj" \
	"$(INTDIR)\XTBrowseDialog.obj" \
	"$(INTDIR)\XTBrowseEdit.obj" \
	"$(INTDIR)\XTButton.obj" \
	"$(INTDIR)\XTCaption.obj" \
	"$(INTDIR)\XTCaptionPopupWnd.obj" \
	"$(INTDIR)\XTCBarDialog.obj" \
	"$(INTDIR)\XTCheckListBox.obj" \
	"$(INTDIR)\XTColorDialog.obj" \
	"$(INTDIR)\XTColorPageCustom.obj" \
	"$(INTDIR)\XTColorPageStandard.obj" \
	"$(INTDIR)\XTColorPicker.obj" \
	"$(INTDIR)\XTColorPopup.obj" \
	"$(INTDIR)\XTColorRef.obj" \
	"$(INTDIR)\XTColorSelectorCtrl.obj" \
	"$(INTDIR)\XTComboBoxEx.obj" \
	"$(INTDIR)\XTCommandsListBox.obj" \
	"$(INTDIR)\XTControlBar.obj" \
	"$(INTDIR)\XTCoolMenu.obj" \
	"$(INTDIR)\XTCrc32.obj" \
	"$(INTDIR)\XTCustomizeAPI.obj" \
	"$(INTDIR)\XTCustomizePage.obj" \
	"$(INTDIR)\XTCustomizeSheet.obj" \
	"$(INTDIR)\XTCustomToolBar.obj" \
	"$(INTDIR)\XTDateTimeCtrl.obj" \
	"$(INTDIR)\XTDialogBar.obj" \
	"$(INTDIR)\XTDockBar.obj" \
	"$(INTDIR)\XTDockColorSelector.obj" \
	"$(INTDIR)\XTDockContext.obj" \
	"$(INTDIR)\XTDockState.obj" \
	"$(INTDIR)\XTDockWindow.obj" \
	"$(INTDIR)\XTDropSource.obj" \
	"$(INTDIR)\XTEditListBox.obj" \
	"$(INTDIR)\XTExpButton.obj" \
	"$(INTDIR)\XTExpMenuWnd.obj" \
	"$(INTDIR)\XTFlatComboBox.obj" \
	"$(INTDIR)\XTFlatHeaderCtrl.obj" \
	"$(INTDIR)\XTFlatTabCtrl.obj" \
	"$(INTDIR)\XTFontCombo.obj" \
	"$(INTDIR)\XTFrameImpl.obj" \
	"$(INTDIR)\XTFrameWnd.obj" \
	"$(INTDIR)\XTGlobal.obj" \
	"$(INTDIR)\XTHelpers.obj" \
	"$(INTDIR)\XTHexEdit.obj" \
	"$(INTDIR)\XTHtmlView.obj" \
	"$(INTDIR)\XTHyperLink.obj" \
	"$(INTDIR)\XTIconMap.obj" \
	"$(INTDIR)\XTImageManager.obj" \
	"$(INTDIR)\XTListCtrl.obj" \
	"$(INTDIR)\XTListView.obj" \
	"$(INTDIR)\XTLoadSysColorBitmap.obj" \
	"$(INTDIR)\XTLogoPane.obj" \
	"$(INTDIR)\XTMaskEdit.obj" \
	"$(INTDIR)\XTMDIChildWnd.obj" \
	"$(INTDIR)\XTMDIFrameWnd.obj" \
	"$(INTDIR)\XTMDIWndTab.obj" \
	"$(INTDIR)\XTMemDC.obj" \
	"$(INTDIR)\XTMemFile.obj" \
	"$(INTDIR)\XTMenuBar.obj" \
	"$(INTDIR)\XTMenuBar1.obj" \
	"$(INTDIR)\XTMiniDockFrameWnd.obj" \
	"$(INTDIR)\XTNewToolbarDlg.obj" \
	"$(INTDIR)\XTOleIPFrameWnd.obj" \
	"$(INTDIR)\XTOptionsManager.obj" \
	"$(INTDIR)\XTOSVersionInfo.obj" \
	"$(INTDIR)\XTOutBarCtrl.obj" \
	"$(INTDIR)\XTOutlookBar.obj" \
	"$(INTDIR)\XTPagerCtrl.obj" \
	"$(INTDIR)\XTPopupTearOffWnd.obj" \
	"$(INTDIR)\XTPreviewView.obj" \
	"$(INTDIR)\XTReBar.obj" \
	"$(INTDIR)\XTReBar1.obj" \
	"$(INTDIR)\XTReBarCtrl.obj" \
	"$(INTDIR)\XTRegistryManager.obj" \
	"$(INTDIR)\XTResize.obj" \
	"$(INTDIR)\XTResizeDialog.obj" \
	"$(INTDIR)\XTResizeFormView.obj" \
	"$(INTDIR)\XTResizePoint.obj" \
	"$(INTDIR)\XTResizePropertyPage.obj" \
	"$(INTDIR)\XTResizePropertySheet.obj" \
	"$(INTDIR)\XTResizeRect.obj" \
	"$(INTDIR)\XTSearchOptionsCtrl.obj" \
	"$(INTDIR)\XTShellListCtrl.obj" \
	"$(INTDIR)\XTShellListView.obj" \
	"$(INTDIR)\XTShellPidl.obj" \
	"$(INTDIR)\XTShellSettings.obj" \
	"$(INTDIR)\XTShellTreeCtrl.obj" \
	"$(INTDIR)\XTShellTreeView.obj" \
	"$(INTDIR)\XTSkins.obj" \
	"$(INTDIR)\XTSortClass.obj" \
	"$(INTDIR)\XTSplitterDock.obj" \
	"$(INTDIR)\XTSplitterWnd.obj" \
	"$(INTDIR)\XTStatusBar.obj" \
	"$(INTDIR)\XTTabCtrl.obj" \
	"$(INTDIR)\XTTabCtrlBar.obj" \
	"$(INTDIR)\XTTabCtrlButtons.obj" \
	"$(INTDIR)\XTTabView.obj" \
	"$(INTDIR)\XTTipOfTheDay.obj" \
	"$(INTDIR)\XTTipWindow.obj" \
	"$(INTDIR)\XTToolBar.obj" \
	"$(INTDIR)\XTToolBar1.obj" \
	"$(INTDIR)\XTToolBarCtrl.obj" \
	"$(INTDIR)\XTToolBarPopupWnd.obj" \
	"$(INTDIR)\XTToolCmdUI.obj" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Lib_vc50\XT3100LibStatic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\Lib_vc50\XT3100LibStaticd.lib"

!ELSE 

ALL : "..\Lib_vc50\XT3100LibStaticd.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Lib_vc50\XT3100LibStaticd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

RSC=rc.exe
CPP=cl.exe
CPP_PROJ=/nologo /MTd /W4 /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK" /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch"\
 /Yu"StdAfx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Lib_vc50.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../Lib_vc50/XT3100LibStaticd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XTAccelKeyEdit.obj" \
	"$(INTDIR)\XTAccelManager.obj" \
	"$(INTDIR)\XTAnimationMemDC.obj" \
	"$(INTDIR)\XTBrowseDialog.obj" \
	"$(INTDIR)\XTBrowseEdit.obj" \
	"$(INTDIR)\XTButton.obj" \
	"$(INTDIR)\XTCaption.obj" \
	"$(INTDIR)\XTCaptionPopupWnd.obj" \
	"$(INTDIR)\XTCBarDialog.obj" \
	"$(INTDIR)\XTCheckListBox.obj" \
	"$(INTDIR)\XTColorDialog.obj" \
	"$(INTDIR)\XTColorPageCustom.obj" \
	"$(INTDIR)\XTColorPageStandard.obj" \
	"$(INTDIR)\XTColorPicker.obj" \
	"$(INTDIR)\XTColorPopup.obj" \
	"$(INTDIR)\XTColorRef.obj" \
	"$(INTDIR)\XTColorSelectorCtrl.obj" \
	"$(INTDIR)\XTComboBoxEx.obj" \
	"$(INTDIR)\XTCommandsListBox.obj" \
	"$(INTDIR)\XTControlBar.obj" \
	"$(INTDIR)\XTCoolMenu.obj" \
	"$(INTDIR)\XTCrc32.obj" \
	"$(INTDIR)\XTCustomizeAPI.obj" \
	"$(INTDIR)\XTCustomizePage.obj" \
	"$(INTDIR)\XTCustomizeSheet.obj" \
	"$(INTDIR)\XTCustomToolBar.obj" \
	"$(INTDIR)\XTDateTimeCtrl.obj" \
	"$(INTDIR)\XTDialogBar.obj" \
	"$(INTDIR)\XTDockBar.obj" \
	"$(INTDIR)\XTDockColorSelector.obj" \
	"$(INTDIR)\XTDockContext.obj" \
	"$(INTDIR)\XTDockState.obj" \
	"$(INTDIR)\XTDockWindow.obj" \
	"$(INTDIR)\XTDropSource.obj" \
	"$(INTDIR)\XTEditListBox.obj" \
	"$(INTDIR)\XTExpButton.obj" \
	"$(INTDIR)\XTExpMenuWnd.obj" \
	"$(INTDIR)\XTFlatComboBox.obj" \
	"$(INTDIR)\XTFlatHeaderCtrl.obj" \
	"$(INTDIR)\XTFlatTabCtrl.obj" \
	"$(INTDIR)\XTFontCombo.obj" \
	"$(INTDIR)\XTFrameImpl.obj" \
	"$(INTDIR)\XTFrameWnd.obj" \
	"$(INTDIR)\XTGlobal.obj" \
	"$(INTDIR)\XTHelpers.obj" \
	"$(INTDIR)\XTHexEdit.obj" \
	"$(INTDIR)\XTHtmlView.obj" \
	"$(INTDIR)\XTHyperLink.obj" \
	"$(INTDIR)\XTIconMap.obj" \
	"$(INTDIR)\XTImageManager.obj" \
	"$(INTDIR)\XTListCtrl.obj" \
	"$(INTDIR)\XTListView.obj" \
	"$(INTDIR)\XTLoadSysColorBitmap.obj" \
	"$(INTDIR)\XTLogoPane.obj" \
	"$(INTDIR)\XTMaskEdit.obj" \
	"$(INTDIR)\XTMDIChildWnd.obj" \
	"$(INTDIR)\XTMDIFrameWnd.obj" \
	"$(INTDIR)\XTMDIWndTab.obj" \
	"$(INTDIR)\XTMemDC.obj" \
	"$(INTDIR)\XTMemFile.obj" \
	"$(INTDIR)\XTMenuBar.obj" \
	"$(INTDIR)\XTMenuBar1.obj" \
	"$(INTDIR)\XTMiniDockFrameWnd.obj" \
	"$(INTDIR)\XTNewToolbarDlg.obj" \
	"$(INTDIR)\XTOleIPFrameWnd.obj" \
	"$(INTDIR)\XTOptionsManager.obj" \
	"$(INTDIR)\XTOSVersionInfo.obj" \
	"$(INTDIR)\XTOutBarCtrl.obj" \
	"$(INTDIR)\XTOutlookBar.obj" \
	"$(INTDIR)\XTPagerCtrl.obj" \
	"$(INTDIR)\XTPopupTearOffWnd.obj" \
	"$(INTDIR)\XTPreviewView.obj" \
	"$(INTDIR)\XTReBar.obj" \
	"$(INTDIR)\XTReBar1.obj" \
	"$(INTDIR)\XTReBarCtrl.obj" \
	"$(INTDIR)\XTRegistryManager.obj" \
	"$(INTDIR)\XTResize.obj" \
	"$(INTDIR)\XTResizeDialog.obj" \
	"$(INTDIR)\XTResizeFormView.obj" \
	"$(INTDIR)\XTResizePoint.obj" \
	"$(INTDIR)\XTResizePropertyPage.obj" \
	"$(INTDIR)\XTResizePropertySheet.obj" \
	"$(INTDIR)\XTResizeRect.obj" \
	"$(INTDIR)\XTSearchOptionsCtrl.obj" \
	"$(INTDIR)\XTShellListCtrl.obj" \
	"$(INTDIR)\XTShellListView.obj" \
	"$(INTDIR)\XTShellPidl.obj" \
	"$(INTDIR)\XTShellSettings.obj" \
	"$(INTDIR)\XTShellTreeCtrl.obj" \
	"$(INTDIR)\XTShellTreeView.obj" \
	"$(INTDIR)\XTSkins.obj" \
	"$(INTDIR)\XTSortClass.obj" \
	"$(INTDIR)\XTSplitterDock.obj" \
	"$(INTDIR)\XTSplitterWnd.obj" \
	"$(INTDIR)\XTStatusBar.obj" \
	"$(INTDIR)\XTTabCtrl.obj" \
	"$(INTDIR)\XTTabCtrlBar.obj" \
	"$(INTDIR)\XTTabCtrlButtons.obj" \
	"$(INTDIR)\XTTabView.obj" \
	"$(INTDIR)\XTTipOfTheDay.obj" \
	"$(INTDIR)\XTTipWindow.obj" \
	"$(INTDIR)\XTToolBar.obj" \
	"$(INTDIR)\XTToolBar1.obj" \
	"$(INTDIR)\XTToolBarCtrl.obj" \
	"$(INTDIR)\XTToolBarPopupWnd.obj" \
	"$(INTDIR)\XTToolCmdUI.obj" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Lib_vc50\XT3100LibStaticd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Unicode Release"

OUTDIR=.\Unicode_Release
INTDIR=.\Unicode_Release

!IF "$(RECURSE)" == "0" 

ALL : "..\Lib_vc50\XT3100LibStaticu.lib"

!ELSE 

ALL : "..\Lib_vc50\XT3100LibStaticu.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Lib_vc50\XT3100LibStaticu.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

RSC=rc.exe
CPP=cl.exe
CPP_PROJ=/nologo /MT /W4 /WX /GX /O2 /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D\
 "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yu"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Unicode_Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Lib_vc50.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../Lib_vc50/XT3100LibStaticu.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XTAccelKeyEdit.obj" \
	"$(INTDIR)\XTAccelManager.obj" \
	"$(INTDIR)\XTAnimationMemDC.obj" \
	"$(INTDIR)\XTBrowseDialog.obj" \
	"$(INTDIR)\XTBrowseEdit.obj" \
	"$(INTDIR)\XTButton.obj" \
	"$(INTDIR)\XTCaption.obj" \
	"$(INTDIR)\XTCaptionPopupWnd.obj" \
	"$(INTDIR)\XTCBarDialog.obj" \
	"$(INTDIR)\XTCheckListBox.obj" \
	"$(INTDIR)\XTColorDialog.obj" \
	"$(INTDIR)\XTColorPageCustom.obj" \
	"$(INTDIR)\XTColorPageStandard.obj" \
	"$(INTDIR)\XTColorPicker.obj" \
	"$(INTDIR)\XTColorPopup.obj" \
	"$(INTDIR)\XTColorRef.obj" \
	"$(INTDIR)\XTColorSelectorCtrl.obj" \
	"$(INTDIR)\XTComboBoxEx.obj" \
	"$(INTDIR)\XTCommandsListBox.obj" \
	"$(INTDIR)\XTControlBar.obj" \
	"$(INTDIR)\XTCoolMenu.obj" \
	"$(INTDIR)\XTCrc32.obj" \
	"$(INTDIR)\XTCustomizeAPI.obj" \
	"$(INTDIR)\XTCustomizePage.obj" \
	"$(INTDIR)\XTCustomizeSheet.obj" \
	"$(INTDIR)\XTCustomToolBar.obj" \
	"$(INTDIR)\XTDateTimeCtrl.obj" \
	"$(INTDIR)\XTDialogBar.obj" \
	"$(INTDIR)\XTDockBar.obj" \
	"$(INTDIR)\XTDockColorSelector.obj" \
	"$(INTDIR)\XTDockContext.obj" \
	"$(INTDIR)\XTDockState.obj" \
	"$(INTDIR)\XTDockWindow.obj" \
	"$(INTDIR)\XTDropSource.obj" \
	"$(INTDIR)\XTEditListBox.obj" \
	"$(INTDIR)\XTExpButton.obj" \
	"$(INTDIR)\XTExpMenuWnd.obj" \
	"$(INTDIR)\XTFlatComboBox.obj" \
	"$(INTDIR)\XTFlatHeaderCtrl.obj" \
	"$(INTDIR)\XTFlatTabCtrl.obj" \
	"$(INTDIR)\XTFontCombo.obj" \
	"$(INTDIR)\XTFrameImpl.obj" \
	"$(INTDIR)\XTFrameWnd.obj" \
	"$(INTDIR)\XTGlobal.obj" \
	"$(INTDIR)\XTHelpers.obj" \
	"$(INTDIR)\XTHexEdit.obj" \
	"$(INTDIR)\XTHtmlView.obj" \
	"$(INTDIR)\XTHyperLink.obj" \
	"$(INTDIR)\XTIconMap.obj" \
	"$(INTDIR)\XTImageManager.obj" \
	"$(INTDIR)\XTListCtrl.obj" \
	"$(INTDIR)\XTListView.obj" \
	"$(INTDIR)\XTLoadSysColorBitmap.obj" \
	"$(INTDIR)\XTLogoPane.obj" \
	"$(INTDIR)\XTMaskEdit.obj" \
	"$(INTDIR)\XTMDIChildWnd.obj" \
	"$(INTDIR)\XTMDIFrameWnd.obj" \
	"$(INTDIR)\XTMDIWndTab.obj" \
	"$(INTDIR)\XTMemDC.obj" \
	"$(INTDIR)\XTMemFile.obj" \
	"$(INTDIR)\XTMenuBar.obj" \
	"$(INTDIR)\XTMenuBar1.obj" \
	"$(INTDIR)\XTMiniDockFrameWnd.obj" \
	"$(INTDIR)\XTNewToolbarDlg.obj" \
	"$(INTDIR)\XTOleIPFrameWnd.obj" \
	"$(INTDIR)\XTOptionsManager.obj" \
	"$(INTDIR)\XTOSVersionInfo.obj" \
	"$(INTDIR)\XTOutBarCtrl.obj" \
	"$(INTDIR)\XTOutlookBar.obj" \
	"$(INTDIR)\XTPagerCtrl.obj" \
	"$(INTDIR)\XTPopupTearOffWnd.obj" \
	"$(INTDIR)\XTPreviewView.obj" \
	"$(INTDIR)\XTReBar.obj" \
	"$(INTDIR)\XTReBar1.obj" \
	"$(INTDIR)\XTReBarCtrl.obj" \
	"$(INTDIR)\XTRegistryManager.obj" \
	"$(INTDIR)\XTResize.obj" \
	"$(INTDIR)\XTResizeDialog.obj" \
	"$(INTDIR)\XTResizeFormView.obj" \
	"$(INTDIR)\XTResizePoint.obj" \
	"$(INTDIR)\XTResizePropertyPage.obj" \
	"$(INTDIR)\XTResizePropertySheet.obj" \
	"$(INTDIR)\XTResizeRect.obj" \
	"$(INTDIR)\XTSearchOptionsCtrl.obj" \
	"$(INTDIR)\XTShellListCtrl.obj" \
	"$(INTDIR)\XTShellListView.obj" \
	"$(INTDIR)\XTShellPidl.obj" \
	"$(INTDIR)\XTShellSettings.obj" \
	"$(INTDIR)\XTShellTreeCtrl.obj" \
	"$(INTDIR)\XTShellTreeView.obj" \
	"$(INTDIR)\XTSkins.obj" \
	"$(INTDIR)\XTSortClass.obj" \
	"$(INTDIR)\XTSplitterDock.obj" \
	"$(INTDIR)\XTSplitterWnd.obj" \
	"$(INTDIR)\XTStatusBar.obj" \
	"$(INTDIR)\XTTabCtrl.obj" \
	"$(INTDIR)\XTTabCtrlBar.obj" \
	"$(INTDIR)\XTTabCtrlButtons.obj" \
	"$(INTDIR)\XTTabView.obj" \
	"$(INTDIR)\XTTipOfTheDay.obj" \
	"$(INTDIR)\XTTipWindow.obj" \
	"$(INTDIR)\XTToolBar.obj" \
	"$(INTDIR)\XTToolBar1.obj" \
	"$(INTDIR)\XTToolBarCtrl.obj" \
	"$(INTDIR)\XTToolBarPopupWnd.obj" \
	"$(INTDIR)\XTToolCmdUI.obj" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Lib_vc50\XT3100LibStaticu.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Unicode Debug"

OUTDIR=.\Unicode_Debug
INTDIR=.\Unicode_Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\Lib_vc50\XT3100LibStaticud.lib"

!ELSE 

ALL : "..\Lib_vc50\XT3100LibStaticud.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Lib_vc50\XT3100LibStaticud.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

RSC=rc.exe
CPP=cl.exe
CPP_PROJ=/nologo /MTd /W4 /WX /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D\
 "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yu"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Unicode_Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Lib_vc50.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../Lib_vc50/XT3100LibStaticud.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XTAccelKeyEdit.obj" \
	"$(INTDIR)\XTAccelManager.obj" \
	"$(INTDIR)\XTAnimationMemDC.obj" \
	"$(INTDIR)\XTBrowseDialog.obj" \
	"$(INTDIR)\XTBrowseEdit.obj" \
	"$(INTDIR)\XTButton.obj" \
	"$(INTDIR)\XTCaption.obj" \
	"$(INTDIR)\XTCaptionPopupWnd.obj" \
	"$(INTDIR)\XTCBarDialog.obj" \
	"$(INTDIR)\XTCheckListBox.obj" \
	"$(INTDIR)\XTColorDialog.obj" \
	"$(INTDIR)\XTColorPageCustom.obj" \
	"$(INTDIR)\XTColorPageStandard.obj" \
	"$(INTDIR)\XTColorPicker.obj" \
	"$(INTDIR)\XTColorPopup.obj" \
	"$(INTDIR)\XTColorRef.obj" \
	"$(INTDIR)\XTColorSelectorCtrl.obj" \
	"$(INTDIR)\XTComboBoxEx.obj" \
	"$(INTDIR)\XTCommandsListBox.obj" \
	"$(INTDIR)\XTControlBar.obj" \
	"$(INTDIR)\XTCoolMenu.obj" \
	"$(INTDIR)\XTCrc32.obj" \
	"$(INTDIR)\XTCustomizeAPI.obj" \
	"$(INTDIR)\XTCustomizePage.obj" \
	"$(INTDIR)\XTCustomizeSheet.obj" \
	"$(INTDIR)\XTCustomToolBar.obj" \
	"$(INTDIR)\XTDateTimeCtrl.obj" \
	"$(INTDIR)\XTDialogBar.obj" \
	"$(INTDIR)\XTDockBar.obj" \
	"$(INTDIR)\XTDockColorSelector.obj" \
	"$(INTDIR)\XTDockContext.obj" \
	"$(INTDIR)\XTDockState.obj" \
	"$(INTDIR)\XTDockWindow.obj" \
	"$(INTDIR)\XTDropSource.obj" \
	"$(INTDIR)\XTEditListBox.obj" \
	"$(INTDIR)\XTExpButton.obj" \
	"$(INTDIR)\XTExpMenuWnd.obj" \
	"$(INTDIR)\XTFlatComboBox.obj" \
	"$(INTDIR)\XTFlatHeaderCtrl.obj" \
	"$(INTDIR)\XTFlatTabCtrl.obj" \
	"$(INTDIR)\XTFontCombo.obj" \
	"$(INTDIR)\XTFrameImpl.obj" \
	"$(INTDIR)\XTFrameWnd.obj" \
	"$(INTDIR)\XTGlobal.obj" \
	"$(INTDIR)\XTHelpers.obj" \
	"$(INTDIR)\XTHexEdit.obj" \
	"$(INTDIR)\XTHtmlView.obj" \
	"$(INTDIR)\XTHyperLink.obj" \
	"$(INTDIR)\XTIconMap.obj" \
	"$(INTDIR)\XTImageManager.obj" \
	"$(INTDIR)\XTListCtrl.obj" \
	"$(INTDIR)\XTListView.obj" \
	"$(INTDIR)\XTLoadSysColorBitmap.obj" \
	"$(INTDIR)\XTLogoPane.obj" \
	"$(INTDIR)\XTMaskEdit.obj" \
	"$(INTDIR)\XTMDIChildWnd.obj" \
	"$(INTDIR)\XTMDIFrameWnd.obj" \
	"$(INTDIR)\XTMDIWndTab.obj" \
	"$(INTDIR)\XTMemDC.obj" \
	"$(INTDIR)\XTMemFile.obj" \
	"$(INTDIR)\XTMenuBar.obj" \
	"$(INTDIR)\XTMenuBar1.obj" \
	"$(INTDIR)\XTMiniDockFrameWnd.obj" \
	"$(INTDIR)\XTNewToolbarDlg.obj" \
	"$(INTDIR)\XTOleIPFrameWnd.obj" \
	"$(INTDIR)\XTOptionsManager.obj" \
	"$(INTDIR)\XTOSVersionInfo.obj" \
	"$(INTDIR)\XTOutBarCtrl.obj" \
	"$(INTDIR)\XTOutlookBar.obj" \
	"$(INTDIR)\XTPagerCtrl.obj" \
	"$(INTDIR)\XTPopupTearOffWnd.obj" \
	"$(INTDIR)\XTPreviewView.obj" \
	"$(INTDIR)\XTReBar.obj" \
	"$(INTDIR)\XTReBar1.obj" \
	"$(INTDIR)\XTReBarCtrl.obj" \
	"$(INTDIR)\XTRegistryManager.obj" \
	"$(INTDIR)\XTResize.obj" \
	"$(INTDIR)\XTResizeDialog.obj" \
	"$(INTDIR)\XTResizeFormView.obj" \
	"$(INTDIR)\XTResizePoint.obj" \
	"$(INTDIR)\XTResizePropertyPage.obj" \
	"$(INTDIR)\XTResizePropertySheet.obj" \
	"$(INTDIR)\XTResizeRect.obj" \
	"$(INTDIR)\XTSearchOptionsCtrl.obj" \
	"$(INTDIR)\XTShellListCtrl.obj" \
	"$(INTDIR)\XTShellListView.obj" \
	"$(INTDIR)\XTShellPidl.obj" \
	"$(INTDIR)\XTShellSettings.obj" \
	"$(INTDIR)\XTShellTreeCtrl.obj" \
	"$(INTDIR)\XTShellTreeView.obj" \
	"$(INTDIR)\XTSkins.obj" \
	"$(INTDIR)\XTSortClass.obj" \
	"$(INTDIR)\XTSplitterDock.obj" \
	"$(INTDIR)\XTSplitterWnd.obj" \
	"$(INTDIR)\XTStatusBar.obj" \
	"$(INTDIR)\XTTabCtrl.obj" \
	"$(INTDIR)\XTTabCtrlBar.obj" \
	"$(INTDIR)\XTTabCtrlButtons.obj" \
	"$(INTDIR)\XTTabView.obj" \
	"$(INTDIR)\XTTipOfTheDay.obj" \
	"$(INTDIR)\XTTipWindow.obj" \
	"$(INTDIR)\XTToolBar.obj" \
	"$(INTDIR)\XTToolBar1.obj" \
	"$(INTDIR)\XTToolBarCtrl.obj" \
	"$(INTDIR)\XTToolBarPopupWnd.obj" \
	"$(INTDIR)\XTToolCmdUI.obj" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Lib_vc50\XT3100LibStaticud.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Release"

OUTDIR=.\Dynamic_Release
INTDIR=.\Dynamic_Release

!IF "$(RECURSE)" == "0" 

ALL : "..\Lib_vc50\XT3100LibDynStatic.lib"

!ELSE 

ALL : "..\Lib_vc50\XT3100LibDynStatic.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Lib_vc50\XT3100LibDynStatic.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

RSC=rc.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /WX /GX /I "..\include" /I "..\include\Common.include"\
 /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D "_AFXDLL" /D "WIN32" /D\
 "_WINDOWS" /D "_XT_DYN_STATICLIB" /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch"\
 /Yu"StdAfx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Dynamic_Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Lib_vc50.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../Lib_vc50/XT3100LibDynStatic.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XTAccelKeyEdit.obj" \
	"$(INTDIR)\XTAccelManager.obj" \
	"$(INTDIR)\XTAnimationMemDC.obj" \
	"$(INTDIR)\XTBrowseDialog.obj" \
	"$(INTDIR)\XTBrowseEdit.obj" \
	"$(INTDIR)\XTButton.obj" \
	"$(INTDIR)\XTCaption.obj" \
	"$(INTDIR)\XTCaptionPopupWnd.obj" \
	"$(INTDIR)\XTCBarDialog.obj" \
	"$(INTDIR)\XTCheckListBox.obj" \
	"$(INTDIR)\XTColorDialog.obj" \
	"$(INTDIR)\XTColorPageCustom.obj" \
	"$(INTDIR)\XTColorPageStandard.obj" \
	"$(INTDIR)\XTColorPicker.obj" \
	"$(INTDIR)\XTColorPopup.obj" \
	"$(INTDIR)\XTColorRef.obj" \
	"$(INTDIR)\XTColorSelectorCtrl.obj" \
	"$(INTDIR)\XTComboBoxEx.obj" \
	"$(INTDIR)\XTCommandsListBox.obj" \
	"$(INTDIR)\XTControlBar.obj" \
	"$(INTDIR)\XTCoolMenu.obj" \
	"$(INTDIR)\XTCrc32.obj" \
	"$(INTDIR)\XTCustomizeAPI.obj" \
	"$(INTDIR)\XTCustomizePage.obj" \
	"$(INTDIR)\XTCustomizeSheet.obj" \
	"$(INTDIR)\XTCustomToolBar.obj" \
	"$(INTDIR)\XTDateTimeCtrl.obj" \
	"$(INTDIR)\XTDialogBar.obj" \
	"$(INTDIR)\XTDockBar.obj" \
	"$(INTDIR)\XTDockColorSelector.obj" \
	"$(INTDIR)\XTDockContext.obj" \
	"$(INTDIR)\XTDockState.obj" \
	"$(INTDIR)\XTDockWindow.obj" \
	"$(INTDIR)\XTDropSource.obj" \
	"$(INTDIR)\XTEditListBox.obj" \
	"$(INTDIR)\XTExpButton.obj" \
	"$(INTDIR)\XTExpMenuWnd.obj" \
	"$(INTDIR)\XTFlatComboBox.obj" \
	"$(INTDIR)\XTFlatHeaderCtrl.obj" \
	"$(INTDIR)\XTFlatTabCtrl.obj" \
	"$(INTDIR)\XTFontCombo.obj" \
	"$(INTDIR)\XTFrameImpl.obj" \
	"$(INTDIR)\XTFrameWnd.obj" \
	"$(INTDIR)\XTGlobal.obj" \
	"$(INTDIR)\XTHelpers.obj" \
	"$(INTDIR)\XTHexEdit.obj" \
	"$(INTDIR)\XTHtmlView.obj" \
	"$(INTDIR)\XTHyperLink.obj" \
	"$(INTDIR)\XTIconMap.obj" \
	"$(INTDIR)\XTImageManager.obj" \
	"$(INTDIR)\XTListCtrl.obj" \
	"$(INTDIR)\XTListView.obj" \
	"$(INTDIR)\XTLoadSysColorBitmap.obj" \
	"$(INTDIR)\XTLogoPane.obj" \
	"$(INTDIR)\XTMaskEdit.obj" \
	"$(INTDIR)\XTMDIChildWnd.obj" \
	"$(INTDIR)\XTMDIFrameWnd.obj" \
	"$(INTDIR)\XTMDIWndTab.obj" \
	"$(INTDIR)\XTMemDC.obj" \
	"$(INTDIR)\XTMemFile.obj" \
	"$(INTDIR)\XTMenuBar.obj" \
	"$(INTDIR)\XTMenuBar1.obj" \
	"$(INTDIR)\XTMiniDockFrameWnd.obj" \
	"$(INTDIR)\XTNewToolbarDlg.obj" \
	"$(INTDIR)\XTOleIPFrameWnd.obj" \
	"$(INTDIR)\XTOptionsManager.obj" \
	"$(INTDIR)\XTOSVersionInfo.obj" \
	"$(INTDIR)\XTOutBarCtrl.obj" \
	"$(INTDIR)\XTOutlookBar.obj" \
	"$(INTDIR)\XTPagerCtrl.obj" \
	"$(INTDIR)\XTPopupTearOffWnd.obj" \
	"$(INTDIR)\XTPreviewView.obj" \
	"$(INTDIR)\XTReBar.obj" \
	"$(INTDIR)\XTReBar1.obj" \
	"$(INTDIR)\XTReBarCtrl.obj" \
	"$(INTDIR)\XTRegistryManager.obj" \
	"$(INTDIR)\XTResize.obj" \
	"$(INTDIR)\XTResizeDialog.obj" \
	"$(INTDIR)\XTResizeFormView.obj" \
	"$(INTDIR)\XTResizePoint.obj" \
	"$(INTDIR)\XTResizePropertyPage.obj" \
	"$(INTDIR)\XTResizePropertySheet.obj" \
	"$(INTDIR)\XTResizeRect.obj" \
	"$(INTDIR)\XTSearchOptionsCtrl.obj" \
	"$(INTDIR)\XTShellListCtrl.obj" \
	"$(INTDIR)\XTShellListView.obj" \
	"$(INTDIR)\XTShellPidl.obj" \
	"$(INTDIR)\XTShellSettings.obj" \
	"$(INTDIR)\XTShellTreeCtrl.obj" \
	"$(INTDIR)\XTShellTreeView.obj" \
	"$(INTDIR)\XTSkins.obj" \
	"$(INTDIR)\XTSortClass.obj" \
	"$(INTDIR)\XTSplitterDock.obj" \
	"$(INTDIR)\XTSplitterWnd.obj" \
	"$(INTDIR)\XTStatusBar.obj" \
	"$(INTDIR)\XTTabCtrl.obj" \
	"$(INTDIR)\XTTabCtrlBar.obj" \
	"$(INTDIR)\XTTabCtrlButtons.obj" \
	"$(INTDIR)\XTTabView.obj" \
	"$(INTDIR)\XTTipOfTheDay.obj" \
	"$(INTDIR)\XTTipWindow.obj" \
	"$(INTDIR)\XTToolBar.obj" \
	"$(INTDIR)\XTToolBar1.obj" \
	"$(INTDIR)\XTToolBarCtrl.obj" \
	"$(INTDIR)\XTToolBarPopupWnd.obj" \
	"$(INTDIR)\XTToolCmdUI.obj" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Lib_vc50\XT3100LibDynStatic.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Debug"

OUTDIR=.\Dynamic_Debug
INTDIR=.\Dynamic_Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\Lib_vc50\XT3100LibDynStaticd.lib"

!ELSE 

ALL : "..\Lib_vc50\XT3100LibDynStaticd.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Lib_vc50\XT3100LibDynStaticd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

RSC=rc.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W4 /WX /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D\
 "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yu"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Dynamic_Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Lib_vc50.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../Lib_vc50/XT3100LibDynStaticd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XTAccelKeyEdit.obj" \
	"$(INTDIR)\XTAccelManager.obj" \
	"$(INTDIR)\XTAnimationMemDC.obj" \
	"$(INTDIR)\XTBrowseDialog.obj" \
	"$(INTDIR)\XTBrowseEdit.obj" \
	"$(INTDIR)\XTButton.obj" \
	"$(INTDIR)\XTCaption.obj" \
	"$(INTDIR)\XTCaptionPopupWnd.obj" \
	"$(INTDIR)\XTCBarDialog.obj" \
	"$(INTDIR)\XTCheckListBox.obj" \
	"$(INTDIR)\XTColorDialog.obj" \
	"$(INTDIR)\XTColorPageCustom.obj" \
	"$(INTDIR)\XTColorPageStandard.obj" \
	"$(INTDIR)\XTColorPicker.obj" \
	"$(INTDIR)\XTColorPopup.obj" \
	"$(INTDIR)\XTColorRef.obj" \
	"$(INTDIR)\XTColorSelectorCtrl.obj" \
	"$(INTDIR)\XTComboBoxEx.obj" \
	"$(INTDIR)\XTCommandsListBox.obj" \
	"$(INTDIR)\XTControlBar.obj" \
	"$(INTDIR)\XTCoolMenu.obj" \
	"$(INTDIR)\XTCrc32.obj" \
	"$(INTDIR)\XTCustomizeAPI.obj" \
	"$(INTDIR)\XTCustomizePage.obj" \
	"$(INTDIR)\XTCustomizeSheet.obj" \
	"$(INTDIR)\XTCustomToolBar.obj" \
	"$(INTDIR)\XTDateTimeCtrl.obj" \
	"$(INTDIR)\XTDialogBar.obj" \
	"$(INTDIR)\XTDockBar.obj" \
	"$(INTDIR)\XTDockColorSelector.obj" \
	"$(INTDIR)\XTDockContext.obj" \
	"$(INTDIR)\XTDockState.obj" \
	"$(INTDIR)\XTDockWindow.obj" \
	"$(INTDIR)\XTDropSource.obj" \
	"$(INTDIR)\XTEditListBox.obj" \
	"$(INTDIR)\XTExpButton.obj" \
	"$(INTDIR)\XTExpMenuWnd.obj" \
	"$(INTDIR)\XTFlatComboBox.obj" \
	"$(INTDIR)\XTFlatHeaderCtrl.obj" \
	"$(INTDIR)\XTFlatTabCtrl.obj" \
	"$(INTDIR)\XTFontCombo.obj" \
	"$(INTDIR)\XTFrameImpl.obj" \
	"$(INTDIR)\XTFrameWnd.obj" \
	"$(INTDIR)\XTGlobal.obj" \
	"$(INTDIR)\XTHelpers.obj" \
	"$(INTDIR)\XTHexEdit.obj" \
	"$(INTDIR)\XTHtmlView.obj" \
	"$(INTDIR)\XTHyperLink.obj" \
	"$(INTDIR)\XTIconMap.obj" \
	"$(INTDIR)\XTImageManager.obj" \
	"$(INTDIR)\XTListCtrl.obj" \
	"$(INTDIR)\XTListView.obj" \
	"$(INTDIR)\XTLoadSysColorBitmap.obj" \
	"$(INTDIR)\XTLogoPane.obj" \
	"$(INTDIR)\XTMaskEdit.obj" \
	"$(INTDIR)\XTMDIChildWnd.obj" \
	"$(INTDIR)\XTMDIFrameWnd.obj" \
	"$(INTDIR)\XTMDIWndTab.obj" \
	"$(INTDIR)\XTMemDC.obj" \
	"$(INTDIR)\XTMemFile.obj" \
	"$(INTDIR)\XTMenuBar.obj" \
	"$(INTDIR)\XTMenuBar1.obj" \
	"$(INTDIR)\XTMiniDockFrameWnd.obj" \
	"$(INTDIR)\XTNewToolbarDlg.obj" \
	"$(INTDIR)\XTOleIPFrameWnd.obj" \
	"$(INTDIR)\XTOptionsManager.obj" \
	"$(INTDIR)\XTOSVersionInfo.obj" \
	"$(INTDIR)\XTOutBarCtrl.obj" \
	"$(INTDIR)\XTOutlookBar.obj" \
	"$(INTDIR)\XTPagerCtrl.obj" \
	"$(INTDIR)\XTPopupTearOffWnd.obj" \
	"$(INTDIR)\XTPreviewView.obj" \
	"$(INTDIR)\XTReBar.obj" \
	"$(INTDIR)\XTReBar1.obj" \
	"$(INTDIR)\XTReBarCtrl.obj" \
	"$(INTDIR)\XTRegistryManager.obj" \
	"$(INTDIR)\XTResize.obj" \
	"$(INTDIR)\XTResizeDialog.obj" \
	"$(INTDIR)\XTResizeFormView.obj" \
	"$(INTDIR)\XTResizePoint.obj" \
	"$(INTDIR)\XTResizePropertyPage.obj" \
	"$(INTDIR)\XTResizePropertySheet.obj" \
	"$(INTDIR)\XTResizeRect.obj" \
	"$(INTDIR)\XTSearchOptionsCtrl.obj" \
	"$(INTDIR)\XTShellListCtrl.obj" \
	"$(INTDIR)\XTShellListView.obj" \
	"$(INTDIR)\XTShellPidl.obj" \
	"$(INTDIR)\XTShellSettings.obj" \
	"$(INTDIR)\XTShellTreeCtrl.obj" \
	"$(INTDIR)\XTShellTreeView.obj" \
	"$(INTDIR)\XTSkins.obj" \
	"$(INTDIR)\XTSortClass.obj" \
	"$(INTDIR)\XTSplitterDock.obj" \
	"$(INTDIR)\XTSplitterWnd.obj" \
	"$(INTDIR)\XTStatusBar.obj" \
	"$(INTDIR)\XTTabCtrl.obj" \
	"$(INTDIR)\XTTabCtrlBar.obj" \
	"$(INTDIR)\XTTabCtrlButtons.obj" \
	"$(INTDIR)\XTTabView.obj" \
	"$(INTDIR)\XTTipOfTheDay.obj" \
	"$(INTDIR)\XTTipWindow.obj" \
	"$(INTDIR)\XTToolBar.obj" \
	"$(INTDIR)\XTToolBar1.obj" \
	"$(INTDIR)\XTToolBarCtrl.obj" \
	"$(INTDIR)\XTToolBarPopupWnd.obj" \
	"$(INTDIR)\XTToolCmdUI.obj" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Lib_vc50\XT3100LibDynStaticd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Unicode Release"

OUTDIR=.\Dynamic_Unicode_Release
INTDIR=.\Dynamic_Unicode_Release

!IF "$(RECURSE)" == "0" 

ALL : "..\Lib_vc50\XT3100LibDynStaticu.lib"

!ELSE 

ALL : "..\Lib_vc50\XT3100LibDynStaticu.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Lib_vc50\XT3100LibDynStaticu.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

RSC=rc.exe
CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /WX /GX /I "..\include" /I "..\include\Common.include"\
 /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D "_UNICODE" /D "_AFXDLL" /D\
 "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yu"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Dynamic_Unicode_Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Lib_vc50.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../Lib_vc50/XT3100LibDynStaticu.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XTAccelKeyEdit.obj" \
	"$(INTDIR)\XTAccelManager.obj" \
	"$(INTDIR)\XTAnimationMemDC.obj" \
	"$(INTDIR)\XTBrowseDialog.obj" \
	"$(INTDIR)\XTBrowseEdit.obj" \
	"$(INTDIR)\XTButton.obj" \
	"$(INTDIR)\XTCaption.obj" \
	"$(INTDIR)\XTCaptionPopupWnd.obj" \
	"$(INTDIR)\XTCBarDialog.obj" \
	"$(INTDIR)\XTCheckListBox.obj" \
	"$(INTDIR)\XTColorDialog.obj" \
	"$(INTDIR)\XTColorPageCustom.obj" \
	"$(INTDIR)\XTColorPageStandard.obj" \
	"$(INTDIR)\XTColorPicker.obj" \
	"$(INTDIR)\XTColorPopup.obj" \
	"$(INTDIR)\XTColorRef.obj" \
	"$(INTDIR)\XTColorSelectorCtrl.obj" \
	"$(INTDIR)\XTComboBoxEx.obj" \
	"$(INTDIR)\XTCommandsListBox.obj" \
	"$(INTDIR)\XTControlBar.obj" \
	"$(INTDIR)\XTCoolMenu.obj" \
	"$(INTDIR)\XTCrc32.obj" \
	"$(INTDIR)\XTCustomizeAPI.obj" \
	"$(INTDIR)\XTCustomizePage.obj" \
	"$(INTDIR)\XTCustomizeSheet.obj" \
	"$(INTDIR)\XTCustomToolBar.obj" \
	"$(INTDIR)\XTDateTimeCtrl.obj" \
	"$(INTDIR)\XTDialogBar.obj" \
	"$(INTDIR)\XTDockBar.obj" \
	"$(INTDIR)\XTDockColorSelector.obj" \
	"$(INTDIR)\XTDockContext.obj" \
	"$(INTDIR)\XTDockState.obj" \
	"$(INTDIR)\XTDockWindow.obj" \
	"$(INTDIR)\XTDropSource.obj" \
	"$(INTDIR)\XTEditListBox.obj" \
	"$(INTDIR)\XTExpButton.obj" \
	"$(INTDIR)\XTExpMenuWnd.obj" \
	"$(INTDIR)\XTFlatComboBox.obj" \
	"$(INTDIR)\XTFlatHeaderCtrl.obj" \
	"$(INTDIR)\XTFlatTabCtrl.obj" \
	"$(INTDIR)\XTFontCombo.obj" \
	"$(INTDIR)\XTFrameImpl.obj" \
	"$(INTDIR)\XTFrameWnd.obj" \
	"$(INTDIR)\XTGlobal.obj" \
	"$(INTDIR)\XTHelpers.obj" \
	"$(INTDIR)\XTHexEdit.obj" \
	"$(INTDIR)\XTHtmlView.obj" \
	"$(INTDIR)\XTHyperLink.obj" \
	"$(INTDIR)\XTIconMap.obj" \
	"$(INTDIR)\XTImageManager.obj" \
	"$(INTDIR)\XTListCtrl.obj" \
	"$(INTDIR)\XTListView.obj" \
	"$(INTDIR)\XTLoadSysColorBitmap.obj" \
	"$(INTDIR)\XTLogoPane.obj" \
	"$(INTDIR)\XTMaskEdit.obj" \
	"$(INTDIR)\XTMDIChildWnd.obj" \
	"$(INTDIR)\XTMDIFrameWnd.obj" \
	"$(INTDIR)\XTMDIWndTab.obj" \
	"$(INTDIR)\XTMemDC.obj" \
	"$(INTDIR)\XTMemFile.obj" \
	"$(INTDIR)\XTMenuBar.obj" \
	"$(INTDIR)\XTMenuBar1.obj" \
	"$(INTDIR)\XTMiniDockFrameWnd.obj" \
	"$(INTDIR)\XTNewToolbarDlg.obj" \
	"$(INTDIR)\XTOleIPFrameWnd.obj" \
	"$(INTDIR)\XTOptionsManager.obj" \
	"$(INTDIR)\XTOSVersionInfo.obj" \
	"$(INTDIR)\XTOutBarCtrl.obj" \
	"$(INTDIR)\XTOutlookBar.obj" \
	"$(INTDIR)\XTPagerCtrl.obj" \
	"$(INTDIR)\XTPopupTearOffWnd.obj" \
	"$(INTDIR)\XTPreviewView.obj" \
	"$(INTDIR)\XTReBar.obj" \
	"$(INTDIR)\XTReBar1.obj" \
	"$(INTDIR)\XTReBarCtrl.obj" \
	"$(INTDIR)\XTRegistryManager.obj" \
	"$(INTDIR)\XTResize.obj" \
	"$(INTDIR)\XTResizeDialog.obj" \
	"$(INTDIR)\XTResizeFormView.obj" \
	"$(INTDIR)\XTResizePoint.obj" \
	"$(INTDIR)\XTResizePropertyPage.obj" \
	"$(INTDIR)\XTResizePropertySheet.obj" \
	"$(INTDIR)\XTResizeRect.obj" \
	"$(INTDIR)\XTSearchOptionsCtrl.obj" \
	"$(INTDIR)\XTShellListCtrl.obj" \
	"$(INTDIR)\XTShellListView.obj" \
	"$(INTDIR)\XTShellPidl.obj" \
	"$(INTDIR)\XTShellSettings.obj" \
	"$(INTDIR)\XTShellTreeCtrl.obj" \
	"$(INTDIR)\XTShellTreeView.obj" \
	"$(INTDIR)\XTSkins.obj" \
	"$(INTDIR)\XTSortClass.obj" \
	"$(INTDIR)\XTSplitterDock.obj" \
	"$(INTDIR)\XTSplitterWnd.obj" \
	"$(INTDIR)\XTStatusBar.obj" \
	"$(INTDIR)\XTTabCtrl.obj" \
	"$(INTDIR)\XTTabCtrlBar.obj" \
	"$(INTDIR)\XTTabCtrlButtons.obj" \
	"$(INTDIR)\XTTabView.obj" \
	"$(INTDIR)\XTTipOfTheDay.obj" \
	"$(INTDIR)\XTTipWindow.obj" \
	"$(INTDIR)\XTToolBar.obj" \
	"$(INTDIR)\XTToolBar1.obj" \
	"$(INTDIR)\XTToolBarCtrl.obj" \
	"$(INTDIR)\XTToolBarPopupWnd.obj" \
	"$(INTDIR)\XTToolCmdUI.obj" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Lib_vc50\XT3100LibDynStaticu.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Unicode Debug"

OUTDIR=.\Dynamic_Unicode_Debug
INTDIR=.\Dynamic_Unicode_Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\Lib_vc50\XT3100LibDynStaticud.lib"

!ELSE 

ALL : "..\Lib_vc50\XT3100LibDynStaticud.lib"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Lib_vc50\XT3100LibDynStaticud.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

RSC=rc.exe
CPP=cl.exe
CPP_PROJ=/nologo /MDd /W4 /WX /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D\
 "_UNICODE" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yu"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Dynamic_Unicode_Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Lib_vc50.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"../Lib_vc50/XT3100LibDynStaticud.lib" 
LIB32_OBJS= \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\XTAccelKeyEdit.obj" \
	"$(INTDIR)\XTAccelManager.obj" \
	"$(INTDIR)\XTAnimationMemDC.obj" \
	"$(INTDIR)\XTBrowseDialog.obj" \
	"$(INTDIR)\XTBrowseEdit.obj" \
	"$(INTDIR)\XTButton.obj" \
	"$(INTDIR)\XTCaption.obj" \
	"$(INTDIR)\XTCaptionPopupWnd.obj" \
	"$(INTDIR)\XTCBarDialog.obj" \
	"$(INTDIR)\XTCheckListBox.obj" \
	"$(INTDIR)\XTColorDialog.obj" \
	"$(INTDIR)\XTColorPageCustom.obj" \
	"$(INTDIR)\XTColorPageStandard.obj" \
	"$(INTDIR)\XTColorPicker.obj" \
	"$(INTDIR)\XTColorPopup.obj" \
	"$(INTDIR)\XTColorRef.obj" \
	"$(INTDIR)\XTColorSelectorCtrl.obj" \
	"$(INTDIR)\XTComboBoxEx.obj" \
	"$(INTDIR)\XTCommandsListBox.obj" \
	"$(INTDIR)\XTControlBar.obj" \
	"$(INTDIR)\XTCoolMenu.obj" \
	"$(INTDIR)\XTCrc32.obj" \
	"$(INTDIR)\XTCustomizeAPI.obj" \
	"$(INTDIR)\XTCustomizePage.obj" \
	"$(INTDIR)\XTCustomizeSheet.obj" \
	"$(INTDIR)\XTCustomToolBar.obj" \
	"$(INTDIR)\XTDateTimeCtrl.obj" \
	"$(INTDIR)\XTDialogBar.obj" \
	"$(INTDIR)\XTDockBar.obj" \
	"$(INTDIR)\XTDockColorSelector.obj" \
	"$(INTDIR)\XTDockContext.obj" \
	"$(INTDIR)\XTDockState.obj" \
	"$(INTDIR)\XTDockWindow.obj" \
	"$(INTDIR)\XTDropSource.obj" \
	"$(INTDIR)\XTEditListBox.obj" \
	"$(INTDIR)\XTExpButton.obj" \
	"$(INTDIR)\XTExpMenuWnd.obj" \
	"$(INTDIR)\XTFlatComboBox.obj" \
	"$(INTDIR)\XTFlatHeaderCtrl.obj" \
	"$(INTDIR)\XTFlatTabCtrl.obj" \
	"$(INTDIR)\XTFontCombo.obj" \
	"$(INTDIR)\XTFrameImpl.obj" \
	"$(INTDIR)\XTFrameWnd.obj" \
	"$(INTDIR)\XTGlobal.obj" \
	"$(INTDIR)\XTHelpers.obj" \
	"$(INTDIR)\XTHexEdit.obj" \
	"$(INTDIR)\XTHtmlView.obj" \
	"$(INTDIR)\XTHyperLink.obj" \
	"$(INTDIR)\XTIconMap.obj" \
	"$(INTDIR)\XTImageManager.obj" \
	"$(INTDIR)\XTListCtrl.obj" \
	"$(INTDIR)\XTListView.obj" \
	"$(INTDIR)\XTLoadSysColorBitmap.obj" \
	"$(INTDIR)\XTLogoPane.obj" \
	"$(INTDIR)\XTMaskEdit.obj" \
	"$(INTDIR)\XTMDIChildWnd.obj" \
	"$(INTDIR)\XTMDIFrameWnd.obj" \
	"$(INTDIR)\XTMDIWndTab.obj" \
	"$(INTDIR)\XTMemDC.obj" \
	"$(INTDIR)\XTMemFile.obj" \
	"$(INTDIR)\XTMenuBar.obj" \
	"$(INTDIR)\XTMenuBar1.obj" \
	"$(INTDIR)\XTMiniDockFrameWnd.obj" \
	"$(INTDIR)\XTNewToolbarDlg.obj" \
	"$(INTDIR)\XTOleIPFrameWnd.obj" \
	"$(INTDIR)\XTOptionsManager.obj" \
	"$(INTDIR)\XTOSVersionInfo.obj" \
	"$(INTDIR)\XTOutBarCtrl.obj" \
	"$(INTDIR)\XTOutlookBar.obj" \
	"$(INTDIR)\XTPagerCtrl.obj" \
	"$(INTDIR)\XTPopupTearOffWnd.obj" \
	"$(INTDIR)\XTPreviewView.obj" \
	"$(INTDIR)\XTReBar.obj" \
	"$(INTDIR)\XTReBar1.obj" \
	"$(INTDIR)\XTReBarCtrl.obj" \
	"$(INTDIR)\XTRegistryManager.obj" \
	"$(INTDIR)\XTResize.obj" \
	"$(INTDIR)\XTResizeDialog.obj" \
	"$(INTDIR)\XTResizeFormView.obj" \
	"$(INTDIR)\XTResizePoint.obj" \
	"$(INTDIR)\XTResizePropertyPage.obj" \
	"$(INTDIR)\XTResizePropertySheet.obj" \
	"$(INTDIR)\XTResizeRect.obj" \
	"$(INTDIR)\XTSearchOptionsCtrl.obj" \
	"$(INTDIR)\XTShellListCtrl.obj" \
	"$(INTDIR)\XTShellListView.obj" \
	"$(INTDIR)\XTShellPidl.obj" \
	"$(INTDIR)\XTShellSettings.obj" \
	"$(INTDIR)\XTShellTreeCtrl.obj" \
	"$(INTDIR)\XTShellTreeView.obj" \
	"$(INTDIR)\XTSkins.obj" \
	"$(INTDIR)\XTSortClass.obj" \
	"$(INTDIR)\XTSplitterDock.obj" \
	"$(INTDIR)\XTSplitterWnd.obj" \
	"$(INTDIR)\XTStatusBar.obj" \
	"$(INTDIR)\XTTabCtrl.obj" \
	"$(INTDIR)\XTTabCtrlBar.obj" \
	"$(INTDIR)\XTTabCtrlButtons.obj" \
	"$(INTDIR)\XTTabView.obj" \
	"$(INTDIR)\XTTipOfTheDay.obj" \
	"$(INTDIR)\XTTipWindow.obj" \
	"$(INTDIR)\XTToolBar.obj" \
	"$(INTDIR)\XTToolBar1.obj" \
	"$(INTDIR)\XTToolBarCtrl.obj" \
	"$(INTDIR)\XTToolBarPopupWnd.obj" \
	"$(INTDIR)\XTToolCmdUI.obj" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Lib_vc50\XT3100LibDynStaticud.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "XTToolkit_Lib - Win32 Release" || "$(CFG)" ==\
 "XTToolkit_Lib - Win32 Debug" || "$(CFG)" ==\
 "XTToolkit_Lib - Win32 Unicode Release" || "$(CFG)" ==\
 "XTToolkit_Lib - Win32 Unicode Debug" || "$(CFG)" ==\
 "XTToolkit_Lib - Win32 Dynamic Release" || "$(CFG)" ==\
 "XTToolkit_Lib - Win32 Dynamic Debug" || "$(CFG)" ==\
 "XTToolkit_Lib - Win32 Dynamic Unicode Release" || "$(CFG)" ==\
 "XTToolkit_Lib - Win32 Dynamic Unicode Debug"
SOURCE=..\Src\Common.Src\XTBrowseDialog.cpp
DEP_CPP_XTBRO=\
	"..\Include\Common.Include\XTBrowseDialog.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTBRO=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTBrowseDialog.obj" : $(SOURCE) $(DEP_CPP_XTBRO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTBrowseEdit.cpp
DEP_CPP_XTBROW=\
	"..\Include\Common.Include\XTBrowseDialog.h"\
	"..\Include\Common.Include\XTBrowseEdit.h"\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTBROW=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTBrowseEdit.obj" : $(SOURCE) $(DEP_CPP_XTBROW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTButton.cpp
DEP_CPP_XTBUT=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTBUT=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTButton.obj" : $(SOURCE) $(DEP_CPP_XTBUT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTCaption.cpp
DEP_CPP_XTCAP=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTCaption.h"\
	"..\Include\Common.Include\XTCaptionPopupWnd.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTCAP=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTCaption.obj" : $(SOURCE) $(DEP_CPP_XTCAP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTCaptionPopupWnd.cpp
DEP_CPP_XTCAPT=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTCaption.h"\
	"..\Include\Common.Include\XTCaptionPopupWnd.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTCAPT=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTCaptionPopupWnd.obj" : $(SOURCE) $(DEP_CPP_XTCAPT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTCheckListBox.cpp
DEP_CPP_XTCHE=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTCheckListBox.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTCHE=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTCheckListBox.obj" : $(SOURCE) $(DEP_CPP_XTCHE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTColorDialog.cpp
DEP_CPP_XTCOL=\
	"..\Include\Common.Include\XTColorDialog.h"\
	"..\Include\Common.Include\XTColorPageCustom.h"\
	"..\Include\Common.Include\XTColorPageStandard.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTCOL=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTColorDialog.obj" : $(SOURCE) $(DEP_CPP_XTCOL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTColorPageCustom.cpp
DEP_CPP_XTCOLO=\
	"..\Include\Common.Include\XTColorDialog.h"\
	"..\Include\Common.Include\XTColorPageCustom.h"\
	"..\Include\Common.Include\XTColorPageStandard.h"\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTCOLO=\
	"..\Include\XTPColorManager.h"\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTColorPageCustom.obj" : $(SOURCE) $(DEP_CPP_XTCOLO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTColorPageStandard.cpp
DEP_CPP_XTCOLOR=\
	"..\Include\Common.Include\XTColorDialog.h"\
	"..\Include\Common.Include\XTColorPageStandard.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTCOLOR=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTColorPageStandard.obj" : $(SOURCE) $(DEP_CPP_XTCOLOR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTColorPicker.cpp
DEP_CPP_XTCOLORP=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTColorPicker.h"\
	"..\Include\Common.Include\XTColorPopup.h"\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\Common.Include\XTColorSelectorCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTCOLORP=\
	"..\Include\XTPColorManager.h"\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTColorPicker.obj" : $(SOURCE) $(DEP_CPP_XTCOLORP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTColorPopup.cpp
DEP_CPP_XTCOLORPO=\
	"..\Include\Common.Include\XTColorDialog.h"\
	"..\Include\Common.Include\XTColorPageCustom.h"\
	"..\Include\Common.Include\XTColorPageStandard.h"\
	"..\Include\Common.Include\XTColorPopup.h"\
	"..\Include\Common.Include\XTColorSelectorCtrl.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTCOLORPO=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTColorPopup.obj" : $(SOURCE) $(DEP_CPP_XTCOLORPO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTColorRef.cpp
DEP_CPP_XTCOLORR=\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\XTDefines.h"\
	
NODEP_CPP_XTCOLORR=\
	"..\Include\XTPColorManager.h"\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTColorRef.obj" : $(SOURCE) $(DEP_CPP_XTCOLORR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTColorSelectorCtrl.cpp
DEP_CPP_XTCOLORS=\
	"..\Include\Common.Include\XTColorDialog.h"\
	"..\Include\Common.Include\XTColorSelectorCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTCOLORS=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTColorSelectorCtrl.obj" : $(SOURCE) $(DEP_CPP_XTCOLORS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTComboBoxEx.cpp
DEP_CPP_XTCOM=\
	"..\Include\Common.Include\XTComboBoxEx.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTCOM=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTComboBoxEx.obj" : $(SOURCE) $(DEP_CPP_XTCOM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTDateTimeCtrl.cpp
DEP_CPP_XTDAT=\
	"..\Include\Common.Include\XTDateTimeCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTDAT=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTDateTimeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTDAT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTDropSource.cpp
DEP_CPP_XTDRO=\
	"..\Include\Common.Include\XTDropSource.h"\
	
NODEP_CPP_XTDRO=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTDropSource.obj" : $(SOURCE) $(DEP_CPP_XTDRO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTEditListBox.cpp
DEP_CPP_XTEDI=\
	"..\Include\Common.Include\XTBrowseEdit.h"\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTEDI=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTEditListBox.obj" : $(SOURCE) $(DEP_CPP_XTEDI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTFlatComboBox.cpp
DEP_CPP_XTFLA=\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTSkins.h"\
	
NODEP_CPP_XTFLA=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTFlatComboBox.obj" : $(SOURCE) $(DEP_CPP_XTFLA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTFlatHeaderCtrl.cpp
DEP_CPP_XTFLAT=\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTFLAT=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTFlatHeaderCtrl.obj" : $(SOURCE) $(DEP_CPP_XTFLAT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTFlatTabCtrl.cpp
DEP_CPP_XTFLATT=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTFlatTabCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTFLATT=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTFlatTabCtrl.obj" : $(SOURCE) $(DEP_CPP_XTFLATT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTFontCombo.cpp
DEP_CPP_XTFON=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFontCombo.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTFON=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTFontCombo.obj" : $(SOURCE) $(DEP_CPP_XTFON) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTHexEdit.cpp
DEP_CPP_XTHEX=\
	"..\Include\Common.Include\XTHexEdit.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTHEX=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTHexEdit.obj" : $(SOURCE) $(DEP_CPP_XTHEX) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTHtmlView.cpp
DEP_CPP_XTHTM=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTHtmlView.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTHTM=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTHtmlView.obj" : $(SOURCE) $(DEP_CPP_XTHTM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTHyperLink.cpp
DEP_CPP_XTHYP=\
	"..\Include\Common.Include\XTHyperLink.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTHYP=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTHyperLink.obj" : $(SOURCE) $(DEP_CPP_XTHYP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTListCtrl.cpp
DEP_CPP_XTLIS=\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTLIS=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTListCtrl.obj" : $(SOURCE) $(DEP_CPP_XTLIS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTListView.cpp
DEP_CPP_XTLIST=\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTListView.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTLIST=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTListView.obj" : $(SOURCE) $(DEP_CPP_XTLIST) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTLoadSysColorBitmap.cpp
DEP_CPP_XTLOA=\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTLOA=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTLoadSysColorBitmap.obj" : $(SOURCE) $(DEP_CPP_XTLOA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTLogoPane.cpp
DEP_CPP_XTLOG=\
	"..\Include\Common.Include\XTLogoPane.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTLOG=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTLogoPane.obj" : $(SOURCE) $(DEP_CPP_XTLOG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTMaskEdit.cpp
DEP_CPP_XTMAS=\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTMaskEdit.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTMAS=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTMaskEdit.obj" : $(SOURCE) $(DEP_CPP_XTMAS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTMDIWndTab.cpp
DEP_CPP_XTMDI=\
	"..\Include\Common.Include\XTMDIWndTab.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTTabCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTTabCtrlButtons.h"\
	
NODEP_CPP_XTMDI=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTMDIWndTab.obj" : $(SOURCE) $(DEP_CPP_XTMDI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTMemDC.cpp
DEP_CPP_XTMEM=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTMEM=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTMemDC.obj" : $(SOURCE) $(DEP_CPP_XTMEM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTMemFile.cpp
DEP_CPP_XTMEMF=\
	"..\Include\Common.Include\XTMemFile.h"\
	
NODEP_CPP_XTMEMF=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTMemFile.obj" : $(SOURCE) $(DEP_CPP_XTMEMF) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTOSVersionInfo.cpp
DEP_CPP_XTOSV=\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTOSV=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTOSVersionInfo.obj" : $(SOURCE) $(DEP_CPP_XTOSV) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTOutBarCtrl.cpp
DEP_CPP_XTOUT=\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTOutBarCtrl.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTOUT=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTOutBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTOUT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTOutlookBar.cpp
DEP_CPP_XTOUTL=\
	"..\Include\Common.Include\XTOutlookBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTOUTL=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTOutlookBar.obj" : $(SOURCE) $(DEP_CPP_XTOUTL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTPagerCtrl.cpp
DEP_CPP_XTPAG=\
	"..\Include\Common.Include\XTPagerCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTPAG=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTPagerCtrl.obj" : $(SOURCE) $(DEP_CPP_XTPAG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTRegistryManager.cpp
DEP_CPP_XTREG=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTREG=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTRegistryManager.obj" : $(SOURCE) $(DEP_CPP_XTREG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTResize.cpp
DEP_CPP_XTRES=\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRES=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTResize.obj" : $(SOURCE) $(DEP_CPP_XTRES) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTResizeDialog.cpp
DEP_CPP_XTRESI=\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRESI=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTResizeDialog.obj" : $(SOURCE) $(DEP_CPP_XTRESI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTResizeFormView.cpp
DEP_CPP_XTRESIZ=\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRESIZ=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTResizeFormView.obj" : $(SOURCE) $(DEP_CPP_XTRESIZ) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTResizePoint.cpp
DEP_CPP_XTRESIZE=\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRESIZE=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTResizePoint.obj" : $(SOURCE) $(DEP_CPP_XTRESIZE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTResizePropertyPage.cpp
DEP_CPP_XTRESIZEP=\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRESIZEP=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTResizePropertyPage.obj" : $(SOURCE) $(DEP_CPP_XTRESIZEP)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTResizePropertySheet.cpp
DEP_CPP_XTRESIZEPR=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRESIZEPR=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTResizePropertySheet.obj" : $(SOURCE) $(DEP_CPP_XTRESIZEPR)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTResizeRect.cpp
DEP_CPP_XTRESIZER=\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRESIZER=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTResizeRect.obj" : $(SOURCE) $(DEP_CPP_XTRESIZER) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTSearchOptionsCtrl.cpp
DEP_CPP_XTSEA=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTSearchOptionsCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSEA=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTSearchOptionsCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSEA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTShellListCtrl.cpp
DEP_CPP_XTSHE=\
	"..\Include\Common.Include\XTDropSource.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTShellListCtrl.h"\
	"..\Include\Common.Include\XTShellPidl.h"\
	"..\Include\Common.Include\XTShellSettings.h"\
	"..\Include\Common.Include\XTSortClass.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTSHE=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTShellListCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSHE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTShellListView.cpp
DEP_CPP_XTSHEL=\
	"..\Include\Common.Include\XTDropSource.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTListView.h"\
	"..\Include\Common.Include\XTShellListView.h"\
	"..\Include\Common.Include\XTShellPidl.h"\
	"..\Include\Common.Include\XTShellSettings.h"\
	"..\Include\Common.Include\XTSortClass.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTSHEL=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTShellListView.obj" : $(SOURCE) $(DEP_CPP_XTSHEL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTShellPidl.cpp
DEP_CPP_XTSHELL=\
	"..\Include\Common.Include\XTShellPidl.h"\
	
NODEP_CPP_XTSHELL=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTShellPidl.obj" : $(SOURCE) $(DEP_CPP_XTSHELL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTShellSettings.cpp
DEP_CPP_XTSHELLS=\
	"..\Include\Common.Include\XTShellSettings.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSHELLS=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTShellSettings.obj" : $(SOURCE) $(DEP_CPP_XTSHELLS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTShellTreeCtrl.cpp
DEP_CPP_XTSHELLT=\
	"..\Include\Common.Include\XTComboBoxEx.h"\
	"..\Include\Common.Include\XTDropSource.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTShellPidl.h"\
	"..\Include\Common.Include\XTShellSettings.h"\
	"..\Include\Common.Include\XTShellTreeCtrl.h"\
	"..\Include\Common.Include\XTTreeCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTSHELLT=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTShellTreeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSHELLT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTShellTreeView.cpp
DEP_CPP_XTSHELLTR=\
	"..\Include\Common.Include\XTComboBoxEx.h"\
	"..\Include\Common.Include\XTDropSource.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTShellPidl.h"\
	"..\Include\Common.Include\XTShellSettings.h"\
	"..\Include\Common.Include\XTShellTreeView.h"\
	"..\Include\Common.Include\XTTreeView.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTSHELLTR=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTShellTreeView.obj" : $(SOURCE) $(DEP_CPP_XTSHELLTR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTSortClass.cpp
DEP_CPP_XTSOR=\
	"..\Include\Common.Include\XTSortClass.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSOR=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTSortClass.obj" : $(SOURCE) $(DEP_CPP_XTSOR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTSplitterWnd.cpp
DEP_CPP_XTSPL=\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTSplitterWnd.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSPL=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTSplitterWnd.obj" : $(SOURCE) $(DEP_CPP_XTSPL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTStatusBar.cpp
DEP_CPP_XTSTA=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTStatusBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSTA=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTStatusBar.obj" : $(SOURCE) $(DEP_CPP_XTSTA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTTabCtrl.cpp
DEP_CPP_XTTAB=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTTabCtrl.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTFrameWnd.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMDIFrameWnd.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolsManager.h"\
	"..\Src\Common.Src\XTTabCtrlButtons.h"\
	
NODEP_CPP_XTTAB=\
	"..\Include\XTPColorManager.h"\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTTabCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTAB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTTabCtrlButtons.cpp
DEP_CPP_XTTABC=\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\Common.Include\XTTabCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTTabCtrlButtons.h"\
	"..\Src\XTDrawHelpers.h"\
	
NODEP_CPP_XTTABC=\
	"..\Include\XTPColorManager.h"\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTTabCtrlButtons.obj" : $(SOURCE) $(DEP_CPP_XTTABC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTTabView.cpp
DEP_CPP_XTTABV=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTTabCtrl.h"\
	"..\Include\Common.Include\XTTabView.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTTabCtrlButtons.h"\
	
NODEP_CPP_XTTABV=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTTabView.obj" : $(SOURCE) $(DEP_CPP_XTTABV) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTTipOfTheDay.cpp
DEP_CPP_XTTIP=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTTipOfTheDay.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTTIP=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTTipOfTheDay.obj" : $(SOURCE) $(DEP_CPP_XTTIP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTTipWindow.cpp
DEP_CPP_XTTIPW=\
	"..\Include\Common.Include\XTTipWindow.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTTIPW=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTTipWindow.obj" : $(SOURCE) $(DEP_CPP_XTTIPW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTTrayIcon.cpp
DEP_CPP_XTTRA=\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTTrayIcon.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTTRA=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTTrayIcon.obj" : $(SOURCE) $(DEP_CPP_XTTRA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTTreeCtrl.cpp
DEP_CPP_XTTRE=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTTreeCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTTRE=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTTreeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTRE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTTreeView.cpp
DEP_CPP_XTTREE=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTTreeView.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTTREE=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTTreeView.obj" : $(SOURCE) $(DEP_CPP_XTTREE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTWindowList.cpp
DEP_CPP_XTWIN=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMDIFrameWnd.h"\
	"..\Include\XTMenuBar.h"\
	"..\Src\Common.Src\XTWindowList.h"\
	
NODEP_CPP_XTWIN=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTWindowList.obj" : $(SOURCE) $(DEP_CPP_XTWIN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTWindowPos.cpp
DEP_CPP_XTWIND=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWindowPos.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTWIND=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTWindowPos.obj" : $(SOURCE) $(DEP_CPP_XTWIND) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTWndHook.cpp
DEP_CPP_XTWND=\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	
NODEP_CPP_XTWND=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTWndHook.obj" : $(SOURCE) $(DEP_CPP_XTWND) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\Common.Src\XTWndShadow.cpp
DEP_CPP_XTWNDS=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTDrawHelpers.h"\
	
NODEP_CPP_XTWNDS=\
	"..\Src\Common.Src\StdAfx.h"\
	

"$(INTDIR)\XTWndShadow.obj" : $(SOURCE) $(DEP_CPP_XTWNDS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	{$(INCLUDE)}"..\src\afximpl.h"\
	

!IF  "$(CFG)" == "XTToolkit_Lib - Win32 Release"

CPP_SWITCHES=/nologo /MT /W4 /WX /GX /O2 /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK" /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch"\
 /Yc"StdAfx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Lib_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W4 /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D\
 "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK" /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch"\
 /Yc"StdAfx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Lib_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MT /W4 /WX /GX /O2 /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D\
 "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yc"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Lib_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MTd /W4 /WX /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D\
 "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yc"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Lib_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Release"

CPP_SWITCHES=/nologo /MD /W4 /WX /GX /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D\
 "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yc"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Lib_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Debug"

CPP_SWITCHES=/nologo /MDd /W4 /WX /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D\
 "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yc"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Lib_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Unicode Release"

CPP_SWITCHES=/nologo /MD /W4 /WX /GX /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D\
 "_UNICODE" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yc"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Lib_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W4 /WX /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D\
 "_UNICODE" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB"\
 /Fp"$(INTDIR)\XTToolkit_Lib_vc50.pch" /Yc"StdAfx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Lib_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=..\Src\XTAccelKeyEdit.cpp
DEP_CPP_XTACC=\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTACC=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTAccelKeyEdit.obj" : $(SOURCE) $(DEP_CPP_XTACC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTAccelManager.cpp
DEP_CPP_XTACCE=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTFrameWnd.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMDIChildWnd.h"\
	"..\Include\XTMDIFrameWnd.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTOleIPFrameWnd.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTToolsManager.h"\
	
NODEP_CPP_XTACCE=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTAccelManager.obj" : $(SOURCE) $(DEP_CPP_XTACCE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTAnimationMemDC.cpp
DEP_CPP_XTANI=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTAnimationMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTANI=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTAnimationMemDC.obj" : $(SOURCE) $(DEP_CPP_XTANI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTCBarDialog.cpp
DEP_CPP_XTCBA=\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTStatusBar.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCBarDialog.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMenuBar.h"\
	
NODEP_CPP_XTCBA=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTCBarDialog.obj" : $(SOURCE) $(DEP_CPP_XTCBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTCommandsListBox.cpp
DEP_CPP_XTCOMM=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTCommandsListBox.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTSkins.h"\
	
NODEP_CPP_XTCOMM=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTCommandsListBox.obj" : $(SOURCE) $(DEP_CPP_XTCOMM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTControlBar.cpp
DEP_CPP_XTCON=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockContext.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTReBar.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTCON=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTControlBar.obj" : $(SOURCE) $(DEP_CPP_XTCON) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTCoolMenu.cpp
DEP_CPP_XTCOO=\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTAnimationMemDC.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTToolsManager.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTDrawHelpers.h"\
	"..\Src\XTImageManager.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTCOO=\
	"..\Include\XTPColorManager.h"\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTCoolMenu.obj" : $(SOURCE) $(DEP_CPP_XTCOO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTCrc32.cpp
DEP_CPP_XTCRC=\
	"..\Src\XTCrc32.h"\
	
NODEP_CPP_XTCRC=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTCrc32.obj" : $(SOURCE) $(DEP_CPP_XTCRC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTCustomizeAPI.cpp
DEP_CPP_XTCUS=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTCustomToolbar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Src\XTCrc32.h"\
	"..\Src\XTRegistryBlob.h"\
	
NODEP_CPP_XTCUS=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTCustomizeAPI.obj" : $(SOURCE) $(DEP_CPP_XTCUS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTCustomizePage.cpp
DEP_CPP_XTCUST=\
	"..\Include\Common.Include\XTBrowseEdit.h"\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTCheckListBox.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTTipWindow.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTCommandsListBox.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTCustomizePage.h"\
	"..\Include\XTCustomizeSheet.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMDIFrameWnd.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTNewToolbarDlg.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolsManager.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTCUST=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTCustomizePage.obj" : $(SOURCE) $(DEP_CPP_XTCUST) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTCustomizeSheet.cpp
DEP_CPP_XTCUSTO=\
	"..\Include\Common.Include\XTBrowseEdit.h"\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTCheckListBox.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTTipWindow.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTCommandsListBox.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTCustomizePage.h"\
	"..\Include\XTCustomizeSheet.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolsManager.h"\
	
NODEP_CPP_XTCUSTO=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTCustomizeSheet.obj" : $(SOURCE) $(DEP_CPP_XTCUSTO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTCustomToolBar.cpp
DEP_CPP_XTCUSTOM=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTCustomToolbar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	
NODEP_CPP_XTCUSTOM=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTCustomToolBar.obj" : $(SOURCE) $(DEP_CPP_XTCUSTOM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTDialogBar.cpp
DEP_CPP_XTDIA=\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDialogBar.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTDIA=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTDialogBar.obj" : $(SOURCE) $(DEP_CPP_XTDIA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTDockBar.cpp
DEP_CPP_XTDOC=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTReBar.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTDOC=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTDockBar.obj" : $(SOURCE) $(DEP_CPP_XTDOC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTDockColorSelector.cpp
DEP_CPP_XTDOCK=\
	"..\Include\Common.Include\XTColorSelectorCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockColorSelector.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarPopupWnd.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTDOCK=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTDockColorSelector.obj" : $(SOURCE) $(DEP_CPP_XTDOCK) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTDockContext.cpp
DEP_CPP_XTDOCKC=\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockContext.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMiniDockFrameWnd.h"\
	"..\Include\XTSplitterDock.h"\
	
NODEP_CPP_XTDOCKC=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTDockContext.obj" : $(SOURCE) $(DEP_CPP_XTDOCKC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTDockState.cpp
DEP_CPP_XTDOCKS=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDockState.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTDOCKS=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTDockState.obj" : $(SOURCE) $(DEP_CPP_XTDOCKS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTDockWindow.cpp
DEP_CPP_XTDOCKW=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTTabCtrl.h"\
	"..\Include\Common.Include\XTTabCtrlBar.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockContext.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\Common.Src\XTTabCtrlButtons.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTDOCKW=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTDockWindow.obj" : $(SOURCE) $(DEP_CPP_XTDOCKW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTExpButton.cpp
DEP_CPP_XTEXP=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTExpButton.h"\
	"..\Src\XTExpMenuWnd.h"\
	
NODEP_CPP_XTEXP=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTExpButton.obj" : $(SOURCE) $(DEP_CPP_XTEXP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTExpMenuWnd.cpp
DEP_CPP_XTEXPM=\
	"..\Include\Common.Include\XTColorPopup.h"\
	"..\Include\Common.Include\XTColorSelectorCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTExpMenuWnd.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTEXPM=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTExpMenuWnd.obj" : $(SOURCE) $(DEP_CPP_XTEXPM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTFrameImpl.cpp
DEP_CPP_XTFRA=\
	"..\Include\Common.Include\XTBrowseEdit.h"\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTCheckListBox.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTTipWindow.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTCommandsListBox.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTCustomizePage.h"\
	"..\Include\XTCustomizeSheet.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockState.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTMiniDockFrameWnd.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolsManager.h"\
	
NODEP_CPP_XTFRA=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTFrameImpl.obj" : $(SOURCE) $(DEP_CPP_XTFRA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTFrameWnd.cpp
DEP_CPP_XTFRAM=\
	"..\Include\Common.Include\XTBrowseEdit.h"\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTCheckListBox.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTTipWindow.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTCommandsListBox.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTCustomizePage.h"\
	"..\Include\XTCustomizeSheet.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockState.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTFrameWnd.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTMiniDockFrameWnd.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolsManager.h"\
	
NODEP_CPP_XTFRAM=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTFRAM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTGlobal.cpp
DEP_CPP_XTGLO=\
	"..\Include\Common.Include\XTColorPageCustom.h"\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTVersion.h"\
	"..\Src\XTPriv.h"\
	{$(INCLUDE)}"shlwapi.h"\
	
NODEP_CPP_XTGLO=\
	"..\Include\XTPColorManager.h"\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTGlobal.obj" : $(SOURCE) $(DEP_CPP_XTGLO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTHelpers.cpp
DEP_CPP_XTHEL=\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTHEL=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTHelpers.obj" : $(SOURCE) $(DEP_CPP_XTHEL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTIconMap.cpp
DEP_CPP_XTICO=\
	"..\Include\XTIconMap.h"\
	
NODEP_CPP_XTICO=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTIconMap.obj" : $(SOURCE) $(DEP_CPP_XTICO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTImageManager.cpp
DEP_CPP_XTIMA=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Src\XTDrawHelpers.h"\
	"..\Src\XTImageManager.h"\
	
NODEP_CPP_XTIMA=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTImageManager.obj" : $(SOURCE) $(DEP_CPP_XTIMA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTMDIChildWnd.cpp
DEP_CPP_XTMDIC=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMDIChildWnd.h"\
	"..\Include\XTMDIFrameWnd.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTMiniDockFrameWnd.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolsManager.h"\
	
NODEP_CPP_XTMDIC=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTMDIChildWnd.obj" : $(SOURCE) $(DEP_CPP_XTMDIC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTMDIFrameWnd.cpp
DEP_CPP_XTMDIF=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockState.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMDIChildWnd.h"\
	"..\Include\XTMDIFrameWnd.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTMiniDockFrameWnd.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolsManager.h"\
	"..\Src\Common.Src\XTWindowList.h"\
	
NODEP_CPP_XTMDIF=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTMDIFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTMDIF) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTMenuBar.cpp
DEP_CPP_XTMEN=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTMiniDockFrameWnd.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolsManager.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTPriv.h"\
	{$(INCLUDE)}"multimon.h"\
	
NODEP_CPP_XTMEN=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTMenuBar.obj" : $(SOURCE) $(DEP_CPP_XTMEN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTMenuBar1.cpp
DEP_CPP_XTMENU=\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMenuBar.h"\
	"..\Src\XTCrc32.h"\
	
NODEP_CPP_XTMENU=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTMenuBar1.obj" : $(SOURCE) $(DEP_CPP_XTMENU) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTMiniDockFrameWnd.cpp
DEP_CPP_XTMIN=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockContext.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTMiniDockFrameWnd.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolBar.h"\
	"..\Src\XTExpMenuWnd.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTMIN=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTMiniDockFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTMIN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTNewToolbarDlg.cpp
DEP_CPP_XTNEW=\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTNewToolbarDlg.h"\
	
NODEP_CPP_XTNEW=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTNewToolbarDlg.obj" : $(SOURCE) $(DEP_CPP_XTNEW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTOleIPFrameWnd.cpp
DEP_CPP_XTOLE=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockState.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTMiniDockFrameWnd.h"\
	"..\Include\XTOleIPFrameWnd.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTSplitterDock.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolsManager.h"\
	
NODEP_CPP_XTOLE=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTOleIPFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTOLE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTOptionsManager.cpp
DEP_CPP_XTOPT=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolsManager.h"\
	
NODEP_CPP_XTOPT=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTOptionsManager.obj" : $(SOURCE) $(DEP_CPP_XTOPT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTPopupTearOffWnd.cpp
DEP_CPP_XTPOP=\
	"..\Include\Common.Include\XTColorSelectorCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockContext.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTPopupTearOffWnd.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolBarPopupWnd.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTDrawHelpers.h"\
	
NODEP_CPP_XTPOP=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTPopupTearOffWnd.obj" : $(SOURCE) $(DEP_CPP_XTPOP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTPreviewView.cpp
DEP_CPP_XTPRE=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTPreviewView.h"\
	"..\Include\XTToolBar.h"\
	"..\Src\XTImageManager.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTPRE=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTPreviewView.obj" : $(SOURCE) $(DEP_CPP_XTPRE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTReBar.cpp
DEP_CPP_XTREB=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTReBar.h"\
	"..\Include\XTReBarCtrl.h"\
	"..\Include\XTToolBar.h"\
	"..\Src\XTReBarBandInfo.h"\
	
NODEP_CPP_XTREB=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTReBar.obj" : $(SOURCE) $(DEP_CPP_XTREB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTReBar1.cpp
DEP_CPP_XTREBA=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTCustomToolbar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTReBar.h"\
	"..\Include\XTReBarCtrl.h"\
	"..\Include\XTToolBar.h"\
	"..\Src\XTReBarBandInfo.h"\
	
NODEP_CPP_XTREBA=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTReBar1.obj" : $(SOURCE) $(DEP_CPP_XTREBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTReBarCtrl.cpp
DEP_CPP_XTREBAR=\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTReBarCtrl.h"\
	
NODEP_CPP_XTREBAR=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTReBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTREBAR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTSkins.cpp
DEP_CPP_XTSKI=\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTCommandsListBox.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTSkins.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTImageManager.h"\
	
NODEP_CPP_XTSKI=\
	"..\Include\XTPColorManager.h"\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTSkins.obj" : $(SOURCE) $(DEP_CPP_XTSKI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTSplitterDock.cpp
DEP_CPP_XTSPLI=\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockBar.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTSplitterDock.h"\
	
NODEP_CPP_XTSPLI=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTSplitterDock.obj" : $(SOURCE) $(DEP_CPP_XTSPLI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTTabCtrlBar.cpp
DEP_CPP_XTTABCT=\
	"..\Include\Common.Include\XTTabCtrl.h"\
	"..\Include\Common.Include\XTTabCtrlBar.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockWindow.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTTabCtrlButtons.h"\
	
NODEP_CPP_XTTABCT=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTTabCtrlBar.obj" : $(SOURCE) $(DEP_CPP_XTTABCT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTToolBar.cpp
DEP_CPP_XTTOO=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTColorPopup.h"\
	"..\Include\Common.Include\XTColorSelectorCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTSkins.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	"..\Src\XTExpButton.h"\
	"..\Src\XTImageManager.h"\
	"..\Src\XTPriv.h"\
	"..\Src\XTToolbarHelpers.h"\
	"..\Src\XTToolCmdUI.h"\
	
NODEP_CPP_XTTOO=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTToolBar.obj" : $(SOURCE) $(DEP_CPP_XTTOO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTToolBar1.cpp
DEP_CPP_XTTOOL=\
	"..\Include\Common.Include\XTBrowseEdit.h"\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTCheckListBox.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFlatComboBox.h"\
	"..\Include\Common.Include\XTFlatHeaderCtrl.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTListCtrl.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizeDialog.h"\
	"..\Include\Common.Include\XTResizeFormView.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizePropertyPage.h"\
	"..\Include\Common.Include\XTResizePropertySheet.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\Common.Include\XTTipWindow.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTAccelKeyEdit.h"\
	"..\Include\XTAccelManager.h"\
	"..\Include\XTCallbacks.h"\
	"..\Include\XTCommandsListBox.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTCustomizePage.h"\
	"..\Include\XTCustomizeSheet.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTDockContext.h"\
	"..\Include\XTFrameImpl.h"\
	"..\Include\XTFrameWnd.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMDIChildWnd.h"\
	"..\Include\XTMDIFrameWnd.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTOleIPFrameWnd.h"\
	"..\Include\XTOptionsManager.h"\
	"..\Include\XTSkins.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolsManager.h"\
	"..\Src\XTCrc32.h"\
	"..\Src\XTImageManager.h"\
	"..\Src\XTPriv.h"\
	"..\Src\XTRegistryBlob.h"\
	"..\Src\XTToolbarHelpers.h"\
	
NODEP_CPP_XTTOOL=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTToolBar1.obj" : $(SOURCE) $(DEP_CPP_XTTOOL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTToolBarCtrl.cpp
DEP_CPP_XTTOOLB=\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBarCtrl.h"\
	
NODEP_CPP_XTTOOLB=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTToolBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTOOLB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTToolBarPopupWnd.cpp
DEP_CPP_XTTOOLBA=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\Common.Include\XTWndShadow.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Include\XTToolBarCtrl.h"\
	"..\Include\XTToolBarPopupWnd.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTTOOLBA=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTToolBarPopupWnd.obj" : $(SOURCE) $(DEP_CPP_XTTOOLBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTToolCmdUI.cpp
DEP_CPP_XTTOOLC=\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCustomizeAPI.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBar.h"\
	"..\Src\XTToolCmdUI.h"\
	
NODEP_CPP_XTTOOLC=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTToolCmdUI.obj" : $(SOURCE) $(DEP_CPP_XTTOOLC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


SOURCE=..\Src\XTToolsManager.cpp
DEP_CPP_XTTOOLS=\
	"..\Include\Common.Include\XTButton.h"\
	"..\Include\Common.Include\XTEditListBox.h"\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTControlBar.h"\
	"..\Include\XTCoolMenu.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDelegate.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTIconMap.h"\
	"..\Include\XTMenuBar.h"\
	"..\Include\XTToolsManager.h"\
	
NODEP_CPP_XTTOOLS=\
	"..\Src\stdafx.h"\
	

"$(INTDIR)\XTToolsManager.obj" : $(SOURCE) $(DEP_CPP_XTTOOLS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Lib_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)



!ENDIF 

