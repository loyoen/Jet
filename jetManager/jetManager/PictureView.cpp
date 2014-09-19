// PictureView.cpp : implementation file
//

#include "stdafx.h"
#include "jetManager.h"
#include "PictureView.h"

#include "jetManagerDoc.h"
#include "MainFrm.h"
#include ".\pictureview.h"
#include "Protocol.h"

#include "SendNotifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureView

IMPLEMENT_DYNCREATE(CPictureView, CScrollView)

CPictureView::CPictureView()
{
	m_PictDisplaySize.cx	= 0;
	m_PictDisplaySize.cy	= 0;

	//笔迹有关变量初始化
	m_bDrawScript			= FALSE;
	m_fDisplayModeX			= 1;
	m_fDisplayModeY			= 1;
	m_ScriptPen.CreatePen(PS_SOLID, 4, RGB(255,0,0));
//, begin
#ifdef	SWITCH_STORE_SCRIPT
	m_ScriptPen1.CreatePen(PS_SOLID, 4, RGB(0,255,0));
	m_ScriptPen2.CreatePen(PS_SOLID, 4, RGB(0,0,255));
	m_ScriptPen3.CreatePen(PS_SOLID, 4, RGB(255,255,0));
#endif
//, ends

	//显示有关参数初始化
	m_fDisplayScale				= 1;
	m_rcPictureOriginal.cx		= 0;
	m_rcPictureOriginal.cy		= 0;

	m_pImageLogo	= NULL;
	m_pImageTask	= NULL;	// 当前显示的图片

	m_bFirstTimeShow	= TRUE	;
}

CPictureView::~CPictureView()
{
	m_ScriptPen.DeleteObject();

	if ( NULL != m_pImageLogo )
		delete	m_pImageLogo;

	if ( NULL != m_pImageTask )
		delete	m_pImageTask;
}


BEGIN_MESSAGE_MAP(CPictureView, CScrollView)
	//{{AFX_MSG_MAP(CPictureView)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_DRAWSCRIPT_OR_NOT, OnDrawscriptOrNot)
	ON_UPDATE_COMMAND_UI(ID_DRAWSCRIPT_OR_NOT, OnUpdateDrawscriptOrNot)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN, OnUpdateViewZoomin)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT, OnUpdateViewZoomout)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureView drawing
