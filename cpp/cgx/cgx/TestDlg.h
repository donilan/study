#pragma once

#include "Game.h"
#include "GameManager.h"


// CTestDlg 对话框

class CTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDlg)

public:
	CTestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestDlg();

// 对话框数据
	enum { IDD = IDD_TEST_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	CGameManager gameManager;
public:
	afx_msg void OnBnClickedRefreshGame();
	afx_msg void OnBnClickedFindMonster();
	afx_msg void OnBnClickedMonsterFight();
	afx_msg void OnBnClickedMakeScreenshot();
	afx_msg void OnBnClickedPlayerFight();
	afx_msg void OnBnClickedCheckStatus();
	afx_msg void OnBnClickedHitHitBtn();
	afx_msg void OnBnClickedHitSkillBtn();
	afx_msg void OnBnClickedFindSkillWindow();
	afx_msg void OnBnClickedRefreshScreenshotList();
	afx_msg void OnBnClickedLoadImage();
};
