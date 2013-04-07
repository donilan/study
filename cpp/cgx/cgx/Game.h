#pragma once

#include <windows.h>
#include <iostream>
#include "cgxData.h"

#define KV_1 49


#define GAME_STATUS_UNKNOW 0
#define GAME_STATUS_TRAVEL 1
#define GAME_STATUS_STANDBY 2
#define GAME_STATUS_FIGHTING 4


UINT AutoRefreshThread( LPVOID pParam );
UINT AutoFightingThread( LPVOID pParam );
UINT AutoSpeedUpWalkThread( LPVOID pParam );



class CGame
{
public:
	enum {FACE_SIT, FACE_HELLO};

	CGame(HWND);
	~CGame(void);

	BOOL doFace(int);
	BOOL doRandomFace();
	BOOL sendKey(int);
	BOOL sendKeyWithCtl(int);
	BOOL leftClick(int x, int y);
	BOOL rightClick(int x, int y);
	BOOL say(const char*);
	BOOL reSay();
	BOOL focus();
	BOOL isFocus();
	BOOL getRect(RECT*);
	BOOL refresh();
	BOOL saveScreenshot2file(LPCTSTR);
	BOOL caclImage();

	// auto refresh
	BOOL startAutoRefresh();
	BOOL stopAutoRefresh();
	BOOL isAutoRefresh();

	// auto fighting
	BOOL startAutoFighting();
	BOOL stopAutoFighting();
	BOOL isAutoFighting();

	// auto speed up walk
	BOOL startSpeedUpWalk();
	BOOL stopSpeedUpWalk();
	BOOL isSpeedUpWalk();

	BOOL autoTakeScreenshot;
	BOOL speedUpBySpeak;

	PGAME_LOCATION findMonster();

	BOOL checkStatus();
	int getStatus();

	unsigned int interval4screenshot;
	unsigned int interval4autoFighting;
	unsigned int interval4speedUpWalk;
	unsigned int interval4speedUpSpeak;
private:
	HWND hwnd;
	CImage* pImage;
	HDC hScreenDC;
	CImageDC* pImageDC;
	BOOL autoRefresh;
	BOOL autoFighting;
	BOOL speedUpWalk;

	int windowWidth;
	int windowHeight;

	int status;
	
	int searchRGB(const PGAME_LOCATION l, int r, int g, int b, int deviation);

	GAME_LOCATION monsterLocations[MAX_MONSTER_LOCATION];
};

