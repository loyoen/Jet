//#include "..\stdafx.h"
#ifndef CCRITICALSECTIONWRAPPERINCLUDED
class CCriticalSectionWrapper
{
public:
	CCriticalSectionWrapper()
	{
		m_bInitialized = TRUE;
		InitializeCriticalSection(&m_criticalSection);
	}

	~CCriticalSectionWrapper()
	{
		if (m_bInitialized)
			DeleteCriticalSection(&m_criticalSection);
		m_bInitialized = FALSE;
	}

	void Lock()
	{
		if (m_bInitialized)
			EnterCriticalSection(&m_criticalSection);
	}
	void Unlock()
	{
		if (m_bInitialized)
			LeaveCriticalSection(&m_criticalSection);
	}
protected:
	CRITICAL_SECTION m_criticalSection;
	BOOL m_bInitialized;
};
#define CCRITICALSECTIONWRAPPERINCLUDED
#endif