// ThreadTaskServer.cpp : 实现文件
//

#include "stdafx.h"
#include "jetManager.h"
#include "ThreadTaskServer.h"
#include "MainFrm.h"
#include "GUI/dlgManager.h"
#include <direct.h>
//#include <afx.h>
#include "Protocol.h"

// CThreadTaskServer

IMPLEMENT_DYNCREATE(CThreadTaskServer, CWinThread)

//==================================================//
//													//
//					调 用 接 口						//
//													//
//==================================================//
BOOL CThreadTaskServer::Start ( )
{
	m_bServerStarted	= TRUE;

	m_iCurrent		= 0;
	m_iProcessing		= 0;
	m_iTotal		= 0;

	m_iPriority		= 0;

	m_lstTaskHead	= NULL;
	m_lstTaskTail	= NULL;
	m_pTaskCurrent	= NULL;

	m_pDlg->m_pDlgControl->m_pDlgClient->m_pTaskServer = this;

	return	TRUE;
}

BOOL CThreadTaskServer::Stop ( )
{
	m_bServerStarted	= FALSE;

	::PostThreadMessage( m_nThreadID, WM_EXIT_THREAD, (WPARAM)0, (LPARAM)0 ); 

    return	TRUE;
}

void CThreadTaskServer::SetAppPath( char* pPath )
{
	this->m_lpPathApp	= pPath;
}

void CThreadTaskServer::SetScoreDlg( CDlgManager* pDlg )
{
	::PostThreadMessage( m_nThreadID, WM_SCOREDLG_HWND, (WPARAM)pDlg, (LPARAM)0 ); 
}

void CThreadTaskServer::SetLogonInfo( CString strAccount, CString strPassword )
{
	m_strAccount	= strAccount;
	m_strPassword	= strPassword;
}

void CThreadTaskServer::SetBufferPtrList( CPtrList* pListLarge, CPtrList* pListSmall, CPtrList* pListCommand )
{
	m_lpPtrLargeMemList		= pListLarge;
	m_lpPtrSmallMemList		= pListSmall;
	m_lpPtrCommandMemList	= pListCommand;
}

BOOL CThreadTaskServer::ConnectServer( CString strAddr, u_short sinport )
{
	if ( m_bServerStarted )
		return	TRUE;

	m_pNet->SetMainServer( strAddr, sinport );

	if ( m_pNet->Connect( ) )
	{
		//m_bConnected = TRUE;

		Start ( );

		/*
		
		*/
		return	TRUE;
	}

	return	FALSE;
}
void CThreadTaskServer::SendTypeToServer()
{
	Sleep(100);
	LPPACKAGE lpPackage	= (LPPACKAGE)m_lpPtrLargeMemList->RemoveHead( );
	lpPackage->pckHeader.dwLength		= sizeof(tagConnectionData) + sizeof(PACKAGE_HEADER);
	lpPackage->pckHeader.wVerify_1		= 8721;
	lpPackage->pckHeader.wVerify_2		= 8721;
	lpPackage->pckHeader.yType				= TAG_CONNECTION_DATA;
	tagConnectionData* lpPackConn			= (tagConnectionData*)(&(lpPackage->pckBody.lpBuffer));
	lpPackConn->yType							= CONTROL_END;
	m_pNet->SendToServer( lpPackage );
}

void CThreadTaskServer::Logon( )
{
	m_bLogout	= FALSE;

	LPPACKAGE		lpPackage;
	LPPACK_LOGIN	lpPackLogin;

	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead( );

	lpPackage->pckHeader.dwLength			= sizeof(PACKAGE_HEADER) + sizeof(PACK_LOGIN);
	lpPackage->pckHeader.wVerify_1			= 8721;
	lpPackage->pckHeader.wVerify_2			= 8721;
//, begin
	lpPackage->pckHeader.yType = MSG_CLIENT_LOGIN;
//, ends
	//lpPackage->packageHeader.yTypePackage	= 0;

	lpPackLogin	= (LPPACK_LOGIN)(&(lpPackage->pckBody.lpBuffer));

	lpPackLogin->yUserType	= MSG_CLIENT_LOGIN;
	lpPackLogin->iVersion	= VERSION_CLIENT;
	lstrcpy(lpPackLogin->strAccount, m_strAccount);
	lstrcpy(lpPackLogin->strPassword, m_strPassword);
	lpPackLogin->userip;

	m_pNet->SendToServer( lpPackage );

	//m_lpPtrCommandMemList->AddTail( (LPVOID)lpPackage );
}

#define	MAX_PRIORITY	1024
void CThreadTaskServer::Logout( )
{
/*	m_bLogout	= TRUE;
	m_pDlg->DisableLogout();
	m_pDlg->SetCurrentTask( NULL );

	// need to submit all tasks to server first!
////, begin
//	// 当是预执行状态时不需要提交job
//	if ( channelPreGrad != m_enumState )
////, ends
//	下面一行是为了避免在作业被提交后服务器的loopResponse线程
//	因为该机器优先级的执行变而给他发job
	m_iPriority	= MAX_PRIORITY;
		this->SubmitAllTask2Server();

	LPPACKAGE		lpPackage		;
	LPPACK_LOGOUT	lpPackLogout	;

	lpPackage		= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_LOGOUT);
	lpPackage->packageHeader.wVerify1		= 8721	;
	lpPackage->packageHeader.wVerify2		= 8721	;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0		;

	lpPackLogout	= (LPPACK_LOGOUT)(&(lpPackage->body));

	lpPackLogout->yUserType	= MSG_CLIENT_LOGOUT	;
	lpPackLogout->iMachineID= m_iMachineID		;

//	m_pNet->SendToServer( lpPackage);
	m_pNet->SendLogoutPackage ( lpPackage )		;*/
}

void CThreadTaskServer::CheckServerState ( )
{
/*	LPPACKAGE			lpPackage			;
	LPPACK_CHECK_SERVER	lpPackCheckServer	;

	lpPackage	= (LPPACKAGE) m_lpPtrCommandMemList->RemoveHead ( )	;

	lpPackage->packageHeader.dwSize			= sizeof ( PACKAGE_HEADER) + sizeof ( PACK_CHECK_SERVER )	;
	lpPackage->packageHeader.wVerify1		= 8721	;
	lpPackage->packageHeader.wVerify2		= 8721	;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0		;

	lpPackCheckServer	= (LPPACK_CHECK_SERVER) ( &(lpPackage->body) )	;

	lpPackCheckServer->yUserType	= MSG_ADMIN_CHECK_SERVER	;

	m_pNet->SendToServer ( lpPackage )	;*/
}

void CThreadTaskServer::Kick_out ( int iMachineID )
{
	Kick_out ( iMachineID, m_pChannelInfo->iIDJob );
}

void CThreadTaskServer::Kick_out ( int iMachineID, int iIDJob )
{
/*	LPPACKAGE		lpPackage		;
	LPPACK_KICKOUT	lpPack_kickout	;

	lpPackage	= (LPPACKAGE) m_lpPtrCommandMemList->RemoveHead ( )	;

	lpPackage->packageHeader.dwSize			= sizeof ( PACKAGE_HEADER) + sizeof ( PACK_KICKOUT )	;
	lpPackage->packageHeader.wVerify1		= 8721	;
	lpPackage->packageHeader.wVerify2		= 8721	;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0		;

	lpPack_kickout	= (LPPACK_KICKOUT) ( &(lpPackage->body) )	;

	lpPack_kickout->yUserType	= MSG_CLIENT_KICKOUT;
	lpPack_kickout->iIDJob	= iIDJob		;
	lpPack_kickout->iMachineID	= iMachineID		;

	m_pNet->SendToServer ( lpPackage )	;*/
}

void CThreadTaskServer::Task_recycle ( int iMachineID )
{
	Task_recycle ( iMachineID, m_pChannelInfo->iIDJob );
}

