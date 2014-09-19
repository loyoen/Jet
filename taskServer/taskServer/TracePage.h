#pragma once
#include "DialogResize.h"
#include "ColorListBox.h"
#include "CCriticalSectionWrapper.h"
//#include "PackProtocol.h"

#include "Resource.h"
// CTracePage 对话框

//WM_NETWORK_GUI_SERVER_LOG
enum
{
	//决定图标
	LOG_FLAG_ERROR=0x0001,
	LOG_FLAG_WARNING=0x0002,
	LOG_FLAG_INFO=0x0004,
	//决定该消息条目的背景色
	LOG_FLAG_MACHINEGROUP=0x0100,
	LOG_FLAG_JOBGROUP=0x0200,
	LOG_FLAG_SLAB     =0x0400,
	LOG_FLAG_FTP      =0x0800,
	LOG_FLAG_IOCP     =0x1000,
	LOG_FLAG_SERVER   =0x2000,
	LOG_FLAG_USER_DROPLINE_DETECTION=0x4000,
};

class CTracePage : public CDialogResize
{
	DECLARE_DYNAMIC(CTracePage)

public:
	CTracePage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTracePage();
	virtual BOOL OnInitDialog();
	void AddTraceLine(int nLevel, LPCTSTR pstrFormat, ...);
    void AddLogLine(unsigned short nClass,CString str);
// 对话框数据
	enum { IDD = IDD_PROPPAGE_SERVER_LOG };
    CColorListBox	m_TraceList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTraceClear();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	LRESULT OnAddTraceLine(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()

	CObList m_LogQueue;
	CCriticalSectionWrapper m_QueueLock;

#ifdef	SWITCH_AUTO_TESTING
	CLogFile	m_Logfile;
#endif

	class CLogMsg : public CObject
	{
	public:
		CLogMsg() {};
		virtual ~CLogMsg() 
		{
		};

		SYSTEMTIME      m_sysTime;
		unsigned short	m_nLevel;
		CString         m_strText;
	};
};
