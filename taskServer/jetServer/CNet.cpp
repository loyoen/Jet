#include "StdAfx.h"
#include "CNet.h"
#include "CServer.h"

void* Wait4Client(void* pParam)
{
	CServer *pServer = (CServer*)pParam;
	CMSG_Server::GetCMSG_Server()->AddMessage("Strat Wait Client...\r\n",1,GREEN);
	pServer->WaitClient();
	return 0;
}

void *LoopNet(void *pParam)
{
	CNet	*pNet = (CNet*)pParam;
	pNet->LoopData();
	return 0;
}

void *ServerWork(void *pParam)
{
	CServer *pServer = (CServer*)pParam;
	
	HANDLE CompletionPort = pServer->mpNet->getIOCompletionPort();

	DWORD               dwBytesTransfered = 0;
	
	IOCompeletionData	*pIOCompeletionData = NULL;
	OVERLAPPED          *pOverlapped = NULL;
	LPPER_IO_OPERATION_DATA PerIoData;

	
	while(!pServer->getStop())
	{
		BOOL RE = GetQueuedCompletionStatus(CompletionPort,&dwBytesTransfered,(PULONG_PTR)&pIOCompeletionData,(LPOVERLAPPED *) &PerIoData,INFINITE);
		if(dwBytesTransfered!=0)
		{
			//LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
			//memcpy(pOnePackage,PerIoData->DataBuf.buf,sizeof(PACKAGE));
			if(PerIoData->OPERATION_TYPE==_SEND_)
			{
				PerIoData->HaveDone += dwBytesTransfered;
				if(PerIoData->HaveDone==PerIoData->ShouldDone)
					pServer->mpNet->doSend(PerIoData,pIOCompeletionData);
				else
				{
					PerIoData->DataBuf.len = PerIoData->ShouldDone - PerIoData->HaveDone;
					PerIoData->DataBuf.buf += PerIoData->HaveDone;
					DWORD Flags = 0;
					DWORD RecvBytes;
					WSASend(pIOCompeletionData->mySocket, &(PerIoData->DataBuf), 1, &RecvBytes, Flags,&(PerIoData->Overlapped), NULL);
				}
				
			}
			else if(PerIoData->OPERATION_TYPE==_RECV_)
			{
				PerIoData->HaveDone += dwBytesTransfered;
				if(PerIoData->HaveDone==PerIoData->ShouldDone)
					pServer->mpNet->doReav(PerIoData,pIOCompeletionData);
				else
				{
					PerIoData->DataBuf.len = PerIoData->ShouldDone - PerIoData->HaveDone;
					PerIoData->DataBuf.buf += PerIoData->HaveDone;
					DWORD Flags = 0;
					DWORD RecvBytes;
					WSARecv(pIOCompeletionData->mySocket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,&(PerIoData->Overlapped), NULL);
				}
			}
		}
		else
		{
			pServer->DealClientOff(pIOCompeletionData->mySocket);
			/*if(PerIoData->DataBuf.buf)
			{
				delete PerIoData->DataBuf.buf;
				free(PerIoData);
			}*/
		}

	}
	return 0;
}

CNet::CNet(void *myServer)
{
	mp_PackHead = NULL;
	mp_PackTail = NULL;
	mbStop = false;
	mpLock = new CLock();
	mpServer = myServer;
	//pDlg = (CDialog*)myDC;
}
CNet::~CNet()
{
	delete mpLock;
	for(int i=0;i<int(cpWorkThreads.size());i++)
		delete cpWorkThreads.at(i);
	delete cpNetThread;
}

