#pragma once
#include "CgxWindow.h"
#include "HWNDScreen.h"
class CCgxPlayerCommand :
	public CCgxWindow
{
public:
	CCgxPlayerCommand(CHWNDScreen*);
	~CCgxPlayerCommand(void);
	void locateCommands(void);
	BOOL isCommandEnable(int index);
	void clickHitCommand(void);
	void clickSkillCommand(void);
	void clickPetCommand(void);
	void clickDefenseCommand(void);
	void clickGoodsCommand(void);
	void clickPositionCommand(void);
	void clickRunawayCommand(void);
	void getLocateRECT(RECT* rectOut);
};

