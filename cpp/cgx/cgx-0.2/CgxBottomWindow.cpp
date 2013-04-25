#include "StdAfx.h"
#include "CgxBottomWindow.h"
#include "resource.h"

CCgxBottomWindow::CCgxBottomWindow(CHWNDScreen* pScreen)
	: CCgxWindow(IDB_BOTTOM_WINDOW, pScreen)
{

}


CCgxBottomWindow::~CCgxBottomWindow(void)
{
}

void CCgxBottomWindow::getLocateRECT(RECT* rectOut)
{
	rectOut->left = 28;
	rectOut->top = 488;
	rectOut->right = 48;
	rectOut->bottom = 506;
}

void CCgxBottomWindow::locateCommands( void )
{
	for(int i = 0; i < 7; ++i)
	{
		commandRECTs[i].left = rect.left + 76* i;
		commandRECTs[i].right = commandRECTs[i].left + 24;
		commandRECTs[i].top = rect.top;
		commandRECTs[i].bottom = rect.bottom;
	}
}

BOOL CCgxBottomWindow::isCommandEnable( int index )
{
	RECT tmp;
	if(getCommand(index, &tmp))
	{
		if(pScreen->colorDeviation(&tmp, RGB(173, 166, 156)) > 2)
			return TRUE;
		else
			return FALSE;
	}
	return FALSE;
}

void CCgxBottomWindow::openGoodWindow()
{
	if(!isCommandEnable(2))
		leftClick(2);
}