void CPictureView::OnDraw(CDC* pDC)
{

	if ( m_bFirstTimeShow )
	{
		((CMainFrame*)GetParent()->GetParent())->ShowScoreDlg();

		m_bFirstTimeShow	= FALSE	;
	}

	if ( NULL == m_pImageTask )
	{
		// show logo
		CRect	rcRectClient;
		GetClientRect( &rcRectClient );
		
		m_pImageLogo->StretchBlt(pDC->GetSafeHdc(),0,0,rcRectClient.right,rcRectClient.bottom,SRCCOPY);

	//int		iOldROP;
	//iOldROP	= pDC->SetROP2( R2_NOTXORPEN );
	//pDC->SetTextColor( RGB( 255, 0, 0 ) ); 

#ifdef	SWITCH_AUTO_TESTING
		//BYTE Data[0x13]	= {0x7F,0x80,0x82,0x8c,0x8C,0x85,0x84,0x90,0x91,0x92,0x93,0x81,0x00};
		BYTE Data[0x3]	= {0x7F,0x80,0x00};
		pDC->TextOut ( 313, 256, (LPSTR)Data );
		pDC->TextOut ( 311, 256, (LPSTR)Data );
		pDC->TextOut ( 308, 256, (LPSTR)Data );
		pDC->TextOut ( 306, 256, (LPSTR)Data );
		pDC->TextOut ( 303, 256, (LPSTR)Data );
		pDC->TextOut ( 300, 256, (LPSTR)Data );
		//pDC->TextOut ( 200, 156, (LPSTR)Data );
		//pDC->TextOut ( 400, 356, (LPSTR)Data );
#endif
	//pDC->SetROP2( iOldROP );

		return;
	}

	CRect	rcRectClient;
	GetClientRect( &rcRectClient );
	CBrush	brush(RGB(255,255,220));
	pDC->FillRect( &rcRectClient,&brush);

	m_pImageTask->Draw( pDC->m_hDC, 0, 0, m_PictDisplaySize.cx, m_PictDisplaySize.cy );

	/*pDC->FillRect( &rcRectClient, &CBrush(RGB(230,220,180)) ); 
	pDC->SetBkColor( RGB(230,220,180) );
	pDC->SetTextColor( RGB( 255, 0, 0 ) ); 
	pDC->TextOut( rcRectClient.CenterPoint().x - 40, rcRectClient.CenterPoint().y - 50,
	"当前job已经打印完" );
	pDC->TextOut( rcRectClient.CenterPoint().x - 55, rcRectClient.CenterPoint().y - 30,
	"请等待服务器分发新job!" );*/

#ifdef	SWITCH_AUTO_TESTING
	BYTE Data[0x3]	= {0x7F,0x80,0x00};
	pDC->TextOut ( 313, 256, (LPSTR)Data );
	pDC->TextOut ( 311, 256, (LPSTR)Data );
	pDC->TextOut ( 308, 256, (LPSTR)Data );
	pDC->TextOut ( 306, 256, (LPSTR)Data );
	pDC->TextOut ( 303, 256, (LPSTR)Data );
	pDC->TextOut ( 300, 256, (LPSTR)Data );
#endif

//, begin
//	int		iOldROP;
//	iOldROP	= pDC->SetROP2( R2_MERGEPENNOT );
//, ends
	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( RGB( 255, 0, 0 ) ); 

//, begin
//	pDC->SetROP2( iOldROP );
//, ends

	//如果有笔迹内容则显示
	if ( m_pDoc->m_bHasScript )
	{
		CTypedPtrList<CObList, CScriptLine*>& ScriptLineList	= m_pDoc->m_ScriptLineList;
		POSITION	pos	= ScriptLineList.GetHeadPosition();
		while ( pos != NULL ) 
		{
			CScriptLine* pScriptLine = ScriptLineList.GetNext(pos);
			pScriptLine->DrawScriptLine( pDC, m_ScriptPen, m_fDisplayModeX, m_fDisplayModeY, m_fDisplayScale );
		}
	}

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	if ( m_pDoc->m_bFlag1 )
	{
		pDC->SetTextColor( RGB( 0, 255, 0 ) ); 

		CTypedPtrList<CObList, CScriptLine*>& ScriptLineList	= m_pDoc->m_lstScript_of_Machine1;
		POSITION	pos	= ScriptLineList.GetHeadPosition();
		while ( pos != NULL ) 
		{
			CScriptLine* pScriptLine = ScriptLineList.GetNext(pos);
			pScriptLine->DrawScriptLine( pDC, m_ScriptPen1, m_fDisplayModeX, m_fDisplayModeY, m_fDisplayScale );
		}
	}

	if ( m_pDoc->m_bFlag2 )
	{
		pDC->SetTextColor( RGB( 0, 0, 255 ) ); 

		CTypedPtrList<CObList, CScriptLine*>& ScriptLineList	= m_pDoc->m_lstScript_of_Machine2;
		POSITION	pos	= ScriptLineList.GetHeadPosition();
		while ( pos != NULL ) 
		{
			CScriptLine* pScriptLine = ScriptLineList.GetNext(pos);
			pScriptLine->DrawScriptLine( pDC, m_ScriptPen2, m_fDisplayModeX, m_fDisplayModeY, m_fDisplayScale );
		}
	}

	if ( m_pDoc->m_bFlag3 )
	{
		pDC->SetTextColor( RGB( 255, 125, 0 ) ); 

		CTypedPtrList<CObList, CScriptLine*>& ScriptLineList	= m_pDoc->m_lstScript_of_Machine3;
		POSITION	pos	= ScriptLineList.GetHeadPosition();
		while ( pos != NULL ) 
		{
			CScriptLine* pScriptLine = ScriptLineList.GetNext(pos);
			pScriptLine->DrawScriptLine( pDC, m_ScriptPen3, m_fDisplayModeX, m_fDisplayModeY, m_fDisplayScale );
		}
	}
#endif
//, ends
}

/////////////////////////////////////////////////////////////////////////////
// CPictureView diagnostics

#ifdef _DEBUG
void CPictureView::AssertValid() const
{
	CView::AssertValid();
}

void CPictureView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPictureView message handlers
#define	MAX_ZOOMFACTOR	2

