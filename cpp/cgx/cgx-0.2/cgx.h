
// cgx-0.2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCgxApp:
// �йش����ʵ�֣������ cgx-0.2.cpp
//

class CCgxApp : public CWinApp
{
public:
	CCgxApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
//	void leftClickCenter(void);
};

extern CCgxApp theApp;