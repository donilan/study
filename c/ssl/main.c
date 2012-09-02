#include <stdio.h>

int main(void)
{
#if defined HAVE_LIBSSL
  printf("defined.\n");
#endif
  return 0;
}
