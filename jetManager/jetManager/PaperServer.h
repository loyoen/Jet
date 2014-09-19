#pragma once

#include "NetServer.h"
enum	enumNextAction
{
	doLogon,
	doLogout,
	doRequestPaper,
	doWait
};

class CPaperServer
{
// 调用接口
public:
	BOOL	StartPaperServer ( );	// 启动服务
	BOOL	StopPaperServer ( );	// 停止服务

	void	SetServerAddr ( CString strAddr );	// 设置服务器地址
	void	SetServerPort ( UINT nPort );
	void	SetLogonInfo ( CString strAccount, CString strPassword );
	void	ConnectServer ( );
	void	ConnectServer ( CString strAddr, u_short sinport );
	void	Logon ( CString strAccount, CString strPassword );
	void	Logout ( );

	BOOL	SetNextAction( enumNextAction eActionNext );

	void	ThreadLoop( );

// 工作参数
protected:
	CString		m_strAccount;		// 用户帐号
	CString		m_strPassword;		// 用户密码

// 工作状态
protected:
	 enumNextAction		m_eActionNext;	// 下一动作

// 工作标志
protected:
	BOOL	m_bExitThreadLoop;
	BOOL	m_bPaperServerStarted;

// 内部动作
protected:
	void	Logon( );

// 服务提供者
protected:
	CNetServer*		m_pNet;
	HANDLE			m_hThreadPaperService;   

public:
	CPaperServer(void);
	~CPaperServer(void);
};
