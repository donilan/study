
// cgx-0.2.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CCgxApp:
// 有关此类的实现，请参阅 cgx-0.2.cpp
//

class CCgxApp : public CWinApp
{
public:
	CCgxApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
//	void leftClickCenter(void);
};

extern CCgxApp theApp;