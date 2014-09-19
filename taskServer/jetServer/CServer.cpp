#include "StdAfx.h"
#include "CServer.h"
#include "jetServer.h"

//#include "jetServer2Dlg.h"

CServer::CServer()
{
	//mpClientHead = NULL;
	mpNet = new CNet(this);
	mpLock = new CLock();
	mbStop = false;
	miChoice = STOPDRAW;
	mpControlClient = NULL;
	mpTaskHead = NULL;
	isNewTask = true;
	pMemTask = new Task();
	//pDlg = (CDialog*)pParam;
}
CServer::~CServer()
{
	mpLock->Lock();
	for(int i=0;i<int(mpDeviceClients.size());i++)
	{
		mpNet->CloseSocket(mpDeviceClients[i]);
		delete mpDeviceClients[i];
	}
	for(int i=0;i<int(mpJetClients.size());i++)
	{
		mpNet->CloseSocket(mpJetClients[i]);
		delete mpJetClients[i];
	}
	mpLock->Unlock();

	for(int i=0;i<int(DataFree.size());i++)
		delete DataFree[i];
	delete mpLock;
	delete mpNet;
	/*for(int i=0;i<int(cpWorkThreads.size());i++)
		delete cpWorkThreads.at(i);
		*/
	//delete cpNetThread;
	if(mpControlClient!=NULL)
		delete mpControlClient;
	if(pMemTask)
	{
		for(int i=0;i<pMemTask->DataPackages.size();i++)
		{
			free(pMemTask->DataPackages[i]->pckBody.lpBuffer);
			free(pMemTask->DataPackages[i]);
		}
		pMemTask->DataPackages.clear();
		pMemTask->PicPackages.clear();
		delete pMemTask;
	}
}

int CServer::StartServer()
{
	return mpNet->StartServer();
}

void CServer::WaitClient()
{
	/*
	struct sockaddr_in saClient; //地址信息   
	SOCKET sServer;



	CMSG_Server::GetCMSG_Server(pDlg)->AddMessage("loop listen client HAHA...\r\n",1,BLUE);

	int maxfdp=NULL;  
	struct fd_set fds;  
	struct timeval timeOut={1,1};

	HANDLE CompletionPort;
	char clientType[10] = {0};
	DWORD                dwBytesTransfered = 0;
	OVERLAPPED           *pOverlapped = NULL;
	LPPER_IO_OPERATION_DATA PerIoData = (LPPER_IO_OPERATION_DATA)malloc(sizeof(PER_IO_OPERATION_DATA));
	ZeroMemory(&(PerIoData->Overlapped), sizeof(OVERLAPPED));
    PerIoData->BytesSEND = 0;
    PerIoData->BytesRECV = 0;
    PerIoData->DataBuf.len = sizeof(PACKAGE);
    PerIoData->DataBuf.buf = new char[sizeof(PACKAGE)];
	PerIoData->OPERATION_TYPE = _RECV_;
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
			CMSG_Server::GetCMSG_Server(pDlg)->AddMessage("one client has connected us...\r\n",1,GREEN);
			
			IOCompeletionData *pIOCompeletionData = (IOCompeletionData*)malloc(sizeof(IOCompeletionData));
			pIOCompeletionData->mySocket = sServer;
			pIOCompeletionData->sin_addr = saClient.sin_addr;
			//pIOCompeletionData->type = _RECV_;
			
			CompletionPort = CreateIoCompletionPort(HANDLE(sServer), m_hIOCompletionPort, (DWORD)pIOCompeletionData, 0);
			WSARecv(sServer, &(PerIoData->DataBuf), 1, &RecvBytes, &Flags,&(PerIoData->Overlapped), NULL);
			sleep(0);
		}
	}
	
	//先注释掉
	//delete PerIoData->DataBuf.buf;
	//free(PerIoData);
	//以后再打开
	mpNet->setStop(true);
	for(int i=0;i<int(cpWorkThreads.size());i++)
		cpWorkThreads.at(i)->wait4ThisThread();
	//cpNetThread->wait4ThisThread();
	*/
	mpNet->WaitClient();
}
/*
bool CServer::AddClient(CClient *pClient,void *pParam)
{
	CDialog *pDlg = (CDialog*)pParam;
	mpLock->Lock();
	if(NULL == pClient)
	{
		mpLock->Unlock();
		return false;
	}
	CClient *tmpClient = mpClientHead;
	while(tmpClient!=NULL)
	{
		if(pClient->getClientIP()==tmpClient->getClientIP())
		{
			char outstr[1024] = {0};
			strcpy_s(outstr,pClient->getClientIP().c_str());
			strcat_s(outstr,"  has been in our list.......\r\n");

			CMSG_Server::GetCMSG_Server(pDlg)->AddMessage(outstr,1,GREEN);
			
			mpLock->Unlock();
			return false;
		}
		tmpClient = tmpClient->getNextClient();
	}

	if(mpClientHead == NULL){
		mpClientHead = pClient;
	}
	else{
		pClient->SetNextClient(mpClientHead);
		mpClientHead = pClient; 
	}
	mpLock->Unlock();
	return true;
}
*/

