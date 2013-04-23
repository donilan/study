// SystemTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SystemTestDlg.h"
#include "afxdialogex.h"

#include "System.h"
#include "CgxMonster.h"
#include "CgxTopRightWindow.h"
#include "CgxPlayerCommand.h"
#include "CgxMapWindow.h"


// CSystemTestDlg 对话框

IMPLEMENT_DYNAMIC(CSystemTestDlg, CDialogEx)

CSystemTestDlg::CSystemTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemTestDlg::IDD, pParent)
{
	pScreen = NULL;
	skillWindow = NULL;
	goodsWindow = NULL;
	topRightWindow = NULL;
	petCommandWindow = NULL;
	checkingStatus = FALSE;
	game = NULL;
}

CSystemTestDlg::~CSystemTestDlg()
{
	if(game) delete game;
}

void CSystemTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSystemTestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_LOCK_SCREEN, &CSystemTestDlg::OnBnClickedLockScreen)
	ON_BN_CLICKED(IDC_LEFT_CLICK, &CSystemTestDlg::OnBnClickedLeftClick)
	ON_BN_CLICKED(IDC_SCREENSHOT, &CSystemTestDlg::OnBnClickedScreenshot)
	ON_BN_CLICKED(IDC_RGB_RATE, &CSystemTestDlg::OnBnClickedRgbRate)
	ON_BN_CLICKED(IDC_DRAW_SCREEN, &CSystemTestDlg::OnBnClickedDrawScreen)
	ON_BN_CLICKED(IDC_REFRESH_IMAGE_LIST, &CSystemTestDlg::OnBnClickedRefreshImageList)
	ON_LBN_DBLCLK(IDC_IMAGE_LIST, &CSystemTestDlg::OnLbnDblclkImageList)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_PRINT, &CSystemTestDlg::OnBnClickedPrint)
	ON_BN_CLICKED(IDC_SAVE_RECT, &CSystemTestDlg::OnBnClickedSaveRect)
	ON_BN_CLICKED(IDC_MATCH, &CSystemTestDlg::OnBnClickedMatch)
	ON_BN_CLICKED(IDC_LOCATE, &CSystemTestDlg::OnBnClickedLocate)
	ON_BN_CLICKED(IDC_REFRESH_MATCH_LIST, &CSystemTestDlg::OnBnClickedRefreshMatchList)
	ON_LBN_DBLCLK(IDC_MATCH_LIST, &CSystemTestDlg::OnLbnDblclkMatchList)
	ON_BN_CLICKED(IDC_LOCATE_JOB_WINDOW, &CSystemTestDlg::OnBnClickedLocateJobWindow)

	ON_BN_CLICKED(IDC_LOCATE_GOODS, &CSystemTestDlg::OnBnClickedLocateGoods)
	ON_BN_CLICKED(IDC_LOCATE_MONSTER, &CSystemTestDlg::OnBnClickedLocateMonster)
	ON_BN_CLICKED(IDC_LOCATE_TOP_RIGHT_WINDOW, &CSystemTestDlg::OnBnClickedLocateTopRightWindow)
	ON_BN_CLICKED(IDC_LOCATE_PET_CAMMAND, &CSystemTestDlg::OnBnClickedLocatePetCammand)
	ON_BN_CLICKED(IDC_LOCATE_PLAYER_COMMAND, &CSystemTestDlg::OnBnClickedLocatePlayerCommand)
	ON_BN_CLICKED(IDC_LOCATE_PET_SKILL, &CSystemTestDlg::OnBnClickedLocatePetSkill)
	ON_BN_CLICKED(IDC_LOCATE_LEVEL, &CSystemTestDlg::OnBnClickedLocateLevel)
	ON_BN_CLICKED(IDC_CHECK_STATUS, &CSystemTestDlg::OnBnClickedCheckStatus)
	ON_BN_CLICKED(IDC_LOCATE_MAP_WINDOW, &CSystemTestDlg::OnBnClickedLocateMapWindow)
END_MESSAGE_MAP()


// CSystemTestDlg 消息处理程序

UINT OnBnClickedLockScreenThread(LPVOID pParam) {
	CSystem::lockScreen(TRUE);
	TRACE(TEXT("Sleeping"));
	for(int i = 0; i < 3; ++i)
	{
		Sleep(1000);
		TRACE(TEXT("."));
	}
	TRACE(TEXT("\n"));
	CSystem::lockScreen(FALSE);
	return 0;
}
void CSystemTestDlg::OnBnClickedLockScreen()
{
	
	AfxBeginThread(OnBnClickedLockScreenThread, NULL);
}


void CSystemTestDlg::OnBnClickedLeftClick()
{
	CSystem::leftClick(30, 30);
}

#define WINDOW_TITLE TEXT("\u9b54\u529b\u5b9d\u8d1d")
BOOL CALLBACK FindTitleWindowsProc (HWND hwnd, LPARAM lParam)
{
	TCHAR buff[MAX_PATH] = {0};
	HWND* out = (HWND *)lParam;

	GetWindowText(hwnd, buff, MAX_PATH);
	if(_tcsncmp(buff, WINDOW_TITLE, lstrlen(WINDOW_TITLE)) == 0)
	{
		TRACE("Found one.\n");
		*out = hwnd;
	}
	return TRUE;
}

void CSystemTestDlg::OnBnClickedScreenshot()
{
	HWND hwnd = NULL;
	TRACE("before EnumWindows\n");
	if(EnumWindows(FindTitleWindowsProc, (LPARAM)&hwnd))
	{
		::ShowWindow(hwnd, SW_SHOW);
		::SetForegroundWindow(hwnd);
		::SetFocus(hwnd);
		Sleep(100);
		//if(pScreen) delete pScreen;
		//pScreen = new CHWNDScreen(hwnd);
		_initScreen(hwnd);
		if(game) delete game;
		game = new CGame(hwnd);
		TRACE("Saving screenshot.\n");
		pScreen->refresh();
		pScreen->screenshot(TEXT("img\\tmp.bmp"));
	}
}


void CSystemTestDlg::OnBnClickedRgbRate()
{
	RECT rect = {100, 100, 200, 200};
	if(pScreen)
	{
		pScreen->colorDeviation(&rect, RGB(255, 255, 255));
		CHWNDScreen::flashRECT(&rect);
	}
}


void CSystemTestDlg::OnBnClickedDrawScreen()
{
	RECT rect = {200, 200, 400, 400};
	CHWNDScreen::flashRECT(&rect);
}


void CSystemTestDlg::OnBnClickedRefreshImageList()
{
	CFileFind finder;
	CListBox* list = (CListBox*)GetDlgItem(IDC_IMAGE_LIST);
	list->ResetContent();
	BOOL bWorking = finder.FindFile(TEXT("img\\screenshot\\*.bmp"));
	while(bWorking) {
		bWorking = finder.FindNextFile();
		list->AddString(finder.GetFileName());
	}
	_out(TEXT("Refresh done."));
}


void CSystemTestDlg::OnLbnDblclkImageList()
{
	CString str;
	TCHAR buff[MAX_PATH] = {0};
	CListBox* listBox = (CListBox *)GetDlgItem(IDC_IMAGE_LIST);
	int idx = listBox->GetCurSel();
	if(idx > -1)
	{
		int len = listBox->GetTextLen(idx);
		listBox->GetText(idx, str);
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("img\\screenshot\\%s"), str.GetBuffer(0));

		_out(TEXT("Loading file:"));
		_out(buff);
		_initScreen(buff);
		
	}
	
}


int CSystemTestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;


	return 0;
}


void CSystemTestDlg::_out(PTSTR pszText)
{
	CEdit* edit = (CEdit*)GetDlgItem(IDC_OUT);
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


BOOL CSystemTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	OnBnClickedRefreshImageList();
	OnBnClickedRefreshMatchList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE


}


void CSystemTestDlg::OnBnClickedPrint()
{
	if(pScreen)
		pScreen->print();
}


void CSystemTestDlg::OnBnClickedSaveRect()
{
	RECT rect = {200, 200, 400, 400};
	if(pScreen)
	{	pScreen->flashRECT(&rect);
		pScreen->saveRECT(&rect, TEXT("img\\rect-tmp.bmp"));
	}
}


void CSystemTestDlg::OnBnClickedMatch()
{
	RECT rect = {200, 200, 400, 400};
	CImage image;
	if(pScreen)
	{	
		CHWNDScreen::loadInToCImage(TEXT("img\\rect-tmp.bmp"), &image);
		if(pScreen->match(&image, &rect))
		{
			_out(TEXT("MATCH"));
		}
	}
}


void CSystemTestDlg::OnBnClickedLocate()
{
	TCHAR buff[MAX_PATH]= {0};
	RECT rect;
	CImage image;
	if(pScreen)
	{	
		CHWNDScreen::loadInToCImage(TEXT("img\\rect-tmp.bmp"), &image);
		if(pScreen->locate(&image, &rect))
		{
			swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("Locate left: %d, right: %d, top: %d, bottom: %d")
				, rect.left, rect.right, rect.top, rect.bottom);
			_out(buff);
		}

	}
}


void CSystemTestDlg::OnBnClickedRefreshMatchList()
{
	CFileFind finder;
	CListBox* list = (CListBox*)GetDlgItem(IDC_MATCH_LIST);
	list->ResetContent();
	BOOL bWorking = finder.FindFile(TEXT("img\\match\\*.bmp"));
	while(bWorking) {
		bWorking = finder.FindNextFile();
		list->AddString(finder.GetFileName());
	}
	_out(TEXT("Refresh Match list done."));
}


void CSystemTestDlg::OnLbnDblclkMatchList()
{
	CString str;
	TCHAR buff[MAX_PATH] = {0};
	CListBox* listBox = (CListBox *)GetDlgItem(IDC_MATCH_LIST);
	int idx = listBox->GetCurSel();
	CImage image;
	RECT rect;
	if(idx > -1)
	{
		int len = listBox->GetTextLen(idx);
		listBox->GetText(idx, str);
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("img\\match\\%s"), str.GetBuffer(0));
		CHWNDScreen::loadInToCImage(buff, &image);
		_out(TEXT("Load match image done."));
		if(pScreen->locate(&image, &rect))
		{
			swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("Locate left: %d, right: %d, top: %d, bottom: %d")
				, rect.left, rect.right, rect.top, rect.bottom);
			_out(buff);
			CHWNDScreen::flashRECT(&rect);
		}
	}
}


void CSystemTestDlg::OnBnClickedLocateJobWindow()
{
	TCHAR buff[MAX_PATH] = {0};
	if(pScreen)
	{
		_locateWindowInfo(skillWindow, TEXT("Player Skill"));
		
	}
}


//void CSystemTestDlg::OnBnClickedLocateSkill()
//{
//	RECT rect;
//	if(pScreen)
//	{
//		if(!skillWindow) skillWindow = new CCgxSkillWindow(IDB_PLAYER_SKILL, pScreen);
//		for(int i = 0; i < 10; ++i)
//		{
//			skillWindow->getCommand(i, &rect);
//			CHWNDScreen::flashRECT(&rect);
//		}
//	}
//}


void CSystemTestDlg::_initScreen(PTSTR pszFilePath)
{
	if(pScreen) delete pScreen;
	pScreen = new CHWNDScreen(NULL);
	pScreen->loadImage(pszFilePath);
	_initWindows();
}


void CSystemTestDlg::_initScreen(HWND hwnd)
{
	if(pScreen) delete pScreen;
	pScreen = new CHWNDScreen(hwnd);
}


void CSystemTestDlg::OnBnClickedLocateGoods()
{
	if(pScreen)
	{
		if(!goodsWindow) goodsWindow = new CCgxGoodsWindow(pScreen);
		_locateWindowInfo(goodsWindow, TEXT("Goods"));
	}
}


void CSystemTestDlg::OnBnClickedLocateMonster()
{
	TCHAR buff[MAX_PATH] = {0};
	if(pScreen)
	{
		CCgxMonster monster(pScreen);
		int alive = monster.countAlive();
		swprintf(buff, sizeof(TCHAR)* MAX_PATH, TEXT("Found number of monster is: %d"), alive);
		_out(buff);
		for(int i = 0; i < NUMBER_OF_MONSTER; ++i)
		{
			if(monster.isAlive(i))
			{
				swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("Alive index: %d"), i);
				_out(buff);
				CHWNDScreen::flashRECT(&monster.monsterRECTs[i]);
			}
		}
	}
}


void CSystemTestDlg::OnBnClickedLocateTopRightWindow()
{
	if(pScreen)
	{
		_locateWindowInfo(topRightWindow, TEXT("Top right"));
	}
}



void CSystemTestDlg::_locateWindowInfo(CCgxWindow* wind, PTSTR winName)
{
	TCHAR buff[MAX_PATH] = {0};
	RECT rect;
	if(wind->locate())
	{
		CHWNDScreen::flashRECT(&(wind->rect));
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("Window [%s] x: %d y: %d"), winName, wind->rect.left, wind->rect.top);
		_out(buff);
		for(int i = 0; i < MAX_COMMAND; ++i)
		{
			if(wind->getCommand(i, &rect))
			{
				CHWNDScreen::flashRECT(&rect);
				if(wind->isCommandEnable(i))
					TRACE("Command[%d] is enabled.\n", i);
				else
					TRACE("Command[%d] is diabled.\n", i);
			}
		}
		
	}
	else
	{
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("Can not locate [%s] window"), winName);
		_out(buff);
	}
}

void CSystemTestDlg::_initWindows( void )
{
	if(skillWindow) delete skillWindow; 
	skillWindow = new CCgxSkillWindow(IDB_PLAYER_SKILL, pScreen);
	if(goodsWindow) delete goodsWindow;
	goodsWindow = new CCgxGoodsWindow(pScreen);

	if(petCommandWindow) delete petCommandWindow; 
	petCommandWindow = new CCgxPetCommandWindow(pScreen);

	if(topRightWindow) delete topRightWindow; 
	topRightWindow = new CCgxTopRightWindow(pScreen);
	
}


void CSystemTestDlg::OnBnClickedLocatePetCammand()
{
	if(pScreen)
	{
		_locateWindowInfo(petCommandWindow, TEXT("Pet Command"));
		
	}
}

void CSystemTestDlg::OnBnClickedLocatePlayerCommand()
{
	if(pScreen)
	{
		CCgxPlayerCommand playerCommand(pScreen);
		_locateWindowInfo(&playerCommand, TEXT("Player Command"));
		playerCommand.clickHitCommand();
	}
}


void CSystemTestDlg::OnBnClickedLocatePetSkill()
{
	if(pScreen)
	{
		CCgxSkillWindow petSkill(IDB_PET_SKILL, pScreen);
		_locateWindowInfo(&petSkill, TEXT("Player Command"));
		
	}
}


void CSystemTestDlg::OnBnClickedLocateLevel()
{
	if(pScreen)
	{
		CCgxSkillWindow skill(IDB_SKILL_LV, pScreen);
		_locateWindowInfo(&skill, TEXT("Skill Level"));
		
	}
}

UINT CheckStatusThread(LPVOID lpVoid)
{
	CGame* game = (CGame*)lpVoid;
	while(TRUE)
	{
		TRACE("Game status: %d\n", game->getStatus());
		Sleep(500);
	}
	
}

void CSystemTestDlg::OnBnClickedCheckStatus()
{
	if(checkingStatus || !game) return;
	AfxBeginThread(CheckStatusThread, game);
}


void CSystemTestDlg::OnBnClickedLocateMapWindow()
{
	if(pScreen)
	{
		CCgxMapWindow map(pScreen);
		_locateWindowInfo(&map, TEXT("Map"));
		//pScreen->flashRECT(&map.xRECT);
		//pScreen->flashRECT(&map.yRECT);
		int x = pScreen->toNumber(&map.xRECT);
		int y = pScreen->toNumber(&map.yRECT);
		TRACE("%d, %d\n", x, y);
	}
}
