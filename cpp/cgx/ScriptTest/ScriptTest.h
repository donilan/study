
// ScriptTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CScriptTestApp:
// �йش����ʵ�֣������ ScriptTest.cpp
//

class CScriptTestApp : public CWinApp
{
public:
	CScriptTestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CScriptTestApp theApp;