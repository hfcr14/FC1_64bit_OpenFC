# Microsoft Developer Studio Project File - Name="XTToolkit_Lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=XTToolkit_Lib - Win32 Dynamic Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XTToolkit_Lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XTToolkit_Lib.mak" CFG="XTToolkit_Lib - Win32 Dynamic Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XTToolkit_Lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Dynamic Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Dynamic Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Dynamic Unicode Release" (based on "Win32 (x86) Static Library")
!MESSAGE "XTToolkit_Lib - Win32 Dynamic Unicode Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XTToolkit_Lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W4 /WX /GX /O2 /I "..\include" /I "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK" /Yu"StdAfx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../Lib_vc60/XT4100LibStatic.lib"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W4 /GX /ZI /Od /I "..\include" /I "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK" /Yu"StdAfx.h" /FD /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../Lib_vc60/XT4100LibStaticd.lib"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XTToolki"
# PROP BASE Intermediate_Dir "XTToolki"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Unicode_Release"
# PROP Intermediate_Dir "Unicode_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MT /W4 /WX /GX /O2 /I "..\include" /I "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK" /Yu"StdAfx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Lib\XT4100LibStatic.lib"
# ADD LIB32 /nologo /out:"../Lib_vc60/XT4100LibStaticu.lib"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XTToolk0"
# PROP BASE Intermediate_Dir "XTToolk0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Unicode_Debug"
# PROP Intermediate_Dir "Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GR /GX /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MTd /W4 /WX /GX /ZI /Od /I "..\include" /I "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D "_UNICODE" /D "WIN32" /D "_WINDOWS" /D "_XT_STATICLINK" /Yu"StdAfx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Lib\XT4100LibStaticd.lib"
# ADD LIB32 /nologo /out:"../Lib_vc60/XT4100LibStaticud.lib"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XTToolki"
# PROP BASE Intermediate_Dir "XTToolki"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Dynamic_Release"
# PROP Intermediate_Dir "Dynamic_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MD /W4 /WX /GX /I "..\include" /I "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB" /Yu"StdAfx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../Lib_vc60/XT4100LibStatic.lib"
# ADD LIB32 /nologo /out:"../Lib_vc60/XT4100LibDynStatic.lib"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XTToolk0"
# PROP BASE Intermediate_Dir "XTToolk0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Dynamic_Debug"
# PROP Intermediate_Dir "Dynamic_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GR /GX /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /WX /GX /ZI /Od /I "..\include" /I "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB" /Yu"StdAfx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../Lib_vc60/XT4100LibStaticd.lib"
# ADD LIB32 /nologo /out:"../Lib_vc60/XT4100LibDynStaticd.lib"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XTToolk1"
# PROP BASE Intermediate_Dir "XTToolk1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Dynamic_Unicode_Release"
# PROP Intermediate_Dir "Dynamic_Unicode_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GR /GX /O2 /I "../Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MD /W4 /WX /GX /I "..\include" /I "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "NDEBUG" /D "_UNICODE" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB" /Yu"StdAfx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../Lib_vc60/XT4100LibStaticu.lib"
# ADD LIB32 /nologo /out:"../Lib_vc60/XT4100LibDynStaticu.lib"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "XTToolk2"
# PROP BASE Intermediate_Dir "XTToolk2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Dynamic_Unicode_Debug"
# PROP Intermediate_Dir "Dynamic_Unicode_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GR /GX /Od /I "../Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /Yu"StdAfx.h" /FD /c
# ADD CPP /nologo /MDd /W4 /WX /GX /ZI /Od /I "..\include" /I "..\include\Common.include" /I "..\src" /I "..\src\Common.src" /D "_DEBUG" /D "_UNICODE" /D "_AFXDLL" /D "WIN32" /D "_WINDOWS" /D "_XT_DYN_STATICLIB" /Yu"StdAfx.h" /FD /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../Lib_vc60/XT4100LibStaticud.lib"
# ADD LIB32 /nologo /out:"../Lib_vc60/XT4100LibDynStaticud.lib"

