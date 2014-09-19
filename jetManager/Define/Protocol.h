
#ifndef __INC_PACKAGE_PROTOCOL__
#define __INC_PACKAGE_PROTOCOL__

#include "Winsock2.h"


#define	SWITCH_AUTO_TESTING		// 自动测试开关

//#define	SWITCH_CLIENT_RESTRICT	// 限制客户端某些功能

#define	SWITCH_STORE_SCRIPT		// 保存画笔信息
#define	SWITCH_QUALITY_MANAGEMENT		// 质量控制


#define	VERSION_CLIENT	715		// 版本(当服务器或客户端只要有一个执行变时更执行之)
#define	TITLE_SERVER	"彩色轮胎打印系统服务器"
#define	TITLE_CLIENT	"彩色轮胎打印系统管理终端"

#ifdef	SWITCH_QUALITY_MANAGEMENT
#define	QUALITY_BUFFER_INCREASING_BYTES		1024
#define	QUALITY_BUFFER_SEND_SEGMENT_BYTES	20
#endif

// 客户端版图结构
typedef	struct _FORMAT_SHEET
{
	int			iFormatSheetID	;
	BOOL		bSerialized		;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	BOOL		bHasScript		;

	int			iMachine1ID		;
	int			iMachine2ID		;
	int			iMachine3ID		;
#endif
//, ends

	char		strName1 [11]	;
	char		strName2 [11]	;
	char		strName3 [11]	;

	_FORMAT_SHEET*	pNextTask		;
	_FORMAT_SHEET*	pPrevTask		;
}FORMAT_SHEET, *LPFORMAT_SHEET;

#define		MAX_CHANNELS_IN_SERVER	64		// 服务器上最多容许的设备组数
#define		MAX_JOBS_IN_CHANNEL		256		// 单task最多容许的job数

// 缓冲的定义
/*
#define SIZE_LARGE_MEM				(300*1024)
#define	SIZE_SMALL_MEM				8192
#define	SIZE_COMMAND_MEM			1024*/

//======= 语文 ==========================================
#define		SIZE_EXLARGE_MEM		(700*1024)
#define		SIZE_LARGE_MEM			(450*1024)
#define		SIZE_NORMAL_MEM			(340*1024)
#define		SIZE_SMALL_MEM			8192
#define		SIZE_COMMAND_MEM		1024

// 482,100 k
#define	CNT_SERVER_EXLARGE_MEM		90		// 28,000 k
#define	CNT_SERVER_LARGE_MEM		810		//229,500 k	
#define	CNT_SERVER_NORMAL_MEM		830		//214,200 k
#define	CNT_SERVER_SMALL_MEM		1200	//  9,600 k
#define	CNT_SERVER_COMMAND_MEM		800		//    800 k
//---------------------------------------------------------

//======= 其它 ==========================================
//#define		SIZE_EXLARGE_MEM		(400*1024)
//#define		SIZE_LARGE_MEM			(80*1024)
//#define		SIZE_NORMAL_MEM			(60*1024)
//#define		SIZE_SMALL_MEM			(20*1024)
//#define		SIZE_COMMAND_MEM		1024
//
//// 205,800 k
//#define	CNT_SERVER_EXLARGE_MEM		40		// 16,000 k
//#define	CNT_SERVER_LARGE_MEM		500		// 40,000 k	
//#define	CNT_SERVER_NORMAL_MEM		2200	//132,000 k
//#define	CNT_SERVER_SMALL_MEM		800		// 16,000 k
//#define	CNT_SERVER_COMMAND_MEM		1800	//  1,800 k
//---------------------------------------------------------

// 22,220 k
#define	CNT_NET_EXLARGE_MEM			1		//    800 k
#define	CNT_NET_LARGE_MEM			1		//    100 k
#define	CNT_NET_NORMAL_MEM			5		//    200 k
#define	CNT_NET_SMALL_MEM			80		//    640 k
#define	CNT_NET_COMMAND_MEM			20480	// 20,480 k

#define	CNT_NET_PERHANDLE_DATA		18192	// * 16	=  32 k
#define	CNT_NET_PERIO_DATA			16384	// * 56	= 448 k

#define	CNT_CLIENT_LARGE_MEM		1000
#define	CNT_CLIENT_SMALL_MEM		0
#define	CNT_CLIENT_COMMAND_MEM		200

