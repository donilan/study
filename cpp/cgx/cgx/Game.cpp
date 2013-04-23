#include "StdAfx.h"
#include "Game.h"
#include <ctime>
#include "resource.h"

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
	this->autoFighting = FALSE;
	this->autoTakeScreenshot = FALSE;
	this->status = GAME_STATUS_UNKNOW;
	interval4screenshot = 3000;
	interval4autoFighting = 1000;
	interval4speedUpWalk = 500;
	interval4speedUpSpeak = 200;
	srand((unsigned) time(NULL));
	this->hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	
	memset(this->locations, 0, sizeof(GAME_LOCATION)*MAX_LOCATION);
	memcpy(this->locations, ALL_LOCATIONS, sizeof(GAME_LOCATION)*MAX_LOCATION);
	memset(this->skillLocations, 0 , sizeof(GAME_LOCATION)*SKILL_LENGTH);

	memset(this->skills, 0, sizeof(CSkill)* SKILL_LENGTH);

	this->skillChoices.allTargetSkillIdx = -1;
	this->skillChoices.fourTargetSkillIdx = -1;
	this->skillChoices.signleSkillIdx = -1;
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

BOOL CGame::leftClick(GAME_LOCATION* l)
{
	int x = l->x+15;
	int y = l->y+10;
	LOG_DEBUG << "mouse move to " << x << "," << y;
	SetCursorPos(x, y);
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTDOWN, x, y, 0, 0);
	Sleep(100);
	mouse_event(MOUSEEVENTF_LEFTUP, x, y, 0, 0);

	return TRUE;
}

BOOL CGame::rightClick(GAME_LOCATION* l)
{
	int x = l->x+15;
	int y = l->y+5;
	LOG_DEBUG << "mouse move to " << x << "," << y;
	SetCursorPos(x, y);
	Sleep(100);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, x, y, 0, 0);
	Sleep(100);
	mouse_event(MOUSEEVENTF_RIGHTUP, x, y, 0, 0);

	return TRUE;
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
	checkStatus();
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

BOOL CGame::findMonster()
{
	monsterHolder.resetAlive();
	for(int i = 0; i < MAX_MONSTER_LOCATION; ++i)
	{
		if(_colorDeviation(monsterHolder.get(i), COLOR_MONSTER_FONT) > DEVIATION_MONSTER_FONT)
		{
			monsterHolder.setAlive(i);
		}
	}
	return TRUE;
}

