// SendNotifyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "jetManager.h"
#include "SendNotifyDlg.h"
#include ".\sendnotifydlg.h"


// CSendNotifyDlg �Ի���

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


// CSendNotifyDlg ��Ϣ�������

void CSendNotifyDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	OnOK();
}
