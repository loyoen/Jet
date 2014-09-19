// ThreadConnServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "eMarker.h"
#include "ThreadConnServer.h"

#include "ClientConnServer.h"
#include "NetServer.h"

#ifndef	WM_EXIT_THREAD
#define	WM_EXIT_THREAD		WM_USER + 300
#endif
// CThreadConnServer

IMPLEMENT_DYNCREATE(CThreadConnServer, CWinThread)

//==================================================//
//													//
//					�� �� �� ��						//
//													//
//==================================================//

BOOL CThreadConnServer::Connect( SOCKADDR * lpSockAddr, int nSockAddrLen )
{
	return	m_pClient->OpenConnection( lpSockAddr, nSockAddrLen );
}

BOOL CThreadConnServer::Send( LPPACKAGE lpPackage )
{
	return	m_pClient->Send( lpPackage );
}

void CThreadConnServer::OnReceivePackage( LPPACKAGE lpPackage )
{
}

void CThreadConnServer::EndThread( )
{
	::PostThreadMessage( m_nThreadID, WM_EXIT_THREAD, (WPARAM)0, (LPARAM)0 ); 
}

void CThreadConnServer::SetNetServer( CNetServer* pServer )
{
	m_pNetServer	= pServer;
}

CThreadConnServer::CThreadConnServer()
{
	m_pClient		= new CClientConnServer(this);
	m_pNetServer	= NULL;

	GetRunTimeClass
}

CThreadConnServer::~CThreadConnServer()
{
	delete	m_pClient;
}

BOOL CThreadConnServer::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��

	return TRUE;
}

int CThreadConnServer::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadConnServer, CWinThread)
	ON_THREAD_MESSAGE( WM_EXIT_THREAD, OnAppExitMessage )
END_MESSAGE_MAP()


// CThreadConnServer ��Ϣ�������

//==================================================//
//													//
//					�� �� �� ��						//
//													//
//==================================================//

void CThreadConnServer::OnAppExitMessage(WPARAM wParam, LPARAM lParam)
{
	AfxEndThread( 801 );
}
