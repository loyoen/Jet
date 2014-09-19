

#include "stdafx.h"
#include "jetManager.h"
#include "jetManagerDoc.h"

#include "PwdModifyDlg.h"
#include "SendNotifyDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CjetManagerDoc
IMPLEMENT_DYNCREATE(CjetManagerDoc, CDocument)

BEGIN_MESSAGE_MAP(CjetManagerDoc, CDocument)
	ON_COMMAND(ID_COMMAND_LOGOUT, OnLogout)
	ON_COMMAND(ID_COMMAND_MODIFYPWD, OnPasswordModify)
    ON_COMMAND(ID_COMMAND_NOTIFY,OnSendNotifyMsg)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_LOGOUT, OnUpdateCommandLogout)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_MODIFYPWD, OnUpdateCommandModifypwd)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_NOTIFY, OnUpdateCommandNotify)
	ON_UPDATE_COMMAND_UI(ID_COMMAND_SUBMITJOB, OnUpdateCommandSubmittask)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
END_MESSAGE_MAP()

//==================================================//
//													//
//					笔 迹 管 理						//
//													//
//==================================================//

void CjetManagerDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable ( m_ScriptLineList.GetCount() );
}

void CjetManagerDoc::OnEditUndo()
{
	CScriptLine *	lpScriptLine;

	lpScriptLine	= m_ScriptLineList.RemoveTail();

	if ( NULL != lpScriptLine )
	{
		delete	lpScriptLine;

		m_bScriptChanged	= TRUE;

		if ( m_ScriptLineList.GetCount() )
		{
			m_bHasScript	= TRUE;
		}
		else
		{
			m_bHasScript	= FALSE;
		}

		UpdateAllViews ( NULL );
	}
}

void CjetManagerDoc::DeleteScripts( )
{
	CScriptLine*	lpScriptLine;

	int	iCnt	= m_ScriptLineList.GetCount();
	for ( int i=0; i<iCnt; i++ )
	{
		lpScriptLine	= m_ScriptLineList.RemoveHead();
		delete	lpScriptLine;
	}
}

BOOL CjetManagerDoc::Delete_script_file ( LPCTSTR lpszScriptPathName )
{
	return	(S_OK == DeleteFile ( lpszScriptPathName ) );
}

