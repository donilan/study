#pragma once
#include "CgxWindow.h"
#include "HWNDScreen.h"
class CCgxPlayerCommand :
	public CCgxWindow
{
public:
	CCgxPlayerCommand(CHWNDScreen*);
	~CCgxPlayerCommand(void);
	void locateCommands(void);
	BOOL isCommandEnable(int index);
};