//
//自定义枚举数据类型，用来标识套接字IO动作类型
//
typedef enum _NET_OPERATION 
{
	NetAccept,	//AcceptEx/accept
	NetRead,		//WSARecv/recv/ReadFile
	NetWrite,	//WSASend/send/WriteFile
	NetEnd,
	NetRecycleIoContextOnly	//仅释放IO_CONTEXT,发送Notify消息时用到
}NET_OPERATION, *LPNET_OPERATION;

//#define		BUFFER_SIZE				4096
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	单句柄数据(完成键)
class	CMachine;
typedef	struct	_PER_HANDLE_DATA		// size = 16
{
	SOCKET						sIO;
	CMachine*					pMachine;
	BOOL						bOnline;

	_PER_HANDLE_DATA	*		pNext;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;
//------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	单IO数据，扩展的WSAOVERLAPPED
typedef	struct	_PER_IO_DATA			// size	= 56
{
	WSAOVERLAPPED			ol;
//	char					szBuffer[BUFFER_SIZE]; 
	WSABUF					wsaBuffer;
	SOCKET					sClient;
	LPVOID					pBuffer;
	DWORD					dwBytesDone;
	DWORD					dwBytesBuffer;

	NET_OPERATION			enumOperation;

	LPPER_HANDLE_DATA		lpPerHandle;

	_PER_IO_DATA	*		pNext;
}PER_IO_DATA, *LPPER_IO_DATA;
//------------------------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 数据缓冲
typedef enum _BUFFER_TYPE 
{
	SizeExLarge,
	SizeLarge,
	SizeNormal,
	SizeSmall,
	SizeCommand
}BUFFER_TYPE, *LPBUFFER_TYPE;

typedef	struct	_EXLARGE_BUFFER
{
	BUFFER_TYPE			sizeType;
	SOCKET				socket;
	LPPER_HANDLE_DATA	lpPerHandle;
	_EXLARGE_BUFFER*	pBufferNext;
	char				pBuffer[SIZE_EXLARGE_MEM];
}EXLARGE_BUFFER, *LPEXLARGE_BUFFER;

typedef	struct	_LARGE_BUFFER
{
	BUFFER_TYPE			sizeType;
	SOCKET				socket;
	LPPER_HANDLE_DATA	lpPerHandle;
	_LARGE_BUFFER*		pBufferNext;
	char				pBuffer[SIZE_LARGE_MEM];
}LARGE_BUFFER, *LPLARGE_BUFFER;

typedef	struct	_NORMAL_BUFFER
{
	BUFFER_TYPE			sizeType;
	SOCKET				socket;
	LPPER_HANDLE_DATA	lpPerHandle;
	_NORMAL_BUFFER*		pBufferNext;
	char				pBuffer[SIZE_NORMAL_MEM];
}NORMAL_BUFFER, *LPNORMAL_BUFFER;

typedef	struct	_SMALL_BUFFER
{
	BUFFER_TYPE			sizeType;
	SOCKET				socket;
	LPPER_HANDLE_DATA	lpPerHandle;
	_SMALL_BUFFER*		pBufferNext;
	char				pBuffer[SIZE_SMALL_MEM];
}SMALL_BUFFER, *LPSMALL_BUFFER;

typedef	struct	_COMMAND_BUFFER
{
	BUFFER_TYPE			sizeType;
	SOCKET				socket;
	LPPER_HANDLE_DATA	lpPerHandle;
	_COMMAND_BUFFER*	pBufferNext;
	char				pBuffer[SIZE_COMMAND_MEM];
}COMMAND_BUFFER, *LPCOMMAND_BUFFER;

typedef	struct	_DATA_BUFFER
{
	BUFFER_TYPE		sizeType;
	SOCKET			socket;
	LPPER_HANDLE_DATA	lpPerHandle;
	_DATA_BUFFER*	pBufferNext;
	char*			pBuffer;
}DATA_BUFFER, *LPDATA_BUFFER;
//------------------------------------------------------

// 下面这个结构体定义了用户数据包的类型
// 用户消息
typedef	struct	_USER_MESSAGE
{
	BYTE	yUserType;					// 用户信息头，标明这是个什么包，定义如下
	LPVOID	pBuffer;					// 缓存，存放具体数据内容
}USER_MESSAGE, *LPUSER_MESSAGE;
// 信息头是下面的情形之一
// User login
#define		MSG_CLIENT_LOGIN						0x00
#define		MSG_SERVER_LOGIN_SUCCESS				0x02
#define		MSG_SERVER_LOGIN_FAILURE				0x03
// User Logout
#define		MSG_CLIENT_LOGOUT						0x04
#define		MSG_SERVER_LOGOUT_SUCCESS				0x05
#define		MSG_SERVER_LOGOUT_FAILURE				0x06

// User ReLogin
#define		MSG_CLIENT_RELOGIN						0x11
#define		MSG_SERVER_RELOGIN_SUCCESS				0x14

// Task Submit
#define		MSG_CLIENT_JOB_SUBMIT					0x07
#define		MSG_SERVER_JOB_SUBMIT_ACK				0x08
// Task Hand out
#define		MSG_CLIENT_JOB_REQUEST				0x09
#define		MSG_CLIENT_JOB_TRANSFER_ACK			0x0A
//
#define		MSG_SERVER_JOB_REQUEST_SUCCESS		0x0B
#define		MSG_SERVER_JOB_REQUEST_FAILURE		0x0C
#define		MSG_SERVER_JOB_FTP					0x0D
// Notifications
#define		MSG_SERVER_NOTIFICATION					0x0E
// Password Modification
#define		MSG_CLIENT_PASSWORD_MODIFICATION		0x0F
#define		MSG_SERVER_PASSWORD_MODIFICATION_ACK	0x10

#define		MSG_CLIENT_CONFIRM_OPERATOR				0x12
#define		MSG_CLIENT_INIT_OPERATOR_ACK			0x13

//#define		MSG_SERVER_CHECK_CUTSHEET				0x15
#define		MSG_CLIENT_SUBMIT_CUTSHEETNO			0x16
#define		MSG_CLIENT_KICKOUT						0x17

#define		MSG_SERVER_RESUBMIT						0x18
#define		MSG_SERVER_SUPRESSLOGOUT				0x19

#define		MSG_CLIENT_HEAD_JOB_REQUEST				0x1A
#define		MSG_CLIENT_SELECTED_CUTSHEET			0x1B
#define		MSG_CLIENT_PREGRADED_MARK				0x1C

#define		MSG_SERVER_JOB_RECYCLE					0x1D
#define		MSG_CLIENT_JOB_RECYCLE_ACK				0x1E

#define		MSG_SERVER_ONFINISHING					0x1F

#define		MSG_SERVER_CHANNEL_DONE					0x20
#define		MSG_CLIENT_CHANNEL_DONE					0x21

#define		MSG_CLIENT_CUTSHEET_IDLIST				0x22

#define		MSG_OPERATOR_CHECK_CHANNEL				0x23
#define		MSG_ADMIN_CHECK_SERVER					0x24

#define		MSG_SERVER_CHANNEL_STATE				0x25

#define		MSG_SERVER_ADMIN_LOGIN_SUCCESS			0x26

#define		MSG_ADMIN_ALL_LOGOUT					0x27

#define		MSG_SERVER_PREGRADE_MARK				0x28

#define		MSG_SERVER_JOBCHECK_FTP					0x29

#define		MSG_SERVER_COMMAND						0x30
#define		MSG_CLIENT_COMMAND						0x31

#define		MSG_OPERATOR_JOB_RECYCLE				0x32

#ifdef	SWITCH_STORE_SCRIPT
#define		MSG_CLIENT_SCRIPT_SUBMIT				0x33
#define		MSG_SERVER_SCRIPT_FTP					0x34
#endif

#ifdef	SWITCH_QUALITY_MANAGEMENT
#define		MSG_SERVER_QUALITY_MANAGEMENT_SEGMENT	0x35	// 发送质量控制块
#define		MSG_SERVER_QUALITY_MANAGEMENT			0x36	// 发送一机器全部质量控制
#endif

// 下面这个结构体定义了网络数据包的类型
typedef	struct	_NET_COMMAND
{
	LPVOID	lpBuffer				;	// 缓存，存放具体数据内容
}NET_COMMAND, *LPNET_COMMAND;

//enum ChannelState
//{
//	channelDisactive,
//	channelPreGrad,
//	channelActive,
//	channelFinishing,
//	channelChecking,
//	channelFinished
//};
typedef	BYTE	ChannelState;

#define		channelDisactive	1
#define		channelPreGrad		2
#define		channelActive		4
#define		channelFinishing	8
#define		channelChecking		16
#define		channelFinished		32

// 包头的定义

// 包头yTypePackage是下面的情形之一
#define		USER_COMMAND	0	// 可以一次传完的用户数据, 可以直接交给用户处理
#define		NET_SCATTER		1	// 被分割的小包, 需要gather后送给用户处理
#define		NET_SERVERIN	2	// 有一新的服务器请求进入系统作为容错服务器
#define		NET_SERVEROUT	3	// 容错服务器请求退出
#define		NET_RECONSTRUCT	4	// 容错服务器请求重建
#define		NET_RELOGIN_SUCCESS		5	// 重新登陆成功
#define		NET_CHECK		254	// 客户端检查网络状态包(当Server收到Check包时立刻将包返回)
#define		NET_PONG		255	// 心跳包, 用户PONG服务器
#define		NET_PING		255	// 心跳包, 服务器PING用户

// 下面这个结构体综合定义了本系统所有包的类型

typedef	struct	_COMMAND_HEADER
{
	WORD	wCmdSize;
}COMMAND_HEADER, *LPCOMMAND_HEADER;

typedef struct _CHANNEL_INFO
{
	int				iIDChannel		;	// task的ID
	int				iIDJob			;	// jobID
	int				iReserved		;	// 
}CHANNEL_INFO, *LPCHANNEL_INFO;

typedef	struct	_CHANNEL_STATE
{
	int		iTaskNum		;
	int		iReservedMark	;
	int		iCntMachine		;
	int		iCntPrinted		;
	int		iCntTotal		;
	char	strSubTask[13]	;
}CHANNEL_STATE, *LPCHANNEL_STATE;

typedef struct _CHANNEL_ATTRIB
{
	int			m_iChannelID		;
	int			m_iTaskID			;
	int			m_iReserved			;	// 保留字,  存放是否加扣分栏等标志
//	BYTE		m_yScoreFormID[16]	;	// 在 TaskFormInfo 中的 ID
	int			m_iScoreFormID[16]	;	// 在 TaskFormInfo 中的 ID
	BYTE		m_yTaskType			;
	char		m_strChannelName[25];
	char		m_strTaskTitle[11]	;
	char		m_strSubTask[13]	;
}   CHANNELATTRIB, *LPCHANNELATTRIB	;

typedef	struct	_MACHINE_ATTRIB
{
	int					m_iMachineID	;	// 该机器ID

	WORD				m_enumStatus	;	// 工作状态
	BYTE				m_yHead			;	// 组长标记

	char				m_strAccount[21];	// 帐号
	char				m_strPsw[21]	;	// 密码
	char				m_strName[11]	;	// 编号
	
	int					m_iPriority		;	// 优先级

	// 统计数据
	int					m_iCntPrinted	;	// 已完成job数
	int					m_iAcceptCnt	;	// 有效打印数
	int					m_iSentCnt		;	// 已经发送未收回的job数
	int					m_iSentExCnt	;	// 已经发送未收回的job数
	int					m_iUpperCnt		;	// 作业过高数
	int					m_iLowerCnt		;	// 作业过低数
}MACHINE_ATTRIB, *LPMACHINE_ATTRIB	;

typedef	struct	_CHANNEL_STATISTIC
{
	int			m_iFreshCnt				;	// 完全未打印的份数	Flag = 0
	int			m_iCntPrinted			;	// 已经打印完的份数	Flag = 1
	int			m_iCntTotal				;	// 总作业数
	int			m_iCntMachine			;	// 本 Channel 中的机器数
	int			m_iRecycleMachineCnt	;	// 当前正在回收job的机器总数
}	CHANNEL_STAT, *LPCHANNEL_STAT	;

//typedef	enum	_Machine_STATUS	//此处定义了在channel中的 m_pMachineArray;
//{
#define	Machine_OffLine			1		// 离线
#define	Machine_DropLine		1		// 掉线
#define	Machine_OnLine			2		// 在线
#define	Machine_Finishing		4		// 在此状态下不再对此机器回收job
#define	Machine_Kickout			8		// 强制下线1轮
#define	Machine_Kickout2nd		16		// 强制下线2轮
#define	Machine_Recycling		32		// 正在回收
//#define	Machine_Recycled	64	// 已经完成回收
#define	Machine_Dummy			64		// 拦路元素, 暂时不参与channel优先级的更新
#define	Machine_Logout			128		// 用户登出
#define	Machine_ReLogin			256		// 用户在重登录
#define	Machine_HeadNeedTask	512
//#define	Machine_TransInfo			512		// 用户在传输数据
//}Machine_STATUS, *LPMachine_STATUS;

typedef	struct	_MACHINE_STATE
{
	int			m_iMachineID			;	// 该机器ID
	char		m_strMachineName[11]	;	// 编号
//	BYTE		m_enumStatus			;	// 工作状态
	WORD		m_enumStatus			;	// 工作状态
	int			m_iPriority				;	// 优先级
	int			m_iCntPrinted			;	// 已完成job数
	int			m_iAcceptCnt			;	// 有效打印数
	int			m_iSentCnt				;	// 已经发送未收回的job数
	int			m_iSentExCnt			;	// 已经发送未收回的job数
}MACHINE_STATE, *LPMACHINE_STATE;

typedef struct _PACK_LOGIN
{
	BYTE			yUserType		;
	char			strAccount[21]	;	// 用户ID
	char			strPassword[21]	;	// 登陆密码
	int				iVersion		;	// 版本
	CString			userip			;	// 本机IP，若是Operator，服务器将把它发送给每个登陆的用户
}PACK_LOGIN, *LPPACK_LOGIN;

// 服务器发送给Client的确认登陆的消息
typedef struct _PACK_LOGIN_SUCCESS
{
	BYTE			yUserType		;
	int				iMachineID		;
	int				iOperator		;	// 本Channel Operator的标识（跟MachineID一样）
	char			strNickName[11]	;	// Machine's name
	int				iPriority		;	// 以前为包的版本, 现在保留
	int				iMarkedCnt		;
	ChannelState	enumState		;	// 试执行状态或其它状态, 其值的含义在下面定义
	BYTE			yHead			;
	CHANNEL_INFO	lpChannel		;	// Channel信息 
}PACK_LOGIN_SUCCESS, *LPPACK_LOGIN_SUCCESS;
//
//#define		SERVER_STATE_PREGRADING		1
//#define		SERVER_STATE_GRADING		2

typedef struct _PACK_LOGIN_FAILURE
{
	BYTE			yUserType		;
	int				iMachineID		;
	int				iIDJob		;
	char			strFailInfo[64]	;
}PACK_LOGIN_FAILURE, *LPPACK_LOGIN_FAILURE;

// 用户发送的重新登陆的消息
typedef struct _PACK_RELOGIN
{
    BYTE			yUserType		;
    int				iMachineID		;
	int				iOperator		;	// 自己的Operator，在Server重启后连接时需要用到
//	CString			userip			;
}PACK_RELOGIN, *LPPACK_RELOGIN;

// 重登陆成功
typedef	struct _PACK_RELOGIN_SUCCESS
{
	BYTE			yUserType		;
	BOOL			bSuccess		;
	BOOL			bServerRestarted;
}PACK_RELOGIN_SUCCESS, *LPPACK_RELOGIN_SUCCESS;

// 重新设置Operator
//
// 服务器设置一个新的Operator,通知它
typedef struct _PACK_SELECT_OPERATOR
{
	BYTE			yUserType		;
	int				iMachineID		;
}PACK_SELECT_OPERATOR, *LPPACK_SELECT_OPERATOR;

// Client接受服务器的Operator授权
typedef struct _PACK_CONFIRM_OPERATOR
{
	BYTE			yUserType		;
	BOOL			bAccept			;
}PACK_CONFIRM_OPERATOR, *LPPACK_CONFIRM_OPERATOR;

// 初始化新的Operator
typedef struct _PACK_INIT_OPERATOR
{
	BYTE			yUserType		;
	CPtrList		ptrMachineList	;
}PACK_INIT_OPERATOR, *LPPACK_INIT_OPERATOR;

typedef struct _PACK_INIT_OPERATOR_ACK
{
	BYTE			yUserType		;
	BOOL			bSuccess		;
}PACK_INIT_OPERATOR_ACK, *LPPACK_INIT_OPERATOR_ACK;

// 通知该Channnel所有用户
typedef struct _PACK_NEW_OPERATOR
{
	BYTE			yUserType		;
	int				iNewOperator	;
}PACK_NEW_OPERATOR, *LPPACK_NEW_OPERATOR;
//,end

// 用户登出
typedef struct _PACK_LOGOUT
{
	BYTE			yUserType		;
	int				iMachineID		;
}PACK_LOGOUT, *LPPACK_LOGOUT;

typedef struct _PACK_LOGOUT_SUCCESS
{
	BYTE			yUserType		;
	int				iMachineID		;
	BYTE			yNewOperator	;	// 新的Operator，主要针对Operator登出时
}PACK_LOGOUT_SUCCESS, *LPPACK_LOGOUT_SUCCESS;

typedef	struct _PACK_LOGOUT_FAILURE
{
	BYTE			yUserType		;
	char			strFailInfo[64]	;
}PACK_LOGOUT_FAILURE, *LPPACK_LOGOUT_FAILURE;

// 递交的task的结构
typedef struct _JOB_SUBMITTED
{
	int				nJobID		;
	BYTE			bBadJob	;	// 不合规范标志
//, begin
#ifdef	SWITCH_STORE_SCRIPT
	BYTE			bHas_script		;	// 是否有笔迹
#endif
//, ends
}JOB_SUBMITTED, *LPJOB_SUBMITTED;

// task submit
typedef struct _PACK_JOB_SUBMIT
{
	BYTE				yUserType		;
	int					iPriority		;	// Client还未执行的job数
	int					iSubmitCnt		;	// 所递交的job数
	JOB_SUBMITTED		pItem[1]		;	// 递交的tasks的信息
}PACK_JOB_SUBMIT, *LPPACK_JOB_SUBMIT;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
typedef	struct _PACK_SCRIPT_SUBMIT
{
	BYTE			yUserType	;
	int				iFormatSheetID	;
	int				iLength		;
	BYTE			pScript[1]	;
}PACK_SCRIPT_SUBMIT, *LPPACK_SCRIPT_SUBMIT;
#endif
//, ends

typedef struct _PACK_JOB_SUBMIT_ACK
{
	BYTE			yUserType		;
	BOOL			bSuccess		;
	int				nAcceptCnt		;	// 用来反馈给用户已经接收的数目
}PACK_JOB_SUBMIT_ACK, *LPPACK_JOB_SUBMIT_ACK;

// task request
typedef struct _PACK_JOB_REQUEST
{
	BYTE			yUserType		;
	int				iUncheckedCnt	;	// Client还未执行的job数
}PACK_JOB_REQUEST, *LPPACK_JOB_REQUEST;

// 组长申请job试执行
typedef struct _PACK_HEAD_JOB_REQUEST
{
	BYTE			yUserType		;
	int				iUncheckedCnt	;	// Client还未执行的job数
}PACK_HEAD_JOB_REQUEST, *LPPACK_HEAD_JOB_REQUEST;

// 客户端向服务器和Operator传输执行完的job
typedef struct _PACK_JOB_TRANSFER_ACK
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	int				iPos			;	// 从文件的什么地方开始的数据块！
    int				iLen			;   // 希望接受多大的数据块
	BOOL			bFTPOver		;   // 结束FTP
}PACK_JOB_TRANSFER_ACK, *LPPACK_JOB_TRANSFER_ACK;