void CServer::CheckClient()
{
	/*
	while(!mbStop)
	{
		mpLock->Lock();
		CClient *pClient = mpClientHead;
		CClient *lastClient = NULL;
		while(pClient!=NULL)
		{
			if(!mpNet->HeartBeat(pClient))
			{
				char outstr[1024] = {0};
				strcpy_s(outstr,pClient->getClientIP().c_str());
				strcat_s(outstr," is droped.......\r\n");

				CMSG_Server::GetCMSG_Server(pDlg)->AddMessage(outstr,1,GREEN);
				
				if(lastClient==NULL)
					mpClientHead = pClient->getNextClient();
				else
				{
					lastClient->SetNextClient(pClient->getNextClient());
				}
				pClient = pClient->getNextClient();
			}
			else
			{
				char outstr[1024] = {0};
				strcpy_s(outstr,pClient->getClientIP().c_str());
				strcat_s(outstr," is checked ok.......\r\n");
				CMSG_Server::GetCMSG_Server(pDlg)->AddMessage(outstr,1,GREEN);
				lastClient = pClient;
				pClient = pClient->getNextClient();
			}
			Sleep(100);
		}
		mpLock->Unlock();
		//::PostMessageA(pDlg->GetSafeHwnd(),ID_MSG_STATE,0,(LPARAM)("into check sleep...\r\n"));
		sleep(10);
		//::PostMessageA(pDlg->GetSafeHwnd(),ID_MSG_STATE,0,(LPARAM)("next loop check...\r\n"));
	}
	*/
}
void CServer::setStop(bool singal)
{
	mbStop = singal;
	mpNet->setStop(singal);
}
void CServer::CloseServer()
{

}
void CServer::Data()
{
	CMSG_Server::GetCMSG_Server()->AddMessage("into data function...\r\n",1,GREEN);
	CData *pData = NULL;
	while(!mbStop)
	{
		switch(miChoice)
		{
		case CIRCLE:			pData = new CData(CIRCLE,20);
								setChoice(STOPDRAW);
								break;
		case SQUARE:			pData = new CData(SQUARE,20);
								setChoice(STOPDRAW);
								break;
		default:				break;
		}
		if(pData!=NULL)
		{
			LPPACKAGE pPackage = pData->GetPackages();
			for(int i=0;i<pData->GetPackNum();i++)
			{
				for(int j=0;j<int(mpDeviceClients.size());j++)
				{
					SendPackToNet(&(pPackage[i]),mpDeviceClients[j]);
				}
			}
			DataFree.push_back(pData);
		}
		pData = NULL;
	}

}
bool CServer::SendPackToNet(LPPACKAGE pPackage, CClient *pClient)
{
	LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
	char *DataPack = (char*)malloc(pPackage->pckHeader.dwLength-sizeof(tagPackageHeader));
	memcpy(&pOnePackage->pckHeader,&pPackage->pckHeader,sizeof(PACKAGE_HEADER));
	memcpy(DataPack,pPackage->pckBody.lpBuffer,pPackage->pckHeader.dwLength-sizeof(tagPackageHeader));
	pOnePackage->pckBody.lpBuffer = DataPack;
	if(pClient!=NULL)
	{
		mpNet->AddPack(pOnePackage,pClient);
		return true;
	}
	else
	{
		CMSG_Server::GetCMSG_Server()->AddMessage("Send package error: no this SOCKET...\r\n",1,GREEN);
		return false;
	}

}
void CServer::SendPackToNet(LPPACKAGE pPackage)
{
	/*CClient *pClient = mpClientHead;
	while(pClient!=NULL)
	{
		mpNet->AddPack(pPackage,pClient);
		pClient = pClient->getNextClient();
	}*/
	for(int i=0;i<int(mpDeviceClients.size());i++)
	{
		mpNet->AddPack(pPackage,mpDeviceClients[i]);
	}
}
void CServer::setChoice(int choice)
{
	miChoice = choice;
}
int CServer::getChoice()
{
	return miChoice;
}
/*
HANDLE CServer::getIOCompletionPort()
{
	return m_hIOCompletionPort;
}
*/
bool CServer::getStop()
{
	return mbStop;
}
void CServer::newConnentClient(CClient *pClient)
{
	int iLoop = 0;

	switch(pClient->getType())
	{
	case DEVICE_END:
		mpDeviceClients.push_back(pClient);
		mpNet->SendClientDataToControl(pClient,mpControlClient);
		CMSG_Server::GetCMSG_Server()->AddMessage("DEVICE_END has connected us...\r\n",1,GREEN);
		break;
	case CONTROL_END:
		if(mpControlClient!=NULL)
			delete mpControlClient;
		mpControlClient = pClient;
		for(iLoop=0;iLoop<int(mpDeviceClients.size());iLoop++)
			mpNet->SendClientDataToControl(mpDeviceClients[iLoop],mpControlClient);
		for(iLoop=0;iLoop<int(mpJetClients.size());iLoop++)
			mpNet->SendClientDataToControl(mpJetClients[iLoop],mpControlClient);
		CMSG_Server::GetCMSG_Server()->AddMessage("CONTROL_END has connected us...\r\n",1,GREEN);
		break;
	case JET_END:
		mpJetClients.push_back(pClient);
		mpNet->SendClientDataToControl(pClient,mpControlClient);
		CMSG_Server::GetCMSG_Server()->AddMessage("JET_END has connected us...\r\n",1,GREEN);
		break;
	default:
		break;
	}
}
void CServer::doTireState(LPPACKAGE onePackage, SOCKET mySocket)
{
	DATA_STATE	pState;
	tagTaskData mytagTaskData;
	memcpy(&mytagTaskData,onePackage->pckBody.lpBuffer,sizeof(tagTaskData));
	memcpy(&pState,(char*)onePackage->pckBody.lpBuffer+sizeof(tagTaskData),sizeof(DATA_STATE));
	if(pState.iTireState == STATE_READY)
	{
		for(int i=0;i<int(mpDeviceClients.size());i++)
		{
			if(mpDeviceClients[i]->getSocket()==mySocket)
			{
				mpDeviceClients[i]->setState(STATE_READY);
				if(mpDeviceClients[i]->getRelationClient()!=NULL)
				{
					SendPackToNet(onePackage,mpDeviceClients[i]->getRelationClient());
					//mpDeviceClients[i]->setState(STATE_DONE);
				}
				break;
			}
		}
	}
	else if(pState.iTireState == STATE_DONE)
	{
		for(int i=0;i<int(mpDeviceClients.size());i++)
		{
			if(mpDeviceClients[i]->getSocket()==mySocket)
			{
				mpDeviceClients[i]->setState(STATE_DONE);
				if(mpDeviceClients[i]->getRelationClient()!=NULL)
				{
					SendPackToNet(onePackage,mpDeviceClients[i]->getRelationClient());
					//mpDeviceClients[i]->setState(STATE_DONE);
				}
				break;
			}
		}
	}
	else if(pState.iTireState == STATE_RIGHTDONE)
	{
		for(int i=0;i<int(mpJetClients.size());i++)
		{
			if(mpJetClients[i]->getSocket()==mySocket)
			{
				mpJetClients[i]->setState(STATE_RIGHTDONE);
				if(mpJetClients[i]->getRelationClient()!=NULL)
				{
					SendPackToNet(onePackage,mpJetClients[i]->getRelationClient());
					//mpDeviceClients[i]->setState(STATE_DONE);
				}
				break;
			}
		}
	}
	else if(pState.iTireState == STATE_WRONGDONE)
	{
		for(int i=0;i<int(mpJetClients.size());i++)
		{
			if(mpJetClients[i]->getSocket()==mySocket)
			{
				mpJetClients[i]->setState(STATE_WRONGDONE);
				if(mpJetClients[i]->getRelationClient()!=NULL)
				{
					SendPackToNet(onePackage,mpJetClients[i]->getRelationClient());
					//mpDeviceClients[i]->setState(STATE_DONE);
				}
				break;
			}
		}
	}
}

