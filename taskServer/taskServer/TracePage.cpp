// TracePage.cpp : 实现文件
//

#include "stdafx.h"
#include "TracePage.h"


// CTracePage 对话框

IMPLEMENT_DYNAMIC(CTracePage, CDialogResize)
CTracePage::CTracePage(CWnd* pParent /*=NULL*/)
	: CDialogResize(CTracePage::IDD, pParent)
{
#ifdef	SWITCH_AUTO_TESTING
	m_Logfile.Open();
#endif
}

CTracePage::~CTracePage()
{
	while(!m_LogQueue.IsEmpty())
	{
		CLogMsg *pLogMsg = (CLogMsg *)m_LogQueue.RemoveHead();
		delete pLogMsg;
	}

#ifdef	SWITCH_AUTO_TESTING
	m_Logfile.Close();
#endif
}

BOOL CTracePage::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TraceList.SubclassDlgItem(IDC_TRACE,this);
	m_TraceList.SetImageList(IDB_BITMAP_ICON,4);

	//LOG_FLAG_MACHINEGROUP
    m_TraceList.AddListBoxItemStyle(0,RGB(0,0,0),RGB(193,254,192));
	//LOG_FLAG_JOBGROUP
	m_TraceList.AddListBoxItemStyle(2,RGB(0,0,0),RGB(249,254,204));
	//LOG_FLAG_SLAB
	m_TraceList.AddListBoxItemStyle(0,RGB(0,0,0),RGB(193,207,253));
   //LOG_FLAG_FTP
    m_TraceList.AddListBoxItemStyle(2,RGB(0,0,0),RGB(209,220,250));
	//LOG_FLAG_IOCP
    m_TraceList.AddListBoxItemStyle(3,RGB(0,0,0),RGB(205,215,204));
	//LOG_FLAG_SERVER
    m_TraceList.AddListBoxItemStyle(0,RGB(0,0,0),RGB(255,255,255));
   
	//错误消息格式
	m_TraceList.AddListBoxItemStyle(1,RGB(0,0,0),RGB(255,204,204));

	//用户在线检测线程
	m_TraceList.AddListBoxItemStyle(0,RGB(0,0,0),RGB(221,237,248));
				
//	SKLSE 2005-08-05, debug initialization of CTracePage, jicheng, begin
	m_TraceList.AddLogString( 0, _T("打印系统服务器准备就绪,等待启动") );
//	SKLSE 2005-08-05, debug initialization of CTracePage, jicheng, ends

	return TRUE;
}
void CTracePage::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_TRACE, m_TraceList);
}


BEGIN_MESSAGE_MAP(CTracePage, CDialogResize)
	//{{AFX_MSG_MAP(CTracePage)
	ON_WM_SIZE()
	ON_COMMAND(ID_TRACE_CLEAR, OnTraceClear)
	ON_WM_CONTEXTMENU()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDTRACELINE, OnAddTraceLine)
END_MESSAGE_MAP()



// CTracePage 消息处理程序

void CTracePage::AddTraceLine(int nLevel, LPCTSTR pstrFormat, ...)
{
	CString str;

	// format and write the data we were given
	va_list args;
	va_start(args, pstrFormat);
	str.FormatV(pstrFormat, args);

	try
	{
		// create new message
		CLogMsg *pLogMsg = new CLogMsg;

		GetLocalTime(&pLogMsg->m_sysTime);
		pLogMsg->m_nLevel = nLevel;
		pLogMsg->m_strText = str;

		m_QueueLock.Lock();
		m_LogQueue.AddTail(pLogMsg);
		m_QueueLock.Unlock();
				
		// schedule log action
		PostMessage(WM_ADDTRACELINE);
	}
	catch(...)
	{
	
	}
}

void CTracePage::AddLogLine(unsigned short nClass,CString str)
{
	// create new message
	CLogMsg *pLogMsg = new CLogMsg;

	GetLocalTime(&pLogMsg->m_sysTime);
	pLogMsg->m_nLevel = nClass;
	pLogMsg->m_strText = str;

	m_QueueLock.Lock();
	m_LogQueue.AddTail(pLogMsg);
	m_QueueLock.Unlock();

	// schedule log action
	PostMessage(WM_ADDTRACELINE);
}


LRESULT CTracePage::OnAddTraceLine(WPARAM, LPARAM)
{
	CLogMsg *pLogMsg;
	
	CString str;
	try
	{
		// get first message in the queue
//		m_QueueLock.Lock();
		pLogMsg = (CLogMsg *)m_LogQueue.RemoveHead();
//		m_QueueLock.Unlock();

		str.Format(_T("%02d-%02d-%02d %02d:%02d:%02d    %s"), 
			pLogMsg->m_sysTime.wYear,pLogMsg->m_sysTime.wMonth,pLogMsg->m_sysTime.wDay,
			pLogMsg->m_sysTime.wHour, pLogMsg->m_sysTime.wMinute, pLogMsg->m_sysTime.wSecond, 
			pLogMsg->m_strText);
		int nLevel=pLogMsg->m_nLevel;

		//对错误纪录特殊处理
		unsigned short nLogType=(unsigned short)(nLevel&0x00FF);
		if(LOG_FLAG_ERROR==nLogType)
		{
            m_TraceList.AddLogString(6,str);
			delete pLogMsg;
			return TRUE;
		}

		//对信息和警告做一般处理
		nLevel&=0xFF00;
		switch(nLevel)
		{
			case LOG_FLAG_MACHINEGROUP:
				m_TraceList.AddLogString(0,str);
				break;
			case LOG_FLAG_JOBGROUP:
				m_TraceList.AddLogString(1,str);
				break;
			case LOG_FLAG_SLAB:
				m_TraceList.AddLogString(2,str);
				break;
			case LOG_FLAG_FTP:
				m_TraceList.AddLogString(3,str);
				break;
			case LOG_FLAG_IOCP:
				m_TraceList.AddLogString(4,str);
				break;
			case LOG_FLAG_SERVER:
				m_TraceList.AddLogString(5,str);
				break;
			case LOG_FLAG_USER_DROPLINE_DETECTION:
				m_TraceList.AddLogString(7,str);
				break;
			default:
				m_TraceList.AddLogString(5,str);
				break;

		}
		delete pLogMsg;
	}
	catch(...)
	{
		// something is wrong...
	}

#ifdef	SWITCH_AUTO_TESTING
	m_Logfile << str << "\r\n";
#endif

	return TRUE;
}

void CTracePage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_TRACE)))
	{
		CRect rect;
		GetClientRect(rect);
//		rect.DeflateRect(5,5,10,10);
		m_TraceList.MoveWindow(rect);
	}
}

void CTracePage::OnTraceClear() 
{
	m_TraceList.ResetContent();	
}

void CTracePage::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	menu.LoadMenu(MAKEINTRESOURCE(IDR_TRACE_MENU));
	menu.GetSubMenu(0)->TrackPopupMenu(0, point.x, point.y, this, NULL);		
}
