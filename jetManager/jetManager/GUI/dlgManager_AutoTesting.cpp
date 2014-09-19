/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[Module]
	dlgManager

[Description]
	A User interaction dialog implementation for management Client!
[Author]
	DavidRipple(DavidRipple@tom.com)
[Date]
	2004-12-12
[Revision History]
Date		Who			Where	Description
----------------------------------------------------------------------
2004-12-12	DavidRipple	WuHan	First released!
----------------------------------------------------------------------
2014-07-15	jicheng		Wuhan	Adapted to general purpose management
----------------------------------------------------------------------
------------------------------------------------------------------------------*/

#include "StdAfx.h"
#include "jetManager.h"
#include "dlgManager.h"

#ifdef	SWITCH_AUTO_TESTING
#include "TimeInterval.h"

DWORD __stdcall	threadAutoTesting( LPVOID lpVoid );

void CDlgManager::ReportError( CString strError )
{
	GetDlgItem(IDC_STATIC_COMMENT4)->	SetWindowText(strError);
}

DWORD __stdcall threadAutoTesting( LPVOID lpVoid )
{
	CDlgManager*	pThis	= (CDlgManager*)lpVoid;

	if ( pThis->LoopAutoTesting( ) )
	{
		pThis->m_hThreadAutoTesting	= NULL;
		return	802;
	}

	pThis->m_hThreadAutoTesting	= NULL;
	return	8021;
}

// 根据界面的当前状态进行自动状态切换
BOOL CDlgManager::LoopAutoTesting( )
{
	while ( m_bAutoTesting )
	{
		Sleep( 100 );
		switch( m_ctrlState )
		{
		case	Control_Regist_Done:
		case	Control_Marking:
			AutoSetScore ( );
			break;

		case	Control_Marked:
			AutoSetNext ( );
			break;
		//Control_Logon		= -11,
		//Control_Logon_Wait	= -12,
		//Control_Regist		= -13,
		//Control_Regist_Wait	= -14,
		//Control_Download	= -15,
		//Control_Check		= -16,
		//Control_Upload		= -17,
		//Control_Remarking	= -20, 
		//Control_Selecting	= -22,
		//Control_All_Done	= -23
		}
		Sleep( m_nTimeInterval/2 );
	}

	return	TRUE;
}
#endif
