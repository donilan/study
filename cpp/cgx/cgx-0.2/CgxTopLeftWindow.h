#pragma once
#include "cgxwindow.h"
class CCgxTopLeftWindow :
	public CCgxWindow
{
public:
	CCgxTopLeftWindow(CHWNDScreen* pScreen);
	~CCgxTopLeftWindow(void);
	void locateCommands(void);
	void getLocateRECT(RECT* rectOut);
	int getHP(void);
	int getHPOnFighting(void);
	int getMP(void);
	int getMPOnFighting(void);
};

