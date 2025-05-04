# Microsoft Developer Studio Generated NMAKE File, Based on XTToolkit_Dll_vc50.dsp
!IF "$(CFG)" == ""
CFG=XTToolkit_Dll - Win32 Unicode Debug
!MESSAGE No configuration specified. Defaulting to XTToolkit_Dll - Win32\
 Unicode Debug.
!ENDIF 

!IF "$(CFG)" != "XTToolkit_Dll - Win32 Release" && "$(CFG)" !=\
 "XTToolkit_Dll - Win32 Debug" && "$(CFG)" !=\
 "XTToolkit_Dll - Win32 Unicode Release" && "$(CFG)" !=\
 "XTToolkit_Dll - Win32 Unicode Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XTToolkit_Dll_vc50.mak"\
 CFG="XTToolkit_Dll - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XTToolkit_Dll - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XTToolkit_Dll - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XTToolkit_Dll - Win32 Unicode Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XTToolkit_Dll - Win32 Unicode Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin_vc50\XT3100Lib.dll"

!ELSE 

ALL : "..\Bin_vc50\XT3100Lib.dll"

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
	-@erase "$(INTDIR)\XTToolkit_Dll.obj"
	-@erase "$(INTDIR)\XTToolkit_Dll.res"
	-@erase "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Bin_vc50\XT3100Lib.dll"
	-@erase "..\Lib_vc50\XT3100Lib.exp"
	-@erase "..\Lib_vc50\XT3100Lib.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /WX /GX /O2 /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT"\
 /Fp"$(INTDIR)\XTToolkit_Dll_vc50.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\XTToolkit_Dll.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Dll_vc50.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /base:"0x67800000" /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\XT3100Lib.pdb" /machine:I386 /def:".\XTToolkit_Dll.def"\
 /out:"../Bin_vc50/XT3100Lib.dll" /implib:"../Lib_vc50/XT3100Lib.lib" 
DEF_FILE= \
	".\XTToolkit_Dll.def"
LINK32_OBJS= \
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
	"$(INTDIR)\XTToolkit_Dll.obj" \
	"$(INTDIR)\XTToolkit_Dll.res" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Bin_vc50\XT3100Lib.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin_vc50\XT3100Libd.dll"

!ELSE 

ALL : "..\Bin_vc50\XT3100Libd.dll"

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
	-@erase "$(INTDIR)\XTToolkit_Dll.obj"
	-@erase "$(INTDIR)\XTToolkit_Dll.res"
	-@erase "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "$(OUTDIR)\XT3100Libd.pdb"
	-@erase "..\Bin_vc50\XT3100Libd.dll"
	-@erase "..\Bin_vc50\XT3100Libd.ilk"
	-@erase "..\Lib_vc50\XT3100Libd.exp"
	-@erase "..\Lib_vc50\XT3100Libd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W4 /GX /Zi /Od /I "..\src" /I "..\src\Common.src" /I\
 "..\include" /I "..\include\Common.include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT"\
 /Fp"$(INTDIR)\XTToolkit_Dll_vc50.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\XTToolkit_Dll.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Dll_vc50.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /base:"0x67800000" /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)\XT3100Libd.pdb" /debug /machine:I386\
 /def:".\XTToolkit_Dll.def" /out:"../Bin_vc50/XT3100Libd.dll"\
 /implib:"../Lib_vc50/XT3100Libd.lib" /pdbtype:sept 
DEF_FILE= \
	".\XTToolkit_Dll.def"
LINK32_OBJS= \
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
	"$(INTDIR)\XTToolkit_Dll.obj" \
	"$(INTDIR)\XTToolkit_Dll.res" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Bin_vc50\XT3100Libd.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"

OUTDIR=.\Unicode_Release
INTDIR=.\Unicode_Release

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin_vc50\XT3100Libu.dll"

!ELSE 

ALL : "..\Bin_vc50\XT3100Libu.dll"

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
	-@erase "$(INTDIR)\XTToolkit_Dll.obj"
	-@erase "$(INTDIR)\XTToolkit_Dll.res"
	-@erase "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "..\Bin_vc50\XT3100Libu.dll"
	-@erase "..\Lib_vc50\XT3100Libu.exp"
	-@erase "..\Lib_vc50\XT3100Libu.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W4 /WX /GX /O2 /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE"\
 /Fp"$(INTDIR)\XTToolkit_Dll_vc50.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
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

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\XTToolkit_Dll.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Dll_vc50.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /base:"0x67800000" /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)\XT3100Libu.pdb" /machine:I386 /def:".\XTToolkit_Dll.def"\
 /out:"../Bin_vc50/XT3100Libu.dll" /implib:"../Lib_vc50/XT3100Libu.lib" 
DEF_FILE= \
	".\XTToolkit_Dll.def"
LINK32_OBJS= \
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
	"$(INTDIR)\XTToolkit_Dll.obj" \
	"$(INTDIR)\XTToolkit_Dll.res" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Bin_vc50\XT3100Libu.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"

OUTDIR=.\Unicode_Debug
INTDIR=.\Unicode_Debug
# Begin Custom Macros
OutDir=.\Unicode_Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "..\Bin_vc50\XT3100Libud.dll" "$(OUTDIR)\XTToolkit_Dll_vc50.bsc"

!ELSE 

