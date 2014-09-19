#include "StdAfx.h"
#include "netserver.h"
#include "..\ClientConnServer.h"
#include "..\ThreadTaskServer.h"

//==================================================//
//													//
//					�� �� �� ��						//
//													//
//==================================================//

BOOL CNetServer::StartNetServer( )
{
	return	TRUE;
}

BOOL CNetServer::StopNetServer( )
{
	return	TRUE;
}

//void CNetServer::AddServer( CString strAddr, u_short sinport )
//{
//	LPSOCKADDR_IN		addrNewServer;
//
//	addrNewServer	= new SOCKADDR_IN;
//
//	addrNewServer->sin_family		= AF_INET;
//	addrNewServer->sin_addr.s_addr	= inet_addr( strAddr );
//	addrNewServer->sin_port			= sinport;
//
//	m_listPtrServer.AddTail( addrNewServer );
//	m_yServerCnt++;
//}
//
//void CNetServer::AddOperator( CString strAddr, u_short sinport )
//{
//	LPSOCKADDR_IN		addrNewOperator;
//
//	addrNewOperator	= new SOCKADDR_IN;
//
//	addrNewOperator->sin_family		= AF_INET;
//	addrNewOperator->sin_addr.s_addr= inet_addr( strAddr );
//	addrNewOperator->sin_port		= sinport;
//
//	m_listPtrOperator.AddTail( addrNewOperator );
//	m_yOperatorCnt++;
//}

void CNetServer::SetMainServer( CString strAddr, u_short sinport )
{
	m_addrMainServer.sin_family		= AF_INET;
	m_addrMainServer.sin_addr.s_addr= inet_addr( strAddr );
	m_addrMainServer.sin_port		= htons(sinport);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[Description]
	������������������
	1. ��������������������
	2. �����뱸�ݷ�����������
	3. ������Operator������
-----------------------------------------------------------------------*/
BOOL CNetServer::Connect( )
{
//	sockaddr*	addr	= (sockaddr*)&m_addrMainServer;

	// ���ȳ���������������
	m_bServerConnected	= m_pClientConnServer->Connect( (sockaddr*)&m_addrMainServer, sizeof(m_addrMainServer) );

	if ( !m_bServerConnected )
		return	FALSE;
 //   
	//// ������������ʧ�ܺ������������ݷ�����
	//while ( !m_bServerConnected )
	//{
	//	// ���������������еı��÷�����
	//	break;
	//}

	//// ����Operator
	//while ( !m_bOperatorConnected )
	//{
	//	break;
	//}

	return	TRUE;
}

BOOL CNetServer::SendToServer( LPPACKAGE lpPackage )
{
	return	m_pClientConnServer->Send( lpPackage );
}

BOOL CNetServer::SendLogoutPackage( LPPACKAGE lpPackage )
{
	return	m_pClientConnServer->SendLogout( lpPackage );
}

void CNetServer::OnReceiveFromServer( LPPACKAGE lpPackage )
{
	m_pTaskServer->OnReceiveFromServer( lpPackage );
}

void CNetServer::OnConnServerFailed()
{
	m_pTaskServer->OnConnServerFailed();
}


//==================================================//
//													//
//				�� �� �� �� ��						//
//													//
//==================================================//

CNetServer::CNetServer(CThreadTaskServer* pServer)
{
	//m_yServerCnt	= 0;	// ����������Ϊ0
	//m_yOperatorCnt	= 0;	// Operator����Ϊ0

	m_bServerConnected		= FALSE;	// δ���ӷ�����
	m_bServerVerified		= FALSE;	// δ��½������
	m_bOperatorConnected	= FALSE;	// δ����operator

//	m_pThreadConnClient		= NULL;
//	m_pThreadServeClient	= NULL;

	m_pTaskServer	= pServer;

	m_pClientConnServer		= new CClientConnServer(this);

	//��ʼ��Winsock��
	if(!AfxSocketInit())
		AfxMessageBox("Failed to Initialize Sockets", MB_OK| MB_ICONSTOP);
}

CNetServer::~CNetServer(void)
{
	delete	m_pClientConnServer;

//	WSACleanup();
}

//05-2-1,by Xy,Begin
void CNetServer::DownReturnAddress(LPPACKAGE lpPackage)
{
	m_pTaskServer->ReturnMsg2MemList(lpPackage);
}

void CNetServer::OnTimer(UINT nIDEvent)
{
	m_pClientConnServer->OnTimer(nIDEvent);
}

void CNetServer::ReLogin( )
{
	m_pTaskServer->ReLogin();
}
//05-2-1,by Xy,End

void CNetServer::OnServerRestarted()
{
//	m_pTaskServer->SendJobListToServer();
}

void CNetServer::OnNetReloginOK()
{
	m_pTaskServer->OnNetReloginOK();
}