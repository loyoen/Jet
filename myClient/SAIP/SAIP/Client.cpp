#include "Client.h"
#include <fstream>
#include <iostream>
CClient::CClient()
{
	mpLock = new CLock();
	pMemTask = new Task();
}
void CClient::InitClient()
{
	HANDLE Thread;
	DWORD dwThreadId;
	
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	struct sockaddr_in saServer; //地址信息
	char *ptr;
	BOOL fSuccess = TRUE;
	//WinSock初始化
	wVersionRequested = MAKEWORD(2, 2); //希望使用的WinSock DLL的版本
	ret = WSAStartup(wVersionRequested, &wsaData);
	if(ret!=0)
	{
		printf("WSAStartup() failed!\n");
		return;
	}
	//确认WinSock DLL支持版本2.2
	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)
	{
		WSACleanup();
		printf("Invalid WinSock version!\n");
		return;
	}
	//创建Socket,使用TCP协议
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() failed!\n");
		return;
	}
	//构建服务器地址信息
	saServer.sin_family = AF_INET; //地址家族
	saServer.sin_port = htons(SERVER_PORT); //注意转化为网络节序
	saServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	//连接服务器
	ret = SOCKET_ERROR;
	while(ret == SOCKET_ERROR)
	{
		ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
		Sleep(500);
	}
	
}
void CClient::SendTypeToServer()
{
	PACKAGE onePackage;
	onePackage.pckHeader.yType = TAG_CONNECTION_DATA;
	onePackage.pckHeader.dwLength = sizeof(tagConnectionData) + sizeof(tagPackageHeader);
	tagConnectionData mytagConnectionData;
	mytagConnectionData.yType = JET_END;
	char* DataPack = (char*)malloc(sizeof(tagConnectionData));
	memcpy(DataPack,&mytagConnectionData,sizeof(mytagConnectionData));
	onePackage.pckBody.lpBuffer = DataPack;
	SendPackToServer(&onePackage);
}
void CClient::LoopRecv()
{
	while(1)
	{
		LPPACKAGE ptagPackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
		int HeadSize = 0;
		while(HeadSize<sizeof(tagPackageHeader))
		{
			HeadSize += recv(sClient,(char*)&(ptagPackage->pckHeader)+HeadSize,sizeof(tagPackageHeader)-HeadSize,0);
		}
	
		int BodySize = 0;
		char* DataPack = (char*)malloc(ptagPackage->pckHeader.dwLength - sizeof(tagPackageHeader));
		ptagPackage->pckBody.lpBuffer = DataPack;
		while(BodySize<ptagPackage->pckHeader.dwLength-sizeof(tagPackageHeader))
		{
			BodySize += recv(sClient,(char*)ptagPackage->pckBody.lpBuffer+BodySize,ptagPackage->pckHeader.dwLength-sizeof(tagPackageHeader)-BodySize,0);
		}
		printf("HeadSize = %d; BodySize=%d ; NeedSize = %d\n",HeadSize,BodySize,ptagPackage->pckHeader.dwLength-sizeof(tagPackageHeader));

		printf("%d\n",ptagPackage->pckHeader.yType);
		if(ptagPackage->pckHeader.yType == TAG_PICTURE_DATA)
		{
			StoreInTask(ptagPackage);
		}
		else if(ptagPackage->pckHeader.yType == TAG_INSTRUCTION_DATA)
		{
			mpLock->Lock();
			Instructions.push(ptagPackage);
			mpLock->Unlock();
		}
		else if(ptagPackage->pckHeader.yType == TAG_STATUE_DATA)
		{
			dealTireData(ptagPackage);
		}
	}

	closesocket(sClient); //关闭套接字
	WSACleanup();
}
void CClient::dealRevise(int &x,int &y)
{
	x = 0;
	y = 0;
}
void CClient::dealTireData(LPPACKAGE pPackage)
{
	DATA_STATE	myDataState;
	tagTaskData mytagTaskData;
	memcpy(&mytagTaskData,pPackage->pckBody.lpBuffer,sizeof(tagTaskData));
	memcpy(&myDataState,(char*)pPackage->pckBody.lpBuffer+sizeof(tagTaskData),sizeof(DATA_STATE));
	if(myDataState.iTireState == STATE_READY)
	{
		int x=0,y=0;
		dealRevise(x,y);
		PACKAGE onePackage;
		onePackage.pckHeader.yType = TAG_REVISE_ZEOR_DATA;
		onePackage.pckHeader.dwLength = sizeof(tagReviseZeorData) + sizeof(tagPackageHeader);
		tagReviseZeorData myReviseZeorData;
		myReviseZeorData.iX = x;
		myReviseZeorData.iY = y;

		char* DataPack = (char*)malloc(sizeof(tagReviseZeorData));
		memcpy(DataPack,&myReviseZeorData,sizeof(tagReviseZeorData));
		onePackage.pckBody.lpBuffer = DataPack;
		SendPackToServer(&onePackage);
	}
	else if(myDataState.iTireState == STATE_DONE)
	{
		bool bCheck = checkPrint(mytagTaskData.dwTaskId);
		PACKAGE onePackage;
		onePackage.pckHeader.yType = TAG_STATUE_DATA;
		onePackage.pckHeader.dwLength = sizeof(tagTaskData)+sizeof(DATA_STATE) + sizeof(tagPackageHeader);
		myDataState.iTireState =  bCheck==true?STATE_RIGHTDONE:STATE_WRONGDONE;
		char* DataPack = (char*)malloc(sizeof(tagTaskData)+sizeof(DATA_STATE));
		memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
		memcpy(DataPack+sizeof(mytagTaskData),&myDataState,sizeof(myDataState));
		onePackage.pckBody.lpBuffer = DataPack;

		SendPackToServer(&onePackage);
	}
}
bool CClient::checkPrint(int taskId)
{
	return true;
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
	int BodySize = 0;
	while(BodySize<pOnePackage->pckHeader.dwLength-sizeof(tagPackageHeader))
	{
		BodySize += send( sClient, (char*)pOnePackage->pckBody.lpBuffer+BodySize,pOnePackage->pckHeader.dwLength-BodySize-sizeof(tagPackageHeader) , 0 );
	}
	Sleep(10);
	delete Buf;
}
void CClient::StoreInTask(LPPACKAGE pOnePackage)
{
	tagTaskData mytagTaskData;
	memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(tagTaskData));
	int TaskId = mytagTaskData.dwTaskId;
	
	mpLock->Lock();
	ofstream TaskFile;
	char FileName[128];
	sprintf_s(FileName,"%d",TaskId);
	TaskFile.open(FileName,ios::app|ios_base::binary);
	
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
	mpLock->Unlock();
	free(pOnePackage);
}
void CClient::DealInstruction()
{
	tagTaskData mytagTaskData;
	while(1)
	{
		if(!Instructions.empty())
		{
			mpLock->Lock();
			LPPACKAGE pOnePackage = Instructions.front();
			mpLock->Unlock();
			memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(tagTaskData));
			int TaskId = mytagTaskData.dwTaskId;

			char FileName[128];
			sprintf_s(FileName,"%d",TaskId);
			ifstream TaskFile;
			TaskFile.open(FileName,ios::in|ios_base::binary);
			if(TaskFile)
			{
				Task *pTask = LoadTaskFromFile(TaskFile);
				pTask->next = NULL;
				pTask->TaskId = TaskId;
				if(IntackTask(pTask,TaskId))
				{
					mpLock->Lock();
					Instructions.pop();
					mpLock->Unlock();
					WritePic(pTask);
					Sleep(1000);
					// 指令解析
				}
				else
				{
					Sleep(2000);
				}
			}
			TaskFile.close();
		}
	}	
}