ALL : "..\Bin_vc50\XT3100Libud.dll" "$(OUTDIR)\XTToolkit_Dll_vc50.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\XTAccelKeyEdit.obj"
	-@erase "$(INTDIR)\XTAccelKeyEdit.sbr"
	-@erase "$(INTDIR)\XTAccelManager.obj"
	-@erase "$(INTDIR)\XTAccelManager.sbr"
	-@erase "$(INTDIR)\XTAnimationMemDC.obj"
	-@erase "$(INTDIR)\XTAnimationMemDC.sbr"
	-@erase "$(INTDIR)\XTBrowseDialog.obj"
	-@erase "$(INTDIR)\XTBrowseDialog.sbr"
	-@erase "$(INTDIR)\XTBrowseEdit.obj"
	-@erase "$(INTDIR)\XTBrowseEdit.sbr"
	-@erase "$(INTDIR)\XTButton.obj"
	-@erase "$(INTDIR)\XTButton.sbr"
	-@erase "$(INTDIR)\XTCaption.obj"
	-@erase "$(INTDIR)\XTCaption.sbr"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.obj"
	-@erase "$(INTDIR)\XTCaptionPopupWnd.sbr"
	-@erase "$(INTDIR)\XTCBarDialog.obj"
	-@erase "$(INTDIR)\XTCBarDialog.sbr"
	-@erase "$(INTDIR)\XTCheckListBox.obj"
	-@erase "$(INTDIR)\XTCheckListBox.sbr"
	-@erase "$(INTDIR)\XTColorDialog.obj"
	-@erase "$(INTDIR)\XTColorDialog.sbr"
	-@erase "$(INTDIR)\XTColorPageCustom.obj"
	-@erase "$(INTDIR)\XTColorPageCustom.sbr"
	-@erase "$(INTDIR)\XTColorPageStandard.obj"
	-@erase "$(INTDIR)\XTColorPageStandard.sbr"
	-@erase "$(INTDIR)\XTColorPicker.obj"
	-@erase "$(INTDIR)\XTColorPicker.sbr"
	-@erase "$(INTDIR)\XTColorPopup.obj"
	-@erase "$(INTDIR)\XTColorPopup.sbr"
	-@erase "$(INTDIR)\XTColorRef.obj"
	-@erase "$(INTDIR)\XTColorRef.sbr"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.obj"
	-@erase "$(INTDIR)\XTColorSelectorCtrl.sbr"
	-@erase "$(INTDIR)\XTComboBoxEx.obj"
	-@erase "$(INTDIR)\XTComboBoxEx.sbr"
	-@erase "$(INTDIR)\XTCommandsListBox.obj"
	-@erase "$(INTDIR)\XTCommandsListBox.sbr"
	-@erase "$(INTDIR)\XTControlBar.obj"
	-@erase "$(INTDIR)\XTControlBar.sbr"
	-@erase "$(INTDIR)\XTCoolMenu.obj"
	-@erase "$(INTDIR)\XTCoolMenu.sbr"
	-@erase "$(INTDIR)\XTCrc32.obj"
	-@erase "$(INTDIR)\XTCrc32.sbr"
	-@erase "$(INTDIR)\XTCustomizeAPI.obj"
	-@erase "$(INTDIR)\XTCustomizeAPI.sbr"
	-@erase "$(INTDIR)\XTCustomizePage.obj"
	-@erase "$(INTDIR)\XTCustomizePage.sbr"
	-@erase "$(INTDIR)\XTCustomizeSheet.obj"
	-@erase "$(INTDIR)\XTCustomizeSheet.sbr"
	-@erase "$(INTDIR)\XTCustomToolBar.obj"
	-@erase "$(INTDIR)\XTCustomToolBar.sbr"
	-@erase "$(INTDIR)\XTDateTimeCtrl.obj"
	-@erase "$(INTDIR)\XTDateTimeCtrl.sbr"
	-@erase "$(INTDIR)\XTDialogBar.obj"
	-@erase "$(INTDIR)\XTDialogBar.sbr"
	-@erase "$(INTDIR)\XTDockBar.obj"
	-@erase "$(INTDIR)\XTDockBar.sbr"
	-@erase "$(INTDIR)\XTDockColorSelector.obj"
	-@erase "$(INTDIR)\XTDockColorSelector.sbr"
	-@erase "$(INTDIR)\XTDockContext.obj"
	-@erase "$(INTDIR)\XTDockContext.sbr"
	-@erase "$(INTDIR)\XTDockState.obj"
	-@erase "$(INTDIR)\XTDockState.sbr"
	-@erase "$(INTDIR)\XTDockWindow.obj"
	-@erase "$(INTDIR)\XTDockWindow.sbr"
	-@erase "$(INTDIR)\XTDropSource.obj"
	-@erase "$(INTDIR)\XTDropSource.sbr"
	-@erase "$(INTDIR)\XTEditListBox.obj"
	-@erase "$(INTDIR)\XTEditListBox.sbr"
	-@erase "$(INTDIR)\XTExpButton.obj"
	-@erase "$(INTDIR)\XTExpButton.sbr"
	-@erase "$(INTDIR)\XTExpMenuWnd.obj"
	-@erase "$(INTDIR)\XTExpMenuWnd.sbr"
	-@erase "$(INTDIR)\XTFlatComboBox.obj"
	-@erase "$(INTDIR)\XTFlatComboBox.sbr"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.obj"
	-@erase "$(INTDIR)\XTFlatHeaderCtrl.sbr"
	-@erase "$(INTDIR)\XTFlatTabCtrl.obj"
	-@erase "$(INTDIR)\XTFlatTabCtrl.sbr"
	-@erase "$(INTDIR)\XTFontCombo.obj"
	-@erase "$(INTDIR)\XTFontCombo.sbr"
	-@erase "$(INTDIR)\XTFrameImpl.obj"
	-@erase "$(INTDIR)\XTFrameImpl.sbr"
	-@erase "$(INTDIR)\XTFrameWnd.obj"
	-@erase "$(INTDIR)\XTFrameWnd.sbr"
	-@erase "$(INTDIR)\XTGlobal.obj"
	-@erase "$(INTDIR)\XTGlobal.sbr"
	-@erase "$(INTDIR)\XTHelpers.obj"
	-@erase "$(INTDIR)\XTHelpers.sbr"
	-@erase "$(INTDIR)\XTHexEdit.obj"
	-@erase "$(INTDIR)\XTHexEdit.sbr"
	-@erase "$(INTDIR)\XTHtmlView.obj"
	-@erase "$(INTDIR)\XTHtmlView.sbr"
	-@erase "$(INTDIR)\XTHyperLink.obj"
	-@erase "$(INTDIR)\XTHyperLink.sbr"
	-@erase "$(INTDIR)\XTIconMap.obj"
	-@erase "$(INTDIR)\XTIconMap.sbr"
	-@erase "$(INTDIR)\XTImageManager.obj"
	-@erase "$(INTDIR)\XTImageManager.sbr"
	-@erase "$(INTDIR)\XTListCtrl.obj"
	-@erase "$(INTDIR)\XTListCtrl.sbr"
	-@erase "$(INTDIR)\XTListView.obj"
	-@erase "$(INTDIR)\XTListView.sbr"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.obj"
	-@erase "$(INTDIR)\XTLoadSysColorBitmap.sbr"
	-@erase "$(INTDIR)\XTLogoPane.obj"
	-@erase "$(INTDIR)\XTLogoPane.sbr"
	-@erase "$(INTDIR)\XTMaskEdit.obj"
	-@erase "$(INTDIR)\XTMaskEdit.sbr"
	-@erase "$(INTDIR)\XTMDIChildWnd.obj"
	-@erase "$(INTDIR)\XTMDIChildWnd.sbr"
	-@erase "$(INTDIR)\XTMDIFrameWnd.obj"
	-@erase "$(INTDIR)\XTMDIFrameWnd.sbr"
	-@erase "$(INTDIR)\XTMDIWndTab.obj"
	-@erase "$(INTDIR)\XTMDIWndTab.sbr"
	-@erase "$(INTDIR)\XTMemDC.obj"
	-@erase "$(INTDIR)\XTMemDC.sbr"
	-@erase "$(INTDIR)\XTMemFile.obj"
	-@erase "$(INTDIR)\XTMemFile.sbr"
	-@erase "$(INTDIR)\XTMenuBar.obj"
	-@erase "$(INTDIR)\XTMenuBar.sbr"
	-@erase "$(INTDIR)\XTMenuBar1.obj"
	-@erase "$(INTDIR)\XTMenuBar1.sbr"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.obj"
	-@erase "$(INTDIR)\XTMiniDockFrameWnd.sbr"
	-@erase "$(INTDIR)\XTNewToolbarDlg.obj"
	-@erase "$(INTDIR)\XTNewToolbarDlg.sbr"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.obj"
	-@erase "$(INTDIR)\XTOleIPFrameWnd.sbr"
	-@erase "$(INTDIR)\XTOptionsManager.obj"
	-@erase "$(INTDIR)\XTOptionsManager.sbr"
	-@erase "$(INTDIR)\XTOSVersionInfo.obj"
	-@erase "$(INTDIR)\XTOSVersionInfo.sbr"
	-@erase "$(INTDIR)\XTOutBarCtrl.obj"
	-@erase "$(INTDIR)\XTOutBarCtrl.sbr"
	-@erase "$(INTDIR)\XTOutlookBar.obj"
	-@erase "$(INTDIR)\XTOutlookBar.sbr"
	-@erase "$(INTDIR)\XTPagerCtrl.obj"
	-@erase "$(INTDIR)\XTPagerCtrl.sbr"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.obj"
	-@erase "$(INTDIR)\XTPopupTearOffWnd.sbr"
	-@erase "$(INTDIR)\XTPreviewView.obj"
	-@erase "$(INTDIR)\XTPreviewView.sbr"
	-@erase "$(INTDIR)\XTReBar.obj"
	-@erase "$(INTDIR)\XTReBar.sbr"
	-@erase "$(INTDIR)\XTReBar1.obj"
	-@erase "$(INTDIR)\XTReBar1.sbr"
	-@erase "$(INTDIR)\XTReBarCtrl.obj"
	-@erase "$(INTDIR)\XTReBarCtrl.sbr"
	-@erase "$(INTDIR)\XTRegistryManager.obj"
	-@erase "$(INTDIR)\XTRegistryManager.sbr"
	-@erase "$(INTDIR)\XTResize.obj"
	-@erase "$(INTDIR)\XTResize.sbr"
	-@erase "$(INTDIR)\XTResizeDialog.obj"
	-@erase "$(INTDIR)\XTResizeDialog.sbr"
	-@erase "$(INTDIR)\XTResizeFormView.obj"
	-@erase "$(INTDIR)\XTResizeFormView.sbr"
	-@erase "$(INTDIR)\XTResizePoint.obj"
	-@erase "$(INTDIR)\XTResizePoint.sbr"
	-@erase "$(INTDIR)\XTResizePropertyPage.obj"
	-@erase "$(INTDIR)\XTResizePropertyPage.sbr"
	-@erase "$(INTDIR)\XTResizePropertySheet.obj"
	-@erase "$(INTDIR)\XTResizePropertySheet.sbr"
	-@erase "$(INTDIR)\XTResizeRect.obj"
	-@erase "$(INTDIR)\XTResizeRect.sbr"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.obj"
	-@erase "$(INTDIR)\XTSearchOptionsCtrl.sbr"
	-@erase "$(INTDIR)\XTShellListCtrl.obj"
	-@erase "$(INTDIR)\XTShellListCtrl.sbr"
	-@erase "$(INTDIR)\XTShellListView.obj"
	-@erase "$(INTDIR)\XTShellListView.sbr"
	-@erase "$(INTDIR)\XTShellPidl.obj"
	-@erase "$(INTDIR)\XTShellPidl.sbr"
	-@erase "$(INTDIR)\XTShellSettings.obj"
	-@erase "$(INTDIR)\XTShellSettings.sbr"
	-@erase "$(INTDIR)\XTShellTreeCtrl.obj"
	-@erase "$(INTDIR)\XTShellTreeCtrl.sbr"
	-@erase "$(INTDIR)\XTShellTreeView.obj"
	-@erase "$(INTDIR)\XTShellTreeView.sbr"
	-@erase "$(INTDIR)\XTSkins.obj"
	-@erase "$(INTDIR)\XTSkins.sbr"
	-@erase "$(INTDIR)\XTSortClass.obj"
	-@erase "$(INTDIR)\XTSortClass.sbr"
	-@erase "$(INTDIR)\XTSplitterDock.obj"
	-@erase "$(INTDIR)\XTSplitterDock.sbr"
	-@erase "$(INTDIR)\XTSplitterWnd.obj"
	-@erase "$(INTDIR)\XTSplitterWnd.sbr"
	-@erase "$(INTDIR)\XTStatusBar.obj"
	-@erase "$(INTDIR)\XTStatusBar.sbr"
	-@erase "$(INTDIR)\XTTabCtrl.obj"
	-@erase "$(INTDIR)\XTTabCtrl.sbr"
	-@erase "$(INTDIR)\XTTabCtrlBar.obj"
	-@erase "$(INTDIR)\XTTabCtrlBar.sbr"
	-@erase "$(INTDIR)\XTTabCtrlButtons.obj"
	-@erase "$(INTDIR)\XTTabCtrlButtons.sbr"
	-@erase "$(INTDIR)\XTTabView.obj"
	-@erase "$(INTDIR)\XTTabView.sbr"
	-@erase "$(INTDIR)\XTTipOfTheDay.obj"
	-@erase "$(INTDIR)\XTTipOfTheDay.sbr"
	-@erase "$(INTDIR)\XTTipWindow.obj"
	-@erase "$(INTDIR)\XTTipWindow.sbr"
	-@erase "$(INTDIR)\XTToolBar.obj"
	-@erase "$(INTDIR)\XTToolBar.sbr"
	-@erase "$(INTDIR)\XTToolBar1.obj"
	-@erase "$(INTDIR)\XTToolBar1.sbr"
	-@erase "$(INTDIR)\XTToolBarCtrl.obj"
	-@erase "$(INTDIR)\XTToolBarCtrl.sbr"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.obj"
	-@erase "$(INTDIR)\XTToolBarPopupWnd.sbr"
	-@erase "$(INTDIR)\XTToolCmdUI.obj"
	-@erase "$(INTDIR)\XTToolCmdUI.sbr"
	-@erase "$(INTDIR)\XTToolkit_Dll.obj"
	-@erase "$(INTDIR)\XTToolkit_Dll.res"
	-@erase "$(INTDIR)\XTToolkit_Dll.sbr"
	-@erase "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	-@erase "$(INTDIR)\XTToolsManager.obj"
	-@erase "$(INTDIR)\XTToolsManager.sbr"
	-@erase "$(INTDIR)\XTTrayIcon.obj"
	-@erase "$(INTDIR)\XTTrayIcon.sbr"
	-@erase "$(INTDIR)\XTTreeCtrl.obj"
	-@erase "$(INTDIR)\XTTreeCtrl.sbr"
	-@erase "$(INTDIR)\XTTreeView.obj"
	-@erase "$(INTDIR)\XTTreeView.sbr"
	-@erase "$(INTDIR)\XTWindowList.obj"
	-@erase "$(INTDIR)\XTWindowList.sbr"
	-@erase "$(INTDIR)\XTWindowPos.obj"
	-@erase "$(INTDIR)\XTWindowPos.sbr"
	-@erase "$(INTDIR)\XTWndHook.obj"
	-@erase "$(INTDIR)\XTWndHook.sbr"
	-@erase "$(INTDIR)\XTWndShadow.obj"
	-@erase "$(INTDIR)\XTWndShadow.sbr"
	-@erase "$(OUTDIR)\XT3100Libud.pdb"
	-@erase "$(OUTDIR)\XTToolkit_Dll_vc50.bsc"
	-@erase "..\Bin_vc50\XT3100Libud.dll"
	-@erase "..\Bin_vc50\XT3100Libud.ilk"
	-@erase "..\Lib_vc50\XT3100Libud.exp"
	-@erase "..\Lib_vc50\XT3100Libud.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE"\
 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\XTToolkit_Dll_vc50.pch" /Yu"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Unicode_Debug/
