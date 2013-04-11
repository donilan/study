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
	POINT point = {x, y};
	AfxBeginThread(CSystem::LeftClickThread, &point);
	return TRUE;
}

UINT CSystem::LeftClickThread( LPVOID lPvoid)
{
	CSystem::lockScreen(TRUE);
	POINT oldPoint;
	GetCursorPos(&oldPoint);
	LPPOINT p = (LPPOINT) lPvoid;
	SetCursorPos(p->x, p->y);
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTDOWN, p->x, p->y, 0, 0);
	Sleep(10);
	mouse_event(MOUSEEVENTF_LEFTUP, p->x, p->y, 0, 0);
	SetCursorPos(oldPoint.x, oldPoint.y);
	CSystem::lockScreen(FALSE);
	return 0;
}
