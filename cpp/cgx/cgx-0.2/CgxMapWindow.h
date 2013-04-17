#pragma once
#include "cgxwindow.h"
#include "HWNDScreen.h"
class CCgxMapWindow :
	public CCgxWindow
{
public:
	CCgxMapWindow(CHWNDScreen* );
	~CCgxMapWindow(void);
	void locateCommands(void);

	RECT xRECT;
	RECT yRECT;
	int getX(void);
	int getY(void);
};

