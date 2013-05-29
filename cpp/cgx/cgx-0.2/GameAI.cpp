#include "StdAfx.h"
#include "GameAI.h"
#include "System.h"
#include "resource.h"
#include <time.h>
#include <stdlib.h>

#define FIGHT_INTERVAL 800
#define HIT_INTERNAL 200
#define TALK_INTERVAL 2500
//185
#define WALK_INTERVAL 180
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
	isGameClosed = TRUE;
	srand (time(NULL));
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
	int tmpX = 0;
	int tmpY = 0;
	CGameAI *ai = (CGameAI *) lpVoid;
	CGame* leader = ai->getLeader();
	CString logTmp;
	int mapWindowCheckCount = 0;
	int currX = 0;
	int currY = 0;
	int lastX = 0, lastY = 0;
	int nextX = 0;
	int nextY = 0;
	int walkStep = 0;
	int checkCounter = 0;
	int notExistsCounter = 0;
	int walkSleepTime = 0;
	BOOL isMapOpened = FALSE;
	BOOL hasNextStep = TRUE;
	int runTotal = 0;
	BOOL isMousePressForWalk = FALSE;
	//统计
	ai->saleCounter = 0;
	ai->healCounter = 0;
	ai->resetCounter = 0;
	ai->talkCounter = 0;
	ai->backToCityCounter = 0;
	
	ai->fightTimes = 0;
	ai->fightRound = 0;
	ai->isFighting = FALSE;
	//End统计
	//leader->getScreen()->startAutoRefresh();
	ai->script.command = CScript::UNKNOW;
	ai->script.resetPos();
	ai->startTime = CTime::GetCurrentTime();
	ai->writeLog(TEXT("开始脚本"));
	CSystem::resetCounter();
	//ai->fuckingMouse();
	while(ai->isAIStart && hasNextStep)
	{
		if(ai->script.command == CScript::UNKNOW) {
			hasNextStep = ai->script.nextStep();
		}
		if(!leader->getScreen()->isFocus() && 
			ai->script.command != CScript::START_GAME
			&& ai->script.command != CScript::TIME
			&& ai->script.command != CScript::AGAIN
			&& ai->isGameClosed == FALSE)
		{
			Sleep(500);
			TRACE("No focus... command[%d]\n", ai->script.command);
			continue;
		}
		
		if(ai->script.command != CScript::START_GAME
			&& ai->script.command != CScript::CLOSE_GAME
			&& ai->script.command != CScript::TIME
			&& !isMapOpened && leader->topRightWindow->isExists())
		{
			Sleep(500);
			ai->checkHPAndMP();
			leader->topRightWindow->openMap();
			isMapOpened = TRUE;
			TRACE("Map is opened!\n");
			Sleep(500);
		}

		if(ai->script.command == CScript::WALK || ai->script.command == CScript::CHANGE_MAP)
		{
			if(leader->mapWindow->isExists())
			{
				if(ai->isFighting == TRUE)
				{
					ai->isFighting = FALSE;
					++ai->fightTimes;
					isMousePressForWalk = FALSE;
					leader->mapWindow->leftClickCenter();
					//防断线
					ai->fuckingNP();

					notExistsCounter = 0;
					Sleep(200);
					continue;
				}

				mapWindowCheckCount = 0;
				currX = leader->mapWindow->getX();
				currY = leader->mapWindow->getY();
			}
			else
			{
				if(leader->isOffline())
				{
					logTmp.Format(TEXT("执行脚本命令[%d] 下一坐标(%d,%d) 目标坐标(%d,%d) 时断线, 断线前坐标(%d,%d)")
						, ai->script.command, ai->script.x, ai->script.y
						, ai->script.targetX, ai->script.targetY
						, lastX, lastY);
					ai->writeLog(logTmp);
					ai->isAIStart = FALSE;
					break;
				}
				++mapWindowCheckCount;
				
				if(mapWindowCheckCount < 5)
				{
					++notExistsCounter;
					Sleep(1000);
					continue;
				}
			}
		}// Travel
		
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
			if(mapWindowCheckCount > 0)
				break;
			if(currX == ai->script.x && currY == ai->script.y)
			{
				//leader->mapWindow->move2center();
				logTmp.Format(TEXT("已经走到达目标坐标(%d,%d)"), ai->script.x, ai->script.y);
				ai->writeLog(logTmp);
				hasNextStep = ai->script.nextStep();
				break;
			}

			nextX = ai->script.x - currX;
			nextY = ai->script.y - currY;
				
				
			walkStep = leader->mapWindow->goNext(nextX, nextY);
			walkSleepTime = 0;
			/*if((nextX != 0 && abs(nextX) < 8 && nextY == 0)
				|| (nextY != 0 && abs(nextY) < 8 && nextX == 0))
			{
				TRACE("Sleep more................................\n");
				walkSleepTime = (WALK_INTERVAL - 80)*walkStep;
			}*/
			walkSleepTime += walkStep*WALK_INTERVAL;
			if(walkStep < 2)
				walkSleepTime += (WALK_INTERVAL)*2;
			else if(walkStep < 4)
				walkSleepTime += (WALK_INTERVAL+100)*walkStep;
			TRACE("Walk Sleep Time: %d\n", walkSleepTime);
			Sleep(walkSleepTime);
			/*
			if(!isMousePressForWalk)
			{
				isMousePressForWalk = TRUE;
				CSystem::leftPress(0, 0);
			}
				
			if( (nextX != 0 && abs(nextX) < 5 )
				|| (nextY != 0 && abs(nextY) < 5 )
				)
			{
				Sleep(250);
				leader->mapWindow->centerXY(&tmpX, &tmpY);
				if(nextX < 0)
				{
					tmpX -= 10;
					tmpY += 10;
				}
				SetCursorPos(tmpX, tmpY);
				Sleep(walkStep*WALK_INTERVAL);
			}
			*/
			//重置机制
			if(lastX == currX && lastY == currY)
			{
				++checkCounter;
				//Sleep(500);
			}
			else
				checkCounter = 0;
			if(checkCounter > 20)
			{
				isMousePressForWalk = FALSE;
				ai->writeLog(TEXT("发现人物卡死，尝试重新开始"));
				ai->script.resetPos();
				hasNextStep = ai->script.nextStep();
				continue;
			} else if(checkCounter > 2)
			{
				isMousePressForWalk = FALSE;
			}
			lastX = currX;
			lastY = currY;
			//end 重置机制
			continue;
		case CScript::HEAL:
			ai->doHeal();
			++ai->healCounter;
			hasNextStep = ai->script.nextStep();
			break;
		case CScript::AGAIN:
			ai->writeLog(TEXT("脚本重来"));
			ai->script.resetPos();
			hasNextStep = ai->script.nextStep();
			TRACE("===============  Again  ==================\n");
			++ai->resetCounter;
			break;
		case CScript::FIND_ENEMY:
			ai->doFindEnemy();
			hasNextStep = ai->script.nextStep();
			break;
		case CScript::TALK:
			ai->doTalk();
			hasNextStep = ai->script.nextStep();
			++ai->talkCounter;
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
			++ai->saleCounter;
			break;
		case CScript::TIME:
			ai->doTime();
			hasNextStep = ai->script.nextStep();
			break;
		case CScript::TEST:
			ai->doTest();
			hasNextStep = ai->script.nextStep();
			break;
		case CScript::CLOSE_GAME:
			ai->doCloseGame();
			hasNextStep = ai->script.nextStep();
			break;
		case CScript::START_GAME:
			ai->doStartGame();
			hasNextStep = ai->script.nextStep();
			isMapOpened = FALSE;
			break;
		} // end switch
		if(hasNextStep == FALSE)
			return 0;
		if(notExistsCounter > 1)
			ai->autoFight();
		TRACE("command: %d, current (%d,%d), next: (%d,%d), target: (%d,%d)\n",
			ai->script.command, currX, currY, 
			ai->script.x, ai->script.y, ai->script.targetX, ai->script.targetY);
		continue;
	}
	ai->endTime = CTime::GetCurrentTime();
	runTotal = ai->endTime.GetTime() - ai->startTime.GetTime();
	logTmp.Format(TEXT("结束脚本，总运行时间: %02d:%02d:%02d, 左键点击次数: %d, 右键点击次数: %d, 键盘按键次数: %d, 回城次数: %d, 治疗次数: %d, 对话次数: %d, 脚本重复次数: %d, 售卖次数: %d, 战斗数: %d, 回合数: %d\r\n\r\n")
		, runTotal / 3600, runTotal / 60 % 60, runTotal % 60, CSystem::getLeftClickCounter()
		, CSystem::getRightClickCounter(), CSystem::getSendKeyCounter()
		, ai->backToCityCounter, ai->healCounter, ai->talkCounter, ai->resetCounter, ai->saleCounter
		, ai->fightTimes, ai->fightRound);
	ai->writeLog(logTmp);
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
	static int lastSkill = 0;
	int numOfMonster = leader->monster->countAlive();
	CString petSkill;
	petSkill.Format(NUMBER_OF_MONSTER_PET_SKILL, numOfMonster);
	int skillNumber = GetPrivateProfileInt(FIGHT_SKILL, petSkill.GetBuffer(20), 1, CONFIG_FILE);
	if(skillNumber != lastSkill)
	{
		leader->petCommandWindow->clickSkillCommand();
		Sleep(FIGHT_INTERVAL);
	}
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
	++fightRound;
	lastSkill = skillNumber;
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
	makeSureLocation();

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
	fuckingNP();
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
	makeSureLocation();
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
	fuckingNP();
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
	int goodsCounter = 0;
	int nowMinu = 0;
	int failCounter = 0;
	CString logTmp;
	writeLog(TEXT("开始遇敌"));
	//统计
	int fightingCounter = 0;
	int fightingRound = fightRound;
	//end 统计
	while(isAIStart)
	{
		if(pMap->isExists())
		{
			
			if(isFighting == TRUE)
			{
				isFighting = FALSE;
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
				Sleep(300);
				pMap->leftClickCenter();

				//防断线
				fuckingNP();

				notExistCounter = 0;
				Sleep(200);
				++fightingCounter;
				++fightTimes;
				//战斗超过10次才检查物品
				if(fightingCounter > 6)
				{
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
	logTmp.Format(TEXT("战斗结束，一共战斗%d次, %d回合"), fightingCounter, fightRound-fightingRound);
	writeLog(logTmp);
}



void CGameAI::autoFight(void)
{
	if(leader->getScreen()->isFocus() && !leader->mapWindow->isExists())
	{
		if(leader->playerCommandWindow->isExists())
		{
			isFighting = TRUE;
			playerFight();
			Sleep(FIGHT_INTERVAL);
		}// fight
		else if(leader->petCommandWindow->isExists())
		{
			petFight();
			//sayRandom();
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
	int currX = 0;
	int currY = 0;
	makeSureLocation();
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
	fuckingNP();
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
		++backToCityCounter;
		Sleep(TALK_INTERVAL);
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
	TRACE("Do Time.\n");
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
	const DWORD unicode = 0xFEFF;
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
		if(isOk)
		{
			file.Write(&unicode, 2);
		}
	}
	if(isOk)
	{
		file.SeekToEnd();
		fmtStr.Format(TEXT("%s - %s\r\n"), nowTime.Format("%Y-%m-%d %H:%M:%S"), str);
		file.Write(fmtStr, fmtStr.GetLength()*sizeof(TCHAR));
		file.Close();
	}
	
}

void CGameAI::sayAgain()
{
	CSystem::sendKey(VK_UP);
	Sleep(500);
	CSystem::sendKey(VK_RETURN);
}

void CGameAI::sayRandom()
{
	int key ;
	int len = rand() % 10;
	for(int i = 0; i < len; ++i)
	{
		key = 'A' + rand() % 26;
		CSystem::sendKey(key);
		
	}
	for(int i = 0; i < len; ++i)
	{
		CSystem::sendKey(VK_SPACE);
	}
	CSystem::sendKey(VK_RETURN);
}

void CGameAI::doTest()
{
	srand(time(NULL));
	RECT rect = {0};
	BOOL done = FALSE;
	while(!done && isAIStart)
	{
		rightClickTager(script.x, script.x);
		Sleep(TALK_INTERVAL*20 + TALK_INTERVAL * (rand() % 10));
		//fuckingMouse();
		
		if(leader->bottomWindow->isExists())
		{
			leader->bottomWindow->openSystemWindow();
			Sleep(TALK_INTERVAL*4);
			leader->bottomWindow->closeSystemWindow();
			break;
		}
	}
}

void CGameAI::makeSureLocation()
{
	int currX = 0;
	int currY = 0;
	int step = 0;
	leader->mapWindow->leftClickCenter();
	Sleep(WALK_INTERVAL*2);
	while(TRUE)
	{
		currX = leader->mapWindow->getX();
		currY = leader->mapWindow->getY();
		if(currX != script.lastX || currY != script.lastY)
		{
			step = leader->mapWindow->goNext(script.lastX - currX, script.lastY - currY);
			Sleep(step * WALK_INTERVAL*3);
		}
		else
			break;
	}
}

void CGameAI::fuckingNP()
{
	/*
	int fuckingTimes = saleCounter + healCounter + talkCounter
		+ backToCityCounter + fightTimes;
	CTime now = CTime::GetCurrentTime();
	int runTotal = now.GetTime() - startTime.GetTime();
	int needSpeedSec = fuckingTimes * (3600 / 40);
	int notRightSec = needSpeedSec - runTotal;
	TRACE("Too fast, Sleep. times %d, now need sec: %d, run total: %d, not right sec: %d "
		,fuckingTimes, needSpeedSec, runTotal, notRightSec);
	if(notRightSec > 0)
	{
		
		while(notRightSec > 0 && isAIStart)
		{
			TRACE(".");
			Sleep(1000);
			--notRightSec;
		}
		TRACE("\n");
	}
	*/
}

void CGameAI::doCloseGame()
{
	TRACE("Doing close the game\n");
	if(leader->getHWND() == NULL)
	{
		Sleep(1000);
		return;
	}
	if(leader->getScreen()->isFocus())
	{
		CSystem::leftClick(630, 16);
		Sleep(1000);
		CSystem::sendKey(VK_SPACE);
		Sleep(1000);
		leader->setHWND(NULL);
		isGameClosed = TRUE;
	}
	else
	{
		Sleep(500);
	}
}

BOOL CALLBACK EnumChildWindowCallBack(HWND hWnd, LPARAM lParam)  
{  
	DWORD dwPid = 0;
	PDWORD pDwPid = (PDWORD)lParam;
	TCHAR buff[MAX_PATH] = {0};
	GetWindowThreadProcessId(hWnd, &dwPid); // 获得找到窗口所属的进程  
	if(dwPid == *pDwPid) // 判断是否是目标进程的窗口  
	{  
		//TRACE("0x%08X    ", hWnd); // 输出窗口信息  
		//TCHAR buf[WINDOW_TEXT_LENGTH];  
		//SendMessage(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)buf);  
		//wprintf(L"%s/n", buf);  
		//EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);    // 递归查找子窗口 
		GetWindowText(hWnd, buff, MAX_PATH);
		OutputDebugString(buff);
		OutputDebugString(TEXT("\n"));
		if(_tcsncmp(buff, TEXT("下一步"), lstrlen(TEXT("下一步"))) == 0)
		{
			*pDwPid = 0;
			return FALSE;
		}
	}  
	return TRUE;  
} 

BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)  
{  
	DWORD dwPid = 0;
	PDWORD pDwPid = (PDWORD)lParam;
	GetWindowThreadProcessId(hWnd, &dwPid); // 获得找到窗口所属的进程  
	if(dwPid == *pDwPid) // 判断是否是目标进程的窗口  
	{  
		//TRACE("0x%08X    ", hWnd); // 输出窗口信息  
		//TCHAR buf[WINDOW_TEXT_LENGTH];  
		//SendMessage(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)buf);  
		//wprintf(L"%s/n", buf);  
		EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);    // 继续查找子窗口
		if(*pDwPid == NULL)
		{
			*pDwPid = (DWORD)hWnd;
			return FALSE;
		}
	}
	return TRUE;  
} 

BOOL CALLBACK enterUsernameAndPassword( HWND hwnd, LPARAM lParam )
{
	TCHAR buff[MAX_PATH] = {0};
	GetClassName(hwnd, buff, MAX_PATH);
	if(_tcsncmp(buff, TEXT("Edit"), lstrlen(TEXT("Edit"))) == 0)
	{
		TRACE("Edit\n");
		PostMessage(hwnd, WM_KEYUP, 'D', NULL);
	}
	return TRUE;
}

BOOL CALLBACK loginPlatformLeftClickNextStep(HWND hwnd, LPARAM lParam)
{
	TCHAR buff[MAX_PATH] = {0};
	RECT rect = {0};
	GetWindowText(hwnd, buff, MAX_PATH);
	//TRACE("find next step button\n");
	if(_tcsncmp(buff, TEXT("下一步"), lstrlen(TEXT("下一步"))) == 0)
	{
		GetWindowRect(hwnd, &rect);
		DEBUG_RECT(rect);
		PostMessage(hwnd, WM_LBUTTONUP, 0, 0x100010);
		//PostMessage(hwnd, WM_KEYUP, VK_RETURN, NULL);
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK FindCrossGateWindowsProc4Login(HWND hwnd, LPARAM lParam)
{
	TCHAR buff[MAX_PATH] = {0};
	GetWindowText(hwnd, buff, MAX_PATH);
	if(_tcsncmp(buff, CROSS_GATE_CN, lstrlen(CROSS_GATE_CN)) == 0)
	{
		*((HWND*)lParam) = hwnd;
		return FALSE;
	}
	*((HWND*)lParam) = NULL;
	return TRUE;
}

void CGameAI::doStartGame()
{
	RECT rect;
	DWORD pId = NULL;

	HWND ywtHWND = NULL;
	HWND crossGameHWND = NULL;
	//HWND ywtHWND = FindWindow(TEXT("#32770"), NULL);
	TRACE("game hwnd: %d\n", leader->getHWND());
	if(leader->getHWND() != NULL)
		return;
	ywtHWND = FindWindow(NULL, TEXT("\u201c\u6613\u73a9\u901a\u201d\u5a31\u4e50\u5e73\u53f0"));
	if(ywtHWND != INVALID_HANDLE_VALUE)
	{
		TRACE("Doing start the game\n");
		GetWindowThreadProcessId(ywtHWND, &pId);
		EnumWindows(EnumWindowCallBack, (LPARAM)&pId);
		ywtHWND = (HWND)pId;
		
		//ShowWindow(ywtHWND, SW_SHOWMINNOACTIVE);
		//Sleep(500);
		//ShowWindow(ywtHWND, SW_RESTORE);

		ShowWindow(ywtHWND, SW_SHOWNORMAL);
		Sleep(500);
		//SetForegroundWindow(ywtHWND);
		//Sleep(1000);
		
		
		//EnumChildWindows(ywtHWND, enterUsernameAndPassword, NULL);
		SetForegroundWindow(ywtHWND);
		Sleep(1000);
		//EnumChildWindows(ywtHWND, loginPlatformLeftClickNextStep, NULL);
		//PostMessage(ywtHWND, WM_KEYUP, VK_RETURN, NULL);
		CSystem::sendKey(VK_RETURN);
		Sleep(1000);
		CSystem::sendKey(VK_RETURN);
		Sleep(5000);
		EnumWindows(FindCrossGateWindowsProc4Login, (LPARAM)&crossGameHWND);
		if(crossGameHWND)
		{
			isGameClosed = FALSE;
			leader->setHWND(crossGameHWND);
			SetForegroundWindow(ywtHWND);
			Sleep(1000);
			leader->mapWindow->leftClickCenter();
			Sleep(2000);
			CSystem::leftClick(156, 148);
			Sleep(1000);
			CSystem::leftClick(156, 276);
			Sleep(100);
			CSystem::leftClick(156, 276);
			rect.left = 142;
			rect.top = 352;
			rect.right = 184;
			rect.bottom = 362;
			while(leader->getScreen()->colorDeviation(&rect, RGB(255,255,255)) < 4)
				Sleep(500);
			CSystem::leftClick(162, 358);
			Sleep(8000);
			
		}
	}
}
