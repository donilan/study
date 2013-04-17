#pragma once

UINT LeftClickThread(LPVOID);

class CSystem
{
public:
	CSystem(void);
	~CSystem(void);

	
	static BOOL lockScreen(BOOL);
	static BOOL leftClick(int x, int y);
	static UINT LeftClickThread(LPVOID);
	static void leftClick(RECT* rect);
	static int ansi2unicode(char* in, size_t inSize, TCHAR* out, size_t outSize);
};

