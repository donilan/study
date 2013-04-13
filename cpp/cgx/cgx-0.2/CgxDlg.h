
// CgxDlg.h : 头文件
//

#pragma once
#include "SystemTestDlg.h"
#define HOTKEY_F2 2


// CCgxDlg 对话框
class CCgxDlg : public CDialogEx
{
// 构造
public:
	CCgxDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CGX02_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CSystemTestDlg systemTestDlg;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
private:
	BOOL isTestDlgOpen;
};