!ENDIF 

# Begin Target

# Name "XTToolkit_Lib - Win32 Release"
# Name "XTToolkit_Lib - Win32 Debug"
# Name "XTToolkit_Lib - Win32 Unicode Release"
# Name "XTToolkit_Lib - Win32 Unicode Debug"
# Name "XTToolkit_Lib - Win32 Dynamic Release"
# Name "XTToolkit_Lib - Win32 Dynamic Debug"
# Name "XTToolkit_Lib - Win32 Dynamic Unicode Release"
# Name "XTToolkit_Lib - Win32 Dynamic Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Src\Common.Src\XTAnimationMemDC.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTBrowseDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTBrowseEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTButton.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTCaption.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTCaptionPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTCheckListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTColorDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTColorPageCustom.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTColorPageStandard.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTColorPicker.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTColorPopup.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTColorRef.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTColorSelectorCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTComboBoxEx.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTDateTimeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTDirWatcher.cpp
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTDirWatcher.h
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTDropSource.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTEditListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTFlatComboBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTFlatHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTFlatTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTFontCombo.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTHelpers.h
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTHexEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTHtmlView.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTListView.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTLoadSysColorBitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTLogoPane.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTMaskEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTMDIWndTab.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTMemDC.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTMemFile.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTOSVersionInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTOutBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTOutlookBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTPagerCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTRegistryManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTResize.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTResizeDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTResizeFormView.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTResizePoint.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTResizePropertyPage.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTResizePropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTResizeRect.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTSearchOptionsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTShellListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTShellListView.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTShellPidl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTShellSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTShellTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTShellTreeView.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTSortClass.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTabCtrlButtons.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTabCtrlButtons.h
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTabView.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTipOfTheDay.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTipWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTrayIcon.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTTreeView.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTWindowList.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTWindowList.h
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTWindowPos.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTWndHook.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\Common.Src\XTWndShadow.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "XTToolkit_Lib - Win32 Release"

# ADD CPP /Yc"StdAfx.h"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Debug"

# ADD CPP /Zi /Yc"StdAfx.h"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Unicode Release"

# ADD BASE CPP /Yc"StdAfx.h"
# ADD CPP /Yc"StdAfx.h"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Unicode Debug"

# ADD BASE CPP /Z7 /Yc"StdAfx.h"
# ADD CPP /Zi /Yc"StdAfx.h"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Release"

# ADD BASE CPP /Yc"StdAfx.h"
# ADD CPP /Yc"StdAfx.h"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Debug"

# ADD BASE CPP /Z7 /Yc"StdAfx.h"
# ADD CPP /Zi /Yc"StdAfx.h"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Unicode Release"

# ADD BASE CPP /Yc"StdAfx.h"
# ADD CPP /Yc"StdAfx.h"

!ELSEIF  "$(CFG)" == "XTToolkit_Lib - Win32 Dynamic Unicode Debug"

# ADD BASE CPP /Z7 /Yc"StdAfx.h"
# ADD CPP /Zi /Yc"StdAfx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\Src\XTAccelKeyEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTAccelManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTCBarDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTCommandsListBox.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTControlBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTCoolMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTCrc32.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTCustomizeAPI.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTCustomizePage.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTCustomizeSheet.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTCustomToolBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTDialogBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTDockBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTDockColorSelector.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTDockContext.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTDockState.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTDockWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTExpButton.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTExpMenuWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTFrameImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTGlobal.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTHelpers.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTIconMap.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTImageManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTMDIChildWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTMDIFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTMenuBar1.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTMiniDockFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTNewToolbarDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTOleIPFrameWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTOptionsManager.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTPopupTearOffWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTPreviewView.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTReBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTReBar1.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTReBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTSkins.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTSplitterDock.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTTabCtrlBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTToolBar.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTToolBar1.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTToolBarCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTToolBarPopupWnd.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTToolCmdUI.cpp
# End Source File
# Begin Source File

