// taskServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "taskServer.h"
#include "taskServerDlg.h"

#include "MgrMessage.h"

#include "..\jetServer\CServer.h"
#include "..\jetServer\CThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void* WaitClient(void* pParam);
void* CheckClient(void* pParam);
void* SendData(void* pParam);
void* EndThread(void* pParam);

static UINT auIDStatusBar[] = 
{
	ID_SEPARATOR,
	//ID_INDICATOR_DATA_RECEIVED,
	//ID_INDICATOR_DATA_SEND,
	ID_INDICATOR_ONLINELED,
	ID_INDICATOR_OFFLINELED
};

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CtaskServerDlg dialog




CtaskServerDlg::CtaskServerDlg(CWnd* pParent /*=NULL*/)
#ifdef	__JICHENG__
	: CDialogResize(CtaskServerDlg::IDD, pParent)
#else
	: CDialog(CtaskServerDlg::IDD, pParent)
#endif
{
	m_hIcon		= AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pServer	= NULL;

	m_bInitialized			= FALSE;
	m_bServerStarted		= FALSE;
	m_bCloseWindowAndExit	= FALSE;
//	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, begin
	m_bStart_button_pressed	= FALSE;
//	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, ends
}

void CtaskServerDlg::DoDataExchange(CDataExchange* pDX)
{
#ifdef	__JICHENG__
//	DDX_Control(pDX, IDC_INFOBAR, m_InfobarCtrl);
	DDX_Control(pDX, IDC_OUTLOOKBAR, m_OutlookBar);

	CDialogResize::DoDataExchange(pDX);
#else
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_showmsg);
#endif
}

#ifdef	__JICHENG__
BEGIN_MESSAGE_MAP(CtaskServerDlg, CDialogResize)
#else
BEGIN_MESSAGE_MAP(CtaskServerDlg, CDialog)
#endif
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_MESSAGE( ID_MSG_STATE, OutString )
	ON_MESSAGE( WM_GUI_LOG,OnGUI_LOG_msg )

	ON_WM_DESTROY()

	ON_NOTIFY(NM_CLICK, IDC_OUTLOOKBAR, OnClickOutlookBar)
	ON_NOTIFY(LVN_KEYDOWN, IDC_OUTLOOKBAR, OnKeydownOutlookBar)
	ON_COMMAND(IDC_VIEW_TRACE, OnViewTrace)
	ON_COMMAND(IDC_VIEW_ONLINE_USERS, OnViewMachines)
	ON_COMMAND(IDC_VIEW_CONFIGURATION, OnViewConfiguration)
	ON_COMMAND(ID_VIEW_MEMORY_STAT, OnViewMemoryStat)
	ON_COMMAND(ID_VIEW_USER_GROUP, OnViewMachineGroup)
	ON_COMMAND(IDC_VIEW_PAPER_MANAGER, OnViewJobManager)

	ON_UPDATE_COMMAND_UI(IDC_VIEW_PAPER_MANAGER, OnUpdateViewJobManager)
	ON_UPDATE_COMMAND_UI(ID_VIEW_USER_GROUP, OnUpdateViewMachineGroup)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MEMORY_STAT, OnUpdateViewMemoryStat)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_TRACE, OnUpdateViewTrace)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_ONLINE_USERS, OnUpdateViewMachines)
	ON_UPDATE_COMMAND_UI(IDC_VIEW_CONFIGURATION, OnUpdateViewConfiguration)

	// Server Start & Stop
	ON_COMMAND(ID_SERVER_START, OnServerStart)
	ON_COMMAND(ID_SERVER_PRESTART, OnServerPreStart)
	ON_COMMAND(ID_SERVER_STOP, OnServerStop)
	ON_UPDATE_COMMAND_UI(ID_SERVER_START, OnUpdateServerStart)
	ON_UPDATE_COMMAND_UI(ID_SERVER_PRESTART, OnUpdateServerPreStart)
	ON_UPDATE_COMMAND_UI(ID_SERVER_STOP, OnUpdateServerStop)
	// UI update
	ON_MESSAGE(WM_KICKIDLE, OnKickIdle)

