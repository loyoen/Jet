#include "StdAfx.h"
#include ".\SocketConnServer.h"
#include "ClientConnServer.h"
#include "NetServer.h"

void CSocketConnServer::OnReceive( int nErrorCode )
{
	// 先判断状态
	if ( GetState() != connected && GetState() != attached && GetState() != closed )
	{
		// 此时应该停止自动登陆
		return;
	}
	
	// 暂时关闭接收的消息响应
	AsyncSelect( FD_CLOSE | FD_WRITE );

	if ( nErrorCode != 0 )
	{
		m_pClientConnServer->m_bConnected	= 0;
		Close();
		return ;
	}

	//第一次接收, 初始化m_mid, 保存数据结构, 分配内存
	if ( m_mid.m_bFree )
	{
		//第一次接收，要创建缓存
		m_mid.m_bFree	= FALSE;

		//接受头四个字节，得到要传的包的大小，假设一次可以接收完4个字节，如果不能，未处理
		PACKAGE_HEADER	header;
		LPBYTE			lpPtr;
		DWORD	dwSize	= sizeof(DWORD) + 2 * sizeof(WORD);
		lpPtr			= (LPBYTE)&header;
		DWORD	dwLen		= Receive( lpPtr, dwSize );
		while ( dwLen < dwSize ) 
		{
			lpPtr	= (LPBYTE)&header + dwLen;
			dwLen	+= Receive( lpPtr, dwSize - dwLen );
		}

		if ( header.dwLength > SIZE_EXLARGE_MEM )
		{
			dwLen	= 1;
			char	chTemp[1024];
			// 读空全部缓冲
			while ( dwLen > 0 )
			{
				dwLen	= Receive( chTemp, 1024 );
			}

#ifdef	SWITCH_AUTO_TESTING
			// error
			CString	strID;
			strID.Format("%d ", header.dwLength);
			strID		+= _T("SIZE_EXLARGE_MEM");

			CString	strError;
			strError.Format( "%s", "打印系统检测到异常！\n" );
			strError	+= _T("请将问题写入测试报告\n警告代码：");

			MessageBox( NULL, strError + strID, "请写入测试报告", MB_OK );
#endif

			//为下次接收作准备
			m_mid.Init();

			AsyncSelect( FD_READ | FD_CLOSE | FD_WRITE );
			return;
		}

		if ( (header.wVerify_1 != 8721 ) ||
			(header.wVerify_2 != 8721) )
		{
			dwLen	= 1;
			char	chTemp[1024];
			// 读空全部缓冲
			while ( dwLen > 0 )
			{
				dwLen	= Receive( chTemp, 1024 );
			}

#ifdef	SWITCH_AUTO_TESTING
			// error
			CString strID;
			strID.Format( "验证码 [%d] [%d]", header.wVerify_1, header.wVerify_2 );

			CString	strError;
			strError.Format( "%s", "打印系统检测到验证码不对！\n" );
			strError	+= _T("请将问题写入测试报告\n警告代码：");

			MessageBox( NULL, strError + strID, "请写入测试报告", MB_OK );
#endif

			//为下次接收作准备
			m_mid.Init();

			AsyncSelect( FD_READ | FD_CLOSE | FD_WRITE );
			return;
		}

		//创建一个接收的缓存
		char* lpBuf	= new char[header.dwLength];
		if ( NULL == lpBuf )
		{
			AfxMessageBox( "内存不足, 无法接受服务器传送来的数据\n请退出程序" );
			return;
		}

		//初始化包头
		memset( lpBuf, 0, header.dwLength );
		((LPPACKAGE)lpBuf)->pckHeader.dwLength	= header.dwLength;

		// 保存分配的缓冲信息
		m_mid.RecInit( lpBuf, header.dwLength );
		// 收到了长度信息，保存起来
		m_mid.Fresh( dwSize );
	}	// 初始化mid结束


	// 接受一次一次传来的数据，直接读入缓存
	DWORD dwLen	= Receive( m_mid.m_lpNow, m_mid.m_dwLeft );
	m_mid.Fresh( dwLen );

	// 如果接收完了
	if ( m_mid.m_dwLeft	== 0 )
	{
		// 写入文档
		m_pClientConnServer->OnReceivePackage( (LPPACKAGE)m_mid.m_lpBegin );

		// 接受成功，删除缓存
		delete []m_mid.m_lpBegin;

		// 为下次接收作准备
		m_mid.Init();
	}

	// 重新准备响应读操作
	AsyncSelect( FD_READ | FD_CLOSE | FD_WRITE );
}

