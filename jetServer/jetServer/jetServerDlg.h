// jetServerDlg.h : ͷ�ļ�
//
#include "CServer.h"
#include "CThread.h"
#pragma once#include "afxwin.h"



// CjetServerDlg �Ի���
class CjetServerDlg : public CDialog
{
// ����
public:
	CjetServerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CServer *pServer;
// �Ի�������
	enum { IDD = IDD_JETSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	CThread* cpStopThread;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
