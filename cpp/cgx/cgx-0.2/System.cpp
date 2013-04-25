#include "StdAfx.h"
#include "System.h"
#include <time.h>
#include <stdlib.h>

CSystem::CSystem(void)
{
}


CSystem::~CSystem(void)
{
}

BOOL CSystem::lockScreen(BOOL lock)
{
	return BlockInput(lock);
}

BOOL CSystem::leftClick(int x, int y)
{
#ifdef DEBUG
	static int leftClickCount = 0;
	TRACE("**Left click counter: %d\n", ++leftClickCount);
#endif
	POINT* point = (POINT*)malloc(sizeof(POINT));
	point->x = x;
	point->y = y;
	AfxBeginThread(CSystem::LeftClickThread, point);
	return TRUE;
}

UINT CSystem::LeftClickThread( LPVOID lPvoid)
{
	//srand(time(NULL));
	CSystem::lockScreen(TRUE);
	POINT oldPoint;
	GetCursorPos(&oldPoint);
	LPPOINT p = (LPPOINT) lPvoid;
	TRACE("Lect click: (%d, %d)\n", p->x, p->y);
	SetCursorPos(p->x, p->y);
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTDOWN, p->x, p->y, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, p->x, p->y, 0, 0);
	Sleep(50);
	//SetCursorPos(30, 400);
	//Sleep(50);
	SetCursorPos(oldPoint.x, oldPoint.y);
	CSystem::lockScreen(FALSE);
	delete p;
	return 0;
}


void CSystem::leftClick(RECT* rect)
{
	int x = (rect->right - rect->left) / 2 + rect->left;
	int y = (rect->bottom - rect->top) / 2 + rect->top;
	leftClick(x, y);
}


int CSystem::ansi2unicode(char* in, size_t inSize, TCHAR* out, size_t outSize)
{
	int size = 0;
	size = MultiByteToWideChar(CP_ACP, NULL, in, inSize, out, outSize);
	out[outSize] = '\0';
	return size;
}


void CSystem::rightClick(int x, int y)
{
	POINT* point = (POINT*)malloc(sizeof(POINT));
	point->x = x;
	point->y = y;
	AfxBeginThread(CSystem::rightClickThread, point);
}

UINT CSystem::rightClickThread( LPVOID lPvoid)
{
	CSystem::lockScreen(TRUE);
	POINT oldPoint;
	GetCursorPos(&oldPoint);
	LPPOINT p = (LPPOINT) lPvoid;
	TRACE("Lect click: (%d, %d)\n", p->x, p->y);
	SetCursorPos(p->x, p->y);
	Sleep(100);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, p->x, p->y, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_RIGHTUP, p->x, p->y, 0, 0);
	Sleep(50);
	SetCursorPos(30, 400);
	Sleep(50);
	SetCursorPos(oldPoint.x, oldPoint.y);
	CSystem::lockScreen(FALSE);
	delete p;
	return 0;
}



void CSystem::leftPress(int x, int y)
{
#ifdef DEBUG
	static int leftPressCount = 0;
	TRACE("*Left press counter: %d\n", ++leftPressCount);
#endif 
	POINT p = {x, y};
	POINT oldPoint = {0};
	CSystem::lockScreen(TRUE);
	GetCursorPos(&oldPoint);
	if(p.x == 0 && p.y == 0)
		memcpy(&p, &oldPoint, sizeof(POINT));
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
	Sleep(1000);
	//SetCursorPos(oldPoint.x, oldPoint.y);
	CSystem::lockScreen(FALSE);
}
