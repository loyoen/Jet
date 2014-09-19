// DlgOperator.cpp : 实现文件
//

#include "stdafx.h"
#include "../jetManager.h"
#include "DlgOperator.h"
#include "dlgManager.h"
#include ".\dlgoperator.h"


// CDlgOperator 对话框

IMPLEMENT_DYNAMIC(CDlgOperator, CDialog)
CDlgOperator::CDlgOperator(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOperator::IDD, pParent)
{
	m_pParent	= (CDlgManager*) pParent	;

	m_nCurrent_questionID	= -1;
	m_nCurrent_MachineID	= -1;
}

CDlgOperator::~CDlgOperator()
{
}

void CDlgOperator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgOperator, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_ALL_LOGOUT, OnBnClickedBtnAllLogout)
//	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlgOperator 消息处理程序

// 执行变为更新信息的处理
void CDlgOperator::OnBnClickedCancel()
{
	m_pParent->UpdateChannelInfo ( )	;

	GetDlgItem(IDCANCEL)->ShowWindow(FALSE);
//	OnCancel();
}

void CDlgOperator::On_admin_channel_state ( LPPACK_CHANNEL_STATE lpChannelState )
{
	m_lstTasksPage.Set_channel_state ( lpChannelState );

	Sleep ( 200 );
	GetDlgItem(IDCANCEL)->ShowWindow(TRUE);
}

void CDlgOperator::OnChannelState ( LPPACK_CHANNEL_STATE lpChannelState )
{
	LPCHANNELATTRIB		lpAttrib		;
	LPCHANNEL_STAT		lpState			;
	LPMACHINE_ATTRIB	lpMachineAttrib	;

	lpAttrib	= &lpChannelState->attribChannel;
	lpState		= &lpChannelState->statisChannel;

	m_lstTasksPage.OnUserGroupAction( 0, 
		lpAttrib->m_iTaskID,
		0, 
		lpState->m_iCntMachine, 
		lpState->m_iCntPrinted,
		lpState->m_iCntTotal,
		lpAttrib->m_strSubTask);

	for ( int i=0; i<lpState->m_iCntMachine; i++ )
	{
		lpMachineAttrib	= &lpChannelState->attribMachine[i];

		m_lstTasksPage.SetChannelInfo ( lpMachineAttrib )	;
	}

	Sleep ( 200 );
	GetDlgItem(IDCANCEL)->ShowWindow(TRUE);
}

void CDlgOperator::ShowInfo ( )
{
	m_lstTasksPage.ShowWindow(SW_SHOW);	
	//GetDlgItem(IDCANCEL)->ShowWindow(TRUE);
}


BOOL CDlgOperator::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstTasksPage.Create(IDD_PROPPAGE_TASKS,this);
	m_lstTasksPage.ShowWindow(SW_SHOW);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgOperator::OnBnClickedOk()
{
	ShowWindow(SW_HIDE);

	//m_lstTasksPage.ShowWindow(SW_HIDE);	

	//GetDlgItem(IDCANCEL)->ShowWindow(TRUE);
	//GetDlgItem(IDC_BTN_ALL_LOGOUT)->ShowWindow(TRUE);
	//OnOK();
}

void CDlgOperator::OnBnClickedBtnAllLogout()
{
	m_pParent->AllLogout ( )	;
	GetDlgItem(IDC_BTN_ALL_LOGOUT)->ShowWindow(FALSE);
}

UINT CDlgOperator::OnNcHitTest(CPoint point)
{
//	return	0;

	return CDialog::OnNcHitTest(point);
}