void CThreadTaskServer::Task_recycle ( int iMachineID, int iIDJob )
{
/*	LPPACKAGE				lpPackage		;
	LPPACK_OPERATOR_RECYCLE	lpPack_recycle	;

	lpPackage	= (LPPACKAGE) m_lpPtrCommandMemList->RemoveHead ( )	;

	lpPackage->packageHeader.dwSize			= sizeof ( PACKAGE_HEADER) + sizeof ( PACK_OPERATOR_RECYCLE )	;
	lpPackage->packageHeader.wVerify1		= 8721	;
	lpPackage->packageHeader.wVerify2		= 8721	;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0		;

	lpPack_recycle	= (LPPACK_OPERATOR_RECYCLE) ( &(lpPackage->body) )	;

	lpPack_recycle->yUserType	= MSG_OPERATOR_JOB_RECYCLE;
	lpPack_recycle->iIDJob	= iIDJob		;
	lpPack_recycle->iMachineID	= iMachineID		;

	m_pNet->SendToServer ( lpPackage )	;*/
}

void CThreadTaskServer::AllLogout ( )
{
/*	LPPACKAGE			lpPackage			;
	LPPACK_ALL_LOGOUT	lpPackCheckServer	;

	lpPackage	= (LPPACKAGE) m_lpPtrCommandMemList->RemoveHead ( )	;

	lpPackage->packageHeader.dwSize			= sizeof ( PACKAGE_HEADER) + sizeof ( PACK_ALL_LOGOUT )	;
	lpPackage->packageHeader.wVerify1		= 8721	;
	lpPackage->packageHeader.wVerify2		= 8721	;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0		;

	lpPackCheckServer	= (LPPACK_ALL_LOGOUT) ( &(lpPackage->body) )	;

	lpPackCheckServer->yUserType	= MSG_ADMIN_ALL_LOGOUT	;

	m_pNet->SendToServer ( lpPackage )	;*/
}

void CThreadTaskServer::ChannelLogout ( )
{
/*	LPPACKAGE				lpPackage	;
	LPPACK_CLIENT_COMMAND	lpPack_command;

	lpPackage		= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_CLIENT_COMMAND);
	lpPackage->packageHeader.wVerify1		= 8721	;
	lpPackage->packageHeader.wVerify2		= 8721	;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0		;

	lpPack_command	= (LPPACK_CLIENT_COMMAND)(&(lpPackage->body));

	lpPack_command->yUserType	= MSG_CLIENT_COMMAND;
	lpPack_command->dwCommandID	= CHANNEL_LOGOUT	;

	m_pNet->SendToServer( lpPackage);*/
}

void CThreadTaskServer::CheckChannelState ( )
{
/*	LPPACKAGE				lpPackage			;
	LPPACK_CHECK_CHANNEL	lpPackCheckChannel	;

	lpPackage	= (LPPACKAGE) m_lpPtrCommandMemList->RemoveHead ( )	;

	lpPackage->packageHeader.dwSize			= sizeof ( PACKAGE_HEADER) + sizeof ( LPPACK_CHECK_CHANNEL )	;
	lpPackage->packageHeader.wVerify1		= 8721	;
	lpPackage->packageHeader.wVerify2		= 8721	;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0		;

	lpPackCheckChannel	= (LPPACK_CHECK_CHANNEL) ( &(lpPackage->body) )	;

	lpPackCheckChannel->yUserType	= MSG_OPERATOR_CHECK_CHANNEL	;

	m_pNet->SendToServer ( lpPackage )	;*/
}

void CThreadTaskServer::ReLogin( )
{
//	LPCSTR strIpAddress = GetIpAddress();
/*	if ( m_bLogout )
		return;

	LPPACKAGE		lpPackage		;
	LPPACK_RELOGIN	lpPackRelogin	;

	lpPackage		= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();
	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_RELOGIN);
	lpPackage->packageHeader.wVerify1		= 8721;
	lpPackage->packageHeader.wVerify2		= 8721;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0;
	
	lpPackRelogin	= (LPPACK_RELOGIN)(&(lpPackage->body));

	if(lpPackRelogin != NULL)
	{
		lpPackRelogin->yUserType = MSG_CLIENT_RELOGIN	;
		lpPackRelogin->iMachineID= m_iMachineID			;
		//lstrcpy(lpPackRelogin->userip, strIpAddress)	;
		//lpPackRelogin->iOperator = GetOperator()		;
	}
	m_pNet->SendToServer(lpPackage);
	//m_lpPtrCommandMemList->AddTail( (LPVOID)lpPackage)	;*/
}

void CThreadTaskServer::OnNetReloginOK()
{
	this->RequestTask(m_iTotal - m_iProcessing);
}

// 应该告诉网络层停止连接尝试
// 在Logout包成功发出后即停止自动连接(已经实现)
void CThreadTaskServer::OnLogoutACK( LPUSER_MESSAGE lpUserMsg )
{
	if(lpUserMsg->yUserType == MSG_SERVER_LOGOUT_SUCCESS)
	{
		LPPACK_LOGOUT_SUCCESS p = (LPPACK_LOGOUT_SUCCESS)lpUserMsg;
		int m_iMachineID = p->iMachineID;
		int m_iOperatpor = p->yNewOperator;
		m_bConnected	 = FALSE;
		// 向GUI发送界面更新的消息
		m_pDlg->OnLogoutOK();

	}

	// 总是认为注销总是会成功, 因此下面的代码似乎多余
	if(lpUserMsg->yUserType == MSG_SERVER_LOGOUT_FAILURE)
	{
		LPPACK_LOGOUT_FAILURE p = (LPPACK_LOGOUT_FAILURE)lpUserMsg;
		CString str;
		str.Format("%S", p->strFailInfo);
		// 向GUI发送界面更新的消息 //
		m_pDlg->OnLogoutFailed(str);
	}
}

void CThreadTaskServer::OnChannelDone( LPUSER_MESSAGE lpUserMsg )
{
/*	m_bLogout		= TRUE;
	LPPACKAGE		lpPackage		;
	LPPACK_LOGOUT	lpPackLogout	;

	lpPackage		= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_LOGOUT);
	lpPackage->packageHeader.wVerify1		= 8721;
	lpPackage->packageHeader.wVerify2		= 8721;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0;

	lpPackLogout	= (LPPACK_LOGOUT)(&(lpPackage->body));

	lpPackLogout->yUserType	= MSG_CLIENT_CHANNEL_DONE	;
	lpPackLogout->iMachineID= m_iMachineID		;

	m_pNet->SendToServer( lpPackage);
	m_bConnected	= FALSE;
	m_pDlg->OnChannelDone();*/
}

void CThreadTaskServer::OnSupressLogout( LPUSER_MESSAGE lpUserMsg )
{
//	// need to submit all tasks to server first!
//	m_bLogout	= TRUE;
//	this->SubmitAllTask2Server();
//	LPPACKAGE		lpPackage		;
//	LPPACK_LOGOUT	lpPackLogout	;
//
//	lpPackage		= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();
//
//	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_LOGOUT);
//	lpPackage->packageHeader.wVerify1		= 8721;
//	lpPackage->packageHeader.wVerify2		= 8721;
////, begin
//	lpPackage->packageHeader.yChannelState	= m_enumState;
////, ends
//	lpPackage->packageHeader.yTypePackage	= 0;
//
//	lpPackLogout	= (LPPACK_LOGOUT)(&(lpPackage->body));
//
//	lpPackLogout->yUserType	= MSG_CLIENT_LOGOUT	;
//	lpPackLogout->iMachineID= m_iMachineID		;
//
//	m_pNet->SendToServer( lpPackage);
	Logout ( );
	
	m_bConnected	 = FALSE;
	m_pDlg->OnLogoutOK();
}
//
//void CThreadTaskServer::UpdateServerState( )
//{
//	LPCLIENT_CUTSHEET	pJob	= NULL;
//
//	if ( m_iTotal != 0 )
//	{
////		pJob	= (LPCLIENT_CUTSHEET)m_lstFreshTask.GetAt(m_posCurrent);
//	}
//}

