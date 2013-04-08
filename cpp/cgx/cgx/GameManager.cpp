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
#ifdef DEBUG
		if(gameSize < 1)
		{ 
			games[0] = new CGame(NULL);
			gameSize = 1;
		}
#endif
		return TRUE;
	}

	return FALSE;
}

BOOL CGameManager::refresh( void )
{
	return FALSE;
}
