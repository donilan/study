
// CgxDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "cgx.h"
#include "CgxDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCgxDlg �Ի���




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


// CCgxDlg ��Ϣ�������

BOOL CCgxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	RegisterHotKey(this->m_hWnd, HOTKEY_F2, 0, VK_F2);
	isTestDlgOpen = FALSE;


	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_NORMAL);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	OnBnClickedRefresh();

	TCHAR buff[MAX_PATH] = {0};
	CComboBox* single = (CComboBox*)GetDlgItem(IDC_SINGLE_SKILL_LIST);
	CComboBox* four = (CComboBox*)GetDlgItem(IDC_FOUR_SKILL_LIST);
	CComboBox* all = (CComboBox*)GetDlgItem(IDC_ALL_SKILL_LIST);
	CComboBox* singleLv = (CComboBox*)GetDlgItem(IDC_SINGLE_SKILL_LIST_LV);
	CComboBox* fourLv = (CComboBox*)GetDlgItem(IDC_FOUR_SKILL_LIST_LV);
	CComboBox* allLv = (CComboBox*)GetDlgItem(IDC_ALL_SKILL_LIST_LV);
	CComboBox* pet = (CComboBox*)GetDlgItem(IDC_PET_SKILL);

	single->AddString(TEXT("����"));
	four->AddString(TEXT("����"));
	all->AddString(TEXT("����"));
	for(int i = 0 ; i < 10; ++i)
	{
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("����%02d"), i+1);
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCgxDlg::OnPaint()
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
		swprintf(buff, sizeof(TCHAR)*MAX_PATH, TEXT("��Ϸ%d"), i);
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
