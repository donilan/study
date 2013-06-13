#include "StdAfx.h"
#include "CgxTopRightWindow.h"
#include "resource.h"
#include "System.h"

CCgxTopRightWindow::CCgxTopRightWindow(CHWNDScreen* pScreen)
	: CCgxWindow(IDB_TOP_RIGHT, pScreen)
{
}


CCgxTopRightWindow::~CCgxTopRightWindow(void)
{
}

void CCgxTopRightWindow::getLocateRECT( RECT* rectOut )
{
	rectOut->left = 405;
	rectOut->top = 30;
	rectOut->right = 435;
	rectOut->bottom = 55;
}

void CCgxTopRightWindow::locateCommands( void )
{
	for(int i = 0; i < 11; ++i)
	{
		commandRECTs[i].left = rect.left + 21* i;
		commandRECTs[i].right = commandRECTs[i].left + 17;
		commandRECTs[i].top = rect.top;
		commandRECTs[i].bottom = rect.bottom;
	}
}

BOOL CCgxTopRightWindow::isCommandEnable( int index )
{
	RECT tmp;
	if(getCommand(index, &tmp))
	{
		if(pScreen->colorDeviation(&tmp, RGB(107, 77, 49)) > 1)
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

void CCgxTopRightWindow::openMap()
{
	if(!isCommandEnable(7))
	{
		//leftClick(7);
		CSystem::sendKeyWithCtrl('S');
	}
}

void CCgxTopRightWindow::disablePets()
{
	if(!isCommandEnable(8))
	{
		leftClick(8);
	}
}
