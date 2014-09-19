#include "StdAfx.h"
#include ".\mid.h"

CMid::CMid(void)
: m_dwTotal(0)
, m_dwLeft(0)
, m_lpBegin(NULL)
, m_lpNow(NULL)
, m_bFree(TRUE)
{
}

CMid::~CMid(void)
{
}

void CMid::Init(void)
{
	m_dwTotal	= 0;
	m_dwLeft	= 0;
	m_lpBegin	= 0;
	m_lpNow		= 0;
	m_bFree		= TRUE;
}

void CMid::Fresh(DWORD dwLen)
{
	m_lpNow		+= dwLen;
	m_dwLeft	-= dwLen;
}

void CMid::SendInit(char* lpBuf, DWORD dwTotalLen)
{
	m_dwTotal	= dwTotalLen;
	m_lpBegin	= lpBuf;
	m_lpNow		= lpBuf;
	m_dwLeft	= m_dwTotal;
	m_bFree		= FALSE;
}

void CMid::RecInit(char* lpBuf, DWORD dwTotalLen)
{
	m_dwTotal	= dwTotalLen;
	m_lpBegin	= lpBuf;
	m_lpNow		= lpBuf;
	m_dwLeft	= m_dwTotal;
}
