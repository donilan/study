#include "StdAfx.h"
#include "GameAI.h"
#include "System.h"
#include "resource.h"
#include <time.h>
#include <time.h>
#include <stdlib.h>

#define FIGHT_INTERVAL 800
#define HIT_INTERNAL 200
#define TALK_INTERVAL 2000
#define WALK_INTERVAL 185
#define LOOP_TIMES 3

const RECT YES_CONDITION = {200, 320, 270, 370}; // yes
const RECT SURE_CONDITION = {200, 320, 400, 370}; // sure
const RECT CANCEL_CONDITION = {200, 310, 400, 380}; // cancel

const RECT SALE_ALL_CONDITION = {380, 390, 410, 410}; // sale all
const RECT SALE_CANCEL_CONDITION = {475, 390, 550, 410};
const RECT SALE_SURE_CONDITION = {305, 390, 375, 410}; // sale sure
const RECT SALE_YES_CONDITION = {100, 405, 170, 430}; // sale yes
const RECT SALE_CONDITION = {310, 302, 322, 312};		// sale
const RECT SALE_CONDITION2 = {310, 288, 322, 298};		// sale2
const RECT NOT_SALE_CONDITION = {310, 314, 322, 326};   // not sale
const RECT NOT_SALE_CONDITION2 = {310, 302, 322, 312};   // not sale2
CGameAI::CGameAI( CGame* game)
{
	this->leader = game;
	isAIStart = FALSE;
	isLocatePetSkill = FALSE;
	isReseted = FALSE;
	resetMinu = 0;
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
	CString logTmp;
	int mapWindowCheckCount = 0;
	int currX;
	int currY;
	int lastX = 0, lastY = 0;
	int nextX = 0;
	int nextY = 0;
	int walkStep = 0;
	int checkCounter = 0;
	BOOL isMapOpened = FALSE;
	BOOL hasNextStep = TRUE;
	//leader->getScreen()->startAutoRefresh();
	ai->script.command = CScript::UNKNOW;
	ai->script.resetPos();
	ai->startTime = CTime::GetCurrentTime();
	ai->writeLog(TEXT("\r\n开始脚本"));
	ai->fuckingMouse();
	while(ai->isAIStart && hasNextStep)
	{
		if(!leader->getScreen()->isFocus())
		{
			Sleep(500);
			continue;
		}
		Sleep(500);
		if(!isMapOpened && leader->topRightWindow->isExists())
		{
			ai->checkHPAndMP();
			leader->topRightWindow->openMap();
			isMapOpened = TRUE;
			TRACE("Map is opened!\n");
			Sleep(500);
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
					ai->writeLog(TEXT("转图完成"));
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
					Sleep(WALK_INTERVAL*walkStep/8);
				}
				Sleep(walkStep*WALK_INTERVAL);

				//重置机制
				if(lastX == currX && lastY == currY)
					++checkCounter;
				else
					checkCounter = 0;
				if(checkCounter > 20)
				{
					ai->writeLog(TEXT("发现人物卡死，尝试重新开始"));
					ai->script.resetPos();
				}
				lastX = currX;
				lastY = currY;
				//end 重置机制
				break;
			case CScript::HEAL:
				ai->doHeal();
				hasNextStep = ai->script.nextStep();
				break;
			case CScript::AGAIN:
				ai->writeLog(TEXT("\r\n脚本重来"));
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
				TRACE("Start auto fight.\n");
				ai->writeLog(TEXT("开始自动战斗"));
				while(ai->isAIStart)
					ai->autoFight();
				break;
			case CScript::BACK_TO_CITY:
				ai->doBackToCity();
				hasNextStep = ai->script.nextStep();
				break;
			case CScript::SALE:
				ai->doSale();
				hasNextStep = ai->script.nextStep();
				break;
			case CScript::TIME:
				ai->doTime();
				hasNextStep = ai->script.nextStep();
				break;
			} // end switch
			TRACE("command: %d, current (%d,%d), next: (%d,%d), target: (%d, %d)\n",
				ai->script.command, currX, currY, 
				ai->script.x, ai->script.y, ai->script.targetX, ai->script.targetY);
			continue;
		}// Travel
		else
		{
			if(leader->isOffline())
			{
				logTmp.Format(TEXT("执行脚本命令[%d] x: %d, y: %d 目标x: %d, 目标y: %d 时断线")
					, ai->script.command, ai->script.x, ai->script.y, ai->script.targetX, ai->script.targetY);
				ai->writeLog(logTmp);
				ai->isAIStart = FALSE;
				break;
			}
			++mapWindowCheckCount;
			if(mapWindowCheckCount < 5)
			{
				Sleep(1000);
				continue;
			}
		}
		if(hasNextStep == FALSE)
			return 0;
		ai->autoFight();
		
	}
	ai->endTime = CTime::GetCurrentTime();
	ai->writeLog(TEXT("结束脚本"));
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
		//复原可重置
		isReseted = FALSE;
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
	Sleep(FIGHT_INTERVAL/3);
	fuckingMouse();
	Sleep(FIGHT_INTERVAL/3);
}


