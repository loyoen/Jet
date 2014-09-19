#pragma once
#include "afxcmn.h"

#include "Resource.h"

// CMachinePage �Ի���

class CMachinePage : public CDialog
{
	DECLARE_DYNAMIC(CMachinePage)

public:
	CMachinePage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMachinePage();
	BOOL AddMachine(
		int         nMachineID,
		const char* strAccount,
		const char* strName,
		//��ӡ��Ϣ
		int         nSubmitToday,
		int         nSubmitTotal,
		//��ǰ��ӡ��ҵ
		unsigned char nCurrentQuestionNum
		);

	BOOL  RemoveMachine(
		int         nMachineID
		);
// �Ի�������
	enum { IDD = IDD_PROPPAGE_MACHINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	CListCtrl m_Machines;
};
