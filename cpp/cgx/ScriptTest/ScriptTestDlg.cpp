
// ScriptTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScriptTest.h"
#include "ScriptTestDlg.h"
#include "afxdialogex.h"
#include "Script.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScriptTestDlg �Ի���




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


// CScriptTestDlg ��Ϣ�������

BOOL CScriptTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScriptTestDlg::OnPaint()
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
HCURSOR CScriptTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScriptTestDlg::OnBnClickedButton1()
{
	CFile file;
	file.Open(TEXT("..\\cgx-0.2\\script\\����.txt"), CFile::modeRead);
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
	GetPrivateProfileString(TEXT("����"), TEXT("1������"),
		TEXT("default"), buff, size, TEXT("config/config.ini") );
	TRACE(buff);
	TRACE("\n");
}


void CScriptTestDlg::OnBnClickedButton3()
{
	WritePrivateProfileString(TEXT("scriptTest"), TEXT("test"),
		TEXT("testValue"), TEXT("config/config.ini"));
	WritePrivateProfileString(TEXT("����"), TEXT("1������"),
		TEXT("testValue"), TEXT("config/config.ini"));
}
