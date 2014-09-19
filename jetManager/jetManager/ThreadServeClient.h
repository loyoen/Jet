#pragma once



// CThreadServeClient

class CThreadServeClient : public CWinThread
{
	DECLARE_DYNCREATE(CThreadServeClient)

protected:
	CThreadServeClient();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadServeClient();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