void CThreadTaskServer::OnReceiveFromServer( LPPACKAGE lpPackage )
{
	switch(lpPackage->pckHeader.yType)
	{
	case TAG_CLIENT_DATA:
		m_pDlg->m_pDlgControl->m_pDlgClient->m_Client.doClientData(lpPackage);
		break;
	case TAG_RESEND_DATA:
		m_pDlg->m_pDlgControl->m_pDlgClient->m_Client.doResendData(lpPackage);
		break;
	default:
		break;
	}
/*	LPUSER_MESSAGE	lpUserMsg;

	lpUserMsg	= &(lpPackage->body.userMsg);
	switch ( lpUserMsg->yUserType )
	{
//, begin
	case	MSG_SERVER_COMMAND:
//	保持与服务器状态同步
		if ( channelPreGrad == m_enumState )
		{
			if ( lpPackage->packageHeader.yChannelState != m_enumState )
			{
				if ( channelActive == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelActive;
				}
				else if ( channelFinishing == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelFinishing;
				}
			}
		}
		break;
//, ends

	case	MSG_SERVER_LOGIN_SUCCESS:
		this->OnLoginACK(lpUserMsg);
		break;

	case	MSG_SERVER_ADMIN_LOGIN_SUCCESS:
		this->OnAdminLoginACK ( lpUserMsg )	;
		break;

	case	MSG_SERVER_LOGIN_FAILURE:
		{
			LPPACK_LOGIN_FAILURE	pPack;
			pPack	= (LPPACK_LOGIN_FAILURE)(lpUserMsg);

			CString	str;
			str.Format( "%s", pPack->strFailInfo );

//			m_pFrame->OnLoginFailure( str );
			if (pPack->iMachineID == -1)
				m_pDlg->OnLoginFailure( str );
			else
				m_pDlg->OnTwiceLogin(pPack->iMachineID, pPack->iIDJob, str);
		}
		break;

	case	MSG_SERVER_PASSWORD_MODIFICATION_ACK:
		{
			OnModifyPwdACK( lpUserMsg );
		}
        break;

	case	MSG_SERVER_JOB_FTP:
//, begin
//	保持与服务器状态同步
		if ( channelPreGrad == m_enumState )
		{
			if ( lpPackage->packageHeader.yChannelState != m_enumState )
			{
				if ( channelActive == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelActive;
				}
				else if ( channelFinishing == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelFinishing;
				}
			}
		}
//, ends

		{
			LPPACK_JOB_FTP	lpTaskFTP;
			lpTaskFTP	= (LPPACK_JOB_FTP)(lpUserMsg);

			EnterCriticalSection( &m_csTaskList );
			RecvOneTask( lpTaskFTP );

			m_iTotal++;
			LeaveCriticalSection( &m_csTaskList );
		}
		break;

	case	MSG_SERVER_JOBCHECK_FTP:
//, begin
//	保持与服务器状态同步
		if ( channelPreGrad == m_enumState )
		{
			if ( lpPackage->packageHeader.yChannelState != m_enumState )
			{
				if ( channelActive == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelActive;
				}
				else if ( channelFinishing == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelFinishing;
				}
			}
		}
//, ends
		{
			LPPACK_JOBCHECK_FTP	lpTaskFTP;
			lpTaskFTP	= (LPPACK_JOBCHECK_FTP)(lpUserMsg);

			EnterCriticalSection( &m_csTaskList );
			RecvCheckTask( lpTaskFTP );

			m_iTotal++;
			LeaveCriticalSection( &m_csTaskList );
		}
		break;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	case	MSG_SERVER_SCRIPT_FTP:
//, begin
//	保持与服务器状态同步
		if ( channelPreGrad == m_enumState )
		{
			if ( lpPackage->packageHeader.yChannelState != m_enumState )
			{
				if ( channelActive == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelActive;
				}
				else if ( channelFinishing == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelFinishing;
				}
			}
		}
//, ends
		{
			LPPACK_SCRIPT_FTP	lpScript_FTP;
			lpScript_FTP	= (LPPACK_SCRIPT_FTP)(lpUserMsg);

			Recv_script ( lpScript_FTP );
			//EnterCriticalSection( &m_csTaskList );
			//RecvCheckTask( lpTaskFTP );

			//m_iTotal++;
			//LeaveCriticalSection( &m_csTaskList );
		}
		break;
#endif
//, ends

//, begin
#ifdef	SWITCH_QUALITY_MANAGEMENT
	case	MSG_SERVER_QUALITY_MANAGEMENT_SEGMENT:
//, begin
//	保持与服务器状态同步
		if ( channelPreGrad == m_enumState )
		{
			if ( lpPackage->packageHeader.yChannelState != m_enumState )
			{
				if ( channelActive == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelActive;
				}
				else if ( channelFinishing == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelFinishing;
				}
			}
		}
//, ends
		{
			LPPACK_QUALITY_SEGMENT	lpQuality_segment;
			lpQuality_segment	= (LPPACK_QUALITY_SEGMENT)(lpUserMsg);

//			Recv_quality_segment ( lpQuality_segment );
		}
		break;
#endif
//, ends

	//case	MSG_SERVER_CHECK_CUTSHEET:
	//	{
	//		this->OnServerCheckJob(lpUserMsg);
	//	}
	//	break;

	case	MSG_SERVER_JOB_RECYCLE:
//, begin
//	保持与服务器状态同步
		if ( channelPreGrad == m_enumState )
		{
			if ( lpPackage->packageHeader.yChannelState != m_enumState )
			{
				if ( channelActive == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelActive;
				}
				else if ( channelFinishing == lpPackage->packageHeader.yChannelState )
				{
					Reset ( );
					m_enumState	= channelFinishing;
				}
			}
		}
//, ends
		{
			this->OnServerTaskRecycle( lpUserMsg );
		}
		break;

	case	MSG_SERVER_ONFINISHING:
		{
			this->OnServerOnFinishing( );
		}
		break;

	case	MSG_SERVER_LOGOUT_SUCCESS:
		{
			this->OnLogoutACK(lpUserMsg);
		}
		break;
		
	case	MSG_SERVER_CHANNEL_STATE:
		{
			LPPACK_CHANNEL_STATE	lpChannelState;
			lpChannelState	= (LPPACK_CHANNEL_STATE)(lpUserMsg);

			this->OnChannelState ( lpChannelState )	;
		}
		break;

	case	MSG_SERVER_SUPRESSLOGOUT:
		{
			this->OnSupressLogout( lpUserMsg );
		}

	case	MSG_SERVER_CHANNEL_DONE:
		{
			this->OnChannelDone( lpUserMsg );
		}

	default:
		break;
	}*/
}

void CThreadTaskServer::OnChannelState ( LPPACK_CHANNEL_STATE lpChannelState )
{
	m_pDlg->OnChannelState ( lpChannelState )	;
}

void CThreadTaskServer::OnConnServerFailed()
{
	m_pDlg->SetStateLogon();
}

