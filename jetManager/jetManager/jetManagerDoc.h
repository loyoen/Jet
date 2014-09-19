
#pragma once

#include "PictureView.h"
#include "GUI/dlgManager.h"
#include "ScriptLine.h"

//#include "NetThread.h"

#define EMARK_MAX_ROLLBACK_COUNT  10

class CjetManagerDoc : public CDocument
{
// Scripts
public:
	//Script Operation
	CScriptLine*	NewScriptLine( );
	void	        DiaplayTaskPict(CString PictPath);	
	void			DeleteScripts( );
	BOOL	        OpenScriptFile(LPCTSTR lpszScriptPathName);
	BOOL	        SaveScriptFile(LPCTSTR lpszScriptPathName);

//	���ʼ�����ȫ����ʱ, ������Ҫɾ����ǰ�ıʼ��ļ�
	BOOL	        Delete_script_file ( LPCTSTR lpszScriptPathName );

	BOOL	m_bScriptChanged;
	BOOL	m_bHasScript;

	CString m_csScriptPath;
	CString m_csOldPictPath;

	CTypedPtrList<CObList, CScriptLine*>	m_ScriptLineList;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	// ����������ʼ�������
	CTypedPtrList<CObList, CScriptLine*>	m_lstScript_of_Machine1;
	CTypedPtrList<CObList, CScriptLine*>	m_lstScript_of_Machine2;
	CTypedPtrList<CObList, CScriptLine*>	m_lstScript_of_Machine3;
	
	// �Ƿ���ʾ�ʼ��ı�־
	BOOL	m_bFlag1;
	BOOL	m_bFlag2;
	BOOL	m_bFlag3;

	void	Set_script_list ( int iMachineID, int iFormatSheetID, int iNumber );
	BOOL	Open_script_file ( LPCTSTR lpszScriptPathName, CTypedPtrList<CObList, CScriptLine*> *lplstScript );
#endif
//, ends

public:
	void	SetTaskServer( CThreadTaskServer* pTaskServer );
	void	SetUserPath( CString strPath );

	CThreadTaskServer*	m_pTaskServer;

//	int		m_iJobID;	// ��ǰͼƬ��ID��

//, begin
//	void	SetCurrentTaskID( int iFormatSheetID );

	LPFORMAT_SHEET	m_pTaskCurrent;	// ��ǰjob

	void	SetCurrentTask( LPFORMAT_SHEET pTask );
//, ends

protected: // �������л�����
	CjetManagerDoc();
	CView*	GetView(CRuntimeClass* pClass);
	DECLARE_DYNCREATE(CjetManagerDoc)
//Methods:
public:
	//Initialize and Deinitialize
	virtual BOOL OnNewDocument();
    virtual ~CjetManagerDoc();

	//Task management
	void   InsertNewJob(int nCutSheeID,float fMaxScore);

	
	//Picture Operation
	BOOL	m_bHasPict;
	CImage*	GetPicture(){ if ( m_bHasPict) return &m_Pict; else return NULL;};
	void	DeleteAllPictures(CString szPath);

	//For Debug Version Only
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//Attributs
public:
    CPtrList        m_lstJob;   	//����ӡjob

	CPictureView*	m_pPictureView;
	CDlgManager*		m_pScoreDlg;
	CImage	        m_Pict;

// ����
public:	
	BOOL	m_bChangePict;

	BOOL    m_bDestroyAndExit;

	CString		m_strUserPath	;	// ��ǰ��ӡ�������ͼƬ��·��
	
	int     m_nPrevTaskCnt;

public:
	afx_msg void OnUpdateCommandLogout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandModifypwd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandNotify(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandSubmittask(CCmdUI *pCmdUI);

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnLogout();
	afx_msg void OnPasswordModify();
	afx_msg void OnSendNotifyMsg();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnUpdateUnchecktask(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
};


