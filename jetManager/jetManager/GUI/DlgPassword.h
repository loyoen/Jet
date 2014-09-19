#pragma once
#include "afxwin.h"


// CDlgPassword 对话框

class CDlgPassword : public CDialog
{
	DECLARE_DYNAMIC(CDlgPassword)

public:
	void	Set_verify_psw ( CString strPsw );
	void	Set_message_string ( CString strMsg );
	void	Set_dlg_title ( CString strTitle );

protected:
	CString	m_strVerify_psw;
	CString	m_strDlg_title;
	CString	m_strMessage;

	BOOL	Check_psw ( );

public:
	CDlgPassword(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPassword();

// 对话框数据
	enum { IDD = IDD_DLG_PASSWORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	CEdit m_editMsg;
	CEdit m_editPassword;
};
