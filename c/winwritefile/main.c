#include <windows.h>
#include <stdio.h>

int main(void)
{
  char str[] = "Hello world!\n";
  long result = 0L;
  long *stdOut;
  stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  int re = WriteFile(stdOut, &str, sizeof(str), &result, NULL);
  printf("result: %d, result2: %d\n", result, re);
  return 0;
}
