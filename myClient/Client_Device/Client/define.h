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

#define SERVER_IP "192.168.1.4"
#define SERVER_PORT 8721

struct tagPackageHeader	// 包头
{
	DWORD	dwVerify_1;	// 第一验证码, 包头验证
	DWORD	dwVerify_2;	// 第二验证码
	
	DWORD	dwTaskId;	// 当前任务编号
	//DWORD	dwLen;
	DWORD	dwLength;	// 包主体长度
	DWORD	dwPackNum;	// 当前数据共拆分成多少个包

	INT		yType;		// 包类型,：打印头数据、指令类数据、状态信令、图片数据、连接认证数据

	DWORD	yID;		// 第几个包

	INT		dwDestination;  // 目的地, -1表示到服务器

	BYTE	yReserved;	// 保留字节
	WORD	wReserved;	// 保留字
};

struct tagPackageBody	// 包主体
{
	BYTE	dwBody[BODY_LEN];		// 包主体最少4个字节
	//BYTE	dwBody;
};

struct tagPackage{
	PACKAGE_HEADER	 pckHeader;
	PACKAGE_BODY	pckBody;
};

typedef	tagPackage	*LPPACKAGE, PACKAGE;


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

#endif