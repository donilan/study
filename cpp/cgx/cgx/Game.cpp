#include "StdAfx.h"
#include "Game.h"
#include <ctime>

#include "log.h"

#define RANDOM(num) rand()%num
#define KEY_PRESS_INTERVAL 100

CGame::CGame(HWND hwnd)
{
	LOG_DEBUG << "Game init with hwnd: " << hwnd;
	this->hwnd = hwnd;
	this->pImageDC = NULL;
	this->pImage = NULL;
	this->autoRefresh = FALSE;
	this->speedUpWalk = FALSE;
	this->speedUpBySpeak = FALSE;
	this->status = GAME_STATUS_UNKNOW;
	interval4screenshot = 3000;
	interval4autoFighting = 1000;
	interval4speedUpWalk = 500;
	interval4speedUpSpeak = 200;
	srand((unsigned) time(NULL));
	this->hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	memcpy(this->monsterLocations, MONSTER_LOCATIONS, sizeof(GAME_LOCATION)*MAX_MONSTER_LOCATION);
}


CGame::~CGame(void)
{
	LOG_DEBUG << "Deleting Game object.";
	DeleteDC(this->hScreenDC);
	if(pImage)
		delete pImage;
	if(pImageDC)
		delete pImageDC;
	if(hScreenDC)
		DeleteDC(hScreenDC);
}

BOOL CGame::sendKey( int key )
{
	if(isFocus())
	{
		keybd_event(key, 0, 0, 0);
		Sleep(KEY_PRESS_INTERVAL);
		keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
		return TRUE;
	}
	return FALSE;
}

BOOL CGame::sendKeyWithCtl( int key )
{
	if(isFocus())
	{
		keybd_event(VK_CONTROL, 0, 0, 0);
		keybd_event(key, 0, 0, 0);
		Sleep(KEY_PRESS_INTERVAL);
		keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	
		return TRUE;
	}
	return FALSE;
}

BOOL CGame::leftClick( int x, int y )
{
	LOG_DEBUG << "mouse move to " << x << "," << y;
	SetCursorPos(x, y);
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);

	return TRUE;
}

BOOL CGame::rightClick( int x, int y )
{
	return FALSE;
}

BOOL CGame::doFace( int faceType )
{
	return this->sendKeyWithCtl(KV_1+faceType);
}

BOOL CGame::say(const char* text)
{
	for(unsigned int i = 0; i < strlen(text); ++i)
	{
		sendKey((int)text[i]);
	}
	sendKey(VK_RETURN);
	return TRUE;
}

BOOL CGame::reSay()
{
	sendKey(VK_UP);
	sendKey(VK_RETURN);
	return TRUE;
}

BOOL CGame::focus()
{
	ShowWindow(hwnd, SW_SHOWNORMAL);
	SetFocus(this->hwnd);
	SetForegroundWindow(this->hwnd);
	return TRUE;
}

BOOL CGame::doRandomFace()
{
	return this->doFace(RANDOM(5));
}

BOOL CGame::getRect( RECT* rect)
{
	return GetWindowRect(hwnd, rect);
}

BOOL CGame::refresh()
{
	focus();
	if(pImageDC)
		delete pImageDC;
	if(pImage)
		delete pImage;

	RECT rect;
	getRect(&rect);
	int width = rect.right-rect.left;
	int height = rect.bottom-rect.top;
	pImage = new CImage();
	pImage->Create(width, height, 24);
	pImageDC = new CImageDC(*pImage);

	Sleep(10);
	BitBlt(*pImageDC, 0, 0, width, height, hScreenDC, rect.left, rect.top, SRCCOPY);

	windowHeight = pImage->GetHeight();
	windowWidth = pImage->GetWidth();
	LOG_DEBUG << "Refresh Done, image width: " << windowWidth << ", height: " << windowHeight;
	return TRUE;
}

BOOL CGame::saveScreenshot2file(LPCTSTR pszFilename)
{
	pImage->Save(pszFilename);
	return TRUE;
}

BOOL CGame::isFocus()
{
	return GetForegroundWindow() == this->hwnd;
}

BOOL CGame::caclImage()
{
	BYTE *pBytes = (BYTE *)pImage->GetBits();
	
	for(int i = 0; i < windowWidth; ++i)
	{
		LOG_DEBUG << (int)pBytes[i*3+2] << " " << (int)pBytes[i*3+1] << " " << (int)pBytes[i*3] ;
	}
	
	return TRUE;
}

