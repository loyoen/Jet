
#ifndef __INC_PACKAGE_PROTOCOL__
#define __INC_PACKAGE_PROTOCOL__

#include "Winsock2.h"


#define	SWITCH_AUTO_TESTING		// �Զ����Կ���

//#define	SWITCH_CLIENT_RESTRICT	// ���ƿͻ���ĳЩ����

#define	SWITCH_STORE_SCRIPT		// ���滭����Ϣ
#define	SWITCH_QUALITY_MANAGEMENT		// ��������


#define	VERSION_CLIENT	715		// �汾(����������ͻ���ֻҪ��һ��ִ�б�ʱ��ִ��֮)
#define	TITLE_SERVER	"��ɫ��̥��ӡϵͳ������"
#define	TITLE_CLIENT	"��ɫ��̥��ӡϵͳ�����ն�"

#ifdef	SWITCH_QUALITY_MANAGEMENT
#define	QUALITY_BUFFER_INCREASING_BYTES		1024
#define	QUALITY_BUFFER_SEND_SEGMENT_BYTES	20
#endif

// �ͻ��˰�ͼ�ṹ
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

#define		MAX_CHANNELS_IN_SERVER	64		// �����������������豸����
#define		MAX_JOBS_IN_CHANNEL		256		// ��task��������job��

// ����Ķ���
/*
#define SIZE_LARGE_MEM				(300*1024)
#define	SIZE_SMALL_MEM				8192
#define	SIZE_COMMAND_MEM			1024*/

//======= ���� ==========================================
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

//======= ���� ==========================================
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
//�Զ���ö���������ͣ�������ʶ�׽���IO��������
//
typedef enum _NET_OPERATION 
{
	NetAccept,	//AcceptEx/accept
	NetRead,		//WSARecv/recv/ReadFile
	NetWrite,	//WSASend/send/WriteFile
	NetEnd,
	NetRecycleIoContextOnly	//���ͷ�IO_CONTEXT,����Notify��Ϣʱ�õ�
}NET_OPERATION, *LPNET_OPERATION;

//#define		BUFFER_SIZE				4096
//++++++++++++++++++++++++++++++++++++++++++++++++++++++
//	���������(��ɼ�)
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
//	��IO���ݣ���չ��WSAOVERLAPPED
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
// ���ݻ���
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

// ��������ṹ�嶨�����û����ݰ�������
// �û���Ϣ
typedef	struct	_USER_MESSAGE
{
	BYTE	yUserType;					// �û���Ϣͷ���������Ǹ�ʲô������������
	LPVOID	pBuffer;					// ���棬��ž�����������
}USER_MESSAGE, *LPUSER_MESSAGE;
// ��Ϣͷ�����������֮һ
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
#define		MSG_SERVER_QUALITY_MANAGEMENT_SEGMENT	0x35	// �����������ƿ�
#define		MSG_SERVER_QUALITY_MANAGEMENT			0x36	// ����һ����ȫ����������
#endif

