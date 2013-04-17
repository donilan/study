#include "StdAfx.h"
#include "FindNum.h"

#define DEVIATION 10

#define EQ_COLOR(rgb1, rgb2) \
	(abs(GetRValue(rgb1) - GetRValue(rgb2)) < 10 \
	&& abs(GetGValue(rgb1) - GetGValue(rgb2)) < DEVIATION \
	&& abs(GetBValue(rgb1) - GetBValue(rgb2)) < DEVIATION )

#define NEQ_COLOR(rgb1, rgb2) \
	!(abs(GetRValue(rgb1) - GetRValue(rgb2)) < DEVIATION \
	&& abs(GetGValue(rgb1) - GetGValue(rgb2)) < DEVIATION \
	&& abs(GetBValue(rgb1) - GetBValue(rgb2)) < DEVIATION )


CFindNum::CFindNum(CImage* pImage)
{
	this->pImg = pImage;
	height = pImage->GetHeight();
	width = pImage->GetWidth();
	posX = 0;
	posY = 0;
	if(height > 0 && width > 0)
		fstColor = pImg->GetPixel(0, 0);
}



CFindNum::~CFindNum(void)
{
}


BOOL CFindNum::findNext(RECT* rectOut)
{
	COLORREF rgb;
	BOOL fstFound = TRUE;
	BOOL found = FALSE;
	memset(rectOut, 0, sizeof(RECT));
	for(; posX < width; ++posX)
	{
		for(posY = 0; posY < height; ++posY)
		{
			rgb = pImg->GetPixel(posX, posY);
			//TRACE("[%d,%d] width: %d, height: %d (%d,%d,%d) == (%d,%d,%d)\n"
			//	, posX, posY , width, height, GetRValue(fstColor), GetGValue(fstColor)
			//	,GetBValue(fstColor), GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
			if(NEQ_COLOR(fstColor, rgb))
			{
				found = TRUE;
				if(fstFound)
				{
					fstFound = FALSE;
					rectOut->left = posX;
					rectOut->top = posY;
				}
				else
				{
					if(rectOut->left > posX) rectOut->left = posX;
					if(rectOut->right < posX) rectOut->right = posX;
					if(rectOut->top > posY) rectOut->top = posY;
					if(rectOut->bottom < posY) rectOut->bottom = posY;
					//TRACE("%d, %d, %d, %d\n", rectOut->left, rectOut->right, rectOut->top
					//	, rectOut->bottom);
				}
			}
			else
			{
				if(posY == height-1 && found == FALSE
					&& posX != rectOut->right)
				{
					if(rectOut->right-rectOut->left < 1)
						continue;
					return TRUE;
				}
				found = FALSE;
			}// if
		}// for y
	}// for x
	/*
	TRACE("Found failed!(%d,%d,%d)[%d, %d] last result is: left: %d, right: %d, top: %d, bottom: %d\n"
		,GetRValue(fstColor), GetGValue(fstColor), GetBValue(fstColor)
		,width, height, rectOut->left, rectOut->right, rectOut->top, rectOut->bottom);
		*/
	return FALSE;
}



int CFindNum::toNum(const RECT* rectIn)
{
	int w = rectIn->right - rectIn->left;
	int h = rectIn->bottom - rectIn->top;
	COLORREF rgb;
	bool foundLeftBreak = FALSE;
	bool foundRightBreak = FALSE;
	int leftBreak = 0;
	int leftLength = 0;
	int rightBreak = 0;
	int rightLength = 0;
	int topLength = 0;
	int bottomLength = 0;
	int middleBreak = 0;
	// not a number
	if(w < 1 || h < 1)
		return -1;
	
	// 1
	if(w*100 / h < 40)
		return 1;

	for(int i = rectIn->left; i < rectIn->right; ++i)
	{
		rgb = pImg->GetPixel(i, rectIn->top);
		if(NEQ_COLOR(fstColor, rgb))
		{
			++topLength;
		}
		rgb = pImg->GetPixel(i, rectIn->bottom);
		if(NEQ_COLOR(fstColor, rgb))
		{
			++bottomLength;
		}
	}

	for(int i = rectIn->top; i < rectIn->bottom; ++i)
	{
		rgb = pImg->GetPixel(rectIn->left, i);
		if(NEQ_COLOR(fstColor, rgb))
		{
			++leftLength;
		}else
		{
			if(leftBreak == 0 && leftLength > 0)
				leftBreak = i - rectIn->top;
		}
		rgb = pImg->GetPixel(rectIn->right, i);
		if(NEQ_COLOR(fstColor, rgb))
		{
			++rightLength;
		}else {
			if(rightBreak == 0 && rightLength > 0)
				rightBreak = i - rectIn->top;
		}
		rgb = pImg->GetPixel(rectIn->left+ w/2, i);
		if(NEQ_COLOR(fstColor, rgb))
		{
			if(abs(i - rectIn->left - h /2) < 3)
				middleBreak = i;
		}
		
	}
	//TRACE("%d, %d, %d, %d\n", leftLength, rightLength, topLength, bottomLength);
	// 4
	if(topLength < w / 2)
	{
		return 4;
	}

	if(topLength == w)
	{
		//7 / 5
		if(leftLength < h / 2)
			return 7;
		else
			return 5;

	}
	if(bottomLength == w)
	{
		return 2;
	}
	if(leftBreak > 0 && leftBreak < h /2)
		return 3;
	if(leftBreak >= h/2 && rightBreak == 0)
		return 9;
	if(leftBreak == 0 && ((rightBreak < h/2 && rightBreak > 0) || rightLength <= h/2))
		return 6;
	if(leftBreak == 0 && rightBreak == 0 )
		return 0;
	if(leftBreak > 0 && rightBreak > 0)
		return 8;
	return -1;
}



int CFindNum::allNum(void)
{
	int result = 0;
	RECT rect;
	while(findNext(&rect))
	{
		result = toNum(&rect) + result * 10;
	}
	return result;
}
