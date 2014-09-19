#ifndef _PACKAGE_H_
#define _PACKAGE_H_
#include<windows.h>

#define BODY_LEN				4080	// 4096 - 12 - 4
#define TAG_LOGON_DATA			0
#define TAG_PRINT_DATA			1
#define TAG_INSTRUCTION_DATA	2
#define TAG_STATUE_DATA			3
#define TAG_CONNECTION_DATA		4
#define TAG_BIND_JET_DEVICE		5
#define TAG_CLIENT_DATA			6
#define TAG_PICTURE_DATA		7
#define TAG_RESEND_DATA			8
#define TAG_REVISE_ZEOR_DATA	9
#define TAG_TEST_DATA			100

#define CONTROL_END	0
#define DEVICE_END	1
#define JET_END		2


#define NEW_CLIENT	0
#define OFF_CLIENT		1
#define STATE_READY	2
#define STATE_DONE	3
#define STATE_RIGHTDONE 4
#define STATE_WRONGDONE 5

#define SERVER_IP "192.168.1.4"
#define SERVER_PORT 8721

typedef	struct	tagPackage			*LPPACKAGE, PACKAGE;
typedef	struct	tagPackageHeader	*LPPACKAGE_HEADER, PACKAGE_HEADER;
typedef	struct	tagPackageBody		*LPPACKAGE_BODY, PACKAGE_BODY;

struct tagPackageHeader	// ��ͷ
{
	WORD	wVerify_1;	// ��һ��֤��, ��ͷ��֤
	WORD	wVerify_2;	// �ڶ���֤��
	
	DWORD	dwLength;	// �����峤��

	BYTE	yType;		// ������,����ӡͷ���ݡ�ָ�������ݡ�״̬���ͼƬ���ݡ�������֤����

	BYTE	yReserved;	// �����ֽ�
	WORD	wReserved;	// ������
};

struct tagPackageBody	// ������
{
	LPVOID	lpBuffer;	// ���棬��ž�����������
};

struct tagPackage
{
	PACKAGE_HEADER	pckHeader;
	PACKAGE_BODY	pckBody;
};
struct tagTaskData
{
	DWORD	dwTaskId;
	DWORD	dwPackNum;
	DWORD	dwID;
};
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
struct tagReviseZeorData
{
	int			iX;
	int			iY;
};
#endif