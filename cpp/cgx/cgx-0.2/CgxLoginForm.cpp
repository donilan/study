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



// CCgxDlg ��Ϣ�������

BOOL CCgxLoginForm::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	AfxSocketInit();


	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_NORMAL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCgxLoginForm::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
