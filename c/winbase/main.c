#include <stdio.h>
#include <windows.h>
#include <Winbase.h>
 
int main(void) {
  HMODULE h = GetModuleHandle(NULL);
  printf("h module %d\n", h);
  return 0;
}

