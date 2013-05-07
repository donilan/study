#pragma once
#include "cgxwindow.h"
class CCgxOfflineWindow :
	public CCgxWindow
{
public:
	CCgxOfflineWindow(CHWNDScreen*);
	~CCgxOfflineWindow(void);
	void getLocateRECT(RECT* rectOut);
};