// 服务器对客户端请求job的响应
//,begin
typedef struct _PACK_JOB_REQUEST_ACK
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	unsigned int	dwTaskImageSize;	// 图片大小
	int         	bLastFile		;	// 作为分包发送时标识是否是最后的一个文件
	char			strFailInfo[64]	;
}PACK_JOB_REQUEST_ACK, *LPPACK_JOB_REQUEST_ACK;
//,end

// 服务器向客户请求调度其job
typedef	struct	_PACK_JOB_RECYCLE
{
	BYTE			yUserType		;
//	BYTE			yAskCnt			;	// 希望回收的份数
	BYTE			yAbnormalCnt	;	// 未正常回收的份数 -- 最多64
	int				iPriority		;	// 所在 Channel 当前的 m_iPriority
	int				iFormatSheetID[64]	;	// 未正常回收的列表
}PACK_JOB_RECYCLE, *LPPACK_JOB_RECYCLE;

// 客户返回的可调度job列表
typedef	struct	_PACK_JOB_RECYCLE_ACK
{
	BYTE			yUserType		;
	BYTE			yRecycleCnt		;	// 回收的数量 -- 最多64
	int				iFormatSheetID[64]	;	// 调度列表
	int				iUncheckedCnt	;	// Client还未执行的job数
}PACK_JOB_RECYCLE_ACK, *LPPACK_JOB_RECYCLE_ACK;

