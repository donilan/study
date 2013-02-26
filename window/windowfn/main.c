#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int iCmdShow)
{
  static TCHAR szAppName[] =TEXT("HelloWin");
  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;


  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(hInstance, TEXT("hand.cur"));
  wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = szAppName;

  if(!RegisterClass(&wndclass))
    {
      MessageBox(NULL, TEXT("This program requires Windows NT!")
                 , szAppName, MB_ICONERROR);
      return 0;
      
    }

  hwnd = CreateWindow(szAppName, 
                      TEXT("Hello world"),
                      WS_OVERLAPPEDWINDOW,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      CW_USEDEFAULT,
                      NULL, NULL, hInstance, NULL);

  printf("%d\n", iCmdShow);

  ShowWindow(hwnd, iCmdShow);
  /* UpdateWindow(hwnd); */

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
  PAINTSTRUCT ps;
  RECT rect;

  switch(message)
    {
    case WM_CREATE:
      MessageBox(NULL, TEXT("Creating... ..."), TEXT("Hello win message"), MB_OK);
      return 0;
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      GetClientRect(hwnd, &rect);
      DrawText(hdc, TEXT("Hello, windows 99!")
               , -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
      EndPaint(hwnd, &ps);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }
  return DefWindowProc(hwnd, message, wParam, lParam);
}
    
