#include "StdAfx.h"
#include "System.h"


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
	POINT* point = (POINT*)malloc(sizeof(POINT));
	point->x = x;
	point->y = y;
	AfxBeginThread(CSystem::LeftClickThread, point);
	return TRUE;
}

UINT CSystem::LeftClickThread( LPVOID lPvoid)
{
	CSystem::lockScreen(TRUE);
	POINT oldPoint;
	GetCursorPos(&oldPoint);
	LPPOINT p = (LPPOINT) lPvoid;
	TRACE("Lect click: (%d, %d)\n", p->x, p->y);
	SetCursorPos(p->x, p->y);
	Sleep(20);
	mouse_event(MOUSEEVENTF_LEFTDOWN, p->x, p->y, 0, 0);
	Sleep(20);
	mouse_event(MOUSEEVENTF_LEFTUP, p->x, p->y, 0, 0);
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
