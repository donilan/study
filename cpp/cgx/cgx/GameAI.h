#pragma once

#include "Game.h"
#define MAX_GAMER 5


class CGameAI
{
public:
	CGameAI(void);
	~CGameAI(void);

	void walk();

	void join(CGame*);

	void setLeader(CGame *);
	CGame* getLeader();
	CGame* getGamer(int idx);
	int getGamerSize();
	// auto fighting
	BOOL startAutoFighting();
	BOOL stopAutoFighting();

private:
	int gamerNumber;
	
	CGame* gamer[MAX_GAMER];
	CGame* leader;
};

