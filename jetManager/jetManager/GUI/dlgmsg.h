#pragma once
#include "afxwin.h"
#include "Protocol.h"
#include "ColorButton.h"


// CDlgMsg 对话框

//	SKLSE 2007-01-17, jicheng, store scripts to server, begin
#ifdef	SWITCH_STORE_SCRIPT
class CjetManagerDoc;
#endif
//	SKLSE 2007-01-17, jicheng, store scripts to server, ends

class CDlgMsg : public CDialog
{
	DECLARE_DYNAMIC(CDlgMsg)

public:
	void	ShowMsg ( CString strMsg, int iControl );

public:
	CDlgMsg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMsg();

// 对话框数据
	enum { IDD = IDD_DLG_MSG };

//	SKLSE 2007-01-17, jicheng, store scripts to server, begin
#ifdef	SWITCH_STORE_SCRIPT
	CjetManagerDoc	*m_pDoc;

	// 是否显示笔迹的标志
	BOOL	m_bFlag1;
	BOOL	m_bFlag2;
	BOOL	m_bFlag3;

	void	Reset ( );
	void	Set_script_flag ( int iNumber );
	void	Set_doc ( CjetManagerDoc *pDoc );
#endif
//	SKLSE 2007-01-17, jicheng, store scripts to server, ends

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
//	SKLSE 2007-04-07, jicheng, color button, begin
//	CButton m_btnMessage1;
//	CButton m_btnMessage2;
//	CButton m_btnMessage3;

	CColorButton m_btnMessage1;
	CColorButton m_btnMessage2;
	CColorButton m_btnMessage3;

	afx_msg void OnBnClickedMsg1();
	afx_msg void OnBnClickedMsg2();
	afx_msg void OnBnClickedMsg3();
//	SKLSE 2007-04-07, jicheng, color button, ends
};
