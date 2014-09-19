

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
//					�� �� �� ��						//
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

//�ʼ��ļ�����
BOOL CjetManagerDoc::SaveScriptFile(LPCTSTR lpszScriptPathName)
{
	CFileException fe;
	CFile* pFile;
	pFile = GetFile(lpszScriptPathName, CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive, &fe);
	if (pFile == NULL)
	{
		AfxMessageBox("�����ļ�δ�ҵ�����ȷ�ϸ��ļ�ȷʵ���ڣ�");
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
		
		AfxMessageBox("��ʼ��ļ�ʱ�����쳣!");
		
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

//�򿪱ʼ��ļ�
BOOL CjetManagerDoc::OpenScriptFile(LPCTSTR lpszScriptPathName)
{
	CFileException fe;
	CFile* pFile;
	pFile = GetFile(lpszScriptPathName, CFile::modeRead | CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		AfxMessageBox("��Ҫ�򿪵ıʼ��ļ��ļ�δ�ҵ�����ȷ�ϸ��ļ�ȷʵ���ڣ�");
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
		AfxMessageBox("�򿪱ʼ��ļ�ʱ�����쳣��");
		
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}

//==================================================//
//													//
//					�� �� �� ��						//
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
//	// ����ǰ�бʼ��򱣴沢����֮
//	if ( m_bHasScript )
//	{
//		strScript.Format( "\\%d.cpt", m_iJobID );
//
//		m_csScriptPath	= m_strUserPath + strScript;
//
//		SaveScriptFile( (LPCTSTR)m_csScriptPath );
//
//		//ɾ���ʼ�����
//		DeleteScripts( );
//	}
//	else if ( m_bScriptChanged )
//	{
//		// �ʼ���������, ���Ҫɾ���ʼ��ļ�
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
//	// ����бʼ��ļ������֮
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

	// ɾ���������ıʼ�����
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

	// ���ʼ���ִ�ж�
	if ( m_bScriptChanged )
	{
		// ���������û�бʼ�
		if ( !m_bHasScript )
		{
			// �������бʼ���Ҫɾ���ʼ��ļ�
			if ( m_pTaskCurrent->bHasScript )
			{
				CString	strScript;

				strScript.Format( "\\%d.cpt", m_pTaskCurrent->iFormatSheetID );

				m_csScriptPath	= m_strUserPath + strScript;

				Delete_script_file ( m_csScriptPath );

				m_pTaskCurrent->bHasScript	= FALSE;
			}
		}
		// �����бʼ���Ҫ����ñʼ�
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
		//ɾ���ʼ�����
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

	// ����бʼ��ļ������֮
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
//                          CjetManagerDoc ���
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

	//ѯ���Ƿ�洢��һ֡�ıʼ��ļ�
	if (m_csOldPictPath.IsEmpty() != TRUE) 
	{
		if (m_bScriptChanged == TRUE) 
		{
			int iAnswer = AfxMessageBox("��ȷ��Ҫ�洢�ʼ��ļ���", MB_YESNO);
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
	//ɾ���ʼ�����
	DeleteContents();

	m_Pict.Destroy();

	if ( !m_bHasPict )
	{
		m_bChangePict = TRUE;
		//����CPictureView�ĺ�����ʾ
		//m_pPictureView->OnInitialUpdate();
		UpdateAllViews(NULL);
		return;
	}
	//���رʼ���ͼ���ļ�����ʾ,�жϸ�job�Ƿ��бʼ��ļ�
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

	//����CPictureView�ĺ�����ʾ
//	m_pPictureView->OnInitialUpdate();
	UpdateAllViews(NULL);
}

//==========================================================
//
//               Picture�Ĳ���
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
		//���ļ�ʱֱ��ɾ��
	
		if (!ff.IsDots() && !ff.IsDirectory())
			DeleteFile(ff.GetFilePath());
		else if (ff.IsDots())
			continue;
		else if (ff.IsDirectory())
		{
			path = ff.GetFilePath();
			//��Ŀ¼ʱ�����ݹ飬ɾ����Ŀ¼�µ��ļ�
			DeleteAllPictures(path);
			//Ŀ¼Ϊ�պ�ɾ��Ŀ¼
			RemoveDirectory(path);
		}
	}
	//����Ŀ¼������ˣ�����ɾ����Ŀ¼
	ff.Close();
//	RemoveDirectory(szPath);
}


//==================================================================
//
//              ��Ϣ��������ʵ��
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
//                  ִ����Ϣ�Ͳ���
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
		//AfxMessageBox("��Ҫ�򿪵ıʼ��ļ��ļ�δ�ҵ�����ȷ�ϸ��ļ�ȷʵ���ڣ�");
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
		//AfxMessageBox("�򿪱ʼ��ļ�ʱ�����쳣��");
		
		return FALSE;
	}
	END_CATCH_ALL

	return TRUE;
}
#endif
//, ends
