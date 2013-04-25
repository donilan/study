#pragma once

#include "Game.h"
#include "Script.h"

#define MAX_GAMER 5
#define SCRIPT_LENGTH 4098
#define FIGHT_SKILL TEXT("战斗技能")
#define SCRIPT_CONTROLL TEXT("脚本控制")
#define WHEN_HP_STOP_FIND_ENEMY TEXT("停止遇敌血量")
#define WHEN_MP_STOP_FIND_ENEMY TEXT("停止遇敌魔力")
#define WHEN_LOW_MP_SKILL TEXT("低于多少MP使用攻击")
#define WHEN_FULL_GOODS_STOP_FIND_ENEMY TEXT("满道具停止遇敌")
#define DROP_CARD TEXT("丢弃卡片")
#define DROP_CRYSTAL TEXT("丢弃水晶")
#define NUMBER_OF_MONSTER_SKILL TEXT("%d个怪物技能")
#define NUMBER_OF_MONSTER_SKILL_LV TEXT("%d个怪物技能等级")
#define CONFIG_FILE TEXT(".\\config.ini")


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
	inline void rightClickTager(int x, int y);
public:
	HWND getHWND(void);
	void startAI(void);
	void stopAI(void);
	CGame* getLeader(void);
	void playerFight();
	void petFight(void);
	BOOL choiceSkill(const int monsterNumber, int* skillIndex, int* skillLv);
	void doHeal();
	void doTalk();
	void doFindEnemy();

};

