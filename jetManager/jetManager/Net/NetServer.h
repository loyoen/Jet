/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[Module]
	CNetServer - Client version

[Description]
	This class provide service for netwrok transformation
	It contains some thread to imply all the functions.

[Authors]
	Jicheng Hu, Wuhan University

[Remain Problems]
	如果传送失败，如何通知本CNetServer的用户此情况

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
// 调用接口
public:
	BOOL	StartNetServer( )	;	// 启动服务
	BOOL	StopNetServer( )	;	// 停止服务

	BOOL	PostMessageToServer( LPPACKAGE_HEADER lpPackageHeader )		;
//	BOOL	PostMessageToOperator( LPPACKAGE_HEADER lpPackageHeader )	;

	BOOL	Connect ( );	// 建立网络连接, 包括与主服务器、备份服务器、Operator等

	BOOL	SendToServer( LPPACKAGE lpPackage );
	void	OnReceiveFromServer( LPPACKAGE lpPackage );
	void	OnConnServerFailed( );

//	void	LoopConnServer( );
//	void	LoopConnClient( );
//	void	LoopServeClient( );
	void	OnNetReloginOK();
	void	OnServerRestarted();

	BOOL	SendLogoutPackage( LPPACKAGE lpPackage );

// 参数输入接口
// Attributes setting
public:
//	void	AddServer ( CString strAddr, u_short sinport );		// 根据地址和端口添加服务器
//	void	AddOperator ( CString strAddr, u_short sinport );	// 根据地址和端口添加Operator
	void	SetMainServer ( CString strAddr, u_short sinport );	// 设置主服务器的地址和端口

// 工作参数
protected:
//	CPtrList	m_listPtrServer;	// 服务器地址列表
//	BYTE		m_yServerCnt;		// 服务器个数

	//CPtrList	m_listPtrOperator;	// Operator的地址列表
	//BYTE		m_yOperatorCnt;		// Operator的个数

	SOCKADDR_IN		m_addrMainServer;

// Attributes
// 工作数据
protected:
//	CSocketConnServer*	m_pSocketConnServer;
	CThreadTaskServer*	m_pTaskServer;

// 工作对象
protected:
	CClientConnServer*	m_pClientConnServer;

// 工作线程
protected:
//	CThreadConnClient*	m_pThreadConnClient;
//	CThreadServeClient*	m_pThreadServeClient;

// 标志
protected:
	BOOL	m_bServerConnected;		// 服务器连接成功标志
	BOOL	m_bServerVerified;		// 登陆服务器security验证成功标志
	BOOL	m_bOperatorConnected;	// Operator连接成功标志

// 缓冲数据管理
protected:

public:
	CNetServer(CThreadTaskServer* pServer);
	~CNetServer(void);

	void	DownReturnAddress(LPPACKAGE lpPackage);
	void	OnTimer(UINT nIDEvent);
	void	ReLogin ( );
};
