
// digitRec.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CdigitRecApp:
// �йش����ʵ�֣������ digitRec.cpp
//

class CdigitRecApp : public CWinApp
{
public:
	CdigitRecApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CdigitRecApp theApp;