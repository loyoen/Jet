#pragma once



// CThreadConnClient

class CThreadConnClient : public CWinThread
{
	DECLARE_DYNCREATE(CThreadConnClient)

protected:
	CThreadConnClient();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CThreadConnClient();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


