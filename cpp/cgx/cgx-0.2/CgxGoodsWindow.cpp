#include "StdAfx.h"
#include "CgxGoodsWindow.h"
#include "resource.h"
#include "System.h"

CCgxGoodsWindow::CCgxGoodsWindow(CHWNDScreen* screen) 
	: CCgxWindow(IDB_GOODS, screen)
{
}


CCgxGoodsWindow::~CCgxGoodsWindow(void)
{
}

void CCgxGoodsWindow::getLocateRECT(RECT* rectOut)
{
	rectOut->left = 420;
	rectOut->top = 180;
	rectOut->right = 580;
	rectOut->bottom = 260;
}

void CCgxGoodsWindow::locateCommands( void )
{
	TRACE("goods window: left %d, top %d\n", rect.left, rect.top);

	for(int y = 0, idx = 0; y < 4; ++y)
	{
		for(int x = 0; x < 5; ++x)
		{
			goods[idx].left = rect.left - 107 + 50 * x;
			goods[idx].right = goods[idx].left + 40;
			goods[idx].top = rect.top + 40 + 50 * y;
			goods[idx].bottom = goods[idx].top + 40;
			++idx;
		}
	}
}

int CCgxGoodsWindow::countGoods()
{
	int counter = 0;
	for(int i = 0; i < NUMBER_OF_GOODS; ++i)
	{
		if(hasGoods(i))
			++counter;
	}
	return counter;
}

BOOL CCgxGoodsWindow::hasGoods( int index )
{
	RECT tmp = {0};
	if(index < 0 || index >= NUMBER_OF_GOODS)
		return FALSE;
	return !pScreen->locate(IDB_GOODS_NO_GOODS, &tmp, &goods[index]);
}

int CCgxGoodsWindow::goodsType( int index )
{
	RECT tmp;
	if(pScreen->locate(IDB_GOODS_NO_GOODS, &tmp, &goods[index]))
		return IDB_GOODS_NO_GOODS;
	else if(pScreen->locate(IDB_GOODS_CARD, &tmp, &goods[index]))
		return IDB_GOODS_CARD;
	else if(pScreen->locate(IDB_GOODS_EARTH_PROP, &tmp, &goods[index]))
		return IDB_GOODS_EARTH_PROP;
	else if(pScreen->locate(IDB_GOODS_WIND_PROP, &tmp, &goods[index]))
		return IDB_GOODS_WIND_PROP;
	return 0;
}

void CCgxGoodsWindow::dropGoods( int index )
{
	if(index < 0 || index >= NUMBER_OF_GOODS)
		return;
	CSystem::leftClick(&goods[index]);
	Sleep(500);
	CSystem::leftClick(rect.left-200, rect.top -100);
	Sleep(500);
}
