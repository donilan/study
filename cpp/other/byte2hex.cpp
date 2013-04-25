#include <stdio.h>

int main(void)
{
  char arr[5] = {0, 16, 25, 255, 255};
  char buff[64] = {0};
  char *pBuff = buff;
  for( int i = 0 ; i < 5; ++i)
    {
      pBuff +=  sprintf(pBuff, "%X", arr[i]);
    }
  printf("%s\n", buff);
  return 0;
}
