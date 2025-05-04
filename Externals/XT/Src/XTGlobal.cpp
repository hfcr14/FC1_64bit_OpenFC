// XTGlobal.cpp : implementation of the XT_AUX_DATA struct.
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
#include "XTVersion.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTColorPageCustom.h"
#include "XTColorRef.h"
#include "XTPriv.h"
#include "XTOSVersionInfo.h"
#include "XTWndHook.h"
#include "XTDelegate.h"
#include "XTControlBar.h"

#include <shlwapi.h>  // used for DLLVERSIONINFO

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define xtVerInfo CXTOSVersionInfo::Get()

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XT_AUX_DATA::XT_AUX_DATA()
{
	strINIFileName = _T("");
	iComCtlVersion = -1;

	bWin95 = xtVerInfo.IsWin95();
	bWin98 = xtVerInfo.IsWin98();
	bWinNT = xtVerInfo.IsWinNT4();
	bWin2K = xtVerInfo.IsWin2K();
	bWinXP = xtVerInfo.IsWinXP();

	bUseOfficeFont      = FALSE;
	bMenuShadows        = TRUE;
	nMenuAnimation      = 0;		// no animation
	bXPMode             = FALSE;
	bCoolMenuCompatMode = FALSE;
	bControlBarMenus    = TRUE;
	bDockBarMenus       = TRUE;
	bToolBarVisualize   = TRUE;
	bMenuRecentCommands = TRUE;
	bShowFullAfterDelay = TRUE;
	bToolBarScreenTips  = TRUE;
	bToolBarAccelTips   = FALSE;

	bUseSolidShadows = FALSE;
	bDisableFadedIcons = FALSE;

#if defined (_XT_STATICLINK) || defined (_XT_DYN_STATICLIB)
#if defined (_XT_USES_RESOURCE_DLL)
#pragma message( "XTGlobal.cpp: ### WARNING: You must call xtAfxData.InitResources(HINSTANCE hInst) to initialize Xtreme Toolkit resources!" )
#else
	InitResources(NULL);
#endif // defined (_XT_USES_RESOURCE_DLL)
#endif // defined (_XT_STATICLINK) || defined (_XT_DYN_STATICLIB)
}

XT_AUX_DATA::~XT_AUX_DATA()
{
	FreeSysFonts();
	FreeSysCursors();
}

void XT_AUX_DATA::InitResources(HINSTANCE hInst)
{
	// Set the resource handle for the toolkit.
	if (hInst == NULL) {
		hInst = ::GetModuleHandle(0);
	}

	ASSERT(hInst != NULL);
	hInstance = hInst;

	UpdateSysColors();
	UpdateSysMetrics();

	LoadSysFonts();
	LoadSysCursors();

	// Verify correct version of Comctl32.dll.
	GetComCtlVersion();

	if (iComCtlVersion < VERSION_IE401)
	{
		TCHAR szMessage[256];
		::LoadString(hInstance, XT_IDS_INVALID_VER, szMessage, _countof(szMessage));
		::MessageBox(NULL, szMessage, _T("Comctl32.dll"), MB_ICONSTOP | MB_OK);
		::TerminateProcess( ::GetCurrentProcess( ), 1 );
	}
}

void XT_AUX_DATA::LoadSysCursors()
{
	FreeSysCursors();

	// Load the cursors used by the Xtreme Toolkit.
	hcurDragNone = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGNONE));

	// If your static linked application asserts
	// here you most likely need to include the
	// following line in your .rc2 file:
	// #include "XTResource.rc"
	ASSERT(hcurDragNone != NULL);

	hcurDragCopy = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGCOPY));
	ASSERT(hcurDragCopy != NULL);

	hcurDragMove = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_DRAGMOVE));
	ASSERT(hcurDragMove != NULL);

	hcurHand = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_HAND));
	ASSERT(hcurHand != NULL);

	hcurHandNone = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_HANDNONE));
	ASSERT(hcurHandNone != NULL);

	hcurHSplitBar = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_HSPLITBAR));
	ASSERT(hcurHSplitBar != NULL);

	hcurVSplitBar = ::LoadCursor(hInstance,
		MAKEINTRESOURCE(XT_IDC_VSPLITBAR));
	ASSERT(hcurVSplitBar != NULL);

	hcurMove = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL));
	ASSERT(hcurMove != NULL);
}

void XT_AUX_DATA::FreeSysCursors()
{
	// Free the cursors used by the Xtreme Toolkit.
	if (hcurDragNone != NULL) {
		::DestroyCursor(hcurDragNone);
	}
	if (hcurDragCopy != NULL) {
		::DestroyCursor(hcurDragCopy);
	}
	if (hcurDragMove != NULL) {
		::DestroyCursor(hcurDragMove);
	}
	if (hcurHand != NULL) {
		::DestroyCursor(hcurHand);
	}
	if (hcurHandNone != NULL) {
		::DestroyCursor(hcurHandNone);
	}
	if (hcurHSplitBar != NULL) {
		::DestroyCursor(hcurHSplitBar);
	}
	if (hcurVSplitBar != NULL) {
		::DestroyCursor(hcurVSplitBar);
	}
}

static int CALLBACK EnumFontFamExProc(ENUMLOGFONTEX* pelf,
	NEWTEXTMETRICEX* lpntm, int FontType, LPVOID pThis)
{
	UNREFERENCED_PARAMETER( lpntm );
	UNREFERENCED_PARAMETER( FontType );

	CString strFontName = *( CString* )pThis;
	CString strFaceName = pelf->elfLogFont.lfFaceName;

	if ( strFaceName.CompareNoCase( strFontName ) == 0 )
	{
		return 0;
	}

	return 1;
}

void XT_AUX_DATA::SetGlobalFont(LPCTSTR lpszFaceName, LPCTSTR lpszVertFaceName/*=NULL*/)
{
    XT_NONCLIENTMETRICS ncm;

    if ( lpszFaceName != NULL )
        _tcscpy( ncm.lfMenuFont.lfFaceName, lpszFaceName );

    CFont fontHorz;
    fontHorz.CreateFontIndirect(&ncm.lfMenuFont);

    if ( lpszVertFaceName != NULL )
        _tcscpy( ncm.lfMenuFont.lfFaceName, lpszVertFaceName );

    CFont fontVert;
    fontVert.CreateFontIndirect(&ncm.lfMenuFont);

    SetGlobalFont(&fontHorz, &fontVert);
}

