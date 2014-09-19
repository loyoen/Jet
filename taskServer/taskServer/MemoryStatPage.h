#pragma once


// CMemoryStatPage 对话框

#include "Resource.h"

class CMemoryStatPage : public CDialog
{
	DECLARE_DYNAMIC(CMemoryStatPage)

//	调用接口
public:
//	SKLSE 2005-08-18, jicheng, displaying memmory state, begin
//    void    UpdateMemoryStatistics( t_network_server_memory* lpMemState )	;
//	SKLSE 2005-08-18, jicheng, displaying memmory state, ends

public:
	CMemoryStatPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMemoryStatPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_MEMORY_USAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_MemoryUsageStat;
};
