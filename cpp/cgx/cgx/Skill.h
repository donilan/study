#pragma once

#include "cgxData.h"

#define MAX_SKILL_NAME_LENGTH MAX_PATH

class CSkill
{
public:
	CSkill(void);
	~CSkill(void);

	int index;
	int level;
	TCHAR name[MAX_SKILL_NAME_LENGTH];
	GAME_LOCATION location;
};