CPP_SBRS=.\Unicode_Debug/

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

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /o /win32 "NUL" 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\XTToolkit_Dll.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\XTToolkit_Dll_vc50.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\XTAccelKeyEdit.sbr" \
	"$(INTDIR)\XTAccelManager.sbr" \
	"$(INTDIR)\XTAnimationMemDC.sbr" \
	"$(INTDIR)\XTBrowseDialog.sbr" \
	"$(INTDIR)\XTBrowseEdit.sbr" \
	"$(INTDIR)\XTButton.sbr" \
	"$(INTDIR)\XTCaption.sbr" \
	"$(INTDIR)\XTCaptionPopupWnd.sbr" \
	"$(INTDIR)\XTCBarDialog.sbr" \
	"$(INTDIR)\XTCheckListBox.sbr" \
	"$(INTDIR)\XTColorDialog.sbr" \
	"$(INTDIR)\XTColorPageCustom.sbr" \
	"$(INTDIR)\XTColorPageStandard.sbr" \
	"$(INTDIR)\XTColorPicker.sbr" \
	"$(INTDIR)\XTColorPopup.sbr" \
	"$(INTDIR)\XTColorRef.sbr" \
	"$(INTDIR)\XTColorSelectorCtrl.sbr" \
	"$(INTDIR)\XTComboBoxEx.sbr" \
	"$(INTDIR)\XTCommandsListBox.sbr" \
	"$(INTDIR)\XTControlBar.sbr" \
	"$(INTDIR)\XTCoolMenu.sbr" \
	"$(INTDIR)\XTCrc32.sbr" \
	"$(INTDIR)\XTCustomizeAPI.sbr" \
	"$(INTDIR)\XTCustomizePage.sbr" \
	"$(INTDIR)\XTCustomizeSheet.sbr" \
	"$(INTDIR)\XTCustomToolBar.sbr" \
	"$(INTDIR)\XTDateTimeCtrl.sbr" \
	"$(INTDIR)\XTDialogBar.sbr" \
	"$(INTDIR)\XTDockBar.sbr" \
	"$(INTDIR)\XTDockColorSelector.sbr" \
	"$(INTDIR)\XTDockContext.sbr" \
	"$(INTDIR)\XTDockState.sbr" \
	"$(INTDIR)\XTDockWindow.sbr" \
	"$(INTDIR)\XTDropSource.sbr" \
	"$(INTDIR)\XTEditListBox.sbr" \
	"$(INTDIR)\XTExpButton.sbr" \
	"$(INTDIR)\XTExpMenuWnd.sbr" \
	"$(INTDIR)\XTFlatComboBox.sbr" \
	"$(INTDIR)\XTFlatHeaderCtrl.sbr" \
	"$(INTDIR)\XTFlatTabCtrl.sbr" \
	"$(INTDIR)\XTFontCombo.sbr" \
	"$(INTDIR)\XTFrameImpl.sbr" \
	"$(INTDIR)\XTFrameWnd.sbr" \
	"$(INTDIR)\XTGlobal.sbr" \
	"$(INTDIR)\XTHelpers.sbr" \
	"$(INTDIR)\XTHexEdit.sbr" \
	"$(INTDIR)\XTHtmlView.sbr" \
	"$(INTDIR)\XTHyperLink.sbr" \
	"$(INTDIR)\XTIconMap.sbr" \
	"$(INTDIR)\XTImageManager.sbr" \
	"$(INTDIR)\XTListCtrl.sbr" \
	"$(INTDIR)\XTListView.sbr" \
	"$(INTDIR)\XTLoadSysColorBitmap.sbr" \
	"$(INTDIR)\XTLogoPane.sbr" \
	"$(INTDIR)\XTMaskEdit.sbr" \
	"$(INTDIR)\XTMDIChildWnd.sbr" \
	"$(INTDIR)\XTMDIFrameWnd.sbr" \
	"$(INTDIR)\XTMDIWndTab.sbr" \
	"$(INTDIR)\XTMemDC.sbr" \
	"$(INTDIR)\XTMemFile.sbr" \
	"$(INTDIR)\XTMenuBar.sbr" \
	"$(INTDIR)\XTMenuBar1.sbr" \
	"$(INTDIR)\XTMiniDockFrameWnd.sbr" \
	"$(INTDIR)\XTNewToolbarDlg.sbr" \
	"$(INTDIR)\XTOleIPFrameWnd.sbr" \
	"$(INTDIR)\XTOptionsManager.sbr" \
	"$(INTDIR)\XTOSVersionInfo.sbr" \
	"$(INTDIR)\XTOutBarCtrl.sbr" \
	"$(INTDIR)\XTOutlookBar.sbr" \
	"$(INTDIR)\XTPagerCtrl.sbr" \
	"$(INTDIR)\XTPopupTearOffWnd.sbr" \
	"$(INTDIR)\XTPreviewView.sbr" \
	"$(INTDIR)\XTReBar.sbr" \
	"$(INTDIR)\XTReBar1.sbr" \
	"$(INTDIR)\XTReBarCtrl.sbr" \
	"$(INTDIR)\XTRegistryManager.sbr" \
	"$(INTDIR)\XTResize.sbr" \
	"$(INTDIR)\XTResizeDialog.sbr" \
	"$(INTDIR)\XTResizeFormView.sbr" \
	"$(INTDIR)\XTResizePoint.sbr" \
	"$(INTDIR)\XTResizePropertyPage.sbr" \
	"$(INTDIR)\XTResizePropertySheet.sbr" \
	"$(INTDIR)\XTResizeRect.sbr" \
	"$(INTDIR)\XTSearchOptionsCtrl.sbr" \
	"$(INTDIR)\XTShellListCtrl.sbr" \
	"$(INTDIR)\XTShellListView.sbr" \
	"$(INTDIR)\XTShellPidl.sbr" \
	"$(INTDIR)\XTShellSettings.sbr" \
	"$(INTDIR)\XTShellTreeCtrl.sbr" \
	"$(INTDIR)\XTShellTreeView.sbr" \
	"$(INTDIR)\XTSkins.sbr" \
	"$(INTDIR)\XTSortClass.sbr" \
	"$(INTDIR)\XTSplitterDock.sbr" \
	"$(INTDIR)\XTSplitterWnd.sbr" \
	"$(INTDIR)\XTStatusBar.sbr" \
	"$(INTDIR)\XTTabCtrl.sbr" \
	"$(INTDIR)\XTTabCtrlBar.sbr" \
	"$(INTDIR)\XTTabCtrlButtons.sbr" \
	"$(INTDIR)\XTTabView.sbr" \
	"$(INTDIR)\XTTipOfTheDay.sbr" \
	"$(INTDIR)\XTTipWindow.sbr" \
	"$(INTDIR)\XTToolBar.sbr" \
	"$(INTDIR)\XTToolBar1.sbr" \
	"$(INTDIR)\XTToolBarCtrl.sbr" \
	"$(INTDIR)\XTToolBarPopupWnd.sbr" \
	"$(INTDIR)\XTToolCmdUI.sbr" \
	"$(INTDIR)\XTToolkit_Dll.sbr" \
	"$(INTDIR)\XTToolsManager.sbr" \
	"$(INTDIR)\XTTrayIcon.sbr" \
	"$(INTDIR)\XTTreeCtrl.sbr" \
	"$(INTDIR)\XTTreeView.sbr" \
	"$(INTDIR)\XTWindowList.sbr" \
	"$(INTDIR)\XTWindowPos.sbr" \
	"$(INTDIR)\XTWndHook.sbr" \
	"$(INTDIR)\XTWndShadow.sbr"

"$(OUTDIR)\XTToolkit_Dll_vc50.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /base:"0x67800000" /subsystem:windows /dll\
 /incremental:yes /pdb:"$(OUTDIR)\XT3100Libud.pdb" /debug /machine:I386\
 /def:".\XTToolkit_Dll.def" /out:"../Bin_vc50/XT3100Libud.dll"\
 /implib:"../Lib_vc50/XT3100Libud.lib" /pdbtype:sept 
DEF_FILE= \
	".\XTToolkit_Dll.def"
LINK32_OBJS= \
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
	"$(INTDIR)\XTToolkit_Dll.obj" \
	"$(INTDIR)\XTToolkit_Dll.res" \
	"$(INTDIR)\XTToolsManager.obj" \
	"$(INTDIR)\XTTrayIcon.obj" \
	"$(INTDIR)\XTTreeCtrl.obj" \
	"$(INTDIR)\XTTreeView.obj" \
	"$(INTDIR)\XTWindowList.obj" \
	"$(INTDIR)\XTWindowPos.obj" \
	"$(INTDIR)\XTWndHook.obj" \
	"$(INTDIR)\XTWndShadow.obj"

"..\Bin_vc50\XT3100Libud.dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "XTToolkit_Dll - Win32 Release" || "$(CFG)" ==\
 "XTToolkit_Dll - Win32 Debug" || "$(CFG)" ==\
 "XTToolkit_Dll - Win32 Unicode Release" || "$(CFG)" ==\
 "XTToolkit_Dll - Win32 Unicode Debug"
