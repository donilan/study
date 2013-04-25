#pragma once

#include "Game.h"
#include "Script.h"

#define MAX_GAMER 5
#define SCRIPT_LENGTH 4098
#define FIGHT_SKILL TEXT("ս������")
#define SCRIPT_CONTROLL TEXT("�ű�����")
#define WHEN_HP_STOP_FIND_ENEMY TEXT("ֹͣ����Ѫ��")
#define WHEN_MP_STOP_FIND_ENEMY TEXT("ֹͣ����ħ��")
#define WHEN_LOW_MP_SKILL TEXT("���ڶ���MPʹ�ù���")
#define WHEN_FULL_GOODS_STOP_FIND_ENEMY TEXT("������ֹͣ����")
#define DROP_CARD TEXT("������Ƭ")
#define DROP_CRYSTAL TEXT("����ˮ��")
#define NUMBER_OF_MONSTER_SKILL TEXT("%d�����＼��")
#define NUMBER_OF_MONSTER_SKILL_LV TEXT("%d�����＼�ܵȼ�")
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