void CGameAI::petFight(void)
{
	int numOfMonster = leader->monster->countAlive();
	CString petSkill;
	petSkill.Format(NUMBER_OF_MONSTER_PET_SKILL, numOfMonster);
	int skillNumber = GetPrivateProfileInt(FIGHT_SKILL, petSkill.GetBuffer(20), 1, CONFIG_FILE);
	leader->petCommandWindow->clickSkillCommand();
	Sleep(FIGHT_INTERVAL);
	if(leader->petSkillWindow->isExists())
	{
		isLocatePetSkill = TRUE;
		leader->petSkillWindow->leftClick(skillNumber-1);
		Sleep(FIGHT_INTERVAL);
		
	}
	
	leader->monster->hitBackOne();
	//防止偷袭
	Sleep(FIGHT_INTERVAL);
	leader->monster->hitOne();
	Sleep(FIGHT_INTERVAL);
	fuckingMouse();
}


BOOL CGameAI::choiceSkill(const int monsterNumber, int* skillIndex, int* skillLv)
{
	CString skillStr;
	CString skillLvStr;
	int skill0MP = 0;
	checkHPAndMP();
	skill0MP = GetPrivateProfileInt(SCRIPT_CONTROLL, WHEN_LOW_MP_SKILL, 20, CONFIG_FILE);
	TRACE("Mp %d, SKILL0 Mp %d\n", mp, skill0MP);
	if(skill0MP > 0 && mp < skill0MP)
	{
		*skillIndex = 0;
	}
	else
	{
		skillStr.Format(NUMBER_OF_MONSTER_SKILL, monsterNumber);
		skillLvStr.Format(NUMBER_OF_MONSTER_SKILL_LV, monsterNumber);
		*skillIndex = GetPrivateProfileInt(FIGHT_SKILL, skillStr.GetBuffer(20), 0, CONFIG_FILE);
		*skillLv = GetPrivateProfileInt(FIGHT_SKILL, skillLvStr.GetBuffer(20), 10, CONFIG_FILE);
	}
	return TRUE;
}

void CGameAI::doHeal()
{
	rightClickTager(script.targetX, script.targetY);
	Sleep(TALK_INTERVAL);
	RECT rect = {309, 218, 323, 233};
	writeLog(TEXT("执行治疗"));
	if(leader->getScreen()->colorDeviation(&rect, RGB(255,255,255)) > 5)
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
	if(leader->getScreen()->colorDeviation(&rect, RGB(255,255,255)) > 5)
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
	writeLog(TEXT("治疗完成"));
}