typedef struct _PACK_JOB_FTP
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	DWORD			dwLen			;  //	下面的包有多长
	char			data[1]			;
}PACK_JOB_FTP, *LPPACK_JOB_FTP;

typedef struct _PACK_JOBCHECK_FTP
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	char			strName1[11]	;	//	第一个机器的编号
	char			strName2[11]	;	//	第二个机器的编号
	char			strName3[11]	;	//	第三个机器的编号
//, begin
#ifdef	SWITCH_STORE_SCRIPT
	int				iMachine1ID		;
	int				iMachine2ID		;
	int				iMachine3ID		;
#endif
//, ends
	BYTE			yRound			;	// 
	BYTE			yScore1			;
	BYTE			yScore2			;
	BYTE			yScore3			;
	DWORD			dwLen			;	//	下面的包有多长
	char			data[1]			;
}PACK_JOBCHECK_FTP, *LPPACK_JOBCHECK_FTP;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
typedef struct _PACK_SCRIPT_FTP
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	int				iMachineID		;

	DWORD			dwLen			;	//	下面的包有多长
	char			data[1]			;
}PACK_SCRIPT_FTP, *LPPACK_SCRIPT_FTP;
#endif
//, ends

//, begin
#ifdef	SWITCH_QUALITY_MANAGEMENT
typedef struct _PACK_QUALITY_SEGMENT
{
	BYTE			yUserType		;
	int				iMachineID		;
	char			strName[11]		;	//	机器的编号

	DWORD			dwLen			;	//	下面的包有多长
	BYTE			data[1]			;
}PACK_QUALITY_SEGMENT, *LPPACK_QUALITY_SEGMENT;

