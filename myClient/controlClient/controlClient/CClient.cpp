#include "stdafx.h"
#include "CClient.h"
#include "controlClientDlg.h"
#include "cvPicture.h"
void* ReadData(void *para)
{
	CClient *pClient = (CClient*)para;
	LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
	
	while(1)
	{
		int HeadSize = 0;
		while(HeadSize<sizeof(tagPackageHeader))
		{
			HeadSize += recv(pClient->sClient,(char*)&(pOnePackage->pckHeader)+HeadSize,sizeof(tagPackageHeader)-HeadSize,0);
		}
		DWORD BodySize = 0;
		char *DataPack = (char*)malloc(pOnePackage->pckHeader.dwLength);
		pOnePackage->pckBody.lpBuffer = DataPack;
		while(BodySize<pOnePackage->pckHeader.dwLength)
		{
			BodySize += recv(pClient->sClient,(char*)(pOnePackage->pckBody.lpBuffer)+BodySize,pOnePackage->pckHeader.dwLength-BodySize,0);
		}
		//printf("type = %d\n",onePackage.pckHeader.yType);
		if(pOnePackage->pckHeader.yType == TAG_CLIENT_DATA)
		{
			pClient->doClientData(pOnePackage);
			
		}
		else if(pOnePackage->pckHeader.yType == TAG_RESEND_DATA)
		{
			pClient->doResendData(pOnePackage);
		}
	}
}

CClient::CClient()
{
	TaskId = 0;
	strcpy(TaskIdFileName,"TaskIdFile");
	TaskId = getCurrentTaskId(TaskIdFileName);
	pMemTask = new Task();
}
int CClient::getCurrentTaskId(char filename[])
{
	ifstream TaskIdFile;
	int TaskId = 0;
	TaskIdFile.open(filename);
	string line;
	if(TaskIdFile)
	{
		getline(TaskIdFile,line);
		if(line.length()>0)
			TaskId = atoi(line.c_str());
		else
			TaskId = 0;
	}
	else
	{
		TaskId = 0;
	}
	TaskIdFile.close();
	ofstream TaskFile;
	TaskFile.open(filename);
	char cTaskId[128] = {0};
	sprintf_s(cTaskId,"%d",TaskId+1);
	TaskFile.write(cTaskId,strlen(cTaskId));
	TaskFile.close();
	return TaskId;
}
void CClient::InitClient()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	struct sockaddr_in saServer;											//地址信息
	BOOL fSuccess = TRUE;
	
	//WinSock初始化
	wVersionRequested = MAKEWORD( 2, 2 );									//希望使用的WinSock DLL的版本
	ret = WSAStartup( wVersionRequested, &wsaData );
	if ( ret != 0 )
	{
		printf( "WSAStartup() failed!\n" );
		Sleep( 2000 );
		return;
	}

	//确认WinSock DLL支持版本2.2
	if ( LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
	{
		printf( "Invalid WinSock version!\n" );
		Sleep( 2000 );
		WSACleanup();
		return;
	}

	//创建Socket,使用TCP协议
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( sClient == INVALID_SOCKET )
	{
		WSACleanup();
		printf( "socket() failed!\n" );
		Sleep(2000);
		return;
	}

	//构建服务器地址信息
	saServer.sin_family = AF_INET;											//地址家族
	saServer.sin_port = htons(SERVER_PORT);									//注意转化为网络节序
	saServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

	//连接服务器
	ret = connect( sClient, (struct sockaddr *)&saServer, sizeof(saServer) );
	if ( ret == SOCKET_ERROR )
	{
		printf( "connect() failed!\n" );
		Sleep( 2000 );	
		closesocket( sClient );												//关闭套接字
		WSACleanup();
		return;
	}

	int count = 0;
	PACKAGE onePackage;
	onePackage.pckHeader.yType = TAG_CONNECTION_DATA;
	onePackage.pckHeader.dwLength = sizeof(tagConnectionData);
	tagConnectionData mytagConnectionData;
	mytagConnectionData.yType = CONTROL_END;
	char* DataPack = (char*)malloc(sizeof(mytagConnectionData));
	memcpy(DataPack,&mytagConnectionData,sizeof(mytagConnectionData));
	onePackage.pckBody.lpBuffer = DataPack;
	SendPackToServer(&onePackage);
	//
	CreateThread(0,0,LPTHREAD_START_ROUTINE(ReadData),this,0,0);
}
void CClient::RemoveDevice(int i)
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	pMain->RemoveDevice(i);
}
void CClient::RemoveJet(int i)
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	pMain->RemoveJet(i);
}
void CClient::AddDeviceInfo(CString strMessage)
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	if( m_pMain!=NULL )
	{
		pMain->AddDeviceEnd(strMessage);
		TRACE( strMessage+_T("\n") );
	}	
}

