#pragma once

//#include "PackProtocol.h"

#include "Resource.h"

// CMachineGroupPage 对话框
class CtaskServerDlg;

class CMachineGroupPage : public CDialog
{
	DECLARE_DYNAMIC(CMachineGroupPage)

public:
	CMachineGroupPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMachineGroupPage();
    
	void ResetAllList();
	void OnMachineGroupAction(int nAction,int nQuestionNum,float fScore,
		int nCntOfMembers,int nSubmitPapersInTotal,int nTotalCnt, char strTitle[13]);

//	void	SetChannelInfo( LPMACHINE_STATE lpState );

// 对话框数据
	enum { IDD = IDD_PROPPAGE_MACHINEGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMClickListGroup(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_MachineGroup;
	CListCtrl m_lstGroupMember;

	CtaskServerDlg*	m_pParentDlg;

	int		m_nCurrentMachineGroupID	;
	int		m_nCurrentMachineID		;

	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	afx_msg void OnNMRclickListGroupMember(NMHDR *pNMHDR, LRESULT *pResult);
};
