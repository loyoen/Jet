#include "stdafx.h"
#include "TransferSocket.h"
#include "eMarkNetMessage.h"
#include "eMarkClient.h"
#include "PackProtocol.h"

#ifndef	_NEW_ARCHITECTURE

CTransferSocket::CTransferSocket(CeMarkNetClient *pClient)
{
	ASSERT(NULL!=pClient);
	m_pClient=pClient;

}

CTransferSocket::~CTransferSocket()
{
    if(GetState()==connected)
		 Close();
	TRACE0("CTransferSocket::~CTransferSocket\n");
}


/*++
   TECOM-20041225-DavidRipple,Add for working out the "packet Sticking" problem.   
--*/
void CTransferSocket::OnReceive(int nErrorCode)
{
	if (GetState() != connected && GetState() != attached && GetState() != closed)
		return;
	CEmarkNetMessageManager *pMgr=CEmarkNetMessageManager::GetManager();
	LPPACK_HEADER p=pMgr->GetMessageEntity();
	
	if(NULL==p)
		return;
	//Firstly get the header
	unsigned short nCmd=0;
	int nCmdBytes=MSG_BUFFER_SIZE;
	//int numread = Receive((void*)p,nCmdBytes);
	
	TRACE0("CTransferSocket::OnReceive\n");
	//Okey recv complete.Now dispatch the msg
	int numread=Receive((void*)&nCmd,sizeof(unsigned short));
	memcpy((void*)p,&nCmd,sizeof(unsigned short));
	char *p1=(char*)p;
	p1+=sizeof(unsigned short);

	LPPACK_SERVER_PAPER_FTP pFTP=NULL;
	//switch(p->cmd)
	switch(nCmd)
	{
	case CMD_SERVER_LOGIN_SUCCESS:
		TRACE0("CMD_SERVER_LOGIN_ACK\n");
        numread=Receive((void*)p1,sizeof(PACK_LOGIN_SUCCESS)-sizeof(unsigned short));
	    m_pClient->OnLoginAck(p);
		break;

	case CMD_SERVER_LOGIN_FAILURE:
		TRACE0("CMD_SERVER_LOGIN_ACK\n");
		numread=Receive((void*)p1,sizeof(PACK_LOGIN_FAILURE)-sizeof(unsigned short));
		m_pClient->OnLoginAck(p);		
		break;

	case CMD_SERVER_LOGOUT_SUCCESS:
		TRACE0("CMD_SERVER_LOGOUT_ACK\n");
		numread=Receive((void*)p1,sizeof(PACK_LOGOUT_SUCCESS)-sizeof(unsigned short));
		m_pClient->OnLogoutAck(p);
		break;

	case CMD_SERVER_LOGOUT_FAILURE:
		TRACE0("CMD_SERVER_LOGOUT_ACK\n");
		numread=Receive((void*)p1,sizeof(PACK_LOGOUT_FAILURE)-sizeof(unsigned short));
		m_pClient->OnLogoutAck(p);
		break;

	case CMD_SERVER_PASSWORD_MODIFICATION_ACK:
		TRACE0("CMD_SERVER_PASSWORD_MODIFICATION_ACK\n");
        numread=Receive((void*)p1,sizeof(PACK_PASSWORD_MODIFICATION_ACK)-sizeof(unsigned short));
		m_pClient->OnModifyUserInfoAck(p);
		break;

	//CutSheet FTP 
	case CMD_SERVER_PAPER_REQUEST_SUCCESS:
	case CMD_SERVER_PAPER_REQUEST_FAILURE:
		TRACE0("OnReive() :CMD_SERVER_PAPER_REQUEST_SUCCESS\n");
		numread=Receive((void*)p1,sizeof(PACK_SERVER_PAPER_REQUEST_ACK)-sizeof(unsigned short));
		m_pClient->OnServerFTPAck(p);
		break;

	case CMD_SERVER_PAPER_FTP:
		{
			TRACE0("OnReive() :CMD_SERVER_PAPER_FTP\n");
			numread=Receive((void*)p1,sizeof(PACK_SERVER_PAPER_FTP)-sizeof(unsigned short)-1);
			p1+=(sizeof(PACK_SERVER_PAPER_FTP)-sizeof(unsigned short)-1);
			pFTP=(LPPACK_SERVER_PAPER_FTP)&p;
			numread=Receive((void*)p1,pFTP->nLen); 
			m_pClient->OnServerFTPAck(p);
			break;
		}
	//服务端对客户端试卷提交包的反馈
	case CMD_SERVER_PAPER_SUBMIT_ACK:
		{
            TRACE0("OnReive() :CMD_SERVER_PAPER_SUBMIT_ACK\n");
            numread=Receive((void*)p1,sizeof(PACK_PAPER_SUBMIT_ACK)-sizeof(unsigned short)); 
			m_pClient->OnSubmitPaperAck(p);
			break;
		}
	case CMD_SERVER_HEARTBEAT:
		{
            TRACE0("OnReive() :CMD_SERVER_HEARTBEAT\n");
			m_pClient->OnHeartBeatRecv(p);
			break;
		}
	case CMD_SERVER_NOTIFICATION:
		{
            TRACE0("OnReive() :CMD_SERVER_NOTIFICATION\n");
			numread=Receive((void*)p1,sizeof(PACK_NOTIFICATION)-sizeof(unsigned short));
			m_pClient->OnServerNotify(p);
			break;
		}
	default:
		{
			TRACE0("OnRecv():Unknown command\n");
			TRACE1("Unknown command:0x%x\n",numread);
			break;
		}
	}
	//Update Status
	if (numread == SOCKET_ERROR)
		{
			int nError=GetLastError();
			if (nError==WSAENOTCONN)
			{
				//Not yet connected
				AsyncSelect(FD_READ | FD_CLOSE);
				return;
			}
			if (nError!=WSAEWOULDBLOCK)
			{
				Close();
				//if (!m_bSentClose)
				//{
				//	m_nMode |= CSMODE_TRANSFERERROR;
				//	m_bSentClose = TRUE;
				//	m_pOwner->m_pOwner->PostThreadMessage(m_nInternalMessageID, FZAPI_THREADMSG_TRANSFEREND, m_nMode);
				//}
			}
		}
	AsyncSelect(FD_READ | FD_CLOSE|FD_WRITE);
}

void CTransferSocket::OnConnect(int nErrorCode)
{
	if (nErrorCode)
	{
		TCHAR buffer[1000];
		memset(buffer,0,1000);
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, nErrorCode, 0, buffer, 999, 0);
		TRACE1("Connect failed:%s\n",buffer);
		Close();
		m_pClient->OnNetworkConnected(FALSE);
	}
	else
	{
		/* Set internal socket send buffer to twice the programs buffer size
		 * this should fix the speed problems some users have reported
		*/
		DWORD value;
		int len = sizeof(value);
		GetSockOpt(SO_SNDBUF, &value, &len);
		if (value < (MSG_BUFFER_SIZE))
		{
			value = MSG_BUFFER_SIZE ;
			SetSockOpt(SO_SNDBUF, &value, sizeof(value));
		}
		m_pClient->m_bConnected=TRUE;
        m_pClient->OnNetworkConnected(TRUE); 	
	}
}


void CTransferSocket::OnClose(int nErrorCode)
{
	Close();
	TRACE0("Connection Disconnect!\n");
	m_pClient->OnNetworkDisconnect(FALSE);
}

void CTransferSocket::OnSend(int nErrorCode)
{
   if (GetState() != connected && GetState() != attached && GetState() != closed)
		return;
   if(NULL==m_pClient)
	    return;

   AsyncSelect(FD_READ|FD_CLOSE); 

   //Prepare packet
   LPPACK_HEADER p=m_pClient->GetMsgToSend();
   if(NULL==p)
   {
	   return;
   }

   //Send the packet
   int numSend=0;
   LPPACK_PAPER_SUBMIT pPackSubmit=NULL;
   int                 nBytesToSend=0;
   switch(p->cmd)
   {
   case CMD_CLIENT_LOGIN:
   	  TRACE0("CMD_CLIENT_LOGIN\n");
	   numSend=Send((void*)p,sizeof(PACK_LOGIN));
	   break;
   case CMD_CLIENT_LOGOUT:
   	   TRACE0("CMD_CLIENT_LOGOUT\n");
	   numSend=Send((void*)p,sizeof(PACK_LOGOUT));
	   break;
   case CMD_CLIENT_PAPER_SUBMIT:
	   {
		   //变长度的包，特殊处理
		   TRACE0("CMD_CLIENT_PAPER_SUBMIT\n");
		   pPackSubmit=(LPPACK_PAPER_SUBMIT)p;
           nBytesToSend=sizeof(PACK_PAPER_SUBMIT);
		   nBytesToSend+=(pPackSubmit->nSubmitCnt-1)*sizeof(PACK_PAPER_SUBMIT_ITEM); 
		   numSend=Send((void*)p,nBytesToSend);
		   break;
	   }
   case CMD_CLIENT_PASSWORD_MODIFICATION:
   	   TRACE0("CMD_CLIENT_PASSWORD_MODIFICATION\n");
	   numSend=Send((void*)p,sizeof(PACK_PASSWORD_MODIFICATION));
	   break;
   case CMD_CLIENT_PAPER_REQUEST:
   	   TRACE0("CMD_CLIENT_PAPER_REQUEST\n");
	   numSend=Send((void*)p,sizeof(PACK_PAPER_REQUEST));
	   break;
   case CMD_CLIENT_PAPER_TRANSFER_ACK:
	   //TRACE0("CMD_CLIENT_PAPER_TRANSFER_ACK\n");
	   numSend=Send((void*)p,sizeof(PACK_CLIENT_PAPER_TRANSFER_ACK));
	   //TRACE1("numSend=%d\n",numSend);
	   break;
   case CMD_CLIENT_HEARTBEAT_ACK:
       numSend=Send((void*)p,sizeof(PACK_HEADER));
	   break;
   default:
	   {
		   TRACE1("OnSend() Unknown Command:0x%X\n",p->cmd);		   
		   break;	   
	   }
   }//End    switch(p->cmd)

   //回收资源
   CEmarkNetMessageManager *pMgr=CEmarkNetMessageManager::GetManager();
   pMgr->InsertMessage(p);

   if(numSend == SOCKET_ERROR)
   {
	   /* Give control back to the system. Else FileZilla would 
	   * consume most CPU power until the data was sent.
	   * This behaviour is very strange, and I can't explain it,
	   * maybe it's just another bug in Windows
	   */
	   Sleep(0);

	   int nError = GetLastError();
	   if (nError == WSAENOTCONN)
	   {
		   //Not yet connected
		   return;
	   }
	   if (nError != WSAEWOULDBLOCK)
	   {
		   if (ShutDown() || GetLastError() != WSAEWOULDBLOCK)
		   {
			   Close();
			   //if (!m_bSentClose)
			   //{
			   //	   m_nMode |= CSMODE_TRANSFERERROR;
			   //	   m_bSentClose = TRUE;
			   //	   m_pOwner->m_pOwner->PostThreadMessage(m_nInternalMessageID, FZAPI_THREADMSG_TRANSFEREND, m_nMode);
			   //  }
		   }
	   }//End if (nError != WSAEWOULDBLOCK)
   }//End  if(numSend == SOCKET_ERROR)


}//End OnSend()
#endif