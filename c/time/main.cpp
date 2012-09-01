#include <time.h>
#include <stdio.h>


int main(void) {
  char buff[50] = {0};
  char outBuff[256] = {0};
  time_t now = time(NULL);

  strftime(buff, sizeof(buff)-1, "%y年%m月%d日 %H:%M", localtime(&now));
  snprintf(outBuff, sizeof(outBuff), "总金额: %d 元\n打印日期: %s \n", 100, buff);
  printf(outBuff);
  return 0;
}
