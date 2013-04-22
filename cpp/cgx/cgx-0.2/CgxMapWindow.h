#pragma once
#include "cgxwindow.h"
#include "HWNDScreen.h"
class CCgxMapWindow :
	public CCgxWindow
{
private:
	inline int _calcNextPoint(const int nextX, const int nextY, PPOINT pPoint);
public:
	CCgxMapWindow(CHWNDScreen* );
	~CCgxMapWindow(void);
	void locateCommands(void);

	RECT xRECT;
	RECT yRECT;
	int getX(void);
	int getY(void);
	
	int goNext(int nextX, int nextY);
	void leftClickCenter(void);
	int moveMouse(int nextX, int nextY);
};

