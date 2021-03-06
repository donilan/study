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
#define NUMBER_OF_MONSTER_PET_SKILL TEXT("%d个怪物宠物使用技能")
#define PET_ATTACK_BACK TEXT("宠物是否攻击后排")
#define NUMBER_OF_MONSTER_SKILL TEXT("%d个怪物技能")
#define NUMBER_OF_MONSTER_SKILL_LV TEXT("%d个怪物技能等级")
#define CONFIG_FILE TEXT(".\\config.ini")


class CGameAI
{
public:
	CGameAI(CGame* );
	~CGameAI(void);

	void join(CGameAI*);

	CTime startTime;
	CTime endTime;
	int singleSkill;
	int singleSkillLv;
	int fourSkill;
	int fourSkillLv;
	int allSkill;
	int allSkillLv;
	int petSkill;
	int resetMinu;
	BOOL isReseted;
	int mp;
	int hp;
	BOOL isAIStart;
	BOOL isLocatePetSkill;
	//统计
	BOOL isFighting;
	int fightTimes; //战斗次数统计
	int fightRound; //战斗回合统计

	int saleCounter;
	int healCounter;
	int resetCounter;
	int talkCounter;
	int backToCityCounter;
	//end统计
	CScript script;

	BOOL isGameClosed;
private:
	
	int gamerNumber;
	CGameAI* gamer[MAX_GAMER];
	CGame* leader;

	static UINT gameAIThread(LPVOID);
	inline void rightClickTager(int x, int y);
	inline int getMinu();
	inline void sayAgain();
	inline void fuckingNP();
public:
	HWND getHWND(void);
	void startAI(void);
	void stopAI(void);
	CGame* getLeader(void);
	void playerFight();
	void petFight(void);
	BOOL choiceSkill(const int monsterNumber, int* skillIndex, int* skillLv);
	void makeSureLocation();
	void doHeal();
	void doTalk();
	void doFindEnemy();

	void autoFight(void);
	BOOL checkHPAndMP(void);
	void doSale();
	void checkGoods(int *);
	BOOL isConfigYes(LPCWSTR app, LPCWSTR key);
	void doBackToCity();
	void doTime();
	void fuckingMouse(void);
	void writeLog(CString str);
	void sayRandom();
	void doTest();
	void doCloseGame();
	void doStartGame();
};

