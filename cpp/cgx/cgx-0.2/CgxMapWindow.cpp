#include "StdAfx.h"
#include "CgxMapWindow.h"
#include "resource.h"

CCgxMapWindow::CCgxMapWindow(CHWNDScreen* pScreen)
	: CCgxWindow(IDB_MAP, pScreen)
{
	memset(&xRECT, 0, sizeof(RECT));
	memset(&yRECT, 0, sizeof(RECT));
}


CCgxMapWindow::~CCgxMapWindow(void)
{
}

void CCgxMapWindow::locateCommands(void)
{
	xRECT.top = rect.top+22;
	xRECT.bottom = xRECT.top + 8;
	xRECT.left = rect.left - 24;
	xRECT.right = xRECT.left + 24;

	yRECT.top = rect.top+22;
	yRECT.bottom = yRECT.top + 8;
	yRECT.left = rect.left+84;
	yRECT.right = yRECT.left + 24;
}
