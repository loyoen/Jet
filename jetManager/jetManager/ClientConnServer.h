#pragma once

#include "Protocol.h"
#include "SocketConnServer.h"

class CNetServer;

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
功能描述:
	1)	重登陆的逻辑判断和实现
	2)	自动连接的逻辑判断与实现 ( 在 OnTimer 中实现 )
	3)	包链表的管理和处理逻辑
	4)	为上层提供 连接 发送包 发送Logout包 服务
	5)	将接收到的包送到上层处理
	6)	将连接失败的消息送到上层处理
--------------------------------------------------------------------------------*/
class CClientConnServer
{
// 调用接口
public:
//	BOOL	IsConnected( )	;
	BOOL	OpenConnection(  SOCKADDR* lpSockAddr, int nSockAddrLen );
	BOOL	Connect( SOCKADDR* lpSockAddr, int nSockAddrLen );
	BOOL	Send( LPPACKAGE lpPackage )				;
	BOOL	SendLogout( LPPACKAGE lpPackage )		;

	// 通过时钟来驱动自动连接逻辑的实现
	void	OnTimer(UINT nIDEvent)	;

	// 将收到的包裹送到上层处理
	void	OnReceivePackage( LPPACKAGE lpPackage )	;
	// 将连接失败的消息送到上层处理
	void	OnConnFailed( )	;

// 工作数据
protected:
	// 网络断开时暂存要发送的包
	CPtrList	m_lstSendBuffer	;
private:
	SOCKADDR_IN m_SockAddr		;
	int			m_nSockAddrLen	;	

// 状态标志
/********************************************************************
状态标志发生执行变的位置
	m_bConnected:
		CSocketConnServer::OnConnect
	m_bWantConnect:
		CSocketConnServer::OnConnect
	m_bRelogon:
		CSocketConnServer::OnConnect	若需要重新登陆则发重登陆包
********************************************************************/
protected:
public:
	// 本来是保护的, 现在是公开的
	BOOL	m_bConnected	;	// 是否已经连接好
	BOOL	m_bReLogon		;	// 是否需要重登陆(当服务器被重新启动后需要重登陆)
	
	// 是否是第一次连接
	BOOL	m_bWantConnect	;	// 是否需要自动连接
	BOOL	m_bLogout		;	// 用户是否已经发出注销请求

// 内部方法
protected:
	BOOL	SendPackageList ( )		;	// 处理包列表, 将列表中的包发送出去

// 关键对象指针
public:
	// 上层
	CNetServer *	m_pNetServer	;
protected:
	// 下层
	CSocketConnServer*	m_pSocket	;

protected:
//	BOOL	m_bVerified;

public:
	CClientConnServer( CNetServer* pServer );
	~CClientConnServer(void);
};
