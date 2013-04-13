#include "stdafx.h"
#include "Game.h"
#include "resource.h"

CGame::CGame(HWND hwnd)
{
	this->hwnd = hwnd;
	pScreen = new CHWNDScreen(hwnd);

	monster = new CCgxMonster(pScreen);
	playerCommandWindow = new CCgxPlayerCommand(pScreen);
	petCommandWindow = new CCgxPetCommandWindow(pScreen);
	playerSkillWindow = new CCgxSkillWindow(IDB_PLAYER_SKILL, pScreen);
	playerSkillLevelWindow = new CCgxSkillWindow(IDB_SKILL_LV, pScreen);
	petSkillWindow = new CCgxSkillWindow(IDB_PET_SKILL, pScreen);
	topRightWindow = new CCgxTopRightWindow(pScreen);
}

CGame::~CGame()
{
	if(pScreen) delete pScreen;
	delete monster;
	delete playerCommandWindow;
	delete petCommandWindow;
	delete playerSkillWindow;
	delete playerSkillLevelWindow;
	delete petSkillWindow;
	delete topRightWindow;
}

void CGame::refresh(void)
{
	if(GetForegroundWindow() == hwnd)
		pScreen->refresh();
}

CGame::GAME_STATUS CGame::getStatus(void)
{
	refresh();
	if(topRightWindow->locate())
		return TRAVEL;
	else if(playerSkillLevelWindow->locate())
		return PLAYER_CHOISE_SKILL_LEVEL;
	else if(playerSkillWindow->locate())
		return PLAYER_CHOISE_SKILL;
	else if(playerCommandWindow->locate())
		return PLAYER_FIGHT;
	else if(petSkillWindow->locate())
		return PET_CHOSE_SKILL;
	else if(petCommandWindow->locate())
		return PET_FIGHT;
	return UNKNOW;
}

HWND CGame::getHWND(void)
{
	return hwnd;
}
