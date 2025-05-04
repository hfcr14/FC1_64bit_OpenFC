// XTSkins.cpp 
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

#include "stdafx.h"
#include "XTDefines.h"
#include "XTGlobal.h"
#include "XTWndHook.h"
#include "XTCoolMenu.h"
#include "XTFlatComboBox.h"
#include "XTDelegate.h"
#include "XTControlbar.h"
#include "XTToolBarCtrl.h"
#include "XTToolBar.h"
#include "XTHelpers.h"
#include "XTWndShadow.h"
#include "XTFlatComboBox.h"
#include "XTSkins.h"
#include "XTCommandsListBox.h"
#include "XTMemDC.h"
#include "XTColorRef.h"

#include "XTImageManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable : 4355)
//C4355: 'this' : used in base member initializer list

// Draws an image of the arrow pointing down
//
static void DrawDownArrow(
            // Device context for drawing
            CDC* pDC,

            // a rectangle in which to draw
            const CRect& rcArrow
        )
{
    // 1000 0011  0 83
    // 1100 0111  1 C7
    // 1110 1111  2 EF
    static const WORD bits[3] = {0x83, 0xC7, 0xEF};
    CBitmap downArrow;
    downArrow.CreateBitmap(8, 3, 1, 1, bits);
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    memDC.SelectObject(&downArrow);
    pDC->BitBlt((rcArrow.left + rcArrow.right - 3) / 2,
                (rcArrow.top + rcArrow.bottom - 1) / 2,
                5, 3, &memDC, 1, 0, SRCCOPY);
}


///////////////////////////////////////////////////////////////////////
// Helper interface provided by a skin to proxies it implements
interface IXTProxySite
{
    // Returns TRUE if a toolbar is being tracked currently
    virtual bool IsTracking() = 0;
};

///////////////////////////////////////////////////////////////////////
// A proxy for drawing edit, XP style
class CXTFlatEditProxyXP : public IXTFlatEditProxy
{
    IXTProxySite* m_pSite;

public:
    CXTFlatEditProxyXP(IXTProxySite* pSite)
        : m_pSite(pSite)
    {}

    // Renders the combo.
    virtual void DrawEdit(CXTFlatEdit* pEdit, CDC* pDC, BOOL bHilight)
    {
		CRect rcClient;
		pEdit->GetClientRect( &rcClient );

		pDC->Draw3dRect( rcClient,
			pEdit->IsWindowEnabled( ) ? xtAfxData.clrWindow : xtAfxData.clr3DFace,
			pEdit->IsWindowEnabled( ) ? xtAfxData.clrWindow : xtAfxData.clr3DFace);
			
		rcClient.InflateRect( 1,1 );
		pDC->Draw3dRect( rcClient,
			bHilight ? xtAfxData.clrXPHighlightBorder : xtAfxData.clrWindow,
			bHilight ? xtAfxData.clrXPHighlightBorder : xtAfxData.clrWindow);
			
		rcClient.InflateRect( 1,1 );
		pDC->Draw3dRect( rcClient,
			xtAfxData.clr3DFace, xtAfxData.clr3DFace );
    }
};

///////////////////////////////////////////////////////////////////////
// A proxy for drawing combo, XP style
class CXTFlatComboProxyXP : public IXTFlatComboProxy
{
    IXTProxySite* m_pSite;

public:
    CXTFlatComboProxyXP(IXTProxySite* pSite)
        : m_pSite(pSite)
    {}

