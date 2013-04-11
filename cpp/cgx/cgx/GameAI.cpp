#include "StdAfx.h"
#include "GameAI.h"


CGameAI::CGameAI(void)
{
	gamerNumber = 0;
	for(int i =0 ; i < MAX_GAMER; ++i)
		this->gamer[i] = NULL;
	leader = NULL;

}


CGameAI::~CGameAI(void)
{
}

void CGameAI::join( CGame* g)
{
	if(gamerNumber == 0)
		leader = g;
	gamer[gamerNumber++] = g;
}

void CGameAI::setLeader( CGame * g)
{
	leader = g;
}

BOOL CGameAI::startAutoFighting()
{
	for(int i = 0; i < gamerNumber; ++i)
		gamer[i]->startAutoFighting();
	return TRUE;
}

BOOL CGameAI::stopAutoFighting()
{
	for(int i = 0; i < gamerNumber; ++i)
		gamer[i]->stopAutoFighting();
	return TRUE;
}


CGame* CGameAI::getLeader()
{
	return leader;
}

CGame* CGameAI::getGamer( int idx )
{
	if(idx > -1 && idx < gamerNumber)
		return gamer[idx];
	return NULL;
}

int CGameAI::getGamerSize()
{
	return gamerNumber;
}
