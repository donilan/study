#include "StdAfx.h"
#include "CgxPlayerCommand.h"
#include "resource.h"

CCgxPlayerCommand::CCgxPlayerCommand(CHWNDScreen* pScreen)
	:CCgxWindow(IDB_PLAYER_COMMAND, pScreen)
{
}


CCgxPlayerCommand::~CCgxPlayerCommand(void)
{
}


void CCgxPlayerCommand::locateCommands(void)
{
	for(int i = 0; i < 3; ++i)
	{
		commandRECTs[i].left = rect.left + 4 + 70*i;
		commandRECTs[i].right = commandRECTs[i].left + 57; //23
		commandRECTs[i].top = rect.top + 20;
		commandRECTs[i].bottom = commandRECTs[i].top + 14;
		memcpy(&commandRECTs[i+3], &commandRECTs[i], sizeof(RECT));
		commandRECTs[i+3].top += 25;
		commandRECTs[i+3].bottom += 25;
	}
	memcpy(&commandRECTs[6], &commandRECTs[5], sizeof(RECT));
	commandRECTs[6].left += 70;
	commandRECTs[6].right += 70;
}


BOOL CCgxPlayerCommand::isCommandEnable(int index)
{
	RECT tmp;
	if(getCommand(index, &tmp))
	{
		if(pScreen->colorDeviation(&tmp, RGB(212, 173, 106)) > 40)
			return FALSE;
		else
			return TRUE;
	}
	return FALSE;
}
