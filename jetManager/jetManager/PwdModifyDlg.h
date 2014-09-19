#pragma once


// CPwdModifyDlg 对话框

class CPwdModifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPwdModifyDlg)

public:
	CPwdModifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPwdModifyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_PWDMODIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strOldPwd;
	CString m_strNewPwd;
	CString m_strNewPwdConfirm;
	afx_msg void OnBnClickedOk();
	CString m_strAccount;
	CString m_strUserName;
};
