
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <windows.h>   
#include "package.h"   
#include <fstream>
#include <iostream>
using namespace std;
#pragma comment( lib, "ws2_32.lib"  )

void SendPackToServer(LPPACKAGE pOnePackage,SOCKET sClient)
{
	char *Buf = (char*)malloc(sizeof(struct tagPackageHeader));
	memcpy(Buf,&(pOnePackage->pckHeader),sizeof(struct tagPackageHeader));
	int HeadSize = 0;
	while(HeadSize<sizeof(struct tagPackageHeader))
	{
		HeadSize += send( sClient, Buf+HeadSize, sizeof(struct tagPackageHeader)-HeadSize, 0 );
	}
	Sleep(10);
	int BodySize = 0;
	while(BodySize<pOnePackage->pckHeader.dwLength - sizeof(struct tagPackageHeader))
	{
		BodySize += send( sClient, (char*)pOnePackage->pckBody.lpBuffer+BodySize,pOnePackage->pckHeader.dwLength-BodySize - sizeof(struct tagPackageHeader) , 0 );
	}
	Sleep(10);
	free(Buf);
}
void SendTypeToServer(SOCKET sClient)
{
	PACKAGE onePackage;
	onePackage.pckHeader.yType = TAG_CONNECTION_DATA;
	onePackage.pckHeader.dwLength = sizeof(struct tagConnectionData) + sizeof(struct tagPackageHeader);
	struct tagConnectionData mytagConnectionData;
	mytagConnectionData.yType = DEVICE_END;
	char* DataPack = (char*)malloc(sizeof(struct tagConnectionData));
	memcpy(DataPack,&mytagConnectionData,sizeof(mytagConnectionData));
	onePackage.pckBody.lpBuffer = DataPack;
	SendPackToServer(&onePackage,sClient);
}


/*
 * Stress the transmit queue -- send a large number of TCP packets
 */

 
int singal = 1;
void  action(void * arg,int fd1)
{		
		struct tagPrinterData *ptagPrinterData = (struct tagPrinterData *)arg;
    
    
		int x = ptagPrinterData->XAxis,y=ptagPrinterData->YAxis,z=ptagPrinterData->ZAxis;
		int color = ptagPrinterData->cColorSelect;
		int printTime = ptagPrinterData->iTime;
		int bContinuous = ptagPrinterData->bContinuous;
		//int x=10,y=10,z=10;
		//printf("x=%d,y=%d,z=%d,color=%d",x,y,z,color);
		int max = abs(x)<abs(y)?abs(y):abs(x);
		max = max<abs(z)?abs(z):max;
}

void InitZero()
{
}
void WritePackInFile(LPPACKAGE pOnePackage)
{
	struct tagTaskData mytagTaskData;
	memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(struct tagTaskData));
	//printf("taskid = %d; packNum = %d; id = %d\n",mytagTaskData.dwTaskId,mytagTaskData.dwPackNum,mytagTaskData.dwID);
	char cTaskId[16];
	sprintf(cTaskId,"%d",mytagTaskData.dwTaskId);
	FILE *fp;
	fp = fopen(cTaskId,"ab+");
	fwrite(&pOnePackage->pckHeader,sizeof(struct tagPackageHeader),1,fp);
	fwrite(pOnePackage->pckBody.lpBuffer,pOnePackage->pckHeader.dwLength - sizeof(struct tagPackageHeader),1,fp);
	fclose(fp);
	//printf("write end\n");
}
struct OrderStruct
{
	int TaskId;
	struct OrderStruct* next;
};
struct Task
{
	int TaskId;
	int PackNum;
	LPPACKAGE pAllPackages[1000];
};

struct OrderStruct *HeadOrder = NULL;
struct OrderStruct *TailOrder = NULL;

void SendPauseToServer()
{
}
void SendRestartToServer()
{
}
void SendPrintEndToServer(int TaskId,SOCKET sClient)
{
	PACKAGE onePackage;
	struct tagTaskData mytagTaskData;
	mytagTaskData.dwID = 0;
	mytagTaskData.dwPackNum = 1;
	mytagTaskData.dwTaskId = TaskId;
	onePackage.pckHeader.yType = TAG_STATUE_DATA;
	onePackage.pckHeader.dwLength = sizeof(struct tagTaskData)+sizeof(DATA_STATE) + sizeof(struct tagPackageHeader);
	DATA_STATE myDataState;
	myDataState.iTireState =  STATE_DONE;
	char* DataPack = (char*)malloc(sizeof(struct tagTaskData)+sizeof(DATA_STATE));
	memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
	memcpy(DataPack+sizeof(mytagTaskData),&myDataState,sizeof(myDataState));
	onePackage.pckBody.lpBuffer = DataPack;

	SendPackToServer(&onePackage,sClient);
	free(DataPack);
}
void doResive(int x,int y)
{
	printf("doResive is starting\n");
}
void doTask(struct Task *pTask)
{
	printf("task %d is starting\n",pTask->TaskId);
	int i = 0;
	LONG LastX=0,LastY=0,LastZ=0;
	for(i=0;i<pTask->PackNum;i++)
	{
		struct tagPrinterData myPrinterData;
		int j = 0;
		for(j=0;j<(pTask->pAllPackages[i]->pckHeader.dwLength-sizeof(struct tagTaskData) - sizeof(struct tagPackageHeader))/sizeof(struct tagPrinterData);j++)
		{
			memcpy(&myPrinterData,(char*)pTask->pAllPackages[i]->pckBody.lpBuffer+sizeof(struct tagTaskData)+j*sizeof(struct tagPrinterData),sizeof(struct tagPrinterData));
			//printf("px = %d, py = %d, pz = %d\n",myPrinterData.XAxis,myPrinterData.YAxis,myPrinterData.ZAxis);
			myPrinterData.XAxis -= LastX;
			myPrinterData.YAxis -= LastY;
			myPrinterData.ZAxis -= LastZ;
			
			//action(&myPrinterData,fd1);
			
			LastX += myPrinterData.XAxis;
			LastY += myPrinterData.YAxis;
			LastZ += myPrinterData.ZAxis;
		}
	}
	
	for(i=0;i<pTask->PackNum;i++)
	{
		free(pTask->pAllPackages[i]->pckBody.lpBuffer);
		free(pTask->pAllPackages[i]);
	}
	free(pTask);
	
	printf("free end\n");
}

struct Task* LoadTaskFromFile(int TaskId)
{
	struct Task *pTask = (struct Task*)malloc(sizeof(struct Task));
	char cTask[16] = {0};
	int AskMemForTask = -1;
	sprintf(cTask,"%d",TaskId);
	FILE *fp;
	fp = fopen(cTask,"rb");
	if(!fp)
	{
		// ask send again
		return NULL;
	}
	while(!feof(fp))
	{	
		printf("loop\n");
		LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
		printf("befor head\n");
		fread(&pOnePackage->pckHeader,sizeof(struct tagPackageHeader),1,fp);
		printf("after head\n");
		if(feof(fp))
			break;
		char *DataPack = (char*)malloc(pOnePackage->pckHeader.dwLength - sizeof(struct tagPackageHeader));
		fread(DataPack,pOnePackage->pckHeader.dwLength-sizeof(struct tagPackageHeader),1,fp);
		pOnePackage->pckBody.lpBuffer = DataPack;
		struct tagTaskData mytagTaskData;
		memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(struct tagTaskData));
		
		printf("Load ID %d\n",mytagTaskData.dwID);
		pTask->pAllPackages[mytagTaskData.dwID] = pOnePackage;
		
		printf("load one package %d %d \n",TaskId,mytagTaskData.dwPackNum);

		pTask->TaskId = TaskId;  
		pTask->PackNum = mytagTaskData.dwPackNum;
		printf("endloop\n");
	}
	printf("end task\n");
	fclose(fp);
	return pTask;
}


int checkTask(struct Task* pTask)
{
	return 1;
}
void SendReadyToServer(SOCKET sClient)
{
	PACKAGE onePackage;
	struct tagTaskData mytagTaskData;
	mytagTaskData.dwID = 0;
	mytagTaskData.dwPackNum = 1;
	mytagTaskData.dwTaskId = 0;
	onePackage.pckHeader.yType = TAG_STATUE_DATA;
	onePackage.pckHeader.dwLength = sizeof(struct tagTaskData)+sizeof(DATA_STATE) + sizeof(struct tagPackageHeader);
	DATA_STATE myDataState;
	myDataState.iTireState =  STATE_READY;
	char* DataPack = (char*)malloc(sizeof(struct tagTaskData)+sizeof(DATA_STATE));
	memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
	memcpy(DataPack+sizeof(mytagTaskData),&myDataState,sizeof(myDataState));
	onePackage.pckBody.lpBuffer = DataPack;

	SendPackToServer(&onePackage,sClient);
	free(DataPack);
}
void main()
{

	HANDLE Thread;
	DWORD dwThreadId;
	
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	SOCKET sClient; //连接套接字
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
	while(ret==SOCKET_ERROR)
		ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));


	int m=1;
	

	SendTypeToServer(sClient);

		printf("send type end\n");
	SendReadyToServer(sClient);
	printf("send ready end\n");
			
	int Cnt = 0;
	int necs = 0;
	int TireState = 0;//-1
	//struct tagPrinterData PrintData[1024*10]; 
	int CurrentTask = -1;
	while(1)
	{
		struct OrderStruct *pOrder = HeadOrder;
		if(pOrder!=NULL&&TireState==1)
		{
			struct Task* pTask = LoadTaskFromFile(pOrder->TaskId);
			printf("load succ\n");
			if(checkTask(pTask)==1)
			{
				printf("check succ\n");
				
				//SendPauseToServer();
				printf("loop succ\n");
				doTask(pTask);
				printf("do task succ\n");
				CurrentTask = pTask->TaskId;
				TireState = 0; // correct later
				SendPrintEndToServer(pTask->TaskId,sClient);  // check print right or not
				SendRestartToServer();
			}
			
		}
		
		LPPACKAGE pOnePackage = (LPPACKAGE)malloc(sizeof(PACKAGE));
		int HeadSize = 0;
		while(HeadSize<sizeof(struct tagPackageHeader))
		{
			HeadSize += recv(sClient,(char*)&(pOnePackage->pckHeader)+HeadSize,sizeof(struct tagPackageHeader)-HeadSize,0);
		}
		int BodySize = 0;
		char *DataPack = (char*)malloc(pOnePackage->pckHeader.dwLength- sizeof(struct tagPackageHeader));
		pOnePackage->pckBody.lpBuffer = DataPack;
		while(BodySize<pOnePackage->pckHeader.dwLength- sizeof(struct tagPackageHeader))
		{
			BodySize += recv(sClient,(char*)(pOnePackage->pckBody.lpBuffer)+BodySize,pOnePackage->pckHeader.dwLength-BodySize - sizeof(struct tagPackageHeader),0);
		}
		if(pOnePackage->pckHeader.yType==TAG_PRINT_DATA)
		{
			WritePackInFile(pOnePackage);
			free(pOnePackage->pckBody.lpBuffer);
			free(pOnePackage);
			/*
			struct tagTaskData mytagTaskData;
			memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(struct tagTaskData));
			necs = mytagTaskData.dwPackNum;
			Cnt += (pOnePackage->pckHeader.dwLength-sizeof(struct tagTaskData))/sizeof(struct tagPrinterData);
			printf("%d  nece = %d\n",Cnt, necs);
			memcpy(&(PrintData[mytagTaskData.dwID*((BODY_LEN-sizeof(struct tagTaskData))/sizeof(struct tagPrinterData))]),(char*)(pOnePackage->pckBody.lpBuffer)+sizeof(struct tagTaskData),pOnePackage->pckHeader.dwLength-sizeof(struct tagTaskData));
			int i=0;
			for(i=0;i<(pOnePackage->pckHeader.dwLength-sizeof(struct tagTaskData))/sizeof(struct tagPrinterData);i++)
			{
				struct tagPrinterData ptagPrinterData;
				memcpy(&ptagPrinterData,(char*)(pOnePackage->pckBody.lpBuffer)+sizeof(struct tagTaskData)+(i*sizeof(struct tagPrinterData)),sizeof(struct tagPrinterData));
				printf("x=%d\ty=%d\tcolor=%d\n",ptagPrinterData.XAxis,ptagPrinterData.YAxis,ptagPrinterData.cColorSelect);
			}
			*/
		}
		else if(pOnePackage->pckHeader.yType==TAG_INSTRUCTION_DATA)
		{
			/*
			printf("type = %d\n",pOnePackage->pckHeader.yType);
			int i = 0;
			int fd1=open("/dev/screwV2",0);
			action(&PrintData[0],fd1);
			LONG LastX = PrintData[0].XAxis;
			LONG LastY = PrintData[0].YAxis;
			LONG LastZ = PrintData[0].ZAxis;
			for(i=1;i<Cnt;i++)
			{
				PrintData[i].XAxis -= LastX;
				PrintData[i].YAxis -= LastY;
				PrintData[i].ZAxis -= LastZ;
				action(&PrintData[i],fd1);
				LastX += PrintData[i].XAxis;
				LastY += PrintData[i].YAxis;
				LastZ += PrintData[i].ZAxis;
			}
			close(fd1);
			Cnt = 0;
			*/
			struct tagTaskData mytagTaskData;
			struct tagInstructionData myInstructionData;
			memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(struct tagTaskData));
			memcpy(&myInstructionData,(char*)pOnePackage->pckBody.lpBuffer+sizeof(struct tagTaskData),sizeof(struct tagInstructionData));
			int iOrderLoop=0;
			for(iOrderLoop=0;iOrderLoop<myInstructionData.iTaskFinished_count;iOrderLoop++)
			{
				struct OrderStruct *pTmpOrder = (struct OrderStruct*)malloc(sizeof(struct OrderStruct));
				pTmpOrder->next = NULL;
				pTmpOrder->TaskId = mytagTaskData.dwTaskId;
				if(HeadOrder==NULL)
				{
					TailOrder = pTmpOrder;
					HeadOrder = pTmpOrder;
				}
				else
				{
					TailOrder->next = pTmpOrder;
					TailOrder = pTmpOrder;
				}
			}
		}
		else if(pOnePackage->pckHeader.yType==TAG_REVISE_ZEOR_DATA)
		{
			printf("revise start\n");
			tagReviseZeorData ReviseZeroData;
			memcpy(&ReviseZeroData,pOnePackage->pckBody.lpBuffer,sizeof(ReviseZeroData));
			doResive(ReviseZeroData.iX,ReviseZeroData.iY);
			TireState = 1;
			printf("revise end\n");
			
		}
		else if(pOnePackage->pckHeader.yType==TAG_STATUE_DATA)
		{
			// tire state , judge which way to go
			printf("recv state start\n");
			struct tagTaskData mytagTaskData;
			DATA_STATE myDataState;
			memcpy(&mytagTaskData,pOnePackage->pckBody.lpBuffer,sizeof(mytagTaskData));
			memcpy(&myDataState,(char*)pOnePackage->pckBody.lpBuffer+sizeof(struct tagTaskData),sizeof(myDataState));
			if(myDataState.iTireState==STATE_RIGHTDONE)
			{
				HeadOrder = HeadOrder->next;
				free(pOrder);
			}
			SendReadyToServer(sClient);
			printf("recv state end\n");
		}
		else if(pOnePackage->pckHeader.yType==TAG_TEST_DATA)
		{
		
		}
		/*
		PicNode packRecv;
		memset(&packRecv, 0, sizeof(packRecv)); 
		
		recv(sClient,(char *)&packRecv,sizeof(packRecv),0);
		int len=packRecv.len;
		
		ofstream ofile;
		ofile.open("d:\\test3.jpg",ios_base::binary|ios_base::out);
		ofile.write(packRecv.pic,len);
		ofile.close();
		*/
		m++;
	}
	closesocket(sClient); //关闭套接字
	WSACleanup();
}



