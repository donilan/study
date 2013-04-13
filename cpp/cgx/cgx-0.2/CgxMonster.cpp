#include "StdAfx.h"
#include "CgxMonster.h"


CCgxMonster::CCgxMonster(CHWNDScreen* pScreen)
{
	this->pScreen = pScreen;
	for(int x =0; x < NUMBER_OF_MONSTER/2; ++x)
	{
		monsterRECTs[x].left = 2+ 64*x;
		monsterRECTs[x].right = monsterRECTs[x].left + 80;
		monsterRECTs[x].top = 255 - 36*x;
		monsterRECTs[x].bottom = monsterRECTs[x].top + 14;

		monsterRECTs[x+5].left = monsterRECTs[x].left+52;
		monsterRECTs[x+5].right = monsterRECTs[x+5].left + 80;
		monsterRECTs[x+5].top = monsterRECTs[x].top +50;
		monsterRECTs[x+5].bottom = monsterRECTs[x+5].top + 14;
	}
}


CCgxMonster::~CCgxMonster(void)
{
}


// Find alive
BOOL CCgxMonster::isAlive(int index)
{
	return pScreen->colorDeviation(&monsterRECTs[index], RGB(255, 255, 255)) > 8;
}



int CCgxMonster::countAlive(void)
{
	int count = 0;
	for(int i = 0; i < NUMBER_OF_MONSTER; ++i)
	{
		if(isAlive(i))
		{
			++count;
		}
	}
	return count;
}
