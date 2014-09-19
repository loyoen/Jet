// DlgQuality.cpp : 实现文件
//

#include "stdafx.h"
#include "../jetManager.h"
#include "DlgQuality.h"
#include "dlgManager.h"
#include ".\dlgquality.h"

// CDlgQuality 对话框

IMPLEMENT_DYNAMIC(CDlgQuality, CDialog)
CDlgQuality::CDlgQuality(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgQuality::IDD, pParent)
{
	m_pParent	= (CDlgManager*) pParent;

	m_iShift	= 0;
}

CDlgQuality::~CDlgQuality()
{
}

void CDlgQuality::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_MSG1, m_editMsg1);
	DDX_Control(pDX, IDC_EDIT_MSG2, m_editMsg2);

	DDX_Control(pDX, IDC_BTN_Machine1, m_btnMachine1);
	DDX_Control(pDX, IDC_BTN_Machine2, m_btnMachine2);
	DDX_Control(pDX, IDC_BTN_Machine3, m_btnMachine3);
	DDX_Control(pDX, IDC_BTN_Machine4, m_btnMachine4);
	DDX_Control(pDX, IDC_BTN_Machine5, m_btnMachine5);
	DDX_Control(pDX, IDC_BTN_Machine6, m_btnMachine6);
	DDX_Control(pDX, IDC_BTN_Machine7, m_btnMachine7);
	DDX_Control(pDX, IDC_BTN_Machine8, m_btnMachine8);
	DDX_Control(pDX, IDC_BTN_Machine9, m_btnMachine9);
	DDX_Control(pDX, IDC_BTN_Machine10, m_btnMachine10);
	DDX_Control(pDX, IDC_BTN_Machine11, m_btnMachine11);
	DDX_Control(pDX, IDC_BTN_Machine12, m_btnMachine12);
	DDX_Control(pDX, IDC_BTN_Machine13, m_btnMachine13);
	DDX_Control(pDX, IDC_BTN_Machine14, m_btnMachine14);
	DDX_Control(pDX, IDC_BTN_Machine15, m_btnMachine15);
	DDX_Control(pDX, IDC_BTN_Machine16, m_btnMachine16);
}


BEGIN_MESSAGE_MAP(CDlgQuality, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_LEFT, OnBnClickedBtnLeft)
	ON_BN_CLICKED(IDC_BTN_RIGHT, OnBnClickedBtnRight)
	ON_WM_ERASEBKGND()

	ON_BN_CLICKED(IDC_BTN_Machine1, OnBnClickedMachine1)
	ON_BN_CLICKED(IDC_BTN_Machine2, OnBnClickedMachine2)
	ON_BN_CLICKED(IDC_BTN_Machine3, OnBnClickedMachine3)
	ON_BN_CLICKED(IDC_BTN_Machine4, OnBnClickedMachine4)
	ON_BN_CLICKED(IDC_BTN_Machine5, OnBnClickedMachine5)
	ON_BN_CLICKED(IDC_BTN_Machine6, OnBnClickedMachine6)
	ON_BN_CLICKED(IDC_BTN_Machine7, OnBnClickedMachine7)
	ON_BN_CLICKED(IDC_BTN_Machine8, OnBnClickedMachine8)
	ON_BN_CLICKED(IDC_BTN_Machine9, OnBnClickedMachine9)
	ON_BN_CLICKED(IDC_BTN_Machine10, OnBnClickedMachine10)
	ON_BN_CLICKED(IDC_BTN_Machine11, OnBnClickedMachine11)
	ON_BN_CLICKED(IDC_BTN_Machine12, OnBnClickedMachine12)
	ON_BN_CLICKED(IDC_BTN_Machine13, OnBnClickedMachine13)
	ON_BN_CLICKED(IDC_BTN_Machine14, OnBnClickedMachine14)
	ON_BN_CLICKED(IDC_BTN_Machine15, OnBnClickedMachine15)
	ON_BN_CLICKED(IDC_BTN_Machine16, OnBnClickedMachine16)
