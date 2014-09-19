#pragma once

#include "Protocol.h"
#include "SocketConnServer.h"

class CNetServer;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
��������:
	1)	�ص�½���߼��жϺ�ʵ��
	2)	�Զ����ӵ��߼��ж���ʵ�� ( �� OnTimer ��ʵ�� )
	3)	������Ĺ���ʹ����߼�
	4)	Ϊ�ϲ��ṩ ���� ���Ͱ� ����Logout�� ����
	5)	�����յ��İ��͵��ϲ㴦��
	6)	������ʧ�ܵ���Ϣ�͵��ϲ㴦��
--------------------------------------------------------------------------------*/
class CClientConnServer
{
// ���ýӿ�
public:
//	BOOL	IsConnected( )	;
	BOOL	OpenConnection(  SOCKADDR* lpSockAddr, int nSockAddrLen );
	BOOL	Connect( SOCKADDR* lpSockAddr, int nSockAddrLen );
	BOOL	Send( LPPACKAGE lpPackage )				;
	BOOL	SendLogout( LPPACKAGE lpPackage )		;

	// ͨ��ʱ���������Զ������߼���ʵ��
	void	OnTimer(UINT nIDEvent)	;

	// ���յ��İ����͵��ϲ㴦��
	void	OnReceivePackage( LPPACKAGE lpPackage )	;
	// ������ʧ�ܵ���Ϣ�͵��ϲ㴦��
	void	OnConnFailed( )	;

// ��������
protected:
	// ����Ͽ�ʱ�ݴ�Ҫ���͵İ�
	CPtrList	m_lstSendBuffer	;
private:
	SOCKADDR_IN m_SockAddr		;
	int			m_nSockAddrLen	;	

// ״̬��־
/********************************************************************
״̬��־����ִ�б��λ��
	m_bConnected:
		CSocketConnServer::OnConnect
	m_bWantConnect:
		CSocketConnServer::OnConnect
	m_bRelogon:
		CSocketConnServer::OnConnect	����Ҫ���µ�½���ص�½��
********************************************************************/
protected:
public:
	// �����Ǳ�����, �����ǹ�����
	BOOL	m_bConnected	;	// �Ƿ��Ѿ����Ӻ�
	BOOL	m_bReLogon		;	// �Ƿ���Ҫ�ص�½(����������������������Ҫ�ص�½)
	
	// �Ƿ��ǵ�һ������
	BOOL	m_bWantConnect	;	// �Ƿ���Ҫ�Զ�����
	BOOL	m_bLogout		;	// �û��Ƿ��Ѿ�����ע������

// �ڲ�����
protected:
	BOOL	SendPackageList ( )		;	// ������б�, ���б��еİ����ͳ�ȥ

// �ؼ�����ָ��
public:
	// �ϲ�
	CNetServer *	m_pNetServer	;
protected:
	// �²�
	CSocketConnServer*	m_pSocket	;

protected:
//	BOOL	m_bVerified;

public:
	CClientConnServer( CNetServer* pServer );
	~CClientConnServer(void);
};
