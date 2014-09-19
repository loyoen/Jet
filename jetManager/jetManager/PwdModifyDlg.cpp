// PwdModifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "jetManager.h"
#include "PwdModifyDlg.h"
#include ".\pwdmodifydlg.h"


// CPwdModifyDlg �Ի���

IMPLEMENT_DYNAMIC(CPwdModifyDlg, CDialog)
CPwdModifyDlg::CPwdModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPwdModifyDlg::IDD, pParent)
	, m_strOldPwd(_T(""))
	, m_strNewPwd(_T(""))
	, m_strNewPwdConfirm(_T(""))
	, m_strAccount(_T(""))
	, m_strUserName(_T(""))
{
}

CPwdModifyDlg::~CPwdModifyDlg()
{
}

void CPwdModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strOldPwd);
	DDV_MaxChars(pDX, m_strOldPwd, 20);
	DDX_Text(pDX, IDC_EDIT2, m_strNewPwd);
	DDV_MaxChars(pDX, m_strNewPwd, 20);
	DDX_Text(pDX, IDC_EDIT3, m_strNewPwdConfirm);
	DDV_MaxChars(pDX, m_strNewPwdConfirm, 20);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, m_strAccount);
	DDV_MaxChars(pDX, m_strAccount, 20);
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_strUserName);
	DDV_MaxChars(pDX, m_strUserName, 20);
}


BEGIN_MESSAGE_MAP(CPwdModifyDlg, CDialog)

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CPwdModifyDlg ��Ϣ�������




void CPwdModifyDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(m_strNewPwd!=m_strNewPwdConfirm)
	{
		AfxMessageBox("�������������");
		return ;
	}
	OnOK();
}
