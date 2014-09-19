
#include "stdafx.h"
#include "jetManager.h"

#include "MainFrm.h"
#include "PictureView.h"
#include "GUI/dlgManager.h"
#include "jetManagerDoc.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

//======================================================
//
//        Initialize and Deinitailize
//
//=======================================================

CMainFrame::CMainFrame()
{
	GetCurrentDirectory( _MAX_PATH, m_pathApp );

	m_bFullScreen		= FALSE ;
	m_pwndFullScrnBar	= NULL	;

	m_pTaskServer	= (CThreadTaskServer*)AfxBeginThread( 
		RUNTIME_CLASS(CThreadTaskServer),
		THREAD_PRIORITY_NORMAL // int nPriority
		);

	m_pTaskServer->SetAppPath( m_pathApp );

	m_lpLargeMemList	= NULL;
	m_lpSmallMemList	= NULL;
	m_lpCommandMemList	= NULL;

	if ( NULL != m_pTaskServer )
	{
		int		i;
		HGLOBAL	hHandle;
		LPVOID	lpBuffer;

		m_lpLargeMemList	= new CPtrList;
		for ( i=0; i<CNT_CLIENT_LARGE_MEM; i++ )
		{
			hHandle		= ::GlobalAlloc( GHND, SIZE_LARGE_MEM );
			lpBuffer	= ::GlobalLock( hHandle );
			if ( NULL == lpBuffer )
			{
				AfxMessageBox( "内存不足\n请退出程序" );
				return;
			}
			m_lpLargeMemList->AddTail( lpBuffer );
		}

		m_lpSmallMemList	= new CPtrList;
		for ( i=0; i<CNT_CLIENT_SMALL_MEM; i++ )
		{
			hHandle		= ::GlobalAlloc( GHND, SIZE_SMALL_MEM );
			lpBuffer	= ::GlobalLock( hHandle );
			if ( NULL == lpBuffer )
			{
				AfxMessageBox( "内存不足\n请退出程序" );
				return;
			}
			m_lpSmallMemList->AddTail( lpBuffer );
		}

		m_lpCommandMemList	= new CPtrList;
		for ( i=0; i<CNT_CLIENT_COMMAND_MEM; i++ )
		{
			hHandle		= ::GlobalAlloc( GHND, SIZE_COMMAND_MEM );
			lpBuffer	= ::GlobalLock( hHandle );
			if ( NULL == lpBuffer )
			{
				AfxMessageBox( "内存不足\n请退出程序" );
				return;
			}
			m_lpCommandMemList->AddTail( lpBuffer );
		}

		m_pTaskServer->SetBufferPtrList( m_lpLargeMemList, m_lpSmallMemList, m_lpCommandMemList );
	}
}

CMainFrame::~CMainFrame()
{
	if ( NULL != m_pTaskServer )
	{
		m_pTaskServer->Stop ( );
	}

	Sleep(100);

	if ( NULL != m_lpLargeMemList )
	{
		HGLOBAL	hHandle;
		for ( int i=0; i<CNT_CLIENT_LARGE_MEM; i++ )
		{
			hHandle	= m_lpLargeMemList->RemoveTail( );
			::GlobalUnlock( hHandle );
			::GlobalFree( hHandle );
		}

		delete	m_lpLargeMemList;
	}

	if ( NULL != m_lpSmallMemList )
	{
		HGLOBAL	hHandle;
		for ( int i=0; i<CNT_CLIENT_SMALL_MEM; i++ )
		{
			hHandle	= m_lpSmallMemList->RemoveTail( );
			::GlobalUnlock( hHandle );
			::GlobalFree( hHandle );
		}

		delete	m_lpSmallMemList;
	}

	////////////////////////疑似泄漏///////////////////////////////////
	if ( NULL != m_lpCommandMemList )
	{
		HGLOBAL	hHandle;
		for ( int i=0; i<CNT_CLIENT_COMMAND_MEM; i++ )
		{
			hHandle	= m_lpCommandMemList->RemoveTail( );
			::GlobalUnlock ( hHandle );
			::GlobalFree( hHandle );
		}

		delete	m_lpCommandMemList;
	}


	if (IsFullScreen()) 
	{
		m_pwndFullScrnBar->DestroyWindow();
		delete m_pwndFullScrnBar;
	}
}