    // Renders the combo.
    virtual void DrawCombo(CXTFlatComboBox* pCombo, CDC* pDC, BOOL bHilight)
    {
        CRect rcItem;
        pCombo->GetClientRect(&rcItem);

        if (m_pSite->IsTracking())
        {
            bHilight = FALSE;
        }

        COLORREF clrBack, clrFore, clrFrame;        
        if (pCombo->GetDroppedState())
        {
            clrFore  = xtAfxData.clrXPBarTextPushed;
            clrBack  = xtAfxData.clrXPHighlightPushed;
            clrFrame = xtAfxData.clrXPHighlightBorder;
        }
        else
        {
            clrFore  = xtAfxData.clrXPMenuText;
            clrBack  = (bHilight) ? xtAfxData.clrXPHighlight       : xtAfxData.clr3DFace;
            clrFrame = (bHilight) ? xtAfxData.clrXPHighlightBorder : xtAfxData.clrWindow;
        }

        pDC->Draw3dRect(rcItem, xtAfxData.clr3DFace, xtAfxData.clr3DFace);
        rcItem.DeflateRect(1,1);
        
        CRect rectArrow = rcItem;
        rectArrow.left = rcItem.right-xtAfxData.cxHThumb - 2;
        pDC->Draw3dRect(rcItem, clrFrame, clrFrame);
        pDC->Draw3dRect(rectArrow, clrFrame, clrFrame);
        
        rectArrow.DeflateRect(1, 1);

        pDC->FillSolidRect(rectArrow, clrBack);

        CXTContextTextColorHandler handlerTextColor(pDC, clrFore);
        CXTContextBkColorHandler handlerBkColor(pDC, clrBack);
        DrawDownArrow(pDC, rectArrow);
    }
};

///////////////////////////////////////////////////////////////////////
// A proxy for drawing toolbar items, XP style
class CXTTbProxyXP : public IXTToolbarProxy
{
    IXTProxySite* m_pSite;

public:
    CXTTbProxyXP(IXTProxySite* pSite)
        : m_pSite(pSite)
    {}

    // constants used when drawing Xp style toolbars
    enum 
    {
        // top marging with TBSTYLE_LIST style
        TBLIST_TOP_MARGING = 3,

        // space between bottom of the image and top of the text with TBSTYLE_LIST style
        TBLIST_TEXT_PADDING = 5,

        // width of the drop-down marker(arrow) rectangle
        RECTARROW_WIDTH = 14,
    };


    // fills in a rectangle allocated to a toolbar control
    virtual void FillControlRect(CDC* pDC, CXTToolBar* pOwner, UINT nIndex)
    {
        CRect rc;
        TBBUTTON button;
        GetItemInfo(pOwner, nIndex, &rc, &button);	
        pDC->FillSolidRect(rc, pOwner->GetBackgroundColor());
    }

    // removes special effects around selected item
    virtual void RemoveDroppedEffects(CXTToolBar* pOwner, UINT nIndex)
    {
		if (pOwner && ::IsWindow(pOwner->m_hWnd))
		{
			CRect rc;
			TBBUTTON button;
			GetItemInfo(pOwner, nIndex, &rc, &button);
		
			// draw shadow
			rc.right += 4;
			rc.bottom  += 4;
			pOwner->InvalidateRect(&rc);
		}
    }


    // draws special effects around selected item
    virtual void DrawDroppedEffects(CDC* /*pDC*/, CXTToolBar* /*pOwner*/, UINT /*nIndex*/)
    {
    }

    // Draws a toolbar separator    
    virtual void DrawSeparator(CDC* pDC, CXTToolBar* pOwner, UINT nIndex)
    {
        TBBUTTON button;
        CRect rc;
        GetItemInfo(pOwner, nIndex, &rc, &button);
        if (button.fsState & TBSTATE_WRAP)
        {
            CRect rcClient;
            pOwner->GetClientRect(&rcClient);

            int x  = rc.left;
            int y  = rc.bottom-rc.Width();
            int cx = rcClient.Width(); //rc.Height()+x;
            int cy = rc.Width() +y;

            rc.SetRect(x,y,cx,cy);            
            rc.top  = (rc.top + rc.bottom)/2 - 1;

            pDC->FillSolidRect(rc, pOwner->GetBackgroundColor());
            CPen pen(PS_SOLID, 1, xtAfxData.clrXPSeparator);
            CXTContextObjectHandler<CPen> penHandler(pDC, &pen);            
            pDC->MoveTo(rc.left, rc.top);
            pDC->LineTo(rc.right, rc.top);

        }
        else
        {
            pDC->FillSolidRect(rc, pOwner->GetBackgroundColor());
            CPen pen(PS_SOLID, 1, xtAfxData.clrXPSeparator);
            CXTContextObjectHandler<CPen> penHandler(pDC, &pen);
            pDC->MoveTo(rc.left + 3, rc.top + 2);
            pDC->LineTo(rc.left + 3, rc.bottom - 2);
        }
    }

    static void GetItemInfo(CXTToolBar* pOwner, UINT nIndex, CRect* rc, TBBUTTON* button)
    {        
	    pOwner->SendMessage(TB_GETITEMRECT, nIndex, (LPARAM)rc);        
        pOwner->GetButton(nIndex, button);
    }

