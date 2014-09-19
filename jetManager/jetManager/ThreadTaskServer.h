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

// ���ýӿ�
public:
	BOOL	Start ( );	// ��������
	BOOL	Stop ( );	// ֹͣ����

	void	ConnectServer ( )	;
	BOOL	ConnectServer ( CString strAddr, u_short sinport );
	void	Logon ( CString strAccount, CString strPassword );
	void	Logon ( )	;
	void	Logout ( )	;
	void	ReLogin ( )	;
	void	RequestTask ( )	;
//, begin
	void	Reset ( );	// �����Ա���ʽ��ӡ
//, ends

	void	Kick_out ( int iMachineID );
	void	Kick_out ( int iMachineID, int iIDJob );

	void	Task_recycle ( int iMachineID );
	void	Task_recycle ( int iMachineID, int iIDJob );

	void	OnReceiveFromServer( LPPACKAGE lpPackage );
	void	OnConnServerFailed( );

	void	SendPreGradToServer( int iMark, int iFormatSheetID );
	void	SendSelectedToServer( int iFormatSheetID );
	
	// ״̬���
	void	CheckServerState ( )	;
	void	CheckChannelState ( )	;
	void	AllLogout ( )			;
	void	ChannelLogout ( )		;

	// ��������
	void	SetAppPath( char* pPath );			// ����Ӧ�ó���ĵ�ǰ·����TaskServer
	void	SetScoreDlg( CDlgManager* pDlg );
	void	SetServerAddr ( CString strAddr );	// ���÷�������ַ
	void	SetServerPort ( UINT nPort );
	void	SetLogonInfo ( CString strAccount, CString strPassword );
	void	SetBufferPtrList( CPtrList* pListLarge, CPtrList* pListSmall, CPtrList* pListCommand );

// ��������
protected:
	CString	m_strAccount	;	// �û��ʺ�
	CString	m_strPassword	;	// �û�����

	CString	m_strUserPath	;	// ��ǰ��ӡ�������ͼƬ��·��
	char*	m_lpPathApp		;	// Ӧ�ó������ڵ�·��
	
	// Ϊ���б����
	CRITICAL_SECTION	m_csTaskList;

// ��������
public:
	CPtrList*	m_lpPtrLargeMemList;	// ���ڴ滺���б�
	CPtrList*	m_lpPtrSmallMemList;	// С�ڴ滺���б�
	CPtrList*	m_lpPtrCommandMemList;	// ������б�

	LPFORMAT_SHEET	m_lstTaskHead;	// �б��ͷ
	LPFORMAT_SHEET	m_lstTaskTail;	// �б��β

	//LPFORMAT_SHEET	m_lstCheckHead;	// �����б��ͷ
	//LPFORMAT_SHEET	m_lstCheckTail;	// �����б��β

	LPFORMAT_SHEET	m_pTaskCurrent;// ��ǰ��������

// ����״̬
public:
	int				m_iCurrent	;	// ��ǰ���������job���
	int				m_iProcessing	;	// ��ǰ������ҵ��job���
	int				m_iTotal	;	// ��ǰ�б���job��Ŀ
	int				m_iMarkedCnt;	// ȫ����ɵ���ҵ��Ŀ

	BOOL			m_bLogout	;

	BYTE			m_yHead		;
	ChannelState	m_enumState	;


// ������־
protected:
	BOOL	m_bServerStarted;

// �ڲ�����
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
	void	RequestTask (int iUncheckedCnt);					// �������(��Operator)����job
	void	RecycleTask (BYTE yRecycleCnt, int iRemain);		// ����job��������
	void	SubmitAllTask2Server ( );							// �ǳ�ǰ�ݽ�����job
	void	SubmitMarks2Server( int iSubmitCnt );				// ��������ύiSubmitCnt����ҵ
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

// �����ṩ��
public:
	CNetServer*		m_pNet;

//5-01-18,begin
public:

	CDlgManager*		m_pDlg;

	int		m_iMachineID			;
	LPCHANNEL_INFO	m_pChannelInfo	;
	int		m_iOperator				;	// Channel��Operator
	int		m_iStepCount			;

protected:
	BOOL	m_bSubmitTaskLock		;	// job�ݽ�ʱ��������
	int		m_iCount				;	// ��ǰ�����������job���������ﵽ30ʱ�ݽ�ǰ20
	int		m_iRollCount			;	// �����ع�����
	int		m_iPriority				;	// ��ǰδ��ӡjob����

//	int		m_iLastRequestPendingCnt;	// ��ֹ������
	BOOL	m_bConnected			;	// �������ӱ�־

//	int		m_iVersion				;
// �ٽ紦��
protected:
//	CCriticalSectionWrapper	m_csSubmit;

//5-01-18,end

protected:
	CThreadTaskServer();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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

