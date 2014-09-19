// ColorButtpn.cpp : 实现文件
//

#include "stdafx.h"
#include "../jetManager.h"
#include "ColorButton.h"
#include ".\colorbutton.h"


// CColorButton

IMPLEMENT_DYNAMIC(CColorButton, CButton)
CColorButton::CColorButton()
{
	m_bHighlight	= FALSE;
	m_bChecked		= FALSE;

	m_color	= RGB ( 0, 0, 0 );
	
	m_hHand	= AfxGetApp()->LoadCursor(IDC_HANDCUR);
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CColorButton 消息处理程序
void CColorButton::Set_text_color ( COLORREF color )
{
	m_color	= color;
}

void CColorButton::Reset ( )
{
	m_bHighlight	= FALSE;
	m_bChecked		= FALSE;
	m_color			= RGB ( 0, 0, 0 );
	SetWindowText ( " " );
}

void CColorButton::Set_highlight ( BOOL bFlag )
{
	m_bHighlight	= bFlag;
}

void CColorButton::DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	// 获取一个CDC指针
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	//定义按钮区域并初始化
	CRect rect(lpDrawItemStruct->rcItem);

	//设置背景模式
	COLORREF oc = pDC->GetTextColor();
	int iObk = pDC->SetBkMode(TRANSPARENT);

	//初始化按钮状态
	UINT state = lpDrawItemStruct->itemState;

	CFont * pOldFont = NULL;

	int iYOffset = 0, iXOffset = 0;

	CString strText;
	GetWindowText(strText);

	rect.top  += iYOffset;
	rect.left += iXOffset;

	if (state & ODS_DISABLED)
	{		
		//按钮置灰
		CBrush grayBrush;
		grayBrush.CreateSolidBrush (GetSysColor (COLOR_GRAYTEXT));
		CSize sz = pDC->GetTextExtent(strText);
		int x = rect.left + (rect.Width() - sz.cx)/2;
		int y = rect.top + (rect.Height() - sz.cy)/2;
		rect.top  += 2;
		rect.left += 2;
		pDC->SetTextColor(GetSysColor(COLOR_3DHIGHLIGHT));
		pDC->DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		rect.top  -= 2;
		rect.left -= 2;
		pDC->SetTextColor(GetSysColor(COLOR_GRAYTEXT));
		pDC->DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		if ( m_bHighlight )	//光标在按钮上
		{
//			if (state & ODS_SELECTED)
			if (m_bChecked)
			{
				//按下按钮
				pDC->Draw3dRect(rect,GetSysColor(COLOR_3DFACE),GetSysColor(COLOR_3DFACE));
			}
			else
			{
				//未按下按钮
				pDC->Draw3dRect(rect,GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));
			}
			//字体颜色
			pDC->SetTextColor( m_color );	//RGB(0,0,255)

			//加下画线（也可以用其他字体）
			//if (fUnderline.GetSafeHandle() == NULL)
			//{
			//	CFont * pFont = GetFont();
			//	ASSERT(pFont);
			//	LOGFONT lf;
			//	pFont->GetLogFont(&lf);
			//	lf.lfStrikeOut	= TRUE;
			//	//lf.lfUnderline = TRUE;
			//	fUnderline.CreateFontIndirect(&lf);		
			//}

			//pOldFont = pDC->SelectObject(&fUnderline);
		}
		else
			pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));

		pDC->DrawText(strText, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//	if (pOldFont) pDC->SelectObject(pOldFont);
	}
}

BOOL CColorButton::OnEraseBkgnd(CDC* pDC)
{
	COLORREF cr = GetSysColor(COLOR_3DFACE);
	int r = GetRValue(cr);
	int g = GetGValue(cr);
	int b = GetBValue(cr);
	if (r > 1) r -= 2;
	if (g > 1) g -= 2;
	if (r < 3 && g < 3 && b < 253) b += 2;
	COLORREF cr1 = RGB(r,g,b);
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(rc, cr1);
	
	return CButton::OnEraseBkgnd(pDC);
}
