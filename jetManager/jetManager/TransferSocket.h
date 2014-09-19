/*++
[Module]
         TransferSocket.h
[Description]
         负责网络的通讯模块。
--*/
#ifndef __EMARK_CLIENT_TRANSFER_SOCKET_H__
#define __EMARK_CLIENT_TRANSFER_SOCKET_H__
#include "AsyncSocketEx.h"

class CeMarkNetClient;
class CTransferSocket:public CAsyncSocketEx
{
//Methods:
public:
   CTransferSocket(CeMarkNetClient *pClient);
   ~CTransferSocket();

	//{{AFX_VIRTUAL(CTransferSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	//}}AFX_VIRTUAL
private:
	CeMarkNetClient *m_pClient;

};
#endif //__EMARK_CLIENT_TRANSFER_SOCKET_H__