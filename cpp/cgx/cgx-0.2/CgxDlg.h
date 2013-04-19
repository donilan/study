
// CgxDlg.h : ͷ�ļ�
//

#pragma once
#include "SystemTestDlg.h"
#include "GameManager.h"
#define HOTKEY_F2 2
#define HOTKEY_F3 3
#define HOTKEY_F4 4
#define HOTKEY_F5 5
#define MAX_GAME 10


// CCgxDlg �Ի���
class CCgxDlg : public CDialogEx
{
// ����
public:
	CCgxDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CGX02_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CSystemTestDlg systemTestDlg;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
private:
	BOOL isTestDlgOpen;
	CGameManager gameManager;
public:
	afx_msg void OnBnClickedRefresh();
	
	afx_msg void OnBnClickedAutoFight();
	afx_msg void OnBnClickedStart();
	virtual void OnOK();
	afx_msg void OnCbnSelchangeScriptList();
	afx_msg void OnCbnSetfocusScriptList();
protected:
	void _initConfigFile(void);
};
