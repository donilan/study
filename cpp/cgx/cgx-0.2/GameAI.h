#pragma once

#include "Game.h"
#define MAX_GAMER 5
#define SCRIPT_LENGTH 4098
#include "Script.h"

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

	BOOL isAIStart;

	CScript script;
private:
	
	int gamerNumber;
	CGameAI* gamer[MAX_GAMER];
	CGame* leader;

	static UINT gameAIThread(LPVOID);
public:
	HWND getHWND(void);
	void startAI(void);
	void stopAI(void);
	CGame* getLeader(void);
	void playerFight(void);
	void petFight(void);
};

