#include "StdAfx.h"
#include "HWNDScreen.h"
#include "System.h"

#define DEVIATION 8

CHWNDScreen::CHWNDScreen(HWND hwnd)
{
	HDC hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	this->hwnd = hwnd;
	CSystem::lockScreen(TRUE);
	GetWindowRect(hwnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	image.Create(width, height, 24);
	pImageDC = new CImageDC(image);
	BitBlt(*pImageDC, 0, 0, width, height, hScreenDC, rect.left, rect.top, SRCCOPY);
	CSystem::lockScreen(FALSE);
	DeleteDC(hScreenDC);
}


CHWNDScreen::~CHWNDScreen(void)
{
	delete pImageDC;
}

BOOL CHWNDScreen::screenshot( PTSTR pszFileName)
{
	return image.Save(pszFileName);
}

INT CHWNDScreen::colorDeviation( RECT* rect, COLORREF rgb)
{
	INT x = 0;
	INT y = 0;

	INT count = 0;
	INT rate = -1;
	if(!image) return rate;
	INT pixSize = (rect->right-rect->left) * (rect->bottom - rect->top);
	
	for(x = rect->left; x <= rect->right; ++x)
		for(y = rect->top; y <= rect->bottom; ++y)
		{
			COLORREF color = image.GetPixel(x, y);
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

void CHWNDScreen::flashRECT( RECT* rect)
{
	TRACE("Flash rect: %p (left: %d, right: %d, top: %d, bottom: %d)\n", rect, rect->left, rect->right, rect->top, rect->bottom);
	RECT* tmp = (RECT*)malloc(sizeof(RECT));
	memcpy(tmp, rect, sizeof(RECT));
	AfxBeginThread(flashRECTThread, (LPVOID)tmp);
}

UINT CHWNDScreen::flashRECTThread( LPVOID lPvoid )
{
	
	RECT *rect = (RECT *)lPvoid;
	TRACE("Flash2 rect: %p (left: %d, right: %d, top: %d, bottom: %d)\n", rect, rect->left, rect->right, rect->top, rect->bottom);
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
		Sleep(200);
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
		Sleep(200);
	}
	
	DeleteDC(hScreenDC);
	free(rect);
	return 0;
}