// ��������ṹ�嶨�����������ݰ�������
typedef	struct	_NET_COMMAND
{
	LPVOID	lpBuffer				;	// ���棬��ž�����������
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

// ��ͷ�Ķ���

// ��ͷyTypePackage�����������֮һ
#define		USER_COMMAND	0	// ����һ�δ�����û�����, ����ֱ�ӽ����û�����
#define		NET_SCATTER		1	// ���ָ��С��, ��Ҫgather���͸��û�����
#define		NET_SERVERIN	2	// ��һ�µķ������������ϵͳ��Ϊ�ݴ������
#define		NET_SERVEROUT	3	// �ݴ�����������˳�
#define		NET_RECONSTRUCT	4	// �ݴ�����������ؽ�
#define		NET_RELOGIN_SUCCESS		5	// ���µ�½�ɹ�
#define		NET_CHECK		254	// �ͻ��˼������״̬��(��Server�յ�Check��ʱ���̽�������)
#define		NET_PONG		255	// ������, �û�PONG������
#define		NET_PING		255	// ������, ������PING�û�

// ��������ṹ���ۺ϶����˱�ϵͳ���а�������

typedef	struct	_COMMAND_HEADER
{
	WORD	wCmdSize;
}COMMAND_HEADER, *LPCOMMAND_HEADER;

typedef struct _CHANNEL_INFO
{
	int				iIDChannel		;	// task��ID
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
	int			m_iReserved			;	// ������,  ����Ƿ�ӿ۷����ȱ�־
//	BYTE		m_yScoreFormID[16]	;	// �� TaskFormInfo �е� ID
	int			m_iScoreFormID[16]	;	// �� TaskFormInfo �е� ID
	BYTE		m_yTaskType			;
	char		m_strChannelName[25];
	char		m_strTaskTitle[11]	;
	char		m_strSubTask[13]	;
}   CHANNELATTRIB, *LPCHANNELATTRIB	;

typedef	struct	_MACHINE_ATTRIB
{
	int					m_iMachineID	;	// �û���ID

	WORD				m_enumStatus	;	// ����״̬
	BYTE				m_yHead			;	// �鳤���

	char				m_strAccount[21];	// �ʺ�
	char				m_strPsw[21]	;	// ����
	char				m_strName[11]	;	// ���
	
	int					m_iPriority		;	// ���ȼ�

	// ͳ������
	int					m_iCntPrinted	;	// �����job��
	int					m_iAcceptCnt	;	// ��Ч��ӡ��
	int					m_iSentCnt		;	// �Ѿ�����δ�ջص�job��
	int					m_iSentExCnt	;	// �Ѿ�����δ�ջص�job��
	int					m_iUpperCnt		;	// ��ҵ������
	int					m_iLowerCnt		;	// ��ҵ������
}MACHINE_ATTRIB, *LPMACHINE_ATTRIB	;

typedef	struct	_CHANNEL_STATISTIC
{
	int			m_iFreshCnt				;	// ��ȫδ��ӡ�ķ���	Flag = 0
	int			m_iCntPrinted			;	// �Ѿ���ӡ��ķ���	Flag = 1
	int			m_iCntTotal				;	// ����ҵ��
	int			m_iCntMachine			;	// �� Channel �еĻ�����
	int			m_iRecycleMachineCnt	;	// ��ǰ���ڻ���job�Ļ�������
}	CHANNEL_STAT, *LPCHANNEL_STAT	;

//typedef	enum	_Machine_STATUS	//�˴���������channel�е� m_pMachineArray;
//{
#define	Machine_OffLine			1		// ����
#define	Machine_DropLine		1		// ����
#define	Machine_OnLine			2		// ����
#define	Machine_Finishing		4		// �ڴ�״̬�²��ٶԴ˻�������job
#define	Machine_Kickout			8		// ǿ������1��
#define	Machine_Kickout2nd		16		// ǿ������2��
#define	Machine_Recycling		32		// ���ڻ���
//#define	Machine_Recycled	64	// �Ѿ���ɻ���
#define	Machine_Dummy			64		// ��·Ԫ��, ��ʱ������channel���ȼ��ĸ���
#define	Machine_Logout			128		// �û��ǳ�
#define	Machine_ReLogin			256		// �û����ص�¼
#define	Machine_HeadNeedTask	512
//#define	Machine_TransInfo			512		// �û��ڴ�������
//}Machine_STATUS, *LPMachine_STATUS;

typedef	struct	_MACHINE_STATE
{
	int			m_iMachineID			;	// �û���ID
	char		m_strMachineName[11]	;	// ���
//	BYTE		m_enumStatus			;	// ����״̬
	WORD		m_enumStatus			;	// ����״̬
	int			m_iPriority				;	// ���ȼ�
	int			m_iCntPrinted			;	// �����job��
	int			m_iAcceptCnt			;	// ��Ч��ӡ��
	int			m_iSentCnt				;	// �Ѿ�����δ�ջص�job��
	int			m_iSentExCnt			;	// �Ѿ�����δ�ջص�job��
}MACHINE_STATE, *LPMACHINE_STATE;

typedef struct _PACK_LOGIN
{
	BYTE			yUserType		;
	char			strAccount[21]	;	// �û�ID
	char			strPassword[21]	;	// ��½����
	int				iVersion		;	// �汾
	CString			userip			;	// ����IP������Operator�����������������͸�ÿ����½���û�
}PACK_LOGIN, *LPPACK_LOGIN;

// ���������͸�Client��ȷ�ϵ�½����Ϣ
typedef struct _PACK_LOGIN_SUCCESS
{
	BYTE			yUserType		;
	int				iMachineID		;
	int				iOperator		;	// ��Channel Operator�ı�ʶ����MachineIDһ����
	char			strNickName[11]	;	// Machine's name
	int				iPriority		;	// ��ǰΪ���İ汾, ���ڱ���
	int				iMarkedCnt		;
	ChannelState	enumState		;	// ��ִ��״̬������״̬, ��ֵ�ĺ��������涨��
	BYTE			yHead			;
	CHANNEL_INFO	lpChannel		;	// Channel��Ϣ 
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

// �û����͵����µ�½����Ϣ
typedef struct _PACK_RELOGIN
{
    BYTE			yUserType		;
    int				iMachineID		;
	int				iOperator		;	// �Լ���Operator����Server����������ʱ��Ҫ�õ�
//	CString			userip			;
}PACK_RELOGIN, *LPPACK_RELOGIN;

// �ص�½�ɹ�
typedef	struct _PACK_RELOGIN_SUCCESS
{
	BYTE			yUserType		;
	BOOL			bSuccess		;
	BOOL			bServerRestarted;
}PACK_RELOGIN_SUCCESS, *LPPACK_RELOGIN_SUCCESS;

// ��������Operator
//
// ����������һ���µ�Operator,֪ͨ��
typedef struct _PACK_SELECT_OPERATOR
{
	BYTE			yUserType		;
	int				iMachineID		;
}PACK_SELECT_OPERATOR, *LPPACK_SELECT_OPERATOR;

// Client���ܷ�������Operator��Ȩ
typedef struct _PACK_CONFIRM_OPERATOR
{
	BYTE			yUserType		;
	BOOL			bAccept			;
}PACK_CONFIRM_OPERATOR, *LPPACK_CONFIRM_OPERATOR;

// ��ʼ���µ�Operator
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

// ֪ͨ��Channnel�����û�
typedef struct _PACK_NEW_OPERATOR
{
	BYTE			yUserType		;
	int				iNewOperator	;
}PACK_NEW_OPERATOR, *LPPACK_NEW_OPERATOR;
//,end

// �û��ǳ�
typedef struct _PACK_LOGOUT
{
	BYTE			yUserType		;
	int				iMachineID		;
}PACK_LOGOUT, *LPPACK_LOGOUT;

typedef struct _PACK_LOGOUT_SUCCESS
{
	BYTE			yUserType		;
	int				iMachineID		;
	BYTE			yNewOperator	;	// �µ�Operator����Ҫ���Operator�ǳ�ʱ
}PACK_LOGOUT_SUCCESS, *LPPACK_LOGOUT_SUCCESS;

typedef	struct _PACK_LOGOUT_FAILURE
{
	BYTE			yUserType		;
	char			strFailInfo[64]	;
}PACK_LOGOUT_FAILURE, *LPPACK_LOGOUT_FAILURE;

// �ݽ���task�Ľṹ
typedef struct _JOB_SUBMITTED
{
	int				nJobID		;
	BYTE			bBadJob	;	// ���Ϲ淶��־
//, begin
#ifdef	SWITCH_STORE_SCRIPT
	BYTE			bHas_script		;	// �Ƿ��бʼ�
#endif
//, ends
}JOB_SUBMITTED, *LPJOB_SUBMITTED;

// task submit
typedef struct _PACK_JOB_SUBMIT
{
	BYTE				yUserType		;
	int					iPriority		;	// Client��δִ�е�job��
	int					iSubmitCnt		;	// ���ݽ���job��
	JOB_SUBMITTED		pItem[1]		;	// �ݽ���tasks����Ϣ
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
	int				nAcceptCnt		;	// �����������û��Ѿ����յ���Ŀ
}PACK_JOB_SUBMIT_ACK, *LPPACK_JOB_SUBMIT_ACK;

// task request
typedef struct _PACK_JOB_REQUEST
{
	BYTE			yUserType		;
	int				iUncheckedCnt	;	// Client��δִ�е�job��
}PACK_JOB_REQUEST, *LPPACK_JOB_REQUEST;

// �鳤����job��ִ��
typedef struct _PACK_HEAD_JOB_REQUEST
{
	BYTE			yUserType		;
	int				iUncheckedCnt	;	// Client��δִ�е�job��
}PACK_HEAD_JOB_REQUEST, *LPPACK_HEAD_JOB_REQUEST;

// �ͻ������������Operator����ִ�����job
typedef struct _PACK_JOB_TRANSFER_ACK
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	int				iPos			;	// ���ļ���ʲô�ط���ʼ�����ݿ飡
    int				iLen			;   // ϣ�����ܶ������ݿ�
	BOOL			bFTPOver		;   // ����FTP
}PACK_JOB_TRANSFER_ACK, *LPPACK_JOB_TRANSFER_ACK;

// �������Կͻ�������job����Ӧ
//,begin
typedef struct _PACK_JOB_REQUEST_ACK
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	unsigned int	dwTaskImageSize;	// ͼƬ��С
	int         	bLastFile		;	// ��Ϊ�ְ�����ʱ��ʶ�Ƿ�������һ���ļ�
	char			strFailInfo[64]	;
}PACK_JOB_REQUEST_ACK, *LPPACK_JOB_REQUEST_ACK;
//,end

