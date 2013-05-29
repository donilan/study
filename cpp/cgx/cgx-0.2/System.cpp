#include "StdAfx.h"
#include "System.h"
#include <time.h>
#include <stdlib.h>

#define KEY_PRESS_INTERVAL 100
#define CLICK_RADOM 500

int __leftClickCounter = 0;
int __rightClickCounter = 0;
int __sendKeyCounter = 0;

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
	if(x <= 10 || y <= 10)
		return FALSE;
	++__leftClickCounter;
	POINT* point = (POINT*)malloc(sizeof(POINT));
	point->x = x;
	point->y = y;
	AfxBeginThread(CSystem::LeftClickThread, point);
	return TRUE;
}

UINT CSystem::LeftClickThread( LPVOID lPvoid)
{
	srand(time(NULL));
	CSystem::lockScreen(TRUE);
	POINT oldPoint;
	int randNum =  rand() % CLICK_RADOM;
	GetCursorPos(&oldPoint);
	LPPOINT p = (LPPOINT) lPvoid;
	TRACE("Lect click: (%d, %d)\n", p->x, p->y);
	SetCursorPos(p->x, p->y);
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTDOWN, p->x, p->y, 0, 0);
	TRACE("Check random number: %d\n", randNum);
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


void CSystem::leftClick(const RECT* rect)
{
	//srand(time(NULL));
//	int ranX = rand() % (rect->right - rect->left -4) +2;
//	int ranY = rand() % (rect->bottom - rect->top -4) +2;
	int x = (rect->right - rect->left) / 2 + rect->left;
	int y = (rect->bottom - rect->top) / 2 + rect->top;
	leftClick(x, y);
	//leftClick(rect->left+ranX, rect->top+ranY);
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
	++__rightClickCounter;
	POINT* point = (POINT*)malloc(sizeof(POINT));
	point->x = x;
	point->y = y;
	AfxBeginThread(CSystem::rightClickThread, point);
	
}

UINT CSystem::rightClickThread( LPVOID lPvoid)
{
	srand(time(NULL));
	CSystem::lockScreen(TRUE);
	POINT oldPoint;
	GetCursorPos(&oldPoint);
	LPPOINT p = (LPPOINT) lPvoid;
	TRACE("Lect click: (%d, %d)\n", p->x, p->y);
	SetCursorPos(p->x, p->y);
	Sleep(100);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, p->x, p->y, 0, 0);
	Sleep(100+ rand() % CLICK_RADOM);
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
	Sleep(100);
	//mouse_event(MOUSEEVENTF_RIGHTUP, p.x, p.y, 0, 0);
	//SetCursorPos(oldPoint.x, oldPoint.y);
	CSystem::lockScreen(FALSE);
}

void CSystem::sendKey( int key )
{
	++__sendKeyCounter;
	keybd_event(key, 0, 0, 0);
	Sleep(KEY_PRESS_INTERVAL + rand() % CLICK_RADOM);
	keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
	
}

void CSystem::sendKeyWithCtrl( int key )
{
	++__sendKeyCounter;
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event(key, 0, 0, 0);
	Sleep(KEY_PRESS_INTERVAL+ rand() % CLICK_RADOM);
	keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	
	
}

void CSystem::resetCounter()
{
	__leftClickCounter = 0;
	__sendKeyCounter = 0;
	__rightClickCounter = 0;
}

int CSystem::getLeftClickCounter()
{
	return __leftClickCounter;
}

int CSystem::getRightClickCounter()
{
	return __rightClickCounter;
}

int CSystem::getSendKeyCounter()
{
	return __sendKeyCounter;
}


BOOL CSystem::SafeTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	DWORD dwTID, dwCode, dwErr = 0;
	HANDLE hProcessDup = INVALID_HANDLE_VALUE;
	HANDLE hRT = NULL;
	HINSTANCE hKernel = GetModuleHandle(TEXT("Kernel32"));
	BOOL bSuccess = FALSE;

	BOOL bDup = DuplicateHandle(GetCurrentProcess(), 
		hProcess, 
		GetCurrentProcess(), 
		&hProcessDup, 
		PROCESS_ALL_ACCESS, 
		FALSE, 
		0);

	// Detect the special case where the process is 
	// already dead...
	if ( GetExitCodeProcess((bDup) ? hProcessDup : hProcess, &dwCode) && 
		(dwCode == STILL_ACTIVE) ) 
	{
		FARPROC pfnExitProc;

		pfnExitProc = GetProcAddress(hKernel, "ExitProcess");

		hRT = CreateRemoteThread((bDup) ? hProcessDup : hProcess, 
			NULL, 
			0, 
			(LPTHREAD_START_ROUTINE)pfnExitProc,
			(PVOID)uExitCode, 0, &dwTID);

		if ( hRT == NULL )
			dwErr = GetLastError();
	}
	else
	{
		dwErr = ERROR_PROCESS_ABORTED;
	}


	if ( hRT )
	{
		// Must wait process to terminate to 
		// guarantee that it has exited...
		WaitForSingleObject((bDup) ? hProcessDup : hProcess, 
			INFINITE);

		CloseHandle(hRT);
		bSuccess = TRUE;
	}

	if ( bDup )
		CloseHandle(hProcessDup);

	if ( !bSuccess )
		SetLastError(dwErr);

	return bSuccess;
}

BOOL CSystem::enumChildProc( HWND hwnd, LPARAM lParam )
{
	TCHAR buff[256];
	::GetWindowText(hwnd, buff, 255);
	int nCtrlID = ::GetDlgCtrlID(hwnd);
	OutputDebugString(buff);
	TRACE("ctrl id: %08x \n", nCtrlID);

	return TRUE;
}

