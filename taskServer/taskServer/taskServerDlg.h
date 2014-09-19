// taskServerDlg.h : header file
//

#pragma once

#include "global.h"

#ifdef	__JICHENG__
#include "DialogResize.h"

#include "InfobarCtrl.h"
#include "TracePage.h"
#include "MachinePage.h"
#include "ConfigurationPage.h"
#include "MemoryStatPage.h"
#include "MachineGroupPage.h"
#include "JobManagerPage.h"
#endif

class CServer;
class CThread;

// CtaskServerDlg dialog
#ifdef	__JICHENG__
class CtaskServerDlg : public CDialogResize
#else
class CtaskServerDlg : public CDialog
#endif
{
// Construction
public:
	CtaskServerDlg(CWnd* pParent = NULL);	// standard constructor

	CServer	*m_pServer;

	CThread* cpWaitClientThread;
	CThread* cpCheckClientThread;
	CThread* cpDataThread;
	CThread* cpStopThread;

// states
public:
	BOOL	m_bServerStarted;
	BOOL	m_bStart_button_pressed;	// in case the button pressed twice

// Dialog Data
#ifdef	__JICHENG__
	enum { IDD = IDD_TASKSERVER_DLG };
#else
	enum { IDD = IDD_TASKSERVER_DIALOG };
#endif

#ifdef	__JICHENG__
//	CInfobarCtrl m_InfobarCtrl;
	CListCtrl	 m_OutlookBar;
	CStatusBar   m_wndStatusBar;
	CToolBar     m_wndToolBar;
	CImageList   m_ImageList;

	CTracePage			m_TracePage;
	CMachinePage		m_MachinesPage;
	CConfigurationPage	m_ConfigurationPage;
	CMemoryStatPage		m_MemoryUsagePage;
	CMachineGroupPage		m_MachineGroupPage;
	CJobManagerPage	m_JobManagerPage;
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	CEdit m_showmsg;

	LRESULT OutString(WPARAM w, LPARAM a);
	LRESULT EndFunc(WPARAM w, LPARAM a);

// Implementation
protected:
	HICON	m_hIcon;
    BOOL	m_bInitialized;
	BOOL	m_bCloseWindowAndExit;

	void	SetOnlineLed(BOOL bOnline);
	void	SetOfflineLed(BOOL bOffline);
	void	MoveChilds();

	void	SetupOutlookBar();
	void	ActivatePage(int nIndex);
	BOOL	CreateStatusbar();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	// 对话框的自身窗口消息响应
	LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);

	// OutlookBar 各个属性页的消息响应
	afx_msg void OnClickOutlookBar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownOutlookBar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewTrace();
	afx_msg void OnViewMachines();
	afx_msg void OnViewConfiguration();
	afx_msg void OnViewMemoryStat();
	afx_msg void OnViewMachineGroup();
	afx_msg void OnViewJobManager();

 	afx_msg void OnUpdateViewJobManager(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMachineGroup(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewTrace(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMachines(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewConfiguration(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMemoryStat(CCmdUI *pCmdUI);

	//Server Start & Stop
	afx_msg void OnServerStart();
	afx_msg void OnServerPreStart();
	afx_msg void OnServerStop();
	afx_msg void OnUpdateServerStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServerPreStart(CCmdUI* pCmdUI);
	afx_msg void OnUpdateServerStop(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
	DECLARE_DLGRESIZE_MAP;

public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();

	afx_msg void OnStopAll();
	afx_msg void OnUpdateStopAll(CCmdUI *pCmdUI);

protected:
	virtual void OnOK();
	virtual void OnCancel();

	// GUI message response
	afx_msg LRESULT	OnGUI_LOG_msg ( WPARAM wParam, LPARAM lParam )		;
};