void CServer::bindClient(SOCKET deviceSocket, SOCKET jetSocket)
{
	CClient *pDeviceClient = NULL, *pJetClient = NULL;
	for(int i=0;i<int(mpDeviceClients.size());i++)
	{
		if(mpDeviceClients[i]->getSocket()==deviceSocket)
		{
			pDeviceClient = mpDeviceClients[i];
			break;
		}
	}
	for(int i=0;i<int(mpJetClients.size());i++)
	{
		if(mpJetClients[i]->getSocket()==jetSocket)
		{
			pJetClient = mpJetClients[i];
			break;
		}
	}
	pDeviceClient->SetRelationClient(pJetClient);
	pJetClient->SetRelationClient(pDeviceClient);

	if(pDeviceClient->getState()==STATE_READY)
	{
		LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
		struct tagTaskData mytagTaskData;
		mytagTaskData.dwID = 0;
		mytagTaskData.dwPackNum = 1;
		mytagTaskData.dwTaskId = 0;
		pOnePackage->pckHeader.yType = TAG_STATUE_DATA;
		pOnePackage->pckHeader.dwLength = sizeof(struct tagTaskData)+sizeof(DATA_STATE) + sizeof(struct tagPackageHeader);
		DATA_STATE myDataState;
		myDataState.iTireState =  STATE_READY;
		char* DataPack = (char*)malloc(sizeof(struct tagTaskData)+sizeof(DATA_STATE));
		memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
		memcpy(DataPack+sizeof(mytagTaskData),&myDataState,sizeof(myDataState));
		pOnePackage->pckBody.lpBuffer = DataPack;

		SendPackToNet(pOnePackage,pJetClient);
	}
}
CClient* CServer::FindDeviceClient(SOCKET deviceSocket)
{
	for(int i=0;i<int(mpDeviceClients.size());i++)
	{
		if(mpDeviceClients[i]->getSocket()==deviceSocket)
			return mpDeviceClients[i];
	}
	return NULL;
}
CClient* CServer::FindJetClient(SOCKET jetSocket)
{
	for(int i=0;i<int(mpJetClients.size());i++)
	{
		if(mpJetClients[i]->getSocket()==jetSocket)
			return mpJetClients[i];
	}
	return NULL;
}

