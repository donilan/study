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
};

