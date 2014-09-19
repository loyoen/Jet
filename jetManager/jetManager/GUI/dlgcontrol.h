#pragma once


class CDlgOperator;
class CDlgPrePrinting;
class CcontrolClientDlg;
// CDlgControl �Ի���
class CDlgControl : public CDialog
{
	DECLARE_DYNAMIC(CDlgControl)

public:
	void	FullScreen ( );

	CDlgOperator *		m_pDlgOperator	;
	CDlgPrePrinting *	m_pDlgPrePrinting	;
	CcontrolClientDlg *	m_pDlgClient	;

protected:
	int		m_iCurrentItem	;	// ��ǰ�Ļpage

	int		m_iLeft, m_iTop, m_iRight, m_iBottom;

	CWnd	* m_pParent;

protected:
	void	ActivatePage ( int nIndex );

public:
	CDlgControl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgControl();

// �Ի�������
	enum { IDD = IDD_DLG_CONTROL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