void CClient::AddJetInfo(CString strMessage)
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	if( m_pMain!=NULL )
	{
		pMain->AddJetEnd(strMessage);
		TRACE( strMessage+_T("\n") );
	}
}
void CClient::AddTaskInfo(CString strMessage,int Column)
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	if( m_pMain!=NULL )
	{
		pMain->AddTask(strMessage,Column);
		TRACE( strMessage+_T("\n") );
	}
}
void CClient::ShowMessage(CString strMessage)
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	if( m_pMain!=NULL )
	{
		pMain->AddInformation(strMessage);
		TRACE( strMessage+_T("\n") );
	}
}
void CClient::SendBindInfo(int i,int j)
{
	PACKAGE onePackage;
	onePackage.pckHeader.dwLength = sizeof(tagBindClientData);

	tagBindClientData mytagBindClientData;
	mytagBindClientData.iDeviceSocket = DeviceClient[i]->iSocket;
	mytagBindClientData.iJetSocket = JetClient[j]->iSocket;
	onePackage.pckHeader.yType = TAG_BIND_JET_DEVICE;
	char *DataPack = (char*)malloc(sizeof(mytagBindClientData));
	memcpy(DataPack,&mytagBindClientData,sizeof(mytagBindClientData));
	onePackage.pckBody.lpBuffer = DataPack;
	//memcpy(ConnectBuf,&onePackage,sizeof(PACKAGE));
	
	SendPackToServer(&onePackage);

	ShowMessage(CString(DeviceClient[i]->cClientIp)+CString("<-->")+CString(JetClient[j]->cClientIp));
}
void CClient::SendPackToServer(LPPACKAGE pOnePackage)
{
	char *Buf = new char[sizeof(tagPackageHeader)];
	memcpy(Buf,&(pOnePackage->pckHeader),sizeof(tagPackageHeader));
	int HeadSize = 0;
	while(HeadSize<sizeof(tagPackageHeader))
	{
		HeadSize += send( sClient, Buf+HeadSize, sizeof(tagPackageHeader)-HeadSize, 0 );
	}
	Sleep(10);
	DWORD BodySize = 0;
	while(BodySize<pOnePackage->pckHeader.dwLength)
	{
		BodySize += send( sClient, (char*)pOnePackage->pckBody.lpBuffer+BodySize,pOnePackage->pckHeader.dwLength-BodySize , 0 );
	}
	Sleep(10);
	delete Buf;
}
void CClient::LoadPic(CString FilePathName)
{
	ifstream file;
	file.open(FilePathName.GetBuffer(0),ios_base::binary|ios_base::in);
	file.seekg(0,ios::end);
	unsigned int ps = unsigned int(file.tellg());
	char *szBuff = new char[ps];
	file.seekg(0,ios::beg);
	file.read(szBuff,ps);

	Task *pTask = new Task();
	pTask->TaskId = TaskId;
		
	pTask->next = mpTaskHead;
	mpTaskHead = pTask;
	int TrueLen = BODY_LEN-sizeof(tagTaskData);
	int PackNum = ps/TrueLen;
	int i=0;	
	for(i=0;i<PackNum;i++)
	{
		LPPACKAGE ptagPackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
		char *DataPack = (char*)malloc(BODY_LEN);
		tagTaskData mytagTaskData;
		mytagTaskData.dwTaskId = TaskId;
		mytagTaskData.dwPackNum = ps%(TrueLen)==0?PackNum:PackNum+1;
		mytagTaskData.dwID = i;
		memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));

		ptagPackage->pckHeader.dwLength = BODY_LEN;
		ptagPackage->pckHeader.yType = TAG_PICTURE_DATA;
		memcpy(DataPack+sizeof(tagTaskData),&szBuff[i*TrueLen],TrueLen);
		ptagPackage->pckBody.lpBuffer = DataPack;
		pTask->PicPackages.push_back(ptagPackage);
	}
	if(ps%(BODY_LEN-sizeof(tagTaskData))!=0)
	{
		LPPACKAGE ptagPackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
		char *DataPack = (char*)malloc((ps%TrueLen)+sizeof(tagTaskData));
		tagTaskData mytagTaskData;
		mytagTaskData.dwTaskId = TaskId;
		mytagTaskData.dwPackNum = ps%(TrueLen)==0?PackNum:PackNum+1;
		mytagTaskData.dwID = i;
		ptagPackage->pckHeader.dwLength = (ps%TrueLen)+sizeof(tagTaskData);
		ptagPackage->pckHeader.yType = TAG_PICTURE_DATA;
		memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
		memcpy(DataPack+sizeof(tagTaskData),&szBuff[i*TrueLen],(ps%TrueLen)+sizeof(tagTaskData));
		ptagPackage->pckBody.lpBuffer = DataPack;
		pTask->PicPackages.push_back(ptagPackage);

	}
	cvPic *pcvPic = new cvPic(TaskId,FilePathName.GetBuffer(0));
	vector<LPPACKAGE> DataPacks = pcvPic->getPrintPack();
	pTask->DataPackages = DataPacks;

	StoreTaskInFile(pTask);

	char cTask[10];
	sprintf_s(cTask,"%d",TaskId);
	TaskIdVector.push_back(TaskId);
	AddTaskInfo(CString(cTask)+CString("\t")+FilePathName,0);


	ofstream TaskFile;
	TaskFile.open(TaskIdFileName);
	char cTaskId[128] = {0};
	sprintf_s(cTaskId,"%d",TaskId+1);
	TaskFile.write(cTaskId,strlen(cTaskId));
	TaskFile.close();

	TaskId++;
}
void CClient::StoreTaskInFile(Task* pTask)
{
	ofstream TaskFile;
	char FileName[128];
	sprintf_s(FileName,"%d",pTask->TaskId);
	TaskFile.open(FileName,ios_base::binary);
	char *HeadData = new char[sizeof(PACKAGE_HEADER)+1];
	for(int i=0;i<pTask->DataPackages.size();i++)
	{
		memcpy(HeadData,&pTask->DataPackages[i]->pckHeader,sizeof(PACKAGE_HEADER));
		HeadData[sizeof(PACKAGE_HEADER)] = '\0';
		TaskFile.write(HeadData,sizeof(PACKAGE_HEADER));
		char *BodyData = new char[pTask->DataPackages[i]->pckHeader.dwLength+1];
		memcpy(BodyData,pTask->DataPackages[i]->pckBody.lpBuffer,pTask->DataPackages[i]->pckHeader.dwLength);
		BodyData[pTask->DataPackages[i]->pckHeader.dwLength] = '\0';
		TaskFile.write(BodyData,pTask->DataPackages[i]->pckHeader.dwLength);
		delete BodyData;
	}
	for(int i=0;i<pTask->PicPackages.size();i++)
	{
		memcpy(HeadData,&pTask->PicPackages[i]->pckHeader,sizeof(PACKAGE_HEADER));
		HeadData[sizeof(PACKAGE_HEADER)] = '\0';
		TaskFile.write(HeadData,sizeof(PACKAGE_HEADER));
		char *BodyData = new char[pTask->PicPackages[i]->pckHeader.dwLength+1];
		memcpy(BodyData,pTask->PicPackages[i]->pckBody.lpBuffer,pTask->PicPackages[i]->pckHeader.dwLength);
		BodyData[pTask->PicPackages[i]->pckHeader.dwLength] = '\0';
		TaskFile.write(BodyData,pTask->PicPackages[i]->pckHeader.dwLength);
		delete BodyData;
	}
	delete HeadData;
	TaskFile.close();
	delete pTask;
}

