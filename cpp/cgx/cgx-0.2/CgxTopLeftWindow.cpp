#include "StdAfx.h"
#include "CgxTopLeftWindow.h"
#include "resource.h"


CCgxTopLeftWindow::CCgxTopLeftWindow(CHWNDScreen* pScreen)
	: CCgxWindow(IDB_TOP_LEFT_WINDOW, pScreen)
{
}


CCgxTopLeftWindow::~CCgxTopLeftWindow(void)
{
}


void CCgxTopLeftWindow::locateCommands(void)
{

}


void CCgxTopLeftWindow::getLocateRECT(RECT* rectOut)
{
	rectOut->left = 2;
	rectOut->top = 45;
	rectOut->right = 24;
	rectOut->bottom = 70;
}



int CCgxTopLeftWindow::getHP(void)
{
	RECT hpRECT;
	hpRECT.left = this->rect.right + 2;
	hpRECT.right = this->rect.right + 31;
	hpRECT.top = this->rect.top-1;
	hpRECT.bottom = hpRECT.top + 11;
	
	return pScreen->toNumber(&hpRECT);
}


int CCgxTopLeftWindow::getMP(void)
{
	RECT mpRECT;
	mpRECT.left = this->rect.right + 2;
	mpRECT.right = this->rect.right + 31;
	mpRECT.top = this->rect.top + 11;
	mpRECT.bottom = mpRECT.top + 10;
	pScreen->flashRECT(&mpRECT);
	return pScreen->toNumber(&mpRECT);
}
