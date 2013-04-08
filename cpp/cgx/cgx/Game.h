#pragma once

#include <windows.h>
#include <iostream>
#include "cgxData.h"

#define KV_1 49
#define SKILL_LENGTH 10

UINT AutoRefreshThread( LPVOID pParam );
UINT AutoFightingThread( LPVOID pParam );
UINT AutoSpeedUpWalkThread( LPVOID pParam );

class CGame
{
public:
	enum STATUS {GAME_STATUS_UNKNOW, 
		GAME_STATUS_TRAVEL, 
		GAME_STATUS_PLAYER_FIGHT, 
		GAME_STATUS_MONSTER_FIGHT};
	enum {FACE_SIT, FACE_HELLO};

	CGame(HWND);
	~CGame(void);

	BOOL doFace(int);
	BOOL doRandomFace();
	BOOL sendKey(int);
	BOOL sendKeyWithCtl(int);
	BOOL leftClick(GAME_LOCATION*);
	BOOL rightClick(GAME_LOCATION*);
	BOOL say(const char*);
	BOOL reSay();
	BOOL focus();
	BOOL isFocus();
	BOOL getRect(RECT*);
	BOOL refresh();
	BOOL saveScreenshot2file(LPCTSTR);
	BOOL hitMonster(int);
	BOOL hitFitBtn(int);
	
	BOOL load4refresh(CString);
	BOOL locateBMP(PTSTR, PGAME_LOCATION);
	BOOL locateBMPFromResource(UINT, PGAME_LOCATION);
	

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

	BOOL saveGameLocation(GAME_LOCATION*, PTSTR);

	PGAME_LOCATION findMonster();
	PGAME_LOCATION findSkillWindow();
	BOOL saveSkillPhotos();
	PGAME_LOCATION getLocation(int idx);
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
	
	GAME_LOCATION skillWindow;
	GAME_LOCATION skillLocations[SKILL_LENGTH];

	int searchRGB(PGAME_LOCATION l, COLORREF rgb, int deviation);
	inline int colorDeviation(PGAME_LOCATION l, COLORREF rgb);
	BOOL _locateBMP(CImage*, PGAME_LOCATION);

	GAME_LOCATION monsterLocations[MAX_MONSTER_LOCATION];
	GAME_LOCATION locations[MAX_LOCATION];

};

