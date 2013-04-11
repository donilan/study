#pragma once

class CHWNDScreen
{
public:
	CHWNDScreen(HWND);
	~CHWNDScreen(void);

	BOOL screenshot(PTSTR);
	INT colorDeviation(RECT*, COLORREF);
	static void flashRECT(RECT*);
	static UINT flashRECTThread(LPVOID);
private:
	HWND hwnd;
	CImageDC* pImageDC;
	CImage image;
	RECT rect;
	LONG height;
	LONG width;
};

