#pragma once
#include "CgxWindow.h"
#include "HWNDScreen.h"

class CCgxPetCommandWindow :
	public CCgxWindow
{
public:
	CCgxPetCommandWindow(CHWNDScreen* );
	~CCgxPetCommandWindow(void);
	void locateCommands(void);
	BOOL isCommandEnable(int);
	void clickSkillCommand(void);
};