void CServer::RecvPackage(LPPACKAGE pOnePackage,IOCompeletionData *pIOCompeletionData)
{
	tagConnectionData mytagConnectionData;
	DATA_STATE	myDataState;		
	tagBindClientData myBindClientData;
	CClient *pClient = NULL;

	switch(pOnePackage->pckHeader.yType)
	{
		case TAG_CONNECTION_DATA:		
					memcpy(&mytagConnectionData,pOnePackage->pckBody.lpBuffer,sizeof(tagConnectionData));
					pClient = new CClient(pIOCompeletionData->sin_addr,pIOCompeletionData->mySocket,mytagConnectionData.yType);
					this->newConnentClient(pClient);
					CMSG_Server::GetCMSG_Server()->AddMessage("connectnation data...\r\n",1,GREEN);
					free(pOnePackage);
					break;
		case TAG_PRINT_DATA:
					//memcpy(pOnePackage->pckBody.lpBuffer,pOnePackage->pckBody.lpBuffer,sizeof(tagPrinterData));
					StorePackage(pOnePackage);
					CMSG_Server::GetCMSG_Server()->AddMessage("print data...\r\n",1,GREEN);
					//this->SendPackToNet(pOnePackage,this->FindDeviceClient(pOnePackage->pckHeader.dwDestination));
					break;

		case TAG_INSTRUCTION_DATA:
					//memcpy(pOnePackage->pckBody.lpBuffer,pOnePackage->pckBody.lpBuffer,pOnePackage->pckHeader.dwLength);
					AnalysisInstruction(pOnePackage);
					CMSG_Server::GetCMSG_Server()->AddMessage("instruction data...\r\n",1,GREEN);
					//free(pOnePackage);
					break;
			
		case TAG_STATUE_DATA:
					this->doTireState(pOnePackage,pIOCompeletionData->mySocket);
					CMSG_Server::GetCMSG_Server()->AddMessage("state data...\r\n",1,GREEN);
					free(pOnePackage);
					break;

		case TAG_BIND_JET_DEVICE:
					memcpy(&myBindClientData,pOnePackage->pckBody.lpBuffer,sizeof(tagBindClientData));
					this->bindClient(myBindClientData.iDeviceSocket,myBindClientData.iJetSocket);
					CMSG_Server::GetCMSG_Server()->AddMessage("BIND data...\r\n",1,GREEN);
					free(pOnePackage);
					break;
		case TAG_PICTURE_DATA:
					StorePackage(pOnePackage);
					
					break;
		case TAG_RESEND_DATA:
					ResendPackage(pOnePackage,pIOCompeletionData->mySocket);
					break;
		case TAG_REVISE_ZEOR_DATA:
					ReviseZero(pOnePackage,pIOCompeletionData->mySocket);
					break;
		default:	
					CMSG_Server::GetCMSG_Server()->AddMessage("error data...\r\n",1,GREEN);
					free(pOnePackage);
					break;
	}
}
void CServer::ReviseZero(LPPACKAGE pOnePackage,SOCKET fromSocket)
{
	for(int i=0;i<int(mpJetClients.size());i++)
	{
		if(mpJetClients[i]->getSocket()==fromSocket)
		{
			if(mpJetClients[i]->getRelationClient()!=NULL)
			{
				SendPackToNet(pOnePackage,mpJetClients[i]->getRelationClient());
			}
			break;
		}
	}
}
void CServer::StorePackage(LPPACKAGE pOnePackage)
{
	tagTaskData mytagTaskData;
	memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(tagTaskData));
	int TaskId = mytagTaskData.dwTaskId;

	mpLock->Lock();
	ofstream TaskFile;
	char FileName[128];
	sprintf_s(FileName,"%d",TaskId);
	if(isNewTask)
	{
		TaskFile.open(FileName,ios_base::binary);
	}
	else
	{
		TaskFile.open(FileName,ios::app|ios_base::binary);
	}
	char *HeadData = new char[sizeof(PACKAGE_HEADER)+1];
	memcpy(HeadData,&pOnePackage->pckHeader,sizeof(PACKAGE_HEADER));
	HeadData[sizeof(PACKAGE_HEADER)] = '\0';
	TaskFile.write(HeadData,sizeof(PACKAGE_HEADER));
	char *BodyData = new char[pOnePackage->pckHeader.dwLength+1-sizeof(tagPackageHeader)];
	memcpy(BodyData,pOnePackage->pckBody.lpBuffer,pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader));
	BodyData[pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader)] = '\0';
	TaskFile.write(BodyData,pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader));
	TaskFile.close();
	delete HeadData;
	delete BodyData;
	isNewTask = false;
	/*
	Task *pTask = mpTaskHead;
	while(pTask!=NULL)
	{
		if(pTask->TaskId==TaskId)
			break;
		pTask = pTask->next;
	}
	if(pTask!=NULL)
	{
		if(pOnePackage->pckHeader.yType==TAG_PRINT_DATA)
			pTask->DataPackages.push_back(pOnePackage);
		else if(pOnePackage->pckHeader.yType==TAG_PICTURE_DATA)
			pTask->PicPackages.push_back(pOnePackage);
	}
	else
	{
		pTask = new Task();
		pTask->TaskId = TaskId;
		if(pOnePackage->pckHeader.yType==TAG_PRINT_DATA)
			pTask->DataPackages.push_back(pOnePackage);
		else if(pOnePackage->pckHeader.yType==TAG_PICTURE_DATA)
			pTask->PicPackages.push_back(pOnePackage);
		pTask->next = mpTaskHead;
		mpTaskHead = pTask;
	}*/
	mpLock->Unlock();
}
void CServer::WritePic(Task *pTask)
{
	ofstream ofile;
	char filename[128];
	sprintf(filename,"%d",pTask->TaskId);
	strcat(filename,".jpg");
	ofile.open(filename,ios_base::binary|ios_base::out);
	LPVOID *PackData;
	int *PackLen;
	int PackNum = 0;
	for(int i=0;i<pTask->PicPackages.size();i++)
	{
		tagTaskData mytagTaskData;
		memcpy(&mytagTaskData,pTask->PicPackages[i]->pckBody.lpBuffer,sizeof(tagTaskData));
		int packId = mytagTaskData.dwID;
		if(i==0)
		{
			PackNum = mytagTaskData.dwPackNum;
			PackData = (LPVOID*)malloc(sizeof(LPVOID)*PackNum);
			PackLen = new int[PackNum];
		}
		PackData[packId] = pTask->PicPackages[i]->pckBody.lpBuffer;
		PackLen[packId] = pTask->PicPackages[i]->pckHeader.dwLength;
		//ofile.write((char*)(pTask->PicPackages[i]->pckBody.lpBuffer)+sizeof(tagTaskData),pTask->PicPackages[i]->pckHeader.dwLength-sizeof(tagTaskData));
	}
	for(int i=0;i<pTask->PicPackages.size()&&i<PackNum;i++)
	{
		ofile.write((char*)(PackData[i])+sizeof(tagTaskData),PackLen[i]-sizeof(tagTaskData)-sizeof(tagPackageHeader));
	}
	ofile.close();
}

