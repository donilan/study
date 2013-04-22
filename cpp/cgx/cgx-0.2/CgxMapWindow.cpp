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

int CCgxMapWindow::_calcNextPoint(const int nextX, const int nextY, PPOINT pPoint)
{
	int walkStep = 0;
	int flagX = 0;
	int flagY = 0;
	if(nextX > 0)
	{
		flagX = 1;
		flagY = -1;
		walkStep = nextX;
	} 
	else if(nextX < 0)
	{
		flagX = -1;
		flagY = 1;
		walkStep = abs(nextX);
	} else if(nextY > 0)
	{
		flagX = 1;
		flagY = 1;
		walkStep = nextY;
	} else
	{
		flagX = -1;
		flagY = -1;
		walkStep = abs(nextY);
	}

	

	if(walkStep < 0)
		walkStep = 0;
	if(walkStep > 6)
		walkStep = 6;
	int centerX = 0;
	int centerY = 0;
	centerXY(&centerX, &centerY);
	pPoint->x = centerX + flagX * walkStep * 32;
	pPoint->y = centerY + flagY * walkStep * 25;
	return walkStep;
}



int CCgxMapWindow::goNext(int nextX, int nextY)
{
	POINT p = {0};
	int walkStep = _calcNextPoint(nextX, nextY, &p);
	CSystem::leftClick(p.x, p.y);
	return walkStep;
}


void CCgxMapWindow::leftClickCenter(void)
{
	int x;
	int y;
	centerXY(&x, &y);
	CSystem::leftClick(x, y);
}


int CCgxMapWindow::moveMouse(int nextX, int nextY)
{
	POINT p = {0};
	int walkStep = _calcNextPoint(nextX, nextY, &p);
	SetCursorPos(p.x, p.y);
	return walkStep;
}
