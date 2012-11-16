#include <stdio.h> 

int main(void) 
{ 
  const short int c1 = 49920;
  const int c2 = 1073742008;

  int (*pf)() = (int (*)())&c2;
  printf("%c%c\n", *(char *)pf()-19, *((char*)pf()+1)-49);
  return 0;
}

