#include "stdafx.h"
#include "CClient.h"
//#include "controlClientDlg.h"
#include "GUI/dlgManager.h"
#include "cvPicture.h"
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
	LPPACKAGE lpPackage = GetPackageFromMem();
	lpPackage->pckHeader.dwLength = sizeof(PACKAGE_HEADER) + sizeof(tagBindClientData);
	lpPackage->pckHeader.yType = TAG_BIND_JET_DEVICE;
	lpPackage->pckHeader.wVerify_1 = 8721;
	lpPackage->pckHeader.wVerify_2 = 8721;

	tagBindClientData* pBindClientData			= (tagBindClientData*)(&(lpPackage->pckBody.lpBuffer));

	pBindClientData->iDeviceSocket = DeviceClient[i]->iSocket;
	pBindClientData->iJetSocket = JetClient[j]->iSocket;
	
	SendPackToServer(lpPackage);

	ShowMessage(CString(DeviceClient[i]->cClientIp)+CString("<-->")+CString(JetClient[j]->cClientIp));
}
void CClient::SendPackToServer(LPPACKAGE pOnePackage)
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	pMain->m_pTaskServer->m_pNet->SendToServer(pOnePackage);
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
		LPPACKAGE ptagPackage = GetPackageFromMem();
		
		tagTaskData *pTaskData = (tagTaskData*)&ptagPackage->pckBody;
		pTaskData->dwTaskId = TaskId;
		pTaskData->dwPackNum = ps%(TrueLen)==0?PackNum:PackNum+1;
		pTaskData->dwID = i;
		
		memcpy((char*)&ptagPackage->pckBody+sizeof(tagTaskData),&szBuff[i*TrueLen],TrueLen);
		ptagPackage->pckHeader.dwLength = BODY_LEN + sizeof(PACKAGE_HEADER);
		ptagPackage->pckHeader.yType = TAG_PICTURE_DATA;

		pTask->PicPackages.push_back(ptagPackage);
	}
	if(ps%(BODY_LEN-sizeof(tagTaskData))!=0)
	{
		LPPACKAGE ptagPackage = GetPackageFromMem();
		
		tagTaskData *pTaskData = (tagTaskData*)&ptagPackage->pckBody;
		pTaskData->dwTaskId = TaskId;
		pTaskData->dwPackNum = ps%(TrueLen)==0?PackNum:PackNum+1;
		pTaskData->dwID = i;

		memcpy((char*)&ptagPackage->pckBody+sizeof(tagTaskData),&szBuff[i*TrueLen],(ps%TrueLen)+sizeof(tagTaskData));
		ptagPackage->pckHeader.dwLength = (ps%TrueLen)+sizeof(tagTaskData) + sizeof(PACKAGE_HEADER);
		ptagPackage->pckHeader.yType = TAG_PICTURE_DATA;
		
		pTask->PicPackages.push_back(ptagPackage);

	}

	cvPic *pcvPic = new cvPic(TaskId,FilePathName.GetBuffer(0));
	vector<LPPACKAGE> DataPacks = pcvPic->getPrintPack(this);
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

	delete pTask;
}
void CClient::StoreTaskInFile(Task* pTask)
{
	ofstream TaskFile;
	char FileName[128];
	sprintf_s(FileName,"%d",pTask->TaskId);
	TaskFile.open(FileName,ios_base::binary);
	for(int i=0;i<pTask->DataPackages.size();i++)
	{
		TaskFile.write((char*)&pTask->DataPackages[i]->pckHeader,sizeof(PACKAGE_HEADER));
		TaskFile.write((char*)&pTask->DataPackages[i]->pckBody,pTask->DataPackages[i]->pckHeader.dwLength - sizeof(PACKAGE_HEADER));
		ReturnPackageToMem(pTask->DataPackages[i]);

	}
	for(int i=0;i<pTask->PicPackages.size();i++)
	{
		TaskFile.write((char*)&pTask->PicPackages[i]->pckHeader,sizeof(PACKAGE_HEADER));
		
		TaskFile.write((char*)&pTask->PicPackages[i]->pckBody,pTask->PicPackages[i]->pckHeader.dwLength - sizeof(PACKAGE_HEADER));
		ReturnPackageToMem(pTask->PicPackages[i]);
	}
	TaskFile.close();
}

