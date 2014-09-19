#ifndef	_THREAD_SHEET_SERVER_H
#define _THREAD_SHEET_SERVER_H

#include "cclient.h"
#include "NetServer.h"
// CThreadTaskServer

#define ROLLBACK_COUNT					10
#define MAX_SUBMIT_COUNT				20
#define	MAX_MARKED_COUNT				30
#define ROLLBACK_FINISHING_COUNT		5
#define MAX_SUBMIT_FINISHING_COUNT		18
#define	MAX_MARKED_FINISHING_COUNT		21

// Messages definition
#define	WM_EXIT_THREAD		WM_USER + 500
#define	WM_SUBMIT_MARKS		WM_USER + 501
#define	WM_NEXT				WM_USER + 502
#define	WM_PREV				WM_USER + 503

class CMainFrame;
class CDlgManager;

class CThreadTaskServer : public CWinThread
{
	DECLARE_DYNCREATE(CThreadTaskServer)

//enum Attribute {
//   normal =    0x00,
//   readOnly =  0x01,
//   hidden =    0x02,
//   system =    0x04,
//   volume =    0x08,
//   directory = 0x10,
//   archive =   0x20
//   };

//================================================
//	from CClient
public:
	vector<tagClientData*>	DeviceClient;
	vector<tagClientData*>	JetClient;
	int						TaskId;
	Task *					mpTaskHead;

	void InitClient();
//	void SetMainDlg( CDialog* p ) { m_pMain=p; }
	void SendBindInfo(int i,int j);
	void ShowMessage(CString strMessage);
	void AddDeviceInfo(CString strMessage);
	void AddJetInfo(CString strMessage);
	void AddTaskInfo(CString strMessage,int Column);
	void SendPic(int TaskId);
	void LoadPic(CString FilePathName);
	void SendInstruction(int Device,int TaskId,int times);
	void SendPackToServer(LPPACKAGE pOnePackage);
	void RemoveDevice(int i);
	void RemoveJet(int i);
	void doClientData(LPPACKAGE pOnePackage);
	void doResendData(LPPACKAGE pOnePackage);
	void SendTypeToServer();
//================================================

// 调用接口
public:
	BOOL	Start ( );	// 启动服务
	BOOL	Stop ( );	// 停止服务

	void	ConnectServer ( )	;
	BOOL	ConnectServer ( CString strAddr, u_short sinport );
	void	Logon ( CString strAccount, CString strPassword );
	void	Logon ( )	;
	void	Logout ( )	;
	void	ReLogin ( )	;
	void	RequestTask ( )	;
//, begin
	void	Reset ( );	// 重置以便正式打印
//, ends

	void	Kick_out ( int iMachineID );
	void	Kick_out ( int iMachineID, int iIDJob );

	void	Task_recycle ( int iMachineID );
	void	Task_recycle ( int iMachineID, int iIDJob );

	void	OnReceiveFromServer( LPPACKAGE lpPackage );
	void	OnConnServerFailed( );

	void	SendPreGradToServer( int iMark, int iFormatSheetID );
	void	SendSelectedToServer( int iFormatSheetID );
	
	// 状态检查
	void	CheckServerState ( )	;
	void	CheckChannelState ( )	;
	void	AllLogout ( )			;
	void	ChannelLogout ( )		;

	// 参数设置
	void	SetAppPath( char* pPath );			// 传递应用程序的当前路径给TaskServer
	void	SetScoreDlg( CDlgManager* pDlg );
	void	SetServerAddr ( CString strAddr );	// 设置服务器地址
	void	SetServerPort ( UINT nPort );
	void	SetLogonInfo ( CString strAccount, CString strPassword );
	void	SetBufferPtrList( CPtrList* pListLarge, CPtrList* pListSmall, CPtrList* pListCommand );

// 工作参数
protected:
	CString	m_strAccount	;	// 用户帐号
	CString	m_strPassword	;	// 用户密码

	CString	m_strUserPath	;	// 当前打印机器存放图片的路径
	char*	m_lpPathApp		;	// 应用程序所在的路径
	
	// 为包列表加锁
	CRITICAL_SECTION	m_csTaskList;

// 缓冲数据
public:
	CPtrList*	m_lpPtrLargeMemList;	// 大内存缓冲列表
	CPtrList*	m_lpPtrSmallMemList;	// 小内存缓冲列表
	CPtrList*	m_lpPtrCommandMemList;	// 命令缓冲列表

	LPFORMAT_SHEET	m_lstTaskHead;	// 列表表头
	LPFORMAT_SHEET	m_lstTaskTail;	// 列表表尾

