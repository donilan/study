#pragma once
#include "cgxwindow.h"
class CCgxBottomWindow :
	public CCgxWindow
{
public:
	CCgxBottomWindow(CHWNDScreen* pScreen);
	~CCgxBottomWindow(void);
	void getLocateRECT(RECT* rectOut);
	void locateCommands( void );
};

