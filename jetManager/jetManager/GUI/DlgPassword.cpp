// DlgPassword.cpp : 实现文件
//

#include "stdafx.h"
#include "../jetManager.h"
#include "DlgPassword.h"
#include ".\dlgpassword.h"


// CDlgPassword 对话框

IMPLEMENT_DYNAMIC(CDlgPassword, CDialog)
CDlgPassword::CDlgPassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPassword::IDD, pParent)
{
	m_strDlg_title	= "请输入密码：";
	m_strVerify_psw	= "84817422";
}

CDlgPassword::~CDlgPassword()
{
}

void CDlgPassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_editMsg);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
}


BEGIN_MESSAGE_MAP(CDlgPassword, CDialog)
END_MESSAGE_MAP()


// CDlgPassword 消息处理程序

void CDlgPassword::OnOK()
{
	if ( !Check_psw() )
		return;

	CDialog::OnOK();
}

BOOL CDlgPassword::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_editPassword.SetWindowText( _T("") );
	m_editPassword.SetFocus();
	SetWindowText ( m_strDlg_title );
	m_editMsg.SetWindowText ( m_strMessage );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgPassword::Check_psw ( )
{
	CString	strPassword;

	m_editPassword.GetWindowText ( strPassword );

	if ( 0 == strPassword.Compare ( m_strVerify_psw ) )
	{
		return	TRUE;
	}

	m_editMsg.SetWindowText ( "密码不对! 请重新输入密码:" );
	return	FALSE;
}

void CDlgPassword::Set_dlg_title ( CString strTitle )
{
	m_strDlg_title	= strTitle;
}

void CDlgPassword::Set_message_string ( CString strMsg )
{
	m_strMessage	= strMsg;
}

void CDlgPassword::Set_verify_psw ( CString strPsw )
{
	m_strVerify_psw	= strPsw;
}