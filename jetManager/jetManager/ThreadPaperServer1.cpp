// ThreadPaperServer.cpp : 实现文件
//

#include "stdafx.h"
#include "eMarker.h"
#include "ThreadPaperServer.h"
#include ".\threadpaperserver.h"
#include "MainFrm.h"
#include "ScoreDlg.h"
#include <direct.h>

// CThreadPaperServer

#define	WM_EXIT_THREAD		WM_USER + 300
#define ROLLBACK_COUNT			10
#define MAX_SUBMIT_COUNT		20
#define	MAX_MARKED_COUNT		30

IMPLEMENT_DYNCREATE(CThreadPaperServer, CWinThread)

//==================================================//
//													//
//					调 用 接 口						//
//													//
//==================================================//
BOOL CThreadPaperServer::StartPaperServer( )
{
	m_bPaperServerStarted	= TRUE;

	m_iCurrent		= 0;
	m_iMarking		= 0;
	m_iTotal		= 0;

	m_iVersion		= 0;

	m_lstPaperHead	= NULL;
	m_lstPaperTail	= NULL;
	m_pPaperCurrent	= NULL;

	return	TRUE;
}

BOOL CThreadPaperServer::StopPaperServer( )
{
	m_bPaperServerStarted	= FALSE;

	::PostThreadMessage( m_nThreadID, WM_EXIT_THREAD, (WPARAM)0, (LPARAM)0 ); 

    return	TRUE;
}

void CThreadPaperServer::SetScoreDlg( CScoreDlg* pDlg )
{
	::PostThreadMessage( m_nThreadID, WM_SCOREDLG_HWND, (WPARAM)pDlg, (LPARAM)0 ); 
}

void CThreadPaperServer::SetLogonInfo( CString strAccount, CString strPassword )
{
	m_strAccount	= strAccount;
	m_strPassword	= strPassword;
}

void CThreadPaperServer::SetBufferPtrList( CPtrList* pListLarge, CPtrList* pListSmall, CPtrList* pListCommand )
{
	m_lpPtrLargeMemList		= pListLarge;
	m_lpPtrSmallMemList		= pListSmall;
	m_lpPtrCommandMemList	= pListCommand;
}

BOOL CThreadPaperServer::ConnectServer( CString strAddr, u_short sinport )
{
	if ( m_bPaperServerStarted )
		return	TRUE;

	m_pNet->SetMainServer( strAddr, sinport );

	if ( m_pNet->Connect( ) )
	{
		StartPaperServer( );
		return	TRUE;
	}


	return	FALSE;
}

void CThreadPaperServer::Logon( )
{
	LPPACKAGE		lpPackage;
	LPPACK_LOGIN	lpPackLogin;

	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead( );

	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_LOGIN);
	lpPackage->packageHeader.yTypePackage	= 0;

	lpPackLogin	= (LPPACK_LOGIN)(&(lpPackage->body));

	lpPackLogin->yUserType	= MSG_CLIENT_LOGIN;
	lstrcpy(lpPackLogin->strAccount, m_strAccount);
	lstrcpy(lpPackLogin->strPassword, m_strPassword);
	lpPackLogin->userip;

	m_pNet->SendToServer( lpPackage );

	//m_lpPtrCommandMemList->AddTail( (LPVOID)lpPackage );
}

void CThreadPaperServer::Logout( )
{
	// need to submit all papers to server first!
	this->SubmitAllPaper2Server();
	LPPACKAGE		lpPackage		;
	LPPACK_LOGOUT	lpPackLogout	;

	lpPackage		= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_LOGOUT);
	lpPackage->packageHeader.yTypePackage	= 0;

	lpPackLogout	= (LPPACK_LOGOUT)(&(lpPackage->body));

	lpPackLogout->yUserType	= MSG_CLIENT_LOGOUT	;
	lpPackLogout->iTeacherID= m_iTeacherID		;

	m_pNet->SendToServer( lpPackage);
}

void CThreadPaperServer::ReLogin( )
{
	LPPACKAGE		lpPackage		;
	LPPACK_RELOGIN	lpPackRelogin	;

	lpPackage		= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();
	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_RELOGIN);
	lpPackage->packageHeader.yTypePackage	= 0;
	
	lpPackRelogin	= (LPPACK_RELOGIN)(&(lpPackage->body));

	if(lpPackRelogin != NULL)
	{
		lpPackRelogin->yUserType = MSG_CLIENT_RELOGIN	;
		lpPackRelogin->iTeacherID= m_iTeacherID			;
	}
	m_pNet->SendToServer(lpPackage);
}

void CThreadPaperServer::OnNetReloginOK()
{
	this->RequestPaper(m_iTotal - m_iMarking);
}

void CThreadPaperServer::OnLogoutACK( LPUSER_MESSAGE lpUserMsg )
{
	if(lpUserMsg->yUserType == MSG_SERVER_LOGOUT_SUCCESS)
	{
		LPPACK_LOGOUT_SUCCESS p = (LPPACK_LOGOUT_SUCCESS)lpUserMsg;
		int m_iTeacherID = p->iTeacherID;
		int m_iOperatpor = p->yNewOperator;
		m_bConnected	 = FALSE;
		// 向GUI发送界面更新的消息
		m_pDlg->OnLogoutOK();

	}
	if(lpUserMsg->yUserType == MSG_SERVER_LOGOUT_FAILURE)
	{
		LPPACK_LOGOUT_FAILURE p = (LPPACK_LOGOUT_FAILURE)lpUserMsg;
		CString str;
		str.Format("%S", p->strFailInfo);
		// 向GUI发送界面更新的消息 //
		m_pDlg->OnLogoutFailed(str);
	}
}

void CThreadPaperServer::UpdateServerState( )
{
	LPCLIENT_CUTSHEET	pCutSheet	= NULL;

	if ( m_iTotal != 0 )
	{
		pCutSheet	= (LPCLIENT_CUTSHEET)m_lstFreshPaper.GetAt(m_posCurrent);
	}
}

void CThreadPaperServer::OnReceiveFromServer( LPPACKAGE lpPackage )
{
	LPUSER_MESSAGE	lpUserMsg;

	lpUserMsg	= &(lpPackage->body.userMsg);
	switch ( lpUserMsg->yUserType )
	{

	case MSG_SERVER_LOGIN_SUCCESS:
			this->OnLoginACK(lpUserMsg);
			break;

	case	MSG_SERVER_LOGIN_FAILURE:
		{
			LPPACK_LOGIN_FAILURE	pPack;
			pPack	= (LPPACK_LOGIN_FAILURE)(lpUserMsg);

			CString	str;
			str.Format( "%s", pPack->strFailInfo );

//			m_pFrame->OnLoginFailure( str );
			if (pPack->iTeacherID == -1)
				m_pDlg->OnLoginFailure( str );
			else
				m_pDlg->OnTwiceLogin(pPack->iTeacherID, str);
		}
		break;
	case	MSG_SERVER_PASSWORD_MODIFICATION_ACK:
		{
			OnModifyPwdACK( lpUserMsg );
		}
        break;

	case	MSG_SERVER_PAPER_FTP:
		{
			LPPACK_PAPER_FTP	lpPaperFTP;
			lpPaperFTP	= (LPPACK_PAPER_FTP)(lpUserMsg);

			RecvOnePaper( lpPaperFTP );

			m_iTotal++;
		}
		break;
	case	MSG_SERVER_CHECK_CUTSHEET:
		{
			this->OnServerCheckCutSheet(lpUserMsg);
		}
		break;
		
	default:
		break;
	}
}

void CThreadPaperServer::OnConnServerFailed()
{
	m_pDlg->SetStateLogon();
}

// 接收下载的试卷处理函数
void CThreadPaperServer::RecvOnePaper( LPPACK_PAPER_FTP	lpPaperFTP )
{
//	m_nLastRequestPendingCnt--;
	// 开始处理新下载的试卷
//	LPPACK_TRANSFER_CUTSHEET pCutsheet = (LPPACK_TRANSFER_CUTSHEET)lpUserMsg;
//	LPCLIENT_CUTSHEET pCutSheet = new CLIENT_CUTSHEET;
//	pCutSheet->iCutSheetID	= lpPaperFTP->iCutSheetID;


	// 将它加到m_lstFreshPaper链表
	//m_lstFreshPaper.AddTail((PVOID)pCutSheet);

	// 存盘 首先切换到当前目录 之后创建文件
	//m_strPicPath.Format("%s", 
	_chdir(m_strPicPath);
    CFile	m_FileRecv;
	CString strFileName;
	strFileName.Format("%d.gif", lpPaperFTP->iCutSheetID);
	if( !m_FileRecv.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	{
		return;
	}
	m_FileRecv.Write(&(lpPaperFTP->data), lpPaperFTP->dwLen);
	m_FileRecv.Flush();
	m_FileRecv.Close();	
/*
	if (m_pDlg->IsNextButtonEnable() == TRUE)
	{
		m_pDlg->EnableNextButton();
	}
*/
	LPPAPER_CUTSHEET	pPaper;
	pPaper	= new PAPER_CUTSHEET;
	ZeroMemory( pPaper, sizeof(PAPER_CUTSHEET) );
	pPaper->iCutSheetID	= lpPaperFTP->iCutSheetID;
	pPaper->bMarked		= FALSE;
	pPaper->bBadCutSheet= FALSE;
	pPaper->pNextPaper	= NULL;

	if( NULL == m_lstPaperHead )
	{
		pPaper->pPrevPaper	= NULL;

		m_lstPaperHead	= pPaper;
		m_lstPaperTail	= pPaper;
		m_pPaperCurrent	= pPaper;
        		
		m_pDlg->SetCurrentPaper( m_pPaperCurrent );
		m_pDlg->InsertNewCutSheet( m_pPaperCurrent->iCutSheetID );
		m_pDlg->DisplayMarkedCnt(0);
		for (int i = 0; i < m_pDlg->m_iSteps; i++)
		{
			m_pDlg->m_iStepMark[i]	= 0;
		}
		m_pDlg->UpdateData(FALSE);
	}
	else
	{
		m_lstPaperTail->pNextPaper	= pPaper;
		pPaper->pPrevPaper			= m_lstPaperTail;
		m_lstPaperTail				= pPaper;			// 设置新尾巴

		m_pDlg->EnableNextButton();
	}
}

void CThreadPaperServer::OnLoginACK( LPUSER_MESSAGE lpUserMsg)
{
	CString					str;
	LPCHANNEL_INFO			pChannelInfo;
	LPPACK_LOGIN_SUCCESS	pPack;

	pPack	= (LPPACK_LOGIN_SUCCESS)lpUserMsg;

	m_iStepCount	= pPack->lpChannel.iStepCnt;
	m_iTeacherID = pPack->iTeacherID;
	m_iOperator	 = pPack->iOperator;
	//str.Format("%s", pPack->strNickName);
	// whu-jarrywhu-2005-01-29,begin
	m_strPicPath.Format("%d", pPack->iTeacherID);
	if(_chdir(m_strPicPath) == -1)
	{
		_mkdir(m_strPicPath);
	}
	// whu-jarrywhu-2005-01-29,end
	// 开始请求试卷
//	int	iUnchecked = m_lstFreshPaper.GetCount();
	this->RequestPaper(0);

	pChannelInfo	 = (LPCHANNEL_INFO)&(pPack->lpChannel);

	// 初始化GUI参数
	//strcpy(m_pDlg->m_strNickname.GetBuffer(), str);
	m_pDlg->m_strNickname = pPack->strNickName;
	
	m_pDlg->m_iSteps	= pChannelInfo->iStepCnt;

	

	for (int j = 0; j < pChannelInfo->iStepCnt; j++)
	{
		m_pDlg->m_iStepReservedMark[j]	= pChannelInfo->yStepScore[j];
		strcpy(m_pDlg->m_strStepName[j], pChannelInfo->strStepName[j]);
	}

	int m = 0;
	for (int k = 0; k < m_pDlg->m_iSteps; k++)
	{
		m += pChannelInfo->yStepScore[k];
	}
	m_pDlg->m_iReservedMark = m;

	for (int i = 0; i < m_pDlg->m_iSteps; i++)
	{
		m_pDlg->m_iStepMark[i]	= 0;
	}

	m_pDlg->OnLoginOK();
}

void CThreadPaperServer::RequestPaper(int iUncheckedCnt)
{
	LPPACKAGE				lpPackage			;
    LPPACK_PAPER_REQUEST	lpPackPaperRequest	;

	if ( m_lpPtrCommandMemList->GetCount() != 0 )
	{
        lpPackage = (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

		lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_PAPER_REQUEST);
		lpPackage->packageHeader.yTypePackage	= 0;

		lpPackPaperRequest	= (LPPACK_PAPER_REQUEST)(&(lpPackage->body));

		if (lpPackPaperRequest != NULL)
		{
			lpPackPaperRequest->yUserType		= MSG_CLIENT_PAPER_REQUEST		;
			lpPackPaperRequest->iUncheckedCnt	= iUncheckedCnt					;
		}
	
		m_pNet->SendToServer( lpPackage)					;
	}
//	m_lpPtrCommandMemList->AddTail( (LPVOID)lpPackage)	;
//	m_nLastRequestPendingCnt++;
}

void CThreadPaperServer::Client_modify_userinfo(CString strOldPwd, CString strNewPwd, CString strAccount)
{
	LPPACKAGE lpPackage;
	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();

	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_PASSWORD_MODIFICATION );
	lpPackage->packageHeader.yTypePackage	= 0;

	LPPACK_PASSWORD_MODIFICATION p = (LPPACK_PASSWORD_MODIFICATION)(&(lpPackage->body)); 

	if(NULL != p)
	{
		p->yUserType = MSG_CLIENT_PASSWORD_MODIFICATION;
		lstrcpy(p->strAccount, strAccount)	;
		lstrcpy(p->strPswdOld, strOldPwd)	;
		lstrcpy(p->strPswddNew, strNewPwd)	;

		m_pNet->SendToServer(lpPackage);
		//m_lpPtrCommandMemList->AddTail( (LPVOID)lpPackage);
	}
}

void CThreadPaperServer::OnModifyPwdACK( LPUSER_MESSAGE lpUserMsg )
{
	LPPACK_PASSWORD_MODIFICATION_ACK p = (LPPACK_PASSWORD_MODIFICATION_ACK)lpUserMsg;
	CString str;
	str.Format("%s", p->strModInfo);
	// BOOL bModify = p->bSuccess;
	// 向GUI发送通知消息
	m_pDlg->OnModifyPwd( str);
}


void CThreadPaperServer::ReturnMsg2MemList( LPPACKAGE lpPackage )
{
	ZeroMemory((void*)lpPackage,SIZE_COMMAND_MEM);
	POSITION pos;
	pos = m_lpPtrCommandMemList->Find((void*)lpPackage);
	if(pos != NULL)
	   m_lpPtrCommandMemList->RemoveAt(pos); 
	m_lpPtrCommandMemList->AddTail((LPVOID)lpPackage); 	
}

// whu-jarrywhu-2005-01-25,begin
void CThreadPaperServer::SubmitPaper( LPCLIENT_PAPER_PACKED pPaperPacked )
{	
	if(pPaperPacked == NULL)
	{
		return;
	}
	else
	{
		if (m_lpPtrCommandMemList->GetCount() != 0)
		{
			LPPACKAGE			lpPackage	;
			LPPACK_PAPER_SUBMIT pPaperSubmit;

			lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead();
			DWORD		dwSubmitSize;
			int m	= pPaperPacked->iPaperPackedCnt;

			dwSubmitSize	= sizeof(PACK_PAPER_SUBMIT) - sizeof(PAPER_SUBMITTED)  + m * ( (sizeof(PAPER_SUBMITTED) - sizeof(BYTE)) + (m_iStepCount * sizeof(BYTE)) );
//			dwSubmitSize	= sizeof(int)  + m * ( sizeof(int) + sizeof(BOOL) + (m_iStepCount * sizeof(BYTE)) );
//			dwSubmitSize	= sizeof(int) + 2 * sizeof(BYTE) + m * ( sizeof(int) + (16 * sizeof(BYTE)) );

			lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + dwSubmitSize;
			lpPackage->packageHeader.yTypePackage	= 0;
		
			pPaperSubmit = (LPPACK_PAPER_SUBMIT)(&(lpPackage->body));
		
			pPaperSubmit->yUserType	 = MSG_CLIENT_PAPER_SUBMIT		;
			pPaperSubmit->iSubmitCnt = pPaperPacked->iPaperPackedCnt; 
			for(int n = 0; n < pPaperSubmit->iSubmitCnt; n++)
			{
				(pPaperSubmit->pItem[n]).nCutSheetID = (pPaperPacked->ClientCutsheet[n]).iCutSheetID;
				(pPaperSubmit->pItem[n]).bBadCutSheet= (pPaperPacked->ClientCutsheet[n]).bBadCutSheet;
				for (int i = 0; i < m_pDlg->m_iSteps; i++)
				{
					(pPaperSubmit->pItem[n]).yStepScore[i] = (pPaperPacked->ClientCutsheet[n]).yStepScore[i];
				}
			}
			pPaperSubmit->iVersion = m_iVersion;

			// 存盘
			CFile file;
			CString strFileName	;
			strFileName.Format("%d", m_iVersion);
			if( !file.Open(strFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
			{
				return;
			}
			file.Write(&(pPaperSubmit), dwSubmitSize);
			file.Flush();
			file.Close();

			m_iVersion++;

			m_iTotal	-= pPaperSubmit->iSubmitCnt;			

			for (int k = 0; k < pPaperSubmit->iSubmitCnt; k++)
			{	
				CFile delFile;	
				CString strDelFileName;
				strDelFileName.Format("%d.gif", pPaperSubmit->pItem[k].nCutSheetID);
				delFile.Remove(strDelFileName);
			}

			// 将包发出去
			m_pNet->SendToServer(lpPackage)						;
//			m_lpPtrCommandMemList->AddTail( (LPVOID)lpPackage)	;

			// 请求试卷
			// this->RequestPaper(m_iTotal - m_iMarking);
			if (m_bLogout == FALSE)
			{
				this->RequestPaper(m_iTotal);
			}
		}
		
	}
}

void CThreadPaperServer::PaperSubmitPacked( int M )
{
	LPCLIENT_PAPER_PACKED pPaperPacked;
	
	pPaperPacked = (LPCLIENT_PAPER_PACKED) new char[1024];	

	pPaperPacked->iPaperPackedCnt = M;

	for(int j = 0; j < M; j++)
	{
		LPPAPER_CUTSHEET	pPaper;			

		pPaper	= m_lstPaperHead;
		
		m_lstPaperHead	= pPaper->pNextPaper;

		//pPaperPacked->iPaperPackedCnt++;
		pPaperPacked->ClientCutsheet[j].iCutSheetID = pPaper->iCutSheetID	;

		// whu-jarrywhu-2005-02-01, for bad paper!
		pPaperPacked->ClientCutsheet[j].bBadCutSheet= pPaper->bBadCutSheet	;
		// whu-jarrywhu-2005-02-01,end

		for (int k = 0; k <m_pDlg->m_iSteps; k++)
		{
			pPaperPacked->ClientCutsheet[j].yStepScore[k] = pPaper->yStepScore[k];
		}
	}
	m_lstPaperHead->pPrevPaper = NULL	;
	this->SubmitPaper(pPaperPacked)		;
	delete pPaperPacked;
}
// 递交m_lstSubmitPaper中所有的SubmitPaper!
void CThreadPaperServer::SubmitAllPaper2Server( )
{
	LPCLIENT_PAPER_PACKED pPaperPacked;

	if (m_iMarking > MAX_MARKED_COUNT)
	{
		m_iMarking -= MAX_MARKED_COUNT;
		PaperSubmitPacked( MAX_MARKED_COUNT );
        PaperSubmitPacked( m_iMarking );
	}
	else
		PaperSubmitPacked( m_iMarking );

	m_iCurrent	= 0;
	m_iTotal	= 0;
	m_iMarking	= 0;
}

// whu-jarrywhu-2005-01-25,begin
void CThreadPaperServer::GetPrevPaper()
{
	// 存储当前笔迹文件	??
	if ( NULL != m_pPaperCurrent->pPrevPaper )
		m_pPaperCurrent	= m_pPaperCurrent->pPrevPaper;

	m_pDlg->SetCurrentPaper( m_pPaperCurrent );

	m_iCurrent	--;
}

// whu-jarrywhu-2005-01-28,begin
void CThreadPaperServer::GetNextPaper()
{
	if ( NULL != m_pPaperCurrent->pNextPaper )
		m_pPaperCurrent	= m_pPaperCurrent->pNextPaper;;

	m_pDlg->SetCurrentPaper( m_pPaperCurrent );

	m_iCurrent	++;
	if ( m_iCurrent > m_iMarking )
	{
		m_iMarking	= m_iCurrent;
	}
	// 当用户阅完10份试卷发送一次请求
	//if ( m_iMarking == 10 )
	//	this->RequestPaper(m_iTotal - m_iMarking);

	// 总数
	if ( m_iMarking >= MAX_MARKED_COUNT )
	{
		// 上缴
		
//		m_csSubmit.Unlock();
		
		//PaperSubmitPacked( MAX_MARKED_COUNT );
		PaperSubmitPacked( MAX_SUBMIT_COUNT );

//		m_iTotal	-= MAX_SUBMIT_COUNT;	将在上传时改变它的值

		// 设置m_iCuurent
		// 设置m_iMarking
		m_iCurrent	-= MAX_SUBMIT_COUNT;
		m_iMarking	-= MAX_SUBMIT_COUNT;
	}

	return;

	// 存储当前笔迹文件	??
}
// whu-jarrywhu-2005-01-28,end

//whu-jarrywhu-2005-01-25,end
/*
// whu-jarrywhu-2005-01-29,begin
void CThreadPaperServer::UpdateMarkedCnt()
{	
	if( m_iCurrent >= m_iMarking)
	{
		m_iMarkedCnt ++		;
		m_pDlg->DisplayMarkedCnt(m_iMarkedCnt);	
	}
	else
		return;
}
// whu-jarrywhu-2005-01-29,end
*/
CThreadPaperServer::CThreadPaperServer()
{
	CString		temp;
	GetCurrentDirectory(255, temp.GetBuffer(255));
	//m_strPicPath.Format("%s\\pics", temp);
	m_strPicPath.Format("%s\\", temp);
	temp.ReleaseBuffer();
	
	/*if(_chdir(m_strPicPath) == -1)
	{
		_mkdir(m_strPicPath);
	}
	_chdir(m_strPicPath);*/
	
	m_pNet	= new CNetServer(this);

	m_pNet->StartNetServer( );

	m_bPaperServerStarted	= FALSE;

	// whu-jarrywhu-2005-01-24,begin
	m_iLastRequestPendingCnt = 0	;
	// whu-jarrywhu-2005-01-24,end

	// whu-jarrywhu-2005-01-25,begin
	m_iCount				 = 0	;
	m_iRollCount			 = 0	;
	m_posMarking			 = m_lstFreshPaper.GetHeadPosition();
	m_bLogout				 = FALSE;
	// whu-jarrywhu-2005-01-25,end

	m_lstPaperTail	= NULL;
	m_lstPaperHead	= NULL;
}

CThreadPaperServer::~CThreadPaperServer()
{
	if ( m_bPaperServerStarted )
	{
		StopPaperServer();
		Sleep(500);

		m_pNet->StopNetServer( );
		Sleep(500);
	}
	
	delete	m_pNet;

	while( NULL != m_lstPaperHead )
	{
		m_lstPaperTail	= m_lstPaperHead->pNextPaper;
		delete	m_lstPaperHead;
		m_lstPaperHead	= m_lstPaperTail;
	}
}

BOOL CThreadPaperServer::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CThreadPaperServer::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CThreadPaperServer, CWinThread)
	ON_THREAD_MESSAGE( WM_EXIT_THREAD, OnAppExitMessage )
	ON_THREAD_MESSAGE( WM_SCOREDLG_HWND, OnScoreDlgHWnd )
END_MESSAGE_MAP()


// CThreadPaperServer 消息处理程序

void CThreadPaperServer::OnAppExitMessage(WPARAM wParam, LPARAM lParam)
{
	AfxEndThread( 888 );
}

void CThreadPaperServer::OnScoreDlgHWnd(WPARAM wParam, LPARAM lParam)
{
//	m_pFrame	= (CMainFrame*)wParam;
	m_pDlg	= (CScoreDlg*)wParam;
}

void CThreadPaperServer::OnTimer(UINT nIDEvent)
{
	m_pNet->OnTimer(nIDEvent);
}

BOOL CThreadPaperServer::FindCutSheetOnDisk(int iCutSheetID)
{
	_chdir(m_strPicPath);

	CString		str		;
	CFileFind	finder	;
	str.Format("%d.gif", iCutSheetID);
	BOOL bfind = finder.FindFile(str);
	if (bfind == TRUE)
	{
		bfind = finder.FindNextFile();
		finder.Close();
		return TRUE;
	}
	else
	{
		finder.Close();
		return FALSE;
	}
}


void CThreadPaperServer::OnServerCheckCutSheet(LPUSER_MESSAGE lpUserMsg)
{
	LPPACK_UNCHECKED_ON_DISK pPaperOnDisk;
	pPaperOnDisk = (LPPACK_UNCHECKED_ON_DISK)lpUserMsg;

	LPPACK_CHECKED_RESULT p ;

	p->iCutSheetCnt = 0;
	int iCount;
	iCount = pPaperOnDisk->iCutSheetCnt;
	for (int i = 0; i < iCount; i++)
	{
		BOOL bFind;
		bFind = FindCutSheetOnDisk(pPaperOnDisk->iCutSheetID[i]);
		if (bFind == FALSE)
		{
			p->iCutSheetID[p->iCutSheetCnt] = pPaperOnDisk->iCutSheetID[i];
			p->iCutSheetCnt++;
		}
	}
	this->ClientCheckedResult(p);
}

void CThreadPaperServer::ClientCheckedResult(LPPACK_CHECKED_RESULT lpCheckedResult)
{
	LPPACK_CHECKED_RESULT pCheckedResult;
	LPPACKAGE lpPackage;

	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead( );
	
	DWORD dwResult;
	dwResult = sizeof(BYTE) + 2 * sizeof(int) + lpCheckedResult->iCutSheetCnt * sizeof(int);
	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + dwResult;
	lpPackage->packageHeader.yTypePackage	= 0;

	pCheckedResult	= (LPPACK_CHECKED_RESULT)(&(lpPackage->body));

	pCheckedResult->yUserType		= MSG_CLIENT_SUBMIT_CUTSHEETNO	;
	pCheckedResult->iCutSheetCnt	= lpCheckedResult->iCutSheetCnt	;
	pCheckedResult->iTeacherID		= lpCheckedResult->iTeacherID	;
	for (int i = 0; i < pCheckedResult->iCutSheetCnt; i++)
	{
		pCheckedResult->iCutSheetID[i] = lpCheckedResult->iCutSheetID[i];
	}
	m_pNet->SendToServer(lpPackage);
}

void CThreadPaperServer::OnKickout(int iTeacherID)
{
	LPPACKAGE		lpPackage	;
	LPPACK_KICKOUT	lpKick		;

	lpPackage	= (LPPACKAGE)m_lpPtrCommandMemList->RemoveHead( );
	lpPackage->packageHeader.dwSize			= sizeof(PACKAGE_HEADER) + sizeof(PACK_KICKOUT);
	lpPackage->packageHeader.yTypePackage	= 0;

	lpKick	= (LPPACK_KICKOUT)(&(lpPackage->body));
	lpKick->yUserType  = MSG_CLIENT_KICKOUT	;	
	lpKick->iTeacherID = iTeacherID			;

	m_pNet->SendToServer(lpPackage);
}