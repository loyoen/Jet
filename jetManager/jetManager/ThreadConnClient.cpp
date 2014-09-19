// ThreadClientConn.cpp : 实现文件
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
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadConnClient::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadConnClient, CWinThread)
END_MESSAGE_MAP()


// CThreadConnClient 消息处理程序
