// DlgControl.cpp : 实现文件
//

#include "stdafx.h"
#include "../jetManager.h"
#include "DlgControl.h"
#include ".\dlgcontrol.h"

#include "DlgOperator.h"
#include "DlgPrePrinting.h"
#include "controlClientDlg.h"

// CDlgControl 对话框

IMPLEMENT_DYNAMIC(CDlgControl, CDialog)
CDlgControl::CDlgControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgControl::IDD, pParent)
{
	m_pDlgOperator	= NULL;
	m_pDlgPrePrinting	= NULL;
	m_pDlgClient	= NULL;

	m_pParent	= pParent;

	m_iCurrentItem	= 0;
}

CDlgControl::~CDlgControl()
{
	if ( NULL != m_pDlgOperator )
		delete	m_pDlgOperator	;

	if ( NULL != m_pDlgPrePrinting )
		delete	m_pDlgPrePrinting	;

	if ( NULL != m_pDlgClient )
		delete	m_pDlgClient	;
}

void CDlgControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_tabCtrl);
}


BEGIN_MESSAGE_MAP(CDlgControl, CDialog)
	ON_WM_ERASEBKGND()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTcnSelchangeTab)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgControl 消息处理程序
void CDlgControl::FullScreen ( )
{
	RECT			rectDesktop;
    WINDOWPLACEMENT	wpNew;

	GetWindowPlacement ( &wpNew );

	//Adjust RECT to new size of window
	::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
	::AdjustWindowRectEx( &rectDesktop, GetStyle(), TRUE, GetExStyle() );

	// Remember this for OnGetMinMaxInfo()
	//m_FullScreenWindowRect = rectDesktop;

	wpNew.showCmd			= SW_SHOWNORMAL;
	wpNew.rcNormalPosition	= rectDesktop;

	//don't allow the toolbar to dock
	SetWindowPlacement ( &wpNew );

	//SetWindowPos ( NULL, rectDesktop.left, rectDesktop.top,  rectDesktop.right, rectDesktop.bottom, SWP_NOREPOSITION );

	// 重新排列界面元素
	m_iLeft		= 0;
	m_iTop		= 22;
	//m_iTop		= 70;
	m_iRight	= rectDesktop.right-rectDesktop.left;
	m_iBottom	= rectDesktop.bottom-rectDesktop.top-150;
	m_tabCtrl.SetWindowPos ( NULL, m_iLeft, m_iTop, m_iRight, m_iBottom, SWP_NOREPOSITION );

	if ( m_pDlgClient )
		m_pDlgClient->SetWindowPos ( NULL, m_iLeft, m_iTop, m_iRight, m_iBottom, SWP_NOREPOSITION );

	if ( m_pDlgPrePrinting )
		m_pDlgPrePrinting->SetWindowPos ( NULL, m_iLeft, m_iTop, m_iRight, m_iBottom, SWP_NOREPOSITION );

	if ( m_pDlgOperator )
		m_pDlgOperator->SetWindowPos ( NULL, m_iLeft, m_iTop, m_iRight, m_iBottom, SWP_NOREPOSITION );

	GetDlgItem ( IDOK )->SetWindowPos ( NULL, (m_iRight - m_iLeft) / 2 + m_iLeft + 100, m_iBottom + 50, 0, 0, SWP_NOSIZE );

	ActivatePage( m_iCurrentItem )	;
}

BOOL CDlgControl::OnEraseBkgnd(CDC* pDC)
{
	return CDialog::OnEraseBkgnd(pDC);
//	return	0;
}

BOOL CDlgControl::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_iCurrentItem	= 0;

	m_tabCtrl.InsertItem ( 0, "     打 印 管 理     " )	;
	m_tabCtrl.InsertItem ( 1, "     试 打 管 理     " )	;
	m_tabCtrl.InsertItem ( 2, "     机 器 管 理     " )	;
	m_tabCtrl.InsertItem ( 3, "     墨 水 管 理     " )	;

