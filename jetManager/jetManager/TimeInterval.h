#pragma once


// CTimeInterval �Ի���

class CTimeInterval : public CDialog
{
	DECLARE_DYNAMIC(CTimeInterval)

public:
	CTimeInterval(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTimeInterval();

// �Ի�������
	enum { IDD = IDD_AUTO_TESTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	UINT nTimeInterval;
};