// ��������ͻ����������job
typedef	struct	_PACK_JOB_RECYCLE
{
	BYTE			yUserType		;
//	BYTE			yAskCnt			;	// ϣ�����յķ���
	BYTE			yAbnormalCnt	;	// δ�������յķ��� -- ���64
	int				iPriority		;	// ���� Channel ��ǰ�� m_iPriority
	int				iFormatSheetID[64]	;	// δ�������յ��б�
}PACK_JOB_RECYCLE, *LPPACK_JOB_RECYCLE;

// �ͻ����صĿɵ���job�б�
typedef	struct	_PACK_JOB_RECYCLE_ACK
{
	BYTE			yUserType		;
	BYTE			yRecycleCnt		;	// ���յ����� -- ���64
	int				iFormatSheetID[64]	;	// �����б�
	int				iUncheckedCnt	;	// Client��δִ�е�job��
}PACK_JOB_RECYCLE_ACK, *LPPACK_JOB_RECYCLE_ACK;

typedef struct _PACK_JOB_FTP
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	DWORD			dwLen			;  //	����İ��ж೤
	char			data[1]			;
}PACK_JOB_FTP, *LPPACK_JOB_FTP;

typedef struct _PACK_JOBCHECK_FTP
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	char			strName1[11]	;	//	��һ�������ı��
	char			strName2[11]	;	//	�ڶ��������ı��
	char			strName3[11]	;	//	�����������ı��
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
	DWORD			dwLen			;	//	����İ��ж೤
	char			data[1]			;
}PACK_JOBCHECK_FTP, *LPPACK_JOBCHECK_FTP;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
typedef struct _PACK_SCRIPT_FTP
{
	BYTE			yUserType		;
	int				iFormatSheetID		;
	int				iMachineID		;

	DWORD			dwLen			;	//	����İ��ж೤
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
	char			strName[11]		;	//	�����ı��

	DWORD			dwLen			;	//	����İ��ж೤
	BYTE			data[1]			;
}PACK_QUALITY_SEGMENT, *LPPACK_QUALITY_SEGMENT;

