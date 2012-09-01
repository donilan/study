#include <unistd.h>
#include <stdio.h>

int main(void) {
  long result = 0L;
  result = sysconf(_SC_ARG_MAX);
  printf("sc arg max is %d\n", result);

  result = sysconf(_SC_CLK_TCK);
  printf("sc clk tck is %d\n", result);
  return 0;
}
