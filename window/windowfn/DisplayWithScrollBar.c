#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,
                   LPSTR lpcmdLine, int nCmdShow) 

{
  static TCHAR szAppName[] = TEXT("DisplayWithScrollBar");
  static TCHAR szClassName[] = TEXT("DisplayWithBarClass");

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
                      WS_OVERLAPPEDWINDOW|WS_VSCROLL,
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
  static int cyClient;
  static int nVScrollPos;
  static int cLineNumber;
  int nCurrentLineNumber;
  TCHAR szBuff[256];
  HDC hdc;
  PAINTSTRUCT ps;
  TEXTMETRIC tm;
  FILE *fp;

  switch (message) 
    {
    case WM_CREATE:
      hdc = GetDC(hwnd);
      GetTextMetrics(hdc, &tm);
      cxChar = tm.tmAveCharWidth;
      cyChar = tm.tmHeight + tm.tmExternalLeading;
      ReleaseDC(hwnd, hdc);
      cLineNumber = 0;
      if (( fp = fopen("DisplayWithScrollBar.c", "r")) != NULL)
        {
          while(!feof(fp))
            {
              char ch;
              while (( ch = fgetc(fp)) != '\n' && ch != EOF)
                ;
              cLineNumber ++;
            }
          fclose(fp);
        }
      SetScrollRange(hwnd, SB_VERT, 0, cLineNumber-1, FALSE);
      SetScrollPos(hwnd, SB_VERT, 0, TRUE);
      return 0;
      
    case WM_SIZE:
      cyClient = HIWORD(lParam);
      return 0;
    case WM_VSCROLL:
      switch(LOWORD(wParam))
        {
        case SB_LINEUP:
          nVScrollPos -= 1;
          break;
        case SB_LINEDOWN:
          nVScrollPos += 1;
          break;
        case SB_PAGEUP:
          nVScrollPos -= cyClient / cyChar;
          break;
        case SB_PAGEDOWN:
          nVScrollPos += cyClient / cyChar;
          break;
        case SB_THUMBPOSITION:
          nVScrollPos = HIWORD(wParam);
          break;
        default :
          break;
        }
      nVScrollPos = min(cLineNumber-1, max(nVScrollPos, 0));
      if (nVScrollPos != GetScrollPos(hwnd, SB_VERT))
        {
          SetScrollPos(hwnd, SB_VERT, nVScrollPos, TRUE);
          InvalidateRect(hwnd, NULL, TRUE);
        }
      return 0;
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      if(( fp = fopen("DisplayWithScrollBar.c", "r"))  != NULL)
        {
          for(nCurrentLineNumber = 0; nCurrentLineNumber < cLineNumber; nCurrentLineNumber++)
            {
              int j = 0, k;
              char ch;
              while((ch = fgetc(fp)) != '\n' && ch != EOF)
                {
                  if(ch == '\t')
                    {
                      for(k = 0; k < 4; ++k) {
                        szBuff[j++] = ' ';
                      }
                    }
                  else
                    {
                      szBuff[j++] = ch;
                    }
                }
              TextOut(hdc, cxChar, cyChar*(nCurrentLineNumber-nVScrollPos), szBuff, j);
            }
        }
      EndPaint(hwnd, &ps);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }
  return DefWindowProc(hwnd, message, wParam, lParam);

}
