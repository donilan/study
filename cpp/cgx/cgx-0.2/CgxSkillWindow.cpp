#include "StdAfx.h"
#include "CgxSkillWindow.h"
#include "resource.h"



CCgxSkillWindow::CCgxSkillWindow(CHWNDScreen* screen)
	: CCgxWindow(IDB_JOB, screen)
{
}


CCgxSkillWindow::~CCgxSkillWindow(void)
{
}


// Find skill rect
void CCgxSkillWindow::skillRECT(INT index, RECT* skillRECTOut)
{
	if(!skillRECTOut) return;
	memset(skillRECTOut, 0, sizeof(RECT));
	index--;
	if(index < 0) index = 0;
	if(index > 9) index = 0;
	if(locate())
	{
		skillRECTOut->left = rect.left+5;
		skillRECTOut->right = skillRECTOut->left+ 12*4;
		skillRECTOut->top = rect.top+36 + (16*index);
		skillRECTOut->bottom = skillRECTOut->top + 12;
	}
}
