
// ScriptTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScriptTest.h"
#include "ScriptTestDlg.h"
#include "afxdialogex.h"
#include "Script.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScriptTestDlg 对话框




CScriptTestDlg::CScriptTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScriptTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScriptTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScriptTestDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CScriptTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CScriptTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CScriptTestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CScriptTestDlg 消息处理程序

BOOL CScriptTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CScriptTestDlg::OnPaint()
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
HCURSOR CScriptTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScriptTestDlg::OnBnClickedButton1()
{
	CFile file;
	file.Open(TEXT("..\\cgx-0.2\\script\\灵堂.txt"), CFile::modeRead);
	if(file.GetLength() < 1)
	{
		TRACE("Empty file.\n");
		return;
	}
	UINT fileSize = file.GetLength();
	TRACE("File size: %d\n", fileSize);
	char* buff = new char[fileSize+1];
	file.Read(buff, fileSize);

	int size = MultiByteToWideChar(CP_ACP, NULL, buff, fileSize, NULL, 0);
	TCHAR* text = new TCHAR[size+1];
	MultiByteToWideChar(CP_ACP, NULL, buff, fileSize, text, size);
	text[size] = '\0';
	CScript script(text);
	script.loadScript(text);
	while(script.nextStep())
		TRACE("COMMAND: %d, (%d, %d), target: (%d, %d)\n",script.command, script.x, script.y, script.targetX, script.targetY);
	delete[] buff;
	delete[] text;
	file.Close();


}


void CScriptTestDlg::OnBnClickedButton2()
{
	TCHAR buff[MAX_PATH] = {0};
	DWORD size = MAX_PATH;
	GetPrivateProfileString(TEXT("scriptTest"), TEXT("test"),
		TEXT("default"), buff, size, TEXT("config/config.ini") );
	TRACE(buff);
	TRACE("\n");
	GetPrivateProfileString(TEXT("测试"), TEXT("1串中文"),
		TEXT("default"), buff, size, TEXT("config/config.ini") );
	TRACE(buff);
	TRACE("\n");
}


void CScriptTestDlg::OnBnClickedButton3()
{
	WritePrivateProfileString(TEXT("scriptTest"), TEXT("test"),
		TEXT("testValue"), TEXT("config/config.ini"));
	WritePrivateProfileString(TEXT("测试"), TEXT("1串中文"),
		TEXT("testValue"), TEXT("config/config.ini"));
}
