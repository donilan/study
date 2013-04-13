#pragma once

#include "Game.h"
#define MAX_GAMER 5


class CGameAI
{
public:
	CGameAI(CGame* );
	~CGameAI(void);

	void join(CGameAI*);

	
	int singleSkill;
	int singleSkillLv;
	int fourSkill;
	int fourSkillLv;
	int allSkill;
	int allSkillLv;
	int petSkill;

	BOOL isAutoFighting;
private:
	
	int gamerNumber;
	CGameAI* gamer[MAX_GAMER];
	CGame* leader;

	static UINT gameAIAutoFightThread(LPVOID);
public:
	HWND getHWND(void);
	void startAutoFight(void);
	void stopAutoFight(void);
	CGame* getLeader(void);
	void playerFight(void);
	void petFight(void);
};

