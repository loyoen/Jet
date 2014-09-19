// ScriptLine.cpp: implementation of the CScriptLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "jetManager.h"
#include "ScriptLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CScriptLine, CObject, 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScriptLine::CScriptLine()
{
	m_strComment.Empty ( );
}

CScriptLine::~CScriptLine()
{

}

BOOL CScriptLine::DrawScriptLine(CDC *pDC, CPen &DrawPen, float ShowModeX, float ShowModeY, float ShowScale)
{
	CPen* pOldPen = pDC->SelectObject(&DrawPen);

	CPoint	point;						//临时转换要画点的坐标
	point.x = m_pointArray[0].x * ShowModeX * ShowScale;
	point.y = m_pointArray[0].y * ShowModeY * ShowScale;
	pDC->MoveTo(point);

	for(int i = 1; i < m_pointArray.GetSize(); i++)
	{
		point.x = m_pointArray[i].x * ShowModeX * ShowScale;
		point.y = m_pointArray[i].y * ShowModeY * ShowScale;
		pDC->LineTo(point);
	}

	pDC->TextOut ( point.x, point.y, m_strComment );

	pDC->SelectObject(pOldPen);

	return TRUE;
}

void CScriptLine::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_strComment;
		m_pointArray.Serialize(ar);
	}
	else
	{
		ar >> m_strComment;
		m_pointArray.Serialize(ar);
	}
}
