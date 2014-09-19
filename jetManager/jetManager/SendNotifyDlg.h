#pragma once


// CSendNotifyDlg 对话框

class CSendNotifyDlg : public CDialog
{
	DECLARE_DYNAMIC(CSendNotifyDlg)

public:
	CSendNotifyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSendNotifyDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SEND_NOTIFY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNotifyMsg;

	afx_msg void OnBnClickedOk();
};
