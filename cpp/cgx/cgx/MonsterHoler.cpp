#include "StdAfx.h"
#include "MonsterHoler.h"


CMonsterHoler::CMonsterHoler(void)
{
	memcpy(this->monsterLocations, MONSTER_LOCATIONS, sizeof(GAME_LOCATION)*MAX_MONSTER_LOCATION);
	memset(this->alive, 0, sizeof(int)*MAX_MONSTER_LOCATION);
}


CMonsterHoler::~CMonsterHoler(void)
{
}

void CMonsterHoler::setAlive( int idx )
{
	alive[idx] = MONSTER_ALIVE;
}

PGAME_LOCATION CMonsterHoler::get( int idx )
{
	return &monsterLocations[idx];
}

int CMonsterHoler::getOneAlive()
{
	for(int i = MAX_MONSTER_LOCATION-1; i >= 0; --i)
	{
		if(alive[i] == MONSTER_ALIVE)
		{
			return i;
		}
	}
	return -1;
}

void CMonsterHoler::resetAlive()
{
	memset(this->alive, 0, sizeof(int)*MAX_MONSTER_LOCATION);
}

BOOL CMonsterHoler::isAlive( int idx )
{
	if( idx < 0 || idx > MAX_MONSTER_LOCATION-1)
		return FALSE;
	return alive[idx] == 1;
}

int CMonsterHoler::getAliveNumber()
{
	int cnt = 0;
	for(int i = 0; i < MAX_MONSTER_LOCATION; ++i)
	{
		if(isAlive(i))
			++cnt; 
	}
	return cnt;
}
