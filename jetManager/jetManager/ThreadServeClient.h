#pragma once



// CThreadServeClient

class CThreadServeClient : public CWinThread
{
	DECLARE_DYNCREATE(CThreadServeClient)

protected:
	CThreadServeClient();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CThreadServeClient();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


