// SystemTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SystemTestDlg.h"
#include "afxdialogex.h"

#include "System.h"


// CSystemTestDlg 对话框

IMPLEMENT_DYNAMIC(CSystemTestDlg, CDialogEx)

CSystemTestDlg::CSystemTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemTestDlg::IDD, pParent)
{
	pScreen = NULL;
}

CSystemTestDlg::~CSystemTestDlg()
{
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

#define WINDOW_TITLE TEXT("cgx")
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
		if(pScreen) delete pScreen;
		pScreen = new CHWNDScreen(hwnd);
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
