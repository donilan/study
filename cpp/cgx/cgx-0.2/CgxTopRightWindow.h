#pragma once
#include "cgxwindow.h"
#include "HWNDScreen.h"
class CCgxTopRightWindow :
	public CCgxWindow
{
public:
	CCgxTopRightWindow(CHWNDScreen*);
	~CCgxTopRightWindow(void);
	void getLocateRECT(RECT* rectOut);
	void locateCommands( void );
	BOOL isCommandEnable(int index);
	void openMap();
};

