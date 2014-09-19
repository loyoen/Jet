
// controlClientDlg.h : header file
//

#pragma once


#include "afxcmn.h"
#include "resource.h"
#include "CClient.h"
// CcontrolClientDlg dialog
class CcontrolClientDlg : public CDialogEx
{
// Construction
public:
	CcontrolClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CONTROLCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support



// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	// 初始化Socket库以及界面信息
	void Init();

	// 初始化List控件
	void InitListCtrl();

public:
	inline void AddInformation(const CString strInfo)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_OUTPUT);
		pList->InsertItem(pList->GetItemCount(),strInfo);
	}
	inline void AddDeviceEnd(const CString strInfo)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_DEVICE);
		pList->InsertItem(pList->GetItemCount(),strInfo);
	}
	inline void AddJetEnd(const CString strInfo)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_JET);
		pList->InsertItem(pList->GetItemCount(),strInfo);
	}
	inline void AddTask(const CString strInfo, int Column)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_TASK);
		int line = pList->GetItemCount();
		pList->InsertItem(line,strInfo);
		pList->SetItemText(line,1,strInfo);
	}
	inline void RemoveDevice(int i)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_DEVICE);
		pList->DeleteItem(i);
	}
	inline void RemoveJet(int i)
	{
		CListCtrl* pList = (CListCtrl*)GetDlgItem(IDC_LIST_JET);
		pList->DeleteItem(i);
	}
private:
	CClient m_Client;
	CString FilePathName;

public:
	afx_msg void OnBnClickedButton1();
	CListCtrl m_List_Device;
	CListCtrl m_List_Jet;
	afx_msg void OnBnClickedButton2();
	void DrawPic(CString filename);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CListCtrl m_List_Task;
	afx_msg void OnBnClickedButton5();
};