int CNet::StartServer()
{
	WORD wVersionRequested;
    WSADATA wsaData;
    int ret;
    
    struct sockaddr_in saServer; //地址信息   

    //WinSock初始化
    wVersionRequested=MAKEWORD(2, 2); //希望使用的WinSock DLL 的版本
    ret=WSAStartup(wVersionRequested, &wsaData);
    if(ret!=0)
    {
        printf("WSAStartup() failed!\n");
        return -1;
    }
    //创建Socket,使用TCP协议
    sListen=WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (sListen == INVALID_SOCKET)
    {
        WSACleanup();
        printf("socket() faild!\n");
        return -1;
    }
	//unsigned long ul=1;  
    //int r=ioctlsocket(sListen,FIONBIO,&ul);
    //构建本地地址信息
    saServer.sin_family = AF_INET; //地址家族
    saServer.sin_port = htons(SERVER_PORT); //注意转化为网络字节序
    saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //使用INADDR_ANY 指示任意地址
  
    //绑定
    ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));
    if (ret == SOCKET_ERROR)
    {
        printf("bind() faild! code:%d\n", WSAGetLastError());
        closesocket(sListen); //关闭套接字
        WSACleanup();
        return -1;
    }
  
    //侦听连接请求
    ret = listen(sListen, 5);
    if (ret == SOCKET_ERROR)
    {
        printf("listen() faild! code:%d\n", WSAGetLastError());
        closesocket(sListen); //关闭套接字
        return -1;
    }



	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );
	SYSTEM_INFO si;

	GetSystemInfo(&si);
	 
	int m_nThreads = 2 * si.dwNumberOfProcessors - 2;
	
	// 为工作者线程初始化句柄
	//m_phWorkerThreads = new HANDLE[m_nThreads];
	
	// 根据计算出来的数量建立工作者线程
	for (int i = 0; i < m_nThreads; i++)
	{
		//m_phWorkerThreads[i] = ::CreateThread(0, 0, _WorkerThread, (void *)pThreadParams, 0, &nThreadID);
		CThread *cpWorkThread = new CThread(ServerWork,mpServer);
		cpWorkThreads.push_back(cpWorkThread);
	}

	cpNetThread = new CThread(LoopNet,this);

	cpWaitClientThread = new CThread(Wait4Client,mpServer);
	
	return 1;
}

void CNet::WaitClient()
{
	struct sockaddr_in saClient; //地址信息   
	SOCKET sServer;

	CMSG_Server::GetCMSG_Server()->AddMessage("loop listen client...\r\n",1,BLUE);

	int maxfdp=NULL;  
	struct fd_set fds;  
	struct timeval timeOut={1,1};

	HANDLE CompletionPort;
	char clientType[10] = {0};
	DWORD                dwBytesTransfered = 0;
	OVERLAPPED           *pOverlapped = NULL;
	
	DWORD Flags = 0;
	DWORD RecvBytes;
	while(!mbStop)//循环监听客户端，永远不停止.
	{
		FD_ZERO(&fds);  
		FD_SET(sListen,&fds);  
		int length = sizeof(saClient);
		if (select(maxfdp,&fds,NULL,NULL,&timeOut))
		{
			sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
			CMSG_Server::GetCMSG_Server()->AddMessage("one client has connected us...\r\n",1,GREEN);
			
			IOCompeletionData *pIOCompeletionData = (IOCompeletionData*)malloc(sizeof(IOCompeletionData));
			pIOCompeletionData->mySocket = sServer;
			pIOCompeletionData->sin_addr = saClient.sin_addr;
			//pIOCompeletionData->type = _RECV_;

			LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)malloc(sizeof(PER_IO_OPERATION_DATA));
			ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
			PerIoData->HaveDone = 0;
			PerIoData->ShouldDone = sizeof(tagPackageHeader);;
			PerIoData->DataBuf.len = sizeof(tagPackageHeader);
			PerIoData->DataBuf.buf = PerIoData->HeadBuffer;
			PerIoData->OPERATION_TYPE = _RECV_;
			PerIoData->HRED_OR_BODY = _HEAD_;
			CompletionPort = CreateIoCompletionPort(HANDLE(sServer), m_hIOCompletionPort, (DWORD)(pIOCompeletionData), 0);
			WSARecv(sServer, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,&(PerIoData->Overlapped), NULL);
			sleep(0);
		}
	}
	
	//先注释掉
	/*if(PerIoData->DataBuf.buf)
	{
		delete PerIoData->DataBuf.buf;
		free(PerIoData);
	}*/
	//以后再打开
	//setStop(true);
	/*
	for(int i=0;i<int(cpWorkThreads.size());i++)
		cpWorkThreads.at(i)->wait4ThisThread();
		*/
	//cpNetThread->wait4ThisThread();

}
HANDLE CNet::getIOCompletionPort()
{
	return m_hIOCompletionPort;
}
int CNet::SendData(CClient *client,void* data,int size)
{
	DWORD byteSend, Flags;  
	OVERLAPPED Overlapped;
	Flags = 0;

	memset((void*)&Overlapped, 0x00, sizeof(OVERLAPPED));
	
	char *buf = new char[size+1];
	memset(buf,0,size+1);
	memcpy(buf,data,size);
	//wsaBuffer.buf = buf;
	//wsaBuffer.len = size;
	//free(data);

	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)malloc(sizeof(PER_IO_OPERATION_DATA));

	ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
	PerIoData->HaveDone = 0;
	PerIoData->ShouldDone = size;
	PerIoData->DataBuf.len = size;
	PerIoData->DataBuf.buf = buf;
	PerIoData->OPERATION_TYPE = _SEND_;
	int Result = WSASend(client->getSocket(),&(PerIoData->DataBuf),1,&byteSend,Flags,&(PerIoData->Overlapped),NULL);

	if (Result = SOCKET_ERROR)
		return -1;
	return Result;
}
int CNet::RecvData(CClient *client,void* data,int size)
{
	return recv(client->getSocket(),(char*)data,size,0);
}
bool CNet::HeartBeat(CClient *client)
{
	return true;
}

