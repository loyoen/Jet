// jetServerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "jetServer.h"
#include "jetServerDlg.h"


#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#ifdef WIN32
	#define sleep(x) Sleep((x)*1000) 
#endif
#ifndef WIN32
	#include<pthread.h>
#else 
	#include<windows.h>
#endif
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CjetServerDlg 对话框




CjetServerDlg::CjetServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CjetServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CjetServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_showmsg);
}

BEGIN_MESSAGE_MAP(CjetServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CjetServerDlg::OnBnClickedButton1)
	ON_MESSAGE( ID_MSG_STATE, OutString )
	ON_MESSAGE( ID_ENDAPP, EndFunc )
	ON_BN_CLICKED(IDCANCEL, &CjetServerDlg::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_BUTTON2, &CjetServerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON2, &CjetServerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CjetServerDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CjetServerDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CjetServerDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CjetServerDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CjetServerDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CjetServerDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CjetServerDlg::OnBnClickedButton9)
END_MESSAGE_MAP()


// CjetServerDlg 消息处理程序

BOOL CjetServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//_CrtSetBreakAlloc(153);

	pServer = NULL;
	GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CjetServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CjetServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CjetServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void* EndThread(void* pParam)
{
	CjetServerDlg *pDlg = (CjetServerDlg*)pParam;
	CMSG_Server::GetCMSG_Server()->AddMessage("end the project...\r\n",1,GREEN);
	if(pDlg->pServer!=NULL)
	{
		pDlg->pServer->setStop(true);
		CMSG_Server::GetCMSG_Server()->AddMessage("waiting for WaitClientThread...\r\n",1,RED);
		pDlg->pServer->mpNet->cpWaitClientThread->wait4ThisThread();
		CMSG_Server::GetCMSG_Server()->AddMessage("WaitClientThread is closed...\r\n",1,RED);
		
		delete pDlg->pServer->mpNet->cpWaitClientThread;
		delete pDlg->pServer;
	}
	if(CMSG_Server::GetCMSG_Server()!=NULL)
			delete CMSG_Server::GetCMSG_Server();
	::PostMessageA(pDlg->GetSafeHwnd(),ID_ENDAPP,0,0);
	return 0;
}

void CjetServerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	pServer = new CServer();
	pServer->StartServer();
	
	GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON3)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON2)->EnableWindow(TRUE);
}
LRESULT CjetServerDlg::OutString(WPARAM w, LPARAM a)
{
	int min = 0,max =0;
	CString str= CString( (char*)a);
	CString text;
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(text);
	text += str;
	//GetDlgItem(IDC_EDIT1)->SetWindowTextW(text);
	this->SetDlgItemTextW(IDC_EDIT1,text);
	text.Empty();
    m_showmsg.LineScroll(m_showmsg.GetLineCount());
	return true;
}

LRESULT CjetServerDlg::EndFunc(WPARAM w, LPARAM a)
{
	delete cpStopThread;
	OnCancel();
	return true;
}

void CjetServerDlg::OnBnClickedCancel()
{
	cpStopThread = new CThread(EndThread,this);
	/*if(pServer!=NULL)
	{
		pServer->setStop(true);
		CMSG_Server::GetCMSG_Server(this)->AddMessage("waiting for WaitClientThread...\r\n",1,RED);
		cpWaitClientThread->wait4ThisThread();
		CMSG_Server::GetCMSG_Server(this)->AddMessage("WaitClientThread is closed...\r\n",1,RED);
		CMSG_Server::GetCMSG_Server(this)->AddMessage("waiting for CheckClientThread...\r\n",1,RED);
		cpCheckClientThread->wait4ThisThread();
		CMSG_Server::GetCMSG_Server(this)->AddMessage("CheckClientThread is closed...\r\n",1,RED);
		CMSG_Server::GetCMSG_Server(this)->AddMessage("waiting for DataThread...\r\n",1,RED);
		cpDataThread->wait4ThisThread();
		CMSG_Server::GetCMSG_Server(this)->AddMessage("DataThread is closed...\r\n",1,RED);
		delete cpWaitClientThread;
		delete cpCheckClientThread;
		delete cpDataThread;
		delete pServer;
		if(CMSG_Server::GetCMSG_Server(this)!=NULL)
			delete CMSG_Server::GetCMSG_Server(this);
	}
	//delete pServer;
	
	OnCancel();
	*/

}
/*void CjetServerDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	pServer->setStop(true);
	//cpWaitClientThread->wait4ThisThread();
	cpCheckClientThread->wait4ThisThread();
	cpDataThread->wait4ThisThread();
	delete pServer;
}*/

void CjetServerDlg::OnBnClickedButton2()
{
	pServer->setChoice(CIRCLE);
}

void CjetServerDlg::OnBnClickedButton3()
{
	pServer->setChoice(SQUARE);
}

void CjetServerDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	pServer->sendTestData(20,0,0);
}

void CjetServerDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	pServer->sendTestData(-20,0,0);
}

void CjetServerDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	pServer->sendTestData(0,20,0);
}

void CjetServerDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	pServer->sendTestData(0,-20,0);
}

void CjetServerDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	pServer->sendTestData(0,0,10);
}

void CjetServerDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	pServer->sendTestData(0,0,-10);
}
