
// SvcMgr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSvcMgrApp:
// �йش����ʵ�֣������ SvcMgr.cpp
//

class CSvcMgrApp : public CWinApp
{
public:
	CSvcMgrApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	BOOL LimitedInst(void);
};

extern CSvcMgrApp theApp;