//	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlgQuality 消息处理程序
void CDlgQuality::OnBnClickedMachine1()
{
}

void CDlgQuality::OnBnClickedMachine2()
{
}

void CDlgQuality::OnBnClickedMachine3()
{
}

void CDlgQuality::OnBnClickedMachine4()
{
}

void CDlgQuality::OnBnClickedMachine5()
{
}

void CDlgQuality::OnBnClickedMachine6()
{
}

void CDlgQuality::OnBnClickedMachine7()
{
}

void CDlgQuality::OnBnClickedMachine8()
{
}

void CDlgQuality::OnBnClickedMachine9()
{
}

void CDlgQuality::OnBnClickedMachine10()
{
}

void CDlgQuality::OnBnClickedMachine11()
{
}

void CDlgQuality::OnBnClickedMachine12()
{
}

void CDlgQuality::OnBnClickedMachine13()
{
}

void CDlgQuality::OnBnClickedMachine14()
{
}

void CDlgQuality::OnBnClickedMachine15()
{
}

void CDlgQuality::OnBnClickedMachine16()
{
}

//void CDlgQuality::OnBnClickedMachine()
//{
//	//if ( !m_bFlag1 )
//	//	return;
//
//	//m_btnMessage1.m_bChecked	= !m_btnMessage1.m_bChecked;
//	//m_pDoc->m_bFlag1	= !m_pDoc->m_bFlag1;
//	//m_pDoc->UpdateAllViews ( NULL, 1 );
//	//RedrawWindow ( );
//}

void CDlgQuality::OnBnClickedCancel()
{
	this->ShowWindow ( SW_HIDE );
	//OnCancel();
}

void CDlgQuality::OnBnClickedOk()
{
	this->ShowWindow ( SW_HIDE );
	//OnOK();
}

void CDlgQuality::OnBnClickedBtnLeft()
{
	//if ( 0 == m_iShift )
	//	return;

	m_iShift	-= SHIFT_QUALITY;

	Show_quality ( );

	//if ( m_iShift <= 0 )
	//{
	//	GetDlgItem ( IDC_BTN_LEFT )->EnableWindow( FALSE );
	//}

	//GetDlgItem ( IDC_BTN_RIGHT )->EnableWindow( );
}

void CDlgQuality::OnBnClickedBtnRight()
{
	//if ( m_pQuality->m_iLine_cnt < m_iShift )
	//	return;

	m_iShift	+= SHIFT_QUALITY;
	Show_quality ( );

	//if ( m_iShift >= m_pQuality->m_iLine_cnt - SHIFT_QUALITY )
	//{
	//	GetDlgItem ( IDC_BTN_RIGHT )->EnableWindow( FALSE );
	//}

	//GetDlgItem ( IDC_BTN_LEFT )->EnableWindow( );
}

void CDlgQuality::Show_quality ( )
{
	UpdateWindow ( );
	RedrawWindow ( );	/////

	CDC *pDC	= GetDC ( );

	if ( NULL == pDC )
		return;

	m_pQuality->Draw_quality ( pDC, m_iShift, SHIFT_QUALITY );

	GetDlgItem ( IDC_BTN_LEFT )->EnableWindow ( m_iShift>0 );
	GetDlgItem ( IDC_BTN_RIGHT )->EnableWindow( m_iShift < m_pQuality->m_iLine_cnt - SHIFT_QUALITY );

	ReleaseDC ( pDC );
//	RedrawWindow ( );	/////
}

