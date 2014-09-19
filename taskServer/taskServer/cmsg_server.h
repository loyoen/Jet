#ifndef CMSG_SERVER_H
#define CMSG_SERVER_H
#define		BLUE		1
#define		RED		2
#define		GREEN	3
#define		YELLOW	4

#include "MgrMessage.h"

//#define	CMSG_Server	CMgrMessage

class CMSG_Server : public CMgrMessage
{
public:
	void	AddMessage ( char *strMessage, int size, int color );

	//CDialog *pDlg;
	//CMSG_Server(void *myDC);
	CMSG_Server();
	static CMSG_Server *pCMSG_Server;

public:
	static CMSG_Server*	GetCMSG_Server();
};


#endif