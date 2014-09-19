#include "StdAfx.h"
#include ".\ClientConnServer.h"
#include "NetServer.h"
#include "Protocol.h"

//==================================================//
//													//
//					调 用 接 口						//
//													//
//==================================================//

BOOL CClientConnServer::Connect( SOCKADDR * lpSockAddr, int nSockAddrLen )
{
	return	OpenConnection( lpSockAddr, nSockAddrLen );
}

BOOL CClientConnServer::Send( LPPACKAGE lpPackage )
{
	//m_pSocket->Send( lpPackage, lpPackage->packageHeader.dwSize ); //by jarry

	if ( lpPackage	== NULL ) return TRUE;

	// 如果是 Login 或 ReLogin 包, 就直接发
	if ( lpPackage->pckHeader.yType == MSG_CLIENT_RELOGIN 
		|| lpPackage->pckHeader.yType == MSG_CLIENT_LOGIN )
	{
		m_pSocket->Send( lpPackage, lpPackage->pckHeader.dwLength );
		m_pNetServer->DownReturnAddress(lpPackage);
		TRACE0("Send ReLogin() Package or Login() Package\n");

		return TRUE;
	}

	// 若网络未连通好则不将请求job包放到链表中
	// 否则会引起网络连通后连续发送请求job包
    if(  m_bConnected== -1 )
	{
		if ( lpPackage->pckHeader.yType == MSG_CLIENT_JOB_REQUEST)
		{
			m_pNetServer->DownReturnAddress( lpPackage );
			return TRUE;
		}
	}
	//m_pSocket->Send( lpPackage, lpPackage->pckHeader.dwLength );
	// 将包添加到链表中
	m_lstSendBuffer.AddTail( lpPackage );

	// 发送链表
	return	SendPackageList ( )	;
}

// 此函数需要执行写
BOOL CClientConnServer::SendLogout( LPPACKAGE lpPackage )
{
	int		iSendReturn	;

	if ( lpPackage == NULL ) 
		return TRUE;

	m_lstSendBuffer.AddTail( lpPackage );

	// 设置注销标志, 以便当Logout包发出后不再自动连接网络
	m_bLogout	= TRUE	;

	SendPackageList ( )	;

	return	TRUE;
}

void CClientConnServer::OnReceivePackage( LPPACKAGE lpPackage )
{
	m_pNetServer->OnReceiveFromServer ( lpPackage );
/*	switch ( lpPackage->packageHeader.yTypePackage )
	{
	// 用户包 - 交给CTaskServer处理
	case	USER_COMMAND:	// 可以一次传完的用户数据, 可以直接交给用户处理
		m_pNetServer->OnReceiveFromServer ( lpPackage );
		break;

	case	NET_SCATTER:	// 被分割的小包, 需要gather后送给用户处理
		break;

	case	NET_CHECK:		// 客户端检查网络状态包(当Server收到Check包时立刻将包返回)
		break;

	//case	NET_PONG:		// 心跳包, 用户PONG服务器
	case	NET_PING:		// 心跳包, 服务器PING用户
		break;

	case	NET_RELOGIN_SUCCESS:
		// 若服务器是重新启动的, 则要将当前的job列表发送给服务器
		{
			LPPACK_RELOGIN_SUCCESS	lpPackReLogin;
			lpPackReLogin	= (LPPACK_RELOGIN_SUCCESS)(&(lpPackage->body));
	
			m_pNetServer->OnNetReloginOK ( );

			// 服务器重启状态
			if ( lpPackReLogin->bServerRestarted )
			{
				m_pNetServer->OnServerRestarted ( );
			}
		}
		break;

	default:
		{
#ifdef	SWITCH_AUTO_TESTING
			CString strID;
			strID.Format ( "%d[%d]\ndwSize=%d wVerify1=%d wVerify2=%d\nyUserType=%d",
				lpPackage->packageHeader.yTypePackage,
				lpPackage->packageHeader.yChannelState,
				lpPackage->packageHeader.dwSize,
				lpPackage->packageHeader.wVerify1,
				lpPackage->packageHeader.wVerify2,
				lpPackage->body.userMsg.yUserType );
			strID		+= _T("default");		//错误类型码

			CString	strError;
			strError.Format( "%s", "打印系统检测到可能的异常:\n" );
			strError	+= _T("CClientConnServer发现包的类型不对\n\n");
			strError	+= _T("请将问题写入测试报告\n警告代码：\n");
			// error
//			while( TRUE )
			{
				MessageBox( NULL, strError + strID, "请写入测试报告", MB_OK );
			}
#endif
			return;
		}
	}*/
}

void CClientConnServer::OnConnFailed( )
{
	m_pNetServer->OnConnServerFailed();
}

