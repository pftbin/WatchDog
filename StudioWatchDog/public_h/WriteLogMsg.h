#ifndef _WRITELOGMSG_H
#define _WROTELOGMSG_H
//#include "ILoger.h"

void InitLogMsg(HANDLE hFile);

void AddLogMsg(TCHAR tMsg[],DWORD dwType=0);

void AddLogMsgByChar(char tMsg[],DWORD dwType=0);

void CloseLogMsg();

//ILoger* CreateLoger(void);

#endif