void XT_AUX_DATA::SetGlobalFont(CFont* pFont, CFont* pVertFont/*=NULL*/)
{
    if ( pFont != NULL && pFont->m_hObject != NULL )
    {
        XT_LOGFONT lf;
        pFont->GetLogFont( &lf );

        // Create the toolkit horizontal fonts.
        CreateSysFont( lf, fontBold, FW_BOLD );
        CreateSysFont( lf, font );
        CreateSysFont( lf, fontULine, FW_NORMAL, 1 );
        CreateSysFont( lf, fontHCapt );
    }

    if ( pVertFont != NULL && pVertFont->m_hObject != NULL )
    {
        XT_LOGFONT lf;
        pVertFont->GetLogFont( &lf );

        // Create the toolkit vertical fonts.
        CreateSysFont( lf, fontVCapt, FW_NORMAL, 0, 900, 900 );
        CreateSysFont( lf, fontVCaptBold, FW_BOLD, 0, 900, 900 );
        CreateSysFont( lf, fontVCaptR, FW_NORMAL, 0, 2700, 2700 );
        CreateSysFont( lf, fontVCaptRBold, FW_BOLD, 0, 2700, 2700 );
    }
}

bool XT_AUX_DATA::FontExists(CString& strFaceName)
{
	// Enumerate all styles and charsets of all fonts:
	XT_LOGFONT lfEnum;
	lfEnum.lfFaceName[ 0 ] = 0;
	lfEnum.lfCharSet = DEFAULT_CHARSET;

	// See if lpszFaceName font exists, if so, then use it...
	CWindowDC dc( NULL );
	if ( ::EnumFontFamiliesEx(
		dc.m_hDC, &lfEnum, ( FONTENUMPROC )EnumFontFamExProc, ( LPARAM )&strFaceName, 0 ) == 0 )
	{
		return true;
	}

	return false;
}

bool XT_AUX_DATA::CreateSysFont(XT_LOGFONT& logFont, CFont& font, long lfWeight/*=-1*/, char lfUnderline/*=-1*/, long lfOrientation/*=-1*/, long lfEscapement/*=-1*/)
{
	XT_LOGFONT lf(logFont);

	if (lfWeight != -1) {
		lf.lfWeight = lfWeight;
	}
	if (lfOrientation != -1) {
		lf.lfOrientation = lfOrientation;
	}
	if (lfEscapement != -1) {
		lf.lfEscapement = lfEscapement;
	}
	if (lfUnderline != -1) {
		lf.lfUnderline = lfUnderline;
	}

	if (font.GetSafeHandle()) {
		font.DeleteObject();
	}

	if ( !font.CreateFontIndirect( &lf ) ) {
		return false;
	}

	return true;
}

void XT_AUX_DATA::UseOfficeFont(bool bUse)
{
    bUseOfficeFont = bUse;
    LoadSysFonts();
}

void XT_AUX_DATA::LoadSysFonts()
{
    CWindowDC dc(NULL);

	// Free up any resources previously allocated.
	FreeSysFonts();

    XT_LOGFONT lf;
    lf.lfCharSet = (BYTE)::GetTextCharsetInfo(dc, NULL, 0);

	// Set the character set to default.
	XT_NONCLIENTMETRICS ncm;
    lf.lfHeight = ncm.lfMenuFont.lfHeight < 0 ? __min(-11, ncm.lfMenuFont.lfHeight) : ncm.lfMenuFont.lfHeight;
	lf.lfWeight = ncm.lfMenuFont.lfWeight;	
	lf.lfItalic = ncm.lfMenuFont.lfItalic;

	_tcscpy (lf.lfFaceName, ncm.lfMenuFont.lfFaceName);

    // see if the Tahoma font was found.
	CString strOfficeFont(_T("Tahoma"));
	bool bTahomaExists = FontExists( strOfficeFont );

    // see if the Arial font was found.
	CString strArialFont(_T("Arial"));
    bool bArialExists = FontExists(strArialFont);

    // see if the system font is set to the Tahoma font, if so set the office font flag to true.
    bool bTahomaLF = strOfficeFont.CompareNoCase(ncm.lfMenuFont.lfFaceName) == 0;
    if (bTahomaLF) {
        bUseOfficeFont = true;
    }
	
    // set the office font if applicable.
	BOOL bUseSystemFont = (ncm.lfMenuFont.lfCharSet > SYMBOL_CHARSET);
	if (!bUseSystemFont && !bTahomaLF)
    {
        if (bUseOfficeFont && bTahomaExists) {
	        _tcscpy( lf.lfFaceName, strOfficeFont );
        }
    }

    // Create the default GUI bold font.
	XT_LOGFONT lfHorz( lf );
	CreateSysFont( lfHorz, fontBold, FW_BOLD );

	// Create the default GUI font.
	CreateSysFont( lfHorz, font );

	// Create the default GUI underlined font.
	CreateSysFont( lfHorz, fontULine, -1, 1 );

	// Create the horizontal caption font.
	CreateSysFont( lfHorz, fontHCapt );

    // If Tahoma font is not used, use Arial for vertical fonts, it displays better on older
    // operating systems than MS Sans Serif.
    if (!bUseSystemFont && !bTahomaLF)
    {
        if (!bUseOfficeFont && bArialExists) {
            _tcscpy( lf.lfFaceName, strArialFont );
        }
    }

	// Create the vertical caption font.
	XT_LOGFONT lfVert( lf );
	CreateSysFont( lfVert, fontVCapt, -1, -1, 900, 900 );

	// Create the bold vertical caption font.
	CreateSysFont( lfVert, fontVCaptBold, FW_BOLD, -1, 900, 900 );

	// Create the right aligned vertical caption font.
	CreateSysFont( lfVert, fontVCaptR, -1, -1, 2700, 2700 );

	// Create the bold right aligned vertical caption font.
	CreateSysFont( lfVert, fontVCaptRBold, FW_BOLD, -1, 2700, 2700 );
}

