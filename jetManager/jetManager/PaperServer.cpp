#include "StdAfx.h"
#include ".\paperserver.h"


//==================================================//
//													//
//						thread						//
//													//
//==================================================//
/*++++++++++++++++++++++++++++++++++++++++++++++++++++
[Description]
	threadPaperServe create to reduce the "UI-Dead" 
when the program runs into the Network Service.
	This thread is created in StartPaperServer, and is
destoryed in StopPaperServer.

[Parameters]
	LPVOID pVoid	-	Pointer to the CPaperServer
----------------------------------------------------*/
DWORD __stdcall   threadPaperServe(LPVOID pVoid) 
{
	CPaperServer* pThis = (CPaperServer*)pVoid;
//	pThis->AllocEventMessage();
	pThis->ThreadLoop();
	return 888;
}//Ends of eMarkNetworkService

void CPaperServer::ThreadLoop( )
{
	while ( !m_bExitThreadLoop )
	{
		switch ( m_eActionNext )
		{
		case	doWait:
			Sleep( 30 );
			break;

		case	doLogon:
			{
				m_eActionNext	= doWait;
				Logon( );
			}
			break;

		case	doLogout:
			break;

		case	doRequestPaper:
			break;

		default:
			break;
		}
	}
}

//==================================================//
//													//
//					调 用 接 口						//
//													//
//==================================================//
BOOL CPaperServer::StartPaperServer( )
{
	m_bExitThreadLoop		= FALSE;
	m_bPaperServerStarted	= TRUE;

	m_hThreadPaperService=CreateThread(NULL, 0, threadPaperServe, (LPVOID)this, 0, NULL);
	if(m_hThreadPaperService==NULL)
	{
		return	FALSE;
	}       

	return	TRUE;
}

BOOL CPaperServer::StopPaperServer( )
{
	m_bExitThreadLoop		= TRUE;
	m_bPaperServerStarted	= FALSE;

    return	TRUE;
}
void CPaperServer::SetLogonInfo( CString strAccount, CString strPassword )
{
	m_strAccount	= strAccount;
	m_strPassword	= strPassword;
}

void CPaperServer::ConnectServer( CString strAddr, u_short sinport )
{
	m_pNet->SetMainServer( strAddr, sinport );
	if ( m_pNet->Connect( ) )
		StartPaperServer( );
}

BOOL CPaperServer::SetNextAction( enumNextAction eActionNext )
{
	for ( int iTryTimes=0; iTryTimes<3; iTryTimes++ )
	{
		if ( m_eActionNext != doWait )
			Sleep( 100 );
		else
		{
			m_eActionNext	= eActionNext;
		}
	}

	return	FALSE;
}

void CPaperServer::Logon( CString strAccount, CString strPassword )
{
	SetLogonInfo( strAccount, strPassword );
	SetNextAction ( doLogon );
}

//==================================================//
//													//
//					内 部 动 作						//
//													//
//==================================================//

void CPaperServer::Logon( )
{
}

CPaperServer::CPaperServer(void)
{
	m_eActionNext	= doWait;

	m_pNet	= new CNetServer();

	m_pNet->StartNetServer( );

	m_bPaperServerStarted	= FALSE;
}

CPaperServer::~CPaperServer(void)
{
	if ( m_bPaperServerStarted )
	{
		StopPaperServer();
		Sleep(500);
	}

	m_pNet->StopNetServer( );
	
	delete	m_pNet;
}
