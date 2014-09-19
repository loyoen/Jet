#include "StdAfx.h"
#include ".\ClientConnServer.h"
#include "NetServer.h"
#include "Protocol.h"

//==================================================//
//													//
//					�� �� �� ��						//
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

	// ����� Login �� ReLogin ��, ��ֱ�ӷ�
	if ( lpPackage->pckHeader.yType == MSG_CLIENT_RELOGIN 
		|| lpPackage->pckHeader.yType == MSG_CLIENT_LOGIN )
	{
		m_pSocket->Send( lpPackage, lpPackage->pckHeader.dwLength );
		m_pNetServer->DownReturnAddress(lpPackage);
		TRACE0("Send ReLogin() Package or Login() Package\n");

		return TRUE;
	}

	// ������δ��ͨ���򲻽�����job���ŵ�������
	// ���������������ͨ��������������job��
    if(  m_bConnected== -1 )
	{
		if ( lpPackage->pckHeader.yType == MSG_CLIENT_JOB_REQUEST)
		{
			m_pNetServer->DownReturnAddress( lpPackage );
			return TRUE;
		}
	}
	//m_pSocket->Send( lpPackage, lpPackage->pckHeader.dwLength );
	// ������ӵ�������
	m_lstSendBuffer.AddTail( lpPackage );

	// ��������
	return	SendPackageList ( )	;
}

// �˺�����Ҫִ��д
BOOL CClientConnServer::SendLogout( LPPACKAGE lpPackage )
{
	int		iSendReturn	;

	if ( lpPackage == NULL ) 
		return TRUE;

	m_lstSendBuffer.AddTail( lpPackage );

	// ����ע����־, �Ա㵱Logout�����������Զ���������
	m_bLogout	= TRUE	;

	SendPackageList ( )	;

	return	TRUE;
}

void CClientConnServer::OnReceivePackage( LPPACKAGE lpPackage )
{
	m_pNetServer->OnReceiveFromServer ( lpPackage );
/*	switch ( lpPackage->packageHeader.yTypePackage )
	{
	// �û��� - ����CTaskServer����
	case	USER_COMMAND:	// ����һ�δ�����û�����, ����ֱ�ӽ����û�����
		m_pNetServer->OnReceiveFromServer ( lpPackage );
		break;

	case	NET_SCATTER:	// ���ָ��С��, ��Ҫgather���͸��û�����
		break;

	case	NET_CHECK:		// �ͻ��˼������״̬��(��Server�յ�Check��ʱ���̽�������)
		break;

	//case	NET_PONG:		// ������, �û�PONG������
	case	NET_PING:		// ������, ������PING�û�
		break;

	case	NET_RELOGIN_SUCCESS:
		// ��������������������, ��Ҫ����ǰ��job�б��͸�������
		{
			LPPACK_RELOGIN_SUCCESS	lpPackReLogin;
			lpPackReLogin	= (LPPACK_RELOGIN_SUCCESS)(&(lpPackage->body));
	
			m_pNetServer->OnNetReloginOK ( );

			// ����������״̬
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
			strID		+= _T("default");		//����������

			CString	strError;
			strError.Format( "%s", "��ӡϵͳ��⵽���ܵ��쳣:\n" );
			strError	+= _T("CClientConnServer���ְ������Ͳ���\n\n");
			strError	+= _T("�뽫����д����Ա���\n������룺\n");
			// error
//			while( TRUE )
			{
				MessageBox( NULL, strError + strID, "��д����Ա���", MB_OK );
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

// ���ӳɹ���, ��������а���Ҫ�������ȷ��Ͱ�
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

// ����Ƿ���Ҫ�Զ�������������
void CClientConnServer::OnTimer(UINT nIDEvent)
{
	static int iCount	= 0;

	// �����ӶϿ�ʱ������Ҫ��������
	if ( !m_bConnected )
	{
		// ���Ͽ���ϵͳ���ڵ�½״̬(�û���Login�ɹ�����Logoutǰ)
		if ( m_bWantConnect )
		{
			iCount ++;
			if ( iCount	== 5 )	// 5Ϊ�����������������һ��
			{
				// m_bConnected	= FALSE;
				// ��һ������ظ���½����
				if ( OpenConnection( (SOCKADDR*)(&m_SockAddr), m_nSockAddrLen ) )
				{
					// ���ӳɹ���, ��������а���Ҫ�������ȷ��Ͱ�
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

// ���Ͱ����б��еĵ�Ԫ��Server
BOOL CClientConnServer::SendPackageList ( )
{
	int			iRemainBytes;	// ʣ����Ҫ���͵�bytes
	int			iSentBytes	;	// �Ѿ��ɹ����͵�bytes
	int			iSendReturn	;	// �˴η��ͳɹ���bytes
	LPBYTE		lpPtr		;	// ��ǰ���͵�ָ��λ��
	LPPACKAGE	lpPackage	;	// ��ǰ���ڷ��͵�package

	// ��δ���Ӻ���򵥷���ʧ��
	if (  m_bConnected== -1 )
		return	FALSE	;

	// ѭ�����б�ͷ��ȡ�ð�������
	while ( !m_lstSendBuffer.IsEmpty() )
	{
		// ȡ�õ�ǰ�б�ͷ
		lpPackage	= (LPPACKAGE)m_lstSendBuffer.GetHead( )	;
		lpPtr		= (LPBYTE)lpPackage						;
		iRemainBytes= lpPackage->pckHeader.dwLength;   // �޸Ĺ�
		iSentBytes	= 0										;

		// ���ʹ�package
		while ( TRUE )
		{
			// ����socket���з���
			iSendReturn	= m_pSocket->Send ( lpPtr, iRemainBytes );

			// ����������򷵻�ʧ��
			if ( SOCKET_ERROR == iSendReturn )
				return	FALSE	;

			iRemainBytes-= iSendReturn	;

			// �����굱ǰ��
			if ( 0 == iRemainBytes )
			{
				m_lstSendBuffer.RemoveHead( )	;
				m_pNetServer->DownReturnAddress( lpPackage );
				break	;
			}

			// δ������, �������͵�ǰ��
			iSentBytes	+= iSendReturn	;
			lpPtr		+= iSendReturn	;
		}
	}

	if ( m_bLogout )
		m_bWantConnect	= FALSE;	// �Ѿ�ע��, ����Ҫ�Զ�����

	return	TRUE;
}