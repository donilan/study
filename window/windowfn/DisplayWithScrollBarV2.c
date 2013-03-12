#include <windows.h>
#include <stdio.h>
#define MaxBuff 256

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
                      WS_OVERLAPPEDWINDOW|WS_VSCROLL|WS_HSCROLL,
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
  static int cxClient, cyClient;
  static int nHScrollPos, nVScrollPos;
  static int cMaxCharNumber;
  static int cLineNumber;
  int nFirstLine, nLastLine;
  int nCurrentCharNumber;
  int nCurrentLineNumber;
  TCHAR szBuff[MaxBuff];
  HDC hdc;
  PAINTSTRUCT ps;
  TEXTMETRIC tm;
  SCROLLINFO si;
  FILE *fp;
  

  switch (message)
    {
    case WM_CREATE:
      hdc = GetDC(hwnd);
      GetTextMetrics(hdc, &tm);
      cxChar = tm.tmAveCharWidth;
      cyChar = tm.tmHeight + tm.tmExternalLeading;
      ReleaseDC(hwnd, hdc);

      nCurrentCharNumber = 0;
      
      cMaxCharNumber =0;
      cLineNumber =0;
      if (( fp=fopen("DisplayWithScrollBarV2.c", "r" )) != NULL)
        {
          while(!feof(fp))
            {
              char ch;
              while(( ch = fgetc(fp))!= '\n' && ch != EOF)
                nCurrentCharNumber ++;
              if(nCurrentCharNumber > cMaxCharNumber)
                cMaxCharNumber = nCurrentCharNumber;
              cLineNumber++;
              nCurrentCharNumber = 0;
            }
          fclose(fp);
        } // end if
      return 0;
      
    case WM_SIZE:
      cxClient = LOWORD(lParam);
      cyClient = HIWORD(lParam);
      
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_RANGE | SIF_PAGE;
      si.nMin = 0;
      si.nMax = cLineNumber -1;
      si.nPage = cyClient / cyChar;
      SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_RANGE | SIF_PAGE;
      si.nMin = 0;
      si.nMax = cMaxCharNumber -1;
      si.nPage = cxClient / cxChar;
      SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
      
      return 0;

    case WM_HSCROLL:
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_ALL;
      GetScrollInfo(hwnd, SB_HORZ, &si);
      nHScrollPos = si.nPos;
      
      switch(LOWORD(wParam))
        {
        case SB_LINELEFT:
          si.nPos -= 1;
          break;
        case SB_LINERIGHT:
          si.nPos += 1;
        case SB_PAGELEFT:
          si.nPos -= si.nPage;
          break;
        case SB_PAGERIGHT:
          si.nPos += si.nPage;
          break;
        case SB_THUMBPOSITION:
          si.nPos = si.nTrackPos;
          break;
        case SB_LEFT:
          si.nPos = si.nMin;
          break;
        case SB_RIGHT:
          si.nPos = si.nMax;
          break;
        default:
          break;
        }// end switch
      si.fMask = SIF_POS;
      SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
      GetScrollInfo(hwnd, SB_HORZ, &si);
      
      if(si.nPos != nHScrollPos)
        ScrollWindow(hwnd, cxChar*(nHScrollPos-si.nPos), 0, NULL, NULL);
      return 0;

    case WM_VSCROLL:
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_ALL;
      GetScrollInfo(hwnd, SB_VERT, &si);
      nVScrollPos = si.nPos;

      switch(LOWORD(wParam))
        {
        case SB_LINEUP:
          si.nPos -= 1;
          break;
        case SB_LINEDOWN:
          si.nPos += 1;
          break;
        case SB_PAGEUP:
          si.nPos -= si.nPage;
          break;
        case SB_PAGEDOWN:
          si.nPos += si.nPage;
          break;
        case SB_THUMBTRACK:
          si.nPos = si.nTrackPos;
          break;
        case SB_TOP:
          si.nPos = si.nMin;
          break;
        case SB_BOTTOM:
          si.nPos = si.nMax;
          break;
        default:
          break;
        }
      si.fMask = SIF_POS;
      SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
      GetScrollInfo(hwnd, SB_VERT, &si);
      
      if(si.nPos != nVScrollPos)
        ScrollWindow(hwnd, 0, cyChar*(nVScrollPos-si.nPos), NULL, NULL);
      return 0;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      si.cbSize = sizeof(SCROLLINFO);
      si.fMask = SIF_POS;
      GetScrollInfo(hwnd, SB_HORZ, &si);
      nHScrollPos = si.nPos;
      GetScrollInfo(hwnd, SB_VERT, &si);
      nVScrollPos = si.nPos;
      
      nFirstLine = max(0, nVScrollPos + ps.rcPaint.top /cyChar);
      nLastLine  = min(cLineNumber -1, nVScrollPos + ps.rcPaint.bottom / cyChar);
      if((fp=fopen("DisplayWithScrollBarV2.c", "r"))!= NULL)
        {
          for(nCurrentLineNumber=0; nCurrentLineNumber<=nLastLine; nCurrentLineNumber++)
            {
              int j = 0, k;
              char ch;
              if(nCurrentLineNumber < nFirstLine)
                while((ch = fgetc(fp))!='\n' && ch != EOF)
                  ;
              else
                while((ch=fgetc(fp))!='\n' && ch!=EOF)
                  if(ch == '\t')
                    for(k=0;k<4;k++)
                      szBuff[j++] = ' ';
                  else
                    szBuff[j++] = ch;
              TextOut(hdc, cxChar*(-nHScrollPos), cyChar*(nCurrentLineNumber-nVScrollPos), szBuff, j);
            }
        }//end if
      EndPaint(hwnd, &ps);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    } // end switch
  return DefWindowProc(hwnd, message, wParam, lParam);
}