SOURCE=..\Src\Common.Src\XTBrowseDialog.cpp
DEP_CPP_XTBRO=\
	"..\Include\Common.Include\XTBrowseDialog.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTBRO=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTBrowseDialog.obj" : $(SOURCE) $(DEP_CPP_XTBRO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTBrowseDialog.obj" : $(SOURCE) $(DEP_CPP_XTBRO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTBrowseDialog.obj" : $(SOURCE) $(DEP_CPP_XTBRO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTBrowseDialog.obj"	"$(INTDIR)\XTBrowseDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_XTBRO) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTBrowseEdit.obj" : $(SOURCE) $(DEP_CPP_XTBROW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTBrowseEdit.obj" : $(SOURCE) $(DEP_CPP_XTBROW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTBrowseEdit.obj" : $(SOURCE) $(DEP_CPP_XTBROW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTBrowseEdit.obj"	"$(INTDIR)\XTBrowseEdit.sbr" : $(SOURCE)\
 $(DEP_CPP_XTBROW) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTButton.obj" : $(SOURCE) $(DEP_CPP_XTBUT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTButton.obj" : $(SOURCE) $(DEP_CPP_XTBUT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTButton.obj" : $(SOURCE) $(DEP_CPP_XTBUT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTButton.obj"	"$(INTDIR)\XTButton.sbr" : $(SOURCE) $(DEP_CPP_XTBUT)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCaption.obj" : $(SOURCE) $(DEP_CPP_XTCAP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCaption.obj" : $(SOURCE) $(DEP_CPP_XTCAP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCaption.obj" : $(SOURCE) $(DEP_CPP_XTCAP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCaption.obj"	"$(INTDIR)\XTCaption.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCAP) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCaptionPopupWnd.obj" : $(SOURCE) $(DEP_CPP_XTCAPT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCaptionPopupWnd.obj" : $(SOURCE) $(DEP_CPP_XTCAPT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCaptionPopupWnd.obj" : $(SOURCE) $(DEP_CPP_XTCAPT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCaptionPopupWnd.obj"	"$(INTDIR)\XTCaptionPopupWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCAPT) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCheckListBox.obj" : $(SOURCE) $(DEP_CPP_XTCHE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCheckListBox.obj" : $(SOURCE) $(DEP_CPP_XTCHE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCheckListBox.obj" : $(SOURCE) $(DEP_CPP_XTCHE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCheckListBox.obj"	"$(INTDIR)\XTCheckListBox.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCHE) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTColorDialog.obj" : $(SOURCE) $(DEP_CPP_XTCOL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTColorDialog.obj" : $(SOURCE) $(DEP_CPP_XTCOL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTColorDialog.obj" : $(SOURCE) $(DEP_CPP_XTCOL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTColorDialog.obj"	"$(INTDIR)\XTColorDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCOL) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTColorPageCustom.obj" : $(SOURCE) $(DEP_CPP_XTCOLO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTColorPageCustom.obj" : $(SOURCE) $(DEP_CPP_XTCOLO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTColorPageCustom.obj" : $(SOURCE) $(DEP_CPP_XTCOLO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTColorPageCustom.obj"	"$(INTDIR)\XTColorPageCustom.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCOLO) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTColorPageStandard.obj" : $(SOURCE) $(DEP_CPP_XTCOLOR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTColorPageStandard.obj" : $(SOURCE) $(DEP_CPP_XTCOLOR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTColorPageStandard.obj" : $(SOURCE) $(DEP_CPP_XTCOLOR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTColorPageStandard.obj"	"$(INTDIR)\XTColorPageStandard.sbr" : \
$(SOURCE) $(DEP_CPP_XTCOLOR) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTColorPicker.obj" : $(SOURCE) $(DEP_CPP_XTCOLORP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTColorPicker.obj" : $(SOURCE) $(DEP_CPP_XTCOLORP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTColorPicker.obj" : $(SOURCE) $(DEP_CPP_XTCOLORP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTColorPicker.obj"	"$(INTDIR)\XTColorPicker.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCOLORP) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTColorPopup.obj" : $(SOURCE) $(DEP_CPP_XTCOLORPO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTColorPopup.obj" : $(SOURCE) $(DEP_CPP_XTCOLORPO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTColorPopup.obj" : $(SOURCE) $(DEP_CPP_XTCOLORPO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTColorPopup.obj"	"$(INTDIR)\XTColorPopup.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCOLORPO) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTColorRef.cpp
DEP_CPP_XTCOLORR=\
	"..\Include\Common.Include\XTColorRef.h"\
	"..\Include\XTDefines.h"\
	
NODEP_CPP_XTCOLORR=\
	"..\Include\XTPColorManager.h"\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTColorRef.obj" : $(SOURCE) $(DEP_CPP_XTCOLORR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTColorRef.obj" : $(SOURCE) $(DEP_CPP_XTCOLORR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTColorRef.obj" : $(SOURCE) $(DEP_CPP_XTCOLORR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTColorRef.obj"	"$(INTDIR)\XTColorRef.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCOLORR) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTColorSelectorCtrl.obj" : $(SOURCE) $(DEP_CPP_XTCOLORS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTColorSelectorCtrl.obj" : $(SOURCE) $(DEP_CPP_XTCOLORS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTColorSelectorCtrl.obj" : $(SOURCE) $(DEP_CPP_XTCOLORS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTColorSelectorCtrl.obj"	"$(INTDIR)\XTColorSelectorCtrl.sbr" : \
$(SOURCE) $(DEP_CPP_XTCOLORS) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTComboBoxEx.cpp
DEP_CPP_XTCOM=\
	"..\Include\Common.Include\XTComboBoxEx.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTCOM=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTComboBoxEx.obj" : $(SOURCE) $(DEP_CPP_XTCOM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTComboBoxEx.obj" : $(SOURCE) $(DEP_CPP_XTCOM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTComboBoxEx.obj" : $(SOURCE) $(DEP_CPP_XTCOM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTComboBoxEx.obj"	"$(INTDIR)\XTComboBoxEx.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCOM) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTDateTimeCtrl.cpp
DEP_CPP_XTDAT=\
	"..\Include\Common.Include\XTDateTimeCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTDAT=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTDateTimeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTDAT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTDateTimeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTDAT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTDateTimeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTDAT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTDateTimeCtrl.obj"	"$(INTDIR)\XTDateTimeCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTDAT) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTDropSource.cpp
DEP_CPP_XTDRO=\
	"..\Include\Common.Include\XTDropSource.h"\
	
NODEP_CPP_XTDRO=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTDropSource.obj" : $(SOURCE) $(DEP_CPP_XTDRO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTDropSource.obj" : $(SOURCE) $(DEP_CPP_XTDRO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTDropSource.obj" : $(SOURCE) $(DEP_CPP_XTDRO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTDropSource.obj"	"$(INTDIR)\XTDropSource.sbr" : $(SOURCE)\
 $(DEP_CPP_XTDRO) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTEditListBox.obj" : $(SOURCE) $(DEP_CPP_XTEDI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTEditListBox.obj" : $(SOURCE) $(DEP_CPP_XTEDI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTEditListBox.obj" : $(SOURCE) $(DEP_CPP_XTEDI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTEditListBox.obj"	"$(INTDIR)\XTEditListBox.sbr" : $(SOURCE)\
 $(DEP_CPP_XTEDI) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTFlatComboBox.obj" : $(SOURCE) $(DEP_CPP_XTFLA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTFlatComboBox.obj" : $(SOURCE) $(DEP_CPP_XTFLA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTFlatComboBox.obj" : $(SOURCE) $(DEP_CPP_XTFLA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTFlatComboBox.obj"	"$(INTDIR)\XTFlatComboBox.sbr" : $(SOURCE)\
 $(DEP_CPP_XTFLA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTFlatHeaderCtrl.obj" : $(SOURCE) $(DEP_CPP_XTFLAT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTFlatHeaderCtrl.obj" : $(SOURCE) $(DEP_CPP_XTFLAT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTFlatHeaderCtrl.obj" : $(SOURCE) $(DEP_CPP_XTFLAT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTFlatHeaderCtrl.obj"	"$(INTDIR)\XTFlatHeaderCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTFLAT) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTFlatTabCtrl.obj" : $(SOURCE) $(DEP_CPP_XTFLATT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTFlatTabCtrl.obj" : $(SOURCE) $(DEP_CPP_XTFLATT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTFlatTabCtrl.obj" : $(SOURCE) $(DEP_CPP_XTFLATT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTFlatTabCtrl.obj"	"$(INTDIR)\XTFlatTabCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTFLATT) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTFontCombo.obj" : $(SOURCE) $(DEP_CPP_XTFON) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTFontCombo.obj" : $(SOURCE) $(DEP_CPP_XTFON) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTFontCombo.obj" : $(SOURCE) $(DEP_CPP_XTFON) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTFontCombo.obj"	"$(INTDIR)\XTFontCombo.sbr" : $(SOURCE)\
 $(DEP_CPP_XTFON) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTHexEdit.obj" : $(SOURCE) $(DEP_CPP_XTHEX) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTHexEdit.obj" : $(SOURCE) $(DEP_CPP_XTHEX) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTHexEdit.obj" : $(SOURCE) $(DEP_CPP_XTHEX) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTHexEdit.obj"	"$(INTDIR)\XTHexEdit.sbr" : $(SOURCE)\
 $(DEP_CPP_XTHEX) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTHtmlView.cpp
DEP_CPP_XTHTM=\
	"..\Include\Common.Include\XTFunctions.h"\
	"..\Include\Common.Include\XTHtmlView.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTHTM=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTHtmlView.obj" : $(SOURCE) $(DEP_CPP_XTHTM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTHtmlView.obj" : $(SOURCE) $(DEP_CPP_XTHTM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTHtmlView.obj" : $(SOURCE) $(DEP_CPP_XTHTM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTHtmlView.obj"	"$(INTDIR)\XTHtmlView.sbr" : $(SOURCE)\
 $(DEP_CPP_XTHTM) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTHyperLink.cpp
DEP_CPP_XTHYP=\
	"..\Include\Common.Include\XTHyperLink.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTHYP=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTHyperLink.obj" : $(SOURCE) $(DEP_CPP_XTHYP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTHyperLink.obj" : $(SOURCE) $(DEP_CPP_XTHYP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTHyperLink.obj" : $(SOURCE) $(DEP_CPP_XTHYP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTHyperLink.obj"	"$(INTDIR)\XTHyperLink.sbr" : $(SOURCE)\
 $(DEP_CPP_XTHYP) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTListCtrl.obj" : $(SOURCE) $(DEP_CPP_XTLIS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTListCtrl.obj" : $(SOURCE) $(DEP_CPP_XTLIS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTListCtrl.obj" : $(SOURCE) $(DEP_CPP_XTLIS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTListCtrl.obj"	"$(INTDIR)\XTListCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTLIS) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTListView.obj" : $(SOURCE) $(DEP_CPP_XTLIST) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTListView.obj" : $(SOURCE) $(DEP_CPP_XTLIST) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTListView.obj" : $(SOURCE) $(DEP_CPP_XTLIST) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTListView.obj"	"$(INTDIR)\XTListView.sbr" : $(SOURCE)\
 $(DEP_CPP_XTLIST) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTLoadSysColorBitmap.cpp
DEP_CPP_XTLOA=\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTLOA=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTLoadSysColorBitmap.obj" : $(SOURCE) $(DEP_CPP_XTLOA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTLoadSysColorBitmap.obj" : $(SOURCE) $(DEP_CPP_XTLOA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTLoadSysColorBitmap.obj" : $(SOURCE) $(DEP_CPP_XTLOA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTLoadSysColorBitmap.obj"	"$(INTDIR)\XTLoadSysColorBitmap.sbr" : \
$(SOURCE) $(DEP_CPP_XTLOA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTLogoPane.cpp
DEP_CPP_XTLOG=\
	"..\Include\Common.Include\XTLogoPane.h"\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTLOG=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTLogoPane.obj" : $(SOURCE) $(DEP_CPP_XTLOG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTLogoPane.obj" : $(SOURCE) $(DEP_CPP_XTLOG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTLogoPane.obj" : $(SOURCE) $(DEP_CPP_XTLOG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTLogoPane.obj"	"$(INTDIR)\XTLogoPane.sbr" : $(SOURCE)\
 $(DEP_CPP_XTLOG) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMaskEdit.obj" : $(SOURCE) $(DEP_CPP_XTMAS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMaskEdit.obj" : $(SOURCE) $(DEP_CPP_XTMAS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMaskEdit.obj" : $(SOURCE) $(DEP_CPP_XTMAS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMaskEdit.obj"	"$(INTDIR)\XTMaskEdit.sbr" : $(SOURCE)\
 $(DEP_CPP_XTMAS) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMDIWndTab.obj" : $(SOURCE) $(DEP_CPP_XTMDI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMDIWndTab.obj" : $(SOURCE) $(DEP_CPP_XTMDI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMDIWndTab.obj" : $(SOURCE) $(DEP_CPP_XTMDI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMDIWndTab.obj"	"$(INTDIR)\XTMDIWndTab.sbr" : $(SOURCE)\
 $(DEP_CPP_XTMDI) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTMemDC.cpp
DEP_CPP_XTMEM=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTMEM=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMemDC.obj" : $(SOURCE) $(DEP_CPP_XTMEM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMemDC.obj" : $(SOURCE) $(DEP_CPP_XTMEM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMemDC.obj" : $(SOURCE) $(DEP_CPP_XTMEM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMemDC.obj"	"$(INTDIR)\XTMemDC.sbr" : $(SOURCE) $(DEP_CPP_XTMEM)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTMemFile.cpp
DEP_CPP_XTMEMF=\
	"..\Include\Common.Include\XTMemFile.h"\
	
NODEP_CPP_XTMEMF=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMemFile.obj" : $(SOURCE) $(DEP_CPP_XTMEMF) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMemFile.obj" : $(SOURCE) $(DEP_CPP_XTMEMF) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMemFile.obj" : $(SOURCE) $(DEP_CPP_XTMEMF) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMemFile.obj"	"$(INTDIR)\XTMemFile.sbr" : $(SOURCE)\
 $(DEP_CPP_XTMEMF) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTOSVersionInfo.cpp
DEP_CPP_XTOSV=\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTOSV=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTOSVersionInfo.obj" : $(SOURCE) $(DEP_CPP_XTOSV) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTOSVersionInfo.obj" : $(SOURCE) $(DEP_CPP_XTOSV) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTOSVersionInfo.obj" : $(SOURCE) $(DEP_CPP_XTOSV) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTOSVersionInfo.obj"	"$(INTDIR)\XTOSVersionInfo.sbr" : $(SOURCE)\
 $(DEP_CPP_XTOSV) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTOutBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTOUT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTOutBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTOUT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTOutBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTOUT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTOutBarCtrl.obj"	"$(INTDIR)\XTOutBarCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTOUT) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTOutlookBar.cpp
DEP_CPP_XTOUTL=\
	"..\Include\Common.Include\XTOutlookBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTOUTL=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTOutlookBar.obj" : $(SOURCE) $(DEP_CPP_XTOUTL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTOutlookBar.obj" : $(SOURCE) $(DEP_CPP_XTOUTL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTOutlookBar.obj" : $(SOURCE) $(DEP_CPP_XTOUTL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTOutlookBar.obj"	"$(INTDIR)\XTOutlookBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTOUTL) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTPagerCtrl.cpp
DEP_CPP_XTPAG=\
	"..\Include\Common.Include\XTPagerCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTPAG=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTPagerCtrl.obj" : $(SOURCE) $(DEP_CPP_XTPAG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTPagerCtrl.obj" : $(SOURCE) $(DEP_CPP_XTPAG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTPagerCtrl.obj" : $(SOURCE) $(DEP_CPP_XTPAG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTPagerCtrl.obj"	"$(INTDIR)\XTPagerCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTPAG) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTRegistryManager.cpp
DEP_CPP_XTREG=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTREG=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTRegistryManager.obj" : $(SOURCE) $(DEP_CPP_XTREG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTRegistryManager.obj" : $(SOURCE) $(DEP_CPP_XTREG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTRegistryManager.obj" : $(SOURCE) $(DEP_CPP_XTREG) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTRegistryManager.obj"	"$(INTDIR)\XTRegistryManager.sbr" : $(SOURCE)\
 $(DEP_CPP_XTREG) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTResize.cpp
DEP_CPP_XTRES=\
	"..\Include\Common.Include\XTResize.h"\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRES=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTResize.obj" : $(SOURCE) $(DEP_CPP_XTRES) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTResize.obj" : $(SOURCE) $(DEP_CPP_XTRES) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTResize.obj" : $(SOURCE) $(DEP_CPP_XTRES) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTResize.obj"	"$(INTDIR)\XTResize.sbr" : $(SOURCE) $(DEP_CPP_XTRES)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTResizeDialog.obj" : $(SOURCE) $(DEP_CPP_XTRESI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTResizeDialog.obj" : $(SOURCE) $(DEP_CPP_XTRESI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTResizeDialog.obj" : $(SOURCE) $(DEP_CPP_XTRESI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTResizeDialog.obj"	"$(INTDIR)\XTResizeDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_XTRESI) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTResizeFormView.obj" : $(SOURCE) $(DEP_CPP_XTRESIZ) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTResizeFormView.obj" : $(SOURCE) $(DEP_CPP_XTRESIZ) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTResizeFormView.obj" : $(SOURCE) $(DEP_CPP_XTRESIZ) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTResizeFormView.obj"	"$(INTDIR)\XTResizeFormView.sbr" : $(SOURCE)\
 $(DEP_CPP_XTRESIZ) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTResizePoint.cpp
DEP_CPP_XTRESIZE=\
	"..\Include\Common.Include\XTResizePoint.h"\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRESIZE=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTResizePoint.obj" : $(SOURCE) $(DEP_CPP_XTRESIZE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTResizePoint.obj" : $(SOURCE) $(DEP_CPP_XTRESIZE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTResizePoint.obj" : $(SOURCE) $(DEP_CPP_XTRESIZE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTResizePoint.obj"	"$(INTDIR)\XTResizePoint.sbr" : $(SOURCE)\
 $(DEP_CPP_XTRESIZE) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTResizePropertyPage.obj" : $(SOURCE) $(DEP_CPP_XTRESIZEP)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTResizePropertyPage.obj" : $(SOURCE) $(DEP_CPP_XTRESIZEP)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTResizePropertyPage.obj" : $(SOURCE) $(DEP_CPP_XTRESIZEP)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTResizePropertyPage.obj"	"$(INTDIR)\XTResizePropertyPage.sbr" : \
$(SOURCE) $(DEP_CPP_XTRESIZEP) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTResizePropertySheet.obj" : $(SOURCE) $(DEP_CPP_XTRESIZEPR)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTResizePropertySheet.obj" : $(SOURCE) $(DEP_CPP_XTRESIZEPR)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTResizePropertySheet.obj" : $(SOURCE) $(DEP_CPP_XTRESIZEPR)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTResizePropertySheet.obj"	"$(INTDIR)\XTResizePropertySheet.sbr" : \
$(SOURCE) $(DEP_CPP_XTRESIZEPR) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTResizeRect.cpp
DEP_CPP_XTRESIZER=\
	"..\Include\Common.Include\XTResizeRect.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTRESIZER=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTResizeRect.obj" : $(SOURCE) $(DEP_CPP_XTRESIZER) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTResizeRect.obj" : $(SOURCE) $(DEP_CPP_XTRESIZER) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTResizeRect.obj" : $(SOURCE) $(DEP_CPP_XTRESIZER) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTResizeRect.obj"	"$(INTDIR)\XTResizeRect.sbr" : $(SOURCE)\
 $(DEP_CPP_XTRESIZER) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTSearchOptionsCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSEA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTSearchOptionsCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSEA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTSearchOptionsCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSEA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTSearchOptionsCtrl.obj"	"$(INTDIR)\XTSearchOptionsCtrl.sbr" : \
$(SOURCE) $(DEP_CPP_XTSEA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTShellListCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSHE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTShellListCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSHE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTShellListCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSHE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTShellListCtrl.obj"	"$(INTDIR)\XTShellListCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSHE) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTShellListView.obj" : $(SOURCE) $(DEP_CPP_XTSHEL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTShellListView.obj" : $(SOURCE) $(DEP_CPP_XTSHEL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTShellListView.obj" : $(SOURCE) $(DEP_CPP_XTSHEL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTShellListView.obj"	"$(INTDIR)\XTShellListView.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSHEL) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTShellPidl.cpp
DEP_CPP_XTSHELL=\
	"..\Include\Common.Include\XTShellPidl.h"\
	
NODEP_CPP_XTSHELL=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTShellPidl.obj" : $(SOURCE) $(DEP_CPP_XTSHELL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTShellPidl.obj" : $(SOURCE) $(DEP_CPP_XTSHELL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTShellPidl.obj" : $(SOURCE) $(DEP_CPP_XTSHELL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTShellPidl.obj"	"$(INTDIR)\XTShellPidl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSHELL) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTShellSettings.cpp
DEP_CPP_XTSHELLS=\
	"..\Include\Common.Include\XTShellSettings.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSHELLS=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTShellSettings.obj" : $(SOURCE) $(DEP_CPP_XTSHELLS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTShellSettings.obj" : $(SOURCE) $(DEP_CPP_XTSHELLS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTShellSettings.obj" : $(SOURCE) $(DEP_CPP_XTSHELLS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTShellSettings.obj"	"$(INTDIR)\XTShellSettings.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSHELLS) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTShellTreeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSHELLT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTShellTreeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSHELLT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTShellTreeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTSHELLT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTShellTreeCtrl.obj"	"$(INTDIR)\XTShellTreeCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSHELLT) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTShellTreeView.obj" : $(SOURCE) $(DEP_CPP_XTSHELLTR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTShellTreeView.obj" : $(SOURCE) $(DEP_CPP_XTSHELLTR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTShellTreeView.obj" : $(SOURCE) $(DEP_CPP_XTSHELLTR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTShellTreeView.obj"	"$(INTDIR)\XTShellTreeView.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSHELLTR) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTSortClass.cpp
DEP_CPP_XTSOR=\
	"..\Include\Common.Include\XTSortClass.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSOR=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTSortClass.obj" : $(SOURCE) $(DEP_CPP_XTSOR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTSortClass.obj" : $(SOURCE) $(DEP_CPP_XTSOR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTSortClass.obj" : $(SOURCE) $(DEP_CPP_XTSOR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTSortClass.obj"	"$(INTDIR)\XTSortClass.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSOR) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTSplitterWnd.cpp
DEP_CPP_XTSPL=\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTSplitterWnd.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSPL=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTSplitterWnd.obj" : $(SOURCE) $(DEP_CPP_XTSPL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTSplitterWnd.obj" : $(SOURCE) $(DEP_CPP_XTSPL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTSplitterWnd.obj" : $(SOURCE) $(DEP_CPP_XTSPL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTSplitterWnd.obj"	"$(INTDIR)\XTSplitterWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSPL) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTStatusBar.cpp
DEP_CPP_XTSTA=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTStatusBar.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTSTA=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTStatusBar.obj" : $(SOURCE) $(DEP_CPP_XTSTA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTStatusBar.obj" : $(SOURCE) $(DEP_CPP_XTSTA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTStatusBar.obj" : $(SOURCE) $(DEP_CPP_XTSTA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTStatusBar.obj"	"$(INTDIR)\XTStatusBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSTA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTabCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTAB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTabCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTAB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTabCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTAB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTabCtrl.obj"	"$(INTDIR)\XTTabCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTAB) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTabCtrlButtons.obj" : $(SOURCE) $(DEP_CPP_XTTABC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTabCtrlButtons.obj" : $(SOURCE) $(DEP_CPP_XTTABC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTabCtrlButtons.obj" : $(SOURCE) $(DEP_CPP_XTTABC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTabCtrlButtons.obj"	"$(INTDIR)\XTTabCtrlButtons.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTABC) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTabView.obj" : $(SOURCE) $(DEP_CPP_XTTABV) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTabView.obj" : $(SOURCE) $(DEP_CPP_XTTABV) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTabView.obj" : $(SOURCE) $(DEP_CPP_XTTABV) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTabView.obj"	"$(INTDIR)\XTTabView.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTABV) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTTipOfTheDay.cpp
DEP_CPP_XTTIP=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTTipOfTheDay.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTTIP=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTipOfTheDay.obj" : $(SOURCE) $(DEP_CPP_XTTIP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTipOfTheDay.obj" : $(SOURCE) $(DEP_CPP_XTTIP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTipOfTheDay.obj" : $(SOURCE) $(DEP_CPP_XTTIP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTipOfTheDay.obj"	"$(INTDIR)\XTTipOfTheDay.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTIP) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTTipWindow.cpp
DEP_CPP_XTTIPW=\
	"..\Include\Common.Include\XTTipWindow.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTTIPW=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTipWindow.obj" : $(SOURCE) $(DEP_CPP_XTTIPW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTipWindow.obj" : $(SOURCE) $(DEP_CPP_XTTIPW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTipWindow.obj" : $(SOURCE) $(DEP_CPP_XTTIPW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTipWindow.obj"	"$(INTDIR)\XTTipWindow.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTIPW) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTTrayIcon.cpp
DEP_CPP_XTTRA=\
	"..\Include\Common.Include\XTOSVersionInfo.h"\
	"..\Include\Common.Include\XTTrayIcon.h"\
	"..\Include\Common.Include\XTWndHook.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTTRA=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTrayIcon.obj" : $(SOURCE) $(DEP_CPP_XTTRA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTrayIcon.obj" : $(SOURCE) $(DEP_CPP_XTTRA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTrayIcon.obj" : $(SOURCE) $(DEP_CPP_XTTRA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTrayIcon.obj"	"$(INTDIR)\XTTrayIcon.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTRA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTTreeCtrl.cpp
DEP_CPP_XTTRE=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTTreeCtrl.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTTRE=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTreeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTRE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTreeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTRE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTreeCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTRE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTreeCtrl.obj"	"$(INTDIR)\XTTreeCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTRE) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTTreeView.cpp
DEP_CPP_XTTREE=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\Common.Include\XTTreeView.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTTREE=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTreeView.obj" : $(SOURCE) $(DEP_CPP_XTTREE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTreeView.obj" : $(SOURCE) $(DEP_CPP_XTTREE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTreeView.obj" : $(SOURCE) $(DEP_CPP_XTTREE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTreeView.obj"	"$(INTDIR)\XTTreeView.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTREE) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTWindowList.obj" : $(SOURCE) $(DEP_CPP_XTWIN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTWindowList.obj" : $(SOURCE) $(DEP_CPP_XTWIN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTWindowList.obj" : $(SOURCE) $(DEP_CPP_XTWIN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTWindowList.obj"	"$(INTDIR)\XTWindowList.sbr" : $(SOURCE)\
 $(DEP_CPP_XTWIN) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\Common.Src\XTWindowPos.cpp
DEP_CPP_XTWIND=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\Common.Include\XTWindowPos.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	
NODEP_CPP_XTWIND=\
	"..\Src\Common.Src\StdAfx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTWindowPos.obj" : $(SOURCE) $(DEP_CPP_XTWIND) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTWindowPos.obj" : $(SOURCE) $(DEP_CPP_XTWIND) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTWindowPos.obj" : $(SOURCE) $(DEP_CPP_XTWIND) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTWindowPos.obj"	"$(INTDIR)\XTWindowPos.sbr" : $(SOURCE)\
 $(DEP_CPP_XTWIND) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTWndHook.obj" : $(SOURCE) $(DEP_CPP_XTWND) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTWndHook.obj" : $(SOURCE) $(DEP_CPP_XTWND) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTWndHook.obj" : $(SOURCE) $(DEP_CPP_XTWND) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTWndHook.obj"	"$(INTDIR)\XTWndHook.sbr" : $(SOURCE)\
 $(DEP_CPP_XTWND) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTWndShadow.obj" : $(SOURCE) $(DEP_CPP_XTWNDS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTWndShadow.obj" : $(SOURCE) $(DEP_CPP_XTWNDS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTWndShadow.obj" : $(SOURCE) $(DEP_CPP_XTWNDS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTWndShadow.obj"	"$(INTDIR)\XTWndShadow.sbr" : $(SOURCE)\
 $(DEP_CPP_XTWNDS) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	{$(INCLUDE)}"..\src\afximpl.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"

CPP_SWITCHES=/nologo /MD /W4 /WX /GX /O2 /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT"\
 /Fp"$(INTDIR)\XTToolkit_Dll_vc50.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Dll_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W4 /GX /Zi /Od /I "..\src" /I "..\src\Common.src" /I\
 "..\include" /I "..\include\Common.include" /D "WIN32" /D "_DEBUG" /D\
 "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT"\
 /Fp"$(INTDIR)\XTToolkit_Dll_vc50.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Dll_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"

CPP_SWITCHES=/nologo /MD /W4 /WX /GX /O2 /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE"\
 /Fp"$(INTDIR)\XTToolkit_Dll_vc50.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\XTToolkit_Dll_vc50.pch" : $(SOURCE)\
 $(DEP_CPP_STDAF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"

CPP_SWITCHES=/nologo /MDd /W3 /GX /Zi /Od /I "..\include" /I\
 "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "WIN32" /D\
 "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_AFXEXT" /D "_UNICODE"\
 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\XTToolkit_Dll_vc50.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"\
	"$(INTDIR)\XTToolkit_Dll_vc50.pch" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"
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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTAccelKeyEdit.obj" : $(SOURCE) $(DEP_CPP_XTACC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTAccelKeyEdit.obj" : $(SOURCE) $(DEP_CPP_XTACC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTAccelKeyEdit.obj" : $(SOURCE) $(DEP_CPP_XTACC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTAccelKeyEdit.obj"	"$(INTDIR)\XTAccelKeyEdit.sbr" : $(SOURCE)\
 $(DEP_CPP_XTACC) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTAccelManager.obj" : $(SOURCE) $(DEP_CPP_XTACCE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTAccelManager.obj" : $(SOURCE) $(DEP_CPP_XTACCE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTAccelManager.obj" : $(SOURCE) $(DEP_CPP_XTACCE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTAccelManager.obj"	"$(INTDIR)\XTAccelManager.sbr" : $(SOURCE)\
 $(DEP_CPP_XTACCE) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\XTAnimationMemDC.cpp
DEP_CPP_XTANI=\
	"..\Include\Common.Include\XTMemDC.h"\
	"..\Include\XTAnimationMemDC.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTANI=\
	"..\Src\stdafx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTAnimationMemDC.obj" : $(SOURCE) $(DEP_CPP_XTANI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTAnimationMemDC.obj" : $(SOURCE) $(DEP_CPP_XTANI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTAnimationMemDC.obj" : $(SOURCE) $(DEP_CPP_XTANI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTAnimationMemDC.obj"	"$(INTDIR)\XTAnimationMemDC.sbr" : $(SOURCE)\
 $(DEP_CPP_XTANI) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCBarDialog.obj" : $(SOURCE) $(DEP_CPP_XTCBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCBarDialog.obj" : $(SOURCE) $(DEP_CPP_XTCBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCBarDialog.obj" : $(SOURCE) $(DEP_CPP_XTCBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCBarDialog.obj"	"$(INTDIR)\XTCBarDialog.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCBA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCommandsListBox.obj" : $(SOURCE) $(DEP_CPP_XTCOMM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCommandsListBox.obj" : $(SOURCE) $(DEP_CPP_XTCOMM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCommandsListBox.obj" : $(SOURCE) $(DEP_CPP_XTCOMM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCommandsListBox.obj"	"$(INTDIR)\XTCommandsListBox.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCOMM) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTControlBar.obj" : $(SOURCE) $(DEP_CPP_XTCON) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTControlBar.obj" : $(SOURCE) $(DEP_CPP_XTCON) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTControlBar.obj" : $(SOURCE) $(DEP_CPP_XTCON) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTControlBar.obj"	"$(INTDIR)\XTControlBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCON) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCoolMenu.obj" : $(SOURCE) $(DEP_CPP_XTCOO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCoolMenu.obj" : $(SOURCE) $(DEP_CPP_XTCOO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCoolMenu.obj" : $(SOURCE) $(DEP_CPP_XTCOO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCoolMenu.obj"	"$(INTDIR)\XTCoolMenu.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCOO) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\XTCrc32.cpp
DEP_CPP_XTCRC=\
	"..\Src\XTCrc32.h"\
	
NODEP_CPP_XTCRC=\
	"..\Src\stdafx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCrc32.obj" : $(SOURCE) $(DEP_CPP_XTCRC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCrc32.obj" : $(SOURCE) $(DEP_CPP_XTCRC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCrc32.obj" : $(SOURCE) $(DEP_CPP_XTCRC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCrc32.obj"	"$(INTDIR)\XTCrc32.sbr" : $(SOURCE) $(DEP_CPP_XTCRC)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCustomizeAPI.obj" : $(SOURCE) $(DEP_CPP_XTCUS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCustomizeAPI.obj" : $(SOURCE) $(DEP_CPP_XTCUS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCustomizeAPI.obj" : $(SOURCE) $(DEP_CPP_XTCUS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCustomizeAPI.obj"	"$(INTDIR)\XTCustomizeAPI.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCUS) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCustomizePage.obj" : $(SOURCE) $(DEP_CPP_XTCUST) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCustomizePage.obj" : $(SOURCE) $(DEP_CPP_XTCUST) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCustomizePage.obj" : $(SOURCE) $(DEP_CPP_XTCUST) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCustomizePage.obj"	"$(INTDIR)\XTCustomizePage.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCUST) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCustomizeSheet.obj" : $(SOURCE) $(DEP_CPP_XTCUSTO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCustomizeSheet.obj" : $(SOURCE) $(DEP_CPP_XTCUSTO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCustomizeSheet.obj" : $(SOURCE) $(DEP_CPP_XTCUSTO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCustomizeSheet.obj"	"$(INTDIR)\XTCustomizeSheet.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCUSTO) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTCustomToolBar.obj" : $(SOURCE) $(DEP_CPP_XTCUSTOM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTCustomToolBar.obj" : $(SOURCE) $(DEP_CPP_XTCUSTOM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTCustomToolBar.obj" : $(SOURCE) $(DEP_CPP_XTCUSTOM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTCustomToolBar.obj"	"$(INTDIR)\XTCustomToolBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTCUSTOM) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTDialogBar.obj" : $(SOURCE) $(DEP_CPP_XTDIA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTDialogBar.obj" : $(SOURCE) $(DEP_CPP_XTDIA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTDialogBar.obj" : $(SOURCE) $(DEP_CPP_XTDIA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTDialogBar.obj"	"$(INTDIR)\XTDialogBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTDIA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTDockBar.obj" : $(SOURCE) $(DEP_CPP_XTDOC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTDockBar.obj" : $(SOURCE) $(DEP_CPP_XTDOC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTDockBar.obj" : $(SOURCE) $(DEP_CPP_XTDOC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTDockBar.obj"	"$(INTDIR)\XTDockBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTDOC) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTDockColorSelector.obj" : $(SOURCE) $(DEP_CPP_XTDOCK) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTDockColorSelector.obj" : $(SOURCE) $(DEP_CPP_XTDOCK) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTDockColorSelector.obj" : $(SOURCE) $(DEP_CPP_XTDOCK) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTDockColorSelector.obj"	"$(INTDIR)\XTDockColorSelector.sbr" : \
$(SOURCE) $(DEP_CPP_XTDOCK) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTDockContext.obj" : $(SOURCE) $(DEP_CPP_XTDOCKC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTDockContext.obj" : $(SOURCE) $(DEP_CPP_XTDOCKC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTDockContext.obj" : $(SOURCE) $(DEP_CPP_XTDOCKC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTDockContext.obj"	"$(INTDIR)\XTDockContext.sbr" : $(SOURCE)\
 $(DEP_CPP_XTDOCKC) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\XTDockState.cpp
DEP_CPP_XTDOCKS=\
	"..\Include\Common.Include\XTRegistryManager.h"\
	"..\Include\XTDefines.h"\
	"..\Include\XTDockState.h"\
	"..\Include\XTGlobal.h"\
	
NODEP_CPP_XTDOCKS=\
	"..\Src\stdafx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTDockState.obj" : $(SOURCE) $(DEP_CPP_XTDOCKS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTDockState.obj" : $(SOURCE) $(DEP_CPP_XTDOCKS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTDockState.obj" : $(SOURCE) $(DEP_CPP_XTDOCKS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTDockState.obj"	"$(INTDIR)\XTDockState.sbr" : $(SOURCE)\
 $(DEP_CPP_XTDOCKS) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTDockWindow.obj" : $(SOURCE) $(DEP_CPP_XTDOCKW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTDockWindow.obj" : $(SOURCE) $(DEP_CPP_XTDOCKW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTDockWindow.obj" : $(SOURCE) $(DEP_CPP_XTDOCKW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTDockWindow.obj"	"$(INTDIR)\XTDockWindow.sbr" : $(SOURCE)\
 $(DEP_CPP_XTDOCKW) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTExpButton.obj" : $(SOURCE) $(DEP_CPP_XTEXP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTExpButton.obj" : $(SOURCE) $(DEP_CPP_XTEXP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTExpButton.obj" : $(SOURCE) $(DEP_CPP_XTEXP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTExpButton.obj"	"$(INTDIR)\XTExpButton.sbr" : $(SOURCE)\
 $(DEP_CPP_XTEXP) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTExpMenuWnd.obj" : $(SOURCE) $(DEP_CPP_XTEXPM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTExpMenuWnd.obj" : $(SOURCE) $(DEP_CPP_XTEXPM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTExpMenuWnd.obj" : $(SOURCE) $(DEP_CPP_XTEXPM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTExpMenuWnd.obj"	"$(INTDIR)\XTExpMenuWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTEXPM) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTFrameImpl.obj" : $(SOURCE) $(DEP_CPP_XTFRA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTFrameImpl.obj" : $(SOURCE) $(DEP_CPP_XTFRA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTFrameImpl.obj" : $(SOURCE) $(DEP_CPP_XTFRA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTFrameImpl.obj"	"$(INTDIR)\XTFrameImpl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTFRA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTFRAM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTFRAM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTFRAM) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTFrameWnd.obj"	"$(INTDIR)\XTFrameWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTFRAM) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTGlobal.obj" : $(SOURCE) $(DEP_CPP_XTGLO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTGlobal.obj" : $(SOURCE) $(DEP_CPP_XTGLO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTGlobal.obj" : $(SOURCE) $(DEP_CPP_XTGLO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTGlobal.obj"	"$(INTDIR)\XTGlobal.sbr" : $(SOURCE) $(DEP_CPP_XTGLO)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\XTHelpers.cpp
DEP_CPP_XTHEL=\
	"..\Src\Common.Src\XTHelpers.h"\
	
NODEP_CPP_XTHEL=\
	"..\Src\stdafx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTHelpers.obj" : $(SOURCE) $(DEP_CPP_XTHEL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTHelpers.obj" : $(SOURCE) $(DEP_CPP_XTHEL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTHelpers.obj" : $(SOURCE) $(DEP_CPP_XTHEL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTHelpers.obj"	"$(INTDIR)\XTHelpers.sbr" : $(SOURCE)\
 $(DEP_CPP_XTHEL) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\XTIconMap.cpp
DEP_CPP_XTICO=\
	"..\Include\XTIconMap.h"\
	
NODEP_CPP_XTICO=\
	"..\Src\stdafx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTIconMap.obj" : $(SOURCE) $(DEP_CPP_XTICO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTIconMap.obj" : $(SOURCE) $(DEP_CPP_XTICO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTIconMap.obj" : $(SOURCE) $(DEP_CPP_XTICO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTIconMap.obj"	"$(INTDIR)\XTIconMap.sbr" : $(SOURCE)\
 $(DEP_CPP_XTICO) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTImageManager.obj" : $(SOURCE) $(DEP_CPP_XTIMA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTImageManager.obj" : $(SOURCE) $(DEP_CPP_XTIMA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTImageManager.obj" : $(SOURCE) $(DEP_CPP_XTIMA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTImageManager.obj"	"$(INTDIR)\XTImageManager.sbr" : $(SOURCE)\
 $(DEP_CPP_XTIMA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMDIChildWnd.obj" : $(SOURCE) $(DEP_CPP_XTMDIC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMDIChildWnd.obj" : $(SOURCE) $(DEP_CPP_XTMDIC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMDIChildWnd.obj" : $(SOURCE) $(DEP_CPP_XTMDIC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMDIChildWnd.obj"	"$(INTDIR)\XTMDIChildWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTMDIC) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMDIFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTMDIF) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMDIFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTMDIF) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMDIFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTMDIF) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMDIFrameWnd.obj"	"$(INTDIR)\XTMDIFrameWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTMDIF) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMenuBar.obj" : $(SOURCE) $(DEP_CPP_XTMEN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMenuBar.obj" : $(SOURCE) $(DEP_CPP_XTMEN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMenuBar.obj" : $(SOURCE) $(DEP_CPP_XTMEN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMenuBar.obj"	"$(INTDIR)\XTMenuBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTMEN) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMenuBar1.obj" : $(SOURCE) $(DEP_CPP_XTMENU) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMenuBar1.obj" : $(SOURCE) $(DEP_CPP_XTMENU) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMenuBar1.obj" : $(SOURCE) $(DEP_CPP_XTMENU) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMenuBar1.obj"	"$(INTDIR)\XTMenuBar1.sbr" : $(SOURCE)\
 $(DEP_CPP_XTMENU) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTMiniDockFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTMIN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTMiniDockFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTMIN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTMiniDockFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTMIN) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTMiniDockFrameWnd.obj"	"$(INTDIR)\XTMiniDockFrameWnd.sbr" : \
$(SOURCE) $(DEP_CPP_XTMIN) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTNewToolbarDlg.obj" : $(SOURCE) $(DEP_CPP_XTNEW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTNewToolbarDlg.obj" : $(SOURCE) $(DEP_CPP_XTNEW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTNewToolbarDlg.obj" : $(SOURCE) $(DEP_CPP_XTNEW) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTNewToolbarDlg.obj"	"$(INTDIR)\XTNewToolbarDlg.sbr" : $(SOURCE)\
 $(DEP_CPP_XTNEW) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTOleIPFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTOLE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTOleIPFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTOLE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTOleIPFrameWnd.obj" : $(SOURCE) $(DEP_CPP_XTOLE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTOleIPFrameWnd.obj"	"$(INTDIR)\XTOleIPFrameWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTOLE) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTOptionsManager.obj" : $(SOURCE) $(DEP_CPP_XTOPT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTOptionsManager.obj" : $(SOURCE) $(DEP_CPP_XTOPT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTOptionsManager.obj" : $(SOURCE) $(DEP_CPP_XTOPT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTOptionsManager.obj"	"$(INTDIR)\XTOptionsManager.sbr" : $(SOURCE)\
 $(DEP_CPP_XTOPT) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTPopupTearOffWnd.obj" : $(SOURCE) $(DEP_CPP_XTPOP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTPopupTearOffWnd.obj" : $(SOURCE) $(DEP_CPP_XTPOP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTPopupTearOffWnd.obj" : $(SOURCE) $(DEP_CPP_XTPOP) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTPopupTearOffWnd.obj"	"$(INTDIR)\XTPopupTearOffWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTPOP) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTPreviewView.obj" : $(SOURCE) $(DEP_CPP_XTPRE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTPreviewView.obj" : $(SOURCE) $(DEP_CPP_XTPRE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTPreviewView.obj" : $(SOURCE) $(DEP_CPP_XTPRE) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTPreviewView.obj"	"$(INTDIR)\XTPreviewView.sbr" : $(SOURCE)\
 $(DEP_CPP_XTPRE) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTReBar.obj" : $(SOURCE) $(DEP_CPP_XTREB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTReBar.obj" : $(SOURCE) $(DEP_CPP_XTREB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTReBar.obj" : $(SOURCE) $(DEP_CPP_XTREB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTReBar.obj"	"$(INTDIR)\XTReBar.sbr" : $(SOURCE) $(DEP_CPP_XTREB)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTReBar1.obj" : $(SOURCE) $(DEP_CPP_XTREBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTReBar1.obj" : $(SOURCE) $(DEP_CPP_XTREBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTReBar1.obj" : $(SOURCE) $(DEP_CPP_XTREBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTReBar1.obj"	"$(INTDIR)\XTReBar1.sbr" : $(SOURCE) $(DEP_CPP_XTREBA)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\XTReBarCtrl.cpp
DEP_CPP_XTREBAR=\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTReBarCtrl.h"\
	
NODEP_CPP_XTREBAR=\
	"..\Src\stdafx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTReBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTREBAR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTReBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTREBAR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTReBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTREBAR) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTReBarCtrl.obj"	"$(INTDIR)\XTReBarCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTREBAR) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Include\XTResource.rc
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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTSkins.obj" : $(SOURCE) $(DEP_CPP_XTSKI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTSkins.obj" : $(SOURCE) $(DEP_CPP_XTSKI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTSkins.obj" : $(SOURCE) $(DEP_CPP_XTSKI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTSkins.obj"	"$(INTDIR)\XTSkins.sbr" : $(SOURCE) $(DEP_CPP_XTSKI)\
 "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTSplitterDock.obj" : $(SOURCE) $(DEP_CPP_XTSPLI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTSplitterDock.obj" : $(SOURCE) $(DEP_CPP_XTSPLI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTSplitterDock.obj" : $(SOURCE) $(DEP_CPP_XTSPLI) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTSplitterDock.obj"	"$(INTDIR)\XTSplitterDock.sbr" : $(SOURCE)\
 $(DEP_CPP_XTSPLI) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTTabCtrlBar.obj" : $(SOURCE) $(DEP_CPP_XTTABCT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTTabCtrlBar.obj" : $(SOURCE) $(DEP_CPP_XTTABCT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTTabCtrlBar.obj" : $(SOURCE) $(DEP_CPP_XTTABCT) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTTabCtrlBar.obj"	"$(INTDIR)\XTTabCtrlBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTABCT) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTToolBar.obj" : $(SOURCE) $(DEP_CPP_XTTOO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTToolBar.obj" : $(SOURCE) $(DEP_CPP_XTTOO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTToolBar.obj" : $(SOURCE) $(DEP_CPP_XTTOO) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTToolBar.obj"	"$(INTDIR)\XTToolBar.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTOO) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTToolBar1.obj" : $(SOURCE) $(DEP_CPP_XTTOOL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTToolBar1.obj" : $(SOURCE) $(DEP_CPP_XTTOOL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTToolBar1.obj" : $(SOURCE) $(DEP_CPP_XTTOOL) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTToolBar1.obj"	"$(INTDIR)\XTToolBar1.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTOOL) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\Src\XTToolBarCtrl.cpp
DEP_CPP_XTTOOLB=\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Include\XTToolBarCtrl.h"\
	
NODEP_CPP_XTTOOLB=\
	"..\Src\stdafx.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTToolBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTOOLB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTToolBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTOOLB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTToolBarCtrl.obj" : $(SOURCE) $(DEP_CPP_XTTOOLB) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTToolBarCtrl.obj"	"$(INTDIR)\XTToolBarCtrl.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTOOLB) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTToolBarPopupWnd.obj" : $(SOURCE) $(DEP_CPP_XTTOOLBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTToolBarPopupWnd.obj" : $(SOURCE) $(DEP_CPP_XTTOOLBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTToolBarPopupWnd.obj" : $(SOURCE) $(DEP_CPP_XTTOOLBA) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTToolBarPopupWnd.obj"	"$(INTDIR)\XTToolBarPopupWnd.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTOOLBA) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTToolCmdUI.obj" : $(SOURCE) $(DEP_CPP_XTTOOLC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTToolCmdUI.obj" : $(SOURCE) $(DEP_CPP_XTTOOLC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTToolCmdUI.obj" : $(SOURCE) $(DEP_CPP_XTTOOLC) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTToolCmdUI.obj"	"$(INTDIR)\XTToolCmdUI.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTOOLC) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\XTToolkit_Dll.cpp
DEP_CPP_XTTOOLK=\
	"..\Include\XTDefines.h"\
	"..\Include\XTGlobal.h"\
	"..\Src\XTPriv.h"\
	".\StdAfx.h"\
	{$(INCLUDE)}"..\src\afximpl.h"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTToolkit_Dll.obj" : $(SOURCE) $(DEP_CPP_XTTOOLK) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTToolkit_Dll.obj" : $(SOURCE) $(DEP_CPP_XTTOOLK) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTToolkit_Dll.obj" : $(SOURCE) $(DEP_CPP_XTTOOLK) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTToolkit_Dll.obj"	"$(INTDIR)\XTToolkit_Dll.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTOOLK) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"


!ENDIF 

SOURCE=.\XTToolkit_Dll.rc
DEP_RSC_XTTOOLKI=\
	"..\Include\res\btn_bar.bmp"\
	"..\Include\res\btn_frame.bmp"\
	"..\Include\res\btn_listedit.bmp"\
	"..\Include\res\btn_out.bmp"\
	"..\Include\res\btn_print.bmp"\
	"..\Include\res\checklist.bmp"\
	"..\Include\res\checklist_3d.bmp"\
	"..\Include\res\dragcopy.cur"\
	"..\Include\res\dragmove.cur"\
	"..\Include\res\dragnone.cur"\
	"..\Include\res\font.bmp"\
	"..\Include\res\hand.cur"\
	"..\Include\res\handnone.cur"\
	"..\Include\res\lightbulb.bmp"\
	"..\Include\res\scroll.bmp"\
	"..\Include\res\tbar_blank.bmp"\
	"..\Include\res\tbar_hdr.bmp"\
	"..\Include\res\tbar_hex.bmp"\
	"..\Include\res\tbar_out.bmp"\
	"..\Include\res\tbar_preview.bmp"\
	"..\Include\res\xtreme.ico"\
	"..\Include\XTResource.rc"\
	".\res\XTToolkit_Dll.rc2"\
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTToolkit_Dll.res" : $(SOURCE) $(DEP_RSC_XTTOOLKI) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\XTToolkit_Dll.res" /i "..\Include" /d "NDEBUG"\
 /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTToolkit_Dll.res" : $(SOURCE) $(DEP_RSC_XTTOOLKI) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\XTToolkit_Dll.res" /i "..\Include" /d "_DEBUG"\
 /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTToolkit_Dll.res" : $(SOURCE) $(DEP_RSC_XTTOOLKI) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\XTToolkit_Dll.res" /i "..\Include" /d "NDEBUG"\
 /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTToolkit_Dll.res" : $(SOURCE) $(DEP_RSC_XTTOOLKI) "$(INTDIR)"
	$(RSC) /l 0x1009 /fo"$(INTDIR)\XTToolkit_Dll.res" /i "..\Include" /d "_DEBUG"\
 /d "_AFXDLL" $(SOURCE)


!ENDIF 

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
	

!IF  "$(CFG)" == "XTToolkit_Dll - Win32 Release"


"$(INTDIR)\XTToolsManager.obj" : $(SOURCE) $(DEP_CPP_XTTOOLS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Debug"


"$(INTDIR)\XTToolsManager.obj" : $(SOURCE) $(DEP_CPP_XTTOOLS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Release"


"$(INTDIR)\XTToolsManager.obj" : $(SOURCE) $(DEP_CPP_XTTOOLS) "$(INTDIR)"\
 "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "XTToolkit_Dll - Win32 Unicode Debug"


"$(INTDIR)\XTToolsManager.obj"	"$(INTDIR)\XTToolsManager.sbr" : $(SOURCE)\
 $(DEP_CPP_XTTOOLS) "$(INTDIR)" "$(INTDIR)\XTToolkit_Dll_vc50.pch"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 


!ENDIF 