	struct TbXpDrawStateParam
	{
		DWORD dwFlags;
		LPCTSTR pszText;
	};

	static 
	BOOL CALLBACK TbXpDrawStateProc(
				HDC hdc,       // handle to device context
				LPARAM lData,  // image information
				WPARAM /*wData*/,  // more image information
				int cx,        // image width
				int cy         // image height
			)
	{
		TbXpDrawStateParam* param = (TbXpDrawStateParam*)lData;
		return ::DrawText(hdc, param->pszText, _tcslen(param->pszText), CRect(0,0, cx, cy),
			       param->dwFlags) != 0;
	}

	void RenderText(CDC* pDC, CXTToolBar* pOwner, CPoint ptText, CSize sizeText, LPCTSTR pszText, 
					DWORD dwDrawTextFlags, bool bDisabled)
	{
		TbXpDrawStateParam param;
		param.dwFlags = pOwner->GetDrawTextFlags(dwDrawTextFlags);
		param.pszText = pszText;

        if (bDisabled)
        {
			CBrush brush( xtAfxData.clrXPDisabled );
            pDC->DrawState(ptText, sizeText, (DRAWSTATEPROC)TbXpDrawStateProc,	(LPARAM)LPVOID(&param), DSS_MONO, (HBRUSH)brush);
        }
        else
        {
			pDC->DrawState(ptText, sizeText, (DRAWSTATEPROC)TbXpDrawStateProc,	(LPARAM)LPVOID(&param), DSS_NORMAL, (HBRUSH)0);
        }
	}