void XT_AUX_DATA::FreeSysFonts()
{
    if (font.GetSafeHandle())
	    font.DeleteObject();
    if (fontBold.GetSafeHandle())
	    fontBold.DeleteObject();
	if (fontULine.GetSafeHandle())
        fontULine.DeleteObject();
	if (fontHCapt.GetSafeHandle())
        fontHCapt.DeleteObject();
	if (fontVCapt.GetSafeHandle())
        fontVCapt.DeleteObject();
	if (fontVCaptBold.GetSafeHandle())
        fontVCaptBold.DeleteObject();
	if (fontVCaptR.GetSafeHandle())
        fontVCaptR.DeleteObject();
	if (fontVCaptRBold.GetSafeHandle())
        fontVCaptRBold.DeleteObject();
}

void XT_AUX_DATA::SetINIFileName(LPCTSTR lpszINIFullPath)
{
	strINIFileName = lpszINIFullPath;
}

LPCTSTR XT_AUX_DATA::GetINIFileName()
{
	return strINIFileName;
}

XT_AUX_DATA& XT_AUX_DATA::Get()
{
	static XT_AUX_DATA xtauxdata;
	return xtauxdata;
}

void XT_AUX_DATA::UpdateSysColors()	
{
	CXTColorRef::RefreshColors();

	// Initialize standard color for windows components.
	clr3DFace                   = CXTColorRef::GetColor( COLOR_3DFACE );
	clr3DShadow					= CXTColorRef::GetColor( COLOR_3DSHADOW );
	clr3DDkShadow				= CXTColorRef::GetColor( COLOR_3DDKSHADOW );
	clr3DHilight				= CXTColorRef::GetColor( COLOR_3DHILIGHT );
	clrBtnText					= CXTColorRef::GetColor( COLOR_BTNTEXT );
	clrGrayText					= CXTColorRef::GetColor( COLOR_GRAYTEXT );
	clrHighlight				= CXTColorRef::GetColor( COLOR_HIGHLIGHT );
	clrHighlightText			= CXTColorRef::GetColor( COLOR_HIGHLIGHTTEXT );
	clrMenu						= CXTColorRef::GetColor( COLOR_MENU );
	clrMenuText					= CXTColorRef::GetColor( COLOR_MENUTEXT );
	clrWindow					= CXTColorRef::GetColor( COLOR_WINDOW );
	clrWindowFrame				= CXTColorRef::GetColor( COLOR_WINDOWFRAME );
	clrWindowText				= CXTColorRef::GetColor( COLOR_WINDOWTEXT );
	clrActiveCaption		    = CXTColorRef::GetColor( COLOR_ACTIVECAPTION );
	clrInActiveCaption          = CXTColorRef::GetColor( COLOR_INACTIVECAPTION );
	clrGradActiveCapt           = CXTColorRef::GetColor( COLOR_GRADIENTACTIVECAPTION );
	clrGradInActiveCapt         = CXTColorRef::GetColor( COLOR_GRADIENTINACTIVECAPTION );
	clrActiveCaptText           = CXTColorRef::GetColor( COLOR_CAPTIONTEXT );
	clrInactiveCaptText         = CXTColorRef::GetColor( COLOR_INACTIVECAPTIONTEXT );

	// For some strange reason Windows 2000/NT does not return
	// the expected RGB value for COLOR_3DLIGHT...
	clr3DLight					= CXTColorRef(clr3DFace) + 15;

	// Initialize special colors for XP style interfaces.
	clrXPBarFace				= CXTColorRef::GetColor( XPCOLOR_BARFACE );
	clrXPHighlight				= CXTColorRef::GetColor( XPCOLOR_HIGHLIGHT );
	clrXPHighlightBorder		= CXTColorRef::GetColor( XPCOLOR_HIGHLIGHT_BORDER );
	clrXPHighlightPushed		= CXTColorRef::GetColor( XPCOLOR_HIGHLIGHT_PUSHED );
	clrXPIconShadow				= CXTColorRef::GetColor( XPCOLOR_ICONSHADDOW );
	clrXPGrayText				= CXTColorRef::GetColor( XPCOLOR_GRAYTEXT );
	clrXPHighlightChecked		= CXTColorRef::GetColor( XPCOLOR_HIGHLIGHT_CHECKED );
	clrXPHighlightCheckedBorder	= CXTColorRef::GetColor( XPCOLOR_HIGHLIGHT_CHECKED_BORDER );
	clrXPGripper				= CXTColorRef::GetColor( XPCOLOR_GRIPPER );
	clrXPSeparator				= CXTColorRef::GetColor( XPCOLOR_SEPARATOR );
	clrXPDisabled				= CXTColorRef::GetColor( XPCOLOR_DISABLED );
	clrXPMenuTextBack			= CXTColorRef::GetColor( XPCOLOR_MENUTEXT_BACK );
	clrXPMenuExpanded           = CXTColorRef::GetColor( XPCOLOR_MENU_EXPANDED );
	clrXPMenuBorder				= CXTColorRef::GetColor( XPCOLOR_MENU_BORDER );
	clrXPMenuText				= CXTColorRef::GetColor( XPCOLOR_MENUTEXT );
	clrXPHighlightText			= CXTColorRef::GetColor( XPCOLOR_HIGHLIGHT_TEXT );
	clrXPBarText				= CXTColorRef::GetColor( XPCOLOR_BARTEXT );
	clrXPBarTextPushed			= CXTColorRef::GetColor( XPCOLOR_BARTEXT_PUSHED );
	clrXPTabInactiveBack		= CXTColorRef::GetColor( XPCOLOR_TAB_INACTIVE_BACK );
	clrXPTabInactiveText		= CXTColorRef::GetColor( XPCOLOR_TAB_INACTIVE_TEXT );
}