void CServer::AnalysisInstruction(LPPACKAGE pOnePackage)
{
	tagTaskData mytagTaskData;
	memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(tagTaskData));
	int TaskId = mytagTaskData.dwTaskId;

	tagInstructionData mytagInstructionData;
	memcpy(&mytagInstructionData,(char*)(pOnePackage->pckBody.lpBuffer)+sizeof(tagTaskData),sizeof(mytagInstructionData));
	int iSocket = mytagInstructionData.iSocket;
	int times = mytagInstructionData.iTaskFinished_count;

	CClient *pClient = FindDeviceClient(iSocket);
	char FileName[128];
	sprintf_s(FileName,"%d",TaskId);
	ifstream TaskFile;
	TaskFile.open(FileName,ios::in|ios_base::binary);
	if(TaskFile)
	{
		mpLock->Lock();
		Task *pTask = LoadTaskFromFile(TaskFile);
		if(pTask!=NULL&&pClient!=NULL&&pClient->getRelationClient()!=NULL)
		{
			
			pTask->next = NULL;
			pTask->TaskId = TaskId;
		//WritePic(pTask);
			for(int j=0;j<int(pTask->DataPackages.size());j++)
			{
				SendPackToNet(pTask->DataPackages[j],pClient);
			}
			
			for(int j=0;j<int(pTask->PicPackages.size());j++)
			{
				SendPackToNet(pTask->PicPackages[j],pClient->getRelationClient());
			}
			SendPackToNet(pOnePackage,pClient);
			SendPackToNet(pOnePackage,pClient->getRelationClient());
		}
		mpLock->Unlock();
	}
	TaskFile.close();
	/*
	Task *pTask = mpTaskHead;

	while(pTask!=NULL)
	{
		if(pTask->TaskId==TaskId)
			break;
		pTask = pTask->next;
	}
	if(pTask!=NULL&&pClient!=NULL&&pClient->getRelationClient()!=NULL)
	{
		for(int j=0;j<int(pTask->DataPackages.size());j++)
		{
			SendPackToNet(pTask->DataPackages[j],pClient);
		}
		
		for(int j=0;j<int(pTask->PicPackages.size());j++)
		{
			SendPackToNet(pTask->PicPackages[j],pClient->getRelationClient());
		}
		SendPackToNet(pOnePackage,pClient);
		SendPackToNet(pOnePackage,pClient->getRelationClient());
	}*/
}

