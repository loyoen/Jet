#ifndef CSERVER_H
#define CSERVER_H
#include "CData.h"
#include "CNet.h"
#include "CLock.h"
#include "CThread.h"
#include "CMSG_Server.h"
#include <vector>
#include <fstream>
#include <iostream>
#define sleep(x) Sleep((x)*1000) 
#define STOPDRAW	0

using namespace std;

struct Task
{
	//CLock				*pLock;
	int					TaskId;
	vector<LPPACKAGE>	DataPackages;
	vector<LPPACKAGE>	PicPackages;
	Task*				next;
};

class CServer
{
public:
	//CClient			*mpClientHead;
	vector<CClient*>	mpDeviceClients;
	vector<CClient*>	mpJetClients;
	CClient				*mpControlClient;
	CNet				*mpNet;

	Task				*mpTaskHead;

	//SOCKET			sListen;
	CLock				*mpLock;
	bool				mbStop;
	//vector<CThread*>	cpWorkThreads;
	//CThread			*cpNetThread;
	int					miChoice;
	vector<CData*>		DataFree;
	bool				isNewTask;
	Task				*pMemTask;
	//HANDLE			m_hIOCompletionPort;           // 完成端口的句柄
	//CDialog				*pDlg;
	//vector<SOCKET> mClientSocket;
public:
	CServer();
	~CServer();
	int StartServer();
	void WaitClient();
	void Data();
	bool SendPackToNet(LPPACKAGE pPackage,CClient *pClient);
	void SendPackToNet(LPPACKAGE pPackage);
	void SendPackToNet(LPPACKAGE pPackage,SOCKET OneSocket);
	//bool AddClient(CClient* myClient);
	void CheckClient();
	void CloseServer();
	void setStop(bool singal);
	void setChoice(int choice);
	int	 getChoice();
	//HANDLE getIOCompletionPort();
	bool getStop();
	void newConnentClient(CClient *pClient);
	void doTireState(LPDATA_STATE pState, LPPACKAGE onePackage, SOCKET mySocket);
	void bindClient(SOCKET deviceSocket,SOCKET jetSocket);

	CClient* FindDeviceClient(SOCKET deviceSocket);
	CClient* FindJetClient(SOCKET deviceSocket);

	void RecvPackage(LPPACKAGE pOnePackage,IOCompeletionData *pIOCompeletionData);
	void StorePackage(LPPACKAGE pOnePackage);
	void AnalysisInstruction(LPPACKAGE pOnePackage);
	void DealClientOff(SOCKET OneSocket);
	void ResendPackage(LPPACKAGE pOnePackage,SOCKET fromSocket);
	Task* LoadTaskFromFile(ifstream &FileTask);
	void WritePic(Task *pTask);
	void sendTestData(int x,int y,int z);
};
#endif