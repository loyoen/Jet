#pragma once

#include "Protocol.h"


// CPageTasks �Ի���
class CDlgOperator	;
class CPageTasks : public CDialog
{
	DECLARE_DYNAMIC(CPageTasks)

public:
	CPageTasks(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPageTasks();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_TASKS };
    
public:
	void ResetAllList();
	void OnUserGroupAction(int nAction,int nTaskNum,float fScore,
		int nCntOfMembers,int nSubmitTasksInTotal,int nTotalCnt, char strTitle[13]);
	//void OnGetUserInGroupAck(int nSheetID,int nCnt,t_usergroup_get_member_ack_item *p);

	void	SetChannelInfo( LPMACHINE_ATTRIB lpMachineAttrib );

	void	Set_channel_state ( LPPACK_CHANNEL_STATE lpChannelState );

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNMClickListGroup(NMHDR *pNMHDR, LRESULT *pResult);

	CPtrList	m_ptrState_list;

	void	Update_user_group ( LPPACK_CHANNEL_STATE lpChannelState );

public:
	CListCtrl m_lstTasks;
	CListCtrl m_lstJobMachine;

	CDlgOperator *	m_pParentDlg;

	int		m_nCurrentUserGroupID	;
	int		m_nCurrentMachineID		;

	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	afx_msg void OnNMRclickListGroupMember(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMRclickListUserGroup(NMHDR *pNMHDR, LRESULT *pResult);
};
