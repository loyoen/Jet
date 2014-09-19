// ThreadChannelService.cpp : 实现文件
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
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadServeClient::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadServeClient, CWinThread)
END_MESSAGE_MAP()


// CThreadServeClient 消息处理程序
