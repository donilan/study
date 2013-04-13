#include "StdAfx.h"
#include "GameManager.h"

BOOL CALLBACK FindCrossGateWindowsProc (HWND hwnd, LPARAM lParam)
{
	TCHAR buff[MAX_PATH] = {0};
	CGameManager* gameManager = (CGameManager *)lParam;

	GetWindowText(hwnd, buff, MAX_PATH);
	if(_tcsncmp(buff, CROSS_GATE_CN, lstrlen(CROSS_GATE_CN)) == 0)
	{
		gameManager->games[gameManager->gameSize++] = new CGame(hwnd);
	}
	return TRUE;
}

CGameManager::CGameManager(void)
{
	for(int i = 0; i < MAX_CROSS_GATE; ++i)
	{
		this->games[i] = NULL;
	}
	this->gameSize = 0;
	this->init();
}


CGameManager::~CGameManager(void)
{
}

BOOL CGameManager::init( void )
{
	if(EnumWindows(FindCrossGateWindowsProc, (LPARAM)this))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CGameManager::refresh( void )
{
	CGame* oldGames[MAX_CROSS_GATE];
	CGame* tmpGame = NULL;
	for(int i = 0; i < MAX_CROSS_GATE; ++i)
	{
		oldGames[i] = this->games[i];
		this->games[i] = NULL;
	}
	this->gameSize = 0;
	if(EnumWindows(FindCrossGateWindowsProc, (LPARAM)this))
	{
		TRACE("Refreshing games.\n");
	}
	for(int i = 0; i < MAX_CROSS_GATE; ++i)
	{
		tmpGame = this->games[i];
		if(tmpGame)
		{
			for(int i2 = 0; i2 < MAX_CROSS_GATE; ++i2)
			{
				if(oldGames[i2])
				{
					if(tmpGame->getHWND() == oldGames[i2]->getHWND())
					{
						TRACE("Found an old game.\n");
						games[i] = oldGames[i2];
						delete tmpGame;
						oldGames[i2] = NULL;
						break;
					}
				}
			}
		}
	}
	for(int i = 0; i < MAX_CROSS_GATE; ++i)
	{
		if(oldGames[i] != NULL)
			delete oldGames[i];
	}
	return TRUE;
}
