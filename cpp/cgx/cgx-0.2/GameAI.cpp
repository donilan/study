#include "StdAfx.h"
#include "GameAI.h"
#include "System.h"
#include "resource.h"

#define FIGHT_INTERVAL 800
#define HIT_INTERNAL 200
#define TALK_INTERVAL 800


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
		{
			Sleep(500);
			continue;
		}
		Sleep(500);
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
				ai->doHeal();
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
			
			ai->playerFight();
		}// fight
		else if(leader->petCommandWindow->isExists())
		{
			ai->petFight();
		}
		else
		{
			Sleep(1000);
		}
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


void CGameAI::playerFight()
{
	int skill = -1;
	int level = -1;
	int numOfMonster = leader->monster->countAlive();
	choiceSkill(numOfMonster, &skill, &level);
	TRACE("Player fighting start skill index [%d] level [%d]...\n", skill, level);
	if(skill == 0)
	{
		if(!leader->playerCommandWindow->isCommandEnable(0))
			leader->playerCommandWindow->clickHitCommand();
	}
	else
	{
		if(!leader->playerCommandWindow->isCommandEnable(1))
		{
			leader->playerCommandWindow->clickSkillCommand();
			Sleep(FIGHT_INTERVAL);
		}
		if(leader->playerSkillWindow->isExists())
		{
			
			leader->playerSkillWindow->leftClick(skill-1);
			Sleep(FIGHT_INTERVAL);
		}
		
		if(leader->playerSkillLevelWindow->isExists())
		{
			leader->playerSkillLevelWindow->leftClick(level-1);
		}
	}
	Sleep(FIGHT_INTERVAL);
	leader->monster->hitOne();
}


void CGameAI::petFight(void)
{
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

void CGameAI::doHeal()
{
	rightClickTager(script.targetX, script.targetY);
	Sleep(TALK_INTERVAL);
	RECT rect = {309, 218, 323, 233};
	RECT condition1 = {220, 340, 260, 356}; // yes
	RECT condition2 = {290, 340, 255, 356}; // sure
	if(leader->getScreen()->colorDeviation(&rect, RGB(255,255,255)> 5))
	{
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}
	if(leader->getScreen()->locate(IDB_SURE, &rect, &condition1))
	{
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}
	else if(leader->getScreen()->locate(IDB_SURE, &rect, &condition2))
	{
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}

	rect.left = 325;
	rect.top = 298;
	rect.right = 340;
	rect.bottom = 313;
	if(leader->getScreen()->colorDeviation(&rect, RGB(255,255,255)> 5))
	{
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}

	if(leader->getScreen()->locate(IDB_SURE, &rect, &condition1))
	{
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}
	if(leader->getScreen()->locate(IDB_SURE, &rect, &condition2))
	{
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}
}

void CGameAI::rightClickTager(int x, int y){
	int centerX = 0, centerY = 0;
	int tmpX = leader->mapWindow->getX() - x;
	int tmpY = leader->mapWindow->getY() - y;
	leader->mapWindow->centerXY(&centerX, &centerY);
	if(tmpX > 0)
	{
		CSystem::rightClick(centerX -38, centerY + 28);
	} 
	else if(tmpX < 0)
	{
		CSystem::rightClick(centerX + 38, centerY - 28);
	}
	else if(tmpY > 0)
	{
		CSystem::rightClick(centerX - 38, centerY - 28);
	}
	else if(tmpX < 0)
	{
		CSystem::rightClick(centerX + 38, centerY + 28);
	}
}
