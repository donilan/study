#define UNICODE

#include <windows.h>
#include <stdio.h>

int main(void)
{
  printf("TCHAR size: %d\n", sizeof(TCHAR));

#ifdef _UNICODE
  printf("define _UNICODE\n");
#endif
#ifdef UNICODE
  printf("define UNICODE");
#endif

  return 0;
}
