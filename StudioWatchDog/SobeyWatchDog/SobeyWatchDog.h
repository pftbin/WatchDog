
// SobeyWatchDog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSobeyWatchDogApp:
// �йش����ʵ�֣������ SobeyWatchDog.cpp
//

class CSobeyWatchDogApp : public CWinAppEx
{
public:
	CSobeyWatchDogApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSobeyWatchDogApp theApp;