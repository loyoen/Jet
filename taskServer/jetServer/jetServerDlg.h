// jetServerDlg.h : 头文件
//
#include "CServer.h"
#include "CThread.h"
#pragma once#include "afxwin.h"



// CjetServerDlg 对话框
class CjetServerDlg : public CDialog
{
// 构造
public:
	CjetServerDlg(CWnd* pParent = NULL);	// 标准构造函数
	CServer *pServer;
// 对话框数据
	enum { IDD = IDD_JETSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CThread* cpStopThread;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
			LRESULT OutString(WPARAM w, LPARAM a);
			LRESULT EndFunc(WPARAM w, LPARAM a);
public:
	CEdit m_showmsg;

	//afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
public:
	afx_msg void OnBnClickedButton4();
public:
	afx_msg void OnBnClickedButton5();
public:
	afx_msg void OnBnClickedButton6();
public:
	afx_msg void OnBnClickedButton7();
public:
	afx_msg void OnBnClickedButton8();
public:
	afx_msg void OnBnClickedButton9();
};
