// ThreadClientConn.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "eMarker.h"
#include "ThreadConnClient.h"


// CThreadConnClient

IMPLEMENT_DYNCREATE(CThreadConnClient, CWinThread)

CThreadConnClient::CThreadConnClient()
{
}

CThreadConnClient::~CThreadConnClient()
{
}

BOOL CThreadConnClient::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CThreadConnClient::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadConnClient, CWinThread)
END_MESSAGE_MAP()


// CThreadConnClient ��Ϣ�������
