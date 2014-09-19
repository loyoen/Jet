#ifndef CCLIENT_H
#define CCLIENT_H
#include "package.h"
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

struct Task
{
	//CLock				*pLock;
	int					TaskId;
	vector<LPPACKAGE>	DataPackages;
	vector<LPPACKAGE>	PicPackages;
	Task*				next;
};

class CClient
{
public:
	vector<tagClientData*>	DeviceClient;
	vector<tagClientData*>	JetClient;
	CDialog*							m_pMain;
	SOCKET							sClient;
	int									TaskId;
	Task								*mpTaskHead;
	Task								*pMemTask;
	char								TaskIdFileName[128];
	vector<int>						TaskIdVector;
public:
	CClient();
	void InitClient();
	void SetMainDlg( CDialog* p ) { m_pMain=p; }
	void SendBindInfo(int i,int j);
	void ShowMessage(CString strMessage);
	void AddDeviceInfo(CString strMessage);
	void AddJetInfo(CString strMessage);
	void AddTaskInfo(CString strMessage,int Column);
	void SendPic(int ListId);
	void LoadPic(CString FilePathName);
	void SendInstruction(int Device,int ListId,int times);
	void SendPackToServer(LPPACKAGE pOnePackage);
	void RemoveDevice(int i);
	void RemoveJet(int i);
	void doClientData(LPPACKAGE pOnePackage);
	void doResendData(LPPACKAGE pOnePackage);
	int getCurrentTaskId(char filename[]);
	void StoreTaskInFile(Task* pTask);
	Task* LoadTaskFromFile(ifstream &FileTask);
};


#endif