#pragma once


// CSendNotifyDlg �Ի���

class CSendNotifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CSendNotifyDlg)

public:
	CSendNotifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSendNotifyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SEND_NOTIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNotifyMsg;

	afx_msg void OnBnClickedOk();
};
