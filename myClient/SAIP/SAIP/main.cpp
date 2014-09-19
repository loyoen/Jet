#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <string.h>
#include <windows.h>   
#include "package.h"   
#include <fstream>
#include <iostream>
#include <map>
#include "Client.h"
#include "CThread.h"
using namespace std;
#pragma comment( lib, "ws2_32.lib"  )

void LoopRecv(void* param)
{
	CClient *pClient = (CClient*)param;
	pClient->LoopRecv();
}

void DealInstructions(void* param)
{
	CClient *pClient = (CClient*)param;
	pClient->DealInstruction();
}
void main()
{
	CClient *pClient = new CClient();
	pClient->InitClient();
	pClient->SendTypeToServer();
	CThread *RecvThread = new CThread(LoopRecv,pClient);
	CThread *InstructionThread = new CThread(DealInstructions,pClient);
	RecvThread->wait4ThisThread();
	InstructionThread->wait4ThisThread();
}