SOURCE=..\Src\XTToolsManager.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Common.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Include\Common.Include\XTAnimationMemDC.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTBrowseDialog.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTBrowseEdit.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTButton.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTCaption.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTCaptionPopupWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTCheckListBox.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTColorDialog.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTColorPageCustom.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTColorPageStandard.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTColorPicker.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTColorPopup.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTColorRef.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTColorSelectorCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTComboBoxEx.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTDateTimeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTDropSource.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTEditListBox.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTFlatComboBox.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTFlatHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTFlatTabCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTFontCombo.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTFunctions.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTHexEdit.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTHtmlView.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTHyperLink.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTListView.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTLogoPane.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTMaskEdit.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTMDIWndTab.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTMemDC.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTMemFile.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTOSVersionInfo.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTOutBarCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTOutlookBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTPagerCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTRegistryManager.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTResize.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTResizeDialog.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTResizeFormView.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTResizePoint.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTResizePropertyPage.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTResizePropertySheet.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTResizeRect.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTSearchOptionsCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTShellListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTShellListView.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTShellPidl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTShellSettings.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTShellTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTShellTreeView.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTSortClass.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTStatusBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTTabCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTTabCtrlBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTTabView.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTTipOfTheDay.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTTipWindow.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTTrayIcon.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTTreeView.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTWindowPos.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTWndHook.h
# End Source File
# Begin Source File

SOURCE=..\Include\Common.Include\XTWndShadow.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTAccelKeyEdit.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTAccelManager.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTCallbacks.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTCBarDialog.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTCommandsListBox.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTControlBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTCoolMenu.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTCrc32.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTCustomizeAPI.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTCustomizePage.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTCustomizeSheet.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTCustomToolbar.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTDefines.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTDelegate.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTDialogBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTDockBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTDockColorSelector.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTDockContext.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTDockState.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTDockWindow.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTDrawHelpers.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTExpButton.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTExpMenuWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTFrameImpl.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTFrameWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTGlobal.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTIconMap.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTImageManager.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTIncludes.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTMDIChildWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTMDIFrameWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTMenuBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTMiniDockFrameWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTNewToolbarDlg.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTOleIPFrameWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTOptionsManager.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTPopupTearOffWnd.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTPreviewView.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTPriv.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTReBar.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTReBarBandInfo.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTReBarCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTRegistryBlob.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTResource.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTSkins.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTSplitterDock.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTToolBar.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTToolBarCtrl.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTToolbarHelpers.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTToolBarPopupWnd.h
# End Source File
# Begin Source File

SOURCE=..\Src\XTToolCmdUI.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTToolkit.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTToolsManager.h
# End Source File
# Begin Source File

SOURCE=..\Include\XTVersion.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\Include\res\btn_bar.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\btn_frame.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\btn_listedit.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\btn_out.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\btn_print.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\checklist.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\dragcopy.cur
# End Source File
# Begin Source File

SOURCE=..\Include\res\dragmove.cur
# End Source File
# Begin Source File

SOURCE=..\Include\res\dragnone.cur
# End Source File
# Begin Source File

SOURCE=..\Include\res\font.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\hand.cur
# End Source File
# Begin Source File

SOURCE=..\Include\res\handnone.cur
# End Source File
# Begin Source File

SOURCE=..\Include\res\lightbulb.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\scroll.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\splith.cur
# End Source File
# Begin Source File

SOURCE=..\Include\res\splitv.cur
# End Source File
# Begin Source File

SOURCE=..\Include\res\tbar_blank.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\tbar_hdr.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\tbar_hex.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\tbar_out.bmp
# End Source File
# Begin Source File

SOURCE=..\Include\res\xtreme.ico
# End Source File
# End Group
# Begin Source File

SOURCE=..\XTToolkit_Dll\License_Agreement.txt
# End Source File
# End Target
# End Project