Task* CClient::LoadTaskFromFile(std::ifstream &FileTask)
{
	//mpLock->Lock();
	pMemTask->next = NULL;
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
		char *DataPack = (char*)malloc(pOnePackage->pckHeader.dwLength);
		memcpy(DataPack,szBuff+Cnt,pOnePackage->pckHeader.dwLength);
		Cnt +=  pOnePackage->pckHeader.dwLength;
		pOnePackage->pckBody.lpBuffer = DataPack;
		if(pOnePackage->pckHeader.yType == TAG_PICTURE_DATA)
			pMemTask->PicPackages.push_back(pOnePackage);
		else if(pOnePackage->pckHeader.yType == TAG_PRINT_DATA)
			pMemTask->DataPackages.push_back(pOnePackage);
	}
	//mpLock->Unlock();
	return pMemTask;
}

void CClient::SendPic(int ListId)
{
	int myTaskId = TaskIdVector[ListId];

	char FileName[128];
	sprintf_s(FileName,"%d",myTaskId);
	ifstream TaskFile;
	TaskFile.open(FileName,ios::in|ios_base::binary);
	if(TaskFile)
	{
		Task *pTask = LoadTaskFromFile(TaskFile);
		if(pTask!=NULL)
		{
			pTask->TaskId = myTaskId;
			pTask->next = NULL;
			for(int i=0;i<int(pTask->PicPackages.size());i++)
			{
				char PackId[10];
				sprintf_s(PackId,"%d",i);
				ShowMessage(CString(PackId));
				SendPackToServer(pTask->PicPackages[i]);
			}
			for(int i=0;i<int(pTask->DataPackages.size());i++)
			{
				char PackId[10];
				sprintf_s(PackId,"%d",i);
				ShowMessage(CString(PackId));
				SendPackToServer(pTask->DataPackages[i]);
			}
		}
		else
		{
			ShowMessage(CString("No this Task"));
		}
	}
	TaskFile.close();
	//ptagPackage.pckHeader.dwDestination = JetClient[JetList.at(0)]->iSocket;;
	//ptagPackage.pckHeader.dwTaskId = TaskId;

}
void CClient::SendInstruction(int Device,int ListId,int times)
{
	int thisTaskId = TaskIdVector[ListId];
	tagPackage ptagPackage;
	tagInstructionData mytagInstructionData;
	mytagInstructionData.iSocket = DeviceClient[Device]->iSocket;
	mytagInstructionData.iTaskFinished_count = times;
	//ptagPackage.pckHeader.dwDestination = DeviceClient[Device]->iSocket;
	char* DataPack = (char*)malloc(sizeof(tagTaskData)+sizeof(mytagInstructionData));
	tagTaskData mytagTaskData;
	mytagTaskData.dwID = 0;
	mytagTaskData.dwPackNum = 1;
	mytagTaskData.dwTaskId = thisTaskId;
	memcpy(DataPack,&mytagTaskData,sizeof(tagTaskData));

	ptagPackage.pckHeader.yType = TAG_INSTRUCTION_DATA;
	ptagPackage.pckHeader.dwLength = sizeof(tagTaskData)+sizeof(mytagInstructionData);
	
	memcpy(DataPack+sizeof(tagTaskData),&mytagInstructionData,sizeof(mytagInstructionData));
	ptagPackage.pckBody.lpBuffer = DataPack;
	SendPackToServer(&ptagPackage);
}

