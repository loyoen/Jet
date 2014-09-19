#pragma once


// CTimeInterval 对话框

class CTimeInterval : public CDialog
{
	DECLARE_DYNAMIC(CTimeInterval)

public:
	CTimeInterval(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTimeInterval();

// 对话框数据
	enum { IDD = IDD_AUTO_TESTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT nTimeInterval;
};
