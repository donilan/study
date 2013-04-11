#pragma once
#include "HWNDScreen.h"

class CGame
{
public:
	CGame(HWND);
	virtual ~CGame();

	BOOL refresh(void);

private:
	HWND hwnd;
	CHWNDScreen* pScreen;
};


