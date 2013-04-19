#pragma once
#include "cgxwindow.h"
#include "HWNDScreen.h"
class CCgxMapWindow :
	public CCgxWindow
{
private:
	inline void go(const int flagX, const int flagY, const int step);
public:
	CCgxMapWindow(CHWNDScreen* );
	~CCgxMapWindow(void);
	void locateCommands(void);

	RECT xRECT;
	RECT yRECT;
	int getX(void);
	int getY(void);
	void goEast(int step);
	void goSouth(int step);
	void goWest(int step);
	void goNorth(int step);
	
};

