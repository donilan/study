#include <stdio.h>


extern "C" {
void hello(void);
int main(void)
{

  hello();
  printf("Hey Hello.");
  return 0;
}
}