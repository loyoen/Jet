#pragma once
#include "DialogResize.h"

#include "Resource.h"

// CJobManagerPage 对话框

class CJobManagerPage : public CDialogResize
{
	DECLARE_DYNAMIC(CJobManagerPage)

public:
	CJobManagerPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJobManagerPage();
	virtual BOOL OnInitDialog();

    void ResetAllList();
	void AddJobGroup(int nAction,int nJobGroupID,int nStatus,int nJobCnt,int nSubmitCnt);

// 对话框数据
	enum { IDD = IDD_PROPPAGE_JOBGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
	DECLARE_DLGRESIZE_MAP;
public:
    CListCtrl m_lstJobGroup;
	CListCtrl m_lstJob;
	CListCtrl m_lstSubJob;
	
	//Track the job selection
	int       m_nCurrentJobGroupID;
	int       m_nCurrentJobID;		

	afx_msg void OnNMClickListJobgroup(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListJob(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListCutJob(NMHDR *pNMHDR, LRESULT *pResult);
};