typedef struct _PACK_QUALITY
{
	BYTE			yUserType		;
	int				iMachineID		;
	char			strName[11]		;	//	�����ı��

	DWORD			dwLen			;	//	����İ��ж೤
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
	char			strAccount[21]	;	// �û�ID
	char			strPswdOld[21]	;	// ������
	char			strPswddNew[21]	;	// ������	
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
	//char	m_strName[11]	;	// ��ҵ�������
	//int		iMark			;	// ��ҵ
	int		iSize			;
	int		iPreMarkDone	;	// �����Ѿ���ҵ�Ļ�������
	int		iPreMarkKind	;	// ���鵱ǰ��ҵ��������Ŀ
	//BYTE	yMarkCnt[1]		;	// ��һ����ҵ�Ļ�����
	//BYTE	yPreMark[1]		;	// ��һ����ҵ�ķ�ֵ
	char	cInfo[1]		;	//  ��Ϣ��
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
// �ص�½job����
// �����������ͻ��˵İ��ṹ
typedef struct _PACK_UNCHECKED_ON_DISK
{
	BYTE	yUserType		;
	int		iMachineID		;
	int		iJobCnt	;		// �ͻ���δ��job��
	int		iFormatSheetID[1]	;		// �ͻ���δ��jobID
}PACK_UNCHECKED_ON_DISK, *LPPACK_UNCHECKED_ON_DISK;

// �ͻ��˷����������İ��Ľṹ
typedef struct _PACK_CHECKED_RESULT
{
	BYTE 	yUserType		;
	int		iMachineID		;
	int		iJobCnt	;
	int		iFormatSheetID[1]	;
}PACK_CHECKED_RESULT, *LPPACK_CHECKED_RESULT;

// ���ε�½����
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
//	���� dwCommandID
#define	CMD_SERVER_CHANGE_CHANNEL_STATE			1	// ֪ͨ�ͻ��˷������и� channel ��״̬����ִ�б�
typedef	struct _PACK_SERVER_COMMAND
{
	BYTE			yUserType		;
	DWORD			dwCommandID		;
}PACK_SERVER_COMMAND, *LPPACK_SERVER_COMMAND;

//	���� dwCommandID
#define	APPLY_FOR_NORMAL_PROCESS		1	//������ʽ��ӡ
#define	CHANNEL_LOGOUT					2	//�鳤�����channel����ȫ������
typedef	struct _PACK_CLIENT_COMMAND
{
	BYTE			yUserType		;
	DWORD			dwCommandID		;
}PACK_CLIENT_COMMAND, *LPPACK_CLIENT_COMMAND;
//, ends

#endif //__INC_PACKAGE_PROTOCOL__