//	m_tabCtrl.DeleteItem ( 1 )	;

	if ( NULL == m_pDlgOperator )
	{
		m_pDlgOperator	= new CDlgOperator ( this );
		m_pDlgOperator->SetScoreDlg ( (CDlgManager*)m_pParent );
		m_pDlgOperator->Create ( IDD_DLG_OPERATOR );
//		m_pDlgOperator->ShowInfo ( )			;
	}

	if ( NULL == m_pDlgPrePrinting )
	{
		m_pDlgPrePrinting	= new CDlgPrePrinting( this );
		m_pDlgPrePrinting->Create ( IDD_DLG_PREPRINTING );
	}

	if ( NULL == m_pDlgClient )
	{
		m_pDlgClient	= new CcontrolClientDlg( this );
		m_pDlgClient->Create ( IDD_CONTROLCLIENT_DIALOG );
	}

	//m_pDlgOperator->ShowWindow(SW_RESTORE)	;
	//m_pDlgOperator->ShowInfo ( )			;

	ActivatePage ( 0 );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgControl::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
//	int	iOldItem;

//	iOldItem		= m_iCurrentItem		;
	m_iCurrentItem	= m_tabCtrl.GetCurSel()	;

	ActivatePage( m_iCurrentItem )	;

	*pResult = 0;
}

void CDlgControl::ActivatePage(int nIndex)
{
	switch(nIndex)
	{
	case	0:
		ShowWindow(SW_SHOW);
//		m_pDlgPrePrinting->ShowWindow(SW_HIDE); 
//		m_pDlgClient->ShowWindow(SW_HIDE); 
//		m_pDlgOperator->ShowWindow(SW_RESTORE)	;
//		m_pDlgOperator->ShowInfo ( )			;
		if ( m_pDlgOperator )
		{
			m_pDlgOperator->ShowWindow(SW_SHOW);
			m_pDlgOperator->SetWindowPos ( NULL, m_iLeft, m_iTop, m_iRight, m_iBottom, SWP_NOREPOSITION );
		}
		break;

	case	1:
		ShowWindow(SW_SHOW);
		//m_pDlgOperator->ShowWindow(SW_HIDE);
		//m_pDlgClient->ShowWindow(SW_HIDE);
		if ( m_pDlgPrePrinting )
		{
			m_pDlgPrePrinting->ShowWindow(SW_SHOW); 
			m_pDlgPrePrinting->SetWindowPos ( NULL, m_iLeft, m_iTop, m_iRight, m_iBottom, SWP_NOREPOSITION );
		}
		break;

	case	2:
		ShowWindow(SW_SHOW);
		//m_pDlgOperator->ShowWindow(SW_HIDE);
		//m_pDlgPrePrinting->ShowWindow(SW_HIDE); 
		if ( m_pDlgClient )
		{
			m_pDlgClient->ShowWindow(SW_SHOW); 
			m_pDlgClient->SetWindowPos ( NULL, m_iLeft, m_iTop, m_iRight, m_iBottom, SWP_NOREPOSITION );
		}
		break;

	default:
		break;
	}
}

void CDlgControl::OnBnClickedOk()
{
	//OnOK();

	if ( m_pDlgOperator )
		m_pDlgOperator->ShowWindow(SW_HIDE);

	if ( m_pDlgPrePrinting )
		m_pDlgPrePrinting->ShowWindow(SW_HIDE);

	if ( m_pDlgClient )
		m_pDlgClient->ShowWindow(SW_HIDE); 

	ShowWindow(SW_HIDE);
}

void CDlgControl::OnBnClickedCancel()
{
	//// TODO: 在此添加控件通知处理程序代码
	//OnCancel();
	ShowWindow(SW_HIDE);
}

BOOL CDlgControl::PreTranslateMessage(MSG* pMsg)
{
	if ( WM_LBUTTONUP == pMsg->message )
	{
		if ( pMsg->hwnd == m_tabCtrl.GetSafeHwnd() )
		{
			CRect	rcTab;
			CPoint	ptPos ( LOWORD(pMsg->lParam), HIWORD(pMsg->lParam) );

			m_tabCtrl.GetWindowRect ( rcTab );
			ScreenToClient ( rcTab );
			rcTab.top	= 0;

			if ( rcTab.PtInRect(ptPos) )
			{
				ActivatePage( m_iCurrentItem )	;

				return	TRUE;
			}
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