int CGame::_searchRGB( PGAME_LOCATION l, COLORREF rgb, int deviation )
{
	LOG_DEBUG << "Searching RGB(" << (int)GetRValue(rgb) << "," << (int)GetGValue(rgb) << "," << (int)GetBValue(rgb) <<") x: " << l->x << " y: " << l->y;
	if(!pImage)
		refresh();

	int result = 0;

	for(int x = l->x; x <= (l->x + l->cx); ++x) 
	{
		for(int y = l->y; y <= (l->y + l->cy); ++y)
		{
			COLORREF color = pImage->GetPixel(x, y);
			//LOG_DEBUG << "RGB(" << (int)GetRValue(color) << "," << (int)GetGValue(color) << "," << (int)GetBValue(color) <<")";
			if(abs(GetRValue(rgb) - GetRValue(color)) < deviation
				&& abs(GetGValue(rgb) - GetGValue(color)) < deviation
				&& abs(GetBValue(rgb) - GetBValue(color)) < deviation)
			{
				result++;
			}
#ifdef DEBUG
			if(x == l->x || x == (l->cx+l->x)
				|| y == l->y || y == (l->cy + l->y))
				pImage->SetPixel(x, y, RGB(255,0,0));
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
	if(_colorDeviation(getLocation(BTN_DUMP), COLOR_BTN_ENABLE) > DEVIATION_BTN)
		status = GAME_STATUS_TRAVEL;
	else if(_colorDeviation(getLocation(FIT_BTN_LOCATION), COLOR_FIT_BTN_DISABLE) > DEVIATION_FIT_BTN) 
	{
		findMonster();
		if(this->monsterHolder.getAliveNumber() > 0)
			status = GAME_STATUS_PLAYER_FIGHT;
	}
	else if(_colorDeviation(getLocation(FIT_BTN_MONSTER_SKILL), COLOR_FIT_BTN_DISABLE) > DEVIATION_FIT_BTN)
	{
		findMonster();
		if(this->monsterHolder.getAliveNumber() > 0)
			status = GAME_STATUS_MONSTER_FIGHT;
	}
	else
		status = GAME_STATUS_UNKNOW;
	return TRUE;
}

BOOL CGame::hitMonster( int witchOne )
{
	PGAME_LOCATION l = this->monsterHolder.get(witchOne);
	if(l)
		leftClick(l);
	return TRUE;
}



inline int CGame::_colorDeviation( PGAME_LOCATION l, COLORREF rgb )
{
	int count = _searchRGB(l, rgb, 8);
	int pixSize = l->cx * l->cy;
	LOG_DEBUG << "size: " << pixSize << " color (" 
		<< (int)GetRValue(rgb) << "," << (int)GetGValue(rgb) << "," << (int)GetBValue(rgb) 
		<<  ") count: " << count << " deviation: " << (100 *count) / pixSize;
	return (100 *count) / pixSize;
}

BOOL CGame::hitFitBtn(int idx)
{
	if(_colorDeviation(getLocation(idx), COLOR_FIT_BTN_DISABLE) > DEVIATION_FIT_BTN) {
		return leftClick(getLocation(idx));
	}
	return FALSE;
}

PGAME_LOCATION CGame::getLocation( int idx )
{
	return &locations[idx];
}

BOOL CGame::_locateBMP(CImage *finder, PGAME_LOCATION l)
{
	for(int x = 0; x < pImage->GetWidth(); ++x)
	{
		for(int y = 0; y < pImage->GetHeight(); ++y) {
			if(pImage->GetPixel(x, y) == finder->GetPixel(0, 0))
			{
				BOOL match = TRUE;
				for(int x2 = 0; x2 < finder->GetWidth(); ++x2) {
					for(int y2 = 0; y2 < finder->GetHeight(); ++y2) {
						if(pImage->GetPixel(x+x2, y+y2) != finder->GetPixel(x2, y2)) {
							match = FALSE;
							break;
						}
					}
					if(!match) break;
				}
				if(match) {
					l->y = y;
					l->x = x;
					l->cx = finder->GetWidth();
					l->cy = finder->GetHeight();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
BOOL CGame::locateBMP( PTSTR pszFilename, PGAME_LOCATION l)
{
	CImage finder;
	finder.Load(pszFilename);
	return _locateBMP(&finder, l);
}

PGAME_LOCATION CGame::findSkillWindow()
{
	memset(&skillWindow, 0, sizeof(GAME_LOCATION));
	if(locateBMPFromResource(IDB_JOB, &skillWindow))
	{
		skillWindow.x += 5;
		skillWindow.y += 36;
		skillWindow.cx = 46;
		skillWindow.cy = 16*SKILL_LENGTH;

		for(int i = 0; i < SKILL_LENGTH; ++i)
		{
			skillLocations[i].x = skillWindow.x;
			skillLocations[i].y = skillWindow.y + (skillWindow.cy/SKILL_LENGTH * i);
			skillLocations[i].cx = skillWindow.cx;
			skillLocations[i].cy = skillWindow.cy/SKILL_LENGTH;
			_initSkill(i, &skillLocations[i]);
		}
		
		//searchRGB(&skillWindow, RGB(0,0,0), 5);
	}
	
	return &skillWindow;
}

BOOL CGame::locateBMPFromResource( UINT id, PGAME_LOCATION l)
{
	CImage finder;
	finder.LoadFromResource(GetModuleHandle(NULL), id);
	return _locateBMP(&finder, l);
}

BOOL CGame::load4refresh( CString pszFilename)
{
	if(pImage)
		delete pImage;

	pImage = new CImage();
	pImage->Load(pszFilename);
	
	return TRUE;
}

BOOL CGame::saveSkillPhotos(PTSTR dir)
{
	TCHAR buff[MAX_PATH] = {0};
	for(int i = 0; i < SKILL_LENGTH; ++i)
	{
		swprintf(buff, TEXT("%s\\skill_%d.bmp"), dir, i);
		saveGameLocation(&skillLocations[i], buff);
	}
	return TRUE;
}

BOOL CGame::saveGameLocation( GAME_LOCATION* l, PTSTR pszFilename)
{
	CImage image;
	image.Create(l->cx, l->cy, pImage->GetBPP());
	LOG_DEBUG << "save image: x: " << l->x << " y: " << l->y << " width: " << l->cx << " height: " << l->cy <<std::endl;
	for(int x= 0; x < l->cx; ++x)
	{
		for(int y = 0; y < l->cy; ++y)
		{
			//LOG_DEBUG << GetRValue(pImage->GetPixel(x, y)) << "," << GetGValue(pImage->GetPixel(x, y)) << "," << GetBValue(pImage->GetPixel(x, y));
			image.SetPixel(x, y, pImage->GetPixel(x+l->x, y+l->y));
			
		}
	}
	image.Save(pszFilename);
	return TRUE;
}

BOOL CGame::_matchImage( CImage* image, PGAME_LOCATION l)
{
	for(int x = 0; x < image->GetWidth(); ++x)
	{
		for(int y = 0; y < image->GetHeight(); ++y) 
		{
			if(pImage->GetPixel(l->x+x, l->y+y) != image->GetPixel(x, y)) 
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

#define INIT_SKILL(id, NAME, attNum) \
	if(pImage) delete pImage; \
	pImage = new CImage(); \
	pImage->LoadFromResource(hInstance, id); \
	if(_matchImage(pImage, l )){ \
	swprintf( skills[idx].name, TEXT(#NAME)); \
	skills[idx].attackNumber = attNum ;\
	return TRUE; \
	}

BOOL CGame::_initSkill( int idx, PGAME_LOCATION l)
{
	CImage* image = NULL;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	skills[idx].index = idx;
	memcpy(&(skills[idx].location), l, sizeof(GAME_LOCATION));
	swprintf(skills[idx].name, TEXT("无技能"));
	skills[idx].attackNumber = 0;

	INIT_SKILL(IDB_SKILL_EN_YANG_YAN, 阳炎, 1)
		INIT_SKILL(IDB_SKILL_EN_LUAN_SHE, 乱射, 10)
		INIT_SKILL(IDB_SKILL_EN_MING_JING_ZHI_SHUI, 明镜止水, 0)
		INIT_SKILL(IDB_SKILL_EN_QIAN_KUN, 乾坤, 1)
		INIT_SKILL(IDB_SKILL_EN_GONG_JI_FAN_TAN, 攻击反弹, 1)
		INIT_SKILL(IDB_SKILL_EN_GONG_JI_XI_SHOU, 攻击吸收, 1)
		INIT_SKILL(IDB_SKILL_DIS_CHONG_WU_QIANG_HUA, 宠物强化, 0)
		INIT_SKILL(IDB_SKILL_DIS_TIAO_JIAO, 调教, 0)

		if(image) delete image;
	return FALSE;
}

BOOL CGame::choiceSkill( int idx)
{
	return leftClick(&skills[idx].location);
}

int CGame::getCurrentSkillMaxLevel()
{
	for(int i = SKILL_LENGTH; i > 0; --i)
	{
		if(_colorDeviation(&(skills[i-1].location), RGB(255,255,255)) > 10)
			return i;
	}
	return -1;
}

BOOL CGame::choiceSkillLevel( int level)
{
	--level;
	if(level < 0) level = 0;
	if(level > 9) level = 9; 
	return leftClick(&skills[level].location);
}

BOOL CGame::isSkillInit()
{
	return skills[0].name[0] != 0;
}

void CGame::changeStatus( int s)
{
	this->status = s;
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
	int skillIdx = -1;
	CGame* game = (CGame *)pParam;
	while(game->isAutoFighting())
	{
		if(game->isFocus())
		{
			game->refresh();
			int status = game->getStatus();
			LOG_DEBUG << "Auto fighting with status: " << status;
			if (status == CGame::GAME_STATUS_MONSTER_FIGHT
				|| status == CGame::GAME_STATUS_PLAYER_FIGHT)
			{
				
				switch (status)
				{
				case CGame::GAME_STATUS_PLAYER_FIGHT:
					LOG_DEBUG << "Player fight status. monster number: " << game->monsterHolder.getAliveNumber();
					game->hitFitBtn(FIT_BTN_SKILL);
					Sleep(100);
					
					game->findSkillWindow();
					
					if(game->monsterHolder.getAliveNumber() < 2) 
					{
						skillIdx = game->skillChoices.signleSkillIdx;
					}
					else if(game->monsterHolder.getAliveNumber() < 4)
					{
						if(game->skillChoices.fourTargetSkillIdx < 0)
						{
							for(int i = 0; i < SKILL_LENGTH; ++i)
							{
								if(game->skills[i].attackNumber == SKILL_FOUR_TARGET)
								{
									game->skillChoices.fourTargetSkillIdx = i;
									
									break;
								} else if(game->skills[i].attackNumber == SKILL_ALL_TARGET)
									game->skillChoices.fourTargetSkillIdx = i;
							}
						}
						LOG_DEBUG << "Choice four target skill number: " << game->skillChoices.fourTargetSkillIdx;
						skillIdx = game->skillChoices.fourTargetSkillIdx;
					} 
					else 
					{
						if(game->skillChoices.allTargetSkillIdx < 0)
						{
							for(int i = 0; i < SKILL_LENGTH; ++i)
							{
								if(game->skills[i].attackNumber == SKILL_FOUR_TARGET)
								{
									game->skillChoices.allTargetSkillIdx = i;
								} 
								else if(game->skills[i].attackNumber == SKILL_ALL_TARGET)
								{
										game->skillChoices.allTargetSkillIdx = i;
										break;
								}
							}
						}
						LOG_DEBUG << "Choice all target skill number: " << game->skillChoices.allTargetSkillIdx;
						skillIdx = game->skillChoices.allTargetSkillIdx;
					}
					// att
					if(skillIdx < 0)
					{
						game->hitFitBtn(FIT_BTN_HIT);
					}
					else 
					{
						
						game->choiceSkill(skillIdx);
						Sleep(200);
						int skillLv = game->monsterHolder.getAliveNumber() - 2;
						int skillMaxLv = game->getCurrentSkillMaxLevel();
						if(skillLv < 1) skillLv = 1;
						if(skillLv > skillMaxLv) skillLv = skillMaxLv;
						game->choiceSkillLevel(skillLv);

					}
					Sleep(100);
					game->hitMonster(game->monsterHolder.getOneAlive());
					game->changeStatus(CGame::GAME_STATUS_MONSTER_FIGHT);
					break;

				case CGame::GAME_STATUS_MONSTER_FIGHT:
					game->hitMonster(game->monsterHolder.getOneAlive());
					break;
				}
			}
			
			
		}
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