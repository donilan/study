#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpcmdLine, int nCmdShow)
{
  MessageBox(NULL, TEXT("Hello World!"), TEXT("First Program"), MB_OK|MB_OKCANCEL);
  MessageBox(NULL, lpcmdLine, TEXT("CMD line"), MB_OK|MB_ERROR);
  return 0;
}
