#pragma once
#include "HWNDScreen.h"
#include "CgxMonster.h"
#include "CgxPlayerCommand.h"
#include "CgxPetCommandWindow.h"
#include "CgxSkillWindow.h"
#include "CgxTopRightWindow.h"
#include "CgxMapWindow.h"

class CGame
{
public:

	enum GAME_STATUS {
		UNKNOW, TRAVEL, PLAYER_FIGHT, PLAYER_CHOISE_SKILL,
		PLAYER_CHOISE_SKILL_LEVEL, PET_FIGHT, PET_CHOSE_SKILL, FIGHTING
	};

	CGame(HWND);
	virtual ~CGame();

	BOOL CGame::refresh(void);

	GAME_STATUS getStatus(void);

	CCgxMonster* monster;
	CCgxPlayerCommand* playerCommandWindow;
	CCgxPetCommandWindow* petCommandWindow;
	CCgxSkillWindow* playerSkillWindow;
	CCgxSkillWindow* playerSkillLevelWindow;
	CCgxSkillWindow* petSkillWindow;
	CCgxTopRightWindow* topRightWindow;
	CCgxMapWindow* mapWindow;

private:
	HWND hwnd;
	CHWNDScreen* pScreen;
	
public:
	HWND getHWND(void);
	CHWNDScreen* getScreen(void);
};


