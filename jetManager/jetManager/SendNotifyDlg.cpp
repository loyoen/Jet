// SendNotifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "jetManager.h"
#include "SendNotifyDlg.h"
#include ".\sendnotifydlg.h"


// CSendNotifyDlg 对话框

IMPLEMENT_DYNAMIC(CSendNotifyDlg, CDialog)
CSendNotifyDlg::CSendNotifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendNotifyDlg::IDD, pParent)
	, m_strNotifyMsg(_T(""))
{
}

CSendNotifyDlg::~CSendNotifyDlg()
{
}

void CSendNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strNotifyMsg);
	DDV_MaxChars(pDX, m_strNotifyMsg, 64);
}


BEGIN_MESSAGE_MAP(CSendNotifyDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSendNotifyDlg 消息处理程序

void CSendNotifyDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnOK();
}
