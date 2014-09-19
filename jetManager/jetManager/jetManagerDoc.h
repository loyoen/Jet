
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

//	当笔迹被完全撤消时, 可能需要删除此前的笔迹文件
	BOOL	        Delete_script_file ( LPCTSTR lpszScriptPathName );

	BOOL	m_bScriptChanged;
	BOOL	m_bHasScript;

	CString m_csScriptPath;
	CString m_csOldPictPath;

	CTypedPtrList<CObList, CScriptLine*>	m_ScriptLineList;

//, begin
#ifdef	SWITCH_STORE_SCRIPT
	// 存放他机器笔迹的链表
	CTypedPtrList<CObList, CScriptLine*>	m_lstScript_of_Machine1;
	CTypedPtrList<CObList, CScriptLine*>	m_lstScript_of_Machine2;
	CTypedPtrList<CObList, CScriptLine*>	m_lstScript_of_Machine3;
	
	// 是否显示笔迹的标志
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

//	int		m_iJobID;	// 当前图片的ID号

//, begin
//	void	SetCurrentTaskID( int iFormatSheetID );

	LPFORMAT_SHEET	m_pTaskCurrent;	// 当前job

	void	SetCurrentTask( LPFORMAT_SHEET pTask );
//, ends

protected: // 仅从序列化创建
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
    CPtrList        m_lstJob;   	//待打印job

	CPictureView*	m_pPictureView;
	CDlgManager*		m_pScoreDlg;
	CImage	        m_Pict;

// 属性
public:	
	BOOL	m_bChangePict;

	BOOL    m_bDestroyAndExit;

	CString		m_strUserPath	;	// 当前打印机器存放图片的路径
	
	int     m_nPrevTaskCnt;

public:
	afx_msg void OnUpdateCommandLogout(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandModifypwd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandNotify(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommandSubmittask(CCmdUI *pCmdUI);

// 生成的消息映射函数
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


