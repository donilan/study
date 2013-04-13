#include "StdAfx.h"
#include "CgxWindow.h"


CCgxWindow::CCgxWindow(UINT resourceId, CHWNDScreen* screen)
{
	this->screen = screen;
	pLocateImage = new CImage();
	pLocateImage->LoadFromResource(GetModuleHandle(NULL), resourceId);
	memset(&rect, 0, sizeof(RECT));
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
	if(screen)
	{
		 found = screen->locate(pLocateImage, &newRECT);
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
	if(screen && rect.right != 0 && rect.bottom != 0)
	{
		result = !(screen->match(pLocateImage, &rect));
		TRACE("Check is window (%d, %d) changed: %d\n", rect.left, rect.top, result);
	}
	return result;
}


BOOL CCgxWindow::getCommand(int index, RECT* rectOut)
{
	if(commandRECTs[index].right == 0 || commandRECTs[index].bottom == 0)
		return FALSE;

	memcpy(rectOut, &commandRECTs[index], sizeof(RECT));
	return TRUE;
}