CToolBar* CMainFrame::GetToolBar()
{
	return &m_wndToolBar;
}

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

//================================================
//
//             消息映射和事件响应
//
//=================================================
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_FULLSCREEN, OnViewFullscreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULLSCREEN, OnUpdateViewFullscreen)
	ON_WM_GETMINMAXINFO()
	// 全局帮助命令
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_WM_SHOWWINDOW()

	ON_WM_TIMER()
	ON_WM_CLOSE()
//	ON_WM_MOUSEWHEEL()
//	ON_WM_VSCROLL()
END_MESSAGE_MAP()




//====================================================
//
//            基类的函数重载
//
//=====================================================
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	 | WS_THICKFRAME|WS_MAXIMIZE | WS_SYSMENU;
	return TRUE;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	m_wndSplitter.CreateStatic(this, 1, 2);
//	m_wndSplitter.Create ( this, 1, 2, CSize(10,10), pContext );
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CPictureView), CSize(750, 0), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CDlgManager), CSize(0, 0), pContext);
//	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CRightView), CSize(100, 0), pContext);
//	m_wndSplitter.CreateView(0, 2, RUNTIME_CLASS(CDlgManager), CSize(0, 0), pContext);
	
	//m_wndSplitter.SetColumnInfo ( 0, 0, 0 );
	//m_wndSplitter.DeleteColumn ( 0 )	;

	return TRUE;
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
}

void CMainFrame::ShowScoreDlg ( )
{
	CjetManagerDoc *	pDoc	;
	CView *			pView	;
	pDoc	= (CjetManagerDoc *)this->GetActiveDocument ( )	;

	POSITION	pos	;
	
	pos	= pDoc->GetFirstViewPosition ( )	;
	while ( NULL != pos )
	{
		pView	= pDoc->GetNextView ( pos )	;

		if ( pView->IsKindOf ( RUNTIME_CLASS(CDlgManager) ) )
		{
			SetActiveView ( pView )	;
			break;
		}
	}
}

//====================================================
//
//          CMainFrame 诊断(Debug Version Only)
//
//====================================================
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

//====================================================
//
//                    窗口消息处理
//
//=====================================================
//WM_CREATE
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//为网络设置Timer 10s
    m_nTimerID=SetTimer(0,2000,NULL);

	return 0;
}

void CMainFrame::EndFullScreen()
{
	if (IsFullScreen()) 
	{
		//得到视图区的客户区大小，以便后面执行变显示的参数
		CjetManagerDoc* pDoc = (CjetManagerDoc*)GetActiveDocument();
		
		m_pwndFullScrnBar->DestroyWindow();
		delete m_pwndFullScrnBar;

		m_bFullScreen=FALSE;

//	    m_wndStatusBar.ShowWindow(SW_SHOWNORMAL);
		m_wndToolBar.ShowWindow(SW_SHOWNORMAL);

		SetWindowPlacement ( &m_wpPrev );

		//恢复job打印窗口的显示
		CDlgManager* pScoreDlg = pDoc->m_pScoreDlg;
		pScoreDlg->ShowWindow(SW_SHOWNORMAL);
		pScoreDlg->m_bDialogHide = FALSE;
		
		//恢复全屏按钮的状态
		m_wndToolBar.GetToolBarCtrl().SetState(ID_VIEW_FULLSCREEN, TBSTATE_ENABLED);
	}
}

BOOL CMainFrame::IsFullScreen()
{
	return m_bFullScreen;
}

