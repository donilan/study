
// CgxDlg.h : ͷ�ļ�
//

#pragma once
#include "SystemTestDlg.h"
#define HOTKEY_F2 2


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
};
