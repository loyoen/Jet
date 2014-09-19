/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[Module]
	dlgManager

[Description]
	A User interaction dialog implementing for management of jetServer through
network connection.

[Author]
	Jicheng Hu (jicheng.whu @ gmail.com)

[Date]
	2014_07_15

[Revision History]
Date		Who			Where	Description
----------------------------------------------------------------------
2014_07_15	jicheng		WuHan	created!
----------------------------------------------------------------------
2014-07-15	jicheng		Wuhan	first released
----------------------------------------------------------------------
------------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "../jetManager.h"
#include "../MainFrm.h"
#include "../jetManagerDoc.h"
#include "../PictureView.h"

#include "dlgManager.h"
#include "DlgOperator.h"

#define	_WIDTH_STEP_TITLE	100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgManager

IMPLEMENT_DYNCREATE(CDlgManager, CFormView)

//==================================================//
//													//
//					调 用 接 口						//
//													//
//==================================================//
void CDlgManager::SetUserPath( CString strPath )
{
    CjetManagerDoc* pDoc = (CjetManagerDoc*)GetDocument();

	m_strUserPath	= strPath; 

	pDoc->SetUserPath( strPath );
}

void CDlgManager::SetMarkedCnt( int iCnt )
{
	m_iMarkedCnt	= iCnt	;
	m_iCurrentNum	= iCnt+1;

	UpdateData( FALSE );

//	UpdateUI();
}

void CDlgManager::FocusFirst ( )
{
}

void CDlgManager::SetCurrentTask( LPFORMAT_SHEET pTask )
{
	int				i;
    CjetManagerDoc* pDoc = (CjetManagerDoc*)GetDocument();

//, begin
	m_pDlgMsg->Reset ( );
//, ends

	if ( NULL == pTask )
	{
		m_pDlgMsg->ShowWindow ( SW_HIDE );

//, begin
//		pDoc->SetCurrentTaskID( -1 );

		pDoc->SetCurrentTask ( NULL );
//, ends
	}
	else
	{
		CString		strMsg;

//, begin
//		pDoc->SetCurrentTaskID( pTask->iFormatSheetID );

		pDoc->SetCurrentTask ( pTask );
//, ends

	}

	if ( NULL == m_pTaskCurrent )
	{
		m_bModify		= TRUE	;
		m_bEnableModify	= TRUE	;
		m_bEnableNext	= FALSE	;
		m_bEnablePrev	= FALSE	;
	}
		
	m_pTaskCurrent	= pTask;

	//if ( (NULL != m_pTaskCurrent) && (!m_bModify) )
	//{
	//	m_bEnableNext	= (NULL != pTask->pNextTask);
	//	m_bEnablePrev	= (NULL != pTask->pPrevTask);
	//}

//	if ( m_ctrlState == Control_Marked )
//	{
//		this->SetState( Control_Processing );
//	}
}

void CDlgManager::SetStateOnProcessing( )
{
	this->SetState( Control_Processing );
}

//=======================================================================
//
//                   Initialize and Deinitialize 
//
//========================================================================
CDlgManager::CDlgManager()
	: CFormView(CDlgManager::IDD)
{
	//加载隐藏按钮的图片
	m_HideDialogButton.LoadBitmaps(IDB_NAILUP);

	m_strPassword = _T("");
	m_strUserName = theApp.GetProfileString( "LogonInfo", "UserName", _T("请输入管理帐号") );
	m_strServerAddress = theApp.GetProfileString( "LogonInfo", "ServerAddress", _T("192.168.1.12") );
	m_lPort = theApp.GetProfileInt( "LogonInfo", "PortNumber", 8721 );
//	m_lPort	= 8721;

	m_ctrlState		= Control_Logon;
	m_pTaskCurrent	= NULL;

	m_lstStrServerAddress	= new CStringList;

	m_pDlgOperator	= NULL	;
	m_pDlgPrePrinting	= NULL	;
	m_pDlgControl	= NULL	;

	m_pDlgMsg		= NULL	;

	m_bHead	= FALSE	;
	m_bAdmin= FALSE	;

	m_bEnableNext	= FALSE	;
	m_bEnablePrev	= FALSE	;
	m_bEnableModify	= FALSE	;

	// add at 2014/07/30 
	m_pDlgControl	= new CDlgControl ( this );
	m_pDlgControl->Create ( IDD_DLG_CONTROL );
}

CDlgManager::~CDlgManager()
{
//	for ( int i=0; i<m_lstStrServerAddress->GetCount(); i++ )
//	{
//		CString	strTail	= m_lstStrServerAddress->RemoveTail();
////		delete	strTail;
//	}
//	m_lstStrServerAddress->RemoveAll();
	delete	m_lstStrServerAddress;

	if ( NULL != m_pDlgControl )
		delete	m_pDlgControl	;

	if ( NULL != m_pDlgOperator )
		delete	m_pDlgOperator	;

	if ( NULL != m_pDlgMsg )
		delete	m_pDlgMsg;

}

void CDlgManager::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_bModify		= TRUE	;
	m_iMarkedCnt	= 0		;

	m_iCurrentNum	= 0	;

	m_pTaskServer	= ((CMainFrame*)GetParentFrame( ))->m_pTaskServer;
	m_pTaskServer->SetScoreDlg( this );

	//调用文档显示logo图片
    CjetManagerDoc* pDoc = (CjetManagerDoc*)GetDocument();

	pDoc->SetTaskServer( m_pTaskServer );

	m_pDlgMsg	= new CDlgMsg ( this );
	m_pDlgMsg->Create ( IDD_DLG_MSG );
	m_pDlgMsg->ShowWindow ( SW_SHOW );
	m_pDlgMsg->ShowWindow ( SW_HIDE );

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	m_pDlgMsg->Set_doc ( pDoc );
#endif
//, ends

	UpdateUI();
}
//===========================================================
//
//          Message Map and DDX/DDA of MFC
//
//============================================================
void CDlgManager::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgManager)
	DDX_Control(pDX, ID_HIDEDLG_OR_NOT, m_HideDialogButton);
