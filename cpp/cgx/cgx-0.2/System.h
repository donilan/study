#pragma once


class CSystem
{
public:
	CSystem(void);
	~CSystem(void);

	static UINT rightClickThread(LPVOID);
	static BOOL lockScreen(BOOL);
	static BOOL leftClick(int x, int y);
	static UINT LeftClickThread(LPVOID);
	static void leftClick(const RECT* rect);
	static int ansi2unicode(char* in, size_t inSize, TCHAR* out, size_t outSize);
	static void rightClick(int x, int y);
	static void leftPress(int x, int y);
	static void sendKey(int key);
	static void sendKeyWithCtrl(int key);
	static void resetCounter();
	static int getLeftClickCounter();
	static int getRightClickCounter();
	static int getSendKeyCounter();
	static BOOL SafeTerminateProcess(HANDLE hProcess, UINT uExitCode);
	static BOOL CALLBACK enumChildProc(HWND hwnd, LPARAM lParam);
};

