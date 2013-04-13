#pragma once
#include "cgx.h"
#include "HWNDScreen.h"
#include "CgxSkillWindow.h"
#include "CgxGoodsWindow.h"
#include "CgxPetCommandWindow.h"
#include "CgxTopRightWindow.h"

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
	CCgxSkillWindow *skillWindow;
	CCgxGoodsWindow *goodsWindow;
	CCgxTopRightWindow* topRightWindow;
	CCgxPetCommandWindow* petCommandWindow;
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
	afx_msg void OnBnClickedLocateGoods();
	afx_msg void OnBnClickedLocateMonster();
	afx_msg void OnBnClickedLocateTopRightWindow();
protected:
	inline void _locateWindowInfo(CCgxWindow*, PTSTR);

	inline void _initWindows(void);
public:
	afx_msg void OnBnClickedLocatePetCammand();
	afx_msg void OnBnClickedLocatePlayerCommand();
};
