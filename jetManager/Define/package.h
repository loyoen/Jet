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
#define STATE_CheckDONE 4

#define SERVER_IP "192.168.1.4"
#define SERVER_PORT 8721

typedef	struct	tagPackage			*LPPACKAGE, PACKAGE;
typedef	struct	tagPackageHeader	*LPPACKAGE_HEADER, PACKAGE_HEADER;
typedef	struct	tagPackageBody		*LPPACKAGE_BODY, PACKAGE_BODY;

struct tagPackageHeader	// 包头
{
	WORD	wVerify_1;	// 第一验证码, 包头验证
	WORD	wVerify_2;	// 第二验证码
	
	DWORD	dwLength;	// 包主体长度

	BYTE	yType;		// 包类型,：打印头数据、指令类数据、状态信令、图片数据、连接认证数据

	BYTE	yReserved;	// 保留字节
	WORD	wReserved;	// 保留字
};

struct tagPackageBody	// 包主体
{
	LPVOID	lpBuffer;	// 缓存，存放具体数据内容
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
struct tagPrinterData	// 打印头数据包
{
	LONG	XAxis;			// 打印头空间位置X
	LONG	YAxis;			// 印头空间位置Y
	LONG	ZAxis;			// 印头空间位置Z
	BOOL	bSwitchControl;	// 打印头开关控制SWITCH
	INT		cColorSelect;	// 打印头颜色选择Color
	INT		iTime;	// 打印头持续工作时间T
	BOOL	bContinuous;  //与上一点连续
};
typedef	tagPrinterData	*LPDATA_PRINTER, DATA_PRINTER;

// 指令类数据包
struct tagInstructionData	
{
	int	 iSocket;			// 目标客户端的SOCKET
	CHAR cInitialize;		// 设备初始化指令
	CHAR cConfigure;		// 设备配置指令
	CHAR cNewTask;			// 开始新任务
	CHAR cCancelTask;		// 取消任务
	INT iTaskFinished_count;// 执行任务（次数）
	CHAR cStopAnyway;		// 无条件停机
	CHAR cTaskStop;			// 暂停任务
	CHAR cVideoRequest;		// 视频数据的传输请求
	CHAR cVideoStop;		// 视频数据的终止传输
};

typedef	tagInstructionData	*LPDATA_INSTRUCTION, DATA_INSTRUCTION;

struct tagConnectionData
{
	BYTE		yType;		// client type
	//
};


// 状态信令类数据包

struct tagStateData	
{
	INT		iTireState;
	/*
	Struct sMechState { };	// 机械状态
	Struct sMateState { };	// 料仓状态
	Struct sGasState{ };		// 气路状态
	Struct sSystemState { };	// 系统状态
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