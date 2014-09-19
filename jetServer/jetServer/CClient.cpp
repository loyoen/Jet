#include "StdAfx.h"
#include "CClient.h"
CClient::CClient(in_addr IP,SOCKET _Socket, int _type)
{
	ClientIP = IP;
	mySocket = _Socket;
	//nextClient = NULL;
	relationClient = NULL;
	type = _type;
	state = NEW_CLIENT;
	mpLock = new CLock();
}
CClient::~CClient()
{
	if(relationClient!=NULL)
	{
		relationClient->SetRelationClient(NULL);
	}
	delete mpLock;
}
in_addr CClient::getClientIP()
{
	return ClientIP;
}
SOCKET CClient::getSocket()
{
	return mySocket;
}
void CClient::ClosethisSocket()
{
	closesocket(mySocket);
	//cout<<"close"<<endl;
}
void CClient::SetRelationClient(CClient* next)
{
	relationClient = next;
}
CClient* CClient::getRelationClient()
{
	return relationClient;
}
int CClient::getType()
{
	return type;
}
void CClient::setState(int myState)
{
	state = myState;
}
int CClient::getState()
{
	return state;
}