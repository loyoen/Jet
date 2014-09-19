#pragma once


// CColorButton

class CColorButton : public CButton
{
	DECLARE_DYNAMIC(CColorButton)

public:
	BOOL	m_bHighlight;
	BOOL	m_bChecked;
	HICON	m_hHand;

	COLORREF	m_color;

	void	Set_text_color ( COLORREF color );
	void	Set_highlight ( BOOL bFlag=TRUE );
	void	Reset ( );

	CColorButton();
	virtual ~CColorButton();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void DrawItem ( LPDRAWITEMSTRUCT lpDrawItemStruct );
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


