// TestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "cgx.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include "cgxData.h"


// CTestDlg 对话框

IMPLEMENT_DYNAMIC(CTestDlg, CDialogEx)

CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestDlg::IDD, pParent)
{

}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_REFRESH_GAME, &CTestDlg::OnBnClickedRefreshGame)
	ON_BN_CLICKED(IDC_FIND_MONSTER, &CTestDlg::OnBnClickedFindMonster)
	ON_BN_CLICKED(IDC_MONSTER_FIGHT, &CTestDlg::OnBnClickedMonsterFight)
	ON_BN_CLICKED(IDC_MAKE_SCREENSHOT, &CTestDlg::OnBnClickedMakeScreenshot)
	ON_BN_CLICKED(IDC_PLAYER_FIGHT, &CTestDlg::OnBnClickedPlayerFight)
	ON_BN_CLICKED(IDC_CHECK_STATUS, &CTestDlg::OnBnClickedCheckStatus)
	ON_BN_CLICKED(IDC_HIT_HIT_BTN, &CTestDlg::OnBnClickedHitHitBtn)
	ON_BN_CLICKED(IDC_HIT_SKILL_BTN, &CTestDlg::OnBnClickedHitSkillBtn)
	ON_BN_CLICKED(IDC_FIND_SKILL_WINDOW, &CTestDlg::OnBnClickedFindSkillWindow)
	ON_BN_CLICKED(IDC_REFRESH_SCREENSHOT_LIST, &CTestDlg::OnBnClickedRefreshScreenshotList)
	ON_BN_CLICKED(IDC_LOAD_IMAGE, &CTestDlg::OnBnClickedLoadImage)
	ON_BN_CLICKED(IDC_SAVE_SKILL, &CTestDlg::OnBnClickedSaveSkill)
	ON_BN_CLICKED(IDC_PRINT_SKILL, &CTestDlg::OnBnClickedPrintSkill)
	ON_BN_CLICKED(IDC_GET_CURRENT_SKILL_MAX_LEVEL, &CTestDlg::OnBnClickedGetCurrentSkillMaxLevel)
	ON_BN_CLICKED(IDC_HIT_SKILL_1, &CTestDlg::OnBnClickedHitSkill1)
	ON_BN_CLICKED(IDC_HIT_MAX_LEVEL_SKILL, &CTestDlg::OnBnClickedHitMaxLevelSkill)
	ON_BN_CLICKED(IDC_AUTO_FIGHT, &CTestDlg::OnBnClickedAutoFight)
END_MESSAGE_MAP()


// CTestDlg 消息处理程序



void CTestDlg::OnBnClickedRefreshGame()
{
	// TODO: 在此添加控件通知处理程序代码
	if(this->gameManager.gameSize > 0)
	{
		this->gameManager.games[0]->refresh();
	}
}


void CTestDlg::OnBnClickedFindMonster()
{
	static CStatic* texts[10] = {
		(CStatic *)GetDlgItem(IDC_MONSTER1),
		(CStatic *)GetDlgItem(IDC_MONSTER2),
		(CStatic *)GetDlgItem(IDC_MONSTER3),
		(CStatic *)GetDlgItem(IDC_MONSTER4),
		(CStatic *)GetDlgItem(IDC_MONSTER5),
		(CStatic *)GetDlgItem(IDC_MONSTER6),
		(CStatic *)GetDlgItem(IDC_MONSTER7),
		(CStatic *)GetDlgItem(IDC_MONSTER8),
		(CStatic *)GetDlgItem(IDC_MONSTER9),
		(CStatic *)GetDlgItem(IDC_MONSTER10),
	};
	if(this->gameManager.gameSize > 0)
	{
		
		this->gameManager.games[0]->findMonster();
		for(int i = 0; i < MAX_MONSTER_LOCATION; ++i)
		{
			if(gameManager.games[0]->monsterHolder.isAlive(i))
			{
				texts[i]->SetWindowTextW(TEXT("1"));
				//this->gameManager.games[0]->leftClick(locations[i].x+30, locations[i].y);
			} else {
				texts[i]->SetWindowTextW(TEXT("0"));
			}
		}
	}
}

void CTestDlg::OnBnClickedMonsterFight()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CTestDlg::OnBnClickedMakeScreenshot()
{
	if(this->gameManager.gameSize > 0)
	{
		this->gameManager.games[0]->saveScreenshot2file(TEXT("D:\\tmp.bmp"));
	}
}


void CTestDlg::OnBnClickedPlayerFight()
{
	if(this->gameManager.gameSize > 0)
	{
		this->gameManager.games[0]->refresh();
		Sleep(300);
		this->gameManager.games[0]->findMonster();
		for(int i = 0; i < MAX_MONSTER_LOCATION; ++i)
		{
			if(gameManager.games[0]->monsterHolder.isAlive(i))
			{
				this->gameManager.games[0]->hitMonster(i);
				return;
			}
		}
	}
}

#define CHECK_STATUS(s) \
	else if(this->gameManager.games[0]->getStatus() == ##s) \
		_appendTextToEditCtrl(TEXT(#s));

void CTestDlg::OnBnClickedCheckStatus()
{
	if(this->gameManager.gameSize > 0)
	{
		this->gameManager.games[0]->checkStatus();
		CEdit* text = (CEdit*)GetDlgItem(IDC_MESSAGE_BOX);
		if(0);
		CHECK_STATUS(CGame::GAME_STATUS_TRAVEL)
		CHECK_STATUS(CGame::GAME_STATUS_PLAYER_FIGHT)
		CHECK_STATUS(CGame::GAME_STATUS_MONSTER_FIGHT)
		CHECK_STATUS(CGame::GAME_STATUS_UNKNOW)
		else
			text->SetWindowTextW(TEXT("=UNKNOW="));
	}
}


void CTestDlg::OnBnClickedHitHitBtn()
{
	if(this->gameManager.gameSize > 0)
	{
		this->gameManager.games[0]->hitFitBtn(FIT_BTN_HIT);
	}
}


void CTestDlg::OnBnClickedHitSkillBtn()
{
	if(this->gameManager.gameSize > 0)
	{
		this->gameManager.games[0]->hitFitBtn(FIT_BTN_SKILL);
	}
}


void CTestDlg::OnBnClickedFindSkillWindow()
{
	if(this->gameManager.gameSize > 0)
	{
		TCHAR buff[1024] = {0};
		PGAME_LOCATION l = this->gameManager.games[0]->findSkillWindow();
		swprintf(buff, TEXT("left: %d, top: %d, width: %d, height: %d"), l->x, l->y, l->cx, l->cy);
		_appendTextToEditCtrl( buff);
	}
}


void CTestDlg::OnBnClickedRefreshScreenshotList()
{
	TCHAR bmpPath[MAX_PATH] = {0};
	TCHAR tmp[MAX_PATH] = {0};
	getCurrentCgx(tmp);
	swprintf(bmpPath, TEXT("%s\\*.bmp"), tmp);
	TRACE(TEXT("%s\n"), bmpPath);
	CFileFind finder;
	BOOL bWorking = finder.FindFile(bmpPath);
	CListBox* listBox = (CListBox *)GetDlgItem(IDC_SCREENSHOT_LIST);
	listBox->ResetContent();
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		listBox->AddString(finder.GetFilePath());
		
	}
}


void CTestDlg::OnBnClickedLoadImage()
{
	if(this->gameManager.gameSize >0)
	{
		TCHAR buff[MAX_PATH] = {0};
		CString str;
		CListBox* listBox = (CListBox *)GetDlgItem(IDC_SCREENSHOT_LIST);
		int idx = listBox->GetCurSel();
		if(idx > -1)
		{
			int len = listBox->GetTextLen(idx);
			listBox->GetText(idx, str);
			this->gameManager.games[0]->load4refresh(str);
		}
		
	}
}


void CTestDlg::OnBnClickedSaveSkill()
{
	if(this->gameManager.gameSize >0)
	{
		TCHAR bmpPath[MAX_PATH] = {0};
		TCHAR tmp[MAX_PATH] = {0};
		getCurrentCgx(tmp);
		swprintf(bmpPath, TEXT("%s\\skill"), tmp);
		this->gameManager.games[0]->saveSkillPhotos(bmpPath);
	}
}

void CTestDlg::getCurrentCgx( PTSTR buff )
{
	TCHAR tmp[MAX_PATH] = {0};
	GetCurrentDirectory(sizeof(tmp), tmp);
	swprintf(buff, TEXT("%c:\\cgx"), tmp[0]);
}


void CTestDlg::OnBnClickedPrintSkill()
{
	if(this->gameManager.gameSize > 0)
	{
		TCHAR buff[1024] = {0};

		for(int i = 0; i < 10; ++i)
		{
			CSkill* skill =  &(this->gameManager.games[0]->skills[i]);
			swprintf(buff, TEXT("%d - %-8s\t%d"), i, skill->name, skill->attackNumber);
			_appendTextToEditCtrl(buff);
		}
	}
}

void CTestDlg::_appendTextToEditCtrl(LPCTSTR pszText )
{
	CEdit* edit = (CEdit*)GetDlgItem(IDC_MESSAGE_BOX);
	// get the initial text length
	int nLength = edit->GetWindowTextLength();
	// put the selection at the end of text
	edit->SetSel(nLength, nLength);
	// replace the selection
	edit->ReplaceSel(pszText);
	nLength = edit->GetWindowTextLength();
	edit->SetSel(nLength, nLength);
	edit->ReplaceSel(TEXT("\n"));
}


void CTestDlg::OnBnClickedGetCurrentSkillMaxLevel()
{
	if(this->gameManager.gameSize > 0)
	{
		TCHAR buff[1024] = {0};
		int lv = this->gameManager.games[0]->getCurrentSkillMaxLevel();
		swprintf(buff, TEXT("Current skill level %d"), lv);
		_appendTextToEditCtrl(buff);
		
	}
}


void CTestDlg::OnBnClickedHitSkill1()
{
	if(this->gameManager.gameSize > 0)
	{
		gameManager.games[0]->choiceSkill(0);
	}
}


void CTestDlg::OnBnClickedHitMaxLevelSkill()
{
	if(this->gameManager.gameSize > 0)
	{
		int maxLv = gameManager.games[0]->getCurrentSkillMaxLevel();
		gameManager.games[0]->choiceSkillLevel(maxLv);
	}
}


void CTestDlg::OnBnClickedAutoFight()
{
	if(this->gameManager.gameSize > 0)
	{
		
		gameManager.games[0]->startAutoFighting();
	}
}