#pragma once

#include "Game.h"
#include <atlimage.h>
#include "GameAI.h"



BOOL CALLBACK FindCrossGateWindowsProc (HWND hwnd, LPARAM lParam);

class CGameManager
{
public:
	CGameManager(void);
	~CGameManager(void);

	CGameAI* games[MAX_CROSS_GATE];
	int gameSize;

	BOOL init(void);

	BOOL refresh(void);

};

