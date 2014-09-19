#include "StdAfx.h"
#include "MgrMessage.h"
#include "Resource.h"

CMgrMessage* CMgrMessage::pThis	= NULL;

CMgrMessage* CMgrMessage::getManager ( )
{
	if ( NULL == pThis )
	{
		pThis	= new CMgrMessage;

		pThis->m_hWndGUI	= NULL;
	}

	return	pThis;
}

void CMgrMessage::postMsgToGUI ( UINT msgID, int wParam, char *pMsg )
{
	if ( NULL == pThis )
		return;

	if ( NULL == pThis->m_hWndGUI )
	{
		// recycle the message space
		if ( NULL != pMsg )
		{
			pThis->InsertMessageEntity ( pMsg );
		}

		return;
	}

	::PostMessage ( pThis->m_hWndGUI, msgID, (WPARAM)wParam, (LPARAM)pMsg );
}

void CMgrMessage::setWndGUI ( HWND hWnd )
{
	CMgrMessage	*p;

	p	= getManager ();

	p->m_hWndGUI	= hWnd;
}

CMgrMessage::CMgrMessage ( )
{
}

CMgrMessage::~CMgrMessage ( )
{
	POSITION	pos;
	char *		p	= NULL;

	if( !m_lstMsg.IsEmpty() )
	{
		for ( pos=m_lstMsg.GetHeadPosition(); pos!=NULL; )
		{
			p	= (char*)m_lstMsg.GetNext(pos);
			delete[] p;
		}
		m_lstMsg.RemoveAll();
	}
    
	if( !m_lstMsgAllocated.IsEmpty() )
	{
		for ( pos=m_lstMsgAllocated.GetHeadPosition(); pos!=NULL; )
		{
			p	= (char*)m_lstMsgAllocated.GetNext(pos);
			TRACE1("CMgrMessage Memory Leak Detected 0x%X\n",(void*)p);
			delete[] p;
		}
		m_lstMsgAllocated.RemoveAll();
	}
}

void CMgrMessage::InsertMessageEntity ( char *p )
{
	if ( NULL == p )
		return;

	// initialzie buffer
	ZeroMemory ( p, GUI_MSG_LENGTH );

	// add to list
	m_csMsg.Lock();
	
	POSITION	pos	= m_lstMsgAllocated.Find((void*)p);
	if ( NULL != pos )
          	m_lstMsgAllocated.RemoveAt(pos); 
	else
	{
		TRACE0("CMgrMessage::InsertMessageEntity recycle a message space that never allocated be4\n");
	}

	m_lstMsg.AddTail((void*)p); 
	m_csMsg.Unlock();
}

char* CMgrMessage::GetMessageEntity ( )
{
	if ( NULL == pThis )
		return	NULL;

	m_csMsg.Lock();
	if ( pThis->m_lstMsg.IsEmpty() )
	{
        char *pMsg	= new char[GUI_MSG_LENGTH];

		if ( NULL != pMsg )
		{
			m_lstMsgAllocated.AddTail ( (void*)pMsg ); 
		}
		m_csMsg.Unlock();
		return	pMsg;
	}

    char *p	= NULL;

	p	= (char*)pThis->m_lstMsg.RemoveHead();

	m_lstMsgAllocated.AddTail ( (void*)p ); 
	m_csMsg.Unlock();

	return	p;
}
