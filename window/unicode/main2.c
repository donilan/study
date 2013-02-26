#define UNICODE
#define HELLO(x) L##x

#include <windows.h>
#include <stdio.h>

int main(void)
{
  WCHAR *pWchar = TEXT("Hello");
  return 0;
}
