#pragma once


// CMemoryStatPage �Ի���

#include "Resource.h"

class CMemoryStatPage : public CDialog
{
	DECLARE_DYNAMIC(CMemoryStatPage)

//	���ýӿ�
public:
//	SKLSE 2005-08-18, jicheng, displaying memmory state, begin
//    void    UpdateMemoryStatistics( t_network_server_memory* lpMemState )	;
//	SKLSE 2005-08-18, jicheng, displaying memmory state, ends

public:
	CMemoryStatPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMemoryStatPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_MEMORY_USAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_MemoryUsageStat;
};
