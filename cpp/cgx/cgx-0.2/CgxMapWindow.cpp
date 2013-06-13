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

#define MAX_STEP 4
int CCgxMapWindow::_calcNextPoint(int nextX, int nextY, PPOINT pPoint)
{
	if(nextX > MAX_STEP)
		nextX = MAX_STEP;
	else if(nextX < -MAX_STEP)
		nextX = -MAX_STEP;
	if(nextY > MAX_STEP)
		nextY = MAX_STEP;
	else if(nextY < -MAX_STEP)
		nextY = -MAX_STEP;

	int centerX = 0;
	int centerY = 0;
	centerXY(&centerX, &centerY);
	if(nextX == 0 && nextY == 0)
	{
		pPoint->x = centerX;
		pPoint->y = centerY;
		return 0;
	}
	else if(nextX == 0)
	{
		if(nextY > 0)
		{
			pPoint->x = centerX + 31 * nextY;
			pPoint->y = centerY + 24 * nextY;
		}
		else
		{
			pPoint->x = centerX + 31 * nextY;
			pPoint->y = centerY + 24 * nextY;
		}
		return abs(nextY);
	}
	else if(nextY == 0)
	{
		if(nextX > 0)
		{
			pPoint->x = centerX + 31 * nextX;
			pPoint->y = centerY - 24 * nextX;
		}
		else
		{
			pPoint->x = centerX + 31 * nextX;
			pPoint->y = centerY - 24 * nextX;
		}
		return abs(nextX);
	}
	else if(nextX > 0 && nextY > 0)
	{
		pPoint->x = centerX + 62 * min(nextX, nextY);
		pPoint->y = centerY;
		return min(nextX, nextY);
	}
	else if(nextX > 0 && nextY < 0)
	{
		pPoint->x = centerX;
		pPoint->y = centerY  - 43 * min(nextX, -nextY);
		return min(nextX, -nextY);
	}
	else if(nextX < 0 && nextY > 0)
	{
		pPoint->x = centerX;
		pPoint->y = centerY  + 43 * min(-nextX, nextY);
		return  min(-nextX, nextY);
	}
	else if(nextX < 0 && nextY < 0)
	{
		pPoint->x = centerX - 62 * min(-nextX, -nextY);
		pPoint->y = centerY;
		return min(-nextX, -nextY);
	}
	return 0;
}
/*
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
*/


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

void CCgxMapWindow::move2center()
{
	int x;
	int y;
	centerXY(&x, &y);
	SetCursorPos(x, y);
}

void CCgxMapWindow::getLocateRECT( RECT* rectOut )
{
	rectOut->left = 500;
	rectOut->top = 40;
	rectOut->right = 600;
	rectOut->bottom = 100;
}