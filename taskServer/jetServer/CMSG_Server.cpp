#include "stdafx.h"
#include "CMSG_Server.h"
#include "jetServer.h"

void	*pMainDlg;

CMSG_Server::CMSG_Server()
{
	//pDlg = (CDialog*)myDC;
}

CMSG_Server* CMSG_Server::pCMSG_Server = NULL;

void CMSG_Server::AddMessage(char *strMessage, int size, int color)
{
	CDialog *pDlg = (CDialog*)pMainDlg;
	::PostMessageA(pDlg->GetSafeHwnd(),ID_MSG_STATE,0,(LPARAM)strMessage);
}
CMSG_Server* CMSG_Server::GetCMSG_Server()
{
	if(NULL == pCMSG_Server)
	{
		pCMSG_Server = new CMSG_Server();
		return pCMSG_Server;
	}
	else
		return pCMSG_Server;

}