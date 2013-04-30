#include "StdAfx.h"
#include "HWNDScreen.h"
#include "System.h"
#include "FindNum.h"

#define DEVIATION 8
#define AUTO_REFRESH_INTERNAL 1000

CHWNDScreen::CHWNDScreen(HWND hwnd)
{
	this->hwnd = hwnd;
	pImageDC = NULL;
	pImage = NULL;
	this->isAutoRefresh = FALSE;
	hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
}


CHWNDScreen::~CHWNDScreen(void)
{
	if(pImageDC) delete pImageDC;
	if(pImage) delete pImage;
	DeleteDC(hScreenDC);
}

BOOL CHWNDScreen::screenshot( PTSTR pszFileName)
{
	if(pImage)
		return pImage->Save(pszFileName);
	return FALSE;
}

INT CHWNDScreen::colorDeviation(const RECT* rect, COLORREF rgb)
{
	INT x = 0;
	INT y = 0;

	INT count = 0;
	INT rate = -1;
	
	CImage img;
	toCImage(rect, &img);
	INT pixSize = img.GetHeight()*img.GetWidth();
	
	for(x = 0; x < img.GetWidth(); ++x)
		for(y = 0; y < img.GetHeight(); ++y)
		{
			COLORREF color = img.GetPixel(x, y);
			if(abs(GetRValue(rgb) - GetRValue(color)) < DEVIATION
				&& abs(GetGValue(rgb) - GetGValue(color)) < DEVIATION
				&& abs(GetBValue(rgb) - GetBValue(color)) < DEVIATION)
			{
				++count;
			}

		}
	
	rate = (100 * count) / pixSize;
	TRACE("Size: %d found: %d RGB(%d,%d,%d) rate: %d%%\n"
		, pixSize, count, (INT)GetRValue(rgb), (INT)GetGValue(rgb)
		, (INT)GetBValue(rgb), rate);


	return rate;
}

void CHWNDScreen::flashRECT(const RECT* rect)
{
	//TRACE("Flash rect: %p (left: %d, right: %d, top: %d, bottom: %d)\n", rect, rect->left, rect->right, rect->top, rect->bottom);
	RECT* tmp = (RECT*)malloc(sizeof(RECT));
	memcpy(tmp, rect, sizeof(RECT));
	AfxBeginThread(flashRECTThread, (LPVOID)tmp);
}

UINT CHWNDScreen::flashRECTThread( LPVOID lPvoid )
{
	
	RECT *rect = (RECT *)lPvoid;
	TRACE("Flash rect: %p (left: %d, right: %d, top: %d, bottom: %d)\n",
		rect, rect->left, rect->right, rect->top, rect->bottom);
	HDC hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	LONG width = rect->right - rect->left;
	LONG height = rect->bottom - rect->top;
	INT i = 0;
	INT times = 0;
	COLORREF rgb;
	for(; times < 3; ++times)
	{
		rgb = RGB(255, 0, 0);
		for(i = 0; i <= width; ++i)
		{
			SetPixel(hScreenDC, rect->left+i, rect->top, rgb);
			SetPixel(hScreenDC, rect->left+i, rect->bottom, rgb);
		}
		for(i = 0; i <= height; ++i)
		{
			SetPixel(hScreenDC, rect->left, rect->top+i, rgb);
			SetPixel(hScreenDC, rect->right, rect->top+i, rgb);
		}
		Sleep(100);
		rgb = RGB(0, 0, 255);
		for(i = 0; i <= width; ++i)
		{
			SetPixel(hScreenDC, rect->left+i, rect->top, rgb);
			SetPixel(hScreenDC, rect->left+i, rect->bottom, rgb);
		}
		for(i = 0; i <= height; ++i)
		{
			SetPixel(hScreenDC, rect->left, rect->top+i, rgb);
			SetPixel(hScreenDC, rect->right, rect->top+i, rgb);
		}
		Sleep(100);
	}
	
	DeleteDC(hScreenDC);
	free(rect);
	return 0;
}

void CHWNDScreen::loadImage(PTSTR pszImagePath)
{
	if(pImage) delete pImage;
	pImage = new CImage();
	pImage->Load(pszImagePath);
	this->rect.left = 0;
	this->rect.right = pImage->GetWidth();
	this->rect.top = 0;
	this->rect.bottom = pImage->GetHeight();
	TRACE("Load image done.\n");
}



// Locate image
BOOL CHWNDScreen::locate(const CImage* pImageIn, RECT* rectOut, const RECT* condition)
{
	RECT rect;
	int width = pImageIn->GetWidth();
	int height = pImageIn->GetHeight();
	memset(rectOut, 0, sizeof(RECT));
	if(condition == NULL)
	{
		GetWindowRect(hwnd, &rect);
		rect.top += 28;
	}
	else
	{
		memcpy(&rect, condition, sizeof(RECT));
	}
	TRACE("locating image: left %d, right %d, top %d, bottom %d\n", rect.left, rect.right, rect.top, rect.bottom);
	for(INT y = rect.top; y < rect.bottom - height; ++y)
		for(INT x = rect.left; x < rect.right - width; ++x)
		{
			rectOut->left = x;
			rectOut->top = y;
			rectOut->right = x + width;
			rectOut->bottom = y + height;
			
			if(match(pImageIn, rectOut))
			{
				return TRUE;
			}
		}

	return FALSE;
}


// Is image match in location
BOOL CHWNDScreen::match(const CImage* pImageIn, const RECT* rectIn)
{
	if(!isFocus())
		return FALSE;
	BOOL flag = TRUE;
	COLORREF rgb1;
	COLORREF rgb2;
	CImage pImage;
	toCImage(rectIn, &pImage);
	//TRACE("Screen do matching...\n");
	for(LONG x = 0; x < pImage.GetWidth(); ++x)
	{
		for(LONG y = 0; y < pImage.GetHeight(); ++y)
		{
			
			rgb1 = pImageIn->GetPixel(x, y);
			rgb2 = pImage.GetPixel(x, y);
			if(abs(GetRValue(rgb1) - GetRValue(rgb2)) > 10
				|| abs(GetGValue(rgb1) - GetGValue(rgb2)) > 10
				|| abs(GetBValue(rgb1) - GetBValue(rgb2)) > 10)
			{
				flag = FALSE;
				break;
			}
			//TRACE("[%d,%d] (%d, %d, %d) == (%d, %d, %d)\n", x, y, GetRValue(rgb1), 
				//GetGValue(rgb1), GetBValue(rgb1), GetRValue(rgb2), GetGValue(rgb2), GetBValue(rgb2));
		}
		if(!flag)
			break;
	}
	return flag;
}

 // save rect
BOOL CHWNDScreen::saveRECT(const RECT* rect, const PTSTR pszImagePath)
{
	if(!pImage) return FALSE;
	CImage tmp;
	COLORREF rgb;
	tmp.Create(rect->right-rect->left, rect->bottom-rect->top, 24);
	for(LONG x = rect->left; x < rect->right; ++x)
		for(LONG y = rect->top; y < rect->bottom; ++y)
		{
			rgb = pImage->GetPixel(x, y);
			tmp.SetPixel(x-rect->left, y-rect->top, rgb);
		}
	tmp.Save(pszImagePath);
	return TRUE;
}


// Load image file in to CImage class
BOOL CHWNDScreen::loadInToCImage(const PTSTR pszImagePath, CImage* pImage)
{
	if(pImage)
	{
		pImage->Load(pszImagePath);
		return TRUE;
	}
	return FALSE;
}



// Print into screen
void CHWNDScreen::print(void)
{
	if(!pImage) return;
	TRACE("print rect: %p (left: %d, right: %d, top: %d, bottom: %d)\n", rect, rect.left, rect.right, rect.top, rect.bottom);
	AfxBeginThread(screenPrintThread, pImage);
}



UINT CHWNDScreen::screenPrintThread(LPVOID lpVoid)
{
	CImage* pImg = (CImage*)lpVoid;
	COLORREF rgb;
	HDC hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	for(LONG x =0; x < pImg->GetWidth(); ++x)
		for(LONG y = 0; y < pImg->GetHeight(); ++y)
		{
			rgb = pImg->GetPixel(x, y);
			SetPixel(hScreenDC, x, y, rgb);
		}

		DeleteDC(hScreenDC);
		return 0;
}


void CHWNDScreen::refresh(void)
{
	if(GetForegroundWindow() == hwnd)
	{
		mutex.Lock();
	
		CSystem::lockScreen(TRUE);
		//GetCursorPos(&oldPoint);
		//SetCursorPos(0, 0);
		GetWindowRect(hwnd, &rect);
		LONG width = rect.right - rect.left;
		LONG height = rect.bottom - rect.top;
		if(pImageDC) delete pImageDC;
		if(pImage) delete pImage;
		
		pImage = new CImage();
		pImage->Create(width, height, 24);
		pImageDC = new CImageDC(*pImage);
		BitBlt(*pImageDC, 0, 0, width, height, hScreenDC, rect.left, rect.top, SRCCOPY);
		//SetCursorPos(oldPoint.x, oldPoint.y);
		CSystem::lockScreen(FALSE);
		mutex.Unlock();
	}
}


UINT CHWNDScreen::autoRefreshThread(LPVOID lpVoid)
{
	CHWNDScreen* s = (CHWNDScreen *)lpVoid;
	while(s->isAutoRefresh)
	{
		
		s->refresh();
		Sleep(AUTO_REFRESH_INTERNAL);
		
	}
	return 0;
}


void CHWNDScreen::startAutoRefresh(void)
{
	if(isAutoRefresh) return;
	isAutoRefresh = TRUE;
	TRACE("Start auto refresh.\n");
	AfxBeginThread(autoRefreshThread, this);
}


void CHWNDScreen::stopAutoRefresh(void)
{
	isAutoRefresh = FALSE;
}


int CHWNDScreen::toNumber(const RECT* pRECT)
{
	CImage pImg;
	toCImage(pRECT, &pImg);
	//TRACE("P Image width: %d, height: %d\n", pImg.GetWidth(), pImg.GetHeight());
	pImg.Save(TEXT("img\\map_tmp.bmp"));
	CFindNum finder(&pImg);
	return finder.allNum();
}


void CHWNDScreen::toCImage(const RECT* rectIn, CImage* pImageOut)
{
	CSystem::lockScreen(TRUE);
	LONG width = rectIn->right - rectIn->left;
	LONG height = rectIn->bottom - rectIn->top;
	pImageOut->Create(width, height, 24);
	CImageDC pImgDC(*pImageOut);
	BitBlt(pImgDC, 0, 0, width, height, hScreenDC, rectIn->left, rectIn->top, SRCCOPY);
	CSystem::lockScreen(FALSE);
	
}


BOOL CHWNDScreen::isFocus(void)
{
	return GetForegroundWindow() == hwnd;
}


BOOL CHWNDScreen::locate(const int resourceId, RECT* rectOut,const RECT* condition)
{
	CImage img;
	img.LoadFromResource(GetModuleHandle(NULL), resourceId);
	TRACE("Locate for image: width: %d height: %d\n", img.GetWidth(), img.GetHeight());
	return locate(&img, rectOut, condition);
}
