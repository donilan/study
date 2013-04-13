#include "StdAfx.h"
#include "HWNDScreen.h"
#include "System.h"

#define DEVIATION 8

CHWNDScreen::CHWNDScreen(HWND hwnd)
{
	if(hwnd)
	{
		HDC hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
		this->hwnd = hwnd;
		CSystem::lockScreen(TRUE);
		GetWindowRect(hwnd, &rect);
		LONG width = rect.right - rect.left;
		LONG height = rect.bottom - rect.top;
		pImage = new CImage();
		pImage->Create(width, height, 24);
		pImageDC = new CImageDC(*pImage);
		BitBlt(*pImageDC, 0, 0, width, height, hScreenDC, rect.left, rect.top, SRCCOPY);
		CSystem::lockScreen(FALSE);
		DeleteDC(hScreenDC);

	} 
	else 
	{
		pImageDC = NULL;
		pImage = NULL;
	}

}


CHWNDScreen::~CHWNDScreen(void)
{
	if(pImageDC) delete pImageDC;
	if(pImage) delete pImage;
}

BOOL CHWNDScreen::screenshot( PTSTR pszFileName)
{
	if(pImage)
		return pImage->Save(pszFileName);
	return FALSE;
}

INT CHWNDScreen::colorDeviation( RECT* rect, COLORREF rgb)
{
	INT x = 0;
	INT y = 0;

	INT count = 0;
	INT rate = -1;
	if(!pImage) return rate;
	INT pixSize = (rect->right-rect->left) * (rect->bottom - rect->top);
	
	for(x = rect->left; x < rect->right; ++x)
		for(y = rect->top; y < rect->bottom; ++y)
		{
			COLORREF color = pImage->GetPixel(x, y);
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
BOOL CHWNDScreen::locate(const CImage* pImageIn, RECT* rectOut)
{
	if(!pImage || !pImageIn || !rectOut) return FALSE;
	int width = pImageIn->GetWidth();
	int height = pImageIn->GetHeight();
	memset(rectOut, 0, sizeof(RECT));
	TRACE("Begin locate...");
	for(INT x = rect.left; x < rect.right - width; ++x)
		for(INT y = rect.top; y < rect.bottom - height; ++y)
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
	COLORREF rgb1;
	COLORREF rgb2;
	if(!pImage || !pImageIn) return FALSE;
	for(LONG x = rectIn->left; x < rectIn->right; ++x)
		for(LONG y = rectIn->top; y < rectIn->bottom; ++y)
		{
			if(x > pImage->GetWidth() || y > pImage->GetHeight())
				return FALSE;
			rgb1 = pImageIn->GetPixel(x-rectIn->left, y-rectIn->top);
			rgb2 = pImage->GetPixel(x, y);
			if(abs(GetRValue(rgb1) - GetRValue(rgb2)) > 10
				|| abs(GetGValue(rgb1) - GetGValue(rgb2)) > 10
				|| abs(GetBValue(rgb1) - GetBValue(rgb2)) > 10)
				return FALSE;
			//TRACE("(%d, %d, %d) == (%d, %d, %d)\n", GetRValue(rgb1), 
			//	GetGValue(rgb1), GetBValue(rgb1), GetRValue(rgb2), GetGValue(rgb2), GetBValue(rgb2));
		}
	TRACE("MATCHED\n");
	return TRUE;
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
	AfxBeginThread(screenPrintThread, this);
}


UINT CHWNDScreen::screenPrintThread(LPVOID lpVoid)
{
	CHWNDScreen* pScreen = (CHWNDScreen*)lpVoid;
	COLORREF rgb;
	HDC hScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	for(LONG x = pScreen->rect.left; x < pScreen->rect.right; ++x)
		for(LONG y = pScreen->rect.top; y < pScreen->rect.bottom; ++y)
		{
			rgb = pScreen->pImage->GetPixel(x-pScreen->rect.left, y-pScreen->rect.top);
			SetPixel(hScreenDC, x, y, rgb);
		}

		DeleteDC(hScreenDC);
		return 0;
}