// 接收下载的复查job处理函数
void CThreadTaskServer::RecvCheckTask( LPPACK_JOBCHECK_FTP lpCheckTaskFTP )
{
	int	i;

	// 存盘 首先切换到当前用户目录 之后创建文件
	_chdir( m_strUserPath );

	// 将图片设置为 隐藏 /* | 系统 只读 |*/
	//CFileStatus	rStatus;
	//m_FileRecv.GetStatus( rStatus );
	//rStatus.m_attribute	= hidden;// | readOnly | system;
	//m_FileRecv.SetStatus( strFileName, rStatus );

	m_iPriority++;

	// 开始处理新下载的job
	LPFORMAT_SHEET	pTask;
	pTask	= new FORMAT_SHEET;
	ZeroMemory( pTask, sizeof(FORMAT_SHEET) );
	pTask->iFormatSheetID	= lpCheckTaskFTP->iFormatSheetID;
	pTask->bSerialized		= FALSE;
	pTask->pNextTask	= NULL;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	pTask->iMachine1ID	= lpCheckTaskFTP->iMachine1ID;
	pTask->iMachine2ID	= lpCheckTaskFTP->iMachine2ID;
	pTask->iMachine3ID	= lpCheckTaskFTP->iMachine3ID;
#endif
//, ends

	//if( NULL == m_lstCheckHead )
	if( NULL == m_lstTaskHead )
	{
		// 此图片为系统接受到的第一张图片
		pTask->pPrevTask	= NULL;

		//m_lstCheckHead	= pTask;
		//m_lstCheckTail	= pTask;
		m_lstTaskHead	= pTask;
		m_lstTaskTail	= pTask;
		m_pTaskCurrent	= pTask;

		CFile	m_FileRecv;
		CString strFileName;
		strFileName.Format("%d.gif", lpCheckTaskFTP->iFormatSheetID);
		if( !m_FileRecv.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
//			int	i=0;
			return;
		}
		m_FileRecv.Write(&(lpCheckTaskFTP->data), lpCheckTaskFTP->dwLen);
		m_FileRecv.Flush();
		m_FileRecv.Close();	
        		
		m_pDlg->SetCurrentTask( m_pTaskCurrent );
		m_pDlg->UpdatePicView( m_pTaskCurrent->iFormatSheetID );

		m_pDlg->UpdateData(FALSE);
		m_pDlg->FocusFirst ( );
	}
	else
	{
		{
//, begin
			//m_lstTaskTail->pNextTask	= pTask;
			//pTask->pPrevTask			= m_lstTaskTail;
			//m_lstTaskTail				= pTask;			// 设置新尾巴

			//m_pDlg->EnableNextButton();
			LPFORMAT_SHEET	lpTask;

			//lpTask	= m_lstCheckHead;
			lpTask	= m_lstTaskHead;
			while ( NULL != lpTask )
			{
				if ( lpTask->iFormatSheetID == pTask->iFormatSheetID )
				{
					break;
				}
				lpTask	= lpTask->pNextTask;
			}

			if ( NULL == lpTask )
			{
				//m_lstCheckTail->pNextTask	= pTask;
				//pTask->pPrevTask			= m_lstCheckTail;
				//m_lstCheckTail				= pTask;			// 设置新尾巴
				m_lstTaskTail->pNextTask	= pTask;
				pTask->pPrevTask			= m_lstTaskTail;
				m_lstTaskTail				= pTask;			// 设置新尾巴

				CFile	m_FileRecv;
				CString strFileName;
				strFileName.Format("%d.gif", lpCheckTaskFTP->iFormatSheetID);
				if( !m_FileRecv.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
				{
					int	i=0;
					return;
				}
				m_FileRecv.Write(&(lpCheckTaskFTP->data), lpCheckTaskFTP->dwLen);
				m_FileRecv.Flush();
				m_FileRecv.Close();	

			}
			else
				m_iPriority--;
//, ends
		}
	}
}


// 接收下载的job处理函数
void CThreadTaskServer::RecvOneTask( LPPACK_JOB_FTP	lpTaskFTP )
{
	int		i;

	// 存盘 首先切换到当前用户目录 之后创建文件
	_chdir( m_strUserPath );

	// 将图片设置为 隐藏 /* | 系统 只读 |*/
	//CFileStatus	rStatus;
	//m_FileRecv.GetStatus( rStatus );
	//rStatus.m_attribute	= hidden;// | readOnly | system;
	//m_FileRecv.SetStatus( strFileName, rStatus );

	m_iPriority++;

	// 开始处理新下载的job
	LPFORMAT_SHEET	pTask;
	pTask	= new FORMAT_SHEET;
	ZeroMemory( pTask, sizeof(FORMAT_SHEET) );
	pTask->iFormatSheetID	= lpTaskFTP->iFormatSheetID;
	pTask->bSerialized		= FALSE;
//, begin
#ifdef	SWITCH_STORE_SCRIPT
	pTask->bHasScript	= FALSE;
#endif
//, ends
	pTask->pNextTask	= NULL;

	if( NULL == m_lstTaskHead )
	{
		// 此图片为系统接受到的第一张图片
		pTask->pPrevTask	= NULL;

		m_lstTaskHead	= pTask;
		m_lstTaskTail	= pTask;
		m_pTaskCurrent	= pTask;

		CFile	m_FileRecv;
		CString strFileName;
		strFileName.Format("%d.gif", lpTaskFTP->iFormatSheetID);
		if( !m_FileRecv.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
//			int	i=0;
			return;
		}
		m_FileRecv.Write(&(lpTaskFTP->data), lpTaskFTP->dwLen);
		m_FileRecv.Flush();
		m_FileRecv.Close();	
        		
		m_pDlg->SetCurrentTask( m_pTaskCurrent );
		m_pDlg->UpdatePicView( m_pTaskCurrent->iFormatSheetID );

		m_pDlg->UpdateData(FALSE);
		m_pDlg->FocusFirst ( );
	}
	else
	{
		// 如果为试执行状态则先替换先前job
		if ( (m_enumState == channelPreGrad) && (m_yHead != 1) )
		{
			LPFORMAT_SHEET	pTaskPrev;

//, begin
			// 若是同一份job
			if ( m_pTaskCurrent->iFormatSheetID == pTask->iFormatSheetID )
			{
				delete	pTask;
				return;
			}
//, ends

			pTaskPrev		= m_lstTaskHead;
			m_lstTaskHead	= pTask;
			m_lstTaskTail	= pTask;
			m_pTaskCurrent	= pTask;

//, begin
			// 增加下面一段来接收图片
			CFile	m_FileRecv;
			CString strFileName;
			strFileName.Format("%d.gif", lpTaskFTP->iFormatSheetID);
			if( !m_FileRecv.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
			{
				int	i=0;
				return;
			}
			m_FileRecv.Write(&(lpTaskFTP->data), lpTaskFTP->dwLen);
			m_FileRecv.Flush();
			m_FileRecv.Close();	
//, ends

			m_pDlg->SetCurrentTask( m_pTaskCurrent );
			m_pDlg->UpdatePicView( m_pTaskCurrent->iFormatSheetID );
			m_pDlg->SetState( CDlgManager::Control_Processing );

			m_pDlg->UpdateData(FALSE);

			// 删除先前的job
			CFile delFile;	
			CString strDelFileName;
			strDelFileName.Format("%d.gif", pTaskPrev->iFormatSheetID);

			delFile.Remove(strDelFileName);

			delete	pTaskPrev;
		}
		else
		{
//, begin
			//m_lstTaskTail->pNextTask	= pTask;
			//pTask->pPrevTask			= m_lstTaskTail;
			//m_lstTaskTail				= pTask;			// 设置新尾巴

			//m_pDlg->EnableNextButton();
			LPFORMAT_SHEET	lpTask;

			lpTask	= m_lstTaskHead;
			while ( NULL != lpTask )
			{
				if ( lpTask->iFormatSheetID == pTask->iFormatSheetID )
				{
					break;
				}
				lpTask	= lpTask->pNextTask;
			}

			if ( NULL == lpTask )
			{
				m_lstTaskTail->pNextTask	= pTask;
				pTask->pPrevTask			= m_lstTaskTail;
				m_lstTaskTail				= pTask;			// 设置新尾巴

				CFile	m_FileRecv;
				CString strFileName;
				strFileName.Format("%d.gif", lpTaskFTP->iFormatSheetID);
				if( !m_FileRecv.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
				{
					int	i=0;
					return;
				}
				m_FileRecv.Write(&(lpTaskFTP->data), lpTaskFTP->dwLen);
				m_FileRecv.Flush();
				m_FileRecv.Close();	

			}
			else
				m_iPriority--;
//, ends
		}
	}
}

// 系统进入终止调度状态
// 此时启动请求job按钮
void CThreadTaskServer::OnServerOnFinishing( )
{
	m_enumState	= channelFinishing;
	m_pDlg->SetChannelState( channelFinishing );
}

void CThreadTaskServer::OnLoginACK( LPUSER_MESSAGE lpUserMsg)
{
	CString					str;
	LPCHANNEL_INFO			pChannelInfo;
	LPPACK_LOGIN_SUCCESS	pPack;

	pPack	= (LPPACK_LOGIN_SUCCESS)lpUserMsg;

	m_iMachineID	= pPack->iMachineID;
	m_iOperator		= pPack->iOperator;
//	m_iPriority		= pPack->iPriority;
	m_iPriority		= 0;
	m_iMarkedCnt	= pPack->iMarkedCnt;
	m_enumState		= pPack->enumState;
	m_yHead			= pPack->yHead;

	// 检查是否是管理员登陆进来
	if ( 2 == m_yHead )
	{
		m_pDlg->SetHead( m_yHead );
		m_pDlg->m_strNickname = pPack->strNickName;
		m_pDlg->OnLoginOK();
		return	;
	}

	m_pDlg->SetMarkedCnt( m_iMarkedCnt );
	m_pDlg->SetChannelState( m_enumState );
	m_pDlg->SetHead( m_yHead );

	if ( _chdir( m_lpPathApp ) == -1 )
	{
		AfxMessageBox( "无法找到应用程序所在的路径!\n  请退出程序!!!" );
		return;
	}

	str.Format("%d", pPack->iMachineID);
	if ( _chdir(str) == -1)
	{
		_mkdir(str);
	}

	m_strUserPath.Empty();
	m_strUserPath	+= m_lpPathApp;
	m_strUserPath	+= "\\" + str;

	m_pDlg->SetUserPath( m_strUserPath );

	//,end
	// 开始请求job
//	int	iUnchecked = m_lstFreshTask.GetCount();
	if ( m_enumState == channelActive )
		this->RequestTask(0);
	else if ( m_enumState == channelFinishing )
		this->RequestTask(0);

	pChannelInfo	 = (LPCHANNEL_INFO)&(pPack->lpChannel);

	::memcpy ( m_pChannelInfo, pChannelInfo, sizeof(CHANNEL_INFO) );

	// 初始化GUI参数
	//strcpy(m_pDlg->m_strNickname.GetBuffer(), str);
	m_pDlg->m_strNickname = pPack->strNickName;
	
	m_pDlg->OnLoginOK();
}

void CThreadTaskServer::OnAdminLoginACK ( LPUSER_MESSAGE lpUserMsg )
{
	m_pDlg->SetHead( 2 );
	m_pDlg->m_strNickname	= "管理员";
	m_pDlg->OnLoginOK();
}

void CThreadTaskServer::RequestTask(int iUncheckedCnt)
{
/*	LPPACKAGE				lpPackage			;
    LPPACK_JOB_REQUEST	lpPackTaskRequest	;

	if ( m_lpPtrCommandMemList->GetCount() != 0 )
	{
        lpPackage = (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

		lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_JOB_REQUEST);
		lpPackage->packageHeader.wVerify1		= 8721;
		lpPackage->packageHeader.wVerify2		= 8721;
//, begin
		lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
		lpPackage->packageHeader.yTypePackage	= 0;

		lpPackTaskRequest	= (LPPACK_JOB_REQUEST)(&(lpPackage->body));

		if (lpPackTaskRequest != NULL)
		{
			lpPackTaskRequest->yUserType		= MSG_CLIENT_JOB_REQUEST		;
			lpPackTaskRequest->iUncheckedCnt	= iUncheckedCnt					;
		}
	
		m_pNet->SendToServer( lpPackage)					;
	}*/
}

// 首先更新GUI的状态
void CThreadTaskServer::RequestTask( )
{
/*	int	iMarked;

	EnterCriticalSection( &m_csTaskList );

	if ( m_iTotal == 0 )
	{
		m_iPriority	= 0;
		LeaveCriticalSection( &m_csTaskList );
		RequestTask( 0 );
		return;
	}

	if ( m_iPriority == 0 )
	{
		LeaveCriticalSection( &m_csTaskList );
		RequestTask( 0 );
		return;
	}

	if ( m_pDlg->IsCurrentMarked() )
	{
		iMarked	= m_iCurrent + 1;
		m_iPriority--;
	}
	else
		iMarked	= m_iCurrent;

	if ( iMarked <= 0 )
	{
		LeaveCriticalSection( &m_csTaskList );
		RequestTask( m_iPriority );
		return;
	}

	// 总数
	if ( iMarked >= MAX_MARKED_COUNT )
	{
		// 上缴
//		TaskSubmitPacked( MAX_SUBMIT_COUNT );
		SubmitMarks2Server( MAX_SUBMIT_COUNT );

//		m_iTotal	-= MAX_SUBMIT_COUNT;	将在上传时执行变它的值

		// 设置m_iCuurent
		// 设置m_iProcessing
		m_iCurrent	-= MAX_SUBMIT_COUNT;
		m_iProcessing	-= MAX_SUBMIT_COUNT;
		iMarked		-= MAX_SUBMIT_COUNT;
	}

	if ( (iMarked < MAX_MARKED_COUNT) && (iMarked > 0) )
	{
		// 上缴
//		TaskSubmitPacked( iMarked );
		SubmitMarks2Server( iMarked );

//		m_iTotal	-= MAX_SUBMIT_COUNT;	将在上传时执行变它的值

		// 设置m_iCuurent
		// 设置m_iProcessing
		m_iCurrent	-= iMarked;
		m_iProcessing	-= iMarked;
		if ( m_iCurrent < 0 )
			m_iCurrent	= 0;
		if ( m_iProcessing < 0 )
			m_iProcessing	= 0;
		//m_iCurrent++;
		//m_iProcessing++;
	}

	LeaveCriticalSection( &m_csTaskList );

	LPPACKAGE				lpPackage			;
    LPPACK_JOB_REQUEST	lpPackTaskRequest	;

	if ( m_lpPtrCommandMemList->GetCount() != 0 )
	{
        lpPackage = (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

		lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_JOB_REQUEST);
		lpPackage->packageHeader.wVerify1		= 8721;
		lpPackage->packageHeader.wVerify2		= 8721;
//, begin
		lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
		lpPackage->packageHeader.yTypePackage	= 0;

		lpPackTaskRequest	= (LPPACK_JOB_REQUEST)(&(lpPackage->body));

		if (lpPackTaskRequest != NULL)
		{
			lpPackTaskRequest->yUserType		= MSG_CLIENT_JOB_REQUEST	;
			lpPackTaskRequest->iUncheckedCnt	= m_iPriority				;
		}
	
		m_pNet->SendToServer( lpPackage)					;
	}*/
}

void CThreadTaskServer::Client_modify_userinfo(CString strOldPwd, CString strNewPwd, CString strAccount)
{
/*	LPPACKAGE lpPackage;
	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_PASSWORD_MODIFICATION );
	lpPackage->packageHeader.wVerify1		= 8721;
	lpPackage->packageHeader.wVerify2		= 8721;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0;

	LPPACK_PASSWORD_MODIFICATION p = (LPPACK_PASSWORD_MODIFICATION)(&(lpPackage->body)); 

	if(NULL != p)
	{
		p->yUserType = MSG_CLIENT_PASSWORD_MODIFICATION;
		lstrcpy(p->strAccount, strAccount)	;
		lstrcpy(p->strPswdOld, strOldPwd)	;
		lstrcpy(p->strPswddNew, strNewPwd)	;

		m_pNet->SendToServer(lpPackage);
	}*/
}

void CThreadTaskServer::OnModifyPwdACK( LPUSER_MESSAGE lpUserMsg )
{
	LPPACK_PASSWORD_MODIFICATION_ACK p = (LPPACK_PASSWORD_MODIFICATION_ACK)lpUserMsg;
	CString str;
	str.Format("%s", p->strModInfo);
	// BOOL bModify = p->bSuccess;
	// 向GUI发送通知消息
	m_pDlg->OnModifyPwd( str);
}


void CThreadTaskServer::ReturnMsg2MemList( LPPACKAGE lpPackage )
{
	ZeroMemory((void*)lpPackage,SIZE_COMMAND_MEM);
	POSITION pos;
	pos = m_lpPtrCommandMemList->Find((void*)lpPackage);
	if(pos != NULL)
	   m_lpPtrCommandMemList->RemoveAt(pos); 
	m_lpPtrCommandMemList->AddTail((LPVOID)lpPackage); 	
}

//,begin
//void CThreadTaskServer::SubmitTask( LPCLIENT_JOB_PACKED pTaskPacked )
//{	
//	if(pTaskPacked == NULL)
//	{
//		return;
//	}
//	else
//	{
//		if (m_lpPtrCommandMemList->GetCount() != 0)
//		{
//			LPPACKAGE			lpPackage	;
//			LPPACK_JOB_SUBMIT pTaskSubmit;
//			int					iSubmitCnt	;
//
//			lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();
//			DWORD		dwSubmitSize;
//			int m	= pTaskPacked->iTaskPackedCnt;
//			dwSubmitSize	= sizeof(PACK_JOB_SUBMIT) - sizeof(JOB_SUBMITTED)  + m * ( (sizeof(JOB_SUBMITTED) - sizeof(BYTE)) + (m_iStepCount * sizeof(BYTE)) );
////			dwSubmitSize	= sizeof(int)  + m * ( sizeof(int) + sizeof(BOOL) + (m_iStepCount * sizeof(BYTE)) );
////			dwSubmitSize	= sizeof(int) + 2 * sizeof(BYTE) + m * ( sizeof(int) + (16 * sizeof(BYTE)) );
//
//			lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + dwSubmitSize;
//			lpPackage->packageHeader.wVerify1		= 8721;
//			lpPackage->packageHeader.wVerify2		= 8721;
//			lpPackage->packageHeader.yTypePackage	= 0;
//		
//			pTaskSubmit = (LPPACK_JOB_SUBMIT)(&(lpPackage->body));
//		
//			pTaskSubmit->yUserType		= MSG_CLIENT_JOB_SUBMIT		;
//			pTaskSubmit->iSubmitCnt	= pTaskPacked->iTaskPackedCnt	; 
//			iSubmitCnt					= pTaskSubmit->iSubmitCnt		;
//			for(int n = 0; n < iSubmitCnt; n++)
//			{
//				(pTaskSubmit->pItem[n]).nJobID = (pTaskPacked->ClientCutsheet[n]).iFormatSheetID;
//				(pTaskSubmit->pItem[n]).bBadJob= (pTaskPacked->ClientCutsheet[n]).bBadJob;
//				for (int i = 0; i < m_pDlg->m_iSteps; i++)
//				{
//					(pTaskSubmit->pItem[n]).yStepScore[i] = (pTaskPacked->ClientCutsheet[n]).yStepScore[i];
//				}
//			}
//
//			m_iTotal	-= iSubmitCnt;	
//			pTaskSubmit->iPriority = m_iPriority;
//
//			for (int k = 0; k < pTaskSubmit->iSubmitCnt; k++)
//			{	
//				CFile delFile;	
//				CString strDelFileName;
//				strDelFileName.Format("%d.gif", pTaskSubmit->pItem[k].nJobID);
//
//				delFile.Remove(strDelFileName);
//			}
//
//			// 将包发出去
//			m_pNet->SendToServer(lpPackage)	;
//		}
//		
//	}
//}

// 向服务器提交iSubmitCnt份作业
void CThreadTaskServer::SubmitMarks2Server( int iSubmitCnt )
{
/*	if ( iSubmitCnt <= 0 )
		return;
	
	if ( m_enumState == channelPreGrad )
	{
		for(int j = 0; j < iSubmitCnt; j++)
		{
			CFile				delFile;
			CString				strDelFileName;
			LPFORMAT_SHEET	pTask;		

			pTask			= m_lstTaskHead;
			m_lstTaskHead	= pTask->pNextTask;
			
			strDelFileName.Format("%d.gif", pTask->iFormatSheetID);
			delFile.Remove(strDelFileName);

			delete	pTask;
		}
		if ( NULL != m_lstTaskHead )
			m_lstTaskHead->pPrevTask = NULL	;

		return;
	}

	// 如果内存缓冲使用完毕则返回
	if ( m_lpPtrCommandMemList->GetCount() <= 0 )
		return;

	DWORD				dwSubmitSize	;
	LPPACKAGE			lpPackage		;
	LPPACK_JOB_SUBMIT	lpTaskSubmit	;
	LPJOB_SUBMITTED	lpSubmitItem	;
	LPFORMAT_SHEET	lpTask			;			

	lpPackage		= (LPPACKAGE) m_lpPtrCommandMemList->RemoveHead( );
//	dwSubmitSize	= sizeof ( PACK_JOB_SUBMIT );
//	dwSubmitSize	= sizeof ( JOB_SUBMITTED );
	dwSubmitSize	= sizeof ( PACK_JOB_SUBMIT )				// 子包头 + 1份作业
					- sizeof ( JOB_SUBMITTED )				// 1 份作业 ==> 余下的是子包头
					+ iSubmitCnt * sizeof ( JOB_SUBMITTED );	// iSubmitCnt份作业
//	dwSubmitSize	= sizeof(PACK_JOB_SUBMIT) - sizeof(JOB_SUBMITTED)  + iSubmitCnt * ( (sizeof(JOB_SUBMITTED) - sizeof(BYTE)) + (m_iStepCount * sizeof(BYTE)) );

	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + dwSubmitSize;
	lpPackage->packageHeader.wVerify1		= 8721;
	lpPackage->packageHeader.wVerify2		= 8721;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0;

	lpTaskSubmit	= (LPPACK_JOB_SUBMIT)(&(lpPackage->body));

	lpTaskSubmit->yUserType	= MSG_CLIENT_JOB_SUBMIT	;
	lpTaskSubmit->iSubmitCnt	= iSubmitCnt				;
	lpTaskSubmit->iPriority	= m_iPriority				;

	if ( NULL != m_lstTaskHead )
		m_lstTaskHead->pPrevTask	= NULL;

	// 将包发出去
	m_pNet->SendToServer(lpPackage)	;*/
}

//, begin
#ifdef	SWITCH_STORE_SCRIPT
void CThreadTaskServer::Recv_script ( LPPACK_SCRIPT_FTP lpScript_FTP )
{
	CFile	fileScript;
	CString	strScript;

	strScript.Format( "\\%d_%d.cpt", lpScript_FTP->iFormatSheetID, lpScript_FTP->iMachineID );
	strScript	= m_strUserPath + strScript;

	// 存盘 首先切换到当前用户目录 之后创建文件
	_chdir( m_strUserPath );

	if( !fileScript.Open(strScript, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary) )
	{
		return;
	}

	fileScript.Write( &(lpScript_FTP->data), lpScript_FTP->dwLen);
	fileScript.Flush();
	fileScript.Close();	
}

void CThreadTaskServer::Submit_script_2_server ( LPFORMAT_SHEET lpTask )
{
/*	CFile	fileScript;
	CString	strScript;

	strScript.Format( "\\%d.cpt", lpTask->iFormatSheetID );
	strScript	= m_strUserPath + strScript;

//	AfxMessageBox ( strScript + "\r\n" + m_strUserPath + "\r\n2" );
	if ( fileScript.Open(strScript, CFile::modeRead | CFile::shareDenyWrite) != NULL ) 
	{
		ULONGLONG	nLength		;
		DWORD		dwPack_size	;
		LPPACKAGE	lpPackage	;

		LPPACK_SCRIPT_SUBMIT	lpScriptSubmit;

		nLength	= fileScript.GetLength();

		dwPack_size	= sizeof(PACKAGE_HEADER) + sizeof(PACK_SCRIPT_SUBMIT)
			- sizeof(BYTE) + nLength;

		// 如果文件过大则不上传
		if ( SIZE_COMMAND_MEM < dwPack_size )
		{
			fileScript.Close();
			fileScript.Remove ( strScript );
//			AfxMessageBox ( "2.1" );
			return;
		}

		if ( m_lpPtrCommandMemList->IsEmpty() )
		{
			fileScript.Close();
			fileScript.Remove ( strScript );
//			AfxMessageBox ( "2.2" );
			return;
		}

		lpPackage	= (LPPACKAGE) m_lpPtrCommandMemList->RemoveHead( );

		lpPackage->packageHeader.dwSize			= dwPack_size;
		lpPackage->packageHeader.wVerify1		= 8721;
		lpPackage->packageHeader.wVerify2		= 8721;
//, begin
		lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
		lpPackage->packageHeader.yTypePackage	= 0;

		lpScriptSubmit	= (LPPACK_SCRIPT_SUBMIT)(&(lpPackage->body));

		lpScriptSubmit->yUserType	= MSG_CLIENT_SCRIPT_SUBMIT;
		lpScriptSubmit->iFormatSheetID	= lpTask->iFormatSheetID;
		lpScriptSubmit->iLength		= nLength;
		fileScript.Read ( lpScriptSubmit->pScript, nLength );

		fileScript.Close();
		fileScript.Remove ( strScript );
		
//		AfxMessageBox ( "3" );

		// 将包发出去
		m_pNet->SendToServer(lpPackage)	;
	}*/
}
#endif
//, ends

//void CThreadTaskServer::TaskSubmitPacked( int M )
//{
//	LPCLIENT_JOB_PACKED pTaskPacked;
//
//	if ( M <= 0 )
//		return;
//	
//	if ( m_enumState == channelPreGrad )
//	{
//		for(int j = 0; j < M; j++)
//		{
//			CFile				delFile;
//			CString				strDelFileName;
//			LPFORMAT_SHEET	pTask;		
//
//			pTask			= m_lstTaskHead;
//			m_lstTaskHead	= pTask->pNextTask;
//			
//			strDelFileName.Format("%d.gif", pTask->iFormatSheetID);
//			delFile.Remove(strDelFileName);
//
//			delete	pTask;
//		}
//		if ( NULL != m_lstTaskHead )
//			m_lstTaskHead->pPrevTask = NULL	;
//
//		return;
//	}
//
//	pTaskPacked = (LPCLIENT_JOB_PACKED) new char[1024];	
//
//	pTaskPacked->iTaskPackedCnt = M;
//
//	for(int j = 0; j < M; j++)
//	{
//		LPFORMAT_SHEET	pTask;			
//
//		pTask	= m_lstTaskHead;
//		
//		m_lstTaskHead	= pTask->pNextTask;
//
//		//pTaskPacked->iTaskPackedCnt++;
//		pTaskPacked->ClientCutsheet[j].iFormatSheetID = pTask->iFormatSheetID	;
//
//		//5-02-01, for bad task!
//		pTaskPacked->ClientCutsheet[j].bBadJob= pTask->bBadJob	;
//		//5-02-01,end
//
//		for (int k = 0; k <m_pDlg->m_iSteps; k++)
//		{
//			pTaskPacked->ClientCutsheet[j].yStepScore[k] = pTask->yStepScore[k];
//		}
//
//		delete	pTask;
//	}
//	if ( NULL != m_lstTaskHead )
//		m_lstTaskHead->pPrevTask = NULL	;
//
//	this->SubmitTask(pTaskPacked)		;
//	delete pTaskPacked;
//}

//, begin
void CThreadTaskServer::Reset( )
{
	LPFORMAT_SHEET	pTask;	
	CFile				delFile;
	CString				strDelFileName;

	// 设置界面
	m_pDlg->SetCurrentTask ( NULL );
	m_pDlg->SetChannelState ( channelActive );

	::EnterCriticalSection( &m_csTaskList );

	pTask	= m_lstTaskHead;
	while ( NULL != pTask )
	{
		strDelFileName.Format("%d.gif", pTask->iFormatSheetID);
		delFile.Remove(strDelFileName);

		m_lstTaskHead	= pTask->pNextTask;

		delete	pTask;

		pTask	= m_lstTaskHead;
	}

	m_iCurrent	= 0;
	m_iTotal	= 0;
	m_iProcessing	= 0;
	m_iPriority	= 0;
	
	::LeaveCriticalSection( &m_csTaskList );

	m_pDlg->OnLoginOK();
}
//, ends

// 递交m_lstSubmitTask中所有的SubmitTask!
void CThreadTaskServer::SubmitAllTask2Server( )
{
	LPCLIENT_JOB_PACKED pTaskPacked;

	EnterCriticalSection( &m_csTaskList );

	// 若已经执行完则应该退出
	if ( NULL == m_lstTaskHead )
	{
		LeaveCriticalSection( &m_csTaskList );
		return;
	}

	if ( ! m_pDlg->IsCurrentMarked() )
	{
		if ( 0 == m_iProcessing )
		{
			m_iCurrent	= 0;
			m_iTotal	= 0;
			m_iProcessing	= 0;
			
			LeaveCriticalSection( &m_csTaskList );
			return;
		}

		m_iProcessing--;
	}

	if (m_iProcessing+1 > MAX_MARKED_COUNT)
	{
		m_iProcessing -= MAX_MARKED_COUNT;
//		TaskSubmitPacked( MAX_MARKED_COUNT );
//		TaskSubmitPacked( m_iProcessing+1 );
		SubmitMarks2Server( MAX_MARKED_COUNT )	;
		SubmitMarks2Server( m_iProcessing+1 )		;
	}
	else
//		TaskSubmitPacked( m_iProcessing+1 );
		SubmitMarks2Server( m_iProcessing+1 );

	m_iCurrent	= 0;
	m_iTotal	= 0;
	m_iProcessing	= 0;
	LeaveCriticalSection( &m_csTaskList );
}

//,begin
void CThreadTaskServer::GetPrevTask()
{
	// 存储当前笔迹文件	??
	if ( NULL != m_pTaskCurrent->pPrevTask )
		m_pTaskCurrent	= m_pTaskCurrent->pPrevTask;

	m_pDlg->SetCurrentTask( m_pTaskCurrent );

	m_iCurrent	--;
}

//5-01-28,begin
void CThreadTaskServer::GetNextTask()
{
	EnterCriticalSection( &m_csTaskList );
	if ( NULL != m_pTaskCurrent->pNextTask )
		m_pTaskCurrent	= m_pTaskCurrent->pNextTask;
	else
		m_pTaskCurrent	= NULL;
	m_pDlg->SetCurrentTask( m_pTaskCurrent );


	m_iCurrent	++;
	if ( m_iCurrent > m_iProcessing )
	{
		m_iProcessing	= m_iCurrent;
		m_iPriority--;
	}
	// 当用户完成完10份job发送一次请求
	//if ( m_iProcessing == 10 )
	//	this->RequestTask(m_iTotal - m_iProcessing);

	// 总数
	if ( m_enumState == channelFinishing )
	{

		if ( m_iProcessing >= MAX_MARKED_FINISHING_COUNT )
		{
			// 上缴
//			TaskSubmitPacked( MAX_SUBMIT_FINISHING_COUNT );
			SubmitMarks2Server( MAX_SUBMIT_FINISHING_COUNT );

			//		m_iTotal	-= MAX_SUBMIT_COUNT;	将在上传时执行变它的值

			// 设置m_iCuurent
			// 设置m_iProcessing
			m_iCurrent	-= MAX_SUBMIT_FINISHING_COUNT;
			m_iProcessing	-= MAX_SUBMIT_FINISHING_COUNT;
		}
		else if ( m_iTotal - m_iProcessing < 2 )
		{
//			RequestTask( 0 );
		}
	}
	else if ( channelPreGrad == m_enumState )
	{
		if ( m_iProcessing >= MAX_MARKED_FINISHING_COUNT )
		{
			// 删除
			SubmitMarks2Server( MAX_SUBMIT_FINISHING_COUNT );

			// 设置m_iCuurent
			// 设置m_iProcessing
			m_iCurrent	-= MAX_SUBMIT_FINISHING_COUNT;
			m_iProcessing	-= MAX_SUBMIT_FINISHING_COUNT;
		}
	}
	else if ( m_iProcessing >= MAX_MARKED_COUNT )
	{
		// 上缴
//		TaskSubmitPacked( MAX_SUBMIT_COUNT );
		SubmitMarks2Server ( MAX_SUBMIT_COUNT )	;

//		m_iTotal	-= MAX_SUBMIT_COUNT;	将在上传时执行变它的值

		// 设置m_iCuurent
		// 设置m_iProcessing
		m_iCurrent	-= MAX_SUBMIT_COUNT;
		m_iProcessing	-= MAX_SUBMIT_COUNT;
	}
	LeaveCriticalSection( &m_csTaskList );

	return;

	// 存储当前笔迹文件	??
}
//5-01-28,end

/*
//,begin
void CThreadTaskServer::UpdateMarkedCnt()
{	
	if( m_iCurrent >= m_iProcessing)
	{
		m_iMarkedCnt ++		;
		m_pDlg->DisplayMarkedCnt(m_iMarkedCnt);	
	}
	else
		return;
}
//,end
*/
CThreadTaskServer::CThreadTaskServer()
{
	InitializeCriticalSection( &m_csTaskList );

	m_pChannelInfo	= new CHANNEL_INFO;

	m_pNet	= new CNetServer(this);

	m_pNet->StartNetServer( );

	m_bServerStarted	= FALSE;

	//5-01-24,begin
//	m_iLastRequestPendingCnt = 0	;
	//5-01-24,end

	//,begin
	m_iCount				 = 0	;
	m_iRollCount			 = 0	;
//	m_posProcessing			 = m_lstFreshTask.GetHeadPosition();
	m_bLogout				 = FALSE;
	//,end

	m_lstTaskTail	= NULL;
	m_lstTaskHead	= NULL;

	//m_lstCheckHead	= NULL;
	//m_lstCheckTail	= NULL;

	m_enumState	= 0;
}

CThreadTaskServer::~CThreadTaskServer()
{
	if ( m_bServerStarted )
	{
		Stop ();
		Sleep(500);

		m_pNet->StopNetServer( );
		Sleep(500);
	}

	delete	m_pNet;

	while( NULL != m_lstTaskHead )
	{
		m_lstTaskTail	= m_lstTaskHead->pNextTask;
		delete	m_lstTaskHead;
		m_lstTaskHead	= m_lstTaskTail;
	}

	//while( NULL != m_lstCheckHead )
	//{
	//	m_lstCheckTail	= m_lstCheckHead->pNextTask;
	//	delete	m_lstCheckHead;
	//	m_lstCheckHead	= m_lstCheckTail;
	//}

	DeleteCriticalSection( &m_csTaskList );

	delete	m_pChannelInfo;
}

BOOL CThreadTaskServer::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadTaskServer::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadTaskServer, CWinThread)
	ON_THREAD_MESSAGE( WM_EXIT_THREAD, OnAppExitMessage )
	ON_THREAD_MESSAGE( WM_SCOREDLG_HWND, OnScoreDlgHWnd )
END_MESSAGE_MAP()


// CThreadTaskServer 消息处理程序

void CThreadTaskServer::OnAppExitMessage(WPARAM wParam, LPARAM lParam)
{
	AfxEndThread( 888 );
}

void CThreadTaskServer::OnScoreDlgHWnd(WPARAM wParam, LPARAM lParam)
{
//	m_pFrame	= (CMainFrame*)wParam;
	m_pDlg	= (CDlgManager*)wParam;
}

void CThreadTaskServer::OnTimer(UINT nIDEvent)
{
//	if ( m_bConnected )
	if ( !m_bLogout )
		m_pNet->OnTimer(nIDEvent);
}
//
//BOOL CThreadTaskServer::FindJobOnDisk(int iFormatSheetID)
//{
//	_chdir( m_strUserPath );
//
//	CString		str		;
//	CFileFind	finder	;
//	str.Format("%d.gif", iFormatSheetID);
//	BOOL bfind = finder.FindFile(str);
//	if (bfind == TRUE)
//	{
//		bfind = finder.FindNextFile();
//		finder.Close();
//		return TRUE;
//	}
//	else
//	{
//		finder.Close();
//		return FALSE;
//	}
//}

//
//void CThreadTaskServer::OnServerCheckJob(LPUSER_MESSAGE lpUserMsg)
//{
//	LPPACK_UNCHECKED_ON_DISK pTaskOnDisk;
//	pTaskOnDisk = (LPPACK_UNCHECKED_ON_DISK)lpUserMsg;
//
//	LPPACK_CHECKED_RESULT p ;
//
//	p->iJobCnt = 0;
//	int iCount;
//	iCount = pTaskOnDisk->iJobCnt;
//	for (int i = 0; i < iCount; i++)
//	{
//		BOOL bFind;
//		bFind = FindJobOnDisk(pTaskOnDisk->iFormatSheetID[i]);
//		if (bFind == FALSE)
//		{
//			p->iFormatSheetID[p->iJobCnt] = pTaskOnDisk->iFormatSheetID[i];
//			p->iJobCnt++;
//		}
//	}
//	this->ClientCheckedResult(p);
//}
//
//void CThreadTaskServer::ClientCheckedResult(LPPACK_CHECKED_RESULT lpCheckedResult)
//{
//	LPPACK_CHECKED_RESULT pCheckedResult;
//	LPPACKAGE lpPackage;
//
//	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead( );
//	
//	DWORD dwResult;
//	dwResult = sizeof(BYTE) + 2 * sizeof(int) + lpCheckedResult->iJobCnt * sizeof(int);
//	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + dwResult;
//	lpPackage->packageHeader.wVerify1		= 8721;
//	lpPackage->packageHeader.wVerify2		= 8721;
//	lpPackage->packageHeader.yTypePackage	= 0;
//
//	pCheckedResult	= (LPPACK_CHECKED_RESULT)(&(lpPackage->body));
//
//	pCheckedResult->yUserType		= MSG_CLIENT_SUBMIT_CUTSHEETNO	;
//	pCheckedResult->iJobCnt	= lpCheckedResult->iJobCnt	;
//	pCheckedResult->iMachineID		= lpCheckedResult->iMachineID	;
//	for (int i = 0; i < pCheckedResult->iJobCnt; i++)
//	{
//		pCheckedResult->iFormatSheetID[i] = lpCheckedResult->iFormatSheetID[i];
//	}
//	m_pNet->SendToServer(lpPackage);
//}

// Admin 使用的函数
void CThreadTaskServer::OnKickout ( int iMachineID, int iIDJob )
{
/*	LPPACKAGE		lpPackage	;
	LPPACK_KICKOUT	lpKick		;

	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead( );
	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_KICKOUT);
	lpPackage->packageHeader.wVerify1		= 8721;
	lpPackage->packageHeader.wVerify2		= 8721;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0;

	lpKick	= (LPPACK_KICKOUT)(&(lpPackage->body));
	lpKick->yUserType  = MSG_CLIENT_KICKOUT	;	
	lpKick->iMachineID = iMachineID			;
	lpKick->iIDJob= iIDJob		;

	m_pNet->SendToServer(lpPackage);*/
}

// 组长使用的函数
void CThreadTaskServer::OnKickout ( int iMachineID )
{
/*	LPPACKAGE		lpPackage	;
	LPPACK_KICKOUT	lpKick		;

	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead( );
	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_KICKOUT);
	lpPackage->packageHeader.wVerify1		= 8721;
	lpPackage->packageHeader.wVerify2		= 8721;
//, begin
	lpPackage->packageHeader.yChannelState	= m_enumState;
//, ends
	lpPackage->packageHeader.yTypePackage	= 0;

	lpKick	= (LPPACK_KICKOUT)(&(lpPackage->body))		;
	lpKick->yUserType  = MSG_CLIENT_KICKOUT				;	
	lpKick->iMachineID = iMachineID						;
	lpKick->iIDJob= m_pChannelInfo->iIDJob	;

	m_pNet->SendToServer(lpPackage);*/
}

int CThreadTaskServer::Run()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CWinThread::Run();
}