BOOL CDlgQuality::OnEraseBkgnd(CDC* pDC)
{
	CRect		rc;
	COLORREF	cr = RGB(0,0,0);

	CRect		rcClient;
	GetDlgItem ( IDC_QUALITY_PIC )->GetClientRect ( rc );
//	GetDlgItem ( IDC_QUALITY_PIC )->GetWindowRect ( rc );
//	GetDlgItem ( IDC_QUALITY_PIC )->GetClientRect ( rcClient );
//	GetDlgItem ( IDC_QUALITY_PIC )->GetWindowRect ( rcClient );
//	GetWindowRect ( rcClient );
//	GetClientRect ( rcClient );

	//rc.left		+= rcClient.left;
	//rc.right	+= rcClient.left;
	//rc.top		+= rcClient.top;
	//rc.bottom	+= rcClient.top;

	rc.left		+= 11;
	rc.right	+= 11;
	rc.top		+= 46;
	rc.bottom	+= 46;

	CDialog::OnEraseBkgnd(pDC);

	pDC->FillSolidRect ( rc, cr );
//	pDC->FillSolidRect ( rcClient, cr );

	// 重新画全部曲线
	if ( m_pQuality->m_iLine_cnt > 0 )
	{
		m_pQuality->Draw_quality ( pDC, m_iShift, SHIFT_QUALITY );

		//Show_quality ( );
	}

//	return CDialog::OnEraseBkgnd(pDC);
	return 0;
}

BOOL CDlgQuality::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect		rc;

//	GetDlgItem ( IDC_QUALITY_PIC )->GetWindowRect ( rc );
	GetDlgItem ( IDC_QUALITY_PIC )->GetClientRect ( rc );

	rc.left		+= 11;
	rc.right	+= 11;
	rc.top		+= 46;
	rc.bottom	+= 46;

	m_pQuality->Set_drawing_rect ( rc );
	m_pQuality->Set_interface_dlg ( this );

	GetDlgItem ( IDC_BTN_LEFT )->EnableWindow( FALSE );
	GetDlgItem ( IDC_BTN_RIGHT )->EnableWindow( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgQuality::Update_item_interface ( DWORD dwIndex, COLORREF cr, CString strTitle )
{
	CColorButton	*pButton;

	pButton	= (CColorButton *)GetDlgItem ( IDC_BTN_Machine1+dwIndex );

	pButton->SetWindowText ( strTitle );
	pButton->Set_text_color ( cr );
	pButton->Set_highlight ( );
}

void CQuality::Draw_quality ( CDC *pDC, int iShift, int iCnt )
{
	int		iDone_number;

	if ( 0 == iCnt )
		return;

	// 找到头一个
	LPQLINE	lpQLine;

	lpQLine	= m_lpQLine_lst_head;
	if ( (iShift+iCnt) < m_iLine_cnt )
	{
		while ( NULL != lpQLine )
		{
			if ( lpQLine->m_iIndex == (iShift+iCnt-1) )
			{
				break;
			}

			lpQLine	= lpQLine->m_lpNext;
		}
	}

	// 先画整体平均
	CPen *	pOldPen = pDC->SelectObject(m_penLine);

	int iY	= m_rcRect.Height() - (m_fMean * m_fFactor_V + m_rcRect.top + 3*MARGIN_V) + 0.5;

	pDC->MoveTo ( m_rcRect.left, iY );
	pDC->LineTo ( m_rcRect.right, iY );

	pDC->SelectObject(pOldPen);

	// 顺序画之
	iDone_number	= 0;
	while ( NULL != lpQLine )
	{
		// 已足数
		if ( iCnt == iDone_number )
			break;

		iDone_number++;

		// 0 号颜色留给当前加亮用
		lpQLine->Draw ( pDC, m_penLine[iDone_number],
			m_fFactor_H, m_fFactor_V,
			m_rcRect.left+MARGIN_H, m_rcRect.top+MARGIN_V,
			m_rcRect.Height() - (2*MARGIN_V) );

		m_pDlg->Update_item_interface ( iDone_number-1, m_crLine_colors[iDone_number], lpQLine->m_strName );

		lpQLine	= lpQLine->m_lpNext;
	}
}

UINT CDlgQuality::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

//	return CDialog::OnNcHitTest(point);
	return	0;
}