void XT_AUX_DATA::UpdateSysMetrics()
{
	cxSmIcon   = ::GetSystemMetrics(SM_CXSMICON);
	cySmIcon   = ::GetSystemMetrics(SM_CYSMICON);
	cxSize	   = 18/*::GetSystemMetrics(SM_CXSIZE)*/;
	cySize     = 18/*::GetSystemMetrics(SM_CYSIZE)*/;
	cxHThumb   = ::GetSystemMetrics(SM_CXHTHUMB);
	cyVThumb   = ::GetSystemMetrics(SM_CYVTHUMB);
	cyMenuItem = ::GetSystemMetrics(SM_CYMENU);
}

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxDrawEmbossed
/////////////////////////////////////////////////////////////////////////////
_XT_EXT_CLASS void AFXAPI _xtAfxDrawEmbossed(CDC* pDC, CImageList& imageList, int nIndex, CPoint point, BOOL bInColor)
{
	IMAGEINFO imgInfo;
	VERIFY(imageList.GetImageInfo(0, &imgInfo));
	CRect rcImage = imgInfo.rcImage;

	// create memory dc
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	// create mono or color bitmap
	CBitmap bitmap;
	if (bInColor) {
		bitmap.CreateCompatibleBitmap(pDC, rcImage.Width(), rcImage.Height());
	} else {
		bitmap.CreateBitmap(rcImage.Width(), rcImage.Height(), 1, 1, NULL);
	}

	// draw image into memory DC--fill BG white first
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rcImage.Width(), rcImage.Height(), WHITENESS);
	imageList.Draw(&memDC, nIndex, CPoint(0,0), ILD_TRANSPARENT);

	// Draw using highlight offset by (1,1), then shadow
	CBrush brShadow(xtAfxData.clr3DShadow);
	CBrush brHilite(xtAfxData.clr3DHilight);

	CBrush* pOldBrush = pDC->SelectObject(&brHilite);
	pDC->BitBlt(point.x+1, point.y+1, rcImage.Width(), rcImage.Height(), &memDC, 0, 0, 0xB8074A);
	
	pDC->SelectObject(&brShadow);
	pDC->BitBlt(point.x, point.y, rcImage.Width(), rcImage.Height(), &memDC, 0, 0, 0xB8074A);

	pDC->SelectObject(pOldBrush);
	memDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
}
/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxDrawShadedRect
/////////////////////////////////////////////////////////////////////////////
_XT_EXT_CLASS void AFXAPI _xtAfxDrawShadedRect(CDC *pDC, CRect& rect)
{
	// Bit pattern for a monochrome brush with every
	// other pixel turned off
	WORD bits[] = {
		0x0055, 0x00AA, 0x0055, 0x00AA,
		0x0055, 0x00AA, 0x0055, 0x00AA
	};

	// Need a monochrome pattern bitmap
	CBitmap bitmap;
	bitmap.CreateBitmap( 8, 8, 1, 1, &bits );

	// Create the pattern brush
	CBrush brush;
	brush.CreatePatternBrush( &bitmap );
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	COLORREF crBack	= pDC->SetBkColor(xtAfxData.clr3DFace);
	COLORREF crText	= pDC->SetTextColor(xtAfxData.clr3DHilight);

	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);

	// Restore the device context
	pDC->SelectObject(pOldBrush);
	pDC->SetBkColor(crBack);
	pDC->SetTextColor(crText);

	brush.DeleteObject();
}


typedef HRESULT (CALLBACK* XT_AFX_DLLGETVERSIONPROC)(XT_DLLVERSIONINFO*);

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: GetComCtlVersion()
//
DWORD XT_AUX_DATA::GetComCtlVersion()
{
	// return cached version if already determined...
	if (iComCtlVersion != -1)
		return iComCtlVersion;

	// otherwise determine comctl32.dll version via DllGetVersion
	HINSTANCE hInst = ::LoadLibraryA("COMCTL32.DLL");
	ASSERT(hInst != NULL);

	XT_AFX_DLLGETVERSIONPROC pfn = (XT_AFX_DLLGETVERSIONPROC)
		::GetProcAddress(hInst, "DllGetVersion");

	if (pfn != NULL)
	{
		XT_DLLVERSIONINFO dvi;
		memset(&dvi, 0, sizeof(dvi));
		dvi.cbSize = sizeof(dvi);

		if (SUCCEEDED((*pfn)(&dvi)))
		{
			ASSERT(dvi.dwMajorVersion <= 0xFFFF);
			ASSERT(dvi.dwMinorVersion <= 0xFFFF);
			DWORD dwVersion = MAKELONG(dvi.dwMinorVersion, dvi.dwMajorVersion);

			iComCtlVersion = dwVersion;
		}
	}

	FreeLibrary(hInst);

	return iComCtlVersion != -1? iComCtlVersion : 0;
}

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: GetXTVersion()
//
CString XT_AUX_DATA::GetXTVersion(bool bVerNumOnly/*=false*/)
{
	CString strVersion;
	if ( bVerNumOnly )
	{
		strVersion.Format( _T( "%d.%02d" ),
			_XTLIB_VERSION_MAJOR, _XTLIB_VERSION_MINOR );
	}
	else
	{
		strVersion.Format( _T( "Xtreme Toolkit v%d.%02d" ),
			_XTLIB_VERSION_MAJOR, _XTLIB_VERSION_MINOR );
	}
	return strVersion;
}

/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxChangeWindowFont()
/////////////////////////////////////////////////////////////////////////////

// helper function which sets the font for a window and all its children
// and also resizes everything according to the new font

