// WriteMsgToFile.h: interface for the WriteMsgToFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WRITEMSGTOFILE_H__169710FA_C9F5_41FB_973D_6A84CDD2AB6D__INCLUDED_)
#define AFX_WRITEMSGTOFILE_H__169710FA_C9F5_41FB_973D_6A84CDD2AB6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef WRITELOG_EXPORTS
#define WRITELOG_CLASS __declspec(dllexport)
#else
#define WRITELOG_CLASS __declspec(dllimport)
#endif

//#include "../public_h/AirBaseDefine.h"

typedef struct _LOGINFO
{
	CString strInfo;
	int nLogLevel;
	int nErrorCode;
	BOOL bAsync;
	BOOL bSnmpLog;
	_LOGINFO()
	{
		nLogLevel=0;
		nErrorCode=0;
		bAsync=FALSE;
		bSnmpLog=TRUE;
	}
}LOGINFO,*PLOGINFO;

class WRITELOG_CLASS CWriteMsgToFile  
{
public:
	BOOL WriteSnmp(int nAction,LPVOID pData);
	void SetLogParam(CString strPath,CString strName);
	void WriteLog(CString strInfo,int nType,BOOL bAsync=FALSE,BOOL bSnmpLog = FALSE);
	void WriteLogInfo(LOGINFO loginfo);
	void Initialize();
	void WriteLogEx(CString strInfo,int nType,CString strUserName,BOOL bAsync=FALSE,BOOL bSnmpLog = FALSE);
	CWriteMsgToFile();
	virtual ~CWriteMsgToFile();

public:
//	void WriteToFile(CString strMsg, DWORD dwType);
//	CString GetMsg(DWORD& dwType);
//	void AddMsg(CStringArray *pMsgArr, CDWordArray* pdwArray);
//	void AddMsg(CString strMsg, DWORD nType);
//	HANDLE GetOpenFileHandle();
//	void SetOpenFileHandle(HANDLE hFile);
//	void ThreadProc();

//	HANDLE m_hJMFile;	
protected:
	class CWriteAction * m_pLogWriter;
private:
	static int _instance;
};

inline BOOL WriteLog(CWriteMsgToFile& loger, DWORD dwType, BOOL bAsync, LPCTSTR lpszFormat, ...)
{
	CString strLog;
	va_list argList;
	va_start(argList, lpszFormat);
	strLog.FormatV(lpszFormat, argList);
	va_end(argList);
	loger.WriteLog(strLog, dwType, bAsync);
	return FALSE;
}

inline void WriteLogInfo(CWriteMsgToFile& loger,int nLevel,int nErrorCode,LPCTSTR lpszFormat, ...)
{
	CString strLog;
	va_list argList;
	va_start(argList, lpszFormat);
	strLog.FormatV(lpszFormat, argList);
	va_end(argList);
	LOGINFO loginfo;
	loginfo.nErrorCode=nErrorCode;
	loginfo.nLogLevel=nLevel;
	loginfo.strInfo=strLog;
	loger.WriteLogInfo(loginfo);
}

inline BOOL WriteLogEx(CWriteMsgToFile& loger, DWORD dwType, BOOL bAsync, LPCTSTR lpszUserName, LPCTSTR lpszFormat, ...)
{
	CString strLog;
	va_list argList;
	va_start(argList, lpszFormat);
	strLog.FormatV(lpszFormat, argList);
	va_end(argList);
	loger.WriteLogEx(strLog, dwType, lpszUserName, bAsync);
	return FALSE;
}

#define DECLARE_LOGER(loger) \
	extern CWriteMsgToFile loger;

#define IMPLEMENT_LOGER(loger) \
	CWriteMsgToFile loger;

#define INIT_LOGER(loger, folder, name) \
	loger.SetLogParam(folder, name);

#define WRITE_LOG	WriteLog

#define WRITE_LOGINFO WriteLogInfo

#define WRITE_LOGEX WriteLogEx	//包含用户名称信息

#endif // !defined(AFX_WRITEMSGTOFILE_H__169710FA_C9F5_41FB_973D_6A84CDD2AB6D__INCLUDED_)