void CGameAI::rightClickTager(int x, int y){
	int centerX = 0, centerY = 0;
	int tmpX = leader->mapWindow->getX() - x;
	int tmpY = leader->mapWindow->getY() - y;
	leader->mapWindow->centerXY(&centerX, &centerY);
	if(tmpX > 0)
	{
		CSystem::rightClick(centerX -38*3, centerY + 28*3);
	} 
	else if(tmpX < 0)
	{
		CSystem::rightClick(centerX + 38*3, centerY - 28*3);
	}
	else if(tmpY > 0)
	{
		CSystem::rightClick(centerX - 38*3, centerY - 28*3);
	}
	else if(tmpX < 0)
	{
		CSystem::rightClick(centerX + 38*3, centerY + 28*3);
	}
}

void CGameAI::doTalk()
{
	RECT rect = {118, 164, 515, 330};
	BOOL done = FALSE;
	int currX = 0;
	int currY = 0;
	rightClickTager(script.x, script.x);
	Sleep(TALK_INTERVAL);
	//leader->getScreen()->flashRECT(&rect);
	writeLog(TEXT("执行对话"));
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
		else if(script.targetX != 0 && script.targetY != 0)
		{
			currX = leader->mapWindow->getX();
			currY = leader->mapWindow->getY();
			if(currX == script.targetX && currY == script.targetY)
				done = TRUE;
		}
		else{
			done = TRUE;
		}
	}
	writeLog(TEXT("执行对话完成"));
}

void CGameAI::doFindEnemy()
{
	static UINT fightCounter = 0;
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
	int goodsCounter = 0;
	int nowMinu = 0;
	int failCounter = 0;
	CString logTmp;
	writeLog(TEXT("开始遇敌"));
	while(isAIStart)
	{
		if(pMap->isExists())
		{
			
			if(notExistCounter > 0)
			{
				// 到点重置
				if(!isReseted && resetMinu != 0)
				{
					nowMinu = getMinu();
					TRACE("Now is %d reset minu %d\n", nowMinu, resetMinu);
					if(resetMinu == nowMinu)
					{
						writeLog(TEXT("发现人物卡死，尝试重新开始"));
						script.resetPos();
						isReseted = TRUE;
					}
				}
				Sleep(200);
				pMap->leftClickCenter();
				notExistCounter = 0;
				Sleep(200);
				checkGoods(&goodsCounter);
				logTmp.Format(TEXT("战斗结束，检查物品有%d个"), goodsCounter);
				writeLog(logTmp);
				//TRACE("==================Goods: %d\n", goodsCounter);
				if(goodsCounter == NUMBER_OF_GOODS)
				{
					if(isConfigYes(SCRIPT_CONTROLL, WHEN_FULL_GOODS_STOP_FIND_ENEMY))
					{
						script.resetPos();
						break;
					}
				}
				if(!checkHPAndMP())
				{
					break;
				}
			}
			currX = pMap->getX();
			currY = pMap->getY();
			// 无限制遇敌
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
				}  
				else
				{
					//TRACE("All found\n");
					if(currY <= endOfNorth+1)
						walkStep = pMap->moveMouse(0, 8);
					else if(currY >= endOfSouth-1)
						walkStep = pMap->moveMouse(0, -8);
					else if(!isPress)
						walkStep = pMap->moveMouse(0, 8);
					if(!isPress)
					{
						isPress = TRUE;
						CSystem::leftPress(0, 0);
					}
				}
				
			} 
			else // 限制遇敌
			{
				
				if(stepFlag)
				{
					//TRACE("current: (%d,%d), target: (%d,%d)\n", currX, currY, script.x, script.y);
					if(currX == script.x && currY == script.y)
						stepFlag = FALSE;
					else
						walkStep = pMap->moveMouse(script.x-currX, script.y-currY);
					
				}
				else
				{
					if(currX == script.targetX && currY == script.targetY)
						stepFlag = TRUE;
					else
						walkStep = pMap->moveMouse(script.targetX-currX, script.targetY-currY);
					
				}
				if(!isPress)
				{
					isPress = TRUE;
					CSystem::leftPress(0, 0);
				}
			} // // end 限制遇敌
			Sleep(WALK_INTERVAL*walkStep);
			//重置机制
			if(lastX == currX && lastY == currY)
				++failCounter;
			else
				failCounter = 0;
			if(failCounter > 10)
			{
				writeLog(TEXT("发现人物遇敌时卡死，尝试恢复"));
				isEndOfWest = FALSE;
				endOfNorth = 0;
				endOfSouth = 0;
				failCounter = 0;
				isPress = FALSE;
			}
			// end 重置机制
			lastX = currX;
			lastY = currY;
			continue;
		}
		else
		{
			if(leader->isOffline())
			{
				logTmp.Format(TEXT("执行脚本命令[%d] x: %d, y: %d 目标x: %d, 目标y: %d 时断线")
					, script.command, script.x, script.y, script.targetX, script.targetY);
				writeLog(logTmp);
				isAIStart = FALSE;
				break;
			}
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
			Sleep(FIGHT_INTERVAL);
		}// fight
		else if(leader->petCommandWindow->isExists())
		{
			petFight();
			Sleep(2000);
		} else 
			Sleep(500);
	} else {
		Sleep(1000);
	}
}


