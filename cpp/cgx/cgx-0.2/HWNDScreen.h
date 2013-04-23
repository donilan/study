#pragma once

class CHWNDScreen
{
public:

	RECT rect;
	CImage* pImage;

	CHWNDScreen(HWND);
	~CHWNDScreen(void);

	BOOL screenshot(PTSTR);
	INT colorDeviation(RECT*, COLORREF);
	static void flashRECT(const RECT*);

	void loadImage(PTSTR pszImagePath);
	// Locate image
	BOOL locate(const CImage* pImage, RECT* rect, const RECT* condition = NULL);
	// Is image match in location
	BOOL match(const CImage* pImage, const RECT* rect);
	BOOL saveRECT(const RECT* rect, const PTSTR pszImagePath);
	// Load image file in to CImage class
	static BOOL loadInToCImage(const PTSTR pszImagePath, CImage* pImage);
	// Print into screen
	void print(void);

	CMutex mutex;
	HWND hwnd;
private:
	static UINT screenPrintThread(LPVOID lpVoid);
	static UINT flashRECTThread(LPVOID);
	
	CImageDC* pImageDC;
	HDC hScreenDC;
public:
	void refresh(void);
	static UINT autoRefreshThread(LPVOID lpVoid);
	void startAutoRefresh(void);
	void stopAutoRefresh(void);
	BOOL isAutoRefresh;
	int toNumber(const RECT* pRECT);
	void toCImage(const RECT* rectIn, CImage* pImageOut);
	BOOL isFocus(void);
	BOOL locate(const int resourceId, RECT* rectOut, const RECT* condition);
};

