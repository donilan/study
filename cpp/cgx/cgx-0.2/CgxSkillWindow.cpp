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
	}
}
