// ScriptLine.h: interface for the CScriptLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCRIPTLINE_H__1B5A2794_967F_4976_B5C5_6C7857DED960__INCLUDED_)
#define AFX_SCRIPTLINE_H__1B5A2794_967F_4976_B5C5_6C7857DED960__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CScriptLine : public CObject  
{
	DECLARE_SERIAL(CScriptLine)

public:
	CScriptLine();
	virtual void Serialize(CArchive& ar);

	CArray<CPoint, CPoint>	m_pointArray;
	CString					m_strComment;

	BOOL DrawScriptLine(CDC* pDC, CPen& DrawPen, float ShowModeX, float ShowModeY, float ShowScale);

	virtual ~CScriptLine();
};

#endif // !defined(AFX_SCRIPTLINE_H__1B5A2794_967F_4976_B5C5_6C7857DED960__INCLUDED_)