void CClient::WritePic(Task *pTask)
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
Task* CClient::FindTask(int TaskId)
{
	for(int i=0;i<Tasks.size();i++)
	{
		if(Tasks[i]->TaskId==TaskId)
		{
			return Tasks[i];
		}
	}
	return NULL;
}
bool CClient::IntackTask(Task *pTask,int TaskId)
{
	tagTaskData mytagTaskData;
	if(pTask==NULL)
	{
		AskPackage(TaskId,TAG_PICTURE_DATA,-1);
		return false;
	}
	bool isIntack = true;
	int PackNum = 0;
	int *Visited;
	for(int i=0;i<pTask->PicPackages.size();i++)
	{
		memcpy(&mytagTaskData,pTask->PicPackages[i]->pckBody.lpBuffer,sizeof(mytagTaskData));
		if(i==0)
		{
			PackNum = mytagTaskData.dwPackNum;
			Visited = new int[PackNum];
			memset(Visited,0,PackNum*sizeof(int));
		}
		if(mytagTaskData.dwID>PackNum)
			continue;
		else
			Visited[mytagTaskData.dwID] = 1;
	}
	for(int i=0;i<PackNum;i++)
	{
		if(Visited[i]==0)
		{
			AskPackage(pTask->TaskId,TAG_PICTURE_DATA,i);
			isIntack = false;
		}
	}
	return isIntack;
}

void CClient::AskPackage(int TaskId,int PackType,int PackId)
{
	tagResendData mytagResendData;
	mytagResendData.TaskId = TaskId;
	mytagResendData.PackType = PackType;
	mytagResendData.PackId = PackId;

	PACKAGE myPackage;
	tagTaskData mytagTaskData;

	mytagTaskData.dwPackNum = 1;
	myPackage.pckHeader.yType = TAG_RESEND_DATA;
	mytagTaskData.dwTaskId = TaskId;
	myPackage.pckHeader.dwLength = sizeof(tagTaskData)+sizeof(tagResendData)+sizeof(tagPackageHeader);
	mytagTaskData.dwID = 0;
	char* DataPack = (char*)malloc(sizeof(tagTaskData)+sizeof(tagResendData));
	memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
	memcpy(DataPack+sizeof(mytagTaskData),&mytagResendData,sizeof(mytagResendData));
	myPackage.pckBody.lpBuffer = DataPack;
	SendPackToServer(&myPackage);
}
Task* CClient::LoadTaskFromFile(std::ifstream &FileTask)
{
	mpLock->Lock();
	pMemTask->next = NULL;
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
	}
	mpLock->Unlock();
	return pMemTask;
}