void CSocketConnServer::OnConnect( int nErrorCode )
{
	// 连接出错的情况处理
	if (nErrorCode)
	{
#ifdef	_DEBUG
		afxDump << "Failed to connect server, errorcode=" << nErrorCode << "lastError:" << WSAGetLastError() << "\n";
#endif
		if ( !m_pClientConnServer->m_bReLogon )
		{
//			AfxMessageBox( "连接失败, 请退出程序后重新启动!" );
			m_pClientConnServer->OnConnFailed();
		}

		m_pClientConnServer->m_bConnected	= FALSE;

		Close()	;
		m_SocketData.hSocket	= INVALID_SOCKET;

//		m_pClient->OnNetworkConnected(FALSE);

		return	;
	}

	/* Set internal socket send buffer to twice the programs buffer size
	* this should fix the speed problems some users have reported
	*/
/*		DWORD value;
		int len = sizeof(value);
		GetSockOpt(SO_SNDBUF, &value, &len);
		if (value < (MSG_BUFFER_SIZE))
		{
			value = MSG_BUFFER_SIZE ;
			SetSockOpt(SO_SNDBUF, &value, sizeof(value));
		}
		m_pClient->m_bConnected=TRUE;
        m_pClient->OnNetworkConnected(TRUE);
*/

	m_pClientConnServer->m_bConnected	= TRUE	;
	m_pClientConnServer->m_bWantConnect	= TRUE	;

	// 现在如果需要重新登陆则发重新登陆包
	if ( m_pClientConnServer->m_bReLogon )
	{
		// 如果网络已经连通, 那么发出ReLogin()包
		(m_pClientConnServer->m_pNetServer)->ReLogin( )	;
		m_pClientConnServer->m_bReLogon	= FALSE			;
	}
}

// 特别要处理当用户已经Logout后不应该重登陆
void CSocketConnServer::OnClose( int nErrorCode )
{
	if ( nErrorCode	!= 0 )
	{
		// 系统发现网络断开或服务器故障
		m_pClientConnServer->m_bConnected	= 0;
		m_pClientConnServer->m_bReLogon		= TRUE;
	}

	if ( !m_mid.m_bFree )
	{
		delete [](m_mid.m_lpBegin);
		m_mid.Init();
	}

	Close();

#ifdef	_DEBUG
	afxDump << "Connection closed!\n";
#endif
}

void CSocketConnServer::OnSend( int nErrorCode )
{
//	AsyncSelect( FD_READ | FD_CLOSE | FD_WRITE );

//	m_bCanSend	= TRUE;

	if ( nErrorCode	!= 0 )
	{
		m_pClientConnServer->m_bConnected	= 0;
#ifdef	_DEBUG
		afxDump << "OnSend	Connection Disconnect!"<<nErrorCode<<"\n";
#endif
		Close();
		return ;
	}
}

int CSocketConnServer::Send(const void* lpBuf, int nBufLen, int nFlags)
{
//	m_bCanSend	= FALSE;

//	AsyncSelect(FD_READ | FD_CLOSE | FD_WRITE);

	return	CAsyncSocketEx::Send( lpBuf, nBufLen, nFlags );
}

CSocketConnServer::CSocketConnServer( CClientConnServer* pClientConnServer )
{
	ASSERT( NULL != pClientConnServer );

	m_pClientConnServer	= pClientConnServer;

//	m_bFinishPackage	= FALSE;
//	m_bCanSend			= TRUE;

	m_lpPackage			= NULL;
}

CSocketConnServer::~CSocketConnServer(void)
{
	if( GetState() == connected )
		Close();
}
