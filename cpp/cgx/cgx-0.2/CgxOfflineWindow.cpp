#include "StdAfx.h"
#include "CgxOfflineWindow.h"
#include "resource.h"

CCgxOfflineWindow::CCgxOfflineWindow(CHWNDScreen* pScreen)
	:CCgxWindow(IDB_SURE, pScreen)
{
}


CCgxOfflineWindow::~CCgxOfflineWindow(void)
{
}

void CCgxOfflineWindow::getLocateRECT( RECT* rectOut )
{
	rectOut->left = 285;
	rectOut->top = 290;
	rectOut->right = 360;
	rectOut->bottom = 320;
}