typedef struct _PACK_QUALITY
{
	BYTE			yUserType		;
	int				iMachineID		;
	char			strName[11]		;	//	机器的编号

	DWORD			dwLen			;	//	下面的包有多长
	BYTE			data[1]			;
}PACK_QUALITY, *LPPACK_QUALITY;
#endif
//, ends

typedef	struct	_PACK_CHANNEL_STATE
{
	BYTE			yUserType		;
	CHANNELATTRIB	attribChannel	;
	CHANNEL_STAT	statisChannel	;
	MACHINE_ATTRIB	attribMachine[1];
}PACK_CHANNEL_STATE, *LPPACK_CHANNEL_STATE	;

typedef struct _PACK_NOTIFICATION
{
	BYTE			yUserType		;
	int				iMachineID		;
	char			strNotyInfo[255]	;
}PACK_NOTIFICATION, *LPPACK_NOTIFICATION;

typedef struct _PACK_PASSWORD_MODIFICATION
{
	BYTE			yUserType		;
	char			strAccount[21]	;	// 用户ID
	char			strPswdOld[21]	;	// 旧密码
	char			strPswddNew[21]	;	// 新密码	
}PACK_PASSWORD_MODIFICATION, *LPPACK_PASSWORD_MODIFICATION;

typedef struct _PACK_PASSWORD_MODIFICATION_ACK
{
	BYTE			yUserType		;
	BOOL            bSuccess		; 
	char			strModInfo[255]	;
}PACK_PASSWORD_MODIFICATION_ACK, *LPPACK_PASSWORD_MODIFICATION_ACK;
//,ends