//笔迹文件存盘
BOOL CjetManagerDoc::SaveScriptFile(LPCTSTR lpszScriptPathName)
{
	CFileException fe;
	CFile* pFile;
	pFile = GetFile(lpszScriptPathName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe);
	if (pFile == NULL)
	{
		AfxMessageBox("存盘文件未找到，请确认该文件确实存在！");
		return FALSE;
	}

	CArchive saveArchive(pFile, CArchive::store | CArchive::bNoFlushOnDelete);
	saveArchive.m_pDocument = this;
	saveArchive.m_bForceFlat = FALSE;

	TRY
	{
		CWaitCursor wait;
		m_ScriptLineList.Serialize(saveArchive);     // save me
		saveArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		
		AfxMessageBox("存笔迹文件时发生异常!");
		
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

//打开笔迹文件
BOOL CjetManagerDoc::OpenScriptFile(LPCTSTR lpszScriptPathName)
{
	CFileException fe;
	CFile* pFile;
	pFile = GetFile(lpszScriptPathName, CFile::modeRead | CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		AfxMessageBox("需要打开的笔迹文件文件未找到，请确认该文件确实存在！");
		return FALSE;
	}

	DeleteContents();

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			m_ScriptLineList.Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents
		AfxMessageBox("打开笔迹文件时发生异常！");
		
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

//==================================================//
//													//
//					调 用 接 口						//
//													//
//==================================================//
void CjetManagerDoc::SetUserPath( CString strPath )
{
	m_strUserPath	= strPath; 

	m_pPictureView->SetUserPath( strPath );
} 

//, begin
//void CjetManagerDoc::SetCurrentTaskID( int iFormatSheetID )
//{
//	if ( iFormatSheetID == m_iJobID )
//		return;
//
//	if (iFormatSheetID == -1)
//	{
//		m_iJobID	= -1;
//		UpdateAllViews( NULL, 1 );
//		return;
//	}
//
//	CString	strScript;
//	// 若当前有笔迹则保存并销毁之
//	if ( m_bHasScript )
//	{
//		strScript.Format( "\\%d.cpt", m_iJobID );
//
//		m_csScriptPath	= m_strUserPath + strScript;
//
//		SaveScriptFile( (LPCTSTR)m_csScriptPath );
//
//		//删除笔迹链表
//		DeleteScripts( );
//	}
//	else if ( m_bScriptChanged )
//	{
//		// 笔迹被撤消完, 因此要删除笔迹文件
//		strScript.Format( "\\%d.cpt", m_iJobID );
//
//		m_csScriptPath	= m_strUserPath + strScript;
//
//		Delete_script_file ( m_csScriptPath );
//	}
//
//	m_iJobID	= iFormatSheetID;
//	m_bHasScript	= FALSE;
//
//	// 如果有笔记文件则加载之
//	strScript.Format( "\\%d.cpt", m_iJobID );
//	m_csScriptPath	= m_strUserPath + strScript;
//	CFile	fileScript;
//	if ( fileScript.Open(m_csScriptPath, CFile::modeRead | CFile::shareDenyWrite) != NULL ) 
//	{
//		OpenScriptFile((LPCTSTR)m_csScriptPath);
//		fileScript.Close();
//
//		m_bHasScript	= TRUE;
//	}
//
//	UpdateAllViews( NULL, 1 );
//}

void CjetManagerDoc::SetCurrentTask ( LPFORMAT_SHEET pTask )
{
	int		i, iCnt;

	if ( m_pTaskCurrent == pTask )
		return;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	m_bFlag1	= FALSE;
	m_bFlag2	= FALSE;
	m_bFlag3	= FALSE;

	// 删除他机器的笔迹链表
	CScriptLine*	lpScriptLine;

	iCnt	= m_lstScript_of_Machine1.GetCount();
	for ( i=0; i<iCnt; i++ )
	{
		lpScriptLine	= m_lstScript_of_Machine1.RemoveHead();
		delete	lpScriptLine;
	}

	iCnt	= m_lstScript_of_Machine2.GetCount();
	for ( i=0; i<iCnt; i++ )
	{
		lpScriptLine	= m_lstScript_of_Machine2.RemoveHead();
		delete	lpScriptLine;
	}

	iCnt	= m_lstScript_of_Machine3.GetCount();
	for ( i=0; i<iCnt; i++ )
	{
		lpScriptLine	= m_lstScript_of_Machine3.RemoveHead();
		delete	lpScriptLine;
	}

	if ( NULL != pTask )
	{
		if ( -1 != pTask->iMachine3ID )
		{
			Set_script_list ( pTask->iMachine3ID, pTask->iFormatSheetID, 3 );
		}
		if ( -1 != pTask->iMachine2ID )
		{
			Set_script_list ( pTask->iMachine2ID, pTask->iFormatSheetID, 2 );
		}
		if ( -1 != pTask->iMachine1ID )
		{
			Set_script_list ( pTask->iMachine1ID, pTask->iFormatSheetID, 1 );
		}
	}
#endif
//, ends

	// 若笔迹有执行动
	if ( m_bScriptChanged )
	{
		// 变成现在又没有笔记
		if ( !m_bHasScript )
		{
			// 若曾经有笔迹则要删除笔迹文件
			if ( m_pTaskCurrent->bHasScript )
			{
				CString	strScript;

				strScript.Format( "\\%d.cpt", m_pTaskCurrent->iFormatSheetID );

				m_csScriptPath	= m_strUserPath + strScript;

				Delete_script_file ( m_csScriptPath );

				m_pTaskCurrent->bHasScript	= FALSE;
			}
		}
		// 现在有笔迹则要保存该笔迹
		else
		{
			CString	strScript;

//			strScript.Format( "\\%d.cpt", m_iJobID );

			strScript.Format( "\\%d.cpt", m_pTaskCurrent->iFormatSheetID );

			m_csScriptPath	= m_strUserPath + strScript;

			SaveScriptFile( (LPCTSTR)m_csScriptPath );

			m_pTaskCurrent->bHasScript	= TRUE;
		}
	}

	if ( m_bHasScript )
	{
		//删除笔迹链表
		DeleteScripts( );
	}

	if ( NULL == pTask )
	{
		m_pTaskCurrent	= pTask;
//		m_iJobID	= -1;

		m_bScriptChanged= FALSE;
		m_bHasScript	= FALSE;

		UpdateAllViews ( NULL, 1 );
		return;
	}

	m_bHasScript	= FALSE;

	// 如果有笔记文件则加载之
	if ( pTask->bHasScript )
	{
		CString	strScript;
		strScript.Format( "\\%d.cpt", pTask->iFormatSheetID );
		m_csScriptPath	= m_strUserPath + strScript;
		CFile	fileScript;
		if ( fileScript.Open(m_csScriptPath, CFile::modeRead | CFile::shareDenyWrite) != NULL ) 
		{
			OpenScriptFile((LPCTSTR)m_csScriptPath);
			fileScript.Close();

			m_bHasScript	= TRUE;
		}
	}

	m_bScriptChanged= FALSE;
	m_pTaskCurrent	= pTask;
//	m_iJobID	= pTask->iFormatSheetID;

	UpdateAllViews( NULL, 1 );
}
//, ends

//=================================================================
//
//        Initialization and Deinitialization
//
//=================================================================
CjetManagerDoc::CjetManagerDoc()
{
	m_bHasScript		= FALSE;
	m_bChangePict		= FALSE;
	m_bHasPict			= FALSE;
	m_bScriptChanged	= FALSE;
	m_bDestroyAndExit	= FALSE;

	m_pPictureView	= NULL;
	m_pScoreDlg		= NULL;

	m_nPrevTaskCnt	= 0;
//	m_iJobID	= -1;

//, begin
	m_pTaskCurrent	= NULL;
//, ends

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	m_bFlag1	= FALSE;
	m_bFlag2	= FALSE;
	m_bFlag3	= FALSE;
#endif
//, ends
}

CjetManagerDoc::~CjetManagerDoc()
{
	//Delete the Job Item
	POSITION pos;
	for(pos=m_lstJob.GetHeadPosition();pos!=NULL;)
	{
       JOB_INFO *pItem=(JOB_INFO *)m_lstJob.GetNext(pos);
	   delete pItem;
	}
	m_lstJob.RemoveAll();
}

BOOL CjetManagerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_pPictureView	= (CPictureView*)GetView(RUNTIME_CLASS(CPictureView));
	m_pScoreDlg		= (CDlgManager*)   GetView(RUNTIME_CLASS(CDlgManager));

	return TRUE;
}

CView* CjetManagerDoc::GetView(CRuntimeClass *pClass)
{
	CView* pView;
    POSITION pos=GetFirstViewPosition();

    while(pos!=NULL)
    {
        pView=GetNextView(pos);
        if(pView->IsKindOf(pClass))
             break;
    }

    if(!pView->IsKindOf(pClass))
        return  NULL;
        
	return pView;
}

//===============================================================
//
//                          CjetManagerDoc 诊断
//
//===============================================================
#ifdef _DEBUG
void CjetManagerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CjetManagerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


//=======================================================================
//
//               Script Operation Implementation
//
//=======================================================================
CScriptLine* CjetManagerDoc::NewScriptLine()
{
	CScriptLine* pLineItem = new CScriptLine();
	m_ScriptLineList.AddTail(pLineItem);
	return pLineItem;
}

void CjetManagerDoc::DiaplayTaskPict(CString PictPath)
{
	if ( 0 == PictPath.GetLength() )
		m_bHasPict	= FALSE;
	else
		m_bHasPict	= TRUE;

	//询问是否存储上一帧的笔迹文件
	if (m_csOldPictPath.IsEmpty() != TRUE) 
	{
		if (m_bScriptChanged == TRUE) 
		{
			int iAnswer = AfxMessageBox("您确定要存储笔迹文件吗？", MB_YESNO);
			if (iAnswer == IDYES) 
			{
				m_csScriptPath = m_csOldPictPath;
				m_csScriptPath += "Script";
				SaveScriptFile((LPCTSTR)m_csScriptPath);
			}
		}
	}
	m_bScriptChanged = FALSE;
	m_csOldPictPath = PictPath;
	//删除笔迹链表
	DeleteContents();

	m_Pict.Destroy();

	if ( !m_bHasPict )
	{
		m_bChangePict = TRUE;
		//调用CPictureView的函数显示
		//m_pPictureView->OnInitialUpdate();
		UpdateAllViews(NULL);
		return;
	}
	//加载笔迹和图像文件供显示,判断该job是否有笔迹文件
	m_Pict.Load(PictPath);
	m_csScriptPath = PictPath;
	m_csScriptPath += "Script";

	CFile fileScript;
	if (fileScript.Open(m_csScriptPath, CFile::modeRead | CFile::shareDenyWrite) != NULL) 
	{
		OpenScriptFile((LPCTSTR)m_csScriptPath);
		fileScript.Close();
	}
	
	m_bChangePict = TRUE;

	//调用CPictureView的函数显示
//	m_pPictureView->OnInitialUpdate();
	UpdateAllViews(NULL);
}

//==========================================================
//
//               Picture的操作
//
//==========================================================
void CjetManagerDoc::DeleteAllPictures(CString szPath)
{
	CFileFind ff;
	CString path = szPath;
	
	if(path.Right(1) != "\\")
		path += "\\";
  
	path += "*.*";

	BOOL res = ff.FindFile(path);

	while(res)
	{
	
		res = ff.FindNextFile();
		//是文件时直接删除
	
		if (!ff.IsDots() && !ff.IsDirectory())
			DeleteFile(ff.GetFilePath());
		else if (ff.IsDots())
			continue;
		else if (ff.IsDirectory())
		{
			path = ff.GetFilePath();
			//是目录时继续递归，删除该目录下的文件
			DeleteAllPictures(path);
			//目录为空后删除目录
			RemoveDirectory(path);
		}
	}
	//最终目录被清空了，于是删除该目录
	ff.Close();
//	RemoveDirectory(szPath);
}


//==================================================================
//
//              消息处理函数的实现
//
//==================================================================
void CjetManagerDoc::OnLogout()
{
	m_pTaskServer->m_bLogout = TRUE;
	m_pTaskServer->Logout( );
}

void CjetManagerDoc::OnPasswordModify()
{
//	//5-02-01,begin
//	CPwdModifyDlg dlg;
//	if (IDOK == dlg.DoModal())
//	{
//		CString OldPwd, NewPwd, Account;
//		OldPwd  = dlg.m_strOldPwd;
//		NewPwd  = dlg.m_strNewPwd;
//		Account = dlg.m_strAccount;
//		m_pTaskServer->Client_modify_userinfo(OldPwd, NewPwd, Account);
//	}
//	//5-02-01,end
}

void CjetManagerDoc::OnSendNotifyMsg()
{
	CSendNotifyDlg dlg;
	if(IDOK==dlg.DoModal())
	{
          //Sent the Message to Network service     
		// m_pNetThread->NetworkService_SendServerNotification( m_pScoreDlg->m_strUserName,dlg.m_strNotifyMsg );
	}
}

//====================================================================
//
//                  执行信息和操作
//
//====================================================================
void   CjetManagerDoc::InsertNewJob(int nJobID,float fMaxScore)
{
}

void CjetManagerDoc::SetTaskServer(CThreadTaskServer* pTaskServer)
{
	m_pTaskServer	= pTaskServer;
}

void CjetManagerDoc::OnUpdateCommandLogout(CCmdUI *pCmdUI)
{
	m_pScoreDlg->OnUpdateCommandLogout( pCmdUI );
}

void CjetManagerDoc::OnUpdateCommandModifypwd(CCmdUI *pCmdUI)
{
	m_pScoreDlg->OnUpdateCommandModifypwd( pCmdUI );
}

void CjetManagerDoc::OnUpdateCommandNotify(CCmdUI *pCmdUI)
{
	m_pScoreDlg->OnUpdateCommandNotify( pCmdUI );
}

void CjetManagerDoc::OnUpdateCommandSubmittask(CCmdUI *pCmdUI)
{
	m_pScoreDlg->OnUpdateCommandSubmittask( pCmdUI );
}

void CjetManagerDoc::OnUpdateUnchecktask(CCmdUI *pCmdUI)
{
	m_pScoreDlg->OnUpdateBadJob( pCmdUI );
}

//, begin
#ifdef	SWITCH_STORE_SCRIPT
void CjetManagerDoc::Set_script_list ( int iMachineID, int iFormatSheetID, int iNumber )
{
	CTypedPtrList<CObList, CScriptLine*>	*lplstScript;

	switch ( iNumber )
	{
	case	1:
		lplstScript	= &m_lstScript_of_Machine1;
		m_bFlag1	= TRUE;
		break;

	case	2:
		lplstScript	= &m_lstScript_of_Machine2;
		m_bFlag2	= TRUE;
		break;

	case	3:
		lplstScript	= &m_lstScript_of_Machine3;
		m_bFlag3	= TRUE;
		break;

	default:
		return;
	}

	CString	strScript;
	CString	strPath;

	strScript.Format( "\\%d_%d.cpt", iFormatSheetID, iMachineID );
	strPath	= m_strUserPath + strScript;
	
	CFile	fileScript;
	if ( fileScript.Open(strPath, CFile::modeRead | CFile::shareDenyWrite) != NULL ) 
	{
		Open_script_file( (LPCTSTR)strPath, lplstScript );
		fileScript.Close( );
	}

//	UpdateAllViews( NULL, 1 );
}

BOOL CjetManagerDoc::Open_script_file (LPCTSTR lpszScriptPathName, CTypedPtrList<CObList, CScriptLine*> *lplstScript )
{
	CFileException fe;
	CFile* pFile;
	pFile = GetFile(lpszScriptPathName, CFile::modeRead | CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		//AfxMessageBox("需要打开的笔迹文件文件未找到，请确认该文件确实存在！");
		return FALSE;
	}

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			lplstScript->Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		//DeleteContents();   // remove failed contents
		//AfxMessageBox("打开笔迹文件时发生异常！");
		
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}
#endif
//, ends