    // Draws a toolbar button
    virtual BOOL DrawButton(CDC* pDC, CXTToolBar* pOwner, UINT nIndex)
    {
        // prepare DC
        CXTContextObjectHandler<CFont> fontHandler(pDC, pOwner->GetFont());
        CXTContextBkModeHandler modeHandler(pDC, TRANSPARENT); 

        // query toolbar for toolbar button description
        TBBUTTON button;
	    CRect rc;
        GetItemInfo(pOwner, nIndex, &rc, &button);

        // figure out state of the button
        enum {pressed, hot, dropped, normal} buttonState;
        if (button.fsState & TBSTATE_PRESSED)
        {
            buttonState = pressed;
        }
        else
        if (pOwner->GetHotItem() == (int)nIndex &&
			!pOwner->IsDroppedItem()) // toolbar is not tracking its popup
        {
            buttonState = hot;
        }
        else
        {
            buttonState = normal;
        }
        // if cursor is over the item and mouse button pressed, then the item is pressed
        CPoint ptCursor;
        if (::GetCursorPos(&ptCursor) && 
			::WindowFromPoint(ptCursor) == pOwner->m_hWnd &&
			CWnd::GetCapture() == NULL)  
        {
            pOwner->ScreenToClient(&ptCursor);
            if (pOwner->GetToolBarCtrl().HitTest(&ptCursor) == (int)nIndex)
            {
                if (::GetKeyState(VK_LBUTTON) < 0)
                {
                    if (!m_pSite->IsTracking())
                    {                        
                        buttonState = pressed;
                    }
                }
            }
            else
            {
                if (buttonState == hot)
                {
                    buttonState = normal;
                }
            }
        }
        if (pOwner->IsDroppedItem((int)nIndex))
        {
            buttonState = dropped;
        }

        bool bDisabled = (button.fsState & TBSTATE_ENABLED) != 0;

        // figure out background/foreground/frame/text/image offset for the current state
        COLORREF clrBack = pOwner->GetBackgroundColor();    // background

        BOOL isBack = TRUE;         // TRUE if shall paint background
        COLORREF clrFore = xtAfxData.clrXPBarText;    // foreground
        COLORREF clrFrame = xtAfxData.clrXPHighlightBorder;   // frame
        BOOL isFrame = FALSE;        // TRUE if shall draw the frame

        if (button.fsState & TBSTATE_CHECKED)
        {
            if (bDisabled)
            {
                // embossed text on normal gray
                clrBack =  xtAfxData.clr3DFace;
                clrFrame = xtAfxData.clrXPMenuBorder;
                isBack  = TRUE;
                isFrame = TRUE;
            }
            else
            {
				clrFrame = xtAfxData.clrXPHighlightCheckedBorder;

                switch (buttonState)
                {
                case pressed:
                case hot:
                    clrFore = xtAfxData.clrXPHighlightText;
                    clrBack = xtAfxData.clrXPHighlightPushed;
                    isBack  = TRUE;
                    isFrame = TRUE;
                    break;
                case normal:
                case dropped:
                    clrBack = xtAfxData.clrXPHighlightChecked;
                    isBack  = TRUE;
                    isFrame = TRUE;
                    break;
                }
            }
        }
        else
        if (button.fsState &  TBSTATE_INDETERMINATE)
        {
            if (bDisabled)
            {
                // embossed text on light gray
                clrBack = xtAfxData.clr3DLight;
                isBack  = TRUE;
            }
            else
            {
                switch (buttonState)
                {
                case pressed:
                    clrFore = xtAfxData.clrXPHighlightText;
                    clrBack = xtAfxData.clrXPHighlightPushed;
                    isBack  = TRUE;
                    break;
                case hot:
                    clrBack = xtAfxData.clrXPHighlight;
                    isBack  = TRUE;
                    break;
                case normal:
                case dropped:
                    break;
                }
            }
        }
        else
        {
            if (bDisabled)
            {
                // embossed text on normal gray
            }
            else
            {
                switch (buttonState)
                {
                case pressed:
                    clrBack = xtAfxData.clrXPHighlightPushed;
                    clrFore = xtAfxData.clrXPBarTextPushed;
                    isBack  = TRUE;
                    isFrame = TRUE;
                    break;
                case hot:
                    clrBack = xtAfxData.clrXPHighlight;
					clrFore = xtAfxData.clrXPHighlightText;
                    isBack  = TRUE;
                    isFrame = TRUE;
                    break;
                case dropped:
                    break;
                case normal:
                    break;
                }
            }
        }

        // fill in the interior
        if (isBack)
        {
            pDC->FillSolidRect(rc, clrBack);
        }
        
        // rectangle for possible arrow
        CRect rcArrow = rc;
        rcArrow.left = rcArrow .right - RECTARROW_WIDTH;
		UINT nButtonStyle = pOwner->GetButtonStyle(nIndex);
        BOOL bHasArrow = pOwner->GetExtendedStyle() & TBSTYLE_EX_DRAWDDARROWS &&
					     nButtonStyle & TBSTYLE_DROPDOWN;
		BOOL bHasArrowSeparator = bHasArrow && !(nButtonStyle & BTNS_WHOLEDROPDOWN);

        CRect rcItem = rc;
        rcItem.right = rcArrow.left;

        // draw frame
        if (buttonState == dropped)
        {
            // current button has its popup displayed - draw menu compatible outline
            /*CPen pen(PS_SOLID, 1, xtAfxData.clrXPMenuBorder);
            CXTContextObjectHandler<CPen> penHandler(pDC, &pen);                        
            pDC->MoveTo(rc.left, rc.top);
            pDC->LineTo(rc.right-1, rc.top);
            pDC->LineTo(rc.right-1, rc.bottom);
            pDC->MoveTo(rc.left, rc.bottom);
            pDC->LineTo(rc.left, rc.top);            */
			pDC->Draw3dRect(rc, xtAfxData.clrXPMenuBorder, xtAfxData.clrXPMenuBorder);
        }
        else
        {
            // draw outline 
            if (isFrame)
            {
                pDC->Draw3dRect(&rc, clrFrame, clrFrame);
                // draw arrow if applicable
                if (bHasArrow && bHasArrowSeparator)
                {
                    pDC->Draw3dRect(&rcArrow, clrFrame, clrFrame);
                }
            }
        }

        // get image, button sizes
        CSize sizeImage, sizeButton;
        pOwner->GetSizes(sizeButton, sizeImage);

        // draw current color picker selection
        if (XT_DROPDOWNBUTTON* pDropButton = pOwner->FindDropDownButton(button.idCommand))
        {
            if (pDropButton->bShowColor && pDropButton->bColorPicker)
	        {
		        CRect rcColor = rcItem;
		        rcColor.left   += 4;
		        rcColor.right   = rcColor.left + (sizeButton.cx - 8);
		        rcColor.bottom -= 4;
		        rcColor.top     = rcColor.bottom - 3;
		        pDC->FillSolidRect(rcColor, 
			        bDisabled ? xtAfxData.clrXPDisabled : pDropButton->clrColor);
		        pDC->ExcludeClipRect(rcColor);
	        }
        }

        // setup text color
        CXTContextTextColorHandler handlerTextColor(pDC, clrFore);
        
        if (isBack)
        {
            pDC->SetBkColor(clrBack);
        }

        if (bHasArrow)
        {
			CRect rect = rcArrow;
			if (!(pOwner->GetStyle() & TBSTYLE_LIST) &&
				bHasArrow && !bHasArrowSeparator)
			{
				rect.left -= 10;
				rect.bottom = rect.top + sizeImage.cy + 3;
			}			
            DrawDownArrow(pDC, rect);
        }

        // figure out NW corner to start drawing 
        CPoint ptImage = rc.TopLeft();

        if (button.iString >= 0)
        {
            // the button has some text, 
            CString strText = pOwner->GetButtonText(nIndex);

            //figure out its placement
            if (pOwner->GetStyle() & TBSTYLE_LIST)
            {
				CPoint ptText;
				DWORD dwDrawTextFlags;
				if (button.iBitmap == I_IMAGENONE)
				{
					// no image, just display centered text
					ptText.x = ptImage.x + 2;
					ptText.y = ptImage.y + 1;
					dwDrawTextFlags = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
				}
				else
				{
					// layout left-to-right: image, button
					ptImage.y = (rc.top + rc.bottom - sizeImage.cy) / 2;
					ptImage.x += (sizeButton.cx - sizeImage.cx) / 2 + 2;
					ptText.x = ptImage.x + sizeImage.cx + 3;
					ptText.y = rc.top + 1;
					dwDrawTextFlags = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
				}
                
                
                CSize sizeText;
                sizeText.cx = rc.right - ptText.x /*- 3*/;
				if (bHasArrow)
                {
					sizeText.cx -= RECTARROW_WIDTH + 3;
					if (!bHasArrowSeparator)
					{
						sizeText.cx += 10;
					}
                }
                sizeText.cy = rc.Height() - 2;

				RenderText(pDC, pOwner, ptText, sizeText, strText, 
					dwDrawTextFlags, bDisabled);
            }
            else
            {
                // layout top-down: image (centered), text (centered)
                CPoint ptText = ptImage;
                
                ptImage.y += TBLIST_TOP_MARGING;
                int nTakenSpace = sizeImage.cx ;	 // accommodate the shadow
                if (bHasArrow)
                {
                    nTakenSpace += RECTARROW_WIDTH;
                }
                ptImage.x += (rc.right - rc.left - nTakenSpace) / 2 - 1;

                ptText.x = rc.left + 3;
                ptText.y += sizeImage.cy + TBLIST_TEXT_PADDING;

                CSize sizeText;
                sizeText.cx = rc.right - rc.left - 6;
                if (bHasArrow && bHasArrowSeparator)
                {
                    sizeText.cx -= RECTARROW_WIDTH;
                }
                sizeText.cy = sizeImage.cy;
				RenderText(pDC, pOwner, ptText, sizeText, strText, DT_CENTER | DT_TOP, bDisabled);
			}
        }
        else
        {
            // no text, just center icon in the rect
			int nTakenSpace = sizeImage.cx;
            if (bHasArrow)
            {
                nTakenSpace += RECTARROW_WIDTH + 2;
            }                
            
            if (pOwner->GetStyle() & TBSTYLE_LIST)
            {            
                ptImage.y = (rc.top + rc.bottom - sizeImage.cy) / 2 ;
            }
            else
            {
                ptImage.y += TBLIST_TOP_MARGING;
            }
			ptImage.x += (rc.right - rc.left - nTakenSpace) / 2;
        }

        // get icon
//        if (button.iBitmap >= 0)
        

#if 1
		{
			CXTImage* pImage = XTImageManager()->GetImage(button.idCommand, TRUE);
			
			if (pImage)
			{
				CPoint pt = ptImage + CSize (1, 1);
                
				if (bDisabled)
                {
					if ( pImage->GetDisabledIcon() )
					{
						pDC->DrawState( pt, sizeImage, pImage->GetDisabledIcon(), DSS_NORMAL | DST_ICON, ( HBRUSH )NULL );
					}
					else
					{
						CBrush brush( xtAfxData.clrXPDisabled );
						pDC->DrawState( pt, sizeImage, pImage->GetIcon(), DST_ICON | DSS_MONO, ( HBRUSH )brush );
					}
                } 
				else
				{
					if ((buttonState == pressed) || (button.fsState & TBSTATE_CHECKED)) 
						pDC->DrawState(pt, sizeImage,  pImage->GetIcon(), (UINT)DST_ICON, HBRUSH(0));
					else if (buttonState != hot)
						pDC->DrawState(pt, sizeImage,  pImage->GetFadedIcon(), DST_ICON, HBRUSH(0));
					else
					{ 
						pDC->DrawState(CPoint(pt.x + 1, pt.y + 1), sizeImage,  pImage->GetShadowIcon(), DST_ICON, HBRUSH(0));
						pDC->DrawState(CPoint(pt.x - 1, pt.y - 1), sizeImage,  pImage->GetIcon(), (UINT)DST_ICON, HBRUSH(0));
					}
				}
			}
#else
        if (button.iBitmap >= 0)
		{
			
            CImageList* imageList = NULL;
			CImageList* imageListDisabled = pOwner->GetToolBarCtrl().GetDisabledImageList();

            if (bDisabled)
            {
                imageList = imageListDisabled;
				
				// draw disabled image list normally...
				if (imageListDisabled)
				{
					bDisabled = false;
				}
            }
            else
            if (buttonState == hot)
            {
                imageList = pOwner->GetToolBarCtrl().GetHotImageList();
            }
            if (!imageList)
            {
                imageList = pOwner->GetToolBarCtrl().GetImageList();
            }
    
            if (imageList)
            {
#ifdef _DEBUG
				int iImageCount = imageList->GetImageCount();
				ASSERT(button.iBitmap < iImageCount);
#endif
				HICON hIcon = imageList->ExtractIcon(button.iBitmap);
				ASSERT(hIcon != NULL);
                CXTIconHandle image(hIcon);

				CPoint ptOrigin = ptImage;

                if (bDisabled)
                {
					CBrush brush( xtAfxData.clrXPDisabled );
                    pDC->DrawState(ptOrigin, sizeImage, image, DSS_MONO | DST_ICON, (HBRUSH)brush);
                }
                else
                {
                    if (buttonState == hot && (button.fsState & TBSTATE_CHECKED) == 0)
                    {
						// if we have a disabled image list, use it in place of creating a mono brush image.
						if (imageListDisabled)
						{
							CXTIconHandle imageDiabled(imageListDisabled->ExtractIcon(button.iBitmap));
							pDC->DrawState(ptOrigin + CSize(1,1), sizeImage, imageDiabled,
								DSS_NORMAL | DST_ICON, (HBRUSH)NULL);							
						}
						else
						{
							CBrush brush( xtAfxData.clrXPIconShadow );

							pDC->DrawState(ptOrigin + CSize(1,1), sizeImage, image,
								DSS_NORMAL | DST_ICON | DSS_MONO, (HBRUSH)brush);
						}
						ptOrigin -= CSize(1,1);
                    }

                    pDC->DrawState(ptOrigin, sizeImage, image, DSS_NORMAL | DST_ICON, (HBRUSH)NULL);
                }
            }
#endif
        }       
        return TRUE;
    }

};

///////////////////////////////////////////////////////////////////////
// A proxy for drawing commands list box, XP style

class CXTCmdsLBoxProxyXP : public IXTCommandsListBoxProxy
{
    IXTProxySite* m_pSite;

public:
    CXTCmdsLBoxProxyXP(IXTProxySite* pSite)
        : m_pSite( pSite )
    {}

	virtual void OnDrawItem(LPDRAWITEMSTRUCT lpDIS, CXTCommandsListBox* pOwner)
	{
		int   pos    = ( int )lpDIS->itemData;
		UINT  nCmdID = pOwner->GetSite( )->GetItemCommand( pos );
		CRect rcItem = lpDIS->rcItem;
		CDC*  pDC	 = CDC::FromHandle( lpDIS->hDC );
		
		int nSavedDC = pDC->SaveDC( );
		
		CRect rcHilite( rcItem );
		
		// Fill the item's background color first...
		pDC->FillSolidRect( &rcItem, xtAfxData.clrXPMenuTextBack );
		
		CRect rc = rcItem;
		rc.right = rc.left + pOwner->GetIconSize( ).cx;
		pDC->FillSolidRect( &rc, xtAfxData.clrXPBarFace );
		
		pDC->SelectObject( pOwner->GetFont( ) );
		
		// Paint the color item in the color requested
		if ( lpDIS->itemState & ODS_SELECTED )
		{
			pDC->FillSolidRect( &rcHilite, xtAfxData.clrXPHighlight );
			pDC->Draw3dRect( &rcHilite,
				xtAfxData.clrXPHighlightBorder, xtAfxData.clrXPHighlightBorder );
		}
		
		pDC->SetTextColor( ( lpDIS->itemState & ODS_SELECTED ) 
			? xtAfxData.clrXPHighlightText : xtAfxData.clrXPMenuText );
		
		rcHilite.InflateRect( -2,-2 );
		
		pDC->SetBkMode( TRANSPARENT );
		
		// Get and display item text.
		CString strItemText;
		pOwner->GetText( lpDIS->itemID, strItemText );
		
		// Calculate the text size first.
		CRect rcText( lpDIS->rcItem );
		pDC->DrawText( strItemText, -1, &rcText,
			DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_CALCRECT );
		
		// Make necessary adjustments to the text size.
		rcText.top    = rcItem.top;
		rcText.bottom = rcItem.top + rcItem.Height( );
		rcText.left	  = pOwner->GetIconSize( ).cx + 6;
		rcText.right  = lpDIS->rcItem.right-pOwner->GetIconSize( ).cx;
		
		// Draw the text.
		pDC->DrawText( strItemText, -1, &rcText,
			DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS );
		
		// popup menu	
		if ( nCmdID == 0 )
		{
			CRect rcIcon( lpDIS->rcItem );
			rcIcon.left = rcIcon.right-11;
			
			CPen pen( PS_SOLID, 1, xtAfxData.clrXPMenuText );
			pDC->SelectObject( &pen );
			
			int x = rcIcon.left;
			int y = rcIcon.top+6;
			
			pDC->MoveTo(   x,y   );
			pDC->LineTo(   x,y+9 );
			pDC->MoveTo( x+1,y+1 );
			pDC->LineTo( x+1,y+8 );
			pDC->MoveTo( x+2,y+2 );
			pDC->LineTo( x+2,y+7 );
			pDC->MoveTo( x+3,y+3 );
			pDC->LineTo( x+3,y+6 );
			pDC->MoveTo( x+4,y+4 );
			pDC->LineTo( x+4,y+5 );
		}
		
		else 
		{
			CXTImage* pImage = XTImageManager()->GetImage(nCmdID);
		
			if (pImage)
			{
				CRect rcIcon( lpDIS->rcItem );
				rcIcon.right = pOwner->GetIconSize( ).cx;
				CSize sizeImage(16, 16);

				CPoint pt;
				pt.x = (rcIcon.left + rcIcon.right - sizeImage.cx) / 2;
				pt.y = (rcIcon.top + rcIcon.bottom - sizeImage.cy) / 2;
				
				if ( lpDIS->itemState & ODS_SELECTED )
				{
					pDC->DrawState(CPoint(pt.x + 1, pt.y + 1), sizeImage,  pImage->GetShadowIcon(), DST_ICON, HBRUSH(0));
					pDC->DrawState(CPoint(pt.x - 1, pt.y - 1), sizeImage,  pImage->GetIcon(), (UINT)DST_ICON, HBRUSH(0));
				}
				else
				{
					pDC->DrawState( pt, sizeImage, pImage->GetFadedIcon(), DST_ICON, HBRUSH(0) );
				}
			}
		}
		
		pDC->RestoreDC( nSavedDC );
	}