//	DDX_Control(pDX, IDC_UNCHECKJOB, m_btnBadJob);
	DDX_Text(pDX, IDC_PASSWARD, m_strPassword);
	DDX_Text(pDX, IDC_USERNAME, m_strUserName);
	DDX_Text(pDX, IDC_SERVERADDRESS, m_strServerAddress);
	DDX_Text(pDX, IDC_PORT, m_lPort);
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CDlgManager, CFormView)
	//{{AFX_MSG_MAP(CDlgManager)
	ON_BN_CLICKED(ID_HIDEDLG_OR_NOT, OnHidedlgOrNot)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
//	ON_WM_KEYDOWN()
	ON_COMMAND(ID_KEY_UP, OnKeyUp)
	ON_COMMAND(ID_KEY_DOWN, OnKeyDown)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_LOGOUT, OnUpdateCommandLogout)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_MODIFYPWD, OnUpdateCommandModifypwd)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_NOTIFY, OnUpdateCommandNotify)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_SUBMITJOB, OnUpdateCommandSubmittask)
	ON_UPDATE_COMMAND_UI(ID_OPERATOR, OnUpdateOperator)
	ON_COMMAND(ID_OPERATOR, OnOperator)
	ON_COMMAND(ID_CONTROL, OnControlClient)
	ON_WM_MOUSEWHEEL()
	ON_UPDATE_COMMAND_UI(IDC_USER_KICKOUT, OnUpdateUserKickout)
	ON_UPDATE_COMMAND_UI(IDC_USER_RECYCLE, OnUpdateUserRecycle)
	ON_COMMAND(IDC_USER_KICKOUT, OnUserKickout)
END_MESSAGE_MAP()

//================================================================
//
//           CDlgManager diagnostics(Debug Version Only)
//                       
//=================================================================
#ifdef _DEBUG
void CDlgManager::AssertValid() const
{
	CFormView::AssertValid();
}

void CDlgManager::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


//=========================================================
//
//              GUI Updating Routines
//
//==========================================================
void CDlgManager::OnHidedlgOrNot() 
{
	/*RECT rcPictureViewNew;
	RECT rcMainWndRect;

	m_bDialogHide = TRUE;

	//隐藏修执行job对话框		
	ShowWindow(SW_HIDE);

	//将job显示窗口原来的显示参数保存
	CjetManagerDoc* pDoc = (CjetManagerDoc*)GetDocument();
	CPictureView* pPictureView = pDoc->m_pPictureView;
	pPictureView->GetWindowPlacement(&m_wpPictureViewOriginal);

	//将job显示窗口放大
	AfxGetMainWnd()->GetClientRect(&rcMainWndRect);
	m_wpPictureViewFull	= m_wpPictureViewOriginal;
	m_wpPictureViewFull.showCmd	= SW_SHOWNORMAL;
	rcPictureViewNew.left	= rcMainWndRect.left;
	rcPictureViewNew.top	= rcMainWndRect.top;
	CMainFrame* pMainFrame = (CMainFrame*)GetParentFrame();
	if (pMainFrame->IsFullScreen()) 
	{
		rcPictureViewNew.bottom	= rcMainWndRect.bottom;
	}
	else
	{
		rcPictureViewNew.bottom	= rcMainWndRect.bottom - 60;
	}
	rcPictureViewNew.right	= rcMainWndRect.right;
	m_wpPictureViewFull.rcNormalPosition	= rcPictureViewNew;

	pPictureView->SetWindowPlacement(&m_wpPictureViewFull);
	
	//重新显示job
	pPictureView->OnInitialUpdate();

	//隐藏全屏工具栏
	CToolBarCtrl& ToolBarCtrl = pMainFrame->GetToolBar()->GetToolBarCtrl();
	ToolBarCtrl.SetState(ID_VIEW_FULLSCREEN, TBSTATE_HIDDEN);*/
}

//job打印窗口是否隐藏
/*BOOL CDlgManager::IsDialogHide()
{
	return m_bDialogHide;
}*/

//恢复job打印窗口
/*void CDlgManager::ResumeDialog()
{
	m_bDialogHide = FALSE;
	
	CPictureView* pPictureView;

	//分别还原图像显示窗口和job打印窗口
	CjetManagerDoc* pDoc = (CjetManagerDoc*)GetDocument();
	pPictureView = pDoc->m_pPictureView;
	pPictureView->SetWindowPlacement(&m_wpPictureViewOriginal);

	ShowWindow(SW_SHOW);

	return;
}*/

void CDlgManager::SetState( ControlState ctrlState )
{
	m_ctrlState	= ctrlState;
	
	UpdateUI();
}