	//LPFORMAT_SHEET	m_lstCheckHead;	// 复查列表表头
	//LPFORMAT_SHEET	m_lstCheckTail;	// 复查列表表尾

	LPFORMAT_SHEET	m_pTaskCurrent;// 当前工作对象

// 工作状态
public:
	int				m_iCurrent	;	// 当前正在浏览的job序号
	int				m_iProcessing	;	// 当前正在作业的job序号
	int				m_iTotal	;	// 当前列表中job数目
	int				m_iMarkedCnt;	// 全部完成的作业数目

	BOOL			m_bLogout	;

	BYTE			m_yHead		;
	ChannelState	m_enumState	;


// 工作标志
protected:
	BOOL	m_bServerStarted;

// 内部动作
protected:
	void	RecvOneTask( LPPACK_JOB_FTP	lpTaskFTP );
	void	RecvCheckTask( LPPACK_JOBCHECK_FTP	lpTaskFTP );
	void	OnModifyPwdACK( LPUSER_MESSAGE lpUserMsg )	;
	LPCSTR	GetIpAddress( )	;
	int		GetOperator( )	;
	// protected before,public now 
public:
	void	ReturnMsg2MemList( LPPACKAGE lpPackage )	;
	void	OnNetReloginOK( )							;
	void	OnKickout(int iMachineID, int iIDJob)	;
	void	OnKickout ( int iMachineID )				;

	//05-2-1,by Xy,End
protected:
	void	OnLoginACK ( LPUSER_MESSAGE lpUserMsg )		;
	void	OnAdminLoginACK ( LPUSER_MESSAGE lpUserMsg );
	void	OnLogoutACK ( LPUSER_MESSAGE lpUserMsg )	;
	void	OnSupressLogout ( LPUSER_MESSAGE lpUserMsg );
	void	OnChannelDone ( LPUSER_MESSAGE lpUserMsg )	;

	void	OnChannelState ( LPPACK_CHANNEL_STATE lpChannelState )	;

	// Task service
	void	RequestTask (int iUncheckedCnt);					// 向服务器(或Operator)请求job
	void	RecycleTask (BYTE yRecycleCnt, int iRemain);		// 回收job给服务器
	void	SubmitAllTask2Server ( );							// 登出前递交所有job
	void	SubmitMarks2Server( int iSubmitCnt );				// 向服务器提交iSubmitCnt份作业
//	void	SubmitTask( LPCLIENT_JOB_PACKED pTaskPacked );
//	void	TaskSubmitPacked( int M );
//	void	UpdateServerState();
	void	OnServerTaskRecycle( LPUSER_MESSAGE lpUserMsg );
	void	OnServerOnFinishing( );

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	void	Submit_script_2_server ( LPFORMAT_SHEET lpTask );
	void	Recv_script ( LPPACK_SCRIPT_FTP lpScript_FTP );
#endif
//, ends


	//5-02-02,begin
//	BOOL	FindJobOnDisk(int iFormatSheetID);
//	void	OnServerCheckJob(LPUSER_MESSAGE lpUserMsg);
//	void	ClientCheckedResult(LPPACK_CHECKED_RESULT lpCheckedResult);
	//5-02-02,end
public:
	//void	UpdateMarkedCnt();
	void	Client_modify_userinfo( CString strOldPwd, CString strPassword, CString strAccount );

	void	GetNextTask();
	void	GetPrevTask();
	// Timer Set
	//void	SetTimeoutDetect(int nSpan, BYTE bWho);
	//void	DisableTimeoutDetect(BYTE bWho);
	//void	GetTaskRequestTimeoutSpan();
	//5-01-18,end

// 服务提供者
public:
	CNetServer*		m_pNet;

//5-01-18,begin
public:

	CDlgManager*		m_pDlg;

	int		m_iMachineID			;
	LPCHANNEL_INFO	m_pChannelInfo	;
	int		m_iOperator				;	// Channel的Operator
	int		m_iStepCount			;

protected:
	BOOL	m_bSubmitTaskLock		;	// job递交时锁定链表
	int		m_iCount				;	// 当前链表中已完成job数，当它达到30时递交前20
	int		m_iRollCount			;	// 用来回滚计数
	int		m_iPriority				;	// 当前未打印job总数

//	int		m_iLastRequestPendingCnt;	// 防止包泛滥
	BOOL	m_bConnected			;	// 网络连接标志

//	int		m_iVersion				;
// 临界处理
protected:
//	CCriticalSectionWrapper	m_csSubmit;

//5-01-18,end

protected:
	CThreadTaskServer();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadTaskServer();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	afx_msg void OnAppExitMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnScoreDlgHWnd(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnTimer(UINT nIDEvent);

	virtual int Run();
};

#endif

