#ifndef CMSG_SERVER_H
#define CMSG_SERVER_H
#define		BLUE		1
#define		RED		2
#define		GREEN	3
#define		YELLOW	4
class CMSG_Server
{
private:
	//CDialog *pDlg;
	//CMSG_Server(void *myDC);
	CMSG_Server();
	static CMSG_Server *pCMSG_Server;
public:
	static CMSG_Server*	GetCMSG_Server();
	void AddMessage(char *strMessage,int size,int color);
};

#endif