typedef	struct	_PACK_SELECTED_CUTSHEET
{
	BYTE	yUserType		;
	int		iFormatSheetID		;
}PACK_SELECTED_CUTSHEET, *LPPACK_SELECTED_CUTSHEET;

typedef	struct	_PACK_CUTSHEET_IDLIST
{
	BYTE	yUserType		;
	WORD	wJobCnt	;
	int		iFormatSheetID[1]	;
}PACK_CUTSHEET_IDLIST, *LPPACK_CUTSHEET_IDLIST;

typedef	struct	_PACK_PREGRADED_MARK
{
	BYTE	yUserType		;
	int		iMark			;
	int		iPreGradeID		;
}PACK_PREGRADED_MARK, *LPPACK_PREGRADED_MARK;

//, begin
typedef	struct	_PACK_PREGRADED_MARK_EX
{
	BYTE	yUserType		;
	//char	m_strName[11]	;	// 作业机器编号
	//int		iMark			;	// 作业
	int		iSize			;
	int		iPreMarkDone	;	// 该组已经作业的机器个数
	int		iPreMarkKind	;	// 该组当前作业的种类数目
	//BYTE	yMarkCnt[1]		;	// 第一种作业的机器数
	//BYTE	yPreMark[1]		;	// 第一种作业的分值
	char	cInfo[1]		;	//  信息体
}PACK_PREGRADED_MARK_EX, *LPPACK_PREGRADED_MARK_EX;
//, ends

