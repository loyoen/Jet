#pragma once


class CDlgOperator;
class CDlgPrePrinting;
class CcontrolClientDlg;
// CDlgControl 对话框
class CDlgControl : public CDialog
{
	DECLARE_DYNAMIC(CDlgControl)

public:
	void	FullScreen ( );

	CDlgOperator *		m_pDlgOperator	;
	CDlgPrePrinting *	m_pDlgPrePrinting	;
	CcontrolClientDlg *	m_pDlgClient	;

protected:
	int		m_iCurrentItem	;	// 当前的活动page

	int		m_iLeft, m_iTop, m_iRight, m_iBottom;

	CWnd	* m_pParent;

protected:
	void	ActivatePage ( int nIndex );

public:
	CDlgControl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgControl();

// 对话框数据
	enum { IDD = IDD_DLG_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl	m_tabCtrl;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
