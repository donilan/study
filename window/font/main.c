#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpcmdLine, int nCmdShow)
{
  static TCHAR szAppName[] = TEXT("SelectFont");
  static TCHAR szClassName[] = TEXT("SelectFontClass");
  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;

  wndclass.style = CS_HREDRAW| CS_VREDRAW;
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
      MessageBox(NULL, TEXT("This program requires Windows NT!"),
                 szAppName, MB_ICONERROR);
      return 0;
    }
  hwnd = CreateWindow(szClassName,
                      TEXT("Select Different Fonts Program"),
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
  PAINTSTRUCT ps;
  HFONT hCurrentFont, hOldFont;
  static TCHAR guiFont[] = TEXT("DEFAULT_GUI_FONT");
  static TCHAR ansiFont[] = TEXT("ANSI_FIXED_FONT");
  static TCHAR ansiVarFont[] = TEXT("ANSI_VAR_FONT");
  static TCHAR deviceFont[] = TEXT("DEVICE_DEFAULT_FONT");

  switch(message)
    {
    case WM_CREATE:
      return 0;
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);

      hCurrentFont = GetStockObject(DEFAULT_GUI_FONT);
      hOldFont = SelectObject(hdc, hCurrentFont);
      TextOut(hdc, 0, 0, guiFont,
              sizeof(guiFont)-1);

      hCurrentFont = GetStockObject(ANSI_FIXED_FONT);
      SelectObject(hdc, hCurrentFont);
      TextOut(hdc, 0, 20, ansiFont, sizeof(ansiFont)-1);

      hCurrentFont = GetStockObject(ANSI_VAR_FONT);
      SelectObject(hdc, hCurrentFont);
      TextOut(hdc, 0, 40, ansiVarFont, sizeof(ansiVarFont)-1);

      hCurrentFont = GetStockObject(DEVICE_DEFAULT_FONT);
      SelectObject(hdc, hCurrentFont);
      TextOut(hdc, 0, 60, deviceFont, sizeof(deviceFont));

      hCurrentFont = GetStockObject(OEM_FIXED_FONT);
      SelectObject(hdc, hCurrentFont);
      TextOut(hdc, 0, 80, TEXT("OEM_FIXED_FONT"), 
              sizeof(TEXT("OEM_FIXED_FONT")));

      hCurrentFont = GetStockObject(SYSTEM_FONT);
      SelectObject(hdc, hCurrentFont);
      TextOut(hdc, 0, 100, TEXT("SYSTEM_FONT"),
              sizeof(TEXT("SYSTEM_FONT"))-1);

      SelectObject(hdc, hOldFont);
      EndPaint(hwnd, &ps);
      return 0;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    }
  return DefWindowProc(hwnd, message, wParam, lParam);
}
