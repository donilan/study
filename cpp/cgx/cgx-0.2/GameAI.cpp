#include "StdAfx.h"
#include "GameAI.h"

#define FIGHT_INTERVAL 400


CGameAI::CGameAI( CGame* game)
{
	this->leader = game;
	isAutoFighting = FALSE;
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

UINT CGameAI::gameAIAutoFightThread(LPVOID lpVoid)
{
	CGameAI *ai = (CGameAI *) lpVoid;
	CGame* leader = ai->getLeader();
	leader->getScreen()->startAutoRefresh();
	while(ai->isAutoFighting)
	{
		if(leader->playerCommandWindow->isExists())
		{
			ai->playerFight();
			Sleep(FIGHT_INTERVAL);
			ai->petFight();
		}
		if(leader->petCommandWindow->isExists())
		{
			ai->petFight();
		}
		Sleep(500);
	}
	return 0;
}

void CGameAI::startAutoFight(void)
{
	TRACE("Start auto fight.\n");
	if(isAutoFighting) return;
	isAutoFighting = TRUE;
	AfxBeginThread(gameAIAutoFightThread, this);
}


void CGameAI::stopAutoFight(void)
{
	TRACE("Stop auto fight.\n");
	isAutoFighting = FALSE;
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