#ifndef	__JICHENG__
	ON_BN_CLICKED(IDC_BUTTON1, &CtaskServerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CtaskServerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CtaskServerDlg::OnBnClickedButton3)
#endif
END_MESSAGE_MAP()

BEGIN_DLGRESIZE_MAP(CtaskServerDlg)
	DLGRESIZE_CONTROL(IDC_OUTLOOKBAR, DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(IDC_DIALOG_AREA, DLSZ_SIZE_X | DLSZ_SIZE_Y)
	DLGRESIZE_CONTROL(AFX_IDW_STATUS_BAR, DLSZ_MOVE_Y | DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(AFX_IDW_TOOLBAR, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_INFOBAR, DLSZ_SIZE_X)
END_DLGRESIZE_MAP()


// CtaskServerDlg message handlers

/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::SetupOutlookBar					*/		
/* Description   : Initialize listview, change color/iconspacing to	*/
/*				   make it look a little bit like the outlook bar.	*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::SetupOutlookBar()
{
	// create Imagelist
	m_ImageList.Create(32, 32, ILC_COLOR16|ILC_MASK,1, 4);

	HICON hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_LOG));
	m_ImageList.Add(hIcon);
	hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ONLINE_USERS));
	m_ImageList.Add(hIcon);
	hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_MACHINEGROUP));
	m_ImageList.Add(hIcon);
	hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_JOBGROUP));
	m_ImageList.Add(hIcon);  
	hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_MEMORY));
	m_ImageList.Add(hIcon);
	hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_CONFIGURATION));
	m_ImageList.Add(hIcon);

	m_OutlookBar.SetImageList(&m_ImageList, LVSIL_NORMAL);

	CRect rc;
	m_OutlookBar.GetClientRect(rc);

	// set new icon spacing
	m_OutlookBar.SetIconSpacing(rc.Width(), 64);

	// change colors
	m_OutlookBar.SetTextColor(RGB(255,255,255));
	m_OutlookBar.SetTextBkColor(RGB(128,128,128));
	m_OutlookBar.SetBkColor(RGB(128,128,128));	

	// insert items
	m_OutlookBar.InsertColumn ( 0, _T("OutlookBar") );

	m_OutlookBar.InsertItem(0,  _T("运行日志") , 0);
	m_OutlookBar.InsertItem(1,  _T("在线机器") , 1);
	m_OutlookBar.InsertItem(2,  _T("设备编组") , 2);
	m_OutlookBar.InsertItem(3,  _T("作业状态") , 3);
	m_OutlookBar.InsertItem(4,  _T("内存状态") , 4);
	m_OutlookBar.InsertItem(5,  _T("系统配置") , 5);


	//	m_OutlookBar.SetExtendedStyle(LVS_EX_TRACKSELECT);
}

/********************************************************************/
/*																	*/
/* Function name : SetOnlineLed										*/	
/* Description   : Turn online LED on/off.							*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::SetOnlineLed(BOOL bOnline)
{
	HICON hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(),
				bOnline ? MAKEINTRESOURCE(IDI_LED_GREEN) : MAKEINTRESOURCE(IDI_LED_OFF),
				 IMAGE_ICON, 16, 16, LR_SHARED);
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(1, hIcon);
	m_wndStatusBar.GetStatusBarCtrl().Invalidate();
	m_wndStatusBar.GetStatusBarCtrl().UpdateWindow();
	DestroyIcon(hIcon);
}


/********************************************************************/
/*																	*/
/* Function name : SetOfflineLed									*/	
/* Description   : Turn offline LED on/off.							*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::SetOfflineLed(BOOL bOffline)
{
	HICON hIcon = (HICON)::LoadImage(AfxGetInstanceHandle(),
		bOffline ? MAKEINTRESOURCE(IDI_LED_RED) : MAKEINTRESOURCE(IDI_LED_OFF),
		IMAGE_ICON, 16, 16, LR_SHARED);
	m_wndStatusBar.GetStatusBarCtrl().SetIcon(2, hIcon);
	m_wndStatusBar.GetStatusBarCtrl().Invalidate();
	m_wndStatusBar.GetStatusBarCtrl().UpdateWindow();
	DestroyIcon(hIcon);
}


/********************************************************************/
/*																	*/
/* Function name : CreateStatusbar									*/	
/* Description   : This function creates a statusbar on a dialogbox.*/
/*																	*/
/********************************************************************/
BOOL CtaskServerDlg::CreateStatusbar()
{
	// Create the Toolbar and attach the resource
	if(!m_wndToolBar.Create(this) || !m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to Create Dialog Toolbar\n");
		return FALSE;	
	}

	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);
	
	// Make it flat.
	m_wndToolBar.ModifyStyle(0, m_wndToolBar.GetStyle()|TBSTYLE_FLAT);


	// Create status bar at the bottom of the dialog window
	if (m_wndStatusBar.Create(this))
	{

		m_wndStatusBar.SetIndicators(auIDStatusBar, sizeof(auIDStatusBar)/sizeof(UINT));
		
		m_wndStatusBar.SetPaneText(0, _T("系统未启动"), TRUE);
		// Make a sunken or recessed border around the first pane
		m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL );

		SetOnlineLed(FALSE);
		SetOfflineLed(FALSE);

		m_wndStatusBar.SetPaneInfo(1,ID_INDICATOR_ONLINELED, SBPS_NOBORDERS, 14);
//		m_wndStatusBar.GetStatusBarCtrl().SetTipText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_ONLINELED), "This status light is green when the server is online");

		m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_OFFLINELED, SBPS_NOBORDERS, 14);
//		m_wndStatusBar.GetStatusBarCtrl().SetTipText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_OFFLINELED), "This status light is green when the server is online");

    }

    // We need to resize the dialog to make room for control bars.
	// First, figure out how big the control bars are.
	CRect rcClientStart;
	CRect rcClientNow;
	GetClientRect(rcClientStart);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0, reposQuery, rcClientNow);

    // Now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.
	CPoint ptOffset(rcClientNow.left - rcClientStart.left, rcClientNow.top - rcClientStart.top); 

	CRect  rcChild;					
	CWnd* pwndChild = GetWindow(GW_CHILD);
	while (pwndChild)
	{                               
		pwndChild->GetWindowRect(rcChild);
		ScreenToClient(rcChild);
		rcChild.OffsetRect(ptOffset);
		pwndChild->MoveWindow(rcChild, FALSE);
		pwndChild = pwndChild->GetNextWindow();
	}

	// Adjust the dialog window dimensions
	CRect rcWindow;
	GetWindowRect(rcWindow);
	rcWindow.right += rcClientStart.Width() - rcClientNow.Width();
	rcWindow.bottom += rcClientStart.Height() - rcClientNow.Height();
	MoveWindow(rcWindow, FALSE);
	
	// And position the control bars
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);


    return TRUE;
}


