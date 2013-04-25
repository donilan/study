#include <stdio.h>

int main() {
  wchar_t pWchar[] = L"unicode";
  printf("unsigned short length: %d\n", sizeof(unsigned short));
  wprintf(L"[%s] length: %d\n", pWchar, wcslen(pWchar));
  printf("char length: %d\n", strlen("unicode"));
  return 0;
}
