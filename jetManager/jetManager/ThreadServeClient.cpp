// ThreadChannelService.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "eMarker.h"
#include "ThreadServeClient.h"


// CThreadServeClient

IMPLEMENT_DYNCREATE(CThreadServeClient, CWinThread)

CThreadServeClient::CThreadServeClient()
{
}

CThreadServeClient::~CThreadServeClient()
{
}

BOOL CThreadServeClient::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CThreadServeClient::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadServeClient, CWinThread)
END_MESSAGE_MAP()


// CThreadServeClient ��Ϣ�������
