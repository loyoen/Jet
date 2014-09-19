
#pragma once

#include "ThreadTaskServer.h"

class CMainFrame : public CFrameWnd
{
public:
	char	m_pathApp[ _MAX_PATH ];

	CThreadTaskServer*	m_pTaskServer;
	CPtrList*	m_lpLargeMemList;
	CPtrList*	m_lpSmallMemList;
	CPtrList*	m_lpCommandMemList;

	void	ShowScoreDlg ( )	;

protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual      ~CMainFrame() ;
	CToolBar*	GetToolBar()   ;
	BOOL		IsFullScreen() ;
    void	    EndFullScreen();

	//For Debug Version Only
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//Virtual function overridens
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);	


protected:  // 控件条嵌入成员
	CStatusBar      m_wndStatusBar   ;
	CToolBar        m_wndToolBar     ;
	CSplitterWnd	m_wndSplitter    ;
	CToolBar*		m_pwndFullScrnBar;

   	BOOL			m_bFullScreen         ;
	WINDOWPLACEMENT m_wpPrev              ;
	CRect			m_FullScreenWindowRect;
    int             m_nTimerID            ;

protected:
	//窗口消息响应
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewFullscreen();
	afx_msg void OnUpdateViewFullscreen(CCmdUI* pCmdUI);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};


