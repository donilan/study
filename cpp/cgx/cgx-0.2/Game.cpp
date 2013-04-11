#include "stdafx.h"
#include "Game.h"

CGame::CGame(HWND hwnd)
{
	this->hwnd = hwnd;
	pScreen = new CHWNDScreen(hwnd);
}

CGame::~CGame()
{
	if(pScreen) delete pScreen;
}

BOOL CGame::refresh(void)
{
	if(pScreen) delete pScreen;
	pScreen = new CHWNDScreen(hwnd);
	return TRUE;
}
