
// controlClientDlg.cpp : implementation file
//

#include <atlimage.h>
#include <afxtempl.h>

#include "stdafx.h"
#include "../jetManager.h"
#include "controlClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcontrolClientDlg dialog

CcontrolClientDlg::CcontrolClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcontrolClientDlg::IDD, pParent)
{
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcontrolClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_List_Device);
	DDX_Control(pDX, IDC_LIST_JET, m_List_Jet);
	DDX_Control(pDX, IDC_LIST_TASK, m_List_Task);
}

BEGIN_MESSAGE_MAP(CcontrolClientDlg, CDialog)
//	ON_WM_SYSCOMMAND()
//	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CcontrolClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CcontrolClientDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CcontrolClientDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CcontrolClientDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CcontrolClientDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CcontrolClientDlg message handlers

BOOL CcontrolClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
//	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
//	ASSERT(IDM_ABOUTBOX < 0xF000);

	//CMenu* pSysMenu = GetSystemMenu(FALSE);
	//if (pSysMenu != NULL)
	//{
	//	BOOL bNameValid;
	//	CString strAboutMenu;
	//	bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
	//	ASSERT(bNameValid);
	//	if (!strAboutMenu.IsEmpty())
	//	{
	//		pSysMenu->AppendMenu(MF_SEPARATOR);
	//		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	//	}
	//}

	//// Set the icon for this dialog.  The framework does this automatically
	////  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	Init();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CcontrolClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	//if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	//{
	//	CAboutDlg dlgAbout;
	//	dlgAbout.DoModal();
	//}
	//else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CcontrolClientDlg::OnPaint()
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
HCURSOR CcontrolClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CcontrolClientDlg::Init()
{
	this->InitListCtrl();
	m_Client.SetMainDlg(this);
	m_Client.InitClient();
}
void CcontrolClientDlg::InitListCtrl()
{
	CListCtrl* pList_Device = (CListCtrl*)GetDlgItem(IDC_LIST_DEVICE);
	pList_Device->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pList_Device->InsertColumn(0,CString("DEVICE END"),LVCFMT_LEFT,500);

	CListCtrl* pList_Jet = (CListCtrl*)GetDlgItem(IDC_LIST_JET);
	pList_Jet->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pList_Jet->InsertColumn(0,CString("JET END"),LVCFMT_LEFT,500);

	CListCtrl* pList_Output = (CListCtrl*)GetDlgItem(IDC_LIST_OUTPUT);
	pList_Output->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pList_Output->InsertColumn(0,CString("Information"),LVCFMT_LEFT,500);

	CListCtrl* pList_Task = (CListCtrl*)GetDlgItem(IDC_LIST_TASK);
	pList_Task->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pList_Task->InsertColumn(0,CString("TASK"),LVCFMT_LEFT,200);
	pList_Task->InsertColumn(1,CString("STATE"),LVCFMT_LEFT,100);
}



void CcontrolClientDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	bool FindDevice = false, FindJet = false;
	int i=0,j=0;
	for(i=0;i<m_List_Device.GetItemCount();i++)
	{
		if(m_List_Device.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			FindDevice = true;
			break;
		}
	}
	for(j=0;j<m_List_Jet.GetItemCount();j++)
	{
		if(m_List_Jet.GetItemState(j,LVIS_SELECTED)==LVIS_SELECTED)
		{
			FindJet = true;
			break;
		}
	}
	if(FindDevice&&FindJet)
		m_Client.SendBindInfo(i,j);
}


void CcontrolClientDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
        NULL, 
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"),
        NULL);
    if(dlg.DoModal()==IDOK)
    {
        FilePathName=dlg.GetPathName(); //文件名保存在了FilePathName里
    }
    else
    {
         return;
    }
	DrawPic(FilePathName);
}
void CcontrolClientDlg::DrawPic(CString filename)
{
	CDialog::OnPaint();
	CImage image; //使用图片类
	image.Load( filename ); //装载路径下图片信息到图片类

	RECT rect = { 0 };
	rect.bottom = image.GetWidth();
	rect.right = image.GetHeight();

	CDC* pDC = GetDlgItem(IDC_PIC)->GetWindowDC();//m_mm.GetWindowDC(); //获得显示控件的DC
	pDC->SetStretchBltMode(STRETCH_HALFTONE); 
	image.Draw( pDC->m_hDC,rect); //图片类的图片绘制Draw函数
	ReleaseDC( pDC ); 
	m_Client.LoadPic(FilePathName);
	//CopyPic();
}


void CcontrolClientDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	/*bool FindJet = false;
	int j=0;
	vector<int> JetList;
	for(j=0;j<m_List_Jet.GetItemCount();j++)
	{
		if(m_List_Jet.GetItemState(j,LVIS_SELECTED)==LVIS_SELECTED)
		{
			FindJet = true;
			JetList.push_back(j);
			//break;
		}
	}
	if(FindJet)*/
	int i=0;
	for(i=0;i<m_List_Task.GetItemCount();i++)
		if(m_List_Task.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			break;
		}
	m_Client.SendPic(i);


}


void CcontrolClientDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	vector<int> DeviceList;
	int i=0;
	for(i=0;i<m_List_Task.GetItemCount();i++)
		if(m_List_Task.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			break;
		}

	int j = 0;
	for(j=0;j<m_List_Device.GetItemCount();j++)
		if(m_List_Device.GetItemState(j,LVIS_SELECTED)==LVIS_SELECTED)
			DeviceList.push_back(j);
	for(int k=0;k<int(DeviceList.size());k++)
		m_Client.SendInstruction(DeviceList[k],i,4);
}


void CcontrolClientDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here

}
