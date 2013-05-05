#include "StdAfx.h"
#include "CgxSkillWindow.h"
#include "resource.h"



CCgxSkillWindow::CCgxSkillWindow(int id, CHWNDScreen* screen)
	: CCgxWindow(id, screen)
{
}


CCgxSkillWindow::~CCgxSkillWindow(void)
{
}

void CCgxSkillWindow::locateCommands(void)
{
	for(int i = 0; i < 10; ++i)
	{
		commandRECTs[i].left = rect.left+5;
		commandRECTs[i].right = commandRECTs[i].left+ 12*4;
		commandRECTs[i].top = rect.top+36 + (16*i);
		commandRECTs[i].bottom = commandRECTs[i].top + 12;
		if(pScreen->colorDeviation(&commandRECTs[i], RGB(5, 5, 5)) > 90)
		{
			memset(&commandRECTs[i], 0, sizeof(RECT));
			return;
		}
	}
}



BOOL CCgxSkillWindow::isCommandEnable(int index)
{
	RECT tmp;
	if(getCommand(index, &tmp))
	{
		if(pScreen->colorDeviation(&tmp, RGB(255, 255, 255)) > 10)
			return TRUE;
	}
	return FALSE;
}

void CCgxSkillWindow::getLocateRECT( RECT* rectOut )
{
	rectOut->left = 348;
	rectOut->top = 98;
	rectOut->right = 435;
	rectOut->bottom = 146;
}
