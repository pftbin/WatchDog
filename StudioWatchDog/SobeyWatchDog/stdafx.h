
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
#include <map>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#define STATUS_PROCESS_NOWATCH  0
#define STATUS_PROCESS_IDLE		1
#define STATUS_PROCESS_STARTING 2
#define STATUS_PROCESS_RUNNING  3

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

typedef struct _tagPROCESSINFO
{
	CString		strName;
	CString     strLocation;
	DWORD		dwProcessID;
	int			nStatus;
	int         nAbnormalCount;

	_tagPROCESSINFO()
	{
		strName		= _T("");
		strLocation = _T("");
		dwProcessID = 0;
		nStatus		= STATUS_PROCESS_NOWATCH;
		nAbnormalCount = 0;
	}

	const _tagPROCESSINFO operator=(const _tagPROCESSINFO& Info)
	{
		strName		= Info.strName;
		strLocation = Info.strLocation;
		dwProcessID = Info.dwProcessID;
		nStatus		= Info.nStatus;
		nAbnormalCount = Info.nAbnormalCount;
		return *this;
	}
}PROCESSINFO, *PPROCESSINFO;

inline CString GetAppFolder(void)
{
	TCHAR exeFullName[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(), exeFullName, MAX_PATH);
	CString mPath = exeFullName;

	int mPos = mPath.ReverseFind('\\');

	if (mPos == -1)return CString(_T(""));

	mPath = mPath.Left(mPos);

	if (mPath.Right(1) == _T("\\"))
	{
		mPath = mPath.Left(mPos - 1);
	}

	return mPath;
}
#include "WriteMsgToFile.h"
#pragma comment(lib, "..\\lib\\WriteLog.lib")
DECLARE_LOGER(LogerWatchDog)