BOOL CGameAI::checkHPAndMP(void)
{
	int leaveHP = 0;
	int leaveMP = 0;
	if(leader->topLeftWindow->isExists())
	{
		leaveHP = GetPrivateProfileInt(SCRIPT_CONTROLL, WHEN_HP_STOP_FIND_ENEMY, 0, CONFIG_FILE);
		leaveMP = GetPrivateProfileInt(SCRIPT_CONTROLL, WHEN_MP_STOP_FIND_ENEMY, 0, CONFIG_FILE);
		hp = leader->topLeftWindow->getHP();
		mp = leader->topLeftWindow->getMP();
		TRACE("HP: %d, MP: %d , leave HP: %d, leave MP: %d.\n", hp, mp, leaveHP, leaveMP);
		if(leaveHP == 0 || leaveMP == 0)
			return TRUE;
		else if(hp <= leaveHP || mp <= leaveMP)
			return FALSE;
	} 
	else
	{
		hp = leader->topLeftWindow->getHPOnFighting();
		mp = leader->topLeftWindow->getMPOnFighting();
		TRACE("HP: %d, MP: %d \n", hp, mp);
	}
	return TRUE;
}

void CGameAI::doSale()
{
	RECT rect = {0};
	int goodsCounter = 0;
	int times = 0;
	BOOL done = FALSE;
	int tmp = 0;
	writeLog(TEXT("出售魔石"));
	while(!done && isAIStart)
	{
		++times;
		rightClickTager(script.targetX, script.targetY);
		Sleep(TALK_INTERVAL/2);
		fuckingMouse();
		Sleep(TALK_INTERVAL/2);
		tmp = leader->getScreen()->colorDeviation(&SALE_CONDITION, RGB(255,255,255));
		if(tmp >=30 && tmp < 50)
		{
			CSystem::leftClick(&SALE_CONDITION);
			Sleep(TALK_INTERVAL);
		} 
		else if (tmp < 30 && tmp > 5)
		{
			CSystem::leftClick(&SALE_CONDITION2);
			Sleep(TALK_INTERVAL);
		}

		for(int i = 0; i < LOOP_TIMES; ++i) 
		{
			if(leader->getScreen()->locate(IDB_ALL, &rect, &SALE_ALL_CONDITION))
			{
				TRACE("Found Sale all\n");
				CSystem::leftClick(&rect);
				Sleep(TALK_INTERVAL);
				for( int x = 0; x < LOOP_TIMES; ++x)
				{
					if(leader->getScreen()->locate(IDB_SURE, &rect, &SALE_SURE_CONDITION))
					{
						TRACE("Found sale sure\n");
						CSystem::leftClick(&rect);
						Sleep(TALK_INTERVAL);
					}
					if(leader->getScreen()->locate(IDB_YES, &rect, &SALE_YES_CONDITION))
					{
						TRACE("Found sale yes\n");
						CSystem::leftClick(&rect);
						Sleep(TALK_INTERVAL);
						break;
					}
					Sleep(TALK_INTERVAL);
				}
				checkGoods(&goodsCounter);
				if(goodsCounter < 10)
				{
					done = TRUE;
					break;
				}
			} 
			if(leader->getScreen()->locate(IDB_CANCEL, &rect, &SALE_CANCEL_CONDITION))
			{
				TRACE("Found Sale Cancel\n");
				CSystem::leftClick(&rect);
				Sleep(TALK_INTERVAL);
			} 
			if(leader->getScreen()->colorDeviation(&NOT_SALE_CONDITION, RGB(255,255,255)) > 5)
			{
				CSystem::leftClick(&NOT_SALE_CONDITION);
				Sleep(TALK_INTERVAL);
				done = TRUE;
				break;
			} else if(leader->getScreen()->colorDeviation(&NOT_SALE_CONDITION2, RGB(255,255,255)) > 5)
			{
				CSystem::leftClick(&NOT_SALE_CONDITION2);
				Sleep(TALK_INTERVAL);
				done = TRUE;
				break;
			}
			Sleep(TALK_INTERVAL);
		}
	}
	writeLog(TEXT("出售魔石完成"));
}


