#pragma once

#include "Game.h"
#include <atlimage.h>
#include "GameAI.h"

#define MAX_CROSS_GATE 10
#define CROSS_GATE_CN TEXT("\u9b54\u529b\u5b9d\u8d1d")

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

