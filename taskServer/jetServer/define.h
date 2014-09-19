#ifndef DEFINE_H
#define DEFINE_H
#include<windows.h>

typedef	struct	tagPackage			*LPPACKAGE, PACKAGE;
typedef	struct	tagPackageHeader	*LPPACKAGE_HEADER, PACKAGE_HEADER;
typedef	struct	tagPackageBody		*LPPACKAGE_BODY, PACKAGE_BODY;
#define BODY_LEN				4096
#define TAG_PRINT_DATA			1
#define TAG_INSTRUCTION_DATA	2
#define TAG_STATUE_DATA			3
#define TAG_CONNECTION_DATA		4
#define TAG_BIND_JET_DEVICE		5
#define TAG_CLIENT_DATA			6
#define TAG_PICTURE_DATA		7
#define TAG_RESEND_DATA		8

#define CONTROL_END	0
#define DEVICE_END	1
#define JET_END		2

#define STATE_READY	0
#define STATE_DONE	1
#define NEW_CLIENT	2
#define OFF_CLIENT	3

#define SERVER_IP "192.168.1.12"
#define SERVER_PORT 8721

struct tagPackageHeader	// ��ͷ
{
	DWORD	dwVerify_1;	// ��һ��֤��, ��ͷ��֤
	DWORD	dwVerify_2;	// �ڶ���֤��
	
	DWORD	dwTaskId;	// ��ǰ������
	//DWORD	dwLen;
	DWORD	dwLength;	// �����峤��
	DWORD	dwPackNum;	// ��ǰ���ݹ���ֳɶ��ٸ���

	INT		yType;		// ������,����ӡͷ���ݡ�ָ�������ݡ�״̬���ͼƬ���ݡ�������֤����

	DWORD	yID;		// �ڼ�����

	INT		dwDestination;  // Ŀ�ĵ�, -1��ʾ��������

	BYTE	yReserved;	// �����ֽ�
	WORD	wReserved;	// ������
};

struct tagPackageBody	// ������
{
	BYTE	dwBody[BODY_LEN];		// ����������4���ֽ�
	//BYTE	lpBuffer;
};

struct tagPackage{
	PACKAGE_HEADER	 pckHeader;
	PACKAGE_BODY	pckBody;
};

typedef	tagPackage	*LPPACKAGE, PACKAGE;


struct tagPrinterData	// ��ӡͷ���ݰ�
{
	LONG	XAxis;			// ��ӡͷ�ռ�λ��X
	LONG	YAxis;			// ӡͷ�ռ�λ��Y
	LONG	ZAxis;			// ӡͷ�ռ�λ��Z
	BOOL	bSwitchControl;	// ��ӡͷ���ؿ���SWITCH
	INT		cColorSelect;	// ��ӡͷ��ɫѡ��Color
	INT		iTime;	// ��ӡͷ��������ʱ��T
	BOOL	bContinuous;  //����һ������
};
typedef	tagPrinterData	*LPDATA_PRINTER, DATA_PRINTER;

// ָ�������ݰ�
struct tagInstructionData	
{
	int	 iSocket;			// Ŀ��ͻ��˵�SOCKET
	CHAR cInitialize;		// �豸��ʼ��ָ��
	CHAR cConfigure;		// �豸����ָ��
	CHAR cNewTask;			// ��ʼ������
	CHAR cCancelTask;		// ȡ������
	INT iTaskFinished_count;// ִ�����񣨴�����
	CHAR cStopAnyway;		// ������ͣ��
	CHAR cTaskStop;			// ��ͣ����
	CHAR cVideoRequest;		// ��Ƶ���ݵĴ�������
	CHAR cVideoStop;		// ��Ƶ���ݵ���ֹ����
};

typedef	tagInstructionData	*LPDATA_INSTRUCTION, DATA_INSTRUCTION;

struct tagConnectionData
{
	BYTE		yType;		// client type
	//
};


// ״̬���������ݰ�

struct tagStateData	
{
	INT		iTireState;
	/*
	Struct sMechState { };	// ��е״̬
	Struct sMateState { };	// �ϲ�״̬
	Struct sGasState{ };		// ��·״̬
	Struct sSystemState { };	// ϵͳ״̬
	*/
};

typedef	tagStateData	*LPDATA_STATE, DATA_STATE;


struct tagBindClientData
{
	INT		iJetSocket;
	INT		iDeviceSocket;
};

struct tagClientData
{
	INT		iClientState;
	char	cClientIp[16];
	INT		iSocket;
	INT		iType;
};

struct tagResendData
{
	INT		TaskId;
	INT		PackType;
	INT		PackId;
};

#endif