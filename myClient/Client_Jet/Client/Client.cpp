
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <windows.h>   
#include "define.h"   
#include <fstream>
#include <iostream>
#include <map>
using namespace std;
#pragma comment( lib, "ws2_32.lib"  )
#define SERVER_PORT 1234 //�����˿�


void main()
{

	HANDLE Thread;
	DWORD dwThreadId;
	
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	SOCKET sClient; //�����׽���
	struct sockaddr_in saServer; //��ַ��Ϣ
	char *ptr;
	BOOL fSuccess = TRUE;
	//WinSock��ʼ��
	wVersionRequested = MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL�İ汾
	ret = WSAStartup(wVersionRequested, &wsaData);
	if(ret!=0)
	{
		printf("WSAStartup() failed!\n");
		return;
	}
	//ȷ��WinSock DLL֧�ְ汾2.2
	if(LOBYTE(wsaData.wVersion)!=2 || HIBYTE(wsaData.wVersion)!=2)
	{
		WSACleanup();
		printf("Invalid WinSock version!\n");
		return;
	}
	//����Socket,ʹ��TCPЭ��
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() failed!\n");
		return;
	}
	//������������ַ��Ϣ
	saServer.sin_family = AF_INET; //��ַ����
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�������
	saServer.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	//���ӷ�����
	ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("connect() failed!\n");
		closesocket(sClient); //�ر��׽���
		WSACleanup();
		return;
	}

	
	//  ptr = (char *)&packRecv;

	int m=1;
	

	PACKAGE onePackage;
	onePackage.pckHeader.yType = TAG_CONNECTION_DATA;
	onePackage.pckHeader.dwLength = sizeof(tagConnectionData);
	tagConnectionData mytagConnectionData;
	mytagConnectionData.yType = JET_END;
	memcpy(onePackage.pckBody.dwBody,&mytagConnectionData,sizeof(mytagConnectionData));


	char *ConnectBuf = new char[sizeof(tagPackageHeader)];
	memcpy(ConnectBuf,&(onePackage.pckHeader),sizeof(tagPackageHeader));
	send( sClient, ConnectBuf, sizeof(tagPackageHeader), 0 );
	send( sClient, (char*)onePackage.pckBody.dwBody,onePackage.pckHeader.dwLength , 0 );

	char *buf = new char[sizeof(tagPackage)];
	int Cnt = 0;
	int necs = 0;
	int PackNum = 0;
	ofstream ofile;
	tagPackage allBody[1000];
	while(1)
	{
		tagPackage ptagPackage;
		int HeadSize = 0;
		while(HeadSize<sizeof(tagPackageHeader))
		{
			HeadSize += recv(sClient,(char*)&(ptagPackage.pckHeader)+HeadSize,sizeof(tagPackageHeader)-HeadSize,0);
		}
	
		int BodySize = 0;
		while(BodySize<ptagPackage.pckHeader.dwLength)
		{
			BodySize += recv(sClient,(char*)ptagPackage.pckBody.dwBody+BodySize,ptagPackage.pckHeader.dwLength-BodySize,0);
		}
		printf("HeadSize = %d; BodySize=%d ; NeedSize = %d\n",HeadSize,BodySize,ptagPackage.pckHeader.dwLength);

		printf("%d\n",ptagPackage.pckHeader.yType);
		if(ptagPackage.pckHeader.yType == TAG_PICTURE_DATA)
		{
			if(PackNum==0)
			{
				char filename[128];
				sprintf(filename,"%d",ptagPackage.pckHeader.dwTaskId);
				strcat(filename,".jpg");
				ofile.open(filename,ios_base::binary|ios_base::out);
			}
			PackNum++;
			printf("ID: %d\n",ptagPackage.pckHeader.yID);
			memcpy(allBody[ptagPackage.pckHeader.yID].pckBody.dwBody,(char*)ptagPackage.pckBody.dwBody,ptagPackage.pckHeader.dwLength);
			memcpy(&(allBody[ptagPackage.pckHeader.yID].pckHeader),&(ptagPackage.pckHeader),sizeof(tagPackageHeader));
			//ofile.write((char*)ptagPackage.pckBody.dwBody,ptagPackage.pckHeader.dwLength);
			if(PackNum == ptagPackage.pckHeader.dwPackNum)
			{
				for(int i=0;i<PackNum;i++)
					ofile.write((char*)(allBody[i].pckBody.dwBody),allBody[i].pckHeader.dwLength);
				PackNum = 0;
				ofile.close();
			}
		}
	}


	if (ret == SOCKET_ERROR)
	{
		printf("send() failed!\n");
	}
	else
		printf("client info has been sent!");
	closesocket(sClient); //�ر��׽���
	WSACleanup();
}



