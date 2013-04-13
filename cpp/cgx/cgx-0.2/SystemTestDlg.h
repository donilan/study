#pragma once
#include "cgx.h"
#include "HWNDScreen.h"
#include "CgxSkillWindow.h"

// CSystemTestDlg 对话框

class CSystemTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSystemTestDlg)

public:
	CSystemTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemTestDlg();

// 对话框数据
	enum { IDD = IDD_SYSTEM_TEST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	CHWNDScreen *pScreen;
	CCgxSkillWindow *skillWindow;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLockScreen();
	afx_msg void OnBnClickedLeftClick();
	afx_msg void OnBnClickedScreenshot();
	afx_msg void OnBnClickedRgbRate();
	afx_msg void OnBnClickedDrawScreen();
	afx_msg void OnBnClickedRefreshImageList();
	afx_msg void OnLbnDblclkImageList();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	void _out(PTSTR msg);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnBnClickedSaveRect();
	afx_msg void OnBnClickedMatch();
	afx_msg void OnBnClickedLocate();
	afx_msg void OnBnClickedRefreshMatchList();
	afx_msg void OnLbnDblclkMatchList();
	afx_msg void OnBnClickedLocateJobWindow();
	afx_msg void OnBnClickedLocateSkill();
	void _initScreen(PTSTR pszFilePath);
	void _initScreen(HWND);
};
