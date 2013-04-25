#include "StdAfx.h"
#include "CgxBottomWindow.h"
#include "resource.h"

CCgxBottomWindow::CCgxBottomWindow(CHWNDScreen* pScreen)
	: CCgxWindow(IDB_BOTTOM_WINDOW, screen)
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

}