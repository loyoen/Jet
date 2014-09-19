#pragma once

#include "PackProtocol.h"


// CThreadConnServer

class CClientConnServer;
class CNetServer;

class CThreadConnServer : public CWinThread
{
	DECLARE_DYNCREATE(CThreadConnServer)

public:
	BOOL	Connect( SOCKADDR* lpSockAddr, int nSockAddrLen );
	void	EndThread( );
	BOOL	Send( LPPACKAGE lpPackage );
	void	OnReceivePackage( LPPACKAGE lpPackage );
	void	SetNetServer( CNetServer* pServer );

//	上、下接口
protected:
	CClientConnServer*	m_pClient;
	CNetServer*			m_pNetServer;

protected:
	CThreadConnServer();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadConnServer();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	afx_msg void OnAppExitMessage(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
};