void CClient::doClientData(LPPACKAGE pOnePackage)
{
	tagClientData *ptagClientData = (tagClientData*)malloc(sizeof(tagClientData));
	memcpy(ptagClientData,pOnePackage->pckBody.lpBuffer,sizeof(tagClientData));
	vector<tagClientData*>::iterator itr;
	if(ptagClientData->iType == DEVICE_END)
	{
		if(ptagClientData->iClientState==NEW_CLIENT)
		{
			DeviceClient.push_back(ptagClientData);
			AddDeviceInfo(CString(ptagClientData->cClientIp));
		}
		else if(ptagClientData->iClientState==STATE_READY||ptagClientData->iClientState==STATE_DONE)
		{
			itr = DeviceClient.begin();
			for(int i=0;i<int(DeviceClient.size());i++)
			{
				if(DeviceClient[i]->iSocket==ptagClientData->iSocket)
					break;
			}
			DeviceClient.push_back(ptagClientData);
			AddDeviceInfo(CString(ptagClientData->cClientIp));
		}
		else if(ptagClientData->iClientState==OFF_CLIENT)
		{
			itr = DeviceClient.begin();
			for(int i=0;i<int(DeviceClient.size());i++)
			{
				if(DeviceClient[i]->iSocket==ptagClientData->iSocket)
				{
					RemoveDevice(i);
					DeviceClient.erase(itr);
					break;
				}
				++itr;
			}
		}
	}
	else if(ptagClientData->iType == JET_END)
	{
		if(ptagClientData->iClientState==NEW_CLIENT)
		{
			JetClient.push_back(ptagClientData);
			AddJetInfo(CString(ptagClientData->cClientIp));
		}
		else if(ptagClientData->iClientState==OFF_CLIENT)
		{
			itr = JetClient.begin();
			for(int i=0;i<int(JetClient.size());i++)
			{
				if(JetClient[i]->iSocket==ptagClientData->iSocket)
				{
					RemoveJet(i);
					JetClient.erase(itr);
					break;
				}
				itr++;
			}
		}
	}
}
void CClient::doResendData(LPPACKAGE pOnePackage)
{
	tagTaskData mytagTaskData;

	tagResendData myResendData;
	memcpy(&myResendData,pOnePackage->pckBody.lpBuffer,sizeof(tagResendData));
	int TaskId = myResendData.TaskId;
	int PackType = myResendData.PackType;
	int PackId = myResendData.PackId;
	Task *pTask = mpTaskHead;
	while(pTask!=NULL)
	{
		if(pTask->TaskId==TaskId)
			break;
		pTask = pTask->next;
	}
	if(pTask==NULL)
	{
		return;
	}
	else
	{
		if(PackType==TAG_PICTURE_DATA)
		{
			if(PackId==-1)
			{
				for(int i=0;i<int(pTask->PicPackages.size());i++)
					SendPackToServer(pTask->PicPackages[i]);
				return;
			}
			for(int i=0;i<int(pTask->PicPackages.size());i++)
			{
				memcpy(&mytagTaskData,pTask->PicPackages[i]->pckBody.lpBuffer,sizeof(mytagTaskData));
				if(mytagTaskData.dwID==PackId)
				{
					SendPackToServer(pTask->PicPackages[i]);
					return;
				}
			}
		}
		else if(PackType==TAG_PICTURE_DATA)
		{
			if(PackId==-1)
			{
				for(int i=0;i<int(pTask->DataPackages.size());i++)
					SendPackToServer(pTask->DataPackages[i]);
				return;
			}
			for(int i=0;i<int(pTask->DataPackages.size());i++)
			{
				memcpy(&mytagTaskData,pTask->DataPackages[i]->pckBody.lpBuffer,sizeof(mytagTaskData));
				if(mytagTaskData.dwID==PackId)
				{
					SendPackToServer(pTask->DataPackages[i]);
					return;
				}
			}
		}
	}
}