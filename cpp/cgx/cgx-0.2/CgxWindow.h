#pragma once

#include "HWNDScreen.h"

class CCgxWindow
{
public:
	CCgxWindow(UINT, CHWNDScreen*);
	~CCgxWindow(void);

	CImage* pLocateImage;
	RECT rect;
protected:
	CHWNDScreen* screen;
	
public:
	BOOL locate(void);
	BOOL isPositionChanged(void);
};

