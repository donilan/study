
// CgxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "cgx.h"
#include "CgxDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCgxDlg 对话框




CCgxDlg::CCgxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCgxDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CCgxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCgxDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HOTKEY()
	ON_BN_CLICKED(IDC_REFRESH, &CCgxDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDC_AUTO_FIGHT, &CCgxDlg::OnBnClickedAutoFight)
END_MESSAGE_MAP()


// CCgxDlg 消息处理程序

BOOL CCgxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	RegisterHotKey(this->m_hWnd, HOTKEY_F2, 0, VK_F2);
	isTestDlgOpen = FALSE;


	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
	OnBnClickedRefresh();

	TCHAR buff[MAX_PATH] = {0};
	CComboBox* single = (CComboBox*)GetDlgItem(IDC_SINGLE_SKILL_LIST);
	CComboBox* four = (CComboBox*)GetDlgItem(IDC_FOUR_SKILL_LIST);
	CComboBox* all = (CComboBox*)GetDlgItem(IDC_ALL_SKILL_LIST);
	CComboBox* singleLv = (CComboBox*)GetDlgItem(IDC_SINGLE_SKILL_LIST_LV);
	CComboBox* fourLv = (CComboBox*)GetDlgItem(IDC_FOUR_SKILL_LIST_LV);
	CComboBox* allLv = (CComboBox*)GetDlgItem(IDC_ALL_SKILL_LIST_LV);
	CComboBox* pet = (CComboBox*)GetDlgItem(IDC_PET_SKILL);

	single->AddString(TEXT("攻击"));
	four->AddString(TEXT("攻击"));
	all->AddString(TEXT("攻击"));
	for(int i = 0 ; i < 10; ++i)
	{
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("技能%02d"), i+1);
		single->AddString(buff);
		four->AddString(buff);
		all->AddString(buff);
		pet->AddString(buff);
		
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("Lv%d"), i+1);
		singleLv->AddString(buff);
		fourLv->AddString(buff);
		allLv->AddString(buff);
	}
	single->SetCurSel(0);
	four->SetCurSel(0);
	all->SetCurSel(0);
	singleLv->SetCurSel(0);
	fourLv->SetCurSel(0);
	allLv->SetCurSel(0);
	pet->SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCgxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCgxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCgxDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{

	switch(nHotKeyId)
	{
	case HOTKEY_F2:
		if(!isTestDlgOpen)
		{
			isTestDlgOpen = TRUE;
			systemTestDlg.DoModal();
			isTestDlgOpen = FALSE;
		}
		break;
	}
	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CCgxDlg::OnBnClickedRefresh()
{
	
	TCHAR buff[MAX_PATH] = {0};
	gameManager.refresh();
	CComboBox* list = (CComboBox*)GetDlgItem(IDC_GAME_LIST);
	list->ResetContent();
	for(int i = 0; i < gameManager.gameSize; ++i)
	{
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("游戏%d"), i);
		list->AddString(buff);
	}
	list->SetCurSel(0);
}



void CCgxDlg::OnBnClickedAutoFight()
{
	CButton* chk = (CButton*)GetDlgItem(IDC_AUTO_FIGHT);
	CComboBox* gameList = (CComboBox*)GetDlgItem(IDC_GAME_LIST);
	int index = gameList->GetCurSel();
	if(index > gameManager.gameSize)
		return;
	if(chk && chk->GetCheck())
	{
		gameManager.games[index]->startAutoFight();
	}
	else
	{
		gameManager.games[index]->stopAutoFight();
	}
}
