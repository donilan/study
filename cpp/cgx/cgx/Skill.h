#pragma once

#include "cgxData.h"

#define MAX_SKILL_NAME_LENGTH MAX_PATH

#define SKILL_LENGTH 10
#define SKILL_SIGNLE 1
#define SKILL_FOUR_TARGET 4
#define SKILL_ALL_TARGET 10

struct SKILL_CHOICER {
	int signleSkillIdx;
	int fourTargetSkillIdx;
	int allTargetSkillIdx;
};

class CSkill
{
public:

	enum SkillType { 
		SKILL_TYPE_USELESS,
		SKILL_TYPE_ATTACK,
		SKILL_TYPE_HEAL,
		SKILL_TYPE_ASSIST};

	CSkill(void);
	~CSkill(void);

	int index;
	int level;
	int attackNumber;
	SkillType skillType;
	TCHAR name[MAX_SKILL_NAME_LENGTH];
	GAME_LOCATION location;

};

