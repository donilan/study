#include "StdAfx.h"
#include "GameAI.h"


CGameAI::CGameAI(void)
{
	for(int i =0 ; i < MAX_GAMER; ++i)
		this->gamer[i] = NULL;
	leader = NULL;
}


CGameAI::~CGameAI(void)
{
}
