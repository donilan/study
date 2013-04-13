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
	
	if( commandRECTs[index].left < pScreen->rect.left 
		|| commandRECTs[index].right > pScreen->rect.right 
		|| commandRECTs[index].top < pScreen->rect.top
		|| commandRECTs[index].bottom > pScreen->rect.bottom
		|| commandRECTs[index].left == commandRECTs[index].right
		|| commandRECTs[index].top == commandRECTs[index].bottom
		)
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
