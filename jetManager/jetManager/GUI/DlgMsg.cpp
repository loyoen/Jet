// DlgMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "../jetManager.h"
#include "DlgMsg.h"

#include "../jetManagerDoc.h"
#include ".\dlgmsg.h"


// CDlgMsg 对话框

IMPLEMENT_DYNAMIC(CDlgMsg, CDialog)
CDlgMsg::CDlgMsg(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMsg::IDD, pParent)
{
}

CDlgMsg::~CDlgMsg()
{
}

void CDlgMsg::ShowMsg ( CString strMsg, int iControl )
{
	GetDlgItem ( IDC_MSG1 + iControl )->SetWindowText ( strMsg );
}

void CDlgMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG1, m_btnMessage1);
	DDX_Control(pDX, IDC_MSG2, m_btnMessage2);
	DDX_Control(pDX, IDC_MSG3, m_btnMessage3);
}


BEGIN_MESSAGE_MAP(CDlgMsg, CDialog)
#ifdef	SWITCH_STORE_SCRIPT
	ON_BN_CLICKED(IDC_MSG1, OnBnClickedMsg1)
	ON_BN_CLICKED(IDC_MSG2, OnBnClickedMsg2)
	ON_BN_CLICKED(IDC_MSG3, OnBnClickedMsg3)
#endif
END_MESSAGE_MAP()


// CDlgMsg 消息处理程序
//, begin
#ifdef	SWITCH_STORE_SCRIPT
void CDlgMsg::Set_doc ( CjetManagerDoc *pDoc )
{
	m_pDoc	= pDoc;
}

void CDlgMsg::Set_script_flag ( int iNumber )
{
	switch ( iNumber )
	{
	case	1:
		m_btnMessage1.Set_text_color ( RGB( 0, 175, 0 ) );
		m_btnMessage1.Set_highlight ( );
		m_bFlag1	= TRUE;
		break;

	case	2:
		m_btnMessage2.Set_text_color ( RGB( 0, 0, 175 ) );
		m_btnMessage2.Set_highlight ( );
		m_bFlag2	= TRUE;
		break;

	case	3:
		m_btnMessage3.Set_text_color ( RGB( 175, 175, 0 ) );
		m_btnMessage3.Set_highlight ( );
		m_bFlag3	= TRUE;
		break;

	default:
		break;
	}
}

void CDlgMsg::Reset ( )
{
	m_bFlag1	= FALSE;
	m_bFlag2	= FALSE;
	m_bFlag3	= FALSE;
	
	m_btnMessage1.Reset ( );
	m_btnMessage2.Reset ( );
	m_btnMessage3.Reset ( );

	//m_btnMessage1.Set_text_color ( RGB( 0, 0, 0 ) );
	//m_btnMessage2.Set_text_color ( RGB( 0, 0, 0 ) );
	//m_btnMessage3.Set_text_color ( RGB( 0, 0, 0 ) );
}

void CDlgMsg::OnBnClickedMsg1()
{
	if ( !m_bFlag1 )
		return;

	m_btnMessage1.m_bChecked	= !m_btnMessage1.m_bChecked;
	m_pDoc->m_bFlag1	= !m_pDoc->m_bFlag1;
	m_pDoc->UpdateAllViews ( NULL, 1 );
	RedrawWindow ( );
}

void CDlgMsg::OnBnClickedMsg2()
{
	if ( !m_bFlag2 )
		return;

	m_btnMessage2.m_bChecked	= !m_btnMessage2.m_bChecked;
	m_pDoc->m_bFlag2	= !m_pDoc->m_bFlag2;
	m_pDoc->UpdateAllViews ( NULL, 1 );
	RedrawWindow ( );
}

void CDlgMsg::OnBnClickedMsg3()
{
	if ( !m_bFlag3 )
		return;

	m_btnMessage3.m_bChecked	= !m_btnMessage3.m_bChecked;
	m_pDoc->m_bFlag3	= !m_pDoc->m_bFlag3;
	m_pDoc->UpdateAllViews ( NULL, 1 );
	RedrawWindow ( );
}
#endif
//, ends
