#pragma once

#include "cgxData.h"

#define MONSTER_ALIVE 1
#define MONSTER_DEAD 0
class CMonsterHoler
{
public:
	CMonsterHoler(void);
	~CMonsterHoler(void);

	void setAlive(int idx);
	void resetAlive();

	PGAME_LOCATION get(int idx);
	int getOneAlive();
	BOOL isAlive(int idx);

	int getAliveNumber();
	int alive[MAX_MONSTER_LOCATION];
	GAME_LOCATION monsterLocations[MAX_MONSTER_LOCATION];
};

