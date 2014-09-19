// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ�������ִ�е�
// ��Ŀ�ض��İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ��ͷ���ų�����ʹ�õ�����
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨������ִ������Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ�� Windows 95 �� Windows NT 4 ����߰汾���ض����ܡ�
#define WINVER 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾ִ�б�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ�� Windows NT 4 ����߰汾���ض����ܡ�
#define _WIN32_WINNT 0x0501		//Ϊ Windows98 �� Windows 2000 �����°汾ִ�б�Ϊ�ʵ���ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ�� Windows 98 ����߰汾���ض����ܡ�
#define _WIN32_WINDOWS 0x0501 //Ϊ Windows Me �����°汾ִ�б�Ϊ�ʵ���ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ�� IE 4.0 ����߰汾���ض����ܡ�
#define _WIN32_IE 0x0501	//Ϊ IE 5.0 �����°汾ִ�б�Ϊ�ʵ���ֵ��
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯��������ʽ��

#include <afxsock.h>		// MFC socket extensions
#include <afxtempl.h>		//�����������ݽṹ����֧��

#define	_TEST_FLAG

#include <atlimage.h>		// CImage

#include <afxwin.h>         // MFC ���ĺͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdisp.h>        // MFC �Զ�����

#include <afxdtctl.h>		// Internet Explorer 4 �����ؼ��� MFC ֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows �����ؼ��� MFC ֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "GUINetworkInterface.h"
//#include "GUINetworkMsgManager.h"

//����socket�����ݿ⽻�������ݽṹ
typedef struct _JOB_INFO
{
	int		nJobID;
	float	fTotalScore;					//д���ݿ�ʱΪjob�������ͽ��ʱΪ������ҵ��
	float   fOldScore;
}JOB_INFO;

//#define	TEST_VERSION

// �ر� MFC ��ĳЩ��������������ȫ���Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS
#pragma warning(disable:4244 4101 4267 4312)

//���Կ���
//������ײ�����ͨѶ�ĵ�����Ϣ
#define SWITCH_GUI_DEBUG_ON
//�ײ����������ͨѶ�ĵ�����Ϣ
#define SWITCH_NETWORK_DEBUG_ON
//�ڴ�������ͷŵļ��
#define SWITCH_MEMORY_DEBUG_ON
//�ͻ��˺ͷ����ͨѶ��Ϣ----����˷���������Ϣ
#define SWITCH_SERVER_IN_DEBUG_ON
//�ͻ��˺ͷ���˵�ͨѶ��Ϣ----�ͻ��˷���ȥ����Ϣ
#define SWITCH_SERVER_OUT_DEBUG_ON

#define	_NEW_ARCHITECTURE