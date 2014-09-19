#pragma once


// CDlgQuality 对话框
#include "Protocol.h"
class CDlgManager	;

//class CQuality;
#include "../Quality.h"
#include "ColorButton.h"

class CDlgQuality : public CDialog
{
	DECLARE_DYNAMIC(CDlgQuality)

public:
	CDlgManager	*m_pParent;
	CQuality	*m_pQuality;

	int		m_iShift;

	void	Show_quality ( );
	void	Update_item_interface ( DWORD dwIndex, COLORREF cr, CString strTitle );

public:
	CDlgQuality(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgQuality();

// 对话框数据
	enum { IDD = IDD_DLG_QUALITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CEdit m_editMsg1;
	CEdit m_editMsg2;

	CColorButton m_btnMachine1;
	CColorButton m_btnMachine2;
	CColorButton m_btnMachine3;
	CColorButton m_btnMachine4;
	CColorButton m_btnMachine5;
	CColorButton m_btnMachine6;
	CColorButton m_btnMachine7;
	CColorButton m_btnMachine8;
	CColorButton m_btnMachine9;
	CColorButton m_btnMachine10;
	CColorButton m_btnMachine11;
	CColorButton m_btnMachine12;
	CColorButton m_btnMachine13;
	CColorButton m_btnMachine14;
	CColorButton m_btnMachine15;
	CColorButton m_btnMachine16;

	afx_msg void OnBnClickedMachine1();
	afx_msg void OnBnClickedMachine2();
	afx_msg void OnBnClickedMachine3();
	afx_msg void OnBnClickedMachine4();
	afx_msg void OnBnClickedMachine5();
	afx_msg void OnBnClickedMachine6();
	afx_msg void OnBnClickedMachine7();
	afx_msg void OnBnClickedMachine8();
	afx_msg void OnBnClickedMachine9();
	afx_msg void OnBnClickedMachine10();
	afx_msg void OnBnClickedMachine11();
	afx_msg void OnBnClickedMachine12();
	afx_msg void OnBnClickedMachine13();
	afx_msg void OnBnClickedMachine14();
	afx_msg void OnBnClickedMachine15();
	afx_msg void OnBnClickedMachine16();

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnLeft();
	afx_msg void OnBnClickedBtnRight();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg UINT OnNcHitTest(CPoint point);
};
