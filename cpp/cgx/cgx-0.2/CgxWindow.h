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
	CHWNDScreen* pScreen;
	
public:
	BOOL locate(void);
	BOOL isExists(void);
	virtual void locateCommands(void){memset(commandRECTs, 0, sizeof(RECT)*MAX_COMMAND);};
	virtual BOOL isCommandEnable(int){return FALSE;};
	BOOL getCommand(int index, RECT* rectOut);
	void leftClick(int index);
	int getCommandSize(void);
	void centerXY(int* centerX, int* centerY);
	void getLocateRECT(RECT* rectOut);
};

