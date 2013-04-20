#include "StdAfx.h"
#include "GameAI.h"

#define FIGHT_INTERVAL 400
#define HIT_INTERNAL 200


CGameAI::CGameAI( CGame* game)
{
	this->leader = game;
	isAIStart = FALSE;

}

CGameAI::~CGameAI()
{

}

void CGameAI::join( CGameAI* )
{

}

HWND CGameAI::getHWND(void)
{
	return this->leader->getHWND();
}

UINT CGameAI::gameAIThread(LPVOID lpVoid)
{
	CGameAI *ai = (CGameAI *) lpVoid;
	CGame* leader = ai->getLeader();

	int currX;
	int currY;
	int nextX = 0;
	int nextY = 0;
	//leader->getScreen()->startAutoRefresh();
	ai->script.command = CScript::UNKNOW;
	ai->script.resetPos();
	while(ai->isAIStart)
	{
		if(!leader->getScreen()->isFocus())
			continue;
		Sleep(200);
		if(leader->mapWindow->isExists())
		{
			
			currX = leader->mapWindow->getX();
			currY = leader->mapWindow->getY();
			if(ai->script.command == CScript::UNKNOW) {
				ai->script.nextStep();
			}
			switch(ai->script.command)
			{
			case CScript::CHANGE_MAP:
				if(abs(ai->script.targetX -currX) < 5 && abs(ai->script.targetY - currY) < 5)
				{
					ai->script.nextStep();
					break;
				}
			case CScript::WALK:
				if(currX == ai->script.x && currY == ai->script.y)
				{
					ai->script.nextStep();
					break;
				}
				nextX = ai->script.x - currX;
				nextY = ai->script.y - currY;
				
				leader->mapWindow->goNext(nextX, nextY);
				break;
			case CScript::HEAL:
				// TODO
				break;
			case CScript::AGAIN:
				ai->script.resetPos();
				ai->script.nextStep();
				TRACE("===============  Again  ==================\n");
				break;
			case CScript::UNKNOW:
				Sleep(500);
				break;
			} // end switch
			TRACE("command: %d, current (%d,%d), next: (%d,%d), target: (%d, %d)\n",
				ai->script.command, currX, currY, 
				ai->script.x, ai->script.y, ai->script.targetX, ai->script.targetY);
		}// Travel
		else if(leader->playerCommandWindow->isExists())
		{
			int monsterNum = leader->monster->countAlive();
			if(monsterNum < 2)
			{
				leader->playerCommandWindow->clickHitCommand();
				Sleep(HIT_INTERNAL);
				leader->monster->hitOne();
				Sleep(HIT_INTERNAL);
				leader->monster->hitOne();
			} else {
				TRACE("Hit skill command!\n");
				leader->playerCommandWindow->clickSkillCommand();
				Sleep(HIT_INTERNAL);
				while(!leader->playerCommandWindow->isExists())
				{
					Sleep(HIT_INTERNAL);
				}
				leader->playerSkillWindow->leftClick(1);
				while(!leader->playerSkillLevelWindow->isExists())
				{
					Sleep(HIT_INTERNAL);
				}
				leader->playerSkillLevelWindow->leftClick(monsterNum-1);
				Sleep(HIT_INTERNAL);
				leader->monster->hitOne();
				Sleep(HIT_INTERNAL);
				leader->monster->hitOne();
			}
		}// fight
		
	}
	return 0;
}

void CGameAI::startAI(void)
{
	TRACE("Start AI.\n");
	if(isAIStart) return;
	isAIStart = TRUE;
	AfxBeginThread(gameAIThread, this);
}


void CGameAI::stopAI(void)
{
	TRACE("Stop AI.\n");
	isAIStart = FALSE;
}


CGame* CGameAI::getLeader(void)
{
	return leader;
}


void CGameAI::playerFight(void)
{
	TRACE("Player fighting start...\n");
	leader->playerCommandWindow->clickHitCommand();
	Sleep(FIGHT_INTERVAL);
	leader->monster->hitOne();
}


void CGameAI::petFight(void)
{
	leader->petSkillWindow->leftClick(0);
	Sleep(FIGHT_INTERVAL);
	leader->monster->hitOne();
}


BOOL CGameAI::choiceSkill(const int monsterNumber, int* skillIndex, int* skillLv)
{
	CString skillStr;
	CString skillLvStr;
	skillStr.Format(NUMBER_OF_MONSTER_SKILL, monsterNumber);
	skillLvStr.Format(NUMBER_OF_MONSTER_SKILL_LV, monsterNumber);
	*skillIndex = GetPrivateProfileInt(FIGHT_SKILL, skillStr.GetBuffer(20), 0, CONFIG_FILE);
	*skillLv = GetPrivateProfileInt(FIGHT_SKILL, skillLvStr.GetBuffer(20), 10, CONFIG_FILE);
	return TRUE;
}