//WM_COMMAND: ID_VIEW_FULLSCREEN
void CMainFrame::OnViewFullscreen() 
{
	// TODO: Add your command handler code here
	RECT rectDesktop;
    WINDOWPLACEMENT wpNew;

    if (!IsFullScreen())
    {
		//得到视图区的客户区大小，以便后面执行变显示的参数
	    m_wndToolBar.ShowWindow(SW_HIDE);

	    // We'll need these to restore the original state.
	    GetWindowPlacement (&m_wpPrev);

	    m_wpPrev.length = sizeof m_wpPrev;

        //Adjust RECT to new size of window
	    ::GetWindowRect ( ::GetDesktopWindow(), &rectDesktop );
	    ::AdjustWindowRectEx(&rectDesktop, GetStyle(), TRUE, GetExStyle());

	    // Remember this for OnGetMinMaxInfo()
	    m_FullScreenWindowRect = rectDesktop;
        
        wpNew = m_wpPrev;
        wpNew.showCmd =  SW_SHOWNORMAL;
	    wpNew.rcNormalPosition = rectDesktop;
	    
         m_pwndFullScrnBar=new CToolBar;

         if (!m_pwndFullScrnBar->Create(this,CBRS_SIZE_DYNAMIC|CBRS_FLOATING) ||
		    !m_pwndFullScrnBar->LoadToolBar(IDR_FULLSCREEN))
    	    {
	    	    TRACE0("Failed to create toolbar\n");
			    return;      // fail to create
	        }
        
         //don't allow the toolbar to dock
         m_pwndFullScrnBar->EnableDocking(0);
		 m_pwndFullScrnBar->SetWindowPos(0, 100,100, 0,0,SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);   
		 m_pwndFullScrnBar->SetWindowText(_T("全屏工具条"));
	     FloatControlBar(m_pwndFullScrnBar, CPoint(800, 690));
		 m_bFullScreen=TRUE;

		 SetWindowPlacement ( &wpNew );
    }
    else
    {
         EndFullScreen();
     }
}

void CMainFrame::OnUpdateViewFullscreen(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable();

	if (IsFullScreen())
		pCmdUI->SetCheck();
	else
        pCmdUI->SetCheck(0);
}

//WM_GETMINMAXINFO
void CMainFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	//CFrameWnd::OnGetMinMaxInfo(lpMMI);
	if (IsFullScreen())
    {
        lpMMI->ptMaxSize.y = m_FullScreenWindowRect.Height();
	    lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y;
	    lpMMI->ptMaxSize.x = m_FullScreenWindowRect.Width();
	    lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
    }
}

//WM_TIME
void CMainFrame::OnTimer(UINT nIDEvent)
{
	CFrameWnd::OnTimer(nIDEvent);

//05-2-1,by Xy,Begin
	m_pTaskServer->OnTimer(nIDEvent);
//05-2-1,by Xy,End
}

//WM_CLOSE
void CMainFrame::OnClose()
{
	
	CjetManagerDoc *pDoc=(CjetManagerDoc*)GetActiveDocument();
	if(NULL!=pDoc)
	{
		if( (m_pTaskServer->m_bLogout != TRUE) &&
			(m_pTaskServer->m_iTotal > 0))
		{
			::MessageBox(NULL,"请从服务器注销，再退出！","信息",MB_OK|MB_ICONINFORMATION);
//			::Sleep(1000); 
			return;
		}
/*		if(NULL!=pDoc->m_pNetThread)
		{
			//::MessageBox(NULL,"请从服务器注销，再退出！","信息",MB_OK|MB_ICONINFORMATION);
			::Sleep(1000); 

			//Exit the network thread
			//Wait for the network thread to exit
			::Sleep(2000);
			//Set the network thread to NULL
			pDoc->m_pNetThread=NULL;

			pDoc->OnNetworkLogout();
			return;
		}
*/	}
	KillTimer(m_nTimerID);
	CFrameWnd::OnClose();
}

//BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
//}

//void CMainFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CFrameWnd::OnVScroll(nSBCode, nPos, pScrollBar);
//}
