#pragma once

#include "HWNDScreen.h"

#define MAX_COMMAND 20

class CCgxWindow
{
public:
	CCgxWindow(UINT, CHWNDScreen*);
	~CCgxWindow(void);

	CImage* pLocateImage;
	RECT rect;
	RECT commandRECTs[MAX_COMMAND];
protected:
	CHWNDScreen* screen;
	
public:
	BOOL locate(void);
	BOOL isPositionChanged(void);
	virtual void locateCommands(void){memset(commandRECTs, 0, sizeof(RECT)*MAX_COMMAND);};
	BOOL getCommand(int index, RECT* rectOut);
	void leftClick(int index);
};