BOOL CGame::startAutoRefresh()
{
	if(this->autoRefresh)
		return FALSE;

	this->autoRefresh = TRUE;
	AfxBeginThread(AutoRefreshThread, this);
	return TRUE;
}

BOOL CGame::stopAutoRefresh()
{
	this->autoRefresh = FALSE;
	return TRUE;
}

BOOL CGame::isAutoRefresh()
{
	return this->autoRefresh;
}

BOOL CGame::isAutoFighting()
{
	return autoFighting;
}

BOOL CGame::startAutoFighting()
{
	if(this->autoFighting)
		return FALSE;

	this->autoFighting = TRUE;
	AfxBeginThread(AutoFightingThread, this);
	return TRUE;
}

BOOL CGame::stopAutoFighting()
{
	this->autoFighting = FALSE;
	return TRUE;
}

BOOL CGame::isSpeedUpWalk()
{
	return speedUpWalk;
}

BOOL CGame::stopSpeedUpWalk()
{
	this->speedUpWalk = FALSE;
	return TRUE;
}

BOOL CGame::startSpeedUpWalk()
{
	if(this->speedUpWalk)
		return FALSE;

	this->speedUpWalk = TRUE;
	AfxBeginThread(AutoSpeedUpWalkThread, this);
	return TRUE;
}

PGAME_LOCATION CGame::findMonster()
{
	for(int i = 0; i < MAX_MONSTER_LOCATION; ++i)
	{
		int count = searchRGB(&monsterLocations[i], 255, 255, 255, 5);
		int pixSize = monsterLocations[i].cx * monsterLocations[i].cy;
		LOG_DEBUG << "pix count: " << monsterLocations[i].cx * monsterLocations[i].cy << " found count: " << count;
		if((count*100 / pixSize) > 5)
		{
			monsterLocations[i].status = 1;
		}else{
			monsterLocations[i].status = 0;
		}
	}
	return monsterLocations;
}

int CGame::searchRGB( const PGAME_LOCATION l, int r, int g, int b, int deviation )
{
	LOG_DEBUG << "Searching RGB... width: " << windowWidth << " height: " << windowHeight;
	if(!pImage)
		refresh();

	int result = 0;

	for(unsigned int x = l->x; x < (l->x + l->cx); ++x) 
	{
		for(unsigned int y = l->y; y < (l->y + l->cy); ++y)
		{
			COLORREF color = pImage->GetPixel(x, y);
			
			if(abs(r - GetRValue(color)) < deviation
				&& abs(g - GetGValue(color)) < deviation
				&& abs(b - GetGValue(color)) < deviation)
			{
				result++;
			}
#ifdef DEBUG
			if(x == l->x || x == (l->cx+l->x)
				|| y == l->y || y == (l->cy + l->y))
				pImage->SetPixel(x, y, RGB(255,255,255));
#endif
		}
	}

	return result;
}

int CGame::getStatus()
{
	return this->status;
}

BOOL CGame::checkStatus()
{
	return FALSE;
}

UINT AutoSpeedUpWalkThread(LPVOID pParam)
{
	CGame* game = (CGame *)pParam;
	while(game->isSpeedUpWalk())
	{
		if(game->isFocus()) {
			if(game->speedUpBySpeak)
				game->reSay();
			else
				game->doRandomFace();
			LOG_DEBUG << "Do face for speed up.";
		}
		
		Sleep(game->interval4speedUpWalk);
	}
	return 0;
}

UINT AutoFightingThread(LPVOID pParam)
{
	CGame* game = (CGame *)pParam;
	while(game->isAutoFighting())
	{
		Sleep(game->interval4autoFighting);
	}
	return 0;
}


UINT AutoRefreshThread( LPVOID pParam )
{
	CGame* game = (CGame *)pParam;
	while(game->isAutoRefresh())
	{
		if(game->isFocus())
		{
			game->refresh();
			LOG_DEBUG << "Refresh game screen shot done.";
			if(game->autoTakeScreenshot)
			{
				TCHAR buff[MAX_PATH] = {0};
				swprintf(buff, TEXT("D:\\cgx\\screenshot_%d.bmp"), time(NULL));
				game->saveScreenshot2file(buff);
				LOG_DEBUG << "Save screen shot done.";
			}
		}
		Sleep(game->interval4screenshot);
	}
	return 0;
}