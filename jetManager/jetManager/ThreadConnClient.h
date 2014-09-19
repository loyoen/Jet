#pragma once



// CThreadConnClient

class CThreadConnClient : public CWinThread
{
	DECLARE_DYNCREATE(CThreadConnClient)

protected:
	CThreadConnClient();           // 动态创建所使用的受保护的构造函数
	virtual ~CThreadConnClient();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