BOOL CtaskServerDlg::OnInitDialog()
{
#ifdef	__JICHENG__
	CDialogResize::OnInitDialog();
#else
	CDialog::OnInitDialog();
#endif

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
#ifdef	__JICHENG__
	// Associate a 'tag'with the window so we can locate it later
	::SetProp( GetSafeHwnd(), AfxGetApp()->m_pszExeName, (HANDLE)1 );

	// setup listview ctrl, so that it looks like the outbar control
	SetupOutlookBar();
	// Add statusbar to the dialog
	CreateStatusbar();

	InitResizing(FALSE);

	// create property pages
	m_TracePage.Create(IDD_PROPPAGE_SERVER_LOG, this);
	m_MachinesPage.Create(IDD_PROPPAGE_MACHINE, this);
	m_ConfigurationPage.Create(IDD_PROPPAGE_CONFIGURATION, this);
	m_MemoryUsagePage.Create(IDD_PROPPAGE_MEMORY_USAGE,this);
	m_MachineGroupPage.Create(IDD_PROPPAGE_MACHINEGROUP,this);
	m_JobManagerPage.Create(IDD_PROPPAGE_JOBGROUP,this);

	// activate main page 
	ActivatePage(0);

	m_bInitialized = TRUE;
#else
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
#endif

	CMgrMessage	*pMgr;

	pMgr	= CMgrMessage::getManager ();
	pMgr->setWndGUI ( this->GetSafeHwnd() );

	SetWindowText( TITLE_SERVER );
	m_wndStatusBar.SetPaneText( 0, _T("打印服务未启动"), TRUE );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::MoveChilds						*/		
/* Description   : Move child windows into place holder area.		*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::MoveChilds()
{
	// position property pages 
	CRect rcDlgs;
	
	// get dialog area rect
	GetDlgItem(IDC_DIALOG_AREA)->GetWindowRect(rcDlgs);
	
	ScreenToClient(rcDlgs);
	
	m_ConfigurationPage.MoveWindow(rcDlgs)	; 
	m_TracePage.MoveWindow(rcDlgs)			; 
	m_MachinesPage.MoveWindow(rcDlgs)	; 
	m_MemoryUsagePage.MoveWindow(rcDlgs)	; 
	m_MachineGroupPage.MoveWindow(rcDlgs)		;
	m_JobManagerPage.MoveWindow(rcDlgs)	; 
}

/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::ActivatePage					*/
/* Description   : Called when an icon on the outlookbar is pressed.*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::ActivatePage(int nIndex)
{
	switch(nIndex)
	{
		case 0:
			m_MachinesPage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);
			m_MemoryUsagePage.ShowWindow(SW_HIDE);	
			m_MachineGroupPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_SHOW);
			m_JobManagerPage.ShowWindow(SW_HIDE);
//			m_InfobarCtrl.SetText(_T("服务日志"));
			break;
		case 1:
			m_TracePage.ShowWindow(SW_HIDE);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);	
			m_MemoryUsagePage.ShowWindow(SW_HIDE);	
			m_MachineGroupPage.ShowWindow(SW_HIDE);	
			m_MachinesPage.ShowWindow(SW_SHOW);
			m_JobManagerPage.ShowWindow(SW_HIDE);
//			m_InfobarCtrl.SetText("在线用户");
			break;
		case 2:
			m_MachinesPage.ShowWindow(SW_HIDE);	
			m_TracePage.ShowWindow(SW_HIDE);	
			m_MemoryUsagePage.ShowWindow(SW_HIDE);	
			m_MachineGroupPage.ShowWindow(SW_SHOW);	
			m_ConfigurationPage.ShowWindow(SW_HIDE);	
			m_JobManagerPage.ShowWindow(SW_HIDE);
//			m_InfobarCtrl.SetText("用户组");
//			UpdateChannelServerState();
			break;
		//case 3:
		//	m_MachinesPage.ShowWindow(SW_HIDE);	
		//	m_TracePage.ShowWindow(SW_HIDE);	
		//	m_MemoryUsagePage.ShowWindow(SW_HIDE);	
		//	m_MachineGroupPage.ShowWindow(SW_HIDE);	
		//	m_ConfigurationPage.ShowWindow(SW_HIDE);	
		//	m_JobManagerPage.ShowWindow(SW_SHOW);
		//	m_InfobarCtrl.SetText("任务状态");
		//	break;
		//case 4:
		//	m_MachinesPage.ShowWindow(SW_HIDE);	
		//	m_TracePage.ShowWindow(SW_HIDE);	
		//	m_MemoryUsagePage.ShowWindow(SW_SHOW);	
		//	m_MachineGroupPage.ShowWindow(SW_HIDE);	
		//	m_ConfigurationPage.ShowWindow(SW_HIDE);	
		//	m_JobManagerPage.ShowWindow(SW_HIDE);
		//	m_InfobarCtrl.SetText("内存状态");
		//	break;
		//case 5:
		//	m_MachinesPage.ShowWindow(SW_HIDE);	
		//	m_TracePage.ShowWindow(SW_HIDE);	
		//	m_MemoryUsagePage.ShowWindow(SW_HIDE);	
		//	m_MachineGroupPage.ShowWindow(SW_HIDE);	
		//	m_ConfigurationPage.ShowWindow(SW_SHOW);	
		//	m_JobManagerPage.ShowWindow(SW_HIDE);
		//	m_InfobarCtrl.SetText("服务器配置");
		//	break;
		default:
			break;
	}

	MoveChilds();
}


void CtaskServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CtaskServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CtaskServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CtaskServerDlg::OnDestroy()
{
#ifdef	__JICHENG__
	CDialogResize::OnDestroy();
#else
	CDialog::OnDestroy();
#endif

	// TODO: Add your message handler code here
	if ( NULL != m_pServer )
		delete	m_pServer;
}

void CtaskServerDlg::OnBnClickedButton1()
{
	ASSERT ( NULL == m_pServer );

	m_pServer	= new CServer();
	m_pServer->StartServer();

	cpWaitClientThread	= new CThread(WaitClient,this);
	cpCheckClientThread	= new CThread(CheckClient,this);
	cpDataThread		= new CThread(SendData,this);

	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}

void CtaskServerDlg::OnBnClickedButton2()
{
	if ( NULL == m_pServer )
		return;

	m_pServer->setChoice(CIRCLE);
}

void CtaskServerDlg::OnBnClickedButton3()
{
	if ( NULL == m_pServer )
		return;

	m_pServer->setChoice(SQUARE);
}

void CtaskServerDlg::OnOK ()
{
	return;
}

void CtaskServerDlg::OnCancel ()
{
	return;
}

void* WaitClient(void* pParam)
{
	CtaskServerDlg *pDlg = (CtaskServerDlg*)pParam;

	CMSG_Server::GetCMSG_Server()->AddMessage("Strat Wait Client...\r\n",1,GREEN);
	pDlg->m_pServer->WaitClient();
	return 0;
}

void* CheckClient(void* pParam)
{
	CtaskServerDlg *pDlg = (CtaskServerDlg*)pParam;

	CMSG_Server::GetCMSG_Server()->AddMessage("Strat Check Client...\r\n",1,GREEN);
	//pDlg->pServer->CheckClient(pParam);
	return 0;
}

void* SendData(void* pParam)
{
	CtaskServerDlg *pDlg = (CtaskServerDlg*)pParam;

	CMSG_Server::GetCMSG_Server()->AddMessage("Strat Send Data...\r\n",1,GREEN);
	pDlg->m_pServer->Data();
	return 0;
}

void* EndThread(void* pParam)
{
	CtaskServerDlg *pDlg = (CtaskServerDlg*)pParam;

	CMSG_Server::GetCMSG_Server()->AddMessage("end the project...\r\n",1,GREEN);
	if(pDlg->m_pServer!=NULL)
	{
		pDlg->m_pServer->setStop(true);
		CMSG_Server::GetCMSG_Server()->AddMessage("waiting for WaitClientThread...\r\n",1,RED);
		pDlg->cpWaitClientThread->wait4ThisThread();
		CMSG_Server::GetCMSG_Server()->AddMessage("WaitClientThread is closed...\r\n",1,RED);
		CMSG_Server::GetCMSG_Server()->AddMessage("waiting for CheckClientThread...\r\n",1,RED);
		pDlg->cpCheckClientThread->wait4ThisThread();
		CMSG_Server::GetCMSG_Server()->AddMessage("CheckClientThread is closed...\r\n",1,RED);
		CMSG_Server::GetCMSG_Server()->AddMessage("waiting for DataThread...\r\n",1,RED);
		pDlg->cpDataThread->wait4ThisThread();
		CMSG_Server::GetCMSG_Server()->AddMessage("DataThread is closed...\r\n",1,RED);
		delete pDlg->cpWaitClientThread;
		delete pDlg->cpCheckClientThread;
		delete pDlg->cpDataThread;
		delete pDlg->m_pServer;
		
	}
	if(CMSG_Server::GetCMSG_Server()!=NULL)
			delete CMSG_Server::GetCMSG_Server();
	::PostMessageA(pDlg->GetSafeHwnd(),ID_ENDAPP,0,0);
	return 0;
}

LRESULT CtaskServerDlg::OutString(WPARAM w, LPARAM a)
{
	//int min = 0,max =0;
	//CString str= CString( (char*)a);
	//CString text;
	//GetDlgItem(IDC_EDIT1)->GetWindowTextW(text);
	//text += str;
	////GetDlgItem(IDC_EDIT1)->SetWindowTextW(text);
	//this->SetDlgItemTextW(IDC_EDIT1,text);
	//text.Empty();
 //   m_showmsg.LineScroll(m_showmsg.GetLineCount());
	return true;
}

LRESULT CtaskServerDlg::OnGUI_LOG_msg ( WPARAM wParam, LPARAM lParam )
{

	CMgrMessage	*pMgr;
	
	pMgr	= CMgrMessage::getManager ();

	//t_network_server_log *p=(t_network_server_log*)lParam;
	//CString	str;
	//str.Format( _T("%s"), (char*)lParam );
	char	str[1024];
	CString text;

	ZeroMemory ( str, 1024 );

	strcpy_s ( str, (char*)lParam );
	pMgr->InsertMessageEntity ( (char*)lParam ); 

	text.Empty();
//	GetDlgItem(IDC_EDIT1)->GetWindowTextW(text);
	text	+= str;
	m_TracePage.AddLogLine(wParam,text);  
//	this->SetDlgItemTextW(IDC_EDIT1, (LPCTSTR)text);

//    m_showmsg.LineScroll(m_showmsg.GetLineCount());

	return 0;
}

LRESULT CtaskServerDlg::EndFunc(WPARAM w, LPARAM a)
{
	delete cpStopThread;
	OnCancel();
	return true;
}

/********************************************************************/
/*																	*/
/* Function name : OnServerStart									*/		
/* Description   : Start eMarkServer -> start listening on port 8721*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnServerStart() 
{
//	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, begin
	if ( m_bStart_button_pressed )
		return;

	m_bStart_button_pressed	= TRUE;
//	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, ends

	//m_ConfigurationPage.UpdateData();
	//if(!IsConfigurationParamValid())
	//	 return;

	//创建网络线程
//	m_pThreadChannelServer	= (CChannelServer*)AfxBeginThread(RUNTIME_CLASS(CChannelServer), THREAD_PRIORITY_NORMAL);
//
//	m_pThreadChannelServer->SetBufferList( 
//		m_lpExlargeBufferList,
//		m_lpLargeBufferList,
//		m_lpNormalBufferList,
//		m_lpSmallBufferList,
//		m_lpCommandBufferList
//		);
//
//	m_pThreadChannelServer->SetNetBufferList(
//		m_lpNetExlargeBufferList,
//		m_lpNetLargeBufferList,
//		m_lpNetNormalBufferList,
//		m_lpNetSmallBufferList,
//		m_lpNetCommandBufferList
////	SKLSE 2005-08-08, pre-allocate memory for Lookaside io-/perhandle-list, jicheng, begin
//		,m_lpIOLookasideList,
//		m_lpHandleLookasideList
////	SKLSE 2005-08-08, pre-allocate memory for Lookaside io-/perhandle-list, jicheng, ends
//		);

	CString	strFilePath;
	CFile	file;
	BOOL	bOpen;

//	strFilePath.Format( "%s", "c:\\WINNT\\system32\\zaba.zed" );
//	bOpen	= file.Open(strFilePath, CFile::modeRead); 
////	bOpen	= TRUE;
//	if ( bOpen )
//		file.Close( );
	bOpen	= TRUE;

//	if ( !m_pThreadChannelServer->Start( FALSE ) )
////	if ( !m_pThreadChannelServer->Start( !bCheckFile ) )
//	{
//		CMessageManager *pMgr=CMessageManager::GetManager();
//		t_network_server_log *p=(t_network_server_log*)pMgr->GetMessageEntity();
//		p->nClass	= LOG_FLAG_INFO | LOG_FLAG_SERVER;
//		lstrcpy( p->strLog, "服务器启动失败!\n务必关闭程序后再重试!!!" );
//		CMessageManager::PostMessageToGUI( WM_NETWORK_GUI_SERVER_LOG, 0, (char*)p );
//		return;	//启动失败
//	}

	ASSERT ( NULL == m_pServer );

	m_pServer	= new CServer();
	m_pServer->StartServer();

	cpWaitClientThread	= new CThread(WaitClient,this);
//	cpCheckClientThread	= new CThread(CheckClient,this);
//	cpDataThread		= new CThread(SendData,this);

	m_bServerStarted	= TRUE;

//	//服务器启动成功
//	CMessageManager *pMgr=CMessageManager::GetManager();
//	t_network_server_log *p=(t_network_server_log*)pMgr->GetMessageEntity();
//	p->nClass	= LOG_FLAG_INFO | LOG_FLAG_SERVER;
//	lstrcpy( p->strLog, "服务器启动成功!" );
//	CMessageManager::PostMessageToGUI( WM_NETWORK_GUI_SERVER_LOG, 0, (char*)p );

	m_wndStatusBar.SetPaneText( 0, _T("打印服务已经启动"), TRUE );

	if ( bOpen )
	{
		SetOnlineLed(TRUE);
		SetOfflineLed(FALSE);
	}
	else
	{
		SetOnlineLed(FALSE);
		SetOfflineLed(TRUE);
	}
}

/********************************************************************/
/*																	*/
/* Function name : OnServerStart									*/		
/* Description   : Start FTP server -> start listening on port 21	*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnServerPreStart() 
{
}

/********************************************************************/
/*																	*/
/* Function name : OnServerStop										*/		
/* Description   : Stop FTP server									*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnServerStop() 
{
//	// 输入停止密码
//	CDlgStop	stopDlg;
//
//	if ( stopDlg.DoModal() != IDOK )
//		return;
//
//	if ( stopDlg.m_iKey != 8721 )
//		return;
//
//	if ( !m_pThreadChannelServer->Stop() )
//		return;	// 停止失败
//
//	m_wndStatusBar.SetPaneText(0, "FTP Server is offline", TRUE);
//	SetOnlineLed(FALSE);
//	SetOfflineLed(TRUE);
//
////==============================================================
////	停止后必须退出
////	故灰掉全部启动按钮
//
//	m_bServerStarted	= FALSE;
////	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, begin
//	m_bStart_button_pressed	= TRUE;
////	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, ends
//
////	return;
////	KillTimer(m_nTimerID);
////	CMessageManager::PostMessageToNetWork(WM_GUI_NETWORK_EXIT_THREAD,0,(char*)NULL); 	
////
}

void CtaskServerDlg::OnStopAll()
{
	//m_pThreadChannelServer->AllLogout();
}

void CtaskServerDlg::OnUpdateStopAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( m_bServerStarted );
}

/********************************************************************/
/*																	*/
/* Function name : OnUpdateServerStart								*/		
/* Description   : Update 'Start' status							*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateServerStart(CCmdUI* pCmdUI) 
{
	//	pCmdUI->Enable(!theServer.IsActive());	
//	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, begin
//	pCmdUI->Enable(!m_bServerStarted);

	pCmdUI->Enable(!m_bStart_button_pressed);
//	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, ends
}

/********************************************************************/
/*																	*/
/* Function name : OnUpdateServerPreStart							*/		
/* Description   : Update 'PreStart' status							*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateServerPreStart(CCmdUI* pCmdUI) 
{
	//	pCmdUI->Enable(!theServer.IsActive());	
//	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, begin
//	pCmdUI->Enable(!m_bServerStarted);

//	pCmdUI->Enable(!m_bStart_button_pressed);
	pCmdUI->Enable( FALSE );
//	SKLSE 2007-04-07, jicheng, 防止启动按钮被按多次, ends
}

/********************************************************************/
/*																	*/
/* Function name : OnUpdateServerStop								*/		
/* Description   : Update 'Stop' status								*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateServerStop(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(theServer.IsActive());
	pCmdUI->Enable(m_bServerStarted);
}

/********************************************************************/
/*																	*/
/* Function name : OnKickIdle										*/		
/* Description   : Make ON_UPDATE_COMMAND_UI work for this dialog.	*/
/*																	*/
/********************************************************************/
LRESULT CtaskServerDlg::OnKickIdle(WPARAM wParam, LPARAM lParam)
{
	UpdateDialogControls(this, FALSE);

	//CMenu* pMainMenu = GetMenu();
 //   CCmdUI cmdUI;
 //   // update menu enable/disable state
 //   for (UINT n = 0; n < pMainMenu->GetMenuItemCount(); ++n)
 //   {
 //       CMenu* pSubMenu = pMainMenu->GetSubMenu(n);
 //       cmdUI.m_nIndexMax = pSubMenu->GetMenuItemCount();
 //       for (UINT i = 0; i < cmdUI.m_nIndexMax;++i)
 //       {
 //           cmdUI.m_nIndex = i;
 //           cmdUI.m_nID = pSubMenu->GetMenuItemID(i);
 //           cmdUI.m_pMenu = pSubMenu;
 //           cmdUI.DoUpdate(this, FALSE);
 //       }
 //   }

	// enable/disable toolbar buttons    
	if (m_wndToolBar.IsWindowVisible()) 
	{
		// OnUpdateCmdUI expects a CFrameWnd pointer, so let's fake it ..
		CFrameWnd *pParent = (CFrameWnd *)this;
		if (pParent)
			m_wndToolBar.OnUpdateCmdUI(pParent, TRUE);
	} 

	return Default();
}

/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::OnClickOutlookBar				*/
/* Description   : User clicked on our listview -> activate page.	*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnClickOutlookBar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// get index of selected item
	int nIndex = m_OutlookBar.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nIndex == -1)
        return;
		
	ActivatePage(nIndex);

	*pResult = 0;
}

/********************************************************************/
/*																	*/
/* Function name : CApplicationDlg::OnKeydownOutlookBar				*/
/* Description   : User pressed a key -> activate page.				*/
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnKeydownOutlookBar(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

    // get index of selected item
	int nIndex = m_OutlookBar.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
    
    if(nIndex == -1)
        return;

	if (pLVKeyDow->wVKey == VK_DOWN)
	{
		if (m_OutlookBar.GetItemCount()-1 > nIndex)
		{
			ActivatePage(nIndex+1);
		}
	}
	else
	if (pLVKeyDow->wVKey == VK_UP)
	{
		if (nIndex > 0)
		{
			ActivatePage(nIndex-1);
		}
	}
	else
	if (pLVKeyDow->wVKey == VK_NEXT)
	{
		ActivatePage(m_OutlookBar.GetItemCount()-1);
	}
	else
	if (pLVKeyDow->wVKey == VK_PRIOR)
	{
		ActivatePage(0);
	}

	*pResult = 0;
}

/********************************************************************/
/*																	*/
/* Function name : OnViewTrace										*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnViewTrace() 
{
	ActivatePage(0);
}

/********************************************************************/
/*																	*/
/* Function name : OnViewMachines								*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnViewMachines() 
{
	ActivatePage(1);
}


/********************************************************************/
/*																	*/
/* Function name : OnViewConfiguration								*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnViewConfiguration() 
{
	ActivatePage(5);
}

/********************************************************************/
/*																	*/
/* Function name : OnViewMemoryStat  								*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnViewMemoryStat()
{
	ActivatePage(4);
}

/********************************************************************/
/*																	*/
/* Function name : OnViewMemoryStat  								*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnViewMachineGroup()
{
	ActivatePage(2);
}

/********************************************************************/
/*																	*/
/* Function name : OnViewJobManager  								*/		
/* Description   : Handle menu selection.							*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnViewJobManager()
{
	ActivatePage(3);
}

/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewMemoryStat							*/		
/* Description   : Update menu state of Trace item.					*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateViewJobManager(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_JobManagerPage.IsWindowVisible());	
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewMemoryStat							*/		
/* Description   : Update menu state of Trace item.					*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateViewMachineGroup(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_MachineGroupPage.IsWindowVisible());	
}

/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewMemoryStat							*/		
/* Description   : Update menu state of Trace item.					*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateViewMemoryStat(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio(m_MemoryUsagePage.IsWindowVisible());	
}
/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewTrace								*/		
/* Description   : Update menu state of Trace item.					*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateViewTrace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_TracePage.IsWindowVisible());	
}

/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewMachines							*/		
/* Description   : Update menu state of Online Users item.			*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateViewMachines(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_MachinesPage.IsWindowVisible());		
}


/********************************************************************/
/*																	*/
/* Function name : OnUpdateViewConfiguration						*/		
/* Description   : Update menu state of Configuration item.			*/	
/*																	*/
/********************************************************************/
void CtaskServerDlg::OnUpdateViewConfiguration(CCmdUI* pCmdUI) 
{
	pCmdUI->SetRadio(m_ConfigurationPage.IsWindowVisible());			
}


