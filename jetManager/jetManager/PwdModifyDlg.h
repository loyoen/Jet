#pragma once


// CPwdModifyDlg �Ի���

class CPwdModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPwdModifyDlg)

public:
	CPwdModifyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPwdModifyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_PWDMODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldPwd;
	CString m_strNewPwd;
	CString m_strNewPwdConfirm;
	afx_msg void OnBnClickedOk();
	CString m_strAccount;
	CString m_strUserName;
};
