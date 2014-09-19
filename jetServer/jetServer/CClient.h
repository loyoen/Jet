#ifndef CCLIENT_H
#define CCLIENT_H
#include <string>
#include <iostream>
#include <WinSock2.h>
#include "CLock.h"
#include "package.h"


using namespace std;
class CClient
{
private:
	in_addr ClientIP;
	SOCKET mySocket;
	//CClient *nextClient;
	CClient *relationClient;
	int		type;
	int		state;
public:
	CLock *mpLock;
	CClient(in_addr IP,SOCKET _Socket, int _type);
	~CClient();
	in_addr getClientIP();
	SOCKET getSocket();
	void ClosethisSocket();
	void SetRelationClient(CClient* next);
	CClient* getRelationClient();
	int getType();
	void setState(int myState);
	int getState();
};


#endif