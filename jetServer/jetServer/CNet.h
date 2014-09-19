#ifndef CNET_H
#define CNET_H
#include "CClient.h"
#include "CLock.h"
#include "package.h"
#include <vector>
#include "CThread.h"
#include "CData.h"

#define _ACCEPT_	0
#define _RECV_		1
#define _SEND_		2
#define _HEAD_		0
#define _BODY_		1

#define DATA_BUFSIZE 4096   //要改
#define HEAD_BUFSIZE 128

struct IOCompeletionData
{
	in_addr sin_addr;
	SOCKET	mySocket;
};

typedef struct
{
   OVERLAPPED Overlapped;
   WSABUF DataBuf;
   CHAR HeadBuffer[HEAD_BUFSIZE];
   CHAR Buffer[DATA_BUFSIZE];
   DWORD HaveDone;
   DWORD ShouldDone;
   DWORD OPERATION_TYPE;
   DWORD HRED_OR_BODY;
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;

struct PACK_DES
{
	LPPACKAGE	mp_Package;
	CClient		*mp_Client;
	PACK_DES	*next;
};
class CNet
{
public:
	PACK_DES			*mp_PackHead;
	PACK_DES			*mp_PackTail;
	SOCKET				sListen;
	bool				mbStop;
	CLock				*mpLock;
	void				*mpServer;
	vector<CThread*>	cpWorkThreads;
	CThread*			cpWaitClientThread;
	CThread				*cpNetThread;
	int					miChoice;
	vector<CData*>		DataFree;
	HANDLE				m_hIOCompletionPort;           // 完成端口的句柄
	//CDialog				*pDlg;
public:
	CNet(void *myServer);
	~CNet();
	
	int StartServer();
	void WaitClient();

	void LoopData();
	int SendData(CClient *client,void* data,int size);
	int RecvData(CClient *client,void* data,int size);
	bool HeartBeat(CClient *client);
	PACK_DES* GetOnePack();
	void AddPack(LPPACKAGE pPackage,CClient *pClient);
	void CloseSocket(CClient *client);
	void setStop(bool singal);
	HANDLE getIOCompletionPort();

	void doReav(LPPER_IO_OPERATION_DATA PerIoData,IOCompeletionData *pIOCompeletionData);
	void doSend(LPPER_IO_OPERATION_DATA PerIoData,IOCompeletionData *pIOCompeletionData);

	int SendClientDataToControl(CClient *pClientData, CClient *pClientControl);

};


#endif