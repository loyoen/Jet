#pragma once

class CMid
{
public:
	CMid(void);
	~CMid(void);

public:
	DWORD	m_dwTotal;
	DWORD	m_dwLeft;
	char*	m_lpBegin;
	char*	m_lpNow;
	BOOL	m_bFree;
	void Init(void);
	void Fresh(DWORD dwLen);
	void SendInit(char* dwBuf, DWORD dwTotalLen);
	void RecInit(char* lpBuf, DWORD dwTotalLen);
};
