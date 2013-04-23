#include <windows.h>
#include <stdio.h>
#include <string.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpcmdLine, int nCmdShow)
{
  static TCHAR szAppName[] = TEXT("DifferentDC");
  static TCHAR szClassName[] = TEXT("DifferentDCClass");
  HWND hwnd;
  MSG msg;
  WNDCLASS wc;
  
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wc.lpszMenuName = NULL;
  wc.lpszClassName = szClassName;

  if(!RegisterClass(&wc))
    {
      MessageBox(NULL, TEXT("This program requires Windows NT!"),
                 szAppName, MB_ICONERROR);
      return 0;
    }

  hwnd = CreateWindow( szClassName,
                       TEXT("Show Different DCs"),
                       WS_OVERLAPPEDWINDOW,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       CW_USEDEFAULT,
                       NULL,
                       NULL,
                       hInstance,
                       NULL);

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  while(GetMessage(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  HDC hdc;
  TCHAR TextInClientRect[] = TEXT("用户区左上角!");
  TCHAR TextInWindow[] = TEXT("窗口左上角!");
  TCHAR TextInScreen[] = TEXT("屏幕左上角");
  switch(message)
    {
    case WM_CREATE:
      return 0;
    case WM_PAINT:
      hdc = GetDC(hwnd);
      TextOut(hdc, 0, 0, TextInClientRect, strlen(TextInClientRect));
      ValidateRect(hwnd, NULL);
      ReleaseDC(hwnd, hdc);
      
      hdc = GetWindowDC(hwnd);
      TextOut(hdc, 0, 0, TextInWindow, strlen(TextInWindow));
      ReleaseDC(hwnd, hdc);

      hdc = CreateDC("Display", NULL, NULL, NULL);
      TextOut(hdc, 0, 0, TextInScreen, strlen(TextInScreen));
      DeleteDC(hdc);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }
  return DefWindowProc(hwnd, message, wParam, lParam);
}

