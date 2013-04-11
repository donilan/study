#pragma once
#include "cgx.h"
#include "HWNDScreen.h"

// CSystemTestDlg �Ի���

class CSystemTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemTestDlg)

public:
	CSystemTestDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemTestDlg();

// �Ի�������
	enum { IDD = IDD_SYSTEM_TEST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	CHWNDScreen *pScreen;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLockScreen();
	afx_msg void OnBnClickedLeftClick();
	afx_msg void OnBnClickedScreenshot();
	afx_msg void OnBnClickedRgbRate();
	afx_msg void OnBnClickedDrawScreen();
};
