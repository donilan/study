#include "StdAfx.h"
#include "GameAI.h"

#define FIGHT_INTERVAL 400


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
	int command;
	int nextX = 0;
	int nextY = 0;
	//leader->getScreen()->startAutoRefresh();
	while(ai->isAIStart)
	{
		if(leader->mapWindow->isExists())
		{
			if(nextX == 0) {
				ai->script.nextStep(&command, &nextX, &nextY);
			}

			TRACE("%d, %d  ==   %d,%d\n", nextX, nextY, leader->mapWindow->getX(), leader->mapWindow->getY());
		}
		Sleep(500);
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
