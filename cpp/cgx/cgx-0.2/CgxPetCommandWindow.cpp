#include "StdAfx.h"
#include "CgxPetCommandWindow.h"
#include "resource.h"

CCgxPetCommandWindow::CCgxPetCommandWindow(CHWNDScreen* pScreen)
	: CCgxWindow(IDB_PET_COMMAND, pScreen)
{
}


CCgxPetCommandWindow::~CCgxPetCommandWindow(void)
{
}



void CCgxPetCommandWindow::locateCommands(void)
{
	commandRECTs[0].left = 562;
	commandRECTs[0].top = 78;
	commandRECTs[0].right = commandRECTs[0].left + 12*4;
	commandRECTs[0].bottom = commandRECTs[0].top + 12;
}

BOOL CCgxPetCommandWindow::isCommandEnable(int index)
{
	RECT tmp;
	if(getCommand(index, &tmp))
	{
		if(!(pScreen->colorDeviation(&tmp, RGB(212, 173, 106)) > 40))
			return TRUE;
	}
	return FALSE;
}