CClientConnServer::CClientConnServer( CNetServer* pServer )
: m_bReLogon(FALSE)
{
	m_pNetServer	= pServer;
	m_pSocket		= new CSocketConnServer(this);

	m_bWantConnect	= FALSE;
	m_bLogout		= FALSE;
}

CClientConnServer::~CClientConnServer(void)
{
	delete	m_pSocket;
}

// 连接成功后, 如果发现有包需要发送则先发送包
BOOL CClientConnServer::OpenConnection( SOCKADDR *lpSockAddr, int nSockAddrLen )
{
	if ( ! m_pSocket->Create( ) )
	{
		::MessageBox( NULL, "Error", "Socket creation failed!", MB_OK|MB_ICONERROR );
		return FALSE;
	}

	m_bConnected	= m_pSocket->Connect( lpSockAddr, nSockAddrLen );

	if( (!m_bConnected) && (GetLastError() != WSAEWOULDBLOCK) )
	{
		// ::MessageBox( NULL, "Error", "Socket Connection Failed", MB_OK|MB_ICONERROR );
#ifdef	_DEBUG
		afxDump << "Connection Failed! ErrorCode=" << GetLastError() << "\n";
#endif
		m_pSocket->Close();
//		m_pSocket->m_SocketData.hSocket= INVALID_SOCKET;
		return FALSE;
	}

#ifdef	_DEBUG
//		afxDump << "OpenConnection blocked! ErrorCode=" << GetLastError() << "\n";
#endif

	m_SockAddr.sin_port			= ((SOCKADDR_IN*)lpSockAddr)->sin_port			;
	m_SockAddr.sin_addr.s_addr	= ((SOCKADDR_IN*)lpSockAddr)->sin_addr.s_addr	;
	m_SockAddr.sin_family		= ((SOCKADDR_IN*)lpSockAddr)->sin_family		;
	m_nSockAddrLen				= nSockAddrLen									;

//	m_pSocket->AsyncSelect(  FD_WRITE | FD_CONNECT);

	return	TRUE;
}

// 检查是否需要自动进行网络连接
void CClientConnServer::OnTimer(UINT nIDEvent)
{
	static int iCount	= 0;

	// 当连接断开时可能需要重新连接
	if ( !m_bConnected )
	{
		// 若断开且系统处于登陆状态(用户的Login成功后至Logout前)
		if ( m_bWantConnect )
		{
			iCount ++;
			if ( iCount	== 5 )	// 5为隔几个间隔重新连接一次
			{
				// m_bConnected	= FALSE;
				// 上一层调用重复登陆函数
				if ( OpenConnection( (SOCKADDR*)(&m_SockAddr), m_nSockAddrLen ) )
				{
					// 连接成功后, 如果发现有包需要发送则先发送包
					SendPackageList ( )	;
				}

				iCount	= 0;

#ifdef _DEBUG
				static int iCount2	= 0;
				iCount2++;
				afxDump << "The net is closed, and we connect again. \nTry the " << iCount2 << "th time, the network is connected=" << m_bConnected << "\n"; 
#endif
			}
		}
	}
}

// 发送包裹列表中的单元到Server
BOOL CClientConnServer::SendPackageList ( )
{
	int			iRemainBytes;	// 剩余需要发送的bytes
	int			iSentBytes	;	// 已经成功发送的bytes
	int			iSendReturn	;	// 此次发送成功的bytes
	LPBYTE		lpPtr		;	// 当前发送的指针位置
	LPPACKAGE	lpPackage	;	// 当前正在发送的package

	// 若未连接好则简单返回失败
	if (  m_bConnected== -1 )
		return	FALSE	;

	// 循环从列表头中取得包裹发送
	while ( !m_lstSendBuffer.IsEmpty() )
	{
		// 取得当前列表头
		lpPackage	= (LPPACKAGE)m_lstSendBuffer.GetHead( )	;
		lpPtr		= (LPBYTE)lpPackage						;
		iRemainBytes= lpPackage->pckHeader.dwLength;   // 修改过
		iSentBytes	= 0										;

		// 发送此package
		while ( TRUE )
		{
			// 调用socket进行发送
			iSendReturn	= m_pSocket->Send ( lpPtr, iRemainBytes );

			// 若网络故障则返回失败
			if ( SOCKET_ERROR == iSendReturn )
				return	FALSE	;

			iRemainBytes-= iSendReturn	;

			// 发送完当前包
			if ( 0 == iRemainBytes )
			{
				m_lstSendBuffer.RemoveHead( )	;
				m_pNetServer->DownReturnAddress( lpPackage );
				break	;
			}

			// 未发送完, 继续发送当前包
			iSentBytes	+= iSendReturn	;
			lpPtr		+= iSendReturn	;
		}
	}

	if ( m_bLogout )
		m_bWantConnect	= FALSE;	// 已经注销, 不需要自动连接

	return	TRUE;
}