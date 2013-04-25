#include "StdAfx.h"
#include "GameAI.h"
#include "System.h"
#include "resource.h"
#include <time.h>
#include <stdlib.h>

#define FIGHT_INTERVAL 800
#define HIT_INTERNAL 200
#define TALK_INTERVAL 1300
#define WALK_INTERVAL 250

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
				if((abs(ai->script.targetX -currX) < 2 && abs(ai->script.targetY - currY) < 2)
				|| (ai->script.targetX == 0 && ai->script.targetY == 0 && 
				(abs(currX - ai->script.x) > 4 || abs(currY - ai->script.y) > 4)))
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
					Sleep(WALK_INTERVAL*walkStep/2);
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
			case CScript::AUTO_FIGHT:
				while(ai->isAIStart)
					ai->autoFight();
				break;
			case CScript::BACK_TO_CITY:
				//TODO 
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
		ai->autoFight();
		
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
	Sleep(FIGHT_INTERVAL*2);
}


void CGameAI::petFight(void)
{
	if(leader->petSkillWindow->isExists())
	{
		leader->petSkillWindow->leftClick(0);
		Sleep(FIGHT_INTERVAL);
	}
	
	leader->monster->hitBackOne();
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
	//leader->getScreen()->flashRECT(&rect);
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
	BOOL stepFlag = TRUE;
	int currX = 0;
	int currY = 0;
	int notExistCounter = 0;
	BOOL isEndOfWest = FALSE;
	BOOL isPress = FALSE;
	
	while(isAIStart)
	{
		if(pMap->isExists())
		{
			if(notExistCounter > 0)
			{
				Sleep(200);
				pMap->leftClickCenter();
				notExistCounter = 0;
				Sleep(1000);
			}
			currX = pMap->getX();
			currY = pMap->getY();
			// ����������
			if(script.x == 0 && script.y == 0)
			{
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
					if(currY <= endOfNorth+1)
						walkStep = pMap->moveMouse(0, 8);
					else if(currY >= endOfSouth-1)
						walkStep = pMap->moveMouse(0, -8);
					if(!isPress || lastY == currY)
					{
						isPress = TRUE;
						CSystem::leftPress(0, 0);
					}
					
				}
			} 
			else // ��������
			{
				
				if(stepFlag)
				{
					TRACE("current: (%d,%d), target: (%d,%d)\n", currX, currY, script.x, script.y);
					if(currX == script.x && currY == script.y)
						stepFlag = FALSE;
					else
						walkStep = pMap->goNext(script.x-currX, script.y-currY);
					
				}
				else
				{
					if(currX == script.targetX && currY == script.targetY)
						stepFlag = TRUE;
					else
						walkStep = pMap->goNext(script.targetX-currX, script.targetY-currY);
					
				}
				
			}
			Sleep(WALK_INTERVAL);
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
		autoFight();
	}
}



void CGameAI::autoFight(void)
{
	if(leader->getScreen()->isFocus() && !leader->mapWindow->isExists())
	{
		if(leader->playerCommandWindow->isExists())
		{
			playerFight();

		}// fight
		else if(leader->petCommandWindow->isExists())
		{
			petFight();
			Sleep(10000);
		}
	} else {
		Sleep(500);
	}
}


BOOL CGameAI::checkHPAndMP(void)
{
	if(leader->topLeftWindow->isExists())
		TRACE("HP: %d, MP: %d\n", leader->topLeftWindow->getHP(), leader->topLeftWindow->getMP());
	return TRUE;
}