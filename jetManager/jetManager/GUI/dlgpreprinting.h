#pragma once
#include "Protocol.h"
#include "afxwin.h"


// CDlgPrePrinting 对话框
class CDlgManager	;

class CDlgPrePrinting : public CDialog
{
	DECLARE_DYNAMIC(CDlgPrePrinting)

public:
	void	OnNewMark ( )	;
	void	ShowInfo ( )	;

	void	OnServerPreGradedMarkEx ( LPPACK_PREGRADED_MARK_EX lpPreGradedMarkEx );

	CDlgPrePrinting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPrePrinting();

	CDlgManager *	m_pParent	;

	int		m_iShift	;
	float	m_fAverage	;	// 平均成绩

	int		m_iPreMarkKind;

	int		m_iReserved;

	LPPACK_PREGRADED_MARK_EX	m_lpPrePrintingEx;

// 对话框数据
	enum { IDD = IDD_DLG_PREPRINTING };

protected:
	float	CalculateAverage ( );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CString m_strEdit1;
	CString m_strEdit2;
	CString m_strEdit3;
	CString m_strEdit4;
	CString m_strEdit5;
	CString m_strEdit6;
	CString m_strEdit7;
	CString m_strEdit8;
	CString m_strEdit9;
	CString m_strEdit10;
	CEdit m_editMsg1;
	CEdit m_editMsg2;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnLeft();
	afx_msg void OnBnClickedBtnRight();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedNormal();
};
