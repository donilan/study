#include <stdio.h>
#include <sys/stat.h>

int main(void) {
  struct stat st;
  if(stat("main.c", &st) == 0) {
    printf("success!\n");
    printf("st mode %d\n", S_ISREG(st.st_mode));
  }
  return 0;
}
