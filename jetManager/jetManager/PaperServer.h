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
// ���ýӿ�
public:
	BOOL	StartPaperServer ( );	// ��������
	BOOL	StopPaperServer ( );	// ֹͣ����

	void	SetServerAddr ( CString strAddr );	// ���÷�������ַ
	void	SetServerPort ( UINT nPort );
	void	SetLogonInfo ( CString strAccount, CString strPassword );
	void	ConnectServer ( );
	void	ConnectServer ( CString strAddr, u_short sinport );
	void	Logon ( CString strAccount, CString strPassword );
	void	Logout ( );

	BOOL	SetNextAction( enumNextAction eActionNext );

	void	ThreadLoop( );

// ��������
protected:
	CString		m_strAccount;		// �û��ʺ�
	CString		m_strPassword;		// �û�����

// ����״̬
protected:
	 enumNextAction		m_eActionNext;	// ��һ����

// ������־
protected:
	BOOL	m_bExitThreadLoop;
	BOOL	m_bPaperServerStarted;

// �ڲ�����
protected:
	void	Logon( );

// �����ṩ��
protected:
	CNetServer*		m_pNet;
	HANDLE			m_hThreadPaperService;   

public:
	CPaperServer(void);
	~CPaperServer(void);
};