typedef	struct	_PACK_CHANNEL_ONFINISHING
{
	BYTE	yUserType		;
}PACK_CHANNEL_ONFINISHING, *LPPACK_CHANNEL_ONFINISHING;

typedef	struct	_CLIENT_CUTSHEET
{
	int		iFormatSheetID		;
	BOOL    bBadJob	;
	BYTE	yStepScore[16]	;
}CLIENT_CUTSHEET, *LPCLIENT_CUTSHEET;

typedef struct _CLIENT_JOB_PACKED
{
	int				iTaskPackedCnt		;
	CLIENT_CUTSHEET ClientCutsheet[1]	;
}CLIENT_JOB_PACKED, *LPCLIENT_JOB_PACKED;

//5-02-02,begin
// 重登陆job处理
// 服务器发给客户端的包结构
typedef struct _PACK_UNCHECKED_ON_DISK
{
	BYTE	yUserType		;
	int		iMachineID		;
	int		iJobCnt	;		// 客户端未交job数
	int		iFormatSheetID[1]	;		// 客户端未交jobID
}PACK_UNCHECKED_ON_DISK, *LPPACK_UNCHECKED_ON_DISK;

// 客户端发给服务器的包的结构
typedef struct _PACK_CHECKED_RESULT
{
	BYTE 	yUserType		;
	int		iMachineID		;
	int		iJobCnt	;
	int		iFormatSheetID[1]	;
}PACK_CHECKED_RESULT, *LPPACK_CHECKED_RESULT;

