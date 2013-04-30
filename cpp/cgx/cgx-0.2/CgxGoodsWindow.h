#pragma once
#include "CgxWindow.h"

#define NUMBER_OF_GOODS 20

class CCgxGoodsWindow :
	public CCgxWindow
{
public:
	CCgxGoodsWindow(CHWNDScreen*);
	~CCgxGoodsWindow(void);

	void getLocateRECT(RECT* rectOut);
	void locateCommands(void);

	int countGoods();
	BOOL hasGoods(int index);
	int goodsType(int index);
	void dropGoods(int index);
private:
	RECT goods[NUMBER_OF_GOODS];
};