	class CWndFriend : public CWnd
	{
		friend class CXTCmdsLBoxProxyXP;
	};

	virtual void OnPaint(
		CXTCommandsListBox* pOwner)
	{
		CPaintDC dc( pOwner );
		
		// Get the client rect.
		CRect rectClient;
		pOwner->GetClientRect( &rectClient );
		
		// Paint to a memory device context to reduce screen flicker.
		CXTMemDC memDC( &dc, rectClient, xtAfxData.clrXPMenuTextBack );
		
		CRect rc = rectClient;
		rc.right = rc.left + pOwner->GetIconSize( ).cx;
		memDC.FillSolidRect( &rc, xtAfxData.clrXPBarFace );
		
		// Let the window do its default painting...
		( ( CWndFriend* )pOwner )->CWnd::DefWindowProc(
			WM_PAINT, ( WPARAM )memDC.m_hDC, 0 );
	}
};

//////////////////////////////////////////////////////////////////////
// Skin factory implementation, XP-style
//
static 
class CXTSkinXP : public IXTSkin, public IXTProxySite
{
    // Proxy to render flat combo.
    CXTFlatComboProxyXP m_proxyFlatCombo;
 
    // Proxy to render flat edit
    CXTFlatEditProxyXP m_proxyFlatEdit;
 