void CPictureView::OnViewZoomout() 
{
	m_PictDisplaySize.cx /= 2;
	m_PictDisplaySize.cy /= 2;
	m_fDisplayScale		 /= 2;

	SetScrollSizes();

	m_iZoomFactor--;

	RedrawWindow();
}

void CPictureView::OnViewZoomin() 
{
	// TODO: Add your command handler code here
	m_PictDisplaySize.cx *= 2;
	m_PictDisplaySize.cy *= 2;
	m_fDisplayScale		 *= 2;

	SetScrollSizes();

	m_iZoomFactor++;

	RedrawWindow();
}

void CPictureView::OnUpdateViewZoomin(CCmdUI *pCmdUI)
{
	if ( m_iZoomFactor > MAX_ZOOMFACTOR )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( TRUE );
}

void CPictureView::OnUpdateViewZoomout(CCmdUI *pCmdUI)
{
	if ( m_iZoomFactor < -MAX_ZOOMFACTOR )
		pCmdUI->Enable( FALSE );
	else
		pCmdUI->Enable( TRUE );
}

void CPictureView::OnInitialUpdate() 
{
	m_pDoc	= (CjetManagerDoc*)GetDocument();

	m_strAppPath	= CString(((CMainFrame*)GetParentFrame( ))->m_pathApp);

	m_pImageLogo	= new CImage;
	m_pImageLogo->Load( m_strAppPath + "\\logo.jpg" );

	m_pImageTask	= NULL;	// 开始时显示logo

	CRect rcClient;
	GetClientRect(&rcClient);

	CSize	szClient;
	szClient.cx	= rcClient.Width();
	szClient.cy	= rcClient.Height();

	CScrollView::SetScrollSizes(MM_TEXT, szClient);

	// TODO: Add your specialized code here and/or call the base class
	//SetScrollSizes();	

	CScrollView::OnInitialUpdate();
}

void CPictureView::SetScrollSizes()
{
	//得到客户区的大小
	CRect rcClient;
	GetClientRect(&rcClient);
	CSize szPage = rcClient.Size();

	//得到图片显示的大小
	CSize szTotal(m_PictDisplaySize);

	CSize szLine = szPage;
	szLine.cx /= 10;
	szLine.cy /= 10;

	CScrollView::SetScrollSizes(MM_TEXT, szTotal, szPage, szLine);
	Invalidate();
}

void CPictureView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here	
	//AfxMessageBox("OnSize!");
	
	CjetManagerDoc* pDoc = (CjetManagerDoc*)GetDocument();
	CImage* pPic = pDoc->GetPicture();
//	ASSERT(pPic);

	if (pPic) 
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		/*
		m_fDisplayModeX = (float)rcClient.right / (float)m_rcClientOriginal.right;
		m_fDisplayModeY = (float)rcClient.bottom / (float)m_rcClientOriginal.bottom;

		m_PictDisplaySize.cx = m_rcClientOriginal.right * m_fDisplayModeX * m_fDisplayScale;
		m_PictDisplaySize.cy = m_rcClientOriginal.bottom * m_fDisplayModeY * m_fDisplayScale;
		*/
		m_fDisplayModeX = (float)rcClient.Width() / (float)m_rcPictureOriginal.cx;
		m_fDisplayModeY = m_fDisplayModeX;

		m_PictDisplaySize.cx = m_rcPictureOriginal.cx * m_fDisplayModeX * m_fDisplayScale;
		m_PictDisplaySize.cy = m_rcPictureOriginal.cy * m_fDisplayModeY * m_fDisplayScale;
		
		OnInitialUpdate();
	}
	/*
	if(pDoc->m_bShowLogo && (NULL!=pDoc->m_pPictureView))
	{
        pDoc->DisplayLogo(); 
	} 
	*/
}

void CPictureView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == VK_ESCAPE) 
	{
		CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;
		pFrame->EndFullScreen();
	}

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPictureView::OnDrawscriptOrNot() 
{
	// TODO: Add your command handler code here
	m_bDrawScript	= !m_bDrawScript;
}

void CPictureView::OnUpdateDrawscriptOrNot(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bDrawScript);
}

void CPictureView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CScrollView::OnLButtonDown(nFlags, point);
	if ( m_bDrawScript == TRUE ) 
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.DPtoLP(&point);

//, begin
		m_pDoc->m_bScriptChanged = TRUE;		// 标识笔迹已修执行
//, ends
		m_pDoc->m_bHasScript	= TRUE;			// 标识有笔迹产生
		m_pScriptLineCur = m_pDoc->NewScriptLine();
		m_ptPrev = point;

		//笔迹点的坐标必须与job显示模式相关联
		point.x /= (m_fDisplayModeX * m_fDisplayScale);
		point.y /= (m_fDisplayModeY * m_fDisplayScale);
		m_pScriptLineCur->m_pointArray.Add(point);
	
		SetCapture();
	}
	
	return;
}

void CPictureView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CScrollView::OnLButtonUp(nFlags, point);
	if (m_bDrawScript == TRUE) 
	{
		if (GetCapture() != this)
			return;
	
		CClientDC dc(this);
		OnPrepareDC(&dc);  // set up mapping mode and viewport origin
		dc.DPtoLP(&point);

		CPen* pOldPen = dc.SelectObject(&m_ScriptPen);
		dc.MoveTo(m_ptPrev);
		dc.LineTo(point);
		dc.SelectObject(pOldPen);


		//笔迹点的坐标必须与job显示模式相关联
		point.x /= (m_fDisplayModeX * m_fDisplayScale);
		point.y /= (m_fDisplayModeY * m_fDisplayScale);
		m_pScriptLineCur->m_pointArray.Add(point);
		
		ReleaseCapture();

		// 添加 comment
		CSendNotifyDlg	dlg;
		if ( IDOK == dlg.DoModal() )
		{
			m_pScriptLineCur->m_strComment	= dlg.m_strNotifyMsg;
		}
	}

	return;
}

void CPictureView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CScrollView::OnMouseMove(nFlags, point);

	//判断是否在制定区域恢复job打印界面
	RECT	rcPictureView;

	GetClientRect(&rcPictureView);

	if (point.x < rcPictureView.right && point.x > (rcPictureView.right - 8) && point.y < rcPictureView.bottom - 200 && point.y > rcPictureView.top + 200)
	{
		CDlgManager* pScoreDlg = m_pDoc->m_pScoreDlg;
		/*if (pScoreDlg->IsDialogHide()) 
		{
			pScoreDlg->ResumeDialog();
			
			//恢复全屏工具栏
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
			CToolBarCtrl& ToolBarCtrl = pMainFrame->GetToolBar()->GetToolBarCtrl();
			ToolBarCtrl.SetState(ID_VIEW_FULLSCREEN, TBSTATE_ENABLED);
			ToolBarCtrl.Invalidate();

			OnInitialUpdate();
		}*/
	}

	//添加笔迹信息
	if (m_bDrawScript == TRUE) 
	{
		if (GetCapture() != this)
			return;
	
		CClientDC dc(this);
		OnPrepareDC(&dc);
		dc.DPtoLP(&point);

		CPen* pOldPen = dc.SelectObject(&m_ScriptPen);
		dc.MoveTo(m_ptPrev);
		dc.LineTo(point);
		dc.SelectObject(pOldPen);
		m_ptPrev = point;
		
		//笔迹点的坐标必须与job显示模式相关联
		point.x /= (m_fDisplayModeX * m_fDisplayScale);
		point.y /= (m_fDisplayModeY * m_fDisplayScale);
		m_pScriptLineCur->m_pointArray.Add(point);
	}

	return;
}

//Display logo bitmap
void CPictureView::ShowLogo(CDC *pDC,HANDLE hBitmap)
{
     CDC      memDC;
	 CBitmap  bmp;
	 bmp.Attach(hBitmap);
	 memDC.CreateCompatibleDC(pDC);
	 CBitmap *pOldBmp=memDC.SelectObject(&bmp);

	 //Get bitmap size
	 BITMAP bm;
	 bmp.GetObject(sizeof(BITMAP),&bm);

	 //Get the Drawing rect
	 CRect rc;
	 GetClientRect(&rc);

	 //Draw the Bitmap
	 pDC->StretchBlt(rc.left ,rc.top ,rc.Width(),rc.Height(),&memDC,0,0,bm.bmWidth,
		             bm.bmHeight,SRCCOPY);

}

BOOL CPictureView::OnEraseBkgnd(CDC* pDC)
{
/*
    CjetManagerDoc* pDoc = (CjetManagerDoc*)GetDocument();
	
	if(pDoc->m_bShowLogo) 
	        ShowLogo(pDC,pDoc->m_hLogoBitmap);
	return TRUE;
*/
	return	TRUE;
	//return CScrollView::OnEraseBkgnd(pDC);
}



void CPictureView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int	iMin, iMax;

	int	i = CScrollView::GetScrollPos( 1 );

	GetScrollRange( SB_VERT, &iMin, &iMax );
	if ( iMax == 0 )
		return CScrollView::OnLButtonDblClk(nFlags, point);

	if ( i < 300 )
	{
		ScrollWindow( -1, -600 );
		CScrollView::SetScrollPos( 1, 600 );
	}
	else
	{
		ScrollWindow( -1, 600 );
		CScrollView::SetScrollPos( 1, -600 );
	}

	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CPictureView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
//, begin
//	int		iFormatSheetID;
//	iFormatSheetID	= m_pDoc->m_iJobID;
//, ends

	if ( NULL != m_pImageTask )
	{
		m_pImageTask->Destroy();
		delete	m_pImageTask;
		m_pImageTask	= NULL;
	}

//, begin
//	if ( -1 == iFormatSheetID )
	if ( NULL == m_pDoc->m_pTaskCurrent )
//, ends
	{
		m_pImageTask	= NULL;

		RedrawWindow( );
		return;
	}
	else
	{
		CString	strID;
//, begin
//		strID.Format( "\\%d.gif", iFormatSheetID );

		strID.Format( "\\%d.gif", m_pDoc->m_pTaskCurrent->iFormatSheetID );
//, ends

		m_pImageTask	= new CImage;
		if ( S_OK != m_pImageTask->Load( m_strUserPath + strID ) )
		{
			CString	strError;
			strError.Format( "%s", "打印系统检测到可能的异常:\n" );
			strError	+= _T("该计算机硬件不稳定\n\n");
			strError	+= _T("请立即和技术机器员联系\n警告代码：");
			// error
			while( TRUE )
			{
				MessageBox( strError + strID, "安全性警告", MB_OK );
			}
		}
	}

	//得到客户区的大小
	CRect rcClient;
	GetClientRect( &rcClient );
	CSize szPage	= rcClient.Size();

	m_fDisplayScale			= 1;
	m_rcPictureOriginal.cx	= m_pImageTask->GetWidth();
	m_rcPictureOriginal.cy	= m_pImageTask->GetHeight();

	m_fDisplayModeX			= (float)rcClient.Width() / (float)m_rcPictureOriginal.cx;
	m_fDisplayModeY			= m_fDisplayModeX;
	m_PictDisplaySize.cx	= rcClient.Width();
	m_PictDisplaySize.cy	= m_rcPictureOriginal.cy * m_fDisplayModeY;

	m_iZoomFactor	= 0;

	SetScrollSizes();

	int		nPos, nMax;
	GetScrollRange ( SB_HORZ, &nPos, &nMax );
	SetScrollPos ( SB_HORZ, nPos );
	GetScrollRange ( SB_VERT, &nPos, &nMax );
	SetScrollPos ( SB_VERT, nPos );

	RedrawWindow( );
}

BOOL CPictureView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int	iMin, iMax;

	int	i = this->CScrollView::GetScrollPos( SB_VERT );

	//GetScrollRange( SB_VERT, &iMin, &iMax );
	//if ( iMax == 0 )
	//	return CScrollView::OnLButtonDblClk(nFlags, point);

	//if ( i < 300 )
	//{
	//	ScrollWindow( -1, -600 );
	//	CScrollView::SetScrollPos( 1, 600 );
	//}
	//else
	//{
	//	ScrollWindow( -1, 600 );
	//	CScrollView::SetScrollPos( 1, -600 );
	//}

	//if ( i - zDelta < iMin )
	//	CScrollView::SetScrollPos ( SB_VERT, iMin )	;
	//else if ( i - zDelta > iMax )
	//	CScrollView::SetScrollPos ( SB_VERT, iMax )	;
	//else
	//	CScrollView::SetScrollPos ( SB_VERT, i + zDelta )	;

	//CScrollView::ScrollWindow ( 0, -zDelta ) ;

	this->ScrollToPosition ( CPoint(0,i-zDelta) );

//	CClientDC	dc(this);
	//CRect		rectClient;
	//GetClientRect( rectClient );

	//InvalidateRect ( rectClient );

	//RedrawWindow ( )	;

	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}
