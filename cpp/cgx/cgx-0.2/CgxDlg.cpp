
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
	ON_BN_CLICKED(IDC_START, &CCgxDlg::OnBnClickedStart)
	ON_CBN_SELCHANGE(IDC_SCRIPT_LIST, &CCgxDlg::OnCbnSelchangeScriptList)
	ON_CBN_SETFOCUS(IDC_SCRIPT_LIST, &CCgxDlg::OnCbnSetfocusScriptList)
END_MESSAGE_MAP()


// CCgxDlg 消息处理程序

BOOL CCgxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	


	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
#ifdef DEBUG
	RegisterHotKey(this->m_hWnd, HOTKEY_F2, 0, VK_F2);
	isTestDlgOpen = FALSE;
#endif
	RegisterHotKey(this->m_hWnd, HOTKEY_F5, 0, VK_F5);
	RegisterHotKey(this->m_hWnd, HOTKEY_F6, 0, VK_F6);
	OnBnClickedRefresh();
	_initConfigFile();

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
	CComboBox* gameList = (CComboBox*)GetDlgItem(IDC_GAME_LIST);
	int index = gameList->GetCurSel();
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
	case HOTKEY_F5:
		OnBnClickedStart();
		break;
	case HOTKEY_F6:
		gameManager.games[index]->stopAI();
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



void CCgxDlg::OnBnClickedStart()
{
	CComboBox* gameList = (CComboBox*)GetDlgItem(IDC_GAME_LIST);
	CButton* startBtn = (CButton *)GetDlgItem(IDC_START);
	CEdit* script = (CEdit *)GetDlgItem(IDC_SCRIPT);
	CString strLine;

	int index = gameList->GetCurSel();
	if(gameManager.gameSize == 0 || index > gameManager.gameSize)
		return;
	if(gameManager.games[index]->isAIStart)
	{
		gameManager.games[index]->stopAI();
		startBtn->SetWindowTextW(TEXT("启动"));
	} 
	else
	{
		script->GetWindowTextW(strLine);
		gameManager.games[index]->script.loadScript((LPCWSTR)strLine);
		gameManager.games[index]->startAI();
		startBtn->SetWindowTextW(TEXT("停止"));
	}
}


void CCgxDlg::OnOK()
{
	// Do nothing
}


void CCgxDlg::OnCbnSelchangeScriptList()
{
	TCHAR fileName[MAX_PATH] = {0};
	TCHAR path[MAX_PATH] = {0};
	CComboBox* scriptList = (CComboBox *)GetDlgItem(IDC_SCRIPT_LIST);
	CEdit* script = (CEdit *)GetDlgItem(IDC_SCRIPT);
	int idx = scriptList->GetCurSel();
	scriptList->GetLBText(idx, fileName);
	swprintf(path, sizeof(TCHAR)*MAX_PATH, TEXT("script\\%s"), fileName);
	CFile file;
	file.Open(path, CFile::modeRead);
	if(file.GetLength() < 1)
	{
		return;
	}
	ULONGLONG fileSize = file.GetLength();
	char* buff = new char[fileSize];
	file.Read(buff, fileSize);
	
	int size = MultiByteToWideChar(CP_ACP, NULL, buff, fileSize, NULL, 0);
	TCHAR* text = new TCHAR[size+1];
	MultiByteToWideChar(CP_ACP, NULL, buff, fileSize, text, size);
	text[size] = '\0';
	script->SetWindowTextW(text);
	TRACE(text);
	delete[] buff;
	delete[] text;
	file.Close();
}


void CCgxDlg::OnCbnSetfocusScriptList()
{
	CFileFind find;
	BOOL isWorking = find.FindFile(TEXT("script/*.txt"));
	CComboBox* scriptList = (CComboBox *)GetDlgItem(IDC_SCRIPT_LIST);
	scriptList->ResetContent();
	while(isWorking)
	{
		isWorking = find.FindNextFile();
		scriptList->AddString(find.GetFileName());
	}
}

void CCgxDlg::_initConfigFile(void)
{
	CString keySk;
	CString valSk;
	CString keyLv;
	CString valLv;

	CFileFind finder;
	BOOL isExists = finder.FindFile(CONFIG_FILE);
	
	if(!isExists)
	{
		TRACE("Init config\n");
		for(int i = 1; i < 11; ++i)
		{
			keySk.Format(NUMBER_OF_MONSTER_SKILL, i);
			valSk.Format(TEXT("%d"), i == 1? 0: 1);
			keyLv.Format(NUMBER_OF_MONSTER_SKILL_LV, i);
			valLv.Format(TEXT("%d"), i < 3? 1: i-1);
			WritePrivateProfileString(FIGHT_SKILL, keySk.GetBuffer(20),
				valSk.GetBuffer(10), CONFIG_FILE);
			WritePrivateProfileString(FIGHT_SKILL, keyLv.GetBuffer(20),
				valLv.GetBuffer(10), CONFIG_FILE);
		}
	}
}
