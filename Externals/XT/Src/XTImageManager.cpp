#include "StdAfx.h"
#include "XTDrawHelpers.h"

#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTDelegate.h"
#include "XTWndHook.h"
#include "XTControlbar.h"
#include "XTToolBar.h"
#include "XTFunctions.h"
#include "XTImageManager.h"

#define TRANSPARENT_COLOR RGB(0xC0, 0xC0, 0xC0)


CXTImageManager CXTImageManager::s_managerInstance;

CXTImageManager::CXTImageManager(void)
{
}

CXTImageManager::~CXTImageManager(void)
{
	RemoveAll();
}

COLORREF CXTImage::LightenColor(COLORREF clr, double factor)
{
	return RGB(
		factor * 255 + (1.0 - factor) * GetRValue(clr),
		factor * 255 + (1.0 - factor) * GetGValue(clr),
		factor * 255 + (1.0 - factor) * GetBValue(clr)) ;
}


void CXTImage::CreateFadedIcon()
{
	ICONINFO info;
	HBITMAP hbmColor = 0;
	
	if (GetIconInfo(m_hIcon, &info))
	{
		hbmColor = info.hbmColor;
		CXTCompatibleDC dc(NULL, CBitmap::FromHandle(hbmColor));
		CXTCompatibleDC dcMask(NULL, CBitmap::FromHandle(info.hbmMask));

		BITMAP bmp;
		::GetObject(info.hbmColor, sizeof (BITMAP), &bmp);

		int i, j;
		for(i = 0; i < bmp.bmWidth; i++)
		for(j = 0; j < bmp.bmHeight; j++)
		{
			COLORREF clr = dc.GetPixel(i, j);		
			COLORREF clrMask = dcMask.GetPixel(i, j);		
			if (clrMask == 0)
				dc.SetPixel(i, j, LightenColor(clr, .3));				
		}		
	}
	if (hbmColor)
	{
		::DeleteObject(info.hbmMask);

		ICONINFO infoMask;
		GetIconInfo(m_hIcon, &infoMask);
		::DeleteObject(infoMask.hbmColor);
		infoMask.hbmColor = hbmColor;

		m_hFaded = CreateIconIndirect(&infoMask);
		
		::DeleteObject(infoMask.hbmColor);
		::DeleteObject(infoMask.hbmMask);
	}
}

void CXTImage::CreateShadowIcon()
{
	ICONINFO info;
	HBITMAP hbmColor = 0;
	
	if (GetIconInfo(m_hIcon, &info))
	{
		hbmColor = info.hbmColor;
		CXTCompatibleDC dc(NULL, CBitmap::FromHandle(hbmColor));
		CXTCompatibleDC dcMask(NULL, CBitmap::FromHandle(info.hbmMask));

		BITMAP bmp;
		::GetObject(info.hbmColor, sizeof (BITMAP), &bmp);

		int i, j;
		for(i = 0; i < bmp.bmWidth; i++)
		for(j = 0; j < bmp.bmHeight; j++)
		{
			COLORREF clrMask = dcMask.GetPixel(i, j);		
			if (clrMask == 0)
				dc.SetPixel(i, j, xtAfxData.clrXPIconShadow);		
		}		
	}
	if (hbmColor)
	{
		::DeleteObject(info.hbmMask);

		ICONINFO infoMask;
		GetIconInfo(m_hIcon, &infoMask);
		::DeleteObject(infoMask.hbmColor);
		infoMask.hbmColor = hbmColor;

		m_hShadow = CreateIconIndirect(&infoMask);
		
		::DeleteObject(infoMask.hbmColor);
		::DeleteObject(infoMask.hbmMask);
	}
}



void CXTImage::InitBimaps(CImageList& imageList, int nIndex)
{
	m_hIcon = imageList.ExtractIcon(nIndex);	
	CreateFadedIcon();	
	CreateShadowIcon();
}

CXTImage::CXTImage()
{
	m_hIcon = 0;
	m_hFaded = 0;
	m_hShadow = 0;
	m_hDisabled = 0;
}
#define SAFE_DESTROYICON(p) if (p) { DestroyIcon(p); p = 0;}

void CXTImage::Clear(BOOL bIcon /* = FALSE */)
{
	if (bIcon && m_hIcon)
	{
		DestroyIcon(m_hIcon);
		m_hIcon = 0;
	}
	SAFE_DESTROYICON(m_hFaded);
	SAFE_DESTROYICON(m_hShadow);
	SAFE_DESTROYICON(m_hDisabled);
}

CXTImage::~CXTImage()
{
	Clear(TRUE);
}

