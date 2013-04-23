#include "StdAfx.h"
#include "GameAI.h"
#include "System.h"
#include "resource.h"
#include <time.h>
#include <stdlib.h>

#define FIGHT_INTERVAL 800
#define HIT_INTERNAL 200
#define TALK_INTERVAL 1300
#define WALK_INTERVAL 300

const RECT YES_CONDITION = {200, 320, 270, 370}; // yes
const RECT SURE_CONDITION = {200, 320, 400, 370}; // sure
const RECT CANCEL_CONDITION = {200, 310, 400, 380}; // cancel

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
	int mapWindowCheckCount = 0;
	int currX;
	int currY;
	int nextX = 0;
	int nextY = 0;
	int walkStep;
	BOOL hasNextStep = TRUE;
	//leader->getScreen()->startAutoRefresh();
	ai->script.command = CScript::UNKNOW;
	ai->script.resetPos();
	while(ai->isAIStart && hasNextStep)
	{
		if(!leader->getScreen()->isFocus())
		{
			Sleep(500);
			continue;
		}
		
		if(leader->mapWindow->isExists())
		{
			mapWindowCheckCount = 0;
			currX = leader->mapWindow->getX();
			currY = leader->mapWindow->getY();
			if(ai->script.command == CScript::UNKNOW) {
				hasNextStep = ai->script.nextStep();
			}
			switch(ai->script.command)
			{
			case CScript::CHANGE_MAP:
				if(abs(ai->script.targetX -currX) < 2 && abs(ai->script.targetY - currY) < 2)
				{
					hasNextStep = ai->script.nextStep();
					break;
				}
			case CScript::WALK:
				if(currX == ai->script.x && currY == ai->script.y)
				{
					hasNextStep = ai->script.nextStep();
					break;
				}
				nextX = ai->script.x - currX;
				nextY = ai->script.y - currY;
				
				walkStep = leader->mapWindow->goNext(nextX, nextY);
				if((nextX != 0 && abs(nextX) < 10)
					|| (nextY != 0 && abs(nextY) < 10))
				{
					TRACE("Sleep more................................\n");
					Sleep(WALK_INTERVAL*walkStep);
				}
				Sleep(walkStep*WALK_INTERVAL);
				break;
			case CScript::HEAL:
				ai->doHeal();
				hasNextStep = ai->script.nextStep();
				break;
			case CScript::AGAIN:
				ai->script.resetPos();
				hasNextStep = ai->script.nextStep();
				TRACE("===============  Again  ==================\n");
				break;
			case CScript::FIND_ENEMY:
				ai->doFindEnemy();
				hasNextStep = ai->script.nextStep();
				break;
			case CScript::TALK:
				ai->doTalk();
				hasNextStep = ai->script.nextStep();
				break;
			case CScript::UNKNOW:
				Sleep(500);
				break;
			} // end switch
			TRACE("command: %d, current (%d,%d), next: (%d,%d), target: (%d, %d)\n",
				ai->script.command, currX, currY, 
				ai->script.x, ai->script.y, ai->script.targetX, ai->script.targetY);
			continue;
		}// Travel
		else
		{
			++mapWindowCheckCount;
			if(mapWindowCheckCount < 5)
			{
				Sleep(1000);
				continue;
			}
		}
		if(leader->playerCommandWindow->isExists())
		{
			ai->playerFight();
		}// fight
		else if(leader->petCommandWindow->isExists())
		{
			ai->petFight();
		}
		else
		{
			Sleep(500);
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
	if(leader->petSkillWindow->isExists())
	{
		leader->petSkillWindow->leftClick(0);
	}
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
	
	if(leader->getScreen()->colorDeviation(&rect, RGB(255,255,255)> 5))
	{
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}
	
	if(leader->getScreen()->locate(IDB_YES, &rect, &YES_CONDITION))
	{
		TRACE("Found yes\n");
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}
	if(leader->getScreen()->locate(IDB_SURE, &rect, &SURE_CONDITION))
	{
		TRACE("Found sure\n");
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

	if(leader->getScreen()->locate(IDB_YES, &rect, &YES_CONDITION))
	{
		
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	}
	if(leader->getScreen()->locate(IDB_SURE, &rect, &SURE_CONDITION))
	{
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
		SetCursorPos(rect.right+3, rect.bottom+3);
	}

	if(leader->getScreen()->locate(IDB_CANCEL, &rect, &CANCEL_CONDITION))
	{
		TRACE("Found cancel\n");
		
		CSystem::leftClick(&rect);
		Sleep(TALK_INTERVAL);
	} else
	{
		TRACE("Cancel button not found!\n");
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

void CGameAI::doTalk()
{
	RECT rect = {118, 164, 515, 330};
	BOOL done = FALSE;
	rightClickTager(script.targetX, script.targetY);
	Sleep(TALK_INTERVAL);
	leader->getScreen()->flashRECT(&rect);
		while(!done)
		{
			if(leader->getScreen()->locate(IDB_YES, &rect, &YES_CONDITION))
			{
				CSystem::leftClick(&rect);
				Sleep(TALK_INTERVAL);
			} 
			else if(leader->getScreen()->locate(IDB_SURE, &rect, &SURE_CONDITION))
			{
				CSystem::leftClick(&rect);
				Sleep(TALK_INTERVAL);
			} 
			else{
				done = TRUE;
			}
		}
	
}

void CGameAI::doFindEnemy()
{
	CCgxMapWindow* pMap = leader->mapWindow;
	int lastX = 0;
	int lastY = 0;
	int endOfNorth = 0;
	int endOfSouth = 0;
	int walkStep = 0;
	int currX = 0;
	int currY = 0;
	int notExistCounter = 0;
	BOOL isEndOfWest = FALSE;
	BOOL isPress = FALSE;
	srand(time(NULL));
	
	while(isAIStart)
	{
		if(pMap->isExists())
		{
			if(notExistCounter > 0)
			{
				Sleep(200);
				pMap->leftClickCenter();
				notExistCounter = 0;
				Sleep(200);
			}
			currX = pMap->getX();
			currY = pMap->getY();
			if(!isEndOfWest)
			{
				if(lastX == currX)
				{
					isEndOfWest = TRUE;
					lastY = 0;
					continue;
				}
				walkStep = pMap->goNext(-10, 0);
				Sleep(walkStep* WALK_INTERVAL);
			} 
			else if(endOfSouth == 0)
			{
				if(lastY == currY)
				{
					endOfSouth = currY;
					lastY = 0;
					continue;
				}
				walkStep = pMap->goNext(0, 8);
				Sleep(walkStep* WALK_INTERVAL);
			}
			else if(endOfNorth == 0)
			{
				if(lastY == currY)
				{
					endOfNorth = currY;
					continue;
				}
				walkStep = pMap->goNext(0, -8);
				Sleep(walkStep* WALK_INTERVAL);
			} else
			{
				TRACE("All found\n");
				if(currY <= endOfNorth)
					walkStep = pMap->moveMouse(0, 8);
				else if(currY >= endOfSouth)
					walkStep = pMap->moveMouse(0, -8);
				if(!isPress || lastY == currY)
				{
					isPress = TRUE;
					CSystem::leftPress(0, 0);
				}
				Sleep(WALK_INTERVAL);
			}
			lastX = currX;
			lastY = currY;
			continue;
		}
		else
		{
			isPress = FALSE;
			++notExistCounter;
			if(notExistCounter < 5)
			{
				Sleep(1000);
				continue;
			}
		}
		if(leader->playerCommandWindow->isExists())
		{
			playerFight();
		}// fight
		else if(leader->petCommandWindow->isExists())
		{
			petFight();
		}
		else
		{
			Sleep(1000);
		}
	}
}
