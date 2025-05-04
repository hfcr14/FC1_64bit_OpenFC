// XTImageManager.h : header file
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

#if !defined(__XTIMAGEMANAGER_H__)
#define __XTIMAGEMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// forwards

class CXTToolBar;
 
// CXTImage is an internal stand alone class. 
class _XT_EXT_CLASS CXTImage
{
	friend class CXTImageManager;
	friend class CXTImageMap;

private:
	CXTImage();
	~CXTImage();

public:
	HICON GetIcon() { return m_hIcon; }
	HICON GetFadedIcon() { return xtAfxData.bDisableFadedIcons? m_hIcon: m_hFaded; }
	HICON GetShadowIcon() { return m_hShadow; }
	HICON GetDisabledIcon() { return m_hDisabled;	}


private:
	HICON m_hIcon;
	HICON m_hFaded;
	HICON m_hShadow;
	HICON m_hDisabled;

private:
	void InitBimaps(CImageList& imageList, int nIndex);
	
	void CreateFadedIcon();	
	void CreateShadowIcon();

	void Clear(BOOL bIcon = FALSE);
	COLORREF LightenColor(COLORREF clr, double factor);
};

class CXTImageMap: public CMap<UINT, UINT, CXTImage*, CXTImage*>
{

public:
	void FreeAll(void)
	{
		POSITION pos = GetStartPosition();
		UINT rKey; CXTImage* rValue;
		while(pos)
		{
			GetNextAssoc(pos, rKey, rValue);
			delete rValue;
		}
		RemoveAll();
	}
};

// CXTImageManager is an internal stand alone class. 
class _XT_EXT_CLASS CXTImageManager
{
	friend CXTImageManager* XTImageManager();
	enum ICONOWNER {TOOLBAR = 0, COOLMENU = 1};
public:
	

private:
	CXTImageManager(void);

public:
	~CXTImageManager(void);

	BOOL LoadToolBarIcons(UINT nIDResource, BOOL bEnabled = TRUE, BOOL bToolBar = FALSE) { return LoadToolBarIcons(MAKEINTRESOURCE(nIDResource), bEnabled, bToolBar); }
	BOOL LoadToolBarIcons(LPCTSTR lpszResourceName, BOOL bEnabled = TRUE, BOOL bToolBar = FALSE);

	BOOL LoadToolBarImageList(CXTToolBar* pBar, HIMAGELIST hList, BOOL bEnabled = TRUE);

	BOOL AddIcon(UINT nIDCmd, HICON hIcon, CSize sz, BOOL bEnabled, BOOL bToolBar = FALSE);

	BOOL Lookup(UINT nCommand, BOOL bToolBar);
	void RemoveAll(void);

	CXTImage* GetImage(UINT nCommand, BOOL bToolBar = FALSE);

private:
	CXTImageMap m_mapImages[2];
	static CXTImageManager s_managerInstance;

private:
	void InitBitmaps(UINT nCommand, CImageList& imageList, int nIndex, BOOL bEnabled, BOOL bToolBar);

	friend class CXTToolBar;

};

inline CXTImageManager* XTImageManager() 
	{ return &CXTImageManager::s_managerInstance; }

#endif