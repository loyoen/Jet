// jetManager.h : jetManager Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������

//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////

// CjetManagerApp:
// �йش����ʵ�֣������� jetManager.cpp
//

class CjetManagerApp : public CWinApp
{
public:
	CjetManagerApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance() ;

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CjetManagerApp theApp;
