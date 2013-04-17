#pragma once
class CFindNum
{
public:
	CFindNum(CImage*);
	~CFindNum(void);

	BOOL findNext(RECT*);

	CImage* pImg;
	int width;
	int height;
	int posX;
	int posY;
	COLORREF fstColor;
	int toNum(const RECT* rectIn);
	int allNum(void);
};

