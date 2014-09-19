#ifndef CLIENT_H
#define CLIENT_H
#include "Task.h"
#include "CLock.h"
#include <queue>
#include "Task.h"
using namespace std;
#pragma comment( lib, "ws2_32.lib"  )
class CClient
{
private:
	vector<Task*>		Tasks;
	SOCKET					sClient;
	queue<LPPACKAGE>	Instructions;
	CLock						*mpLock;
	Task						*pMemTask;
public:
	CClient();
	void InitClient();
	void SendTypeToServer();
	void SendPackToServer(LPPACKAGE pOnePackage);
	void LoopRecv();
	void StoreInTask(LPPACKAGE pOnePackage);
	void DealInstruction();
	Task*	FindTask(int TaskId);
	bool IntackTask(Task *pTask,int TaskId);
	void AskPackage(int TaskId,int PackType,int PackId);
	void WritePic(Task *pTask);
	Task* LoadTaskFromFile(std::ifstream &FileTask);
	void dealTireData(LPPACKAGE pPackage);
	void dealRevise(int &x,int &y);
	bool checkPrint(int taskId);
};
#endif