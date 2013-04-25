#include "StdAfx.h"
#include "CgxGoodsWindow.h"
#include "resource.h"

CCgxGoodsWindow::CCgxGoodsWindow(CHWNDScreen* screen) 
	: CCgxWindow(IDB_GOODS, screen)
{
}


CCgxGoodsWindow::~CCgxGoodsWindow(void)
{
}

void CCgxGoodsWindow::getLocateRECT(RECT* rectOut)
{
	rectOut->left = 420;
	rectOut->top = 180;
	rectOut->right = 580;
	rectOut->bottom = 260;
}

void CCgxGoodsWindow::locateCommands( void )
{

}
