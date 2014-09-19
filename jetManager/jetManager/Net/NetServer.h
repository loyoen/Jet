/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[Module]
	CNetServer - Client version

[Description]
	This class provide service for netwrok transformation
	It contains some thread to imply all the functions.

[Authors]
	Jicheng Hu, Wuhan University

[Remain Problems]
	�������ʧ�ܣ����֪ͨ��CNetServer���û������

[Bug Reports]

[Revision History]
	When		Who				Where				Description
------------------------------------------------------------------------
------------------------------------------------------------------------ 
---------------------------------------------------------------------------------*/

#pragma once

#include "Protocol.h"
#include "..\SocketConnServer.h"
//#include "..\ThreadConnServer.h"
//#include "..\ThreadConnClient.h"
//#include "..\ThreadServeClient.h"

class CThreadTaskServer;
class CNetServer
{
// ���ýӿ�
public:
	BOOL	StartNetServer( )	;	// ��������
	BOOL	StopNetServer( )	;	// ֹͣ����

	BOOL	PostMessageToServer( LPPACKAGE_HEADER lpPackageHeader )		;
//	BOOL	PostMessageToOperator( LPPACKAGE_HEADER lpPackageHeader )	;

	BOOL	Connect ( );	// ������������, �������������������ݷ�������Operator��

	BOOL	SendToServer( LPPACKAGE lpPackage );
	void	OnReceiveFromServer( LPPACKAGE lpPackage );
	void	OnConnServerFailed( );

//	void	LoopConnServer( );
//	void	LoopConnClient( );
//	void	LoopServeClient( );
	void	OnNetReloginOK();
	void	OnServerRestarted();

	BOOL	SendLogoutPackage( LPPACKAGE lpPackage );

// ��������ӿ�
// Attributes setting
public:
//	void	AddServer ( CString strAddr, u_short sinport );		// ���ݵ�ַ�Ͷ˿���ӷ�����
//	void	AddOperator ( CString strAddr, u_short sinport );	// ���ݵ�ַ�Ͷ˿����Operator
	void	SetMainServer ( CString strAddr, u_short sinport );	// �������������ĵ�ַ�Ͷ˿�

// ��������
protected:
//	CPtrList	m_listPtrServer;	// ��������ַ�б�
//	BYTE		m_yServerCnt;		// ����������

	//CPtrList	m_listPtrOperator;	// Operator�ĵ�ַ�б�
	//BYTE		m_yOperatorCnt;		// Operator�ĸ���

	SOCKADDR_IN		m_addrMainServer;

// Attributes
// ��������
protected:
//	CSocketConnServer*	m_pSocketConnServer;
	CThreadTaskServer*	m_pTaskServer;

// ��������
protected:
	CClientConnServer*	m_pClientConnServer;

// �����߳�
protected:
//	CThreadConnClient*	m_pThreadConnClient;
//	CThreadServeClient*	m_pThreadServeClient;

// ��־
protected:
	BOOL	m_bServerConnected;		// ���������ӳɹ���־
	BOOL	m_bServerVerified;		// ��½������security��֤�ɹ���־
	BOOL	m_bOperatorConnected;	// Operator���ӳɹ���־

// �������ݹ���
protected:

public:
	CNetServer(CThreadTaskServer* pServer);
	~CNetServer(void);

	void	DownReturnAddress(LPPACKAGE lpPackage);
	void	OnTimer(UINT nIDEvent);
	void	ReLogin ( );
};
