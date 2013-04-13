#include "StdAfx.h"
#include "CgxWindow.h"
#include "System.h"


CCgxWindow::CCgxWindow(UINT resourceId, CHWNDScreen* screen)
{
	this->pScreen = screen;
	pLocateImage = new CImage();
	pLocateImage->LoadFromResource(GetModuleHandle(NULL), resourceId);
	memset(&rect, 0, sizeof(RECT));
	memset(commandRECTs, 0, sizeof(RECT)*MAX_COMMAND);
}


CCgxWindow::~CCgxWindow(void)
{
	if(pLocateImage) delete pLocateImage;
}


BOOL CCgxWindow::locate(void)
{
	TRACE("Locating window...\n");
	RECT newRECT;
	BOOL found;
	if(!isPositionChanged())
	{
		TRACE("Window position not changed.\n");
		return TRUE;
	}
	if(pScreen)
	{
		 found = pScreen->locate(pLocateImage, &newRECT);
		 if(found)
		 {
			 TRACE("Found window position and found a new RECT.\n");
			 memcpy(&rect, &newRECT, sizeof(RECT));
			 locateCommands();
			 return TRUE;
		 }
		 return FALSE;
	}
	return FALSE;
}


BOOL CCgxWindow::isPositionChanged(void)
{
	BOOL result = TRUE;
	if(pScreen && rect.right != 0 && rect.bottom != 0)
	{
		result = !(pScreen->match(pLocateImage, &rect));
		TRACE("Check is window (%d, %d) changed: %d\n", rect.left, rect.top, result);
	}
	return result;
}


BOOL CCgxWindow::getCommand(int index, RECT* rectOut)
{
	int size = getCommandSize();
	if(index >= size)
		return FALSE;
	memcpy(rectOut, &commandRECTs[index], sizeof(RECT));
	return TRUE;
}



void CCgxWindow::leftClick(int index)
{
	RECT rect;
	int x = 0;
	int y = 0;
	getCommand(index, &rect);
	if(rect.right> 0 && rect.bottom>0)
	{
		x = (rect.right - rect.left) / 2 + rect.left;
		y = (rect.bottom - rect.top) / 2 + rect.top;
		if(x > 0 && y > 0) 
		{
			CSystem::leftClick(x, y);
			return;
		}
	}
	TRACE("Error: Command rect not right, (%d, %d)", rect.left, rect.right);
	
}



int CCgxWindow::getCommandSize(void)
{
	int count = 0;
	for(int i = 0; i < MAX_COMMAND; ++i)
	{
		if( commandRECTs[i].left < pScreen->rect.left 
			|| commandRECTs[i].right > pScreen->rect.right 
			|| commandRECTs[i].top < pScreen->rect.top
			|| commandRECTs[i].bottom > pScreen->rect.bottom
			|| commandRECTs[i].left == commandRECTs[i].right
			|| commandRECTs[i].top == commandRECTs[i].bottom
			)
			continue;
		++count;
	}
	return count;
}
