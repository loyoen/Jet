// jetServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CjetServerApp:
// �йش����ʵ�֣������ jetServer.cpp
//

class CjetServerApp : public CWinApp
{
public:
	CjetServerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CjetServerApp theApp;

extern void	*pMainDlg;