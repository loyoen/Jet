/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[Module]
	message management

[Description]
	This class manages all messages sent to / received by GUI

[Author]
	jicheng @ whu.edu.cn

[Date]
	2014_07_09

[Revision History]
When		Who			Where		Description
----------------------------------------------------------------------
2014/07/09	jicheng		WuHan		created
----------------------------------------------------------------------
2014/07/10	jicheng		WuHan		first released
----------------------------------------------------------------------

------------------------------------------------------------------------------*/

#ifndef	__MGR_MESSAGE_H__
#define	__MGR_MESSAGE_H__

#include "CCriticalSectionWrapper.h"

#define	GUI_MSG_LENGTH		1024

#define WM_GUI_LOG			WM_USER + 2100

class CMgrMessage
{
public:
	static	CMgrMessage	*pThis;

	HWND	m_hWndGUI;

public:
	CMgrMessage ( );
	~CMgrMessage ( );

	static CMgrMessage*	getManager ();
	static void	postMsgToGUI ( UINT msgID, int wParam, char *pMsg );

	void	setWndGUI ( HWND hWnd );

	void	InsertMessageEntity ( char *p );
	char*	GetMessageEntity ( );

	CPtrList	m_lstMsg;
	CPtrList	m_lstMsgAllocated;

	CCriticalSectionWrapper	m_csMsg;
};

#endif