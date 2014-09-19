#pragma once

#ifndef	__SOCKET_CONN_SERVER_H__
#define	__SOCKET_CONN_SERVER_H__

#include "AsyncSocketEx.h"
#include "Protocol.h"
#include "package.h"
#include "mid.h"

class CClientConnServer;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
��������:
	1)	����������Ϣ: ���� ���� ���� �ر�
	2)	����������Ϣ����Ӧ״̬�������ϲ� CClientConnServer ��״̬��־
	3)	�ṩ Send �� Connect �������ϲ�
--------------------------------------------------------------------------------*/
class CSocketConnServer : public CAsyncSocketEx
{
public:
//	BOOL	CanSend( ){ return m_bCanSend; };

	CSocketConnServer( CClientConnServer* pClientConnServer );
	~CSocketConnServer( void );

	//{{AFX_VIRTUAL(CSocketConnServer)
public:
	virtual void OnReceive( int nErrorCode );
	virtual void OnClose( int nErrorCode )	;
	virtual void OnSend( int nErrorCode )	;
	virtual void OnConnect( int nErrorCode );

	// Sends data to a connected socket.
	virtual int Send( const void* lpBuf, int nBufLen, int nFlags = 0 );
	//}}AFX_VIRTUAL

protected:
	CClientConnServer *	m_pClientConnServer	;

//	BOOL	m_bFinishPackage;
//	BOOL	m_bCanSend		;

	LPPACKAGE	m_lpPackage	;

	CMid	m_mid	;

};

#endif	// __SOCKET_CONN_SERVER_H__