_XT_EXT_CLASS void  AFXAPI _xtAfxChangeWindowFont(CWnd* pWnd, CFont* pFont)
{
	CRect windowRect;
	
	// grab old and new text metrics
	TEXTMETRIC tmOld, tmNew;
	CDC * pDC = pWnd->GetDC();
	CFont * pSavedFont = pDC->SelectObject(pWnd->GetFont());
	pDC->GetTextMetrics(&tmOld);
	pDC->SelectObject(pFont);
	pDC->GetTextMetrics(&tmNew);
	pDC->SelectObject(pSavedFont);
	pWnd->ReleaseDC(pDC);
	
	long oldHeight = tmOld.tmHeight+tmOld.tmExternalLeading;
	long newHeight = tmNew.tmHeight+tmNew.tmExternalLeading;
	
	// calculate new dialog window rectangle
	CRect clientRect, newClientRect, newWindowRect;
	
	pWnd->GetWindowRect(windowRect);
	pWnd->GetClientRect(clientRect);
	long xDiff = windowRect.Width() - clientRect.Width();
	long yDiff = windowRect.Height() - clientRect.Height();
	
	newClientRect.left = newClientRect.top = 0;
	newClientRect.right = clientRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
	newClientRect.bottom = clientRect.bottom * newHeight / oldHeight;
	
	newWindowRect.left = windowRect.left - 
		(newClientRect.right - clientRect.right)/2;
	newWindowRect.top = windowRect.top -
		(newClientRect.bottom - clientRect.bottom)/2;
	newWindowRect.right = newWindowRect.left + newClientRect.right + xDiff;
	newWindowRect.bottom = newWindowRect.top + newClientRect.bottom + yDiff;
	
	pWnd->MoveWindow(newWindowRect);
	
	pWnd->SetFont(pFont);
	
	// iterate through and move all child windows and change their font.
	CWnd* pChildWnd = pWnd->GetWindow(GW_CHILD);
	
	while (pChildWnd)
	{
		pChildWnd->SetFont(pFont);
		pChildWnd->GetWindowRect(windowRect);
		
		CString strClass;
		::GetClassName(pChildWnd->m_hWnd, strClass.GetBufferSetLength(32), 31);
		strClass.MakeUpper();
		if(strClass==_T("COMBOBOX"))
		{
			CRect rect;
			pChildWnd->SendMessage(CB_GETDROPPEDCONTROLRECT,0,(LPARAM) &rect);
			windowRect.right = rect.right;
			windowRect.bottom = rect.bottom;
		}
		
		pWnd->ScreenToClient(windowRect);
		windowRect.left = windowRect.left * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		windowRect.right = windowRect.right * tmNew.tmAveCharWidth / tmOld.tmAveCharWidth;
		windowRect.top = windowRect.top * newHeight / oldHeight;
		windowRect.bottom = windowRect.bottom * newHeight / oldHeight;
		pChildWnd->MoveWindow(windowRect);
		
		pChildWnd = pChildWnd->GetWindow(GW_HWNDNEXT);
	}
}
/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxRelease()
/////////////////////////////////////////////////////////////////////////////
_XT_EXT_CLASS DWORD AFXAPI _xtAfxRelease(LPUNKNOWN* lplpUnknown)
{
	ASSERT(lplpUnknown != NULL);
	if (*lplpUnknown != NULL)
	{
		DWORD dwRef = (*lplpUnknown)->Release();
		*lplpUnknown = NULL;
		return dwRef;
	}
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxMakeOleVariant()
/////////////////////////////////////////////////////////////////////////////
_XT_EXT_CLASS void AFXAPI _xtAfxMakeOleVariant(COleVariant &ov, LPCITEMIDLIST pidl)
{
	AfxVariantInit(&ov);

	if (pidl != NULL)
	{
		// walk through entries in the list and accumulate their size

		UINT cbTotal = 0;
		SAFEARRAY *psa = NULL;
		LPCITEMIDLIST pidlWalker = pidl;

		while (pidlWalker->mkid.cb)
		{
			cbTotal += pidlWalker->mkid.cb;
			pidlWalker = (LPCITEMIDLIST)
				(((LPBYTE)pidlWalker) + pidlWalker->mkid.cb);
		}

		// add the base structure size
		cbTotal += sizeof(ITEMIDLIST);

		// get a safe array for them
		psa = SafeArrayCreateVector(VT_UI1, 0, cbTotal);

		// copy it and set members
		if (psa != NULL)
		{
			memcpy(psa->pvData, (LPBYTE) pidl, cbTotal);
			ov.vt = VT_ARRAY | VT_UI1;
			ov.parray = psa;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// FUNCTION: _xtAfxChildWindowFromPoint()
/////////////////////////////////////////////////////////////////////////////
_XT_EXT_CLASS HWND AFXAPI _xtAfxChildWindowFromPoint(HWND hWnd, POINT pt)
{
  ASSERT(hWnd != NULL);

  // check child windows
  ::ClientToScreen(hWnd, &pt);
  HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
  for (; hWndChild != NULL; hWndChild = ::GetWindow(hWndChild, GW_HWNDNEXT))
  {
	if ((UINT)(WORD)::GetDlgCtrlID(hWndChild) != (WORD)-1 &&
	  (::GetWindowLong(hWndChild, GWL_STYLE) & WS_VISIBLE))
	{
	  // see if point hits the child window
	  CRect rect;
	  ::GetWindowRect(hWndChild, rect);
	  if (rect.PtInRect(pt))
		return hWndChild;
	}
  }

  return NULL;    // not found
}

/////////////////////////////////////////////////////////////////////////////
// _xtAfxTreeViewCompareProc(LPARAM, LPARAM, LPARAM)
// Callback routine for sorting tree 

_XT_EXT_CLASS int CALLBACK _xtAfxTreeViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM /*lparamSort*/)
{
	XT_TVITEMDATA* lptvid1 = (XT_TVITEMDATA*)lparam1;
	XT_TVITEMDATA* lptvid2 = (XT_TVITEMDATA*)lparam2;
	
	HRESULT hr = lptvid1->lpsfParent->CompareIDs(0, lptvid1->lpi, lptvid2->lpi);
	if (FAILED(hr)) {
		return 0;
	}
	
	return (short)SCODE_CODE(GetScode(hr));
}

/////////////////////////////////////////////////////////////////////////////
// _xtAfxListViewCompareProc(LPARAM, LPARAM, LPARAM)
// Callback routine for sorting list 

_XT_EXT_CLASS int CALLBACK _xtAfxListViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM /*lparamSort*/)
{
	XT_LVITEMDATA* lplvid1 = (XT_LVITEMDATA*)lparam1;
	XT_LVITEMDATA* lplvid2 = (XT_LVITEMDATA*)lparam2;

	HRESULT hr = lplvid1->lpsfParent->CompareIDs(0,lplvid1->lpi,lplvid2->lpi);
	if (FAILED(hr)) {
		return 0;
	}

	return (short)SCODE_CODE(GetScode(hr));
}

#if _MSC_VER < 1200 // MFC 5.0

CXTString::CXTString(CString strIn) : CString(strIn) { }

int CXTString::Find(LPCTSTR lpszSub, int nStart) const
{
	ASSERT(AfxIsValidString(lpszSub));
	
	int nLength = GetData()->nDataLength;
	if (nStart > nLength)
		return -1;
	
	// find first matching substring
	LPTSTR lpsz = _tcsstr(m_pchData + nStart, lpszSub);
	
	// return -1 for not found, distance from beginning otherwise
	return (lpsz == NULL) ? -1 : (int)(lpsz - m_pchData);
}

int CXTString::Insert(int nIndex, TCHAR ch)
{
	CopyBeforeWrite();

	if (nIndex < 0)
		nIndex = 0;

	int nNewLength = GetData()->nDataLength;
	if (nIndex > nNewLength)
		nIndex = nNewLength;
	nNewLength++;

	if (GetData()->nAllocLength < nNewLength)
	{
		CStringData* pOldData = GetData();
		LPTSTR pstr = m_pchData;
		AllocBuffer(nNewLength);
		memcpy(m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(TCHAR));
		CString::Release(pOldData);
	}

	// move existing bytes down
	memmove(m_pchData + nIndex + 1,
		m_pchData + nIndex, (nNewLength-nIndex)*sizeof(TCHAR));
	m_pchData[nIndex] = ch;
	GetData()->nDataLength = nNewLength;

	return nNewLength;
}

int CXTString::Insert(int nIndex, LPCTSTR pstr)
{
	if (nIndex < 0)
		nIndex = 0;

	int nInsertLength = SafeStrlen(pstr);
	int nNewLength = GetData()->nDataLength;
	if (nInsertLength > 0)
	{
		CopyBeforeWrite();
		if (nIndex > nNewLength)
			nIndex = nNewLength;
		nNewLength += nInsertLength;

		if (GetData()->nAllocLength < nNewLength)
		{
			CStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, (pOldData->nDataLength+1)*sizeof(TCHAR));
			CString::Release(pOldData);
		}

		// move existing bytes down
		memmove(m_pchData + nIndex + nInsertLength,
			m_pchData + nIndex,
			(nNewLength-nIndex-nInsertLength+1)*sizeof(TCHAR));
		memmove(m_pchData + nIndex,
			pstr, nInsertLength*sizeof(TCHAR));
		GetData()->nDataLength = nNewLength;
	}

	return nNewLength;
}

int CXTString::Remove(TCHAR chRemove)
{
	CopyBeforeWrite();
	
	LPTSTR pstrSource = m_pchData;
	LPTSTR pstrDest = m_pchData;
	LPTSTR pstrEnd = m_pchData + GetData()->nDataLength;
	
	while (pstrSource < pstrEnd)
	{
		if (*pstrSource != chRemove)
		{
			*pstrDest = *pstrSource;
			pstrDest = _tcsinc(pstrDest);
		}
		pstrSource = _tcsinc(pstrSource);
	}
	*pstrDest = '\0';
	int nCount = pstrSource - pstrDest;
	GetData()->nDataLength -= nCount;
	
	return nCount;
}

int CXTString::Replace(TCHAR chOld, TCHAR chNew)
{
	int nCount = 0;

	// short-circuit the nop case
	if (chOld != chNew)
	{
		// otherwise modify each character that matches in the string
		CopyBeforeWrite();
		LPTSTR psz = m_pchData;
		LPTSTR pszEnd = psz + GetData()->nDataLength;
		while (psz < pszEnd)
		{
			// replace instances of the specified character only
			if (*psz == chOld)
			{
				*psz = chNew;
				nCount++;
			}
			psz = _tcsinc(psz);
		}
	}
	return nCount;
}

int CXTString::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	// can't have empty or NULL lpszOld
	
	int nSourceLen = SafeStrlen(lpszOld);
	if (nSourceLen == 0)
		return 0;
	int nReplacementLen = SafeStrlen(lpszNew);
	
	// loop once to figure out the size of the result string
	int nCount = 0;
	LPTSTR lpszStart = m_pchData;
	LPTSTR lpszEnd = m_pchData + GetData()->nDataLength;
	LPTSTR lpszTarget;
	while (lpszStart < lpszEnd)
	{
		while ((lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
		{
			nCount++;
			lpszStart = lpszTarget + nSourceLen;
		}
		lpszStart += lstrlen(lpszStart) + 1;
	}
	
	// if any changes were made, make them
	if (nCount > 0)
	{
		CopyBeforeWrite();
		
		// if the buffer is too small, just
		//   allocate a new buffer (slow but sure)
		int nOldLength = GetData()->nDataLength;
		int nNewLength =  nOldLength + (nReplacementLen-nSourceLen)*nCount;
		if (GetData()->nAllocLength < nNewLength || GetData()->nRefs > 1)
		{
			CStringData* pOldData = GetData();
			LPTSTR pstr = m_pchData;
			AllocBuffer(nNewLength);
			memcpy(m_pchData, pstr, pOldData->nDataLength*sizeof(TCHAR));
			CString::Release(pOldData);
		}
		// else, we just do it in-place
		lpszStart = m_pchData;
		lpszEnd = m_pchData + GetData()->nDataLength;
		
		// loop again to actually do the work
		while (lpszStart < lpszEnd)
		{
			while ( (lpszTarget = _tcsstr(lpszStart, lpszOld)) != NULL)
			{
				int nBalance = nOldLength - (lpszTarget - m_pchData + nSourceLen);
				memmove(lpszTarget + nReplacementLen, lpszTarget + nSourceLen,
					nBalance * sizeof(TCHAR));
				memcpy(lpszTarget, lpszNew, nReplacementLen*sizeof(TCHAR));
				lpszStart = lpszTarget + nReplacementLen;
				lpszStart[nBalance] = '\0';
				nOldLength += (nReplacementLen - nSourceLen);
			}
			lpszStart += lstrlen(lpszStart) + 1;
		}
		ASSERT(m_pchData[nNewLength] == '\0');
		GetData()->nDataLength = nNewLength;
	}
	
	return nCount;
}

int CXTString::Delete(int nIndex, int nCount)
{
	if (nIndex < 0)
		nIndex = 0;
	int nNewLength = GetData()->nDataLength;
	if (nCount > 0 && nIndex < nNewLength)
	{
		CopyBeforeWrite();
		int nBytesToCopy = nNewLength - (nIndex + nCount) + 1;
		
		memcpy(m_pchData + nIndex,
			m_pchData + nIndex + nCount, nBytesToCopy * sizeof(TCHAR));
		GetData()->nDataLength = nNewLength - nCount;
	}
	
	return nNewLength;
}

CXTString CXTString::Left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	if (nCount >= GetData()->nDataLength)
		return *this;
	
	CXTString dest;
	AllocCopy(dest, nCount, 0, 0);
	return dest;
}


#endif // MFC 5.0


_XT_EXT_CLASS void AFXAPI _xtAfxStripMnemonics(CString& strIn)
{
#if _MSC_VER < 1200 // MFC 5.0
	CXTString strText(strIn);
#else
	CString  strText(strIn);
#endif

	// Check to see if '&&' was used for format an ampersand.
	int nIndex = strText.Find(_T("&&"), 0);

	// If not then remove mnemonic formatting.
	if (nIndex == -1) {
		strText.Remove(_T('&'));
	}

	// Otherwise swap out the '&&' temporarily and remove
	// the mnemonic character...
	else
	{
		strText.Replace(_T("&&"), _T("^^"));
		strText.Remove(_T('&'));
		strText.Replace(_T("^^"), _T("&&"));
	}

	strIn = strText;
}

_XT_EXT_CLASS BOOL AFXAPI _xtAfxExist(LPCTSTR lpszFileName)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(lpszFileName, &fd);

	if (hFind != INVALID_HANDLE_VALUE) {
		::FindClose(hFind);
	}

	return (hFind != INVALID_HANDLE_VALUE);
}

_XT_EXT_CLASS LPTSTR AFXAPI _xtAfxPathFindNextComponent(LPCTSTR pszPath)
{
	// We will return static memory
	static TCHAR tNextComponent[MAX_PATH + 1];
	
	// Make sure we were given a valid path
	if ( pszPath == NULL || _tcslen( pszPath ) == 0 ) {
		return NULL;
	}

	// Find the path delimiter
	const TCHAR *cp = _tcschr( pszPath, _T('\\') );
	if ( cp == NULL ) {
		// If we didn't find it, return an empty string
		tNextComponent[0] = 0;
		return tNextComponent;
	}

	// OK, we have the delimiter. Copy the rest of
	// the path to our internal buffer and return it
	_tcscpy( tNextComponent, cp + 1 );
	return tNextComponent;
}

_XT_EXT_CLASS CWnd* AFXAPI _xtAfxCreateView(CWnd* pParentWnd, CRuntimeClass *pViewClass, CDocument *pDocument/*=NULL*/, CCreateContext *pContext/*=NULL*/, DWORD dwStyle/*=AFX_WS_DEFAULT_VIEW*/, CWnd* pOwnerWnd/*=NULL*/, UINT nID/*=AFX_IDW_PANE_FIRST*/)
{
#ifdef _DEBUG
	ASSERT(pContext != NULL || pViewClass != NULL);
	ASSERT(pContext != NULL || pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(pContext != NULL || AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	if (pOwnerWnd == NULL)
		pOwnerWnd = pParentWnd;

	CCreateContext contextT;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the 
		// currently selected client if possible.
		contextT.m_pLastView	   = NULL;
		contextT.m_pCurrentFrame   = NULL;
		contextT.m_pNewDocTemplate = NULL;
		contextT.m_pCurrentDoc	   = pDocument;
		contextT.m_pNewViewClass   = pViewClass;

		if (pDocument != NULL)
			contextT.m_pNewDocTemplate = pDocument->GetDocTemplate();

		pContext = &contextT;
	}

	CWnd* pWnd = NULL;
	TRY
	{
		pWnd = (CWnd*)pContext->m_pNewViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" ); // Note: DELETE_EXCEPTION(e) not required
		return NULL;
	}
	END_CATCH_ALL

	ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL); // not yet created.

	// Create with the right size (wrong position)
	CRect rect(0,0,0,0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, pParentWnd, nID, pContext))
	{
		TRACE0( "Failed to create docking window view.\n" );
		return NULL;
	}
	
	pWnd->SetOwner(pOwnerWnd);
	return pWnd;
}