    // proxy to render toolbar
    CXTTbProxyXP m_proxyTb;

    // proxy to render commands list box.
	CXTCmdsLBoxProxyXP m_proxyCmdsLBox;

    // Flag to track status of toolbar drag/resize operation
    bool m_bTracking;
        
public:    
    CXTSkinXP()
    : m_bTracking(FALSE)
    , m_proxyFlatCombo((IXTProxySite*)this)
	, m_proxyFlatEdit((IXTProxySite*)this)
    , m_proxyTb((IXTProxySite*)this)
	, m_proxyCmdsLBox((IXTProxySite*)this)
    { }

    ~CXTSkinXP()
    { }

    // Returns a proxy for drawing toolbar button. 
    // Implementation can return NULL to indicate default control behaviour shall be used
    virtual IXTToolbarProxy* GetToolbarProxy()
    {
        return &m_proxyTb;
    }

    // Returns a proxy for drawing flat edit. 
    // Implementation can return NULL to indicate default control behaviour shall be used
    virtual IXTFlatEditProxy* GetFlatEditProxy()
    {
        return &m_proxyFlatEdit;
    }

    // Returns a proxy for drawing flat combo. 
    // Implementation can return NULL to indicate default control behaviour shall be used
    virtual IXTFlatComboProxy* GetFlatComboProxy() 
    {
        return &m_proxyFlatCombo;
    }