void CXTImageManager::InitBitmaps(UINT nCommand, CImageList& imageList, int nIndex, BOOL bEnabled, BOOL bToolBar)
{
	if (bEnabled)
	{		
		if (Lookup(nCommand, bToolBar))
		{
			delete m_mapImages[bToolBar? TOOLBAR: COOLMENU][nCommand];
		}

		CXTImage* pImage = new CXTImage;
		m_mapImages[bToolBar? TOOLBAR: COOLMENU].SetAt(nCommand, pImage);
		pImage->InitBimaps(imageList, nIndex);
	} 
	else if (Lookup(nCommand, bToolBar))
	{
		CXTImage* pImage = 0;
		m_mapImages[bToolBar? TOOLBAR: COOLMENU].Lookup(nCommand, pImage);
		ASSERT(pImage);
		
		if (pImage->m_hDisabled == 0)
		{
			pImage->m_hDisabled = imageList.ExtractIcon(nIndex);	
		}
	}
}

BOOL CXTImageManager::LoadToolBarIcons(LPCTSTR lpszResourceName, BOOL bEnabled, BOOL bToolBar)
{
	// determine location of the bitmap in resource fork
	HINSTANCE hInst = AfxFindResourceHandle(lpszResourceName, RT_TOOLBAR);
	HRSRC hRsrc = ::FindResource(hInst, lpszResourceName, RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	XT_TOOLBARDATA* pData = (XT_TOOLBARDATA*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;
	ASSERT(pData->wVersion == 1);

	UnlockResource(hGlobal);
	FreeResource(hGlobal);


	//

	CImageList ilTemp;
	if ( !ilTemp.Create( pData->wWidth, pData->wHeight, ILC_COLOR32 | ILC_MASK, 0, 10 ) )
	{
		return FALSE;
	}

	CBitmap bmp;
	bmp.LoadBitmap(lpszResourceName);
	ilTemp.Add( &bmp, TRANSPARENT_COLOR );
	
	int i, j = 0;
	for (i = 0; i < pData->wItemCount; i++)
	{
		if (pData->GetItem(i) != 0)
		{
			InitBitmaps(pData->GetItem(i), ilTemp, j, bEnabled, bToolBar);
			j++;
		}
	}
	return TRUE;
}

BOOL CXTImageManager::LoadToolBarImageList(CXTToolBar* pBar, HIMAGELIST hList, BOOL bEnabled)
{
	if (!hList)
		return FALSE;

	CImageList* pList = CImageList::FromHandle(hList);
	ASSERT(pList);

	int nButtons = pBar->GetButtonCount();

	int i;
	for (i = 0; i < nButtons; i++)
	{
		TBBUTTON button;
		pBar->GetButton(i, &button);

		if (button.iBitmap > -1 && button.idCommand > 0)
		{
			InitBitmaps(button.idCommand, *pList, button.iBitmap, bEnabled, TRUE);
		}
	}

	return TRUE;
}


BOOL CXTImageManager::AddIcon(UINT nIDCmd, HICON hIcon, CSize sz, BOOL bEnabled, BOOL bToolBar)
{
	CImageList ilTemp;
	if ( !ilTemp.Create( sz.cx, sz.cy , ILC_COLOR32 | ILC_MASK, 0, 1 ) )
	{
		return FALSE;
	}
	VERIFY(ilTemp.Add( hIcon ) == 0);
	InitBitmaps(nIDCmd, ilTemp, 0, bEnabled, bToolBar);
	return TRUE;
}

BOOL CXTImageManager::Lookup(UINT nCommand, BOOL bToolBar)
{
	CXTImage* Object;
	return (m_mapImages[bToolBar? TOOLBAR: COOLMENU].Lookup(nCommand, Object));
}

void CXTImageManager::RemoveAll(void)
{
	m_mapImages[0].FreeAll();
	m_mapImages[1].FreeAll();
}

CXTImage* CXTImageManager::GetImage(UINT nCommand, BOOL bToolBar) 
{
	ASSERT(nCommand > 0);
	CXTImage* pImage;
	// if needs cool menu icon the first search in CoolMenu map then in ToolBar map
	if (!bToolBar)
	{
		if (m_mapImages[COOLMENU].Lookup(nCommand, pImage))
			return pImage;
		if (m_mapImages[TOOLBAR].Lookup(nCommand, pImage))
			return pImage;		
	} 
	else
	{
		if (m_mapImages[TOOLBAR].Lookup(nCommand, pImage))
			return pImage;
		if (m_mapImages[COOLMENU].Lookup(nCommand, pImage))
			return pImage;	
	}	

	return NULL;
}