void CDlgManager::SetButtons ( )
{
	switch ( m_ctrlState )
	{
	case	Control_Logon:
	case	Control_Logon_Wait:
		GetDlgItem(IDC_NUM)					->ShowWindow( SW_HIDE );
//		GetDlgItem(IDC_NEXTJOB)			->ShowWindow( SW_HIDE );
//		GetDlgItem(IDC_NUM)					->ShowWindow( SW_HIDE );
//		GetDlgItem(IDC_PREVIOUSJOB)		->ShowWindow( SW_HIDE );
//		GetDlgItem(IDC_RSCORE)				->ShowWindow( SW_HIDE );
		GetDlgItem(ID_HIDEDLG_OR_NOT)		->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_CONNECTDB)	->ShowWindow( SW_HIDE );
		break;

	case	Control_Head_Registed:	// 组长登陆服务器成功后
	case	Control_Regist_Done:	// 登陆服务器完全成功
		GetDlgItem(IDC_STATIC_MARKED)	->	ShowWindow( SW_SHOW );
		GetDlgItem(IDC_CURRENTNUM)		->	ShowWindow( SW_SHOW );

		//GetDlgItem(ID_HIDEDLG_OR_NOT)	->	ShowWindow( SW_HIDE );
		//GetDlgItem(IDC_NUM)				->	ShowWindow( SW_HIDE );
		break;

	case Control_All_Done:		//所有task都已完成
	case Control_Channel_Done:	//所有job都执行完了
		GetDlgItem(IDC_NUM)					->ShowWindow( SW_HIDE );
		GetDlgItem(ID_HIDEDLG_OR_NOT)		->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_CONNECTDB)	->ShowWindow( SW_HIDE );
		break;

	default:
		break;
	}
}

void CDlgManager::ShowLoginInputs ( int nCmdShow )
{
	GetDlgItem(IDC_USERNAME)->ShowWindow( nCmdShow );
	GetDlgItem(IDC_PASSWARD)->ShowWindow( nCmdShow );
	GetDlgItem(IDC_SERVERADDRESS)->ShowWindow( nCmdShow );
	GetDlgItem(IDC_PORT)->ShowWindow( nCmdShow );
	GetDlgItem(IDOK)->ShowWindow( nCmdShow );
//	GetDlgItem(IDCANCEL)->ShowWindow( nCmdShow );
	GetDlgItem(IDCANCEL)->ShowWindow( SW_HIDE );
}

