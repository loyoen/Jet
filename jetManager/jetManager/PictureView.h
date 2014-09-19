#if !defined(AFX_PICTUREVIEW_H__BF25BB17_A7D8_4B80_A3B2_E0B8FDB61459__INCLUDED_)
#define AFX_PICTUREVIEW_H__BF25BB17_A7D8_4B80_A3B2_E0B8FDB61459__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PictureView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPictureView view

#include <afxtempl.h>

#include "ScriptLine.h"
#include "GUI/dlgManager.h"

class CjetManagerDoc;

class CPictureView : public CScrollView
{
protected:
	CImage*		m_pImageLogo;	// 标志图片
	CImage*		m_pImageTask;	// job图片

	CjetManagerDoc*	m_pDoc;		// 文档

	int		m_iZoomFactor;

public:
	void	SetUserPath( CString strPath ){ m_strUserPath = strPath; };
	void	SetAppPath( CString strPath ){ m_strAppPath = strPath; };

protected:
	CString		m_strUserPath	;	// 当前打印机器存放图片的路径
	CString		m_strAppPath	;	// 应用程序的路径
	
	BOOL	m_bFirstTimeShow	;	// 第一次显示视

protected:
	CPictureView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPictureView)

// Attributes
public:
    void ShowLogo(CDC *pDC,HANDLE hBitmap);
// Operations
public:
	//RECT m_rcClientOriginal;
	float m_fDisplayModeX;
	float m_fDisplayModeY;
	SIZE m_PictDisplaySize;
	void SetScrollSizes();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	CScriptLine* m_pScriptLineCur;
	virtual ~CPictureView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	CSize	m_rcPictureOriginal;
	float	m_fDisplayScale;
	CPoint	m_ptPrev;
	BOOL	m_bDrawScript;
	CPen	m_ScriptPen;
//, begin
#ifdef	SWITCH_STORE_SCRIPT
	CPen	m_ScriptPen1;
	CPen	m_ScriptPen2;
	CPen	m_ScriptPen3;
#endif
//, ends

	//{{AFX_MSG(CPictureView)
	afx_msg void OnViewZoomout();
	afx_msg void OnViewZoomin();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDrawscriptOrNot();
	afx_msg void OnUpdateDrawscriptOrNot(CCmdUI* pCmdUI);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
protected:
	virtual void OnUpdate(CView* pSender, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnUpdateViewZoomin(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewZoomout(CCmdUI *pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTUREVIEW_H__BF25BB17_A7D8_4B80_A3B2_E0B8FDB61459__INCLUDED_)