void CServer::DealClientOff(SOCKET OneSocket)
{
	vector<CClient*>::iterator itr = mpDeviceClients.begin();
	CClient *pClient = NULL;
	for(int i=0;i<int(mpDeviceClients.size());i++)
	{
		if(OneSocket==mpDeviceClients[i]->getSocket())
		{
			pClient = mpDeviceClients[i];
			mpDeviceClients.erase(itr);
			pClient->ClosethisSocket();
			pClient->setState(OFF_CLIENT);
			mpNet->SendClientDataToControl(pClient,mpControlClient);

			delete pClient;
			return;
		}
		++itr;
	}
	itr = mpJetClients.begin();
	for(int i=0;i<int(mpJetClients.size());i++)
	{
		if(OneSocket==mpJetClients[i]->getSocket())
		{
			pClient = mpJetClients[i];
			mpJetClients.erase(itr);
			pClient->ClosethisSocket();
			pClient->setState(OFF_CLIENT);
			mpNet->SendClientDataToControl(pClient,mpControlClient);
			delete pClient;
			return;
		}
		++itr;
	}
	if(mpControlClient)
	{
		if(mpControlClient->getSocket()==OneSocket)
		{
			pClient = mpControlClient;
			mpControlClient = NULL;
			pClient->ClosethisSocket();
			pClient->setState(OFF_CLIENT);
			delete pClient;
			return;
		}
	}
}

