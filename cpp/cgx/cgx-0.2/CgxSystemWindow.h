#pragma once
#include "cgxwindow.h"

class CCgxSystemWindow :
	public CCgxWindow
{
public:
	CCgxSystemWindow(CHWNDScreen*);
	~CCgxSystemWindow(void);

	void locateCommands(void);

	void clickBackToCity();
};

