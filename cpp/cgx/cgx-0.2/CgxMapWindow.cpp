#include "StdAfx.h"
#include "CgxMapWindow.h"
#include "resource.h"
#include "System.h"

CCgxMapWindow::CCgxMapWindow(CHWNDScreen* pScreen)
	: CCgxWindow(IDB_MAP, pScreen)
{
	memset(&xRECT, 0, sizeof(RECT));
	memset(&yRECT, 0, sizeof(RECT));
}


CCgxMapWindow::~CCgxMapWindow(void)
{
}

void CCgxMapWindow::locateCommands(void)
{
	xRECT.top = rect.top+22;
	xRECT.bottom = xRECT.top + 8;
	xRECT.left = rect.left - 24;
	xRECT.right = xRECT.left + 24;

	yRECT.top = rect.top+22;
	yRECT.bottom = yRECT.top + 8;
	yRECT.left = rect.left+84;
	yRECT.right = yRECT.left + 24;
}



int CCgxMapWindow::getX(void)
{
	return pScreen->toNumber(&xRECT);
}


int CCgxMapWindow::getY(void)
{
	return pScreen->toNumber(&yRECT);
}


void CCgxMapWindow::goEast(int step)
{
	go(1, -1, step);
}


void CCgxMapWindow::goSouth(int step)
{
	go(1, 1, step);
}


void CCgxMapWindow::goWest(int step)
{
	go(-1, 1, step);
}


void CCgxMapWindow::goNorth(int step)
{
	go(-1, -1, step);
}


void CCgxMapWindow::go(const int flagX, const int flagY, const int step)
{
	int centerX = 0;
	int centerY = 0;
	centerXY(&centerX, &centerY);
	CSystem::leftClick(centerX + flagX * step * 38, centerY + flagY * step * 28);
}