void CGameAI::checkGoods(int* goodsCounter)
{
	int goodsType = 0;
	writeLog(TEXT("检查物品"));
	if(leader->bottomWindow->isExists())
	{
		leader->bottomWindow->openGoodsWindow();
		Sleep(TALK_INTERVAL);
		
		if(leader->goodsWindows->isExists())
		{
			fuckingMouse();
			Sleep(100);
			while(goodsType == 0 && *goodsCounter < NUMBER_OF_GOODS)
			{
				goodsType = leader->goodsWindows->goodsType(*goodsCounter);
				TRACE("goods[%d] type: %d\n", *goodsCounter, goodsType);

				if(goodsType == IDB_GOODS_NO_GOODS)
				{
					break;
				}
				else if(goodsType == 0)
				{
					++(*goodsCounter);
				} 
				else if(goodsType == IDB_GOODS_CARD)
				{
					if(isConfigYes(SCRIPT_CONTROLL, DROP_CARD))
						leader->goodsWindows->dropGoods(*goodsCounter);
					break;
				} 
				else if (goodsType == IDB_GOODS_EARTH_PROP
					|| goodsType == IDB_GOODS_WIND_PROP
					|| goodsType == IDB_GOODS_FIRE_PROP
					|| goodsType == IDB_GOODS_WATER_PROP)
				{
					if(isConfigYes(SCRIPT_CONTROLL, DROP_CRYSTAL))
						leader->goodsWindows->dropGoods(*goodsCounter);
					break;
				}
				
			}
		}
	} // counter goods
	Sleep(TALK_INTERVAL);
	leader->bottomWindow->closeGoodsWindow();
	Sleep(TALK_INTERVAL);
	writeLog(TEXT("检查物品完成"));
}


BOOL CGameAI::isConfigYes(LPCWSTR app, LPCWSTR key)
{
	TCHAR buff[MAX_PATH] = {0};
	GetPrivateProfileString(app, key, CN_NO, buff, MAX_PATH, CONFIG_FILE);
	TRACE("config: ");
	OutputDebugString(buff);
	TRACE("\n");
	if(_tcsncmp(buff, CN_YES, wcslen(CN_YES)) == 0)
		return TRUE;
	return FALSE;
}

void CGameAI::doBackToCity()
{
	int x = 0;
	int y = 0;
	if(script.x == 0 || script.y == 0)
		return;
	writeLog(TEXT("回城开始"));
	while(TRUE && isAIStart)
	{
		if(!leader->mapWindow->isExists())
		{
			fuckingMouse();
			Sleep(TALK_INTERVAL);
			continue;
		}
		
		x = leader->mapWindow->getX();
		y = leader->mapWindow->getY();
		if((x == script.x && y == script.y) 
			|| (x == script.targetX && y == script.targetY))
			break;
		TRACE("Do back city: %d,%d %d,%d\n", script.x, script.y, script.targetX, script.targetY);
		if((script.x == 72 && script.y == 123)
			|| (script.x == 233 && script.y == 78)
			|| (script.x == 162 && script.y == 130)
			|| (script.targetX == 72 && script.targetY == 123)
			|| (script.targetX == 233 && script.targetY == 78)
			|| (script.targetX == 162 && script.targetY == 130))
		{
			if(x == 72 && y == 123)
			{
				rightClickTager(73, 123);
				continue;
			} else if(x == 233 && y == 78)
			{
				rightClickTager(233, 77);
				continue;
			} else if(x == 162 && y == 130 )
			{
				rightClickTager(163, 130);
				continue;
			}
		}
		if((script.x == 63 && script.y == 79)
			|| (script.x == 242 && script.y == 100)
			|| (script.x == 141 && script.y == 148)
			|| (script.targetX == 63 && script.targetY == 79)
			|| (script.targetX == 242 && script.targetY == 100)
			|| (script.targetX == 141 && script.targetY == 148))
		{
			if(x == 63 && y == 79)
			{
				rightClickTager(63, 78);
				continue;
			} else if(x == 242 && y == 100)
			{
				rightClickTager(243, 100);
				continue;
			} else if(x == 141 && y == 148)
			{
				rightClickTager(141, 147);
				continue;
			}
		}
		
		leader->bottomWindow->openSystemWindow();
		Sleep(TALK_INTERVAL);
		if(leader->systemWindow->isExists())
		{
			leader->systemWindow->clickBackToCity();
		} else 
		{
			TRACE("System window not exists\n");
		}
	}
	writeLog(TEXT("回城完成"));
}

void CGameAI::doTime()
{
	int minu = getMinu();
	BOOL hasNext = TRUE;
	if(minu >= script.x && minu <= script.y)
	{
		resetMinu = script.targetX;
		return;
	} 
	else {
		while(hasNext)
		{
			hasNext = script.nextStep();
			if(script.command == CScript::TIME)
			{
				if(minu >= script.x && minu <= script.y)
				{
					resetMinu = script.targetX;
					return;
				}
			} 
			else if(script.command == CScript::AGAIN)
			{
				TRACE("Time.....\n");
				script.resetPos();
				Sleep(1000);
				return;
			}
		}
		
	}
}

int CGameAI::getMinu()
{
	time_t tt = time(NULL);
	tm *t = localtime(&tt);
	int minu = t->tm_min;
	//free(t);
	return minu;
}


void CGameAI::fuckingMouse(void)
{
	SetCursorPos(630, 500);
}

void CGameAI::writeLog( CString str )
{
	static TCHAR* szFileName = TEXT("运行日志.log");
	CString fmtStr;
	CTime nowTime = CTime::GetCurrentTime();
	CFile file;
	CFileStatus status;
	BOOL isOk = FALSE;
	if( CFile::GetStatus( szFileName, status ) )
	{
		// Open the file without the Create flag
		//OutputDebugString(TEXT("Write file.\n"));
		isOk = file.Open( szFileName, CFile::modeWrite );
	}
	else
	{
		// Open the file with the Create flag
		//OutputDebugString(TEXT("Create file.\n"));
		isOk = file.Open( szFileName, CFile::modeCreate | CFile::modeWrite );
	}
	if(isOk)
	{
		file.SeekToEnd();
		fmtStr.Format(TEXT("%s - %s\r\n"), nowTime.Format("%Y-%m-%d %H:%M:%S"), str);
		file.Write(fmtStr, fmtStr.GetLength()*sizeof(TCHAR));
		file.Close();
	}
	
}