void CDlgManager::UpdateUI()
{
	CButton btn;

	SetButtons ( );

	switch( m_ctrlState )
	{
	case	Control_Logon:

		ShowLoginInputs ( );

		GetDlgItem(IDC_STATIC_COMMENT0)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_STATIC_COMMENT1)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_STATIC_COMMENT2)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_STATIC_COMMENT3)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_STATIC_COMMENT4)->ShowWindow( SW_SHOW );

		// 下面的几行与Tab顺序有关, 所以不能随意执行动位置
		//GetDlgItem(IDCANCEL)->SetWindowPos( &CWnd::wndTop, 
		//	110, 302, 76, 22, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDOK)->SetWindowPos( &CWnd::wndTop, 
			90, 232, 76, 22, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_STATIC_COMMENT0)->SetWindowPos( &CWnd::wndTop, 
			10, 36, 245, 110, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowPos( &CWnd::wndTop, 
			37, 72, 56, 20, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_SERVERADDRESS)->SetWindowPos( &CWnd::wndTop, 
			98, 188, 136, 26, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_PASSWARD)->SetWindowPos( &CWnd::wndTop, 
			98, 98, 136, 26, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_USERNAME)->SetWindowPos( &CWnd::wndTop, 
			98, 68, 136, 26, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_STATIC_COMMENT2)->SetWindowPos( &CWnd::wndTop, 
			37, 102, 56, 20, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_STATIC_COMMENT3)->SetWindowPos( &CWnd::wndTop, 
			37, 165, 136, 20, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_STATIC_COMMENT4)->SetWindowPos( &CWnd::wndTop, 
			37, 225, 136, 20, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		GetDlgItem(IDC_PORT)->SetWindowPos( &CWnd::wndTop, 
			98, 248, 136, 26, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
	
		GetDlgItem(IDC_STATIC_MARKED)	->	ShowWindow( SW_HIDE );

		GetDlgItem(IDC_CURRENTNUM)		->	ShowWindow( SW_HIDE );

		GetDlgItem(IDC_STATIC_COMMENT0)->SetWindowText( _T("用户登陆信息") );
		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowText( _T("帐号：") );
		GetDlgItem(IDC_USERNAME)->SetWindowText( m_strUserName );
		GetDlgItem(IDC_STATIC_COMMENT2)->SetWindowText( _T("密码：") );
		GetDlgItem(IDC_PASSWARD)->SetWindowText( _T("") );
		GetDlgItem(IDC_STATIC_COMMENT3)->SetWindowText( _T("服务器地址：") );
		GetDlgItem(IDC_SERVERADDRESS)->SetWindowText( m_strServerAddress );
//		GetDlgItem(IDC_STATIC_COMMENT4)->SetWindowText( _T("      端口：") );
		GetDlgItem(IDC_STATIC_COMMENT4)->SetWindowText( _T("      ") );
//		GetDlgItem(IDC_PORT)->SetWindowText( _T(m_lPort) );
		GetDlgItem(IDC_PORT)->ShowWindow( SW_HIDE );
		GetDlgItem(IDOK)->SetWindowText( _T("登 陆") );
		GetDlgItem(IDCANCEL)->SetWindowText( _T("取 消") );
		
		btn.Attach(GetDlgItem(IDOK)->GetSafeHwnd());
		btn.ModifyStyle(0,BS_DEFPUSHBUTTON);
		btn.Detach();

		GetDlgItem(IDC_USERNAME)		->SetFocus();
		((CEdit*)GetDlgItem(IDC_USERNAME))->SetSel( 0, 10 );

		break;

	case	Control_Logon_Wait:
		ShowLoginInputs ( SW_HIDE );

		GetDlgItem(IDC_CURRENTNUM)			->ShowWindow( SW_HIDE );

		GetDlgItem(IDC_STATIC_COMMENT0)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_COMMENT2)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_COMMENT3)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_COMMENT4)->ShowWindow( SW_HIDE );


		GetDlgItem(IDC_STATIC_COMMENT1)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowText( _T("系统正在连接到服务器\r\n\r\n请稍候...") );
		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowPos( &CWnd::wndTop, 
			37, 172, 180, 80, SWP_FRAMECHANGED | SWP_SHOWWINDOW );

		break;

	case	Control_Regist_Wait:

		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowText( _T("正在连接服务器！\r\n\r\n") );
		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowPos( &CWnd::wndTop, 
			37, 62, 180, 80, SWP_FRAMECHANGED | SWP_SHOWWINDOW );

		GetDlgItem(IDC_STATIC_COMMENT2)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_STATIC_COMMENT2)->SetWindowText( _T("系统开始登陆到服务器\r\n\r\n请稍候...") );
		GetDlgItem(IDC_STATIC_COMMENT2)->SetWindowPos( &CWnd::wndTop, 
			37, 162, 180, 80, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		break;

	case	Control_Head_Registed:	// 组长登陆服务器成功后

		ShowLoginInputs ( SW_HIDE );
        
        //Set comments
		GetDlgItem(IDC_STATIC_COMMENT1)->	SetWindowPos( &CWnd::wndTop, 
			10, 42, 260, 18, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		{
			int height = 70;
			GetDlgItem(IDC_STATIC_COMMENT0)->	SetWindowPos( &CWnd::wndTop, 
				5, TOPLINE-50, 240, height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}

		GetDlgItem(IDC_STATIC_COMMENT2)->	SetWindowPos( &CWnd::wndTop, 
			116, TOPLINE-28, 66, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT3)->	SetWindowPos( &CWnd::wndTop, 
			202, TOPLINE-28, 34, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT4)->	SetWindowPos( &CWnd::wndTop, 
			102, 12, 88, 24, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		GetDlgItem(IDC_STATIC_COMMENT0)->	SetWindowText(_T("comment0"));
		GetDlgItem(IDC_STATIC_COMMENT2)->	SetWindowText(_T("comment2"));
		GetDlgItem(IDC_STATIC_COMMENT3)->	SetWindowText(_T("comment3"));
		GetDlgItem(IDC_STATIC_COMMENT4)->	SetWindowText(_T("comment4"));

		{
			CString	strText;
			strText.Format( "当前用户comment1：%s", m_strNickname );
			GetDlgItem(IDC_STATIC_COMMENT1)->	SetWindowText( strText );	
		}

		//Set buttons
		//{	
		//	int yposition;

		//	GetDlgItem(IDC_MODIFY)			->	SetWindowPos( &CWnd::wndTop, 
		//		6,yposition+50,80,42, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		//}
		GetDlgItem(ID_HIDEDLG_OR_NOT)	->	ShowWindow( SW_HIDE );

		break;

	case	Control_Regist_Done:	// 登陆服务器完全成功

		ShowLoginInputs ( SW_HIDE );
        
        //Set comments
		GetDlgItem(IDC_STATIC_COMMENT1)->	SetWindowPos( &CWnd::wndTop, 
			10, 42, 260, 18, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		{
			int height = 70;
			GetDlgItem(IDC_STATIC_COMMENT0)->	SetWindowPos( &CWnd::wndTop, 
				5, TOPLINE-50, 240, height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}

		GetDlgItem(IDC_STATIC_COMMENT2)->	SetWindowPos( &CWnd::wndTop, 
			116, TOPLINE-28, 66, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT3)->	SetWindowPos( &CWnd::wndTop, 
			202, TOPLINE-28, 34, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT4)->	SetWindowPos( &CWnd::wndTop, 
			102, 12, 88, 24, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(ID_HIDEDLG_OR_NOT)	->	ShowWindow( SW_HIDE );

		break;

	// /01/17
	case	Control_Processing://打印job

		ShowLoginInputs ( SW_HIDE );
        
        //Set comments
		GetDlgItem(IDC_STATIC_COMMENT1)->	SetWindowPos( &CWnd::wndTop, 
			10, 42, 260, 18, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		{
			int height = 70;
			GetDlgItem(IDC_STATIC_COMMENT0)->	SetWindowPos( &CWnd::wndTop, 
				5, TOPLINE-50, 240, height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}

		GetDlgItem(IDC_STATIC_COMMENT2)->	SetWindowPos( &CWnd::wndTop, 
			116, TOPLINE-28, 66, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT3)->	SetWindowPos( &CWnd::wndTop, 
			202, TOPLINE-28, 34, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT4)->	SetWindowPos( &CWnd::wndTop, 
			102, 12, 88, 24, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		break;

	// /01/17
	case	Control_Selecting://打印job

		ShowLoginInputs ( SW_HIDE );

        
        //Set comments
		GetDlgItem(IDC_STATIC_COMMENT1)->	SetWindowPos( &CWnd::wndTop, 
			10, 42, 260, 18, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		{
			int height = 70;
			GetDlgItem(IDC_STATIC_COMMENT0)->	SetWindowPos( &CWnd::wndTop, 
				5, TOPLINE-50, 240, height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}

		GetDlgItem(IDC_STATIC_COMMENT2)->	SetWindowPos( &CWnd::wndTop, 
			116, TOPLINE-28, 66, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT3)->	SetWindowPos( &CWnd::wndTop, 
			202, TOPLINE-28, 34, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT4)->	SetWindowPos( &CWnd::wndTop, 
			102, 12, 88, 24, SWP_FRAMECHANGED | SWP_SHOWWINDOW);

		break;

	case	Control_Reprocessing://重新打印job

		ShowLoginInputs ( SW_HIDE );
        
        //Set comments
		GetDlgItem(IDC_STATIC_COMMENT1)->	SetWindowPos( &CWnd::wndTop, 
			10, 42, 260, 18, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		{
			int height = 70;
			GetDlgItem(IDC_STATIC_COMMENT0)->	SetWindowPos( &CWnd::wndTop, 
				5, 126, 240, height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}

		break;

	case	Control_Marked: //task已经修执行过
		
		ShowLoginInputs ( SW_HIDE );
        
        //Set comments
		GetDlgItem(IDC_STATIC_COMMENT1)->	SetWindowPos( &CWnd::wndTop, 
			10, 42, 260, 18, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		{
			int height = 70;
			GetDlgItem(IDC_STATIC_COMMENT0)->	SetWindowPos( &CWnd::wndTop, 
				5, TOPLINE-50, 240, height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}

		GetDlgItem(IDC_STATIC_COMMENT2)->	SetWindowPos( &CWnd::wndTop, 
			116, TOPLINE-28, 66, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT3)->	SetWindowPos( &CWnd::wndTop, 
			202, TOPLINE-28, 34, 16, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		GetDlgItem(IDC_STATIC_COMMENT4)->	SetWindowPos( &CWnd::wndTop, 
			102, 12, 88, 24, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		break;

	case Control_All_Done://所有工作都已完成


		GetDlgItem(IDC_STATIC_COMMENT0)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_COMMENT3)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_COMMENT4)->ShowWindow( SW_HIDE );

		ShowLoginInputs ( SW_HIDE );

		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowText( _T("成功登出!\r\n\r\n") );
		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowPos( &CWnd::wndTop, 
			37, 122, 180, 30, SWP_FRAMECHANGED | SWP_SHOWWINDOW );

		GetDlgItem(IDC_STATIC_COMMENT2)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_STATIC_COMMENT2)->SetWindowText( _T("全部完成!\r\n\r\n请关闭程序") );
		GetDlgItem(IDC_STATIC_COMMENT2)->SetWindowPos( &CWnd::wndTop, 
			37, 182, 180, 80, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		break;

	case Control_Channel_Done://所有task都做完了

		ShowLoginInputs ( SW_HIDE );

		GetDlgItem(IDC_STATIC_COMMENT0)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_COMMENT3)->ShowWindow( SW_HIDE );
		GetDlgItem(IDC_STATIC_COMMENT4)->ShowWindow( SW_HIDE );

		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowText( _T("成功登出!\r\n\r\n") );
		GetDlgItem(IDC_STATIC_COMMENT1)->SetWindowPos( &CWnd::wndTop, 
			37, 122, 180, 30, SWP_FRAMECHANGED | SWP_SHOWWINDOW );

		GetDlgItem(IDC_STATIC_COMMENT2)->ShowWindow( SW_SHOW );
		GetDlgItem(IDC_STATIC_COMMENT2)->SetWindowText( _T("\r\n\r\n请关闭程序") );
		GetDlgItem(IDC_STATIC_COMMENT2)->SetWindowPos( &CWnd::wndTop, 
			37, 182, 200, 150, SWP_FRAMECHANGED | SWP_SHOWWINDOW );
		break;

	default:
		break;
	}
	
	RedrawWindow();
}
/*++
[Description]
GUI 元素：
ID                    class
-----------------------------
IDC_NUM                  Combox
IDC_RSCORE               Combox

--*/
void CDlgManager::UpdateGUI(int nJobID,
						  float fTotalScore,
						  float fOldScore,
						  float fNowScore
						  )
{
	CString str;

	CjetManagerDoc* pFinalDoc = (CjetManagerDoc*)GetDocument();
	CComboBox  *pJobList = (CComboBox*)GetDlgItem(IDC_NUM);
	
	//显示task图片
	GetCurrentDirectory(MAX_PATH,str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	CString m_strCurPictPath;
    m_strCurPictPath.Format("%s\\Pictures\\%d.gif",str,nJobID);
	TRACE1("DiaplayTaskPict:%s\n",m_strCurPictPath);
	pFinalDoc->DiaplayTaskPict(m_strCurPictPath);	
}


//===================================================================
//
//                    用户界面的事件处理函数
//
//====================================================================
void CDlgManager::OnBnClickedOk()
{
//	CMessageManager *pMgr=NULL;
//	t_gui_connect *p=NULL;
    CjetManagerDoc* pFinalDoc = (CjetManagerDoc*)GetDocument();

	char *		strStop;
	CString		strAddress;
	CString		strRight;
	CString		strTemp;
	int			iPos;
	long		lAddress;

	switch( m_ctrlState )
	{
	case	Control_Logon:
		UpdateData(TRUE);

		// 将输入的帐号转变为小写
		m_strUserName.MakeLower();

		// 过滤掉空格
		while ( TRUE )
		{
			int	iPos	= m_strUserName.Find( " " );
			if ( iPos == -1 )
				break;

			m_strUserName.Delete( iPos );
		}

		if ( m_strUserName.GetLength() == 0 )	// block stack overflow attacks
		{
			AfxMessageBox( "未输入用户帐号！" );
			GetDlgItem(IDC_USERNAME)->SetFocus();
			return;
		}

		// 将输入的密码转变为小写
		m_strPassword.MakeLower();

		// 过滤掉空格
		while ( TRUE )
		{
			int	iPos	= m_strPassword.Find( " " );
			if ( iPos == -1 )
				break;

			m_strPassword.Delete( iPos );
		}
		if ( m_strPassword.GetLength() == 0 )	// block stack overflow attacks
		{
			AfxMessageBox( "未输入密码！" );
			GetDlgItem(IDC_PASSWARD)->SetFocus();
			return;
		}

		m_ctrlState	= Control_Logon_Wait;
		UpdateUI();

		strAddress.Empty();
		iPos	= m_strServerAddress.Find( "." );
		strTemp	= m_strServerAddress.Left( iPos );
		strRight= m_strServerAddress.Right(m_strServerAddress.GetLength() - iPos - 1);
		lAddress= strtol( strTemp, &strStop, 10 );
		if ( (lAddress<0) || (lAddress>255) )
		{
			AfxMessageBox( "服务器地址输入有错误！" );
			GetDlgItem(IDC_SERVERADDRESS)->SetFocus();
			return;
		}
		strTemp.Format( "%d.", lAddress );
		strAddress	+= strTemp;
		m_lstStrServerAddress->AddTail( strAddress );

		iPos	= strRight.Find( "." );
		strTemp	= strRight.Left( iPos );
		strRight= strRight.Right( strRight.GetLength() - iPos - 1 );
		lAddress= strtol( strTemp, &strStop, 10 );
		if ( (lAddress<0) || (lAddress>255) )
		{
			AfxMessageBox( "服务器地址输入有错误！" );
			GetDlgItem(IDC_SERVERADDRESS)->SetFocus();
			return;
		}
		strTemp.Format( "%d.", lAddress );
		strAddress	+= strTemp;
		m_lstStrServerAddress->AddTail( strAddress );

		iPos	= strRight.Find( "." );
		strTemp	= strRight.Left( iPos );
		strRight= strRight.Right( strRight.GetLength() - iPos - 1 );
		lAddress= strtol( strTemp, &strStop, 10 );
		if ( (lAddress<0) || (lAddress>255) )
		{
			AfxMessageBox( "服务器地址输入有错误！" );
			GetDlgItem(IDC_SERVERADDRESS)->SetFocus();
			return;
		}
		strTemp.Format( "%d.", lAddress );
		strAddress	+= strTemp;
		m_lstStrServerAddress->AddTail( strAddress );

		lAddress= strtol( strRight, &strStop, 10 );
		if ( (lAddress<0) || (lAddress>255) )
		{
			AfxMessageBox( "服务器地址输入有错误！" );
			GetDlgItem(IDC_SERVERADDRESS)->SetFocus();
			return;
		}
		strTemp.Format( "%d", lAddress );
		strAddress	+= strTemp;
		m_lstStrServerAddress->AddTail( strAddress );

		theApp.WriteProfileString( "LogonInfo", "ServerAddress", strAddress );
		theApp.WriteProfileString( "LogonInfo", "UserName", m_strUserName );
		theApp.WriteProfileInt( "LogonInfo", "PortNumber", m_lPort );

		if ( m_strUserName.GetLength() > 16 )	// block stack overflow attacks
		{
			m_strUserName	= m_strUserName.Left( 16 );
		}
		if ( m_strPassword.GetLength() > 32 )	// block stack overflow attacks
		{
			m_strPassword	= m_strPassword.Left( 32 );
		}

		m_pTaskServer->SetLogonInfo( m_strUserName, m_strPassword );
		if ( m_pTaskServer->ConnectServer( strAddress, (u_short)m_lPort ) )
		{
			m_ctrlState	= Control_Regist_Wait;
			UpdateUI();
			m_pTaskServer->Logon( );
			m_pTaskServer->SendTypeToServer();
		}
		else
		{
			AfxMessageBox( "连接到服务器失败!" );
			m_ctrlState	= Control_Logon;
			UpdateUI();
		}

		break;

	default:
		break;
	}
}

//======================================================
//
//                 job管理函数
//
//=======================================================
void  CDlgManager::InsertNewJob(int nJobID)
{
	UpdatePicView(nJobID);
/*
	CjetManagerDoc* pFinalDoc = (CjetManagerDoc*)GetDocument();
	CComboBox  *pJobList = (CComboBox*)GetDlgItem(IDC_NUM);
	CString str;
	str.Format("%d",nJobID);
	pJobList->InsertString(-1,str);

	if(pJobList->GetCount()==1)
	{/*
		int     nJobID=0;
		float   fTotalScore=0.0f;
		float   fOldScore=0.0f;

		pFinalDoc->GetNextJob(nJobID,fTotalScore,fOldScore);****

		if(nJobID!=-1)
		{
			//UpdateGUI(nJobID,fTotalScore,fOldScore,0.0f);
		}
	}
*/
}

// 先根据情况来更新界面
void CDlgManager::OnBnClickedButtonRequest()
{
	if ( NULL == m_pTaskCurrent )
	{
		m_pTaskServer->RequestTask( );
		return;
	}

	// 如果当前的job已经执行完
	// 则先移动到下一张
	if ( IsCurrentMarked( ) )
	{
		// 如果没有下一张, 则将当前图片设置为logo
		if ( NULL == m_pTaskCurrent->pNextTask )
		{
			CjetManagerDoc* pDoc = (CjetManagerDoc*)GetDocument();

			pDoc->SetCurrentTask ( NULL );
		
			m_pTaskCurrent	= NULL;

			UpdateData(FALSE);
//%%%%%%%% today
//			UpdateData(TRUE);
//today
			SetState( Control_Processing );
		}
		else
		{
		}
	}
	m_pTaskServer->RequestTask( );

//	if ( NULL == m_pTaskCurrent )
//	{
//	}
}

void CDlgManager::OnLoginFailure( CString strInfo )
{
	AfxMessageBox( strInfo );
	SetState( Control_Logon );
}

//5-01-21,begin
void CDlgManager::OnLoginOK()
{
	if ( m_bAdmin )
	{
		SetState ( Control_All_Done )	;
		return;
	}

	UpdateData( FALSE );
//	UpdateData( TRUE );

	if ( m_channelState == channelPreGrad )
	{
		if ( 1 == m_bHead )
		{
			SetState ( Control_Selecting );
		}
		else
		{
			SetState ( Control_Regist_Done );
		}
	}
	else
	{
		if ( 1 == m_bHead )
		{
			SetState ( Control_Head_Registed );
	
			m_pDlgMsg->ShowWindow ( SW_SHOW );

		}
		else
		{
			SetState ( Control_Regist_Done );
	
			m_pDlgMsg->ShowWindow ( SW_HIDE );
		}
	}
	
}
//5-01-21,end

void CDlgManager::OnChannelState ( LPPACK_CHANNEL_STATE lpChannelState )
{
	if ( NULL != m_pDlgOperator )
	{
		if ( m_bAdmin )
			m_pDlgOperator->On_admin_channel_state ( lpChannelState );
		else if ( m_bHead )
			m_pDlgOperator->OnChannelState ( lpChannelState );
	}
}

//5-01-23,begin
// 更新显示图片
void CDlgManager::UpdatePicView(int iFormatSheetID)
{
	CjetManagerDoc* pFinalDoc = (CjetManagerDoc*)GetDocument();

	CString str;
	GetCurrentDirectory(MAX_PATH, str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();

	CString m_strPicPath;
    m_strPicPath.Format("%s\\%d.gif", str, iFormatSheetID);

//	pFinalDoc->DiaplayTaskPict(m_strPicPath);
}
//5-01-23,end

void CDlgManager::OnKeyUp()
{
	int j=-1;
	
	//判断是否为IDC_MODIFY
//	if (this->GetFocus()==GetDlgItem(IDC_MODIFY))
//	{
//		GetDlgItem(IDC_STEP_MARK0)->SetFocus();
//		((CEdit*)GetDlgItem(IDC_STEP_MARK0))->SetSel( 0, 3 );
//		return;
//	}
//
//	for (int i=0;i<m_iSteps;i++)
//		if (this->GetFocus()==GetDlgItem(IDC_STEP_MARK0+i))
//		{
//			j=i;
//			break;
//		}
//
//	if (j!=-1)
//	{
//		j--;
//		if (j<0)
//			GetDlgItem(IDC_MODIFY)->SetFocus();
//		else
//		{
////			GetDlgItem(IDC_STEP_MARK0+j)->SetFocus();
////			((CEdit*)GetDlgItem(IDC_STEP_MARK0+j))->SetSel( 0, 3 );
//		}
//	}
}

void CDlgManager::OnKeyDown()
{
	// TODO: 在此添加命令处理程序代码
	int j=-1;

	//判断是否为IDC_MODIFY
	if (this->GetFocus()==GetDlgItem(IDC_MODIFY))
	{
//		GetDlgItem(IDC_STEP_MARK0)->SetFocus();
//		((CEdit*)GetDlgItem(IDC_STEP_MARK0))->SetSel( 0, 3 );
		return;
	}

	//判断目前焦点在哪个作业栏
	//for (int i=0;i<m_iSteps;i++)
	//	if (this->GetFocus()==GetDlgItem(IDC_STEP_MARK0+i))
	//	{
	//		j=i;
	//		break;
	//	}

	//if (j!=-1)
	//{
	//	j++;
	//	if (j>(m_iSteps-1))
	//		GetDlgItem(IDC_MODIFY)->SetFocus();
	//	else	
	//	{
	//		GetDlgItem(IDC_STEP_MARK0+j)->SetFocus();
	//		((CEdit*)GetDlgItem(IDC_STEP_MARK0+j))->SetSel( 0, 3 );
	//	}
	//}

}

//5-02-01,begin
void CDlgManager::OnModifyPwd(CString str)
{
	AfxMessageBox(str);
}

void CDlgManager::OnLogoutFailed(CString str)
{
	AfxMessageBox(str);
}
//5-02-01,end

void CDlgManager::OnLogoutOK()
{
	// 切换到登出界面
	this->SetState(Control_All_Done);
}

void CDlgManager::OnChannelDone()
{
	this->SetState(Control_Channel_Done);
}

void CDlgManager::DisableLogout()
{
	this->SetState(Control_Logon_Wait);
}

void CDlgManager::OnTwiceLogin(int iMachineID, int iIDJob, CString str)
{
	int result = MessageBox( str, "警告", MB_YESNO);
	switch(result)
	{
	case IDYES:
		m_pTaskServer->OnKickout(iMachineID, iIDJob);
		AfxMessageBox("已经将该用户踢出！请登陆");
		//m_pTaskServer->Logon();
		this->SetState(Control_Logon);
		break;

	case IDNO:
		this->SetState(Control_Logon);
		break;

	default:
		break;
	}
}

BOOL CDlgManager::IsCurrentMarked( )
{
	return	(m_iMarkedCnt == m_iCurrentNum);
}

void CDlgManager::OnUpdateCommandLogout(CCmdUI *pCmdUI)
{
	switch ( m_ctrlState )
	{
	case	Control_Regist_Done:
	case	Control_Processing:	
	case	Control_Reprocessing: 
	case	Control_Marked:
	case	Control_Selecting:
		if ( m_iMarkedCnt == m_iCurrentNum )
			pCmdUI->Enable( TRUE );
		else if ( m_channelState == channelPreGrad )
			pCmdUI->Enable( TRUE );
		else
			pCmdUI->Enable( FALSE );
		break;

	default:
		pCmdUI->Enable( FALSE );
		break;
	}

	if ( m_bAdmin )
		pCmdUI->Enable ( FALSE );
}

void CDlgManager::OnUpdateCommandModifypwd(CCmdUI *pCmdUI)
{
	switch ( m_ctrlState )
	{
	case	Control_Regist_Done:
	case	Control_Processing:	
	case	Control_Reprocessing: 
	case	Control_Marked:
		pCmdUI->Enable( TRUE );
		break;

	default:
		pCmdUI->Enable( FALSE );
		break;
	}
}

void CDlgManager::OnUpdateCommandNotify(CCmdUI *pCmdUI)
{
	switch ( m_ctrlState )
	{
	case	Control_Regist_Done:
	case	Control_Processing:	
	case	Control_Reprocessing: 
	case	Control_Marked:
		pCmdUI->Enable( TRUE );
		break;

	default:
		pCmdUI->Enable( FALSE );
		break;
	}
}

void CDlgManager::OnUpdateCommandSubmittask(CCmdUI *pCmdUI)
{
	switch ( m_ctrlState )
	{
	case	Control_Regist_Done:
	case	Control_Processing:	
	case	Control_Reprocessing: 
	case	Control_Marked:
		pCmdUI->Enable( TRUE );
		break;

	default:
		pCmdUI->Enable( FALSE );
		break;
	}
}

void CDlgManager::OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	int	i=0;
	if ( pSender != this )
		return;
}

void CDlgManager::OnUpdateBadJob(CCmdUI *pCmdUI)
{
	pCmdUI->Enable( m_pTaskCurrent != NULL );
	if ( NULL == m_pTaskCurrent )
		return;

//	pCmdUI->SetCheck( TRUE == m_pTaskCurrent->bBadJob );
}

void CDlgManager::OnUpdateRequesttask(CCmdUI *pCmdUI)
{
//	pCmdUI->Enable( m_pTaskCurrent == NULL );
	pCmdUI->Enable( TRUE );
}

void CDlgManager::OnUpdateOperator(CCmdUI *pCmdUI)
{
	switch ( m_ctrlState )
	{
	case	Control_Regist_Done:
	case	Control_Processing:	
	case	Control_Reprocessing: 
	case	Control_Marked:
	case	Control_Head_Registed:
		if ( m_bHead )
			pCmdUI->Enable( TRUE )	;
		else
			pCmdUI->Enable( FALSE )	;
		break;

	default:
		pCmdUI->Enable( FALSE );
		break;
	}

	if ( m_bAdmin )
		pCmdUI->Enable( TRUE )	;
}

void CDlgManager::OnOperator()
{
	if ( NULL == m_pDlgOperator )
	{
		m_pDlgOperator	= new CDlgOperator( this );
		m_pDlgOperator->Create( IDD_DLG_OPERATOR );
	}
	m_pDlgOperator->ShowWindow(SW_RESTORE)	;
	m_pDlgOperator->ShowInfo ( )			;

//	if ( NULL == m_pDlgControl )
//	{
//		m_pDlgControl	= new CDlgControl ( this );
//		m_pDlgControl->Create ( IDD_DLG_CONTROL );
//		m_pDlgControl->FullScreen ( );
//
//		m_pDlgOperator	= m_pDlgControl->m_pDlgOperator;
//		m_pDlgPrePrinting	= m_pDlgControl->m_pDlgPrePrinting;
//	}
//
////	m_pDlgControl->ShowWindow ( SW_RESTORE );
//	m_pDlgControl->ShowWindow ( SW_SHOW );
//	m_pDlgPrePrinting->ShowWindow ( SW_SHOW ); 
//	m_pDlgOperator->ShowWindow ( SW_SHOW );
}


void CDlgManager::OnControlClient()
{
	if ( NULL == m_pDlgControl )
	{
//		m_pDlgControl	= new CcontrolClientDlg ( this );
//		m_pDlgControl->Create ( IDD_CONTROLCLIENT_DIALOG );
		m_pDlgControl	= new CDlgControl ( this );
		m_pDlgControl->Create ( IDD_DLG_CONTROL );
	}

//	m_pDlgControl->ShowWindow ( SW_RESTORE );
	m_pDlgControl->ShowWindow ( SW_SHOW );
	m_pDlgControl->FullScreen ( );
}

void CDlgManager::UpdateChannelInfo ( )
{
	if ( m_bAdmin )
	{
		m_pTaskServer->CheckServerState ( )	;
		return	;
	}

	if ( m_bHead )
	{
		m_pTaskServer->CheckChannelState ( )	;
		return	;
	}
}

void CDlgManager::AllLogout ( )
{
	if ( m_bAdmin )
	{
		m_pTaskServer->AllLogout ( )	;
		return	;
	}

	if ( m_bHead )
	{
		m_pTaskServer->ChannelLogout ( )	;
		return	;
	}
}

BOOL CDlgManager::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CjetManagerDoc* pFinalDoc = (CjetManagerDoc*)GetDocument();

	pFinalDoc->m_pPictureView->OnMouseWheel( nFlags, zDelta, pt )	;

	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlgManager::OnUpdateUserKickout(CCmdUI *pCmdUI)
{
	OnUpdateOperator ( pCmdUI );
}

void CDlgManager::OnUpdateUserRecycle(CCmdUI *pCmdUI)
{
	OnUpdateOperator ( pCmdUI );
}

void CDlgManager::OnUserKickout()
{
	if ( NULL == m_pDlgOperator )
		return;

	if ( -1 == m_pDlgOperator->m_nCurrent_MachineID )
		return;

	if ( m_bHead )
	{
		m_pTaskServer->Kick_out ( m_pDlgOperator->m_nCurrent_MachineID );
	}
	else if ( m_bAdmin )
	{
		if ( -1 == m_pDlgOperator->m_nCurrent_questionID )
			return;

		m_pTaskServer->Kick_out ( m_pDlgOperator->m_nCurrent_MachineID, m_pDlgOperator->m_nCurrent_questionID );
//		m_pDlgOperator->Set_current_questionID ( -1 );
	}

	m_pDlgOperator->Set_current_teacerID ( -1 );
}

