#pragma once

#include <Game.h>
#define MAX_GAMER 5

class CGameAI
{
public:
	CGameAI(void);
	~CGameAI(void);

	void walk();

	void join(CGame*);
private:
	CGame* gamer[MAX_GAMER];
	CGame* leader;
};

