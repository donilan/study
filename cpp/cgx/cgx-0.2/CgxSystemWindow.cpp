#include "StdAfx.h"
#include "CgxSystemWindow.h"
#include "resource.h"

CCgxSystemWindow::CCgxSystemWindow(CHWNDScreen* screen) 
	: CCgxWindow(IDB_SYSTEM_WINDOW, screen)
{
}


CCgxSystemWindow::~CCgxSystemWindow(void)
{
}

void CCgxSystemWindow::locateCommands( void )
{
	TRACE("System window: left %d, top %d\n", rect.left, rect.top);

	for(int i = 0; i < 5; ++i)
	{
		commandRECTs[i].left = rect.left;
		commandRECTs[i].right = rect.right;
		commandRECTs[i].top = rect.top + 38 + 28*i;
		commandRECTs[i].bottom = commandRECTs[i].top + 10;
		pScreen->flashRECT(&commandRECTs[i]);
	}
	commandRECTs[5].left = rect.left - 185;
	commandRECTs[5].right = commandRECTs[5].left + 80;
	commandRECTs[5].top = rect.top + 18;
	commandRECTs[5].bottom = commandRECTs[5].top + 10;

	commandRECTs[6].left = rect.left - 185;
	commandRECTs[6].right = commandRECTs[6].left + 80;
	commandRECTs[6].top = rect.top + 46;
	commandRECTs[6].bottom = commandRECTs[6].top + 10;
	pScreen->flashRECT(&commandRECTs[5]);
	pScreen->flashRECT(&commandRECTs[6]);
}

void CCgxSystemWindow::clickBackToCity()
{
	leftClick(0);
	Sleep(800);
	leftClick(5);
	Sleep(300);
}
