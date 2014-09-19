/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[Module]
	dlgManager

[Description]
	A User interaction dialog implementing for management of jetServer through
network connection.

[Author]
	Jicheng Hu (jicheng.whu @ gmail.com)

[Date]
	2014_07_15

[Revision History]
Date		Who			Where	Description
----------------------------------------------------------------------
2014_07_15	jicheng		WuHan	created!
----------------------------------------------------------------------
2014-07-15	jicheng		Wuhan	first released
----------------------------------------------------------------------
------------------------------------------------------------------------------*/

#ifndef	_DLG_MANAGER_H_
#define _DLG_MANAGER_H_

#define MARGIN		24
#define TOPLINE		222
#define E_TOPLINE	220
#define ROW_SPACING	12
#define HEIGHT		12

#include "../Resource.h"
#include "afxcmn.h"

#include "../MainFrm.h"
//#include "DlgOperator.h"

#include "DlgControl.h"
#include "DlgMsg.h"
#include "controlClientDlg.h"


class CPictureView;
class CDlgManager : public CFormView 
{
	// 控制快捷键的标志
	BOOL	m_bEnableNext	;
	BOOL	m_bEnablePrev	;
	BOOL	m_bEnableModify	;

//	响应CTaskServer状态的接口
public:
	BOOL	IsCurrentMarked( );
	void	OnLoginFailure( CString	strInfo );
	void	OnLoginOK();
	void	UpdatePicView(int iFormatSheetID);

	void	OnChannelState ( LPPACK_CHANNEL_STATE lpChannelState );

	void	OnTwiceLogin(int iMachineID, int iIDJob, CString str);

	void	OnModifyPwd(CString str);
    void	OnLogoutFailed(CString str);
	void	OnLogoutOK();
	void	OnChannelDone();
	void	SetStateLogon(){SetState( Control_Logon );};

	void	SetCurrentTask( LPFORMAT_SHEET pTask );
	void	SetUserPath( CString strPath );
	void	SetMarkedCnt( int iCnt );
	void	SetChannelState( ChannelState enumState ){ m_channelState = enumState; };
	void	SetHead( BYTE yHead ){ m_bHead = (1==yHead); m_bAdmin = (2==yHead);};
	void	SetStateOnProcessing( );

	// 设置焦点到第一个
	void	FocusFirst ( );

	void	DisableLogout();

	void	UpdateChannelInfo ( )	;
	void	AllLogout ( )	;

public:
	LPFORMAT_SHEET	m_pTaskCurrent;	// 当前task

	CString			m_strUserPath	;	// 当前存放task图片的路径

//	状态参数
public:
	BOOL	m_bModify	;
	BOOL	m_bHead		;	// 组长标志
	BOOL	m_bAdmin	;	// 管理员标志

public:
	CThreadTaskServer*	m_pTaskServer;

	CDlgOperator *	m_pDlgOperator	;
	CDlgPrePrinting *	m_pDlgPrePrinting	;

	//CcontrolClientDlg *	m_pDlgControl;
	CDlgControl *	m_pDlgControl;

	CDlgMsg *		m_pDlgMsg	;

protected:
	CDlgManager();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDlgManager)

public:
	enum ControlState
	{ 
		Control_Logon			= -11,
		Control_Logon_Wait		= -12,
		Control_Regist			= -13,
		Control_Regist_Wait		= -14,
		Control_Download		= -15,
		Control_Check			= -16,
		Control_Upload			= -17,
		Control_Regist_Done		= -18,
		Control_Head_Registed	= -19,
		Control_Processing		= -20,
		Control_Reprocessing	= -21, 
		Control_Marked			= -22,
		Control_Selecting		= -23,
		Control_All_Done		= -24,
		Control_Channel_Done	= -25
	}m_ctrlState;

	int		m_iMarkedCnt	;
	int		m_iCurrentNum	;

protected:
	// UI functions
	void	ShowLoginInputs ( int nCmdShow=SW_SHOW );
	void	SetButtons ( );

public:
	//Initialize and Deinitialize
	virtual ~CDlgManager();
	virtual void OnInitialUpdate();

	// task管理
    void InsertNewJob(int nJobID);

	//界面的更新控制
	void UpdateGUI(int nJobID,float fTotalScore,float fOldScore,float fNowScore);
	void UpdateUI();	
	void SetState(ControlState ctrlState);
//	void ResumeDialog();
//	BOOL IsDialogHide();

	//For Debug Version only
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
	//{{AFX_DATA(CDlgManager)
	enum { IDD = IDD_FORMVIEW };
	CBitmapButton	m_HideDialogButton;
	CBitmapButton	m_btnBadJob;
	CString	m_strPassword;
	CString	m_strUserName;
	CString	m_strServerAddress;
	long	m_lPort;
	//}}AFX_DATA
	CString	m_strNickname;
    BOOL	m_bDialogHide;

	CStringList	*	m_lstStrServerAddress;

protected:
	WINDOWPLACEMENT	m_wpPictureViewOriginal	;
	WINDOWPLACEMENT	m_wpPictureViewFull		;
	CString	        m_strCurPictPath		;	
	ChannelState	m_channelState			;
	
    //事件和消息响应函数
protected:
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgManager)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CDlgManager)
	afx_msg void OnHidedlgOrNot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedButtonRequest();
	afx_msg void OnKeyUp();
	afx_msg void OnKeyDown();
	afx_msg void OnUpdateCommandLogout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandModifypwd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandNotify(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandSubmittask(CCmdUI *pCmdUI);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnUpdateBadJob(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRequesttask(CCmdUI *pCmdUI);

	afx_msg void OnUpdateOperator(CCmdUI *pCmdUI);
	afx_msg void OnOperator();
	afx_msg void OnControlClient();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnUpdateUserKickout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateUserRecycle(CCmdUI *pCmdUI);
	afx_msg void OnUserKickout();
};

#endif	// ifndef _DLG_MANAGER_H_
