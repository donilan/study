#pragma once
#include "cgxwindow.h"
#include "HWNDScreen.h"

class CCgxSkillWindow :
	public CCgxWindow
{
public:
	CCgxSkillWindow(int, CHWNDScreen*);
	~CCgxSkillWindow(void);
	// Find skill rect, start index is 1
	//void skillRECT(INT index, RECT* skillRECTOut);
	void locateCommands(void);
	BOOL isCommandEnable(int);
	void getLocateRECT(RECT* rectOut);
};

