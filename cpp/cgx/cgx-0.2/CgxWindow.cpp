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
	//TRACE("Locating window...\n");
	RECT newRECT;
	BOOL found;
	RECT condition;

	if(pScreen)
	{
		getLocateRECT(&condition);
		found = pScreen->locate(pLocateImage, &newRECT, &condition);
		if(found)
		{
			//TRACE("Found window position and found a new RECT.\n");
			memcpy(&rect, &newRECT, sizeof(RECT));
			locateCommands();
#ifdef DEBUG
			for(int i = 0; i < MAX_COMMAND; ++i)
			{
				if(commandRECTs[i].left > 0)
					TRACE("[%d] left: %d, right: %d, top: %d, height: %d\n", i,
						commandRECTs[i].left, commandRECTs[i].right, commandRECTs[i].top, commandRECTs[i].bottom);
			}
#endif
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


BOOL CCgxWindow::isExists(void)
{
	BOOL result = FALSE;
	if(pScreen && rect.right != 0 && rect.bottom != 0)
	{
		result = pScreen->match(pLocateImage, &rect);
		//TRACE("Check is window (%d, %d) exist: %d\n", rect.left, rect.top, result);
	} else {
		//pScreen->refresh();
		result = locate();
	}
	return result;
}



BOOL CCgxWindow::getCommand(int index, RECT* rectOut)
{
	int size = getCommandSize();
	if(index < 0 || index >= size)
		return FALSE;
	memcpy(rectOut, &commandRECTs[index], sizeof(RECT));
	
	return TRUE;
}



void CCgxWindow::leftClick(int index)
{
	static int randomNum = -10;
	randomNum += 3;
	if(randomNum > 10)
		randomNum = -10;
	RECT rect;
	int x = 0;
	int y = 0;
	//BOOL isCommandEabled = isCommandEnable(index);
	//TRACE("Is this command enabled: %d\n", isCommandEabled);
	//if(isCommandEnable(index)) return;
	getCommand(index, &rect);
	
	if(rect.right > 0 && rect.bottom > 0)
	{
		x = (rect.right - rect.left) / 2 + rect.left + randomNum;
		y = (rect.bottom - rect.top) / 2 + rect.top;
		TRACE("Doing left click index: %d (%d, %d)\n", index, x, y);
		if(x > 0 && y > 0) 
		{
			CSystem::leftClick(x, y);
			return;
		}
	}
	TRACE("Error: Command [%d] rect not right, (%d, %d)\n", index, rect.left, rect.right);
	
}



int CCgxWindow::getCommandSize(void)
{
	int count = 0;
	for(int i = 0; i < MAX_COMMAND; ++i)
	{
		if(	commandRECTs[i].right < 1
			|| commandRECTs[i].bottom < 1
			|| commandRECTs[i].left == commandRECTs[i].right
			|| commandRECTs[i].top == commandRECTs[i].bottom
			)
			continue;
		++count;
	}
	return count;
}


void CCgxWindow::centerXY(int* centerX, int* centerY)
{
	RECT rect;
	GetWindowRect(pScreen->hwnd, &rect);
	*centerX = rect.left + (rect.right - rect.left) / 2;
	*centerY = rect.top + (rect.bottom - rect.top) / 2;
}




void CCgxWindow::getLocateRECT(RECT* rectOut)
{
	rectOut->left = 346;
	rectOut->top = 30;
	rectOut->right = 620;
	rectOut->bottom = 234;
}