Task* CClient::LoadTaskFromFile(std::ifstream &FileTask)
{
	//mpLock->Lock();
	pMemTask->next = NULL;
	
	for(int i=0;i<pMemTask->DataPackages.size();i++)
	{

	}
	pMemTask->DataPackages.clear();
	pMemTask->PicPackages.clear();
	//ReturnMsg2MemList
	FileTask.seekg(0,ios::end);
	unsigned int ps = unsigned int(FileTask.tellg());
	char *szBuff = new char[ps];
	FileTask.seekg(0,ios::beg);
	FileTask.read(szBuff,ps);

	int Cnt = 0;
	while(Cnt<ps)
	{
		LPPACKAGE pOnePackage = GetPackageFromMem();
		memcpy((char*)&pOnePackage->pckHeader,szBuff+Cnt,sizeof(PACKAGE_HEADER));
		Cnt += sizeof(PACKAGE_HEADER);
		memcpy(&pOnePackage->pckBody,szBuff+Cnt,pOnePackage->pckHeader.dwLength - sizeof(PACKAGE_HEADER));
		Cnt +=  pOnePackage->pckHeader.dwLength - sizeof(PACKAGE_HEADER);
		
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
				Sleep(10);
			}
			for(int i=0;i<int(pTask->DataPackages.size());i++)
			{
				char PackId[10];
				sprintf_s(PackId,"%d",i);
				ShowMessage(CString(PackId));
				SendPackToServer(pTask->DataPackages[i]);
				Sleep(10);
			}
		}
		else
		{
			ShowMessage(CString("No this Task"));
		}
	}
	TaskFile.close();

}
void CClient::SendInstruction(int Device,int ListId,int times)
{
	int thisTaskId = TaskIdVector[ListId];
	LPPACKAGE lpPackage = GetPackageFromMem();
	
	
	tagTaskData *pTaskData = (tagTaskData*)&lpPackage->pckBody;
	pTaskData->dwID = 0;
	pTaskData->dwPackNum = 1;
	pTaskData->dwTaskId = thisTaskId;

	tagInstructionData *pInstructionData = (tagInstructionData*)((char*)&lpPackage->pckBody + sizeof(tagTaskData));
	pInstructionData->iSocket = DeviceClient[Device]->iSocket;
	pInstructionData->iTaskFinished_count = times;
	
	lpPackage->pckHeader.yType = TAG_INSTRUCTION_DATA;
	lpPackage->pckHeader.dwLength = sizeof(tagTaskData)+sizeof(tagInstructionData) + sizeof(PACKAGE_HEADER);
	
	
	SendPackToServer(lpPackage);
}

void CClient::doClientData(LPPACKAGE pOnePackage)
{
	tagClientData *ptagClientData = (tagClientData*)malloc(sizeof(tagClientData));
	memcpy(ptagClientData,&pOnePackage->pckBody,sizeof(tagClientData));
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
	
	char FileName[128];
	sprintf_s(FileName,"%d",TaskId);
	ifstream TaskFile;
	TaskFile.open(FileName,ios::in|ios_base::binary);

	Task *pTask  = LoadTaskFromFile(TaskFile);

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
			LPPACKAGE lpPackage = NULL;
			for(int i=0;i<int(pTask->PicPackages.size());i++)
			{
				memcpy(&mytagTaskData,pTask->PicPackages[i]->pckBody.lpBuffer,sizeof(mytagTaskData));
				if(mytagTaskData.dwID==PackId)
				{
					lpPackage = pTask->PicPackages[i];
				}
				else
					ReturnPackageToMem(pTask->PicPackages[i]);
			}
			if(lpPackage!=NULL)
					SendPackToServer(lpPackage);
		}
		else if(PackType==TAG_PRINT_DATA)
		{
			if(PackId==-1)
			{
				for(int i=0;i<int(pTask->DataPackages.size());i++)
					SendPackToServer(pTask->DataPackages[i]);
				return;
			}
			LPPACKAGE lpPackage = NULL;
			for(int i=0;i<int(pTask->DataPackages.size());i++)
			{
				memcpy(&mytagTaskData,pTask->DataPackages[i]->pckBody.lpBuffer,sizeof(mytagTaskData));
				if(mytagTaskData.dwID==PackId)
				{
					lpPackage = pTask->DataPackages[i];
				}
				else
					ReturnPackageToMem(pTask->DataPackages[i]);
			}
			if(lpPackage!=NULL)
					SendPackToServer(lpPackage);
		}
	}
}

LPPACKAGE CClient::GetPackageFromMem()
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	LPPACKAGE lpPackage	= (LPPACKAGE)pMain->m_pTaskServer->m_lpPtrLargeMemList->RemoveHead( );
	return lpPackage;
}
void CClient::ReturnPackageToMem(LPPACKAGE lpPackage)
{
	CcontrolClientDlg* pMain = (CcontrolClientDlg*)m_pMain;
	pMain->m_pTaskServer->ReturnMsg2MemList(lpPackage);
}