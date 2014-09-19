#if !defined(AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_)
#define AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorListBox window
typedef struct tagListBoxItemStyle
{
    COLORREF clrText;
	COLORREF clrBg;
	UINT     nIconIndex;
}t_listbox_item_style;

class CColorListBox : public CListBox
{
// Construction
public:
	CColorListBox();

// Attributes
public:    
	int  AddListBoxItemStyle(int nIconIndex,COLORREF clrText,COLORREF clrBg);
	int  AddLogString(int nClass,LPCTSTR lpszItem);
	void SetImageList(int nBitmapID,int nCnt);

// Operations
public:
   t_listbox_item_style  m_arrListBoxItemStyle[15];
   int                   m_nNumStyle;

   int                   m_nBitmapID;
   int                   m_nIconCnt;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORLISTBOX_H__D23C2394_326F_11D6_AB38_00D0B70C3D79__INCLUDED_)
