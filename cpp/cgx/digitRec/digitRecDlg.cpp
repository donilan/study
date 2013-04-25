
// digitRecDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "digitRec.h"
#include "digitRecDlg.h"
#include "afxdialogex.h"
#include "FindNum.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CdigitRecDlg 对话框




CdigitRecDlg::CdigitRecDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdigitRecDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdigitRecDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdigitRecDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CdigitRecDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CdigitRecDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CdigitRecDlg 消息处理程序

BOOL CdigitRecDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdigitRecDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdigitRecDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CdigitRecDlg::OnBnClickedButton1()
{
	CImage img;
	img.Create(80, 16, 24);
	
	CImageDC dc(img);
	TCHAR* buff = TEXT("1234567890");
	
	TextOut(dc,0, 0, buff, wcslen(buff)*sizeof(TCHAR));
	img.Save(TEXT("tmp.bmp"));
}

#define NEQ_COLOR(rgb1, rgb2) \
	!(abs(GetRValue(rgb1) - GetRValue(rgb2)) < 5 \
	&& abs(GetGValue(rgb1) - GetGValue(rgb2)) < 5 \
	&& abs(GetBValue(rgb1) - GetBValue(rgb2)) < 5 )

void printColorCounts(CImage* pImg)
{
	POSITION pos;
	COLORREF rgb;
	int value, defaultVal = 1;
	CMap<COLORREF, COLORREF&, int, int&> map;
	TRACE("interate color width: %d, height: %d\n", pImg->GetWidth(), pImg->GetHeight());
	for(int x = 0; x < pImg->GetWidth(); ++x)
	{
		for(int y = 0; y < pImg->GetHeight(); ++y)
		{
			rgb = pImg->GetPixel(x, y);
			if(map.Lookup(rgb, value))
			{
				map.SetAt(rgb, ++value);
			}
			else
			{
				map.SetAt(rgb, defaultVal);
			}
		}
	}
	TRACE("Get start position\n");
	pos = map.GetStartPosition();
	while(pos != NULL)
	{
		map.GetNextAssoc(pos, rgb, value);
		TRACE("sizeof COLOREF: %d, (%d,%d,%d), %d, %X\n", sizeof(COLORREF), 
			GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), value, RGB(253,254,255));
	}
}

void printNumber(int minX, int maxX, int minY, int maxY, COLORREF* colors, COLORREF rgb)
{
	int result = -1;
	int width = maxX - minX;
	int height = maxY - minY;
//	COLORREF* tmpRgb;
	if(height - width * 2 > 1) {
		result = 1;
	}
	for(int x = 0; x < maxX; ++x)
	{
		if(NEQ_COLOR(colors[x], rgb))
		{
			break;
		} 
		else if(x == maxX-1)
		{
			result = 7;
		}

	}

	TRACE("number: %d\n", result);
}

void analysis(CImage* pImg)
{
	COLORREF fstColor = pImg->GetPixel(0, 0);
	COLORREF rgb;
	COLORREF tmpRGB;
	bool frtFound = TRUE;
	bool found = FALSE;
	int minX = 0;
	int minY = 0;
	int maxX = 0;
	int maxY = 0;
	int height = 0;
	int width = 0;
	int result = 0;
	int num = 0;
	int topLength = 0;
	int leftLength = 0;
	int bottomLength = 0;
	int rightLength = 0;
	bool leftBreak = FALSE;
	bool rightBreak = FALSE;
	for(int x = 0; x < pImg->GetWidth(); ++x)
	{
		for(int y = 0; y < pImg->GetHeight(); ++y)
		{
			rgb = pImg->GetPixel(x, y);
			if(NEQ_COLOR(fstColor, rgb))
			{
				tmpRGB = rgb;
				if(frtFound)
				{
					frtFound = FALSE;
					minX = x;
					minY = y;
				}
				found = TRUE;
				if(minX > x) minX = x;
				if(maxX < x) maxX = x;
				if(minY > y) minY = y;
				if(maxY < y) maxY = y;

			}
			else
			{
				if(y == pImg->GetHeight()-1 
					&& found == FALSE
					&& maxX != x)
				{
					width = maxX - minX;
					height = maxY - minY;
					topLength = rightLength = bottomLength = leftLength = 0;
					leftBreak = rightBreak = FALSE;
					if(frtFound)
						continue;
					//TRACE("(%d,%d,%d) minX; %d, minY: %d, maxX: %d, maxY: %d\n",
					//	GetRValue(tmpRGB), GetGValue(tmpRGB), GetBValue(tmpRGB), minX, minY, maxX, maxY);
					
					
					for(int x2 = minX; x2 < maxX; ++x2)
					{
						rgb = pImg->GetPixel(x2, minY);
						if(NEQ_COLOR(fstColor, rgb))
							++topLength;
						rgb = pImg->GetPixel(x2, maxY);
						if(NEQ_COLOR(fstColor, rgb))
							++bottomLength;
					}
					for(int y2 = minY; y2 < maxY; ++y2)
					{
						rgb = pImg->GetPixel(minX, y2);
						if(minX == 65)
						{
							//TRACE("(%d,%d,%d)\n", GetRValue(rgb), GetGValue(rgb), GetBValue(rgb));
						}
						if(NEQ_COLOR(fstColor, rgb))
							++leftLength;
						else
							leftBreak = TRUE;
						rgb = pImg->GetPixel(maxX, y2);
						if(NEQ_COLOR(fstColor, rgb))
							++rightLength;
						else if(y2 > minX && y2 < maxX && abs(y2 - height/2) < 4)
							rightBreak = TRUE;
					}
					///TRACE("Head length: %d\n", topLength);
					
					if(height - width * 2 > 1) {
						num = 1;
					}
					else if(topLength < width / 2)
					{
						num = 4;
					}
					else if(topLength > width /2 && topLength < width)
					{
						if(leftLength * 100 / height > 50 && leftLength * 100 / height > 50)
						{	
							//TRACE("(%d,%d) left length: %d, right length: %d\n",minX, minY, leftLength, rightLength);
						}
						else if(leftLength * 100 / height < 30 && rightLength * 100 / height > 50)
							num = 3;
						else
							num = 2;
					}
					else if(topLength == width)
					{
						if(leftLength < height / 2)
							num = 7;
						else
							num = 5;

					} else 
					{
						num = -1;
					}
					if(num > -1)
						result = result * 10 + num;
					num = -1;
					frtFound = TRUE;
					//跳到下一列处理
					break;
				}
				found = FALSE;
			}
		}
	}
	TRACE("RESULT: %d\n", result);
}



#define IMG_NUM 11
void CdigitRecDlg::OnBnClickedButton2()
{
	TCHAR buff[MAX_PATH] = {0};
	int result;
	CImage *image = NULL;
	CFindNum *pFindNum = NULL;
	RECT rect;
	for(int i = 0 ; i < IMG_NUM; ++i)
	{
		if(image) delete image;
		image = new CImage();
		wsprintf(buff, TEXT("digit%d.bmp"), i);
		TRACE(TEXT("========%s=======\n"), buff);
		image->Load(buff);
		if(pFindNum) delete pFindNum;
		pFindNum = new CFindNum(image);
		TRACE("Width: %d, height: %d\n", image->GetWidth(), image->GetHeight());
		TRACE("%d\n", pFindNum->allNum());
		TRACE("=================END==================\n\n");

	}
	if(pFindNum) delete pFindNum;
	if(image) delete image;
}
