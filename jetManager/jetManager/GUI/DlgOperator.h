#pragma once

#include "Protocol.h"
#include "../pageTasks.h"

// CDlgOperator 对话框
class CDlgManager	;

class CDlgOperator : public CDialog
{
	DECLARE_DYNAMIC(CDlgOperator)

public:
	void	On_admin_channel_state ( LPPACK_CHANNEL_STATE lpChannelState );
	void	OnChannelState ( LPPACK_CHANNEL_STATE lpChannelState );
	void	ShowInfo ( );

	void	SetScoreDlg ( CDlgManager *pDlg ){m_pParent=(CDlgManager*)pDlg;};

	CDlgOperator(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOperator();

	CDlgManager *	m_pParent	;

    CPageTasks     m_lstTasksPage;

	// 来自CPageTasks的变量
	int		m_nCurrent_questionID	;
	int		m_nCurrent_MachineID	;
	void	Set_current_questionID ( int iID ){m_nCurrent_questionID=iID;};
	void	Set_current_teacerID ( int iID ){m_nCurrent_MachineID=iID;};

// 对话框数据
	enum { IDD = IDD_DLG_OPERATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnAllLogout();
	afx_msg UINT OnNcHitTest(CPoint point);
};
