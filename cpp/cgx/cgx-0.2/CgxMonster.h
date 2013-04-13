#pragma once

#include "HWNDScreen.h"

#define NUMBER_OF_MONSTER 10

class CCgxMonster
{
public:
	CCgxMonster(CHWNDScreen*);
	~CCgxMonster(void);

	RECT monsterRECTs[NUMBER_OF_MONSTER];
private:
	CHWNDScreen* pScreen;
	
public:
	// Find alive
	BOOL isAlive(int);
	int countAlive(void);
};