void CServer::ResendPackage(LPPACKAGE pOnePackage,SOCKET fromSocket)
{
	tagTaskData mytagTaskData;
	
	tagResendData myResendData;
	memcpy(&myResendData,pOnePackage->pckBody.lpBuffer,sizeof(tagResendData));
	int TaskId = myResendData.TaskId;
	int PackType = myResendData.PackType;
	int PackId = myResendData.PackId;


	char FileName[128];
	sprintf_s(FileName,"%d",TaskId);
	ifstream TaskFile;
	TaskFile.open(FileName,ios::in|ios_base::binary);
	mpLock->Lock();
	Task *pTask = LoadTaskFromFile(TaskFile);
	
	if(pTask==NULL)
	{
		SendPackToNet(pOnePackage,mpControlClient);
	}
	else
	{
		if(PackType==TAG_PICTURE_DATA)
		{
			if(PackId==-1)
			{
				for(int i=0;i<int(pTask->PicPackages.size());i++)
				{
					SendPackToNet(pTask->PicPackages[i],fromSocket);
				}
				return;
			}
			for(int i=0;i<int(pTask->PicPackages.size());i++)
			{
				memcpy(&mytagTaskData,pTask->PicPackages[i]->pckBody.lpBuffer,sizeof(mytagTaskData));
				if(mytagTaskData.dwID==PackId)
				{
					SendPackToNet(pTask->PicPackages[i],fromSocket);
					return;
				}
			}
		}
		else if(PackType==TAG_PRINT_DATA)
		{
			if(PackId==-1)
			{
				for(int i=0;i<int(pTask->DataPackages.size());i++)
				{
					SendPackToNet(pTask->DataPackages[i],fromSocket);
				}
				return;
			}

			for(int i=0;i<int(pTask->DataPackages.size());i++)
			{
				memcpy(&mytagTaskData,pTask->DataPackages[i]->pckBody.lpBuffer,sizeof(mytagTaskData));
				if(mytagTaskData.dwID==PackId)
				{
					SendPackToNet(pTask->DataPackages[i],fromSocket);
					return;
				}
			}
		}
	}
	SendPackToNet(pOnePackage,mpControlClient);
	mpLock->Unlock();
}

