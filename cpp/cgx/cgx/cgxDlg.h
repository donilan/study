
// cgxDlg.h : ͷ�ļ�
//

#pragma once

#include "GameManager.h"

// CcgxDlg �Ի���
class CcgxDlg : public CDialogEx
{
// ����
public:
	CcgxDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CGX_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	CGameManager gameManager;
public:
	afx_msg void OnBnClickedButton1();

	LRESULT OnHotkey(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedTime2screenshot();
	afx_msg void OnBnClickedSpeedUpWalk();
};
