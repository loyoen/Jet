#include "StdAfx.h"
#include ".\SocketConnServer.h"
#include "ClientConnServer.h"
#include "NetServer.h"

void CSocketConnServer::OnReceive( int nErrorCode )
{
	// ���ж�״̬
	if ( GetState() != connected && GetState() != attached && GetState() != closed )
	{
		// ��ʱӦ��ֹͣ�Զ���½
		return;
	}
	
	// ��ʱ�رս��յ���Ϣ��Ӧ
	AsyncSelect( FD_CLOSE | FD_WRITE );

	if ( nErrorCode != 0 )
	{
		m_pClientConnServer->m_bConnected	= 0;
		Close();
		return ;
	}

	//��һ�ν���, ��ʼ��m_mid, �������ݽṹ, �����ڴ�
	if ( m_mid.m_bFree )
	{
		//��һ�ν��գ�Ҫ��������
		m_mid.m_bFree	= FALSE;

		//����ͷ�ĸ��ֽڣ��õ�Ҫ���İ��Ĵ�С������һ�ο��Խ�����4���ֽڣ�������ܣ�δ����
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
			// ����ȫ������
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
			strError.Format( "%s", "��ӡϵͳ��⵽�쳣��\n" );
			strError	+= _T("�뽫����д����Ա���\n������룺");

			MessageBox( NULL, strError + strID, "��д����Ա���", MB_OK );
#endif

			//Ϊ�´ν�����׼��
			m_mid.Init();

			AsyncSelect( FD_READ | FD_CLOSE | FD_WRITE );
			return;
		}

		if ( (header.wVerify_1 != 8721 ) ||
			(header.wVerify_2 != 8721) )
		{
			dwLen	= 1;
			char	chTemp[1024];
			// ����ȫ������
			while ( dwLen > 0 )
			{
				dwLen	= Receive( chTemp, 1024 );
			}

#ifdef	SWITCH_AUTO_TESTING
			// error
			CString strID;
			strID.Format( "��֤�� [%d] [%d]", header.wVerify_1, header.wVerify_2 );

			CString	strError;
			strError.Format( "%s", "��ӡϵͳ��⵽��֤�벻�ԣ�\n" );
			strError	+= _T("�뽫����д����Ա���\n������룺");

			MessageBox( NULL, strError + strID, "��д����Ա���", MB_OK );
#endif

			//Ϊ�´ν�����׼��
			m_mid.Init();

			AsyncSelect( FD_READ | FD_CLOSE | FD_WRITE );
			return;
		}

		//����һ�����յĻ���
		char* lpBuf	= new char[header.dwLength];
		if ( NULL == lpBuf )
		{
			AfxMessageBox( "�ڴ治��, �޷����ܷ�����������������\n���˳�����" );
			return;
		}

		//��ʼ����ͷ
		memset( lpBuf, 0, header.dwLength );
		((LPPACKAGE)lpBuf)->pckHeader.dwLength	= header.dwLength;

		// �������Ļ�����Ϣ
		m_mid.RecInit( lpBuf, header.dwLength );
		// �յ��˳�����Ϣ����������
		m_mid.Fresh( dwSize );
	}	// ��ʼ��mid����


	// ����һ��һ�δ��������ݣ�ֱ�Ӷ��뻺��
	DWORD dwLen	= Receive( m_mid.m_lpNow, m_mid.m_dwLeft );
	m_mid.Fresh( dwLen );

	// �����������
	if ( m_mid.m_dwLeft	== 0 )
	{
		// д���ĵ�
		m_pClientConnServer->OnReceivePackage( (LPPACKAGE)m_mid.m_lpBegin );

		// ���ܳɹ���ɾ������
		delete []m_mid.m_lpBegin;

		// Ϊ�´ν�����׼��
		m_mid.Init();
	}

	// ����׼����Ӧ������
	AsyncSelect( FD_READ | FD_CLOSE | FD_WRITE );
}

void CSocketConnServer::OnConnect( int nErrorCode )
{
	// ���ӳ�����������
	if (nErrorCode)
	{
#ifdef	_DEBUG
		afxDump << "Failed to connect server, errorcode=" << nErrorCode << "lastError:" << WSAGetLastError() << "\n";
#endif
		if ( !m_pClientConnServer->m_bReLogon )
		{
//			AfxMessageBox( "����ʧ��, ���˳��������������!" );
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

	// ���������Ҫ���µ�½�����µ�½��
	if ( m_pClientConnServer->m_bReLogon )
	{
		// ��������Ѿ���ͨ, ��ô����ReLogin()��
		(m_pClientConnServer->m_pNetServer)->ReLogin( )	;
		m_pClientConnServer->m_bReLogon	= FALSE			;
	}
}

// �ر�Ҫ�����û��Ѿ�Logout��Ӧ���ص�½
void CSocketConnServer::OnClose( int nErrorCode )
{
	if ( nErrorCode	!= 0 )
	{
		// ϵͳ��������Ͽ������������
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
