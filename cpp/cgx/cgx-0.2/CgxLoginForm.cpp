#include "StdAfx.h"
#include "CgxLoginForm.h"
#include <afxsock.h>


CCgxLoginForm::CCgxLoginForm(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCgxLoginForm::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCgxLoginForm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCgxLoginForm, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCgxLoginForm::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CCgxLoginForm::OnBnClickedCancel)
END_MESSAGE_MAP()



// CCgxDlg 消息处理程序

BOOL CCgxLoginForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	AfxSocketInit();


	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCgxLoginForm::OnPaint()
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
HCURSOR CCgxLoginForm::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

extern BOOLEAN getActiveMac( char* sMac );

void CCgxLoginForm::OnBnClickedOk()
{
	const int BUFF_LEN = 1024;
	CSocket socket;
	TCHAR buff[BUFF_LEN] = {0};
	char mac[40] = {0};
	TCHAR uMac[40] = {0};
	TCHAR username[MAX_PATH] = {0};
	TCHAR password[MAX_PATH] = {0};
	CEdit *usernameEdit = (CEdit *)GetDlgItem(IDC_USERNAME);
	CEdit *passwordEdit = (CEdit *)GetDlgItem(IDC_PASSWORD);
	usernameEdit->GetWindowText(username, sizeof(TCHAR)*MAX_PATH);
	passwordEdit->GetWindowText(password, sizeof(TCHAR)*MAX_PATH);
	getActiveMac(mac);
	int len = MultiByteToWideChar(CP_ACP, 0, mac, strlen(mac), NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, mac, strlen(mac), uMac, len);
	uMac[len] = '\0';
	swprintf(buff, 
		TEXT("<auth><username>%s</username><password>%s</password><mac>%s</mac></auth>")
		, username, password, uMac);
	
	
	if(!socket.Create())
	{
		ExitProcess(-2);
	}
#ifdef DEBUG
	TRACE(TEXT("%s\n"), buff);
	if(socket.Connect(TEXT("10.0.2.2"), 8002))
#else
	if(socket.Connect(TEXT("php.ii2d.com"), 8002))
#endif
	{
		socket.Send(buff, sizeof(TCHAR)*wcslen(buff));
		memset(buff, 0, sizeof(TCHAR)*BUFF_LEN);
		socket.Receive(buff, BUFF_LEN);
		socket.Close();
		//OutputDebugString(buff);
		if(strcmp((char*)buff, "succ") == 0)
			CDialogEx::OnOK();
		else
			ExitProcess(-1);
	}
	else
	{
		ExitProcess(-3);
	}
	
}


void CCgxLoginForm::OnBnClickedCancel()
{
	ExitProcess(0);
}