    // Returns a proxy for drawing command list box. 
    // Implementation can return NULL to indicate default control behaviour shall be used
	virtual IXTCommandsListBoxProxy* GetCommandsListBoxProxy()
	{
        return &m_proxyCmdsLBox;
	}

    // notify if toolbar is currently tracked (dragged/resized)
    virtual void NotifyToolbarTracking(BOOL bTracking)
    {
        m_bTracking = bTracking != 0;
    }

    virtual bool IsTracking()
    {
        return m_bTracking;
    }
} s_factoryXP;


//////////////////////////////////////////////////////////////////////
// Skin factory implementation, default style
//
static 
class CXTSkinDefault : public IXTSkin
{
public:
    CXTSkinDefault()
    { }

    ~CXTSkinDefault()
    { }

    // Returns a proxy for drawing toolbar button. 
    // Implementation can return NULL to indicate default control behaviour shall be used
    virtual IXTToolbarProxy* GetToolbarProxy()
    {
        // use default
        return NULL;
    }

    // Returns a proxy for drawing flat edit. 
    // Implementation can return NULL to indicate default control behaviour shall be used
    virtual IXTFlatEditProxy* GetFlatEditProxy()
    {
        return NULL;
    }

    // Returns a proxy for drawing flat combo. 
    // Implementation can return NULL to indicate default control behaviour shall be used
    virtual IXTFlatComboProxy* GetFlatComboProxy() 
    {
        // use default
        return NULL;
    }

    // Returns a proxy for drawing command list box. 
    // Implementation can return NULL to indicate default control behaviour shall be used
	virtual IXTCommandsListBoxProxy* GetCommandsListBoxProxy()
	{
        // use default
        return NULL;
	}

    // notify if toolbar is currently tracked (dragged/resized)
    virtual void NotifyToolbarTracking(BOOL /*bTracking*/)
    {
        
    }

} s_factoryDefault;


// 
// Returns an instance of skin factory to use
IXTSkin* IXTSkin::GetInstance()
{
    return (xtAfxData.bXPMode) ? (IXTSkin*)&s_factoryXP : &s_factoryDefault;
};
