#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,
                   LPSTR lpcmdLine, int nCmdShow) 

{
  static TCHAR szAppName[] = TEXT("SimpleDisplay");
  static TCHAR szClassName[] = TEXT("SimDisplayClass");

  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;

  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = szClassName;

  if(!RegisterClass(&wndclass)) 
    {
      MessageBox(NULL, TEXT("This program requies Windows NT!"),
                 szAppName, MB_ICONERROR);
      return 0;
    }
  hwnd = CreateWindow(szClassName,
                      TEXT("Source Text Simple Display Program"),
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
  static int cxChar, cyChar;
  int cLineNumber;
  TCHAR szBuff[256];
  HDC hdc;
  PAINTSTRUCT ps;
  TEXTMETRIC tm;
  FILE *fp;

  switch(message)
    {
    case WM_CREATE:
      hdc = GetDC(hwnd);
      GetTextMetrics(hdc, &tm);
      cxChar = tm.tmAveCharWidth;
      cyChar = tm.tmHeight+tm.tmExternalLeading;

      ReleaseDC(hwnd, hdc);
      return 0;
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      cLineNumber = 0;
      if(( fp = fopen("main2.c", "r")) != NULL)
        {
          while(!feof(fp))
            {
              int i = 0;
              char ch;
              while((ch = fgetc(fp)) != '\n' && ch != EOF)
                {
                  if(ch != '\t')
                    {
                      szBuff[i++] = ch;
                    }
                }
              TextOut(hdc, cxChar, cyChar* cLineNumber, szBuff, i);
              cLineNumber++;
            }
          fclose(fp);
        }
      EndPaint(hwnd, &ps);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }
  return DefWindowProc(hwnd, message, wParam, lParam);
}