// 二次登陆处理
typedef struct _PACK_KICKOUT
{
	BYTE	yUserType		;
	int		iMachineID		;
	int		iIDJob		;
}PACK_KICKOUT, *LPPACK_KICKOUT;

typedef struct _PACK_OPERATOR_RECYCLE
{
	BYTE	yUserType		;
	int		iMachineID		;
	int		iIDJob		;
}PACK_OPERATOR_RECYCLE, *LPPACK_OPERATOR_RECYCLE;

typedef	struct	_PACK_SUPRESS_LOGOUT
{
	BYTE	yUserType		;
}PACK_SUPRESS_LOGOUT, *LPPACK_SUPRESS_LOGOUT;

typedef	struct	_PACK_ALL_LOGOUT
{
	BYTE	yUserType		;
}PACK_ALL_LOGOUT, *LPPACK_ALL_LOGOUT;

typedef	struct	_PACK_CHECK_SERVER
{
	BYTE	yUserType		;
}PACK_CHECK_SERVER, *LPPACK_CHECK_SERVER	;

typedef	struct	_PACK_CHECK_CHANNEL
{
	BYTE	yUserType		;
}PACK_CHECK_CHANNEL, *LPPACK_CHECK_CHANNEL	;

//, begin
//	定义 dwCommandID
#define	CMD_SERVER_CHANGE_CHANNEL_STATE			1	// 通知客户端服务器中该 channel 的状态发生执行变
typedef	struct _PACK_SERVER_COMMAND
{
	BYTE			yUserType		;
	DWORD			dwCommandID		;
}PACK_SERVER_COMMAND, *LPPACK_SERVER_COMMAND;

//	定义 dwCommandID
#define	APPLY_FOR_NORMAL_PROCESS		1	//请求正式打印
#define	CHANNEL_LOGOUT					2	//组长请求该channel机器全部下线
typedef	struct _PACK_CLIENT_COMMAND
{
	BYTE			yUserType		;
	DWORD			dwCommandID		;
}PACK_CLIENT_COMMAND, *LPPACK_CLIENT_COMMAND;
//, ends

#endif //__INC_PACKAGE_PROTOCOL__