#ifdef _DEBUG
_XT_EXT_CLASS void AFXAPI _xtAfxAssertValidBarIDs(CFrameWnd* pFrameWnd)
{
	CPtrList& listCBars = pFrameWnd->m_listControlBars;
	POSITION posID;
	for (posID = listCBars.GetHeadPosition(); posID!=NULL; listCBars.GetNext(posID))
	{
		CXTControlBar* pBar = (CXTControlBar*)pFrameWnd->m_listControlBars.GetAt(posID);
		ASSERT_VALID(pBar);
		
		if (!pBar->IsKindOf(RUNTIME_CLASS(CXTControlBar)))
			continue;
		
		// validate the control bar's ID.
		pBar->AssertValidCtrlID();
	}
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CXTIconHandle

CXTIconHandle::CXTIconHandle()
	: m_hIcon( NULL )
{
}

CXTIconHandle::CXTIconHandle(HICON hIcon)
	: m_hIcon( hIcon )
{
}

CXTIconHandle::~CXTIconHandle()
{
	if ( m_hIcon != NULL )
	{
		::DestroyIcon( m_hIcon );
	}
}

CXTIconHandle::operator HICON() const
{
	return m_hIcon;
}

CXTIconHandle& CXTIconHandle::operator = (HICON hIcon)
{
	if ( m_hIcon != NULL )
	{
		::DestroyIcon( m_hIcon );
	}

	m_hIcon = hIcon;

	return *this;
}

CSize CXTIconHandle::GetExtent(HICON hIcon)
{
	CSize extent(0);
	if (hIcon)
	{
		ICONINFO iconinfo;
		if (::GetIconInfo(hIcon,&iconinfo))
		{
			BITMAP bmpinfo;
			if (::GetObject(iconinfo.hbmMask, sizeof(bmpinfo), &bmpinfo))
			{
				extent.cx = (int)bmpinfo.bmWidth;
				extent.cy = (int)bmpinfo.bmHeight;
				if (!iconinfo.hbmColor)
				{
					// b/w icons have double size for XOR and AND masks
					extent.cy /= 2;
				}
			}
			// cleanup GDI
			if (iconinfo.hbmMask)
			{
				::DeleteObject(iconinfo.hbmMask);
			}
			if (iconinfo.hbmColor)
			{
				::DeleteObject(iconinfo.hbmColor);
			}
		}		
	}
	return extent;
}

CSize CXTIconHandle::GetExtent() const
{
	return GetExtent(m_hIcon);
}

HICON CXTIconHandle::ScaleToFit(HICON hIcon, CSize desiredExtent)
{
	if (desiredExtent == CSize(0))
	{
		// invalid arg
		return NULL;
	}

	CSize realExtent = GetExtent(hIcon);
	if (realExtent == CSize(0))
	{
		// icon destroyed or not created yet
		return NULL;
	}


	// ensure icon retains aspect after scaling
	int delta = desiredExtent.cx * realExtent.cy - desiredExtent.cy * realExtent.cx;
	if (delta < 0)
	{
		desiredExtent.cy = MulDiv(realExtent.cy, desiredExtent.cx, realExtent.cx);
	}
	else
	if (delta > 0)
	{
		desiredExtent.cx = MulDiv(realExtent.cx, desiredExtent.cy, realExtent.cy);
	}
	
	// scale the icon
	CImageList images;
	VERIFY(images.Create(desiredExtent.cx, desiredExtent.cy, ILC_COLOR32 | ILC_MASK, 1, 1 ) );
	images.Add(hIcon);
	return images.ExtractIcon(0);
}

HICON CXTIconHandle::ScaleToFit(CSize desiredExtent) const
{
	return ScaleToFit(m_hIcon, desiredExtent);
}

/////////////////////////////////////////////////////////////////////////////
// CXTLoadLibrary

CXTLoadLibrary::CXTLoadLibrary(LPCTSTR lpszModule/*=NULL*/)
{
	m_hInstance = NULL;

	if (lpszModule != NULL) {
		m_hInstance = ::LoadLibrary(lpszModule); 
	}
}

CXTLoadLibrary::~CXTLoadLibrary()
{
	if (m_hInstance != NULL) {
		::FreeLibrary(m_hInstance);  
	}
}

/////////////////////////////////////////////////////////////////////////////

void CXTLoadLibrary::LoadLibrary(LPCTSTR lpszModule)
{
	if (m_hInstance != NULL) {
		::FreeLibrary(m_hInstance);  
		m_hInstance = NULL;
	}
	if (lpszModule != NULL) {
		m_hInstance = ::LoadLibrary(lpszModule); 
	}
}

DWORD CXTLoadLibrary::GetModuleVersion()
{
	typedef HRESULT ( CALLBACK* DLLGETVERSIONPROC )( DLLVERSIONINFO* );

	DLLGETVERSIONPROC pDllGetVersion =
		( DLLGETVERSIONPROC )::GetProcAddress( m_hInstance, "DllGetVersion" );

	DWORD dwVersion = 0L;
	if ( pDllGetVersion != NULL )
	{
		DLLVERSIONINFO dvi;
		::ZeroMemory( &dvi, sizeof( dvi ) );
		dvi.cbSize = sizeof( dvi );
		
		HRESULT hr = ( *pDllGetVersion )( &dvi );
		if ( SUCCEEDED( hr ) )
		{
			ASSERT( dvi.dwMajorVersion <= 0xFFFF );
			ASSERT( dvi.dwMinorVersion <= 0xFFFF );
			dwVersion = MAKELONG( dvi.dwMinorVersion, dvi.dwMajorVersion );
		}
	}
	
	return dwVersion;
}

/////////////////////////////////////////////////////////////////////////////
// CXTManageState

HMODULE CXTManageState::m_hModXTInstance = NULL;
HMODULE CXTManageState::m_hModXTResource = NULL;

CXTManageState::CXTManageState()
{
	Redo();
}

CXTManageState::~CXTManageState()
{
	Undo();
	m_hModOldResource = NULL;
	m_hModOldInstance = NULL;
}

void CXTManageState::SetModuleInfo(HMODULE hModRes, HMODULE hModInst)
{
	m_hModXTInstance = hModInst;
	m_hModXTResource = hModRes;
}

void CXTManageState::Undo()
{
	if (!m_bSet) return;
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	pModuleState->m_hCurrentResourceHandle = m_hModOldResource;
	pModuleState->m_hCurrentInstanceHandle = m_hModOldInstance;
	m_bSet = false;
}

void CXTManageState::Redo()
{
	ASSERT(m_hModXTResource != NULL);
	ASSERT(m_hModXTInstance != NULL);
	AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
	m_hModOldResource = pModuleState->m_hCurrentResourceHandle;
	m_hModOldInstance = pModuleState->m_hCurrentInstanceHandle;
	pModuleState->m_hCurrentResourceHandle = m_hModXTResource;
	if (pModuleState->m_hCurrentInstanceHandle == NULL)
	{
		pModuleState->m_hCurrentInstanceHandle = m_hModXTInstance;
	}
	m_bSet = true;
}


/////////////////////////////////////////////////////////////////////////////
// CXTDialogState

CXTDialogState::CXTDialogState()
{
#if defined(_XT_STATICLINK) || defined(_XT_DYN_STATICLIB) || defined(_XT_NO_MANAGED_STATE)
    // not needed for static link
    m_pState = NULL;
#else
    m_pState = new CXTManageState();
#endif
}

CXTDialogState::~CXTDialogState()
{
    XT_ASSERT_MSG(m_pState == NULL, 
        "The dialog resource context should be reset using RevertResourceContext() inside the Toolkit dialog constructor.\n"
        "Use XT_MANAGE_STATE (as needed) after that to switch back to the Toolkit resource context.");
    RevertResourceContext();
}

void CXTDialogState::RevertResourceContext()
{
    SAFE_DELETE(m_pState);
}


/////////////////////////////////////////////////////////////////////////////
// XT_MENUITEMINFO

XT_MENUITEMINFO::XT_MENUITEMINFO()
{
	::ZeroMemory(this, sizeof(MENUITEMINFO));
	cbSize = sizeof(MENUITEMINFO);
	// WINBUG: NT4 doesn't work with newer structures with size larger than
	// 44 bytes
	if (cbSize > 44  &&  xtVerInfo.IsWinNT4())
	{
		cbSize = 44;
	}
}

/////////////////////////////////////////////////////////////////////////////
// XT_DROPDOWNBUTTON
XT_DROPDOWNBUTTON::XT_DROPDOWNBUTTON()
: idButton(0)
, idMenu(0)
, bShowColor(FALSE)
, bColorPicker(FALSE)
, dwPopup(0L)
, clrDefault(CLR_DEFAULT)
, clrColor(CLR_DEFAULT)
, bArrow(FALSE)
, bMenuBarPopup(false)
, dwMenuBarPopupID(0)
{

}

XT_DROPDOWNBUTTON::XT_DROPDOWNBUTTON(const XT_DROPDOWNBUTTON* pOther)
: idButton(pOther->idButton)
, idMenu(pOther->idMenu)
, bShowColor(pOther->bShowColor)
, bColorPicker(pOther->bColorPicker)
, dwPopup(pOther->dwPopup)
, clrDefault(pOther->clrDefault)
, clrColor(pOther->clrColor)
, bArrow(pOther->bArrow)
, bMenuBarPopup(pOther->bMenuBarPopup)
, dwMenuBarPopupID(pOther->dwMenuBarPopupID)
{
}

