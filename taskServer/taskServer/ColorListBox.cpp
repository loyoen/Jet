/********************************************************************/
/*																	*/
/*  ColorListBox.cpp												*/
/*																	*/
/*  Implementation of the CColorListBox.							*/
/*																	*/
/*  Programmed by Pablo van der Meer								*/
/*	http://www.pablovandermeer.nl									*/
/*																	*/
/*  Last updated: 10 july 2002										*/
/*																	*/
/********************************************************************/


#include "stdafx.h"
#include "Resource.h"
#include "ColorListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
RGB(153,255,255)
RGB(153,255,153)
RGB(255,255,255)
RGB(255,204,204)
*/
CColorListBox::CColorListBox()
{
	ZeroMemory(m_arrListBoxItemStyle,10*sizeof(t_listbox_item_style));
	m_nNumStyle=0;
	m_nBitmapID=0;
	m_nIconCnt=0;
}

CColorListBox::~CColorListBox()
{

}


BEGIN_MESSAGE_MAP(CColorListBox, CListBox)
	//{{AFX_MSG_MAP(CColorListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/********************************************************************/
/*																	*/
/* Function name : AddString										*/		
/* Description   : Add string to the listbox and save color info.	*/
/*																	*/
/********************************************************************/
int CColorListBox::AddListBoxItemStyle(int nIconIndex,COLORREF clrText,COLORREF clrBg)
{
    if(m_nNumStyle>=14)
		return -1;
	else
	{
		m_arrListBoxItemStyle[m_nNumStyle].clrBg= clrBg;
		m_arrListBoxItemStyle[m_nNumStyle].clrText =clrText;
		//Load the Icon
		m_arrListBoxItemStyle[m_nNumStyle].nIconIndex=nIconIndex;
		++m_nNumStyle;
	}
	return 0;
}
int CColorListBox::AddLogString(int nClass,LPCTSTR lpszItem)
{
	// Add the string to the list box
	int nIndex = CListBox::AddString(lpszItem);
   
	// save color data
	if (nIndex >= 0)
		SetItemData(nIndex, nClass);

	//Set the scroll info
	SendMessage(WM_VSCROLL,(WPARAM)SB_BOTTOM,(LPARAM)NULL);
	return nIndex;
}

void CColorListBox::SetImageList(int nBitmapID,int nCnt)
{
	m_nBitmapID=nBitmapID;
	m_nIconCnt=nCnt;
}
/********************************************************************/
/*																	*/
/* Function name : DrawItem											*/		
/* Description   : Called by the framework when a visual aspect of	*/
/*				   an owner-draw list box changes.					*/
/*																	*/
/********************************************************************/
void CColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    int nIndex=(int)(lpDrawItemStruct->itemData);

	CRect    rcClient(lpDrawItemStruct->rcItem);

	COLORREF clrOld;
    int nBkMode;

	// get color info from item data
    CString strText;
	GetText(lpDrawItemStruct->itemID,strText);	


	COLORREF clrText=m_arrListBoxItemStyle[nIndex].clrText ;
	COLORREF clrBg=m_arrListBoxItemStyle[nIndex].clrBg ;
	UINT     nIconIndex=m_arrListBoxItemStyle[nIndex].nIconIndex ;

     //»­±³¾°À²:)
	pDC->FillSolidRect(&rcClient,clrBg);

	// item selected ?
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		 (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		//CBrush brush(::GetSysColor(COLOR_HIGHLIGHT));
		//pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	}

	// item deselected ?
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&	
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
         pDC->FillSolidRect(&rcClient,clrBg);
	}	 	

	//Focus ÏÔÊ¾

	// item has focus ?
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		//pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
	}

	// lost focus ?
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
		!(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		//pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
	}

	//»­Í¼±ê
    CRect rect = lpDrawItemStruct->rcItem;
    CRect rcIcon=rect;
    rcIcon.DeflateRect(2,0);
	rcIcon.right =rcIcon.left +16;

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitmap;
	bitmap.LoadBitmap(m_nBitmapID);
	memDC.SelectObject(bitmap);	
	pDC->BitBlt(rcIcon.left ,rcIcon.top ,16,16,&memDC,nIconIndex*16,0,SRCCOPY); 

    
	pDC->SetBkMode(TRANSPARENT);
	// set the background mode to TRANSPARENT
	rect.left =rcIcon.right +5;

	nBkMode = pDC->SetBkMode(TRANSPARENT);

	//ÉèÖÃÎÄ×ÖÑÕÉ«
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		clrOld = pDC->SetTextColor(RGB(255,0,0));
	else 
	if (lpDrawItemStruct->itemState & ODS_DISABLED)
		clrOld = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	else
		clrOld=pDC->SetTextColor(clrText);

	//»­ÎÄ×ÖÀ²:)
	// text format
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;
	
	// draw the text
	pDC->DrawText(strText, -1, &rect, nFormat);

	// restore old values
	pDC->SetTextColor(clrOld); 
	pDC->SetBkMode(nBkMode);
}


void CColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight =::GetSystemMetrics(SM_CYMENUCHECK);	
}