void CServer::SendPackToNet(LPPACKAGE pPackage, SOCKET OneSocket)
{
	CClient *pClient = FindDeviceClient(OneSocket);
	if(pClient!=NULL)
	{
		SendPackToNet(pPackage,pClient);
		return;
	}
	pClient = FindJetClient(OneSocket);
	if(pClient!=NULL)
	{
		SendPackToNet(pPackage,pClient);
		return;
	}
}
Task* CServer::LoadTaskFromFile(std::ifstream &FileTask)
{
	pMemTask->next = NULL;
	for(int i=0;i<pMemTask->DataPackages.size();i++)
	{
		free(pMemTask->DataPackages[i]->pckBody.lpBuffer);
		free(pMemTask->DataPackages[i]);
	}
	pMemTask->DataPackages.clear();
	pMemTask->PicPackages.clear();

	FileTask.seekg(0,ios::end);
	unsigned int ps = unsigned int(FileTask.tellg());
	char *szBuff = new char[ps];
	FileTask.seekg(0,ios::beg);
	FileTask.read(szBuff,ps);

	int Cnt = 0;
	while(Cnt<ps)
	{
		LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
		memcpy((char*)&pOnePackage->pckHeader,szBuff+Cnt,sizeof(PACKAGE_HEADER));
		Cnt += sizeof(PACKAGE_HEADER);
		char *DataPack = (char*)malloc(pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader));
		memcpy(DataPack,szBuff+Cnt,pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader));
		Cnt +=  pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader);
		pOnePackage->pckBody.lpBuffer = DataPack;
		if(pOnePackage->pckHeader.yType == TAG_PICTURE_DATA)
			pMemTask->PicPackages.push_back(pOnePackage);
		else if(pOnePackage->pckHeader.yType == TAG_PRINT_DATA)
			pMemTask->DataPackages.push_back(pOnePackage);
	}
	return pMemTask;
}

void CServer::sendTestData(int x, int y, int z)
{
	LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
	pOnePackage->pckHeader.dwLength = sizeof(tagPrinterData) + sizeof(tagPackageHeader);
	pOnePackage->pckHeader.yType = TAG_TEST_DATA;
	tagPrinterData PrinterData;
	PrinterData.XAxis = x;
	PrinterData.YAxis = y;
	PrinterData.ZAxis = z;

	char *DataPack = (char*)malloc(pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader));
	memcpy(DataPack,&PrinterData,sizeof(tagPrinterData));
	pOnePackage->pckBody.lpBuffer = DataPack;
	SendPackToNet(pOnePackage);
}