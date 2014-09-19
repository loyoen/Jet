#pragma once
#include "afxcmn.h"

#include "Resource.h"

// CMachinePage 对话框

class CMachinePage : public CDialog
{
	DECLARE_DYNAMIC(CMachinePage)

public:
	CMachinePage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMachinePage();
	BOOL AddMachine(
		int         nMachineID,
		const char* strAccount,
		const char* strName,
		//打印信息
		int         nSubmitToday,
		int         nSubmitTotal,
		//当前打印作业
		unsigned char nCurrentQuestionNum
		);

	BOOL  RemoveMachine(
		int         nMachineID
		);
// 对话框数据
	enum { IDD = IDD_PROPPAGE_MACHINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_Machines;
};
