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
	topLeftWindow = new CCgxTopLeftWindow(pScreen);
	mapWindow = new CCgxMapWindow(pScreen);
	goodsWindows = new CCgxGoodsWindow(pScreen);
	bottomWindow = new CCgxBottomWindow(pScreen);
	systemWindow = new CCgxSystemWindow(pScreen);
	offlineWindow = new CCgxOfflineWindow(pScreen);
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
	delete topLeftWindow;
	delete goodsWindows;
	delete bottomWindow;
	delete systemWindow;
	delete offlineWindow;
}

BOOL CGame::refresh(void)
{
	
	if(GetForegroundWindow() == hwnd)
	{
		TRACE("Game window is focus, so refresh.\n");
		pScreen->refresh();
		return TRUE;
	}
	else {
		TRACE("Game window is not focus, so no refresh.\n");
	}
	return FALSE;
}

CGame::GAME_STATUS CGame::getStatus(void)
{
	if(!refresh())
		return UNKNOW;

	if(playerCommandWindow->isExists())
	{
		if(playerSkillLevelWindow->isExists())
		{
			TRACE("Not TRAVEL status.\n");
			return PLAYER_CHOISE_SKILL_LEVEL;
		}
		else if(playerSkillWindow->isExists())
		{
			TRACE("Not PLAYER_CHOISE_SKILL_LEVEL status.\n");
			return PLAYER_CHOISE_SKILL;
		}
		TRACE("Not PLAYER_CHOISE_SKILL status.\n");
		return PLAYER_FIGHT;
	}
	else if(petCommandWindow->isExists())
	{
		if(petSkillWindow->isExists())
		{
			TRACE("Not PLAYER_FIGHT status.\n");
			return PET_CHOSE_SKILL;
		}
		return PET_FIGHT;
	}
	else if(topRightWindow->isExists())
		return TRAVEL;
	return UNKNOW;
}

HWND CGame::getHWND(void)
{
	return hwnd;
}



CHWNDScreen* CGame::getScreen(void)
{
	return this->pScreen;
}

BOOL CGame::isOffline()
{
	return offlineWindow->locate();
}

void CGame::setHWND( HWND _hwnd)
{
	this->hwnd = _hwnd;
	this->pScreen->hwnd = _hwnd;
}