void CNet::CloseSocket(CClient *client)
{
	client->ClosethisSocket();
}
void CNet::AddPack(LPPACKAGE pPackage, CClient *pClient)
{
	mpLock->Lock();

	PACK_DES	*pPackDes = (PACK_DES*)malloc(sizeof(PACK_DES));
	pPackDes->mp_Client = pClient;
	pPackDes->mp_Package = pPackage;
	pPackDes->next = NULL;
	if(mp_PackTail==NULL||NULL==mp_PackHead)
	{
		mp_PackTail = pPackDes;
		mp_PackHead = pPackDes;
	}
	else
	{
		mp_PackTail->next = pPackDes;
		mp_PackTail = pPackDes;
	}
	mpLock->Unlock();
}
PACK_DES* CNet::GetOnePack()
{
	mpLock->Lock();
	if(NULL==mp_PackHead)
	{
		mpLock->Unlock();
		return NULL;
	}
	else
	{
		PACK_DES	*pTmpPack = mp_PackHead;
		mp_PackHead = mp_PackHead->next;
		if(NULL==mp_PackHead)
			mp_PackTail = NULL;

		mpLock->Unlock();
		return pTmpPack;
	}
	
}
void CNet::LoopData()
{
	//char thId[10];
	while(!mbStop)
	{
		
		PACK_DES	*pPackDes = GetOnePack();
		if(pPackDes!=NULL)
		{
			pPackDes->mp_Client->mpLock->Lock();
			
			//CMSG_Server::GetCMSG_Server()->AddMessage("LOCKED...\r\n",1,GREEN);
			int ThreadId = ::GetCurrentThreadId();
			//sprintf(thId,"%d",pPackDes->mp_Package->pckHeader.yID);
			//CMSG_Server::GetCMSG_Server()->AddMessage(thId,1,GREEN);
			int BodyLen = pPackDes->mp_Package->pckHeader.dwLength - sizeof(tagPackageHeader);
			
			SendData(pPackDes->mp_Client,&(pPackDes->mp_Package->pckHeader),sizeof(tagPackageHeader));
			Sleep(10);
			
			SendData(pPackDes->mp_Client,pPackDes->mp_Package->pckBody.lpBuffer,BodyLen);
			Sleep(10); // 对付粘包
			
			//CMSG_Server::GetCMSG_Server()->AddMessage("UNLOCKED...\r\n",1,GREEN);
			pPackDes->mp_Client->mpLock->Unlock();
			//LeaveCriticalSection(&g_CriticalSection);
			
			//free(pPackDes->mp_Package);
			free(pPackDes);
		}
		Sleep(0);
	}
}
void CNet::setStop(bool singal)
{
	mbStop = singal;
}
void CNet::doReav(LPPER_IO_OPERATION_DATA PerIoData,IOCompeletionData *pIOCompeletionData)
{
	CServer* pServer = (CServer*)mpServer;
	DWORD RecvBytes;
	tagPackageHeader mytagPackageHeader;
	memcpy(&mytagPackageHeader,PerIoData->HeadBuffer,sizeof(tagPackageHeader));
	if(PerIoData->HRED_OR_BODY == _HEAD_)
	{
			
		PerIoData->DataBuf.len = mytagPackageHeader.dwLength-sizeof(tagPackageHeader);
		PerIoData->HaveDone = 0;
		PerIoData->ShouldDone = mytagPackageHeader.dwLength-sizeof(tagPackageHeader);
		PerIoData->OPERATION_TYPE = _RECV_;
		PerIoData->DataBuf.buf = PerIoData->Buffer;
		DWORD Flags = 0;
		PerIoData->HRED_OR_BODY = _BODY_;
		WSARecv(pIOCompeletionData->mySocket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,&(PerIoData->Overlapped), NULL);
		return;
	}
	else
	{
		LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
		memcpy(&(pOnePackage->pckHeader),&mytagPackageHeader,sizeof(mytagPackageHeader));
		char*	DataPack = (char*)malloc(pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader));
		
		memcpy(DataPack,PerIoData->Buffer,pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader));
		pOnePackage->pckBody.lpBuffer = DataPack;
		pServer->RecvPackage(pOnePackage,pIOCompeletionData);
	}
	ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
	PerIoData->HRED_OR_BODY = _HEAD_;
	PerIoData->HaveDone = 0;
	PerIoData->ShouldDone = sizeof(tagPackageHeader);
	PerIoData->DataBuf.len = sizeof(tagPackageHeader);
	PerIoData->DataBuf.buf = PerIoData->HeadBuffer;
	PerIoData->OPERATION_TYPE = _RECV_;
				
	DWORD Flags = 0;
	WSARecv(pIOCompeletionData->mySocket, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,&(PerIoData->Overlapped), NULL);
}
void CNet::doSend(LPPER_IO_OPERATION_DATA PerIoData, IOCompeletionData *pIOCompeletionData)
{
	//delete PerIoData->DataBuf.buf;
	//free(PerIoData);
}
int CNet::SendClientDataToControl(CClient *pClientData, CClient *pClientCotrol)
{
	if(pClientCotrol!=NULL)
	{
		PACKAGE *pOnePackage = (PACKAGE*)malloc(sizeof(PACKAGE));
		pOnePackage->pckHeader.yType = TAG_CLIENT_DATA;
		pOnePackage->pckHeader.wVerify_1 = 8721;
		pOnePackage->pckHeader.wVerify_2 = 8721;
		pOnePackage->pckHeader.dwLength = sizeof(tagClientData)+sizeof(tagPackageHeader);
		tagClientData myClientData;
		myClientData.iSocket = int(pClientData->getSocket());
		myClientData.iType = pClientData->getType();
		myClientData.iClientState = pClientData->getState();
		strcpy_s(myClientData.cClientIp,inet_ntoa(pClientData->getClientIP()));
		char* DataPack = (char*)malloc(sizeof(tagClientData));
		memcpy(DataPack,&myClientData,sizeof(tagClientData));
		pOnePackage->pckBody.lpBuffer = DataPack;
		AddPack(pOnePackage,pClientCotrol);
	}
	return 0;
}