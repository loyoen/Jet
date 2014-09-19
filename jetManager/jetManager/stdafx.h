// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更执行的
// 项目特定的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 标头中排除不常使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修执行下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用 Windows 95 和 Windows NT 4 或更高版本的特定功能。
#define WINVER 0x0501		//为 Windows98 和 Windows 2000 及更新版本执行变为适当的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用 Windows NT 4 或更高版本的特定功能。
#define _WIN32_WINNT 0x0501		//为 Windows98 和 Windows 2000 及更新版本执行变为适当的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用 Windows 98 或更高版本的特定功能。
#define _WIN32_WINDOWS 0x0501 //为 Windows Me 及更新版本执行变为适当的值。
#endif

#ifndef _WIN32_IE			// 允许使用 IE 4.0 或更高版本的特定功能。
#define _WIN32_IE 0x0501	//为 IE 5.0 及更新版本执行变为适当的值。
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将是显式的

#include <afxsock.h>		// MFC socket extensions
#include <afxtempl.h>		//用作下面数据结构数组支持

#define	_TEST_FLAG

#include <atlimage.h>		// CImage

#include <afxwin.h>         // MFC 核心和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdisp.h>        // MFC 自动化类

#include <afxdtctl.h>		// Internet Explorer 4 公共控件的 MFC 支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 公共控件的 MFC 支持
#endif // _AFX_NO_AFXCMN_SUPPORT

//#include "GUINetworkInterface.h"
//#include "GUINetworkMsgManager.h"

//定义socket与数据库交互的数据结构
typedef struct _JOB_INFO
{
	int		nJobID;
	float	fTotalScore;					//写数据库时为job总数，送结果时为机器作业数
	float   fOldScore;
}JOB_INFO;

//#define	TEST_VERSION

// 关闭 MFC 对某些常见但经常被安全忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS
#pragma warning(disable:4244 4101 4267 4312)

//测试开关
//界面向底层网络通讯的调试信息
#define SWITCH_GUI_DEBUG_ON
//底层网络向界面通讯的调试信息
#define SWITCH_NETWORK_DEBUG_ON
//内存申请和释放的监控
#define SWITCH_MEMORY_DEBUG_ON
//客户端和服务端通讯信息----服务端发过来的信息
#define SWITCH_SERVER_IN_DEBUG_ON
//客户端和服务端的通讯信息----客户端发出去的信息
#define SWITCH_SERVER_OUT_DEBUG_ON

#define	_NEW_ARCHITECTURE