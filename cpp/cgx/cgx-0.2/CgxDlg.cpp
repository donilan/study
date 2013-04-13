
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
