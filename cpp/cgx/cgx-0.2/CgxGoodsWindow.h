#pragma once
#include "CgxWindow.h"

class CCgxGoodsWindow :
	public CCgxWindow
{
public:
	CCgxGoodsWindow(CHWNDScreen*);
	~CCgxGoodsWindow(void);

	void getLocateRECT(RECT* rectOut);
	void locateCommands(void);
};

