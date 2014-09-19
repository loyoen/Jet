#pragma once

#ifndef	__SOCKET_CONN_SERVER_H__
#define	__SOCKET_CONN_SERVER_H__

#include "AsyncSocketEx.h"
#include "Protocol.h"
#include "package.h"
#include "mid.h"

class CClientConnServer;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
功能描述:
	1)	处理网络消息: 连接 接收 发送 关闭
	2)	根据网络消息的响应状态来设置上层 CClientConnServer 的状态标志
	3)	提供 Send 和 Connect 方法